#pragma once

/**
 * @file debug_optimized.h
 * @brief Оптимизированная система отладки с условной компиляцией
 * @details Заменяет избыточные DEBUG_PRINTF/DEBUG_PRINTLN на условные макросы
 */

#include "jxct_constants.h"

// ============================================================================
// УСЛОВНАЯ КОМПИЛЯЦИЯ DEBUG СООБЩЕНИЙ
// ============================================================================

#ifdef DEBUG_BUILD
#define DEBUG_ENABLED true
#else
#define DEBUG_ENABLED false
#endif

// ============================================================================
// ОПТИМИЗИРОВАННЫЕ DEBUG МАКРОСЫ
// ============================================================================

// Общие отладочные макросы
#if DEBUG_ENABLED
#define DEBUG_PRINT(x) Serial.print(x)
#define DEBUG_PRINTLN(x) Serial.println(x)
#define DEBUG_PRINTF(fmt, ...) Serial.printf(fmt, ##__VA_ARGS__)
#else
#define DEBUG_PRINT(x) \
    do                 \
    {                  \
    } while (0)
#define DEBUG_PRINTLN(x) \
    do                   \
    {                    \
    } while (0)
#define DEBUG_PRINTF(fmt, ...) \
    do                         \
    {                          \
    } while (0)
#endif

// Модульные отладочные макросы (включаются отдельно)
#if DEBUG_ENABLED && DEBUG_MODBUS_ENABLED
#define DEBUG_MODBUS_PRINT(x)  \
    Serial.print("[MODBUS] "); \
    Serial.print(x)
#define DEBUG_MODBUS_PRINTLN(x) \
    Serial.print("[MODBUS] ");  \
    Serial.println(x)
#define DEBUG_MODBUS_PRINTF(fmt, ...) Serial.printf("[MODBUS] " fmt, ##__VA_ARGS__)
#else
#define DEBUG_MODBUS_PRINT(x) \
    do                        \
    {                         \
    } while (0)
#define DEBUG_MODBUS_PRINTLN(x) \
    do                          \
    {                           \
    } while (0)
#define DEBUG_MODBUS_PRINTF(fmt, ...) \
    do                                \
    {                                 \
    } while (0)
#endif

#if DEBUG_ENABLED && DEBUG_MQTT_ENABLED
#define DEBUG_MQTT_PRINT(x)  \
    Serial.print("[MQTT] "); \
    Serial.print(x)
#define DEBUG_MQTT_PRINTLN(x) \
    Serial.print("[MQTT] ");  \
    Serial.println(x)
#define DEBUG_MQTT_PRINTF(fmt, ...) Serial.printf("[MQTT] " fmt, ##__VA_ARGS__)
#else
#define DEBUG_MQTT_PRINT(x) \
    do                      \
    {                       \
    } while (0)
#define DEBUG_MQTT_PRINTLN(x) \
    do                        \
    {                         \
    } while (0)
#define DEBUG_MQTT_PRINTF(fmt, ...) \
    do                              \
    {                               \
    } while (0)
#endif

#if DEBUG_ENABLED && DEBUG_WIFI_ENABLED
#define DEBUG_WIFI_PRINT(x)  \
    Serial.print("[WIFI] "); \
    Serial.print(x)
#define DEBUG_WIFI_PRINTLN(x) \
    Serial.print("[WIFI] ");  \
    Serial.println(x)
#define DEBUG_WIFI_PRINTF(fmt, ...) Serial.printf("[WIFI] " fmt, ##__VA_ARGS__)
#else
#define DEBUG_WIFI_PRINT(x) \
    do                      \
    {                       \
    } while (0)
#define DEBUG_WIFI_PRINTLN(x) \
    do                        \
    {                         \
    } while (0)
#define DEBUG_WIFI_PRINTF(fmt, ...) \
    do                              \
    {                               \
    } while (0)
#endif

// ============================================================================
// ПРОИЗВОДИТЕЛЬНЫЕ ОТЛАДОЧНЫЕ ФУНКЦИИ
// ============================================================================

/**
 * @brief Условная отладочная печать с проверкой уровня логирования
 * @param level Минимальный уровень логирования для вывода
 * @param format Формат строки
 * @param ... Аргументы форматирования
 */
inline void debugConditionalPrint(int level, const char* format, ...)
{
#if DEBUG_ENABLED
    extern int currentLogLevel;
    if (currentLogLevel >= level)
    {
        va_list args;
        va_start(args, format);
        Serial.printf(format, args);
        va_end(args);
    }
#endif
}

/**
 * @brief Отладочная печать буфера в HEX формате (оптимизированная)
 * @param prefix Префикс сообщения
 * @param buffer Буфер данных
 * @param length Длина буфера
 */
inline void debugPrintHexBuffer(const char* prefix, const uint8_t* buffer, size_t length)
{
#if DEBUG_ENABLED && DEBUG_MODBUS_ENABLED
    extern int currentLogLevel;
    if (currentLogLevel >= LOG_LEVEL_DEBUG && length > 0)
    {
        Serial.print(prefix);
        for (size_t i = 0; i < length; i++)
        {
            if (buffer[i] < 0x10) Serial.print("0");
            Serial.print(buffer[i], HEX);
            Serial.print(" ");
        }
        Serial.println();
    }
#endif
}

/**
 * @brief Отладочная печать состояния системы (компактная)
 * @param module Название модуля
 * @param status Статус (true/false)
 * @param details Дополнительная информация
 */
inline void debugPrintStatus(const char* module, bool status, const char* details = nullptr)
{
#if DEBUG_ENABLED
    Serial.printf("[%s] %s", module, status ? "✅" : "❌");
    if (details)
    {
        Serial.printf(" - %s", details);
    }
    Serial.println();
#endif
}

// ============================================================================
// МАКРОСЫ ДЛЯ ЗАМЕНЫ СТАРЫХ DEBUG ВЫЗОВОВ
// ============================================================================

// Замена для избыточных DEBUG_PRINTF в mqtt_client.cpp
#define DEBUG_MQTT_DELTA(fmt, ...) DEBUG_MQTT_PRINTF("[DELTA] " fmt "\n", ##__VA_ARGS__)
#define DEBUG_MQTT_DNS(fmt, ...) DEBUG_MQTT_PRINTF("[DNS] " fmt "\n", ##__VA_ARGS__)
#define DEBUG_MQTT_HA(fmt, ...) DEBUG_MQTT_PRINTF("[HA] " fmt "\n", ##__VA_ARGS__)

// Замена для избыточных DEBUG в modbus_sensor.cpp
#define DEBUG_MODBUS_TX() DEBUG_MODBUS_PRINTLN("TX режим")
#define DEBUG_MODBUS_RX() DEBUG_MODBUS_PRINTLN("RX режим")
#define DEBUG_MODBUS_MOVING_AVG(fmt, ...) DEBUG_MODBUS_PRINTF("[MOVING_AVG] " fmt "\n", ##__VA_ARGS__)

// Замена для main.cpp
#define DEBUG_MAIN_BATCH(msg) DEBUG_PRINTLN("[BATCH] " msg)
#define DEBUG_MAIN_BUTTON(msg) DEBUG_PRINTLN("[BUTTON] " msg)

// ============================================================================
// СТАТИСТИКА ОТЛАДКИ (для профилирования)
// ============================================================================

#if DEBUG_ENABLED
struct DebugStats
{
    unsigned long total_messages;
    unsigned long modbus_messages;
    unsigned long mqtt_messages;
    unsigned long wifi_messages;
    unsigned long start_time;
};

extern DebugStats debug_stats;

inline void debugStatsInit()
{
    debug_stats = {0, 0, 0, 0, millis()};
}

inline void debugStatsIncrement(const char* category)
{
    debug_stats.total_messages++;
    if (strstr(category, "MODBUS"))
        debug_stats.modbus_messages++;
    else if (strstr(category, "MQTT"))
        debug_stats.mqtt_messages++;
    else if (strstr(category, "WIFI"))
        debug_stats.wifi_messages++;
}

inline void debugStatsPrint()
{
    unsigned long uptime = millis() - debug_stats.start_time;
    Serial.printf("\n=== DEBUG СТАТИСТИКА ===\n");
    Serial.printf("Время работы: %lu мс\n", uptime);
    Serial.printf("Всего сообщений: %lu\n", debug_stats.total_messages);
    Serial.printf("MODBUS: %lu, MQTT: %lu, WIFI: %lu\n", debug_stats.modbus_messages, debug_stats.mqtt_messages,
                  debug_stats.wifi_messages);
    Serial.printf("Частота: %.2f сообщений/сек\n", debug_stats.total_messages * 1000.0 / uptime);
    Serial.printf("========================\n\n");
}
#else
inline void debugStatsInit() {}
inline void debugStatsIncrement(const char* category) {}
inline void debugStatsPrint() {}
#endif

// ============================================================================
// МИГРАЦИОННЫЕ МАКРОСЫ (для постепенного перехода)
// ============================================================================

// Эти макросы помогают постепенно мигрировать с старых DEBUG_PRINTF на новые
#define MIGRATE_DEBUG_PRINTF(fmt, ...) DEBUG_PRINTF(fmt, ##__VA_ARGS__)
#define MIGRATE_DEBUG_PRINTLN(msg) DEBUG_PRINTLN(msg)

// Макросы для критически важных отладочных сообщений (всегда включены)
#define CRITICAL_DEBUG_PRINT(x)  \
    Serial.print("[CRITICAL] "); \
    Serial.print(x)
#define CRITICAL_DEBUG_PRINTLN(x) \
    Serial.print("[CRITICAL] ");  \
    Serial.println(x)
#define CRITICAL_DEBUG_PRINTF(fmt, ...) Serial.printf("[CRITICAL] " fmt, ##__VA_ARGS__)