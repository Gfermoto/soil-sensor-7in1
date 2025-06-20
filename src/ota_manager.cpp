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

// Глобальные переменные для OTA 2.0
static const char* manifestUrlGlobal = nullptr;
static WiFiClient* clientPtr = nullptr;
static char statusBuf[64] = "Ожидание";

// Переменные для двухэтапного OTA (проверка -> установка)
static bool updateAvailable = false;
static String pendingUpdateUrl = "";
static String pendingUpdateSha256 = "";
static String pendingUpdateVersion = "";

const char* getOtaStatus() { return statusBuf; }

void setupOTA(const char* manifestUrl, WiFiClient& client)
{
    manifestUrlGlobal = manifestUrl;
    clientPtr = &client;
    strlcpy(statusBuf, "Готов", sizeof(statusBuf));
    
    // ИСПРАВЛЕНО: Защита от повреждения памяти - копируем URL в локальный буфер
    char urlBuffer[256];
    strlcpy(urlBuffer, manifestUrl, sizeof(urlBuffer));
    logSystem("[OTA] Initialized, manifest: %s", urlBuffer);
}

static bool verifySha256(const uint8_t* calcDigest, const char* expectedHex)
{
    char calcHex[65];
    for (int i = 0; i < 32; ++i)
        sprintf(&calcHex[i * 2], "%02x", calcDigest[i]);
    return strcasecmp(calcHex, expectedHex) == 0;
}

// Вспомогательная функция для инициализации загрузки
static bool initializeDownload(HTTPClient& http, const String& binUrl, int& contentLen)
{
    esp_task_wdt_reset();
    strcpy(statusBuf, "Подключение");
    
    // ИСПРАВЛЕНО: Защита от повреждения памяти - копируем URL в локальный буфер
    char urlBuffer[256];
    strlcpy(urlBuffer, binUrl.c_str(), sizeof(urlBuffer));
    logSystem("[OTA] Загрузка: %s", urlBuffer);

    http.begin(*clientPtr, binUrl);
    http.setTimeout(30000);
    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
    
    int code = http.GET();
    esp_task_wdt_reset();
    
    if (code != HTTP_CODE_OK)
    {
        snprintf(statusBuf, sizeof(statusBuf), "Ошибка HTTP %d", code);
        logError("[OTA] HTTP ошибка %d", code);
        return false;
    }

    contentLen = http.getSize();
    if (contentLen == -1)
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
        strcpy(statusBuf, "Нет места");
        logError("[OTA] Update.begin() failed");
        Update.printError(Serial);
        return false;
    }

    return true;
}

// Вспомогательная функция для загрузки данных
static bool downloadData(HTTPClient& http, int contentLen, mbedtls_sha256_context& shaCtx)
{
    strcpy(statusBuf, "Загрузка");
    
    WiFiClient* stream = http.getStreamPtr();
    uint8_t buf[512];
    size_t totalDownloaded = 0;
    unsigned long lastProgress = millis();
    unsigned long lastActivity = millis();
    const unsigned long TIMEOUT_MS = 30000;
    bool isChunked = (contentLen == UPDATE_SIZE_UNKNOWN);

    while (http.connected())
    {
        esp_task_wdt_reset();
        
        size_t avail = stream->available();
        if (avail > 0)
        {
            lastActivity = millis();
            size_t toRead = (avail > sizeof(buf)) ? sizeof(buf) : avail;
            int readBytes = stream->readBytes(buf, toRead);
            
            if (readBytes <= 0)
            {
                strcpy(statusBuf, "Ошибка чтения");
                logError("[OTA] Ошибка чтения данных");
                Update.abort();
                return false;
            }

            if (Update.write(buf, readBytes) != (size_t)readBytes)
            {
                strcpy(statusBuf, "Ошибка записи");
                logError("[OTA] Ошибка записи во flash");
                Update.printError(Serial);
                Update.abort();
                return false;
            }

            mbedtls_sha256_update_ret(&shaCtx, buf, readBytes);
            totalDownloaded += readBytes;

            // Прогресс каждые 3 секунды
            if (millis() - lastProgress > 3000)
            {
                if (isChunked)
                {
                    snprintf(statusBuf, sizeof(statusBuf), "Загружено %dКБ", (int)(totalDownloaded / 1024));
                }
                else
                {
                    int percent = (totalDownloaded * 100) / contentLen;
                    snprintf(statusBuf, sizeof(statusBuf), "Загружено %d%%", percent);
                }
                logSystem("[OTA] Загружено: %d байт", totalDownloaded);
                lastProgress = millis();
            }
        }
        else
        {
            if (millis() - lastActivity > TIMEOUT_MS)
            {
                strcpy(statusBuf, "Таймаут");
                logError("[OTA] Таймаут загрузки (нет данных %lu мс)", TIMEOUT_MS);
                Update.abort();
                return false;
            }
            
            if (isChunked && !http.connected())
            {
                logSystem("[OTA] Chunked transfer завершён, загружено %d байт", totalDownloaded);
                break;
            }
            
            esp_task_wdt_reset();
            delay(10);
        }
    }

    if (!isChunked && totalDownloaded != (size_t)contentLen)
    {
        snprintf(statusBuf, sizeof(statusBuf), "Неполная загрузка %d/%d", totalDownloaded, contentLen);
        logError("[OTA] Неполная загрузка: %d из %d байт", totalDownloaded, contentLen);
        Update.abort();
        return false;
    }

    return true;
}

// Основная функция загрузки и обновления (упрощенная)
static bool downloadAndUpdate(const String& binUrl, const char* expectedSha256)
{
    HTTPClient http;
    int contentLen;
    
    // Инициализация загрузки
    if (!initializeDownload(http, binUrl, contentLen))
    {
        http.end();
        return false;
    }
    
    // Инициализация SHA256
    mbedtls_sha256_context shaCtx;
    mbedtls_sha256_init(&shaCtx);
    mbedtls_sha256_starts_ret(&shaCtx, 0);
    
    // Загрузка данных
    bool downloadSuccess = downloadData(http, contentLen, shaCtx);
    http.end();
    
    if (!downloadSuccess)
    {
        mbedtls_sha256_free(&shaCtx);
        return false;
    }

    // Проверка SHA256
    strcpy(statusBuf, "Проверка");
    uint8_t digest[32];
    mbedtls_sha256_finish_ret(&shaCtx, digest);
    mbedtls_sha256_free(&shaCtx);

    if (!verifySha256(digest, expectedSha256))
    {
        strcpy(statusBuf, "Ошибка подписи");
        logError("[OTA] SHA256 не совпадает");
        Update.abort();
        return false;
    }

    // Финализация
    strcpy(statusBuf, "Завершение");
    if (!Update.end(true))
    {
        strcpy(statusBuf, "Ошибка установки");
        logError("[OTA] Ошибка финализации");
        Update.printError(Serial);
        return false;
    }

    if (!Update.isFinished())
    {
        strcpy(statusBuf, "Не завершено");
        logError("[OTA] Update не завершён");
        return false;
    }

    logSuccess("[OTA] Обновление завершено успешно, перезагрузка...");
    strcpy(statusBuf, "Успешно, перезагрузка");
    delay(2000);
    ESP.restart();
    return true;
}

// Принудительная проверка OTA (игнорирует таймер)
void triggerOtaCheck()
{
    static bool isChecking = false;
    
    if (isChecking) {
        logWarn("[OTA] Проверка уже выполняется, пропускаем");
        return;
    }
    
    isChecking = true;
    logSystem("[OTA] Принудительная проверка OTA запущена");
    handleOTA();
    isChecking = false;
}

// Принудительная установка найденного обновления
void triggerOtaInstall()
{
    if (!updateAvailable || pendingUpdateUrl.isEmpty())
    {
        logError("[OTA] Нет доступных обновлений для установки");
        strcpy(statusBuf, "Нет обновлений");
        return;
    }
    
    logSystem("[OTA] Принудительная установка обновления %s", pendingUpdateVersion.c_str());
    logSystem("[OTA] URL: %s", pendingUpdateUrl.c_str());
    logSystem("[OTA] SHA256: %.16s...", pendingUpdateSha256.c_str());
    
    bool result = downloadAndUpdate(pendingUpdateUrl, pendingUpdateSha256.c_str());
    if (!result)
    {
        logError("[OTA] Установка обновления не удалась");
        strcpy(statusBuf, "Ошибка установки");
        // Сбрасываем информацию об обновлении при ошибке
        updateAvailable = false;
        pendingUpdateUrl = "";
        pendingUpdateSha256 = "";
        pendingUpdateVersion = "";
    }
}

void handleOTA()
{
    // Сброс watchdog перед началом проверки
    esp_task_wdt_reset();
    
    if (!manifestUrlGlobal) 
    {
        logError("[OTA] manifestUrlGlobal не задан");
        return;
    }

    logSystem("[OTA] Начинаем проверку обновлений: %s", manifestUrlGlobal);
    strcpy(statusBuf, "Проверка обновлений");

    HTTPClient http;
    http.begin(*clientPtr, manifestUrlGlobal);
    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
    int code = http.GET();
    esp_task_wdt_reset();
    
    logSystem("[OTA] Ответ манифеста: HTTP %d", code);
    
    if (code != HTTP_CODE_OK)
    {
        snprintf(statusBuf, sizeof(statusBuf), "Ошибка манифеста %d", code);
        logError("[OTA] Ошибка загрузки манифеста: HTTP %d", code);
        http.end();
        return;
    }

    String manifestContent = http.getString();
    http.end();
    
    logSystem("[OTA] Манифест получен: %d символов", manifestContent.length());

    const size_t capacity = JSON_OBJECT_SIZE(3) + 150;
    StaticJsonDocument<capacity> doc;
    DeserializationError err = deserializeJson(doc, manifestContent);
    if (err)
    {
        strcpy(statusBuf, "Ошибка JSON");
        logError("[OTA] Ошибка парсинга JSON: %s", err.c_str());
        return;
    }

    const char* newVersion = doc["version"] | "";
    const char* binUrl = doc["url"] | "";
    const char* sha256 = doc["sha256"] | "";

    logSystem("[OTA] Версия в манифесте: '%s', текущая: '%s'", newVersion, JXCT_VERSION_STRING);
    logSystem("[OTA] URL: %s", binUrl);
    logSystem("[OTA] SHA256: %.16s...", sha256);

    if (strlen(newVersion) == 0 || strlen(binUrl) == 0 || strlen(sha256) != 64)
    {
        strcpy(statusBuf, "Неверный манифест");
        logError("[OTA] Некорректный манифест");
        return;
    }

    if (strcmp(newVersion, JXCT_VERSION_STRING) == 0)
    {
        strcpy(statusBuf, "Актуальная версия");
        updateAvailable = false;
        pendingUpdateUrl = "";
        pendingUpdateSha256 = "";
        pendingUpdateVersion = "";
        logSystem("[OTA] Версия актуальна, обновление не требуется");
        return;
    }

    // Сохраняем информацию об обновлении
    updateAvailable = true;
    pendingUpdateUrl = String(binUrl);
    pendingUpdateSha256 = String(sha256);
    pendingUpdateVersion = String(newVersion);
    
    snprintf(statusBuf, sizeof(statusBuf), "Доступно обновление: %s", newVersion);
    logSystem("[OTA] Найдено обновление %s -> %s, ожидаем подтверждения установки", JXCT_VERSION_STRING, newVersion);
} 