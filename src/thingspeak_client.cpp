#include <WiFiClient.h>
#include "thingspeak_client.h"
#include "config.h"
#include "modbus_sensor.h"
#include "wifi_manager.h"
#include <ThingSpeak.h>

// URL для отправки данных в ThingSpeak
const char* THINGSPEAK_API_URL = "https://api.thingspeak.com/update";

static unsigned long lastTsPublish = 0;

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
    ThingSpeak.setField(1, sensorData.temperature);
    ThingSpeak.setField(2, sensorData.humidity);
    ThingSpeak.setField(3, sensorData.ec);
    ThingSpeak.setField(4, sensorData.ph);
    ThingSpeak.setField(5, sensorData.nitrogen);
    ThingSpeak.setField(6, sensorData.phosphorus);
    ThingSpeak.setField(7, sensorData.potassium);
    int res = ThingSpeak.writeFields(channelId, config.thingSpeakApiKey);
    if (res == 200) {
        Serial.println("[ThingSpeak] Данные успешно отправлены");
        return true;
    } else if (res == -401) {
        Serial.println("[ThingSpeak] Превышен лимит публикаций (15 сек)");
    } else {
        Serial.print("[ThingSpeak] Ошибка публикации, код: ");
        Serial.println(res);
    }
    return false;
} 