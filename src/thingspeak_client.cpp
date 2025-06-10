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
extern NTPClient* timeClient;

// URL для отправки данных в ThingSpeak
const char* THINGSPEAK_API_URL = "https://api.thingspeak.com/update";

static unsigned long lastTsPublish = 0;
String thingSpeakLastPublish = "";
String thingSpeakLastError = "";

void setupThingSpeak(WiFiClient& client) {
    ThingSpeak.begin(client);
}

bool sendDataToThingSpeak() {
    // Проверки
    if (!config.thingSpeakEnabled) return false;
    if (!wifiConnected) return false;
    if (!sensorData.valid) return false;
    
    unsigned long now = millis();
    if (now - lastTsPublish < config.thingspeakInterval * 1000UL) {
        return false; // Слишком часто
    }
    lastTsPublish = now;
    
    unsigned long channelId = strtoul(config.thingSpeakChannelId, nullptr, 10);
    
    // Подготовка данных
    String temp = String(format_temperature(sensorData.temperature).c_str());
    String hum = String(format_moisture(sensorData.humidity).c_str());
    String ec = String(format_ec(sensorData.ec).c_str());
    String ph = String(format_ph(sensorData.ph).c_str());
    String n = String(format_npk(sensorData.nitrogen).c_str());
    String p = String(format_npk(sensorData.phosphorus).c_str());
    String k = String(format_npk(sensorData.potassium).c_str());
    // Отправка (убираем timestamp для избежания ошибок)
    ThingSpeak.setField(1, temp.c_str());
    ThingSpeak.setField(2, hum.c_str());
    ThingSpeak.setField(3, ec.c_str());
    ThingSpeak.setField(4, ph.c_str());
    ThingSpeak.setField(5, n.c_str());
    ThingSpeak.setField(6, p.c_str());
    ThingSpeak.setField(7, k.c_str());
    
    logData("Отправка в ThingSpeak: T=%.1f°C, H=%.1f%%, PH=%.2f", 
           sensorData.temperature, sensorData.humidity, sensorData.ph);
    
    int res = ThingSpeak.writeFields(channelId, config.thingSpeakApiKey);
    
    if (res == 200) {
        logSuccess("ThingSpeak: данные отправлены");
        thingSpeakLastPublish = String(millis());
        thingSpeakLastError = "";
        return true;
    } else if (res == -301) {
        // Ошибка -301 часто возникает при таймауте, но данные могут быть отправлены
        logWarn("ThingSpeak: таймаут ответа (данные могли быть отправлены)");
        thingSpeakLastPublish = String(millis());
        thingSpeakLastError = "Таймаут ответа (возможно успешно)";
        return true; // Считаем успешным для избежания повторных отправок
    } else if (res == -401) {
        logDebug("ThingSpeak: превышен лимит публикаций");
        thingSpeakLastError = "Превышен лимит публикаций (15 сек)";
    } else if (res == -302) {
        logError("ThingSpeak: неверный API ключ");
        thingSpeakLastError = "Неверный API ключ";
    } else if (res == -304) {
        logError("ThingSpeak: неверный Channel ID");
        thingSpeakLastError = "Неверный Channel ID";
    } else if (res == 0) {
        logError("ThingSpeak: ошибка подключения");
        thingSpeakLastError = "Ошибка подключения";
    } else {
        logError("ThingSpeak: ошибка %d", res);
        thingSpeakLastError = "Ошибка публикации ThingSpeak";
    }
    return false;
} 