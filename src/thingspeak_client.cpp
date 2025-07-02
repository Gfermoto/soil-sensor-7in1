#include "thingspeak_client.h"
#include <NTPClient.h>
#include <ThingSpeak.h>
#include <WiFiClient.h>
#include <ctype.h>
#include "jxct_config_vars.h"
#include "jxct_device_info.h"
#include "jxct_format_utils.h"
#include "logger.h"
#include "modbus_sensor.h"
#include "wifi_manager.h"
extern NTPClient* timeClient;

// URL для отправки данных в ThingSpeak
const char* THINGSPEAK_API_URL = "https://api.thingspeak.com/update";

static unsigned long lastTsPublish = 0;
static int consecutiveFailCount = 0;  // счётчик подряд неудач

// Утилита для обрезки пробелов в начале/конце строки C
static void trim(char* s)
{
    if (!s) return;
    // Trim leading
    char* p = s;
    while (*p && isspace((unsigned char)*p)) ++p;
    if (p != s) memmove(s, p, strlen(p) + 1);

    // Trim trailing
    size_t len = strlen(s);
    while (len > 0 && isspace((unsigned char)s[len - 1])) s[--len] = '\0';
}

// ✅ Заменяем String на статические буферы
static char thingSpeakLastPublishBuffer[32] = "0";
static char thingSpeakLastErrorBuffer[64] = "";

// Геттеры для совместимости с внешним кодом
const char* getThingSpeakLastPublish()
{
    return thingSpeakLastPublishBuffer;
}
const char* getThingSpeakLastError()
{
    return thingSpeakLastErrorBuffer;
}

void setupThingSpeak(WiFiClient& client)
{
    ThingSpeak.begin(client);
}

bool sendDataToThingSpeak()
{
    // Проверки
    if (!config.flags.thingSpeakEnabled) return false;
    if (!wifiConnected) return false;
    if (!sensorData.valid) return false;

    unsigned long now = millis();
    if (now - lastTsPublish < config.thingSpeakInterval)  // too frequent
        return false;

    char apiKeyBuf[25];
    char channelBuf[16];
    strlcpy(apiKeyBuf, config.thingSpeakApiKey, sizeof(apiKeyBuf));
    strlcpy(channelBuf, config.thingSpeakChannelId, sizeof(channelBuf));
    trim(apiKeyBuf);
    trim(channelBuf);

    unsigned long channelId = strtoul(channelBuf, nullptr, 10);

    // Проверяем корректность ID и API ключа - если неверные, молча пропускаем
    if (channelId == 0 || strlen(apiKeyBuf) < 16)
    {
        // Не логируем ошибку каждый раз, просто пропускаем отправку
        if (strlen(thingSpeakLastErrorBuffer) == 0)  // логируем только первый раз
        {
            logWarn("ThingSpeak: настройки не заданы (Channel ID: %s, API Key: %d символов)", channelBuf,
                    strlen(apiKeyBuf));
            strlcpy(thingSpeakLastErrorBuffer, "Настройки не заданы", sizeof(thingSpeakLastErrorBuffer));
        }
        return false;
    }

    // Отправка данных
    ThingSpeak.setField(1, format_temperature(sensorData.temperature).c_str());
    ThingSpeak.setField(2, format_moisture(sensorData.humidity).c_str());
    ThingSpeak.setField(3, format_ec(sensorData.ec).c_str());
    ThingSpeak.setField(4, format_ph(sensorData.ph).c_str());
    ThingSpeak.setField(5, format_npk(sensorData.nitrogen).c_str());
    ThingSpeak.setField(6, format_npk(sensorData.phosphorus).c_str());
    ThingSpeak.setField(7, format_npk(sensorData.potassium).c_str());

    logData("Отправка в ThingSpeak: T=%.1f°C, H=%.1f%%, PH=%.2f", sensorData.temperature, sensorData.humidity,
            sensorData.ph);

    int res = ThingSpeak.writeFields(channelId, apiKeyBuf);

    if (res == 200)
    {
        logSuccess("ThingSpeak: данные отправлены");
        lastTsPublish = millis();
        snprintf(thingSpeakLastPublishBuffer, sizeof(thingSpeakLastPublishBuffer), "%lu", lastTsPublish);
        thingSpeakLastErrorBuffer[0] = '\0';  // Очистка ошибки
        consecutiveFailCount = 0;             // обнуляем при успехе
        return true;
    }
    else if (res == -301)
    {
        logWarn("ThingSpeak: таймаут (-301), повторим позже");
        strlcpy(thingSpeakLastErrorBuffer, "Timeout -301", sizeof(thingSpeakLastErrorBuffer));
    }
    else if (res == -401)
    {
        logDebug("ThingSpeak: превышен лимит публикаций");
        strlcpy(thingSpeakLastErrorBuffer, "Превышен лимит публикаций (15 сек)", sizeof(thingSpeakLastErrorBuffer));
    }
    else if (res == -302)
    {
        logError("ThingSpeak: неверный API ключ");
        strlcpy(thingSpeakLastErrorBuffer, "Неверный API ключ", sizeof(thingSpeakLastErrorBuffer));
    }
    else if (res == -304)
    {
        logError("ThingSpeak: неверный Channel ID");
        strlcpy(thingSpeakLastErrorBuffer, "Неверный Channel ID", sizeof(thingSpeakLastErrorBuffer));
    }
    else if (res == 0)
    {
        logError("ThingSpeak: ошибка подключения");
        strlcpy(thingSpeakLastErrorBuffer, "Ошибка подключения", sizeof(thingSpeakLastErrorBuffer));
    }
    else if (res == 400)
    {
        logError("ThingSpeak: HTTP 400 – неверный запрос (проверьте API Key/Channel)");
        strlcpy(thingSpeakLastErrorBuffer, "HTTP 400 – неверный запрос (API/Channel)",
                sizeof(thingSpeakLastErrorBuffer));
    }
    else
    {
        logError("ThingSpeak: ошибка %d", res);
        snprintf(thingSpeakLastErrorBuffer, sizeof(thingSpeakLastErrorBuffer), "Ошибка %d", res);
    }

    consecutiveFailCount++;

    // Если слишком много ошибок подряд, временно отключаем на 1 час
    if (consecutiveFailCount >= 10)
    {
        logWarn("ThingSpeak: %d ошибок подряд, отключаем на 1 час", consecutiveFailCount);
        lastTsPublish = millis();  // устанавливаем время последней попытки
        consecutiveFailCount = 0;  // сбрасываем счётчик
        strlcpy(thingSpeakLastErrorBuffer, "Отключён на 1 час (много ошибок)", sizeof(thingSpeakLastErrorBuffer));
    }

    return false;
}
