/**
 * @file main.cpp
 * @brief Главный файл проекта JXCT датчика
 */

// *** УНИКАЛЬНЫЙ ИДЕНТИФИКАТОР СБОРКИ v3.1.7-DEBUG-20250620-1501 ***
// *** ЕСЛИ ВЫ ВИДИТЕ ЭТО СООБЩЕНИЕ, ПРОШИВКА ОБНОВИЛАСЬ ***

#include <Arduino.h>
#include <NTPClient.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <WiFiUdp.h>
#include <esp_ota_ops.h>
#include <esp_task_wdt.h>
#include "advanced_filters.h"  // ✅ Улучшенная система фильтрации
#include "debug.h"  // ✅ Добавляем систему условной компиляции
#include "fake_sensor.h"
#include "jxct_config_vars.h"
#include "logger.h"
#include "modbus_sensor.h"
#include "mqtt_client.h"
#include "ota_manager.h"
#include "sensor_factory.h"
#include "thingspeak_client.h"
#include "version.h"     // ✅ Централизованное управление версией
#include "jxct_constants.h"  // ✅ Константы системы
#include "web_routes.h"  // ✅ CSRF защита
#include "wifi_manager.h"
#include "business/crop_recommendation_engine.h"
#include "business/sensor_calibration_service.h"
#include "business/sensor_compensation_service.h"
#ifdef TEST_BUILD
#include "esp32_stubs.h"
#elif defined(ESP32) || defined(ARDUINO)
#include <WebServer.h>
#include <LittleFS.h>
#else
#include "test/stubs/esp32_stubs.h"
#endif
// Веб-сервер
WebServer server(80); // NOLINT(misc-use-internal-linkage)

// Глобальные экземпляры бизнес-сервисов
extern CropRecommendationEngine gCropEngine;
extern SensorCalibrationService gCalibrationService;
extern SensorCompensationService gCompensationService;

// Переменные для отслеживания времени
namespace
{
unsigned long lastDataPublish = 0;
unsigned long lastNtpUpdate = 0;

unsigned long lastStatusPrint = 0;
unsigned long mqttBatchTimer = 0;
unsigned long thingspeakBatchTimer = 0;
bool pendingMqttPublish = false;
bool pendingThingspeakPublish = false;
}  // namespace

// Функции уже объявлены в соответствующих заголовочных файлах:
// setupModbus() - в modbus_sensor.h
// startRealSensorTask() - в modbus_sensor.h
// startFakeSensorTask() - в fake_sensor.h
// handleMQTT() - в mqtt_client.h

WiFiUDP ntpUDP;                   // NOLINT(misc-use-internal-linkage)
NTPClient* timeClient = nullptr;  // NOLINT(misc-use-internal-linkage)

// Константы определены в jxct_constants.h

namespace {
// Внутренняя инициализация Preferences
bool initPreferences()
{
    return preferences.begin("jxct", false);
}
}

// === ОПТИМИЗАЦИЯ 3.2: Интеллектуальный батчинг данных для группировки сетевых отправок ===
// перемещены в анонимное пространство имён выше

namespace {
// ✅ Неблокирующая задача мониторинга кнопки сброса
void resetButtonTask(void* /*parameter*/)
{
    pinMode(BOOT_BUTTON, INPUT_PULLUP);
    static unsigned long buttonPressTime = 0;
    static bool buttonPressed = false;

    while (true)
    {
        const bool currentState = (digitalRead(BOOT_BUTTON) == LOW);

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
}

#ifndef PIO_UNIT_TESTING

void setup() // NOLINT(misc-use-internal-linkage)
{
    Serial.begin(115200);

    // *** КРИТИЧЕСКОЕ ОТЛАДОЧНОЕ СООБЩЕНИЕ ***
    Serial.printf("*** УНИКАЛЬНЫЙ ИДЕНТИФИКАТОР СБОРКИ v%s ***\n", JXCT_FULL_VERSION_STRING);
    Serial.println("*** ЕСЛИ ВЫ ВИДИТЕ ЭТО СООБЩЕНИЕ, ПРОШИВКА ОБНОВИЛАСЬ УСПЕШНО ***");

    // Красивый баннер запуска
    logPrintBanner("JXCT 7-в-1 Датчик v" JXCT_VERSION_STRING " - Запуск системы");

    /*-----------------------------------------------------------
     *  Подтверждаем OTA-образ ДО тяжёлой инициализации Wi-Fi/FS.
     *  Нужно успеть до истечения тайм-аутa загрузчика (~5 с).
     *----------------------------------------------------------*/
    const esp_partition_t* runningNow = esp_ota_get_running_partition();
    esp_ota_img_states_t otaStateNow;
    if (esp_ota_get_state_partition(runningNow, &otaStateNow) == ESP_OK && otaStateNow == ESP_OTA_IMG_PENDING_VERIFY)
    {
        logSystem("OTA image pending verify → подтверждаем (ранний этап)");
        if (esp_ota_mark_app_valid_cancel_rollback() == ESP_OK)
        {
            logSuccess("OTA image подтверждена, откат отменён");
        }
        else
        {
            logError("Не удалось подтвердить OTA image (ранний этап)!");
        }
    }

    logPrintHeader("ИНИЦИАЛИЗАЦИЯ СИСТЕМЫ", LogColor::CYAN);

    // Настройка Watchdog Timer
    logSystem("Настройка Watchdog Timer (30 сек)...");
    esp_task_wdt_init(WATCHDOG_TIMEOUT_SEC, true);
    esp_task_wdt_add(nullptr);
    logSuccess("Watchdog Timer активирован");

    // Инициализация Preferences
    if (!initPreferences())
    {
        logError("Ошибка инициализации Preferences!");
        return;
    }
    logSuccess("Preferences инициализирован");

    // Инициализация LittleFS
    if (!LittleFS.begin(true))
    {
        logError("Ошибка инициализации LittleFS!");
        return;
    }
    logSuccess("LittleFS инициализирован");

    // Загрузка конфигурации
    loadConfig();
    logSuccess("Конфигурация загружена");

    // Информация о режиме работы
    logSystemSafe("\1", config.flags.useRealSensor ? "РЕАЛЬНЫЙ" : "ЭМУЛЯЦИЯ");
    logSystemSafe("\1", static_cast<unsigned int>(config.sensorReadInterval));

    // Инициализация WiFi
    setupWiFi();

    // Инициализация CSRF защиты (безопасность веб-интерфейса)
    initCSRFProtection();

    // Инициализация ThingSpeak
    if (config.flags.thingSpeakEnabled)
    {
        // extern WiFiClient espClient;  // объявлен в mqtt_client.h
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
    otaClient.setInsecure();  // временно отключаем проверку сертификата
    // Всегда устанавливаем URL манифеста для ручной и автоматической проверки
    // ИСПРАВЛЕНО - используем правильный URL манифеста
    // ВОЗВРАЩАЮ КАК РАБОТАЛО - /latest/download/
    setupOTA("https://github.com/Gfermoto/soil-sensor-7in1/releases/latest/download/manifest.json", otaClient);

    // Создаём экземпляр абстрактного сенсора
    static std::unique_ptr<ISensor> gSensor = createSensorInstance();
    gSensor->begin();

    // ✅ v3.10.0: Инициализация улучшенной системы фильтрации
    AdvancedFilters::resetAllFilters();
    logSuccess("Улучшенная система фильтрации инициализирована");

    // Legacy: оставляем старые задачи для поточного обновления sensorData
    if (config.flags.useRealSensor)
    {
        startRealSensorTask();
    }
    else
    {
        startFakeSensorTask();
    }

    // Запуск задачи мониторинга кнопки сброса
    xTaskCreate(resetButtonTask, "ResetButton", 2048, nullptr, 1, nullptr);

    // Если мы загружаемся после OTA и система ждёт подтверждения, отменяем откат после успешного старта
    const esp_partition_t* running = esp_ota_get_running_partition();
    esp_ota_img_states_t ota_state;
    if (esp_ota_get_state_partition(running, &ota_state) == ESP_OK && ota_state == ESP_OTA_IMG_PENDING_VERIFY)
    {
        logSystem("OTA image pending verify → помечаем как valid");
        if (esp_ota_mark_app_valid_cancel_rollback() == ESP_OK)
        {
            logSuccess("OTA image подтверждена, откат отменён");
        }
        else
        {
            logError("Не удалось подтвердить OTA image!");
        }
    }

    logSuccess("Инициализация завершена успешно!");
    logPrintSeparator("─", 60);
}

// ✅ Неблокирующий главный цикл с оптимизированными интервалами
void loop() // NOLINT(misc-use-internal-linkage)
{
    const unsigned long currentTime = millis();
    esp_task_wdt_reset();

    // Обновление NTP каждые 6 часов
    if (timeClient != nullptr && millis() - lastNtpUpdate > 6 * 3600 * 1000)
    {
        timeClient->forceUpdate();
        lastNtpUpdate = millis();
        logSystemSafe("\1", timeClient->isTimeSet() ? "OK" : "не удалось");
    }

    // ✅ Вывод статуса системы каждые 30 секунд (неблокирующий)
    if (currentTime - lastStatusPrint >= STATUS_PRINT_INTERVAL)
    {
        logPrintHeader("СТАТУС СИСТЕМЫ", LogColor::GREEN);

        logUptime();
        logMemoryUsage();
        logWiFiStatus();
        logSystemSafe("\1", config.flags.useRealSensor ? "РЕАЛЬНЫЙ" : "ЭМУЛЯЦИЯ");

        // Статус данных датчика
        if (sensorData.valid)
        {
            logDataSafe("\1", (currentTime - sensorData.last_update) / 1000.0);
        }
        else
        {
            logWarn("Данные датчика недоступны");
        }

        // ✅ v3.10.0: Статистика улучшенной фильтрации
        AdvancedFilters::logFilterStatistics();

        logPrintSeparator("─", 60);
        lastStatusPrint = currentTime;
    }

    // === Проверяем наличие новых данных датчика (НАСТРАИВАЕМО v2.3.0) ===
    if (sensorData.valid && (currentTime - lastDataPublish >= config.sensorReadInterval))
    {
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
        const bool tsOk = sendDataToThingSpeak();
        if (tsOk)
        {
            thingspeakBatchTimer = currentTime;  // Сбрасываем таймер только при успешной отправке
            DEBUG_PRINTLN("[BATCH] ThingSpeak данные отправлены группой");
        }
        else
        {
            DEBUG_PRINTLN("[BATCH] ThingSpeak отправка не удалась, повтор через следующий интервал");
        }
        pendingThingspeakPublish = false;
    }

    // ✅ Управление MQTT (каждые 100мс)
    static unsigned long lastMqttCheck = 0;
    if (currentTime - lastMqttCheck >= 100)
    {
        handleMQTT();
        lastMqttCheck = currentTime;
    }

    // ✅ Управление WiFi (каждые 20 мс для более высокой отзывчивости веб-интерфейса)
    static unsigned long lastWiFiCheck = 0;
    if (currentTime - lastWiFiCheck >= 20)
    {
        handleWiFi();
        lastWiFiCheck = currentTime;
    }

    // Проверяем OTA раз в час (или при принудительной проверке)
    static unsigned long lastOtaCheck = 0;
    if (config.flags.autoOtaEnabled && (currentTime - lastOtaCheck >= 3600000UL))
    {
        handleOTA();
        lastOtaCheck = currentTime;
    }

    // ✅ Минимальная задержка для стабильности (10мс вместо 100мс)
    vTaskDelay(10 / portTICK_PERIOD_MS);
}

#endif  // PIO_UNIT_TESTING
