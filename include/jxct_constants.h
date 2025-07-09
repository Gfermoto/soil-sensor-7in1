#pragma once
#include <Arduino.h>
#include <stddef.h>
#include <stdint.h>

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
constexpr const char* HTTP_CACHE_CONTROL = "no-cache, no-store, must-revalidate";
constexpr const char* HTTP_CONTENT_TYPE_HTML = "text/html; charset=utf-8";
constexpr const char* HTTP_CONTENT_TYPE_JSON = "application/json";
constexpr const char* HTTP_CONTENT_TYPE_PLAIN = "text/plain";

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
// UI И ФОРМАТИРОВАНИЕ
// ============================================================================

// Размеры разделителей и отступов
constexpr int DEFAULT_SEPARATOR_LENGTH = 60;  // Длина разделителя по умолчанию
constexpr int JXCT_REDIRECT_DELAY_MS = 1000;  // Задержка редиректа (мс)

// ============================================================================
// HTTP КОДЫ И ЗАГОЛОВКИ
// ============================================================================

// HTTP статус коды
constexpr int HTTP_OK = 200;
constexpr int HTTP_FORBIDDEN = 403;
constexpr int HTTP_REDIRECT_TEMPORARY = 302;
constexpr int HTTP_REDIRECT_PERMANENT = 307;

// HTTP статус коды (дополнительные)
constexpr int HTTP_BAD_REQUEST = 400;
constexpr int HTTP_SEE_OTHER = 303;

// ============================================================================
// JSON И ДАННЫЕ
// ============================================================================

// Размеры JSON документов
constexpr int JSON_DOC_SMALL = 512;
constexpr int JSON_DOC_MEDIUM = 1024;

// ============================================================================
// ВРЕМЯ И КОНВЕРСИИ
// ============================================================================

// Константы для конвертации времени
constexpr unsigned long SECONDS_IN_MINUTE = 60;
constexpr unsigned long MINUTES_IN_HOUR = 60;
constexpr unsigned long HOURS_IN_DAY = 24;
constexpr unsigned long MILLISECONDS_IN_SECOND = 1000;

// Задержки для веб-интерфейса
constexpr int WEB_OPERATION_DELAY_MS = 2000;  // Задержка после операций (мс)

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

// ============================================================================
// ОТЧЁТЫ И МЕТРИКИ
// ============================================================================
constexpr unsigned long REPORT_CACHE_TTL_MS = 300000;  // 5 минут
constexpr int JSON_DOC_LARGE = 2048;
constexpr int TEST_SUCCESS_RATE_THRESHOLD = 90;
constexpr int TESTS_TOTAL_COUNT = 13;
constexpr int TESTS_PASSED_COUNT = 13;
constexpr float TEST_SUCCESS_RATE_MAX = 100.0F;
constexpr const char* TEST_TIMESTAMP_EXAMPLE = "2025-01-22T12:00:00Z";
constexpr int TECH_DEBT_CODE_SMELLS = 66;
constexpr int TECH_DEBT_DUPLICATED_LINES = 933;
constexpr int TECH_DEBT_SECURITY_HOTSPOTS = 134;
constexpr float TECH_DEBT_DEBT_RATIO = 1.93F;
constexpr float TECH_DEBT_COVERAGE = 70.8F;
constexpr unsigned long REPORTS_AUTOREFRESH_INTERVAL_MS = 300000;  // 5 минут

// ============================================================================
// КОНФИГУРАЦИЯ И ВАЛИДАЦИЯ
// ============================================================================

// Лимиты интервалов конфигурации (в секундах/минутах)
constexpr int CONFIG_SENSOR_INTERVAL_MIN_SEC = 1;
constexpr int CONFIG_SENSOR_INTERVAL_MAX_SEC = 300;
constexpr int CONFIG_MQTT_INTERVAL_MIN_MIN = 1;
constexpr int CONFIG_MQTT_INTERVAL_MAX_MIN = 60;
constexpr int CONFIG_THINGSPEAK_INTERVAL_MIN_MIN = 5;
constexpr int CONFIG_THINGSPEAK_INTERVAL_MAX_MIN = 120;
constexpr int CONFIG_WEB_INTERVAL_MIN_SEC = 5;
constexpr int CONFIG_WEB_INTERVAL_MAX_SEC = 60;

// Лимиты дельта-фильтров
constexpr float CONFIG_DELTA_HUMIDITY_MIN = 0.5F;
constexpr float CONFIG_DELTA_HUMIDITY_MAX = 10.0F;
constexpr float CONFIG_DELTA_PH_MIN = 0.01F;
constexpr float CONFIG_DELTA_PH_MAX = 1.0F;
constexpr int CONFIG_DELTA_EC_MIN = 10;
constexpr int CONFIG_DELTA_EC_MAX = 500;
constexpr int CONFIG_DELTA_NPK_MIN = 1;
constexpr int CONFIG_DELTA_NPK_MAX = 50;

// Лимиты алгоритмических параметров
constexpr int CONFIG_AVG_WINDOW_MIN = 5;
constexpr int CONFIG_AVG_WINDOW_MAX = 15;
constexpr int CONFIG_FORCE_CYCLES_MIN = 5;
constexpr int CONFIG_FORCE_CYCLES_MAX = 50;

// Шаги для input полей
constexpr float CONFIG_STEP_HUMIDITY = 0.5F;
constexpr float CONFIG_STEP_PH = 0.01F;

// Конвертации времени
constexpr unsigned long CONVERSION_SEC_TO_MS = 1000;
constexpr unsigned long CONVERSION_MIN_TO_MS = 60000;

// Размеры JSON документов для конфигурации
constexpr size_t CONFIG_JSON_DOC_SIZE = 2048;
constexpr size_t CONFIG_JSON_ROOT_SIZE = 1024;

// Размеры JSON документов для отчетов
constexpr size_t REPORTS_JSON_DOC_SIZE = 1024;
constexpr size_t REPORTS_JSON_DOC_LARGE_SIZE = 2048;

// Размеры JSON документов для OTA
constexpr size_t OTA_JSON_DOC_SIZE = 256;

// ============================================================================
// OTA ОБНОВЛЕНИЯ
// ============================================================================

// Пороги и интервалы для OTA
constexpr unsigned long OTA_PROGRESS_LOG_THRESHOLD = 65536;  // 64KB
constexpr unsigned long OTA_DELAY_MS = 2000;
constexpr unsigned long OTA_UPDATE_INTERVAL_MS = 1000;

// Процентные значения для стадий OTA
constexpr int OTA_STAGE_CONNECTION = 25;
constexpr int OTA_STAGE_DOWNLOAD = 50;
constexpr int OTA_STAGE_VERIFY = 75;
constexpr int OTA_STAGE_FINISH = 90;
constexpr int OTA_STAGE_INSTALL = 95;
constexpr int OTA_STAGE_CHECK = 30;
constexpr int OTA_STAGE_DEFAULT = 25;

// ============================================================================
// ТЕСТОВЫЕ ДАННЫЕ
// ============================================================================

// Базовые значения для тестовых данных
constexpr float TEST_DATA_TEMP_BASE = 20.0F;
constexpr float TEST_DATA_HUM_BASE = 60.0F;
constexpr int TEST_DATA_EC_BASE = 1200;
constexpr float TEST_DATA_PH_BASE = 6.3F;
constexpr int TEST_DATA_NPK_BASE = 1000;  // ИСПРАВЛЕНО: увеличено с 25 до 1000 мг/кг для соответствия агрономическим нормам

// Вариации для тестовых данных
constexpr float TEST_DATA_HUM_VARIATION = 10.0F;
constexpr int TEST_DATA_EC_VARIATION = 300;
constexpr int TEST_DATA_EC_VARIATION_SMALL = 200;
constexpr int TEST_DATA_EC_VARIATION_MIN = 100;

// Факторы для NPK
constexpr float TEST_DATA_NPK_FACTOR = 6.5F;       // пересчёт мг/дм³ → мг/кг
constexpr float TEST_DATA_NPK_INCREASE_N = 1.20F;  // +20%
constexpr float TEST_DATA_NPK_INCREASE_P = 1.15F;  // +15%
constexpr float TEST_DATA_NPK_INCREASE_K = 1.10F;  // +10%
constexpr float TEST_DATA_NPK_DECREASE_N = 0.90F;  // -10%
constexpr float TEST_DATA_NPK_DECREASE_P = 1.05F;  // +5%
constexpr float TEST_DATA_NPK_DECREASE_K = 1.25F;  // +25%

// ============================================================================
// ДОПОЛНИТЕЛЬНЫЕ КОНСТАНТЫ
// ============================================================================

// HTTP статус коды (дополнительные)
constexpr int HTTP_REDIRECT = 302;

// Пороги для отчетов
constexpr int SUCCESS_RATE_THRESHOLD = 90;

// Интервалы
// constexpr unsigned long REPORTS_AUTOREFRESH_INTERVAL_MS = 300000; // 5 минут - уже определено выше

// NTP и время
constexpr unsigned long NTP_TIMESTAMP_2000 = 946684800;  // 2000-01-01 00:00:00 UTC

// Валидация сенсорных данных
constexpr float TEMP_MIN_VALID = -45.0F;
constexpr float TEMP_MAX_VALID = 115.0F;
constexpr float HUM_MIN_VALID = 0.0F;
constexpr float HUM_MAX_VALID = 100.0F;
constexpr int EC_MAX_VALID = 10000;
constexpr int NPK_MAX_VALID = 1999;

// Размеры JSON документов
constexpr size_t SENSOR_JSON_DOC_SIZE = 512;
