#include <WiFiClient.h>
#include "thingspeak_client.h"
#include "modbus_sensor.h"
#include "wifi_manager.h"
#include <ThingSpeak.h>
#include "jxct_device_info.h"
#include "jxct_config_vars.h"
#include "jxct_format_utils.h"
#include "logger.h"
#include <NTPClient.h>
#include <ctype.h>
extern NTPClient* timeClient;

// URL для отправки данных в ThingSpeak
const char* THINGSPEAK_API_URL = "https://api.thingspeak.com/update";

static unsigned long lastTsPublish = 0;

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
    while (len > 0 && isspace((unsigned char)s[len - 1]))
        s[--len] = '\0';
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

    // Проверяем корректность ID и API ключа до отправки
    if (channelId == 0 || strlen(apiKeyBuf) < 16)
    {
        logError("ThingSpeak: некорректный Channel ID или API ключ");
        strlcpy(thingSpeakLastErrorBuffer, "Неверный Channel ID или API Key", sizeof(thingSpeakLastErrorBuffer));
        return false;
    }

    // ✅ Используем прямую передачу данных без String объектов
    // Отправка (убираем timestamp для избежания ошибок)
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
        return true;
    }
    else if (res == -301)
    {
        logWarn("ThingSpeak: таймаут (-301), повторим позже");
        strlcpy(thingSpeakLastErrorBuffer, "Timeout -301", sizeof(thingSpeakLastErrorBuffer));
        return false; // do not treat as success
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
        strlcpy(thingSpeakLastErrorBuffer, "HTTP 400 – неверный запрос (API/Channel)", sizeof(thingSpeakLastErrorBuffer));
    }
    else
    {
        logError("ThingSpeak: ошибка %d", res);
        // Сохраняем числовой код для быстрой диагностики в UI/логах
        snprintf(thingSpeakLastErrorBuffer, sizeof(thingSpeakLastErrorBuffer), "Ошибка %d", res);
    }
    return false;
}