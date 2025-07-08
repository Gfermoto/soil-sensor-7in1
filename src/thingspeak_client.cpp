#include "thingspeak_client.h"
#include <NTPClient.h>
#include <ThingSpeak.h>
#include <WiFiClient.h>
#include <array>
#include <cctype>
#include "jxct_config_vars.h"
#include "jxct_device_info.h"
#include "jxct_format_utils.h"
#include "logger.h"
#include "modbus_sensor.h"
#include "wifi_manager.h"
extern NTPClient* timeClient;

// URL для отправки данных в ThingSpeak
const char* THINGSPEAK_API_URL = "https://api.thingspeak.com/update";

namespace
{
unsigned long lastTsPublish = 0;
int consecutiveFailCount = 0;  // счётчик подряд неудач

// Утилита для обрезки пробелов в начале/конце строки C
void trim(char* str)
{
    if (str == nullptr)
    {
        return;
    }
    // Trim leading
    char* ptr = str;
    while (*ptr && isspace((unsigned char)*ptr))
    {
        ++ptr;
    }
    if (ptr != str)
    {
        memmove(str, ptr, strlen(ptr) + 1);
    }

    // Trim trailing
    size_t len = strlen(str);
    while (len > 0 && isspace((unsigned char)str[len - 1]))
    {
        str[--len] = '\0';
    }
}

// ✅ Заменяем String на статические буферы
std::array<char, 32> thingSpeakLastPublishBuffer = {"0"};
std::array<char, 64> thingSpeakLastErrorBuffer = {""};
}  // namespace

// Геттеры для совместимости с внешним кодом
const char* getThingSpeakLastPublish()
{
    return thingSpeakLastPublishBuffer.data();
}
const char* getThingSpeakLastError()
{
    return thingSpeakLastErrorBuffer.data();
}

void setupThingSpeak(WiFiClient& client)
{
    ThingSpeak.begin(client);
}

bool sendDataToThingSpeak()
{
    // Проверки
    if (!config.flags.thingSpeakEnabled)
    {
        return false;
    }
    if (!wifiConnected)
    {
        return false;
    }
    if (!sensorData.valid)
    {
        return false;
    }

    const unsigned long now = millis();
    if (now - lastTsPublish < config.thingSpeakInterval)
    {  // too frequent
        return false;
    }

    std::array<char, 25> apiKeyBuf;
    std::array<char, 16> channelBuf;
    strlcpy(apiKeyBuf.data(), config.thingSpeakApiKey, apiKeyBuf.size());
    strlcpy(channelBuf.data(), config.thingSpeakChannelId, channelBuf.size());
    trim(apiKeyBuf.data());
    trim(channelBuf.data());

    const unsigned long channelId = strtoul(channelBuf.data(), nullptr, 10);

    // Проверяем корректность ID и API ключа - если неверные, молча пропускаем
    if (channelId == 0 || strlen(apiKeyBuf.data()) < 16)
    {
        // Не логируем ошибку каждый раз, просто пропускаем отправку
        if (strlen(thingSpeakLastErrorBuffer.data()) == 0)  // логируем только первый раз
        {
            logWarnSafe("\1", channelBuf.data(), strlen(apiKeyBuf.data()));
            strlcpy(thingSpeakLastErrorBuffer.data(), "Настройки не заданы", thingSpeakLastErrorBuffer.size());
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

    logDataSafe("\1", sensorData.temperature, sensorData.humidity, sensorData.ph);

    int res = ThingSpeak.writeFields(channelId, apiKeyBuf.data());

    if (res == 200)
    {
        logSuccess("ThingSpeak: данные отправлены");
        lastTsPublish = millis();
        snprintf(thingSpeakLastPublishBuffer.data(), thingSpeakLastPublishBuffer.size(), "%lu", lastTsPublish);
        thingSpeakLastErrorBuffer[0] = '\0';  // Очистка ошибки
        consecutiveFailCount = 0;             // обнуляем при успехе
        return true;
    }
    if (res == -301)
    {
        logWarn("ThingSpeak: таймаут (-301), повторим позже");
        strlcpy(thingSpeakLastErrorBuffer.data(), "Timeout -301", thingSpeakLastErrorBuffer.size());
    }
    else if (res == -401)
    {
        logDebug("ThingSpeak: превышен лимит публикаций");
        strlcpy(thingSpeakLastErrorBuffer.data(), "Превышен лимит публикаций (15 сек)",
                thingSpeakLastErrorBuffer.size());
    }
    else if (res == -302)
    {
        logError("ThingSpeak: неверный API ключ");
        strlcpy(thingSpeakLastErrorBuffer.data(), "Неверный API ключ", thingSpeakLastErrorBuffer.size());
    }
    else if (res == -304)
    {
        logError("ThingSpeak: неверный Channel ID");
        strlcpy(thingSpeakLastErrorBuffer.data(), "Неверный Channel ID", thingSpeakLastErrorBuffer.size());
    }
    else if (res == 0)
    {
        logError("ThingSpeak: ошибка подключения");
        strlcpy(thingSpeakLastErrorBuffer.data(), "Ошибка подключения", thingSpeakLastErrorBuffer.size());
    }
    else if (res == 400)
    {
        logError("ThingSpeak: HTTP 400 – неверный запрос (проверьте API Key/Channel)");
        strlcpy(thingSpeakLastErrorBuffer.data(), "HTTP 400 – неверный запрос (API/Channel)",
                thingSpeakLastErrorBuffer.size());
    }
    else
    {
        logErrorSafe("\1", res);
        snprintf(thingSpeakLastErrorBuffer.data(), thingSpeakLastErrorBuffer.size(), "Ошибка %d", res);
    }

    consecutiveFailCount++;

    // Если слишком много ошибок подряд, временно отключаем на 1 час
    if (consecutiveFailCount >= 10)
    {
        logWarnSafe("\1", consecutiveFailCount);
        lastTsPublish = millis();  // устанавливаем время последней попытки
        consecutiveFailCount = 0;  // сбрасываем счётчик
        strlcpy(thingSpeakLastErrorBuffer.data(), "Отключён на 1 час (много ошибок)", thingSpeakLastErrorBuffer.size());
    }

    return false;
}
