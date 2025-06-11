/**
 * @file main.cpp
 * @brief Главный файл проекта JXCT датчика
 */

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
#include "config.h"
#include "fake_sensor.h"
#include "debug.h"  // ✅ Добавляем систему условной компиляции
#include "logger.h"

// Переменные для отслеживания времени
unsigned long lastDataPublishTime = 0;
unsigned long lastNtpUpdate = 0;

// Объявления функций
bool initPreferences();
void setupWiFi();
void setupModbus();
void loadConfig();
void startRealSensorTask();
void startFakeSensorTask();
void handleWiFi();
void handleMQTT();

WiFiUDP ntpUDP;
NTPClient* timeClient = nullptr;

// Константы
const int RESET_BUTTON_PIN = 0;                     // GPIO0 для кнопки сброса
const unsigned long STATUS_PRINT_INTERVAL = 30000;  // 30 секунд

// Переменные
unsigned long lastStatusPrint = 0;

// ✅ Неблокирующая задача мониторинга кнопки сброса
void resetButtonTask(void* parameter)
{
    pinMode(RESET_BUTTON_PIN, INPUT_PULLUP);
    static unsigned long buttonPressTime = 0;
    static bool buttonPressed = false;

    while (true)
    {
        bool currentState = (digitalRead(RESET_BUTTON_PIN) == LOW);
        
        if (currentState && !buttonPressed) {
            // Кнопка только что нажата
            buttonPressed = true;
            buttonPressTime = millis();
            logWarn("Кнопка сброса нажата! Сброс настроек через 2 сек...");
        }
        else if (!currentState && buttonPressed) {
            // Кнопка отпущена раньше времени
            buttonPressed = false;
            DEBUG_PRINTLN("Кнопка сброса отпущена");
        }
        else if (currentState && buttonPressed && (millis() - buttonPressTime >= 2000)) {
            // Кнопка удерживалась 2 секунды
            logError("Выполняется сброс настроек!");
            resetConfig();
            ESP.restart();
        }
        
        // ✅ Неблокирующая задержка - проверяем кнопку каждые 50мс
        vTaskDelay(50 / portTICK_PERIOD_MS);
    }
}

void setup()
{
    Serial.begin(115200);

    // Красивый баннер запуска
    logPrintBanner("JXCT 7-в-1 Датчик v2.0 - Запуск системы");

    logPrintHeader("ИНИЦИАЛИЗАЦИЯ СИСТЕМЫ", COLOR_CYAN);

    // Настройка Watchdog Timer
    logSystem("Настройка Watchdog Timer (30 сек)...");
    esp_task_wdt_init(30, true);
    esp_task_wdt_add(NULL);
    logSuccess("Watchdog Timer активирован");

    // Инициализация Preferences
    if (!initPreferences())
    {
        logError("Ошибка инициализации Preferences!");
        return;
    }
    logSuccess("Preferences инициализирован");

    // Загрузка конфигурации
    loadConfig();
    logSuccess("Конфигурация загружена");

    // Информация о режиме работы
    logSystem("Режим датчика: %s", config.flags.useRealSensor ? "РЕАЛЬНЫЙ" : "ЭМУЛЯЦИЯ");
    logSystem("Интервал чтения: %d мс", SENSOR_READ_INTERVAL);

    // Инициализация WiFi
    setupWiFi();

    // Инициализация MQTT
    if (config.flags.mqttEnabled)
    {
        setupMQTT();
        logSuccess("MQTT инициализирован");
    }

    // Инициализация Modbus (если используется реальный датчик)
    if (config.flags.useRealSensor)
    {
        setupModbus();
    }

    // Запуск задач
    if (config.flags.useRealSensor)
    {
        startRealSensorTask();
        logSuccess("Запущена задача реального датчика");
    }
    else
    {
        startFakeSensorTask();
        logSuccess("Запущена задача эмулятора датчика");
        logWarn("Включите 'useRealSensor' в настройках для работы с реальным датчиком");
    }

    // Запуск задачи мониторинга кнопки сброса
    xTaskCreate(resetButtonTask, "ResetButton", 2048, NULL, 1, NULL);

    logSuccess("Инициализация завершена успешно!");
    logPrintSeparator("─", 60);
}

bool initPreferences()
{
    return preferences.begin("jxct", false);
}

// ✅ Неблокирующий главный цикл с оптимизированными интервалами
void loop()
{
    static unsigned long lastDataPublish = 0;
    static unsigned long lastMqttCheck = 0;
    static unsigned long lastWiFiCheck = 0;
    
    // Текущее время
    unsigned long currentTime = millis();
    
    // Сброс watchdog
    esp_task_wdt_reset();

    // ✅ Вывод статуса системы каждые 30 секунд (неблокирующий)
    if (currentTime - lastStatusPrint >= STATUS_PRINT_INTERVAL)
    {
        logPrintHeader("СТАТУС СИСТЕМЫ", COLOR_GREEN);

        logUptime();
        logMemoryUsage();
        logWiFiStatus();
        logSystem("Режим датчика: %s", config.flags.useRealSensor ? "РЕАЛЬНЫЙ" : "ЭМУЛЯЦИЯ");

        // Статус данных датчика
        if (sensorData.valid)
        {
            logData("Последние измерения получены %.1f сек назад", (currentTime - sensorData.last_update) / 1000.0);
        }
        else
        {
            logWarn("Данные датчика недоступны");
        }

        logPrintSeparator("─", 60);
        lastStatusPrint = currentTime;
    }

    // ✅ ОПТИМИЗАЦИЯ 3.2: Интеллектуальный батчинг данных для группировки сетевых отправок
    static unsigned long mqttBatchTimer = 0;
    static unsigned long thingspeakBatchTimer = 0;
    static bool pendingMqttPublish = false;
    static bool pendingThingspeakPublish = false;
    
    // Проверяем наличие новых данных датчика (ОПТИМИЗИРОВАНО v2.2.1)
    if (sensorData.valid && (currentTime - lastDataPublish >= SENSOR_READ_INTERVAL))
    {
        // Помечаем что есть данные для отправки (не отправляем сразу)
        pendingMqttPublish = true;
        pendingThingspeakPublish = true;
        lastDataPublish = currentTime;
        
        DEBUG_PRINTLN("[BATCH] Новые данные помечены для групповой отправки");
    }

    // ✅ Групповая отправка MQTT (каждые 5 минут - ОПТИМИЗИРОВАНО v2.2.1)
    if (pendingMqttPublish && (currentTime - mqttBatchTimer >= MQTT_PUBLISH_INTERVAL))
    {
        publishSensorData();
        pendingMqttPublish = false;
        mqttBatchTimer = currentTime;
        DEBUG_PRINTLN("[BATCH] MQTT данные отправлены группой");
    }

    // ✅ Групповая отправка ThingSpeak (каждые 15 минут - ОПТИМИЗИРОВАНО v2.2.1)
    if (pendingThingspeakPublish && (currentTime - thingspeakBatchTimer >= THINGSPEAK_INTERVAL))
    {
        sendDataToThingSpeak();
        pendingThingspeakPublish = false;
        thingspeakBatchTimer = currentTime;
        DEBUG_PRINTLN("[BATCH] ThingSpeak данные отправлены группой");
    }

    // ✅ Управление MQTT (каждые 100мс)
    if (currentTime - lastMqttCheck >= 100)
    {
        handleMQTT();
        lastMqttCheck = currentTime;
    }

    // ✅ Управление WiFi (каждые 1 секунду)
    if (currentTime - lastWiFiCheck >= 1000)
    {
        handleWiFi();
        lastWiFiCheck = currentTime;
    }

    // ✅ Минимальная задержка для стабильности (10мс вместо 100мс)
    vTaskDelay(10 / portTICK_PERIOD_MS);
}