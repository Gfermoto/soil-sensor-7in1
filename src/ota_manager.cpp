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
    strcpy(statusBuf, "dl");

    HTTPClient http;
    http.begin(*clientPtr, binUrl);
    int code = http.GET();
    if (code != HTTP_CODE_OK)
    {
        snprintf(statusBuf, sizeof(statusBuf), "http %d", code);
        return false;
    }

    int contentLen = http.getSize();
    if (contentLen <= 0)
    {
        strcpy(statusBuf, "size 0");
        return false;
    }

    if (!Update.begin(contentLen)) // автоматически выберет следующую OTA партицию
    {
        strcpy(statusBuf, "no space");
        return false;
    }

    mbedtls_sha256_context shaCtx;
    mbedtls_sha256_init(&shaCtx);
    mbedtls_sha256_starts_ret(&shaCtx, 0); // 0 = SHA-256

    WiFiClient* stream = http.getStreamPtr();
    uint8_t buf[1024];
    int remaining = contentLen;
    unsigned long lastProgress = millis();

    while (http.connected() && remaining > 0)
    {
        size_t avail = stream->available();
        if (avail)
        {
            size_t toRead = avail > sizeof(buf) ? sizeof(buf) : avail;
            int readBytes = stream->readBytes(buf, toRead);
            if (readBytes <= 0)
            {
                strcpy(statusBuf, "read err");
                Update.abort();
                return false;
            }

            if (Update.write(buf, readBytes) != (size_t)readBytes)
            {
                strcpy(statusBuf, "write err");
                Update.abort();
                return false;
            }

            mbedtls_sha256_update_ret(&shaCtx, buf, readBytes);
            remaining -= readBytes;

            // простейший прогресс ~1 раз/5с
            if (millis() - lastProgress > 5000)
            {
                snprintf(statusBuf, sizeof(statusBuf), "down %d%%", 100 - (remaining * 100 / contentLen));
                lastProgress = millis();
            }
        }
        delay(1);
    }

    uint8_t digest[32];
    mbedtls_sha256_finish_ret(&shaCtx, digest);
    mbedtls_sha256_free(&shaCtx);

    if (!verifySha256(digest, expectedSha256))
    {
        strcpy(statusBuf, "sha mismatch");
        Update.abort();
        return false;
    }

    strcpy(statusBuf, "finishing");

    if (!Update.end())
    {
        strcpy(statusBuf, "upd end err");
        return false;
    }

    if (!Update.isFinished())
    {
        strcpy(statusBuf, "not finished");
        return false;
    }

    // Partition переключится автоматически; откат возможен, ожидаем подтверждения после перезагрузки

    strcpy(statusBuf, "reboot");
    delay(1000);
    ESP.restart();
    return true;
}

void handleOTA()
{
    if (millis() - lastCheckTs < 3600000UL) return; // 1 раз в час
    lastCheckTs = millis();

    if (!manifestUrlGlobal) return;

    strcpy(statusBuf, "chk");

    HTTPClient http;
    http.begin(*clientPtr, manifestUrlGlobal);
    int code = http.GET();
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
    lastCheckTs = 0; // сброс, чтобы handleOTA выполнилась сразу
} 