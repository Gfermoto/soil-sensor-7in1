/**
 * @file main.cpp
 * @brief Главный файл проекта JXCT датчика
 */

#include <WiFiClientSecure.h>
#include <WiFiClient.h>
#include <Arduino.h>
#include "version.h"  // ✅ Централизованное управление версией
#include "wifi_manager.h"
#include "modbus_sensor.h"
#include "mqtt_client.h"
#include "thingspeak_client.h"
#include "jxct_config_vars.h"
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <esp_task_wdt.h>
#include "fake_sensor.h"
#include "debug.h"  // ✅ Добавляем систему условной компиляции
#include "logger.h"
#include "sensor_factory.h"
#include "ota_manager.h"
#include <esp_ota_ops.h>

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

        if (currentState && !buttonPressed)
        {
            // Кнопка только что нажата
            buttonPressed = true;
            buttonPressTime = millis();
            logWarn("Кнопка сброса нажата! Сброс настроек через 2 сек...");
        }
        else if (!currentState && buttonPressed)
        {
            // Кнопка отпущена раньше времени
            buttonPressed = false;
            DEBUG_PRINTLN("Кнопка сброса отпущена");
        }
        else if (currentState && buttonPressed && (millis() - buttonPressTime >= BUTTON_HOLD_TIME_MS))
        {
            // Кнопка удерживалась 2 секунды
            logError("Выполняется сброс настроек!");
            resetConfig();
            ESP.restart();
        }

        // ✅ Неблокирующая задержка - проверяем кнопку каждые 50мс
        vTaskDelay(50 / portTICK_PERIOD_MS);
    }
}

#ifndef PIO_UNIT_TESTING

void setup()
{
    Serial.begin(115200);

    // Красивый баннер запуска
    logPrintBanner("JXCT 7-в-1 Датчик v" JXCT_VERSION_STRING " - Запуск системы");

    logPrintHeader("ИНИЦИАЛИЗАЦИЯ СИСТЕМЫ", COLOR_CYAN);

    // Настройка Watchdog Timer
    logSystem("Настройка Watchdog Timer (30 сек)...");
    esp_task_wdt_init(WATCHDOG_TIMEOUT_SEC, true);
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
    logSystem("Интервал чтения: %d мс", config.sensorReadInterval);

    // Инициализация WiFi
    setupWiFi();

    // Инициализация ThingSpeak
    if (config.flags.thingSpeakEnabled)
    {
        extern WiFiClient espClient;  // объявлен в mqtt_client.cpp
        setupThingSpeak(espClient);
        logSuccess("ThingSpeak инициализирован");
    }

    // Инициализация MQTT
    if (config.flags.mqttEnabled)
    {
        setupMQTT();
        logSuccess("MQTT инициализирован");
    }

    // Инициализация OTA 2.0 (проверка манифеста раз в час) через HTTPS
    static WiFiClientSecure otaClient;
    otaClient.setInsecure(); // временно отключаем проверку сертификата
    // Всегда устанавливаем URL манифеста для ручной и автоматической проверки
    setupOTA("https://github.com/Gfermoto/soil-sensor-7in1/releases/latest/download/manifest.json", otaClient);

    // Создаём экземпляр абстрактного сенсора
    static std::unique_ptr<ISensor> gSensor = createSensorInstance();
    gSensor->begin();

    // Legacy: оставляем старые задачи для поточного обновления sensorData
    if (config.flags.useRealSensor)
        startRealSensorTask();
    else
        startFakeSensorTask();

    // Запуск задачи мониторинга кнопки сброса
    xTaskCreate(resetButtonTask, "ResetButton", 2048, NULL, 1, NULL);

    // Если мы загружаемся после OTA и система ждёт подтверждения, отменяем откат после успешного старта
    const esp_partition_t* running = esp_ota_get_running_partition();
    esp_ota_img_states_t ota_state;
    if (esp_ota_get_state_partition(running, &ota_state) == ESP_OK && ota_state == ESP_OTA_IMG_PENDING_VERIFY)
    {
        logSystem("OTA image pending verify → помечаем как valid");
        if (esp_ota_mark_app_valid_cancel_rollback() == ESP_OK)
            logSuccess("OTA image подтверждена, откат отменён");
        else
            logError("Не удалось подтвердить OTA image!");
    }

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

    // Обновление NTP каждые 6 часов
    if (timeClient && millis() - lastNtpUpdate > 6 * 3600 * 1000)
    {
        timeClient->forceUpdate();
        lastNtpUpdate = millis();
        logSystem("NTP обновление: %s", timeClient->isTimeSet() ? "OK" : "не удалось");
    }

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

    // Проверяем наличие новых данных датчика (НАСТРАИВАЕМО v2.3.0)
    if (sensorData.valid && (currentTime - lastDataPublish >= config.sensorReadInterval))
    {
        // Помечаем что есть данные для отправки (не отправляем сразу)
        pendingMqttPublish = true;
        pendingThingspeakPublish = true;
        lastDataPublish = currentTime;

        DEBUG_PRINTLN("[BATCH] Новые данные помечены для групповой отправки");
    }

    // ✅ Групповая отправка MQTT (настраиваемо v2.3.0)
    if (pendingMqttPublish && (currentTime - mqttBatchTimer >= config.mqttPublishInterval))
    {
        publishSensorData();
        pendingMqttPublish = false;
        mqttBatchTimer = currentTime;
        DEBUG_PRINTLN("[BATCH] MQTT данные отправлены группой");
    }

    // ✅ Групповая отправка ThingSpeak (настраиваемо v2.3.0)
    if (pendingThingspeakPublish && (currentTime - thingspeakBatchTimer >= config.thingSpeakInterval))
    {
        bool tsOk = sendDataToThingSpeak();
        pendingThingspeakPublish = false;
        if (tsOk)
        {
            thingspeakBatchTimer = currentTime;  // Сбрасываем таймер только при успешной отправке
            DEBUG_PRINTLN("[BATCH] ThingSpeak данные отправлены группой");
        }
        else
        {
            DEBUG_PRINTLN("[BATCH] ThingSpeak отправка не удалась, повтор через следующий интервал");
        }
    }

    // ✅ Управление MQTT (каждые 100мс)
    if (currentTime - lastMqttCheck >= 100)
    {
        handleMQTT();
        lastMqttCheck = currentTime;
    }

    // ✅ Управление WiFi (каждые 20 мс для более высокой отзывчивости веб-интерфейса)
    if (currentTime - lastWiFiCheck >= 20)
    {
        handleWiFi();
        lastWiFiCheck = currentTime;
    }

    // ✅ Проверяем OTA только раз в час (или при принудительной проверке)
    static unsigned long lastOtaCheck = 0;
    if (config.flags.autoOtaEnabled && (currentTime - lastOtaCheck >= 3600000UL)) // 1 час
    {
        handleOTA();
        lastOtaCheck = currentTime;
    }

    // ✅ Минимальная задержка для стабильности (10мс вместо 100мс)
    vTaskDelay(10 / portTICK_PERIOD_MS);
}

#endif // PIO_UNIT_TESTING