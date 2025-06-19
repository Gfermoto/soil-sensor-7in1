#include "ota_manager.h"
#include <Update.h>
#include "logger.h"
#include "jxct_config_vars.h"
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <mbedtls/sha256.h>
#include <esp_ota_ops.h>
#include <strings.h>
#include "version.h"
#include <Arduino.h>
#include <esp_task_wdt.h>

static char statusBuf[64] = "idle";
static const char* manifestUrlGlobal = nullptr;
static WiFiClient* clientPtr = nullptr;
static unsigned long lastCheckTs = 0; // таймер проверки OTA

const char* getOtaStatus() { return statusBuf; }

void setupOTA(const char* manifestUrl, WiFiClient& client)
{
    manifestUrlGlobal = manifestUrl;
    clientPtr = &client;
    strlcpy(statusBuf, "ready", sizeof(statusBuf));
    logSystem("[OTA] Initialized, manifest: %s", manifestUrl);
}

static bool verifySha256(const uint8_t* calcDigest, const char* expectedHex)
{
    char calcHex[65];
    for (int i = 0; i < 32; ++i)
        sprintf(&calcHex[i * 2], "%02x", calcDigest[i]);
    return strcasecmp(calcHex, expectedHex) == 0;
}

static bool downloadAndUpdate(const String& binUrl, const char* expectedSha256)
{
    esp_task_wdt_reset();
    strcpy(statusBuf, "connecting");
    logSystem("[OTA] Загрузка: %s", binUrl.c_str());

    HTTPClient http;
    http.begin(*clientPtr, binUrl);
    http.setTimeout(30000); // 30 сек таймаут
    
    int code = http.GET();
    esp_task_wdt_reset();
    
    if (code != HTTP_CODE_OK)
    {
        snprintf(statusBuf, sizeof(statusBuf), "http %d", code);
        logError("[OTA] HTTP ошибка %d", code);
        http.end();
        return false;
    }

    int contentLen = http.getSize();
    bool isChunked = (contentLen == -1);
    
    if (isChunked)
    {
        logSystem("[OTA] Chunked transfer, размер неизвестен");
        contentLen = UPDATE_SIZE_UNKNOWN;
    }
    else
    {
        logSystem("[OTA] Размер файла: %d байт", contentLen);
    }

    if (!Update.begin(contentLen))
    {
        strcpy(statusBuf, "no space");
        logError("[OTA] Update.begin() failed");
        Update.printError(Serial);
        http.end();
        return false;
    }

    strcpy(statusBuf, "downloading");
    
    mbedtls_sha256_context shaCtx;
    mbedtls_sha256_init(&shaCtx);
    mbedtls_sha256_starts_ret(&shaCtx, 0);

    WiFiClient* stream = http.getStreamPtr();
    uint8_t buf[1024];
    size_t totalDownloaded = 0;
    unsigned long lastProgress = millis();
    unsigned long lastActivity = millis();
    const unsigned long TIMEOUT_MS = 30000; // 30 сек без данных = таймаут

    while (http.connected())
    {
        size_t avail = stream->available();
        if (avail > 0)
        {
            lastActivity = millis();
            size_t toRead = (avail > sizeof(buf)) ? sizeof(buf) : avail;
            int readBytes = stream->readBytes(buf, toRead);
            
            if (readBytes <= 0)
            {
                strcpy(statusBuf, "read error");
                logError("[OTA] Ошибка чтения данных");
                Update.abort();
                http.end();
                return false;
            }

            if (Update.write(buf, readBytes) != (size_t)readBytes)
            {
                strcpy(statusBuf, "write error");
                logError("[OTA] Ошибка записи во flash");
                Update.printError(Serial);
                Update.abort();
                http.end();
                return false;
            }

            mbedtls_sha256_update_ret(&shaCtx, buf, readBytes);
            totalDownloaded += readBytes;

            // Прогресс каждые 3 секунды
            if (millis() - lastProgress > 3000)
            {
                if (isChunked)
                {
                    snprintf(statusBuf, sizeof(statusBuf), "dl %dKB", (int)(totalDownloaded / 1024));
                }
                else
                {
                    int percent = (totalDownloaded * 100) / contentLen;
                    snprintf(statusBuf, sizeof(statusBuf), "dl %d%%", percent);
                }
                logSystem("[OTA] Загружено: %d байт", totalDownloaded);
                lastProgress = millis();
            }
            
            esp_task_wdt_reset();
        }
        else
        {
            // Проверяем таймаут бездействия
            if (millis() - lastActivity > TIMEOUT_MS)
            {
                strcpy(statusBuf, "timeout");
                logError("[OTA] Таймаут загрузки (нет данных %lu мс)", TIMEOUT_MS);
                Update.abort();
                http.end();
                return false;
            }
            
            // Для chunked проверяем завершение
            if (isChunked && !http.connected())
            {
                logSystem("[OTA] Chunked transfer завершён, загружено %d байт", totalDownloaded);
                break;
            }
            
            esp_task_wdt_reset();
            delay(10);
        }
    }

    http.end();
    
    if (!isChunked && totalDownloaded != (size_t)contentLen)
    {
        snprintf(statusBuf, sizeof(statusBuf), "incomplete %d/%d", totalDownloaded, contentLen);
        logError("[OTA] Неполная загрузка: %d из %d байт", totalDownloaded, contentLen);
        Update.abort();
        return false;
    }

    strcpy(statusBuf, "verifying");
    
    uint8_t digest[32];
    mbedtls_sha256_finish_ret(&shaCtx, digest);
    mbedtls_sha256_free(&shaCtx);

    if (!verifySha256(digest, expectedSha256))
    {
        strcpy(statusBuf, "sha mismatch");
        logError("[OTA] SHA256 не совпадает");
        Update.abort();
        return false;
    }

    strcpy(statusBuf, "finalizing");
    
    if (!Update.end(true)) // true = устанавливать как boot partition
    {
        strcpy(statusBuf, "finalize error");
        logError("[OTA] Ошибка финализации");
        Update.printError(Serial);
        return false;
    }

    if (!Update.isFinished())
    {
        strcpy(statusBuf, "not finished");
        logError("[OTA] Update не завершён");
        return false;
    }

    logSuccess("[OTA] Обновление завершено успешно, перезагрузка...");
    strcpy(statusBuf, "success, rebooting");
    delay(2000);
    ESP.restart();
    return true;
}

void handleOTA()
{
    // Сброс watchdog перед началом проверки
    esp_task_wdt_reset();
    if (millis() - lastCheckTs < 3600000UL) return; // 1 раз в час
    lastCheckTs = millis();

    if (!manifestUrlGlobal) return;

    strcpy(statusBuf, "chk");

    HTTPClient http;
    http.begin(*clientPtr, manifestUrlGlobal);
    int code = http.GET();
    esp_task_wdt_reset(); // 🔄 после блокирующего http.GET
    if (code != HTTP_CODE_OK)
    {
        snprintf(statusBuf, sizeof(statusBuf), "mf %d", code);
        return;
    }

    const size_t capacity = JSON_OBJECT_SIZE(3) + 200;
    StaticJsonDocument<capacity> doc;
    DeserializationError err = deserializeJson(doc, http.getString());
    if (err)
    {
        strcpy(statusBuf, "json err");
        return;
    }

    const char* newVersion = doc["version"] | "";
    const char* binUrl = doc["url"] | "";
    const char* sha256 = doc["sha256"] | "";

    if (strlen(newVersion) == 0 || strlen(binUrl) == 0 || strlen(sha256) != 64)
    {
        strcpy(statusBuf, "manifest bad");
        return;
    }

    if (strcmp(newVersion, JXCT_VERSION_STRING) == 0)
    {
        strcpy(statusBuf, "up-to-date");
        return;
    }

    // Можно добавить сравнение >, но пока достаточно !=

    downloadAndUpdate(String(binUrl), sha256);
}

void triggerOtaCheck()
{
    // Смещаем таймер так, будто последний чек был час назад
    // (handleOTA запускается, если прошло >=1 часа).
    lastCheckTs = (millis() > 3600000UL) ? millis() - 3600000UL : 0;
} 