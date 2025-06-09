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
    if (!config.thingSpeakEnabled || !wifiConnected || !sensorData.valid) {
        return false;
    }
    unsigned long now = millis();
    if (now - lastTsPublish < config.thingspeakInterval * 1000UL) {
        Serial.println("[ThingSpeak] Публикация слишком часто, пропущено");
        return false;
    }
    lastTsPublish = now;
    unsigned long channelId = strtoul(config.thingSpeakChannelId, nullptr, 10);
    ThingSpeak.setField(1, format_temperature(sensorData.temperature).c_str());
    ThingSpeak.setField(2, format_moisture(sensorData.humidity).c_str());
    ThingSpeak.setField(3, format_ec(sensorData.ec).c_str());
    ThingSpeak.setField(4, format_ph(sensorData.ph).c_str());
    ThingSpeak.setField(5, format_npk(sensorData.nitrogen).c_str());
    ThingSpeak.setField(6, format_npk(sensorData.phosphorus).c_str());
    ThingSpeak.setField(7, format_npk(sensorData.potassium).c_str());
    ThingSpeak.setField(8, String(timeClient ? timeClient->getEpochTime() : 0));
    int res = ThingSpeak.writeFields(channelId, config.thingSpeakApiKey);
    if (res == 200) {
        Serial.println("[ThingSpeak] Данные успешно отправлены");
        thingSpeakLastPublish = String(millis());
        thingSpeakLastError = "";
        return true;
    } else if (res == -401) {
        Serial.println("[ThingSpeak] Превышен лимит публикаций (15 сек)");
        thingSpeakLastError = "Превышен лимит публикаций (15 сек)";
    } else {
        Serial.print("[ThingSpeak] Ошибка публикации, код: ");
        Serial.println(res);
        thingSpeakLastError = "Ошибка публикации ThingSpeak";
    }
    return false;
} 