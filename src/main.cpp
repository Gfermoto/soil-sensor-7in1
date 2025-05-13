#include <WiFiClient.h>
#include <Arduino.h>
#include "wifi_manager.h"
#include "modbus_sensor.h"
#include "mqtt_client.h"
#include "thingspeak_client.h"
#include "jxct_config_vars.h"
#include <WiFiUdp.h>
#include <NTPClient.h>

// Переменные для отслеживания времени
unsigned long lastSensorReadTime = 0;
unsigned long lastDataPublishTime = 0;
unsigned long lastNtpUpdate = 0;

extern void startFakeSensorTask();
extern void startRealSensorTask();

WiFiUDP ntpUDP;
NTPClient* timeClient = nullptr;

void resetButtonTask(void *pvParameters) {
    for (;;) {
        if (checkResetButton()) {
            resetConfig();
            setLedFastBlink();
            delay(2000);
            ESP.restart();
        }
        vTaskDelay(50 / portTICK_PERIOD_MS);
    }
}

void setup() {
    Serial.begin(115200);
    Serial.println("\n\nJXCT 7in1 Soil Sensor");
    Serial.println("=====================");
    Serial.println("[setup] Запуск инициализации...");
    
    // Настройка пина кнопки
    pinMode(BOOT_BUTTON, INPUT_PULLUP);
    Serial.print("[setup] BOOT_BUTTON пин: "); Serial.println(BOOT_BUTTON);

    // Запуск задачи FreeRTOS для кнопки сброса
    xTaskCreate(resetButtonTask, "ResetButton", 2048, NULL, 1, NULL);
    Serial.println("[setup] Задача resetButtonTask запущена");
    
    // Инициализация WiFi (и загрузка конфига)
    setupWiFi();
    Serial.println("[setup] setupWiFi завершён");
    
    // Инициализация Modbus
    setupModbus();
    Serial.println("[setup] setupModbus завершён");
    
    // Инициализация MQTT
    setupMQTT();
    Serial.println("[setup] setupMQTT завершён");
    
    // Инициализация ThingSpeak
    setupThingSpeak(espClient);
    Serial.println("[setup] setupThingSpeak завершён");
    
    // Инициализация NTP
    if (timeClient) delete timeClient;
    timeClient = new NTPClient(ntpUDP, config.ntpServer, 3 * 3600, config.ntpUpdateInterval);
    timeClient->begin();
    
    // Запуск задачи датчика
    if (config.useRealSensor) {
        startRealSensorTask();
        Serial.println("[setup] Запущена задача реального датчика");
    } else {
        startFakeSensorTask();
        Serial.println("[setup] Запущена задача фейкового датчика");
    }
    Serial.println("[setup] Инициализация завершена");
}

void loop() {
    handleWiFi();
    handleMQTT();
    
    if (millis() - lastSensorReadTime >= SENSOR_READ_INTERVAL) {
        lastSensorReadTime = millis();
        Serial.println("[loop] Опрос датчика...");
        if (readSensorData()) {
            Serial.println("[loop] Данные с датчика успешно прочитаны");
            Serial.print("[loop] Температура: "); Serial.println(sensorData.temperature);
            Serial.print("[loop] Влажность: "); Serial.println(sensorData.humidity);
            Serial.print("[loop] EC: "); Serial.println(sensorData.ec);
            Serial.print("[loop] pH: "); Serial.println(sensorData.ph);
            Serial.print("[loop] Азот: "); Serial.println(sensorData.nitrogen);
            Serial.print("[loop] Фосфор: "); Serial.println(sensorData.phosphorus);
            Serial.print("[loop] Калий: "); Serial.println(sensorData.potassium);
        } else {
            Serial.println("[loop] Ошибка чтения данных с датчика");
        }
    }
    if (millis() - lastDataPublishTime >= DATA_PUBLISH_INTERVAL) {
        lastDataPublishTime = millis();
        if (sensorData.valid) {
            Serial.println("[loop] Публикация данных в MQTT и ThingSpeak...");
            publishSensorData();
            sendDataToThingSpeak();
        } else {
            Serial.println("[loop] Данные невалидны, публикация пропущена");
        }
    }
    if (timeClient && millis() - lastNtpUpdate > config.ntpUpdateInterval) {
        timeClient->setPoolServerName(config.ntpServer);
        timeClient->setUpdateInterval(config.ntpUpdateInterval);
        timeClient->update();
        lastNtpUpdate = millis();
    }
    delay(10);
} 