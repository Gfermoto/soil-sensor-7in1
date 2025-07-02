#pragma once

/**
 * @file jxct_constants.h
 * @brief Централизованные константы системы JXCT
 * @details Все магические числа и конфигурационные константы в одном месте
 */

// ============================================================================
// ВРЕМЕННЫЕ КОНСТАНТЫ (миллисекунды)
// ============================================================================

// Интервалы чтения и публикации
constexpr unsigned long DEFAULT_SENSOR_READ_INTERVAL = 5000;    // 5 секунд
constexpr unsigned long DEFAULT_MQTT_PUBLISH_INTERVAL = 30000;  // 30 секунд
constexpr unsigned long DEFAULT_THINGSPEAK_INTERVAL = 60000;    // 1 минута
constexpr unsigned long DEFAULT_NTP_UPDATE_INTERVAL = 3600000;  // 1 час

// Таймауты и задержки
constexpr unsigned long MODBUS_CACHE_TIMEOUT = 5000;     // 5 секунд
constexpr unsigned long MODBUS_RETRY_DELAY = 1000;       // 1 секунда
constexpr unsigned long DNS_CACHE_TTL = 300000;          // 5 минут
constexpr unsigned long MQTT_RECONNECT_INTERVAL = 5000;  // 5 секунд
constexpr unsigned long SENSOR_JSON_CACHE_TTL = 1000;    // 1 секунда

// Системные интервалы
constexpr unsigned long STATUS_PRINT_INTERVAL = 30000;    // 30 секунд
constexpr unsigned long JXCT_WATCHDOG_TIMEOUT_SEC = 30;   // 30 секунд (избегаем конфликта)
constexpr unsigned long JXCT_BUTTON_HOLD_TIME_MS = 2000;  // 2 секунды (избегаем конфликта)

// LED индикация
constexpr unsigned long LED_BLINK_SLOW = 1000;   // 1 секунда
constexpr unsigned long LED_BLINK_NORMAL = 500;  // 0.5 секунды
constexpr unsigned long LED_BLINK_FAST = 100;    // 0.1 секунды

// ============================================================================
// СЕТЕВЫЕ КОНСТАНТЫ
// ============================================================================

// Порты и адреса
constexpr int DEFAULT_WEB_SERVER_PORT = 80;
constexpr int DEFAULT_MQTT_PORT = 1883;
constexpr int DEFAULT_DNS_PORT = 53;
constexpr uint8_t DEFAULT_MODBUS_ADDRESS = 1;

// Размеры буферов
constexpr size_t MQTT_BUFFER_SIZE = 512;
constexpr size_t JSON_BUFFER_SIZE = 256;
constexpr size_t TOPIC_BUFFER_SIZE = 128;
constexpr size_t CLIENT_ID_BUFFER_SIZE = 32;
constexpr size_t HOSTNAME_BUFFER_SIZE = 64;

// Лимиты подключений
constexpr int WIFI_CONNECTION_ATTEMPTS = 20;
constexpr int MQTT_CONNECTION_ATTEMPTS = 3;
constexpr unsigned long WIFI_CONNECTION_TIMEOUT = 10000;  // 10 секунд

// ============================================================================
// MODBUS КОНСТАНТЫ
// ============================================================================

// Параметры связи
constexpr unsigned long MODBUS_BAUD_RATE = 9600;
constexpr uint8_t MODBUS_DATA_BITS = 8;
constexpr uint8_t MODBUS_STOP_BITS = 1;
constexpr uint8_t MODBUS_PARITY = 0;  // None

// Повторы и таймауты
constexpr uint8_t MODBUS_MAX_RETRIES = 3;
constexpr unsigned long MODBUS_RESPONSE_TIMEOUT = 2000;  // 2 секунды
constexpr unsigned long MODBUS_FRAME_DELAY = 100;        // 100 мс между кадрами

// ============================================================================
// ВАЛИДАЦИОННЫЕ КОНСТАНТЫ
// ============================================================================

// Диапазоны значений датчика
constexpr float SENSOR_TEMP_MIN = -40.0F;
constexpr float SENSOR_TEMP_MAX = 80.0F;
constexpr float SENSOR_HUMIDITY_MIN = 0.0F;
constexpr float SENSOR_HUMIDITY_MAX = 100.0F;
constexpr float SENSOR_PH_MIN = 0.0F;
constexpr float SENSOR_PH_MAX = 14.0F;
constexpr uint16_t SENSOR_EC_MIN = 0;
constexpr uint16_t SENSOR_EC_MAX = 20000;
constexpr uint16_t SENSOR_NPK_MIN = 0;
constexpr uint16_t SENSOR_NPK_MAX = 9999;

// Диапазоны конфигурации
constexpr unsigned long CONFIG_INTERVAL_MIN = 1000;       // 1 секунда
constexpr unsigned long CONFIG_INTERVAL_MAX = 3600000;    // 1 час
constexpr unsigned long CONFIG_THINGSPEAK_MIN = 15000;    // 15 секунд (лимит API)
constexpr unsigned long CONFIG_THINGSPEAK_MAX = 7200000;  // 2 часа
constexpr int CONFIG_MQTT_PORT_MIN = 1;
constexpr int CONFIG_MQTT_PORT_MAX = 65535;

// ============================================================================
// АЛГОРИТМИЧЕСКИЕ КОНСТАНТЫ
// ============================================================================

// Скользящее среднее
constexpr uint8_t MOVING_AVERAGE_WINDOW_MIN = 1;
constexpr uint8_t MOVING_AVERAGE_WINDOW_MAX = 20;
constexpr uint8_t MOVING_AVERAGE_WINDOW_DEFAULT = 5;

// Дельта-фильтры (пороги изменений)
constexpr float DEFAULT_DELTA_TEMPERATURE = 0.5F;  // 0.5°C
constexpr float DEFAULT_DELTA_HUMIDITY = 2.0F;     // 2%
constexpr float DEFAULT_DELTA_PH = 0.1F;           // 0.1 pH
constexpr float DEFAULT_DELTA_EC = 50.0F;          // 50 µS/cm
constexpr float DEFAULT_DELTA_NPK = 10.0F;         // 10 mg/kg

// Принудительная публикация
constexpr uint8_t DEFAULT_FORCE_PUBLISH_CYCLES = 10;  // Каждые 10 циклов

// ============================================================================
// СТРОКОВЫЕ КОНСТАНТЫ
// ============================================================================

// WiFi
constexpr const char* JXCT_WIFI_AP_PASS = "12345678";  // Избегаем конфликта
constexpr const char* WIFI_HOSTNAME_PREFIX = "jxct-";

// MQTT топики
constexpr const char* MQTT_TOPIC_STATE = "/state";
constexpr const char* MQTT_TOPIC_STATUS = "/status";
constexpr const char* MQTT_TOPIC_COMMAND = "/command";
constexpr const char* MQTT_TOPIC_AVAILABILITY = "/availability";

// Home Assistant
constexpr const char* HASS_DISCOVERY_PREFIX = "homeassistant/sensor/";
constexpr const char* HASS_CONFIG_SUFFIX = "/config";

// HTTP заголовки
constexpr const char* HTTP_CONTENT_TYPE_JSON = "application/json";
constexpr const char* HTTP_CONTENT_TYPE_HTML = "text/html; charset=utf-8";
constexpr const char* HTTP_CACHE_CONTROL = "no-cache, no-store, must-revalidate";

// ============================================================================
// GPIO КОНСТАНТЫ
// ============================================================================

constexpr int RESET_BUTTON_PIN = 0;     // GPIO0
constexpr int JXCT_STATUS_LED_PIN = 2;  // GPIO2 (встроенный LED) - избегаем конфликта

// Modbus RS485 пины
constexpr int MODBUS_RX_PIN = 16;  // GPIO16 - прием данных
constexpr int MODBUS_TX_PIN = 17;  // GPIO17 - передача данных
constexpr int MODBUS_DE_PIN = 4;   // GPIO4 - управление передачей (Driver Enable)
constexpr int MODBUS_RE_PIN = 5;   // GPIO5 - управление приемом (Receiver Enable)

// ============================================================================
// ПРОИЗВОДИТЕЛЬНОСТЬ И ПАМЯТЬ
// ============================================================================

// Размеры стеков задач (в байтах)
constexpr size_t SENSOR_TASK_STACK_SIZE = 4096;
constexpr size_t RESET_BUTTON_TASK_STACK_SIZE = 2048;
constexpr size_t WEB_SERVER_TASK_STACK_SIZE = 8192;

// Приоритеты задач
constexpr UBaseType_t SENSOR_TASK_PRIORITY = 2;
constexpr UBaseType_t RESET_BUTTON_TASK_PRIORITY = 1;
constexpr UBaseType_t WEB_SERVER_TASK_PRIORITY = 1;

// Лимиты памяти
constexpr size_t MAX_CONFIG_JSON_SIZE = 2048;  // 2KB для конфигурации
constexpr size_t MAX_SENSOR_JSON_SIZE = 512;   // 512B для данных датчика
constexpr size_t MAX_LOG_MESSAGE_SIZE = 256;   // 256B для лог сообщений

// ============================================================================
// ОТЛАДКА И ЛОГИРОВАНИЕ
// ============================================================================

// Уровни логирования (определены в logger.h, но дублируем)
constexpr int LOG_LEVEL_ERROR = 0;
constexpr int LOG_LEVEL_WARN = 1;
constexpr int LOG_LEVEL_INFO = 2;
constexpr int LOG_LEVEL_DEBUG = 3;

// Отладочные флаги
#ifdef DEBUG_BUILD
constexpr bool DEBUG_MODBUS_ENABLED = true;
constexpr bool DEBUG_MQTT_ENABLED = true;
constexpr bool DEBUG_WIFI_ENABLED = true;
#else
constexpr bool DEBUG_MODBUS_ENABLED = false;
constexpr bool DEBUG_MQTT_ENABLED = false;
constexpr bool DEBUG_WIFI_ENABLED = false;
#endif

// ============================================================================
// ВЕРСИОНИРОВАНИЕ И СОВМЕСТИМОСТЬ
// ============================================================================

// API версии
constexpr const char* API_VERSION_V1 = "v1";
constexpr const char* API_VERSION_CURRENT = API_VERSION_V1;

// Совместимость конфигурации
constexpr int CONFIG_VERSION_CURRENT = 1;
constexpr int CONFIG_VERSION_MIN_SUPPORTED = 1;

// OTA константы (подготовка к v2.5.0)
constexpr size_t OTA_BUFFER_SIZE = 1024;       // 1KB буфер для OTA
constexpr unsigned long OTA_TIMEOUT = 300000;  // 5 минут таймаут
constexpr const char* OTA_UPDATE_URL_TEMPLATE = "https://api.github.com/repos/%s/%s/releases/latest";