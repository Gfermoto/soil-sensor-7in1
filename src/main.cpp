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

// Задача мониторинга кнопки сброса
void resetButtonTask(void* parameter)
{
    pinMode(RESET_BUTTON_PIN, INPUT_PULLUP);

    while (true)
    {
        if (digitalRead(RESET_BUTTON_PIN) == LOW)
        {
            logWarn("Кнопка сброса нажата! Сброс настроек через 2 сек...");
            vTaskDelay(2000 / portTICK_PERIOD_MS);

            if (digitalRead(RESET_BUTTON_PIN) == LOW)
            {
                resetConfig();
                ESP.restart();
            }
        }
        vTaskDelay(100 / portTICK_PERIOD_MS);
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
    logSystem("Режим датчика: %s", config.useRealSensor ? "РЕАЛЬНЫЙ" : "ЭМУЛЯЦИЯ");
    logSystem("Интервал чтения: %d мс", SENSOR_READ_INTERVAL);

    // Инициализация WiFi
    setupWiFi();

    // Инициализация MQTT
    if (config.mqttEnabled)
    {
        setupMQTT();
        logSuccess("MQTT инициализирован");
    }

    // Инициализация Modbus (если используется реальный датчик)
    if (config.useRealSensor)
    {
        setupModbus();
    }

    // Запуск задач
    if (config.useRealSensor)
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

void loop()
{
    // Сброс watchdog
    esp_task_wdt_reset();

    // Вывод статуса системы каждые 30 секунд
    if (millis() - lastStatusPrint >= STATUS_PRINT_INTERVAL)
    {
        logPrintHeader("СТАТУС СИСТЕМЫ", COLOR_GREEN);

        logUptime();
        logMemoryUsage();
        logWiFiStatus();
        logSystem("Режим датчика: %s", config.useRealSensor ? "РЕАЛЬНЫЙ" : "ЭМУЛЯЦИЯ");

        // Статус данных датчика
        if (sensorData.valid)
        {
            logData("Последние измерения получены %.1f сек назад", (millis() - sensorData.last_update) / 1000.0);
        }
        else
        {
            logWarn("Данные датчика недоступны");
        }

        logPrintSeparator("─", 60);
        lastStatusPrint = millis();
    }

    // Основная логика публикации данных
    if (sensorData.valid)
    {
        // Публикация в MQTT
        publishSensorData();

        // Публикация в ThingSpeak
        sendDataToThingSpeak();
    }

    // Управление WiFi
    handleWiFi();

    // Управление MQTT
    handleMQTT();

    // Небольшая задержка
    vTaskDelay(100 / portTICK_PERIOD_MS);
}