#pragma once

// Аппаратные пины (перенесены в jxct_constants.h для централизации)
// Оставляем только те, которые еще не перенесены
#define BOOT_BUTTON 0
#define STATUS_LED_PIN 2

// Интервалы (мс) - МАКСИМАЛЬНАЯ ОТЗЫВЧИВОСТЬ
#define SENSOR_READ_INTERVAL 1000    // 1 сек - максимальная частота опроса датчика
#define MQTT_PUBLISH_INTERVAL 60000  // 1 мин (было 5 мин) - увеличенная частота отправки
#define THINGSPEAK_INTERVAL 900000   // 15 мин (было 15 сек) - соблюдение rate limit
#define WEB_UPDATE_INTERVAL 5000     // 5 сек (было 10 сек) - улучшенная отзывчивость

// Константы для системы - v2.5.0 подготовка к OTA
#define HTTP_PORT 80                 // Порт веб-сервера
#define DNS_PORT 53                  // Порт DNS сервера для captive portal
#define LED_FAST_BLINK_MS 100        // Быстрое мигание LED
#define LED_SLOW_BLINK_MS 500        // Медленное мигание LED
#define WIFI_MAX_ATTEMPTS 20         // Максимум попыток подключения к WiFi
#define WIFI_RETRY_DELAY_MS 500      // Задержка между попытками WiFi
#define BUTTON_HOLD_TIME_MS 2000     // Время удержания кнопки для сброса
#define CONFIG_SAVE_DELAY_MS 2000    // Задержка перед перезагрузкой после сохранения
#define WATCHDOG_TIMEOUT_SEC 30      // Таймаут watchdog timer
#define OTA_WATCHDOG_TIMEOUT_SEC 60  // Увеличенный таймаут для OTA

// ДЕЛЬТА-ФИЛЬТР v2.2.1: Пороговые значения для публикации (МИНИМАЛЬНАЯ ФИЛЬТРАЦИЯ)
#define DELTA_TEMPERATURE 0.1f  // ±0.1°C (минимум)
#define DELTA_HUMIDITY 0.5f     // ±0.5% (минимум)
#define DELTA_PH 0.01f          // ±0.01 pH (минимум)
#define DELTA_EC 10.0f          // ±10 µS/cm (минимум)
#define DELTA_NPK 1.0f          // ±1 mg/kg (минимум для N, P, K)
#define FORCE_PUBLISH_CYCLES 5  // Принудительная публикация каждые 5 циклов (5 мин при 1 мин интервале)

// Modbus ID по умолчанию
#define JXCT_MODBUS_ID 1

// Константы для точки доступа
#define WIFI_AP_SSID "JXCT-Setup"
#define WIFI_AP_PASS "12345678"

// Экспорт глобальной конфигурации
#include <Preferences.h>

// Оптимизированная упакованная структура конфигурации
struct __attribute__((packed)) Config
{
    // WiFi настройки
    char ssid[32];
    char password[32];  // Сократил с 64 до 32 (обычно достаточно)

    // MQTT настройки
    char mqttServer[48];  // Сократил с 64 до 48 байт
    uint16_t mqttPort;
    char mqttUser[24];         // Сократил с 32 до 24 байт
    char mqttPassword[24];     // Сократил с 32 до 24 байт
    char mqttTopicPrefix[48];  // Сократил с 64 до 48 байт
    char mqttDeviceName[24];   // Сократил с 32 до 24 байт
    uint8_t mqttQos;

    // ThingSpeak настройки
    char thingSpeakApiKey[24];     // Сократил с 32 до 24 байт
    char thingSpeakChannelId[12];  // Сократил с 16 до 12 байт
    uint16_t thingspeakInterval;

    // Информация о устройстве
    char manufacturer[24];  // Сократил с 32 до 24 байт
    char model[24];         // Сократил с 32 до 24 байт
    char swVersion[12];     // Сократил с 16 до 12 байт

    // NTP настройки
    char ntpServer[48];  // Сократил с 64 до 48 байт
    uint32_t ntpUpdateInterval;

    // Датчик настройки
    uint8_t modbusId;

    // Безопасность веб-интерфейса
    char webPassword[24];  // Пароль для доступа к веб-интерфейсу

    // v2.3.0: Настраиваемые интервалы (16 байт)
    uint32_t sensorReadInterval;   // Интервал опроса датчика (1-300 сек, по умолчанию 1 сек)
    uint32_t mqttPublishInterval;  // Интервал MQTT публикации (1-60 мин)
    uint32_t thingSpeakInterval;   // Интервал ThingSpeak (5-120 мин)
    uint32_t webUpdateInterval;    // Интервал обновления веб-интерфейса (5-60 сек)

    // v2.3.0: Настраиваемые пороги дельта-фильтра (20 байт)
    float deltaTemperature;  // Порог температуры (0.1-5.0°C)
    float deltaHumidity;     // Порог влажности (0.5-10.0%)
    float deltaPh;           // Порог pH (0.01-1.0)
    float deltaEc;           // Порог EC (10-500 µS/cm)
    float deltaNpk;          // Порог NPK (1-50 mg/kg)

    // v2.3.0: Настройки скользящего среднего (8 байт)
    uint8_t movingAverageWindow;   // Размер окна (5-15 измерений)
    uint8_t forcePublishCycles;    // Принудительная публикация (5-50 циклов)
    uint8_t filterAlgorithm;       // 0=среднее, 1=медиана
    uint8_t outlierFilterEnabled;  // 0=отключен, 1=включен (>2σ)

    // v2.5.1: Настройки калибровки
    uint8_t soilProfile;  // 0 = sand, 1 = loam, 2 = peat

    // v2.6.0: Агро-профили
    float latitude;   // Широта устройства (градусы)
    float longitude;  // Долгота устройства (градусы)
    char cropId[16];  // Идентификатор культуры ("tomato", ...)

    // v2.6.0: Детектор полива
    float irrigationSpikeThreshold;      // % прироста влажности
    uint16_t irrigationHoldMinutes;      // мин удержания повышенной влажности
    uint16_t postIrrigationHoldMinutes;  // мин игнорировать коррекцию после полива

    // v2.6.1: Тип среды выращивания (0=outdoor,1=greenhouse,2=indoor)
    uint8_t environmentType;

    // Битовые поля для boolean флагов (экономия 4 байта)
    struct __attribute__((packed))
    {
        uint8_t hassEnabled : 1;            // 1 бит вместо 1 байта
        uint8_t useRealSensor : 1;          // 1 бит вместо 1 байта
        uint8_t mqttEnabled : 1;            // 1 бит вместо 1 байта
        uint8_t thingSpeakEnabled : 1;      // 1 бит вместо 1 байта
        uint8_t calibrationEnabled : 1;     // Включена ли компенсация
        uint8_t isGreenhouse : 1;           // 1 = теплица, 0 = открытый грунт (устарело)
        uint8_t seasonalAdjustEnabled : 1;  // Учитывать сезонные коэффициенты
        uint8_t autoOtaEnabled : 1;         // автоматическое OTA разрешено
    } flags;
};

extern Config config;
extern Preferences preferences;

// Объявления функций работы с конфигом
void loadConfig();
void saveConfig();
void resetConfig();
bool isConfigValid();