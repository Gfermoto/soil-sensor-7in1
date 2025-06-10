#include <WiFiClient.h>
#include <Arduino.h>
#include "wifi_manager.h"
#include "modbus_sensor.h"
#include "mqtt_client.h"
#include "thingspeak_client.h"
#include "jxct_config_vars.h"
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <esp_task_wdt.h>

// Переменные для отслеживания времени
unsigned long lastDataPublishTime = 0;
unsigned long lastNtpUpdate = 0;

// Объявления функций
void initPreferences();
void setupWiFi();
void setupModbus();
void loadConfig();
void startRealSensorTask();
void startFakeSensorTask();
void handleWiFi();
void handleMQTT();

WiFiUDP ntpUDP;
NTPClient* timeClient = nullptr;

void resetButtonTask(void *pvParameters) {
    for (;;) {
        if (checkResetButton()) {
            Serial.println("[resetButtonTask] Кнопка сброса нажата!");
            resetConfig();
            setLedFastBlink();
            vTaskDelay(2000 / portTICK_PERIOD_MS); // Неблокирующая задержка
            ESP.restart();
        }
        vTaskDelay(50 / portTICK_PERIOD_MS);
    }
}

void setup() {
    Serial.begin(115200);
    Serial.println("\n[setup] Инициализация устройства...");

    // Инициализация Watchdog Timer (30 секунд)
    Serial.println("[setup] Настройка Watchdog Timer...");
    esp_task_wdt_init(30, true);
    esp_task_wdt_add(NULL);
    Serial.println("[setup] Watchdog Timer активирован");

    // Инициализация EEPROM и конфигурации
    initPreferences();
    loadConfig();

    // Настройка WiFi
    setupWiFi();

    // Настройка Modbus и датчика
    setupModbus();
    Serial.print("[setup] Режим реального датчика: "); 
    Serial.println(config.useRealSensor ? "ВКЛЮЧЕН" : "ВЫКЛЮЧЕН");
    Serial.print("[setup] Интервал чтения датчика: "); 
    Serial.print(SENSOR_READ_INTERVAL); 
    Serial.println(" мс");

    // Запуск задачи датчика
    if (config.useRealSensor) {
        startRealSensorTask();
        Serial.println("[setup] Запущена задача реального датчика");
    } else {
        startFakeSensorTask();
        Serial.println("[setup] Запущена задача фейкового датчика");
        Serial.println("[setup] ВНИМАНИЕ: Для работы с реальным датчиком включите опцию 'useRealSensor' в настройках!");
    }
    Serial.println("[setup] Инициализация завершена");
}

void initPreferences() {
    // Инициализация Preferences
    if (!preferences.begin("jxct-sensor", false)) {
        Serial.println("[initPreferences] Ошибка инициализации Preferences!");
    } else {
        Serial.println("[initPreferences] Preferences успешно инициализирован");
    }
}

void loop() {
    handleWiFi();
    handleMQTT();
    
    // Расширенная отладка чтения датчика
    static unsigned long lastDebugTime = 0;
    if (millis() - lastDebugTime >= 10000) {  // Отладочное сообщение каждые 10 секунд
        lastDebugTime = millis();
        Serial.println("\n[DEBUG] Состояние системы:");
        Serial.print("Время работы: "); Serial.print(millis()/1000); Serial.println(" сек");
        Serial.print("Режим реального датчика: "); 
        Serial.println(config.useRealSensor ? "ВКЛЮЧЕН" : "ВЫКЛЮЧЕН");
    }
    
    // Чтение датчика выполняется в отдельной задаче - дублирование удалено!
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
    
    // Сброс watchdog timer
    esp_task_wdt_reset();
    
    // Неблокирующая пауза
    vTaskDelay(10 / portTICK_PERIOD_MS);
} 