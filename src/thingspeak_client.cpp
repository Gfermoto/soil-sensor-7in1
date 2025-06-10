#include <WiFiClient.h>
#include "thingspeak_client.h"
#include "modbus_sensor.h"
#include "wifi_manager.h"
#include <ThingSpeak.h>
#include "jxct_device_info.h"
#include "jxct_config_vars.h"
#include "jxct_format_utils.h"
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
    Serial.println("\n[ThingSpeak] === ОТЛАДКА ПУБЛИКАЦИИ ===");
    Serial.printf("[ThingSpeak] Enabled: %s\n", config.thingSpeakEnabled ? "ДА" : "НЕТ");
    Serial.printf("[ThingSpeak] WiFi connected: %s\n", wifiConnected ? "ДА" : "НЕТ");
    Serial.printf("[ThingSpeak] Sensor valid: %s\n", sensorData.valid ? "ДА" : "НЕТ");
    Serial.printf("[ThingSpeak] Channel ID: %s\n", config.thingSpeakChannelId);
    Serial.printf("[ThingSpeak] API Key: %s\n", strlen(config.thingSpeakApiKey) > 0 ? "ЗАДАН" : "ПУСТОЙ");
    
    if (!config.thingSpeakEnabled) {
        Serial.println("[ThingSpeak] ❌ ОТКЛЮЧЕН в настройках!");
        return false;
    }
    if (!wifiConnected) {
        Serial.println("[ThingSpeak] ❌ WiFi не подключен!");
        return false;
    }
    if (!sensorData.valid) {
        Serial.println("[ThingSpeak] ❌ Данные датчика невалидны!");
        return false;
    }
    unsigned long now = millis();
    if (now - lastTsPublish < config.thingspeakInterval * 1000UL) {
        Serial.println("[ThingSpeak] Публикация слишком часто, пропущено");
        return false;
    }
    lastTsPublish = now;
    unsigned long channelId = strtoul(config.thingSpeakChannelId, nullptr, 10);
    
    Serial.println("[ThingSpeak] 📊 Подготовка данных для отправки:");
    String temp = String(format_temperature(sensorData.temperature).c_str());
    String hum = String(format_moisture(sensorData.humidity).c_str());
    String ec = String(format_ec(sensorData.ec).c_str());
    String ph = String(format_ph(sensorData.ph).c_str());
    String n = String(format_npk(sensorData.nitrogen).c_str());
    String p = String(format_npk(sensorData.phosphorus).c_str());
    String k = String(format_npk(sensorData.potassium).c_str());
    String timestamp = String(timeClient ? timeClient->getEpochTime() : 0);
    
    Serial.printf("[ThingSpeak] Field 1 (Temp): %s\n", temp.c_str());
    Serial.printf("[ThingSpeak] Field 2 (Hum): %s\n", hum.c_str());
    Serial.printf("[ThingSpeak] Field 3 (EC): %s\n", ec.c_str());
    Serial.printf("[ThingSpeak] Field 4 (PH): %s\n", ph.c_str());
    Serial.printf("[ThingSpeak] Field 5 (N): %s\n", n.c_str());
    Serial.printf("[ThingSpeak] Field 6 (P): %s\n", p.c_str());
    Serial.printf("[ThingSpeak] Field 7 (K): %s\n", k.c_str());
    Serial.printf("[ThingSpeak] Field 8 (Time): %s\n", timestamp.c_str());
    Serial.printf("[ThingSpeak] Channel ID: %lu\n", channelId);
    
    ThingSpeak.setField(1, temp.c_str());
    ThingSpeak.setField(2, hum.c_str());
    ThingSpeak.setField(3, ec.c_str());
    ThingSpeak.setField(4, ph.c_str());
    ThingSpeak.setField(5, n.c_str());
    ThingSpeak.setField(6, p.c_str());
    ThingSpeak.setField(7, k.c_str());
    ThingSpeak.setField(8, timestamp);
    Serial.println("[ThingSpeak] 🚀 Отправка данных...");
    int res = ThingSpeak.writeFields(channelId, config.thingSpeakApiKey);
    
    Serial.printf("[ThingSpeak] Код ответа: %d\n", res);
    if (res == 200) {
        Serial.println("[ThingSpeak] ✅ Данные успешно отправлены!");
        thingSpeakLastPublish = String(millis());
        thingSpeakLastError = "";
        return true;
    } else if (res == -401) {
        Serial.println("[ThingSpeak] ❌ Превышен лимит публикаций (15 сек)");
        thingSpeakLastError = "Превышен лимит публикаций (15 сек)";
    } else if (res == -302) {
        Serial.println("[ThingSpeak] ❌ Неверный API ключ");
        thingSpeakLastError = "Неверный API ключ";
    } else if (res == -304) {
        Serial.println("[ThingSpeak] ❌ Неверный Channel ID");
        thingSpeakLastError = "Неверный Channel ID";
    } else if (res == 0) {
        Serial.println("[ThingSpeak] ❌ Ошибка подключения к серверу");
        thingSpeakLastError = "Ошибка подключения";
    } else {
        Serial.printf("[ThingSpeak] ❌ Неизвестная ошибка, код: %d\n", res);
        thingSpeakLastError = "Ошибка публикации ThingSpeak";
    }
    return false;
} 