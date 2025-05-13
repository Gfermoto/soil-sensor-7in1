#include <WiFiClient.h>
#include <Arduino.h>
#include "config.h"
#include "wifi_manager.h"
#include "modbus_sensor.h"
#include "mqtt_client.h"
#include "thingspeak_client.h"

// Переменные для отслеживания времени
unsigned long lastSensorReadTime = 0;
unsigned long lastDataPublishTime = 0;

extern void startFakeSensorTask();
extern void startRealSensorTask();

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
    
    // Инициализация WiFi
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
    Serial.println("Hello, ESP32!");
    delay(1000);
} 