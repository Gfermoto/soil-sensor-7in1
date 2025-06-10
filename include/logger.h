/**
 * @file logger.h
 * @brief Система логгирования с красивым форматированием
 */

#ifndef LOGGER_H
#define LOGGER_H

#include <Arduino.h>

// Уровни логгирования
enum LogLevel {
    LOG_ERROR = 0,
    LOG_WARN = 1,
    LOG_INFO = 2,
    LOG_DEBUG = 3
};

// Текущий уровень логгирования (можно менять)
extern LogLevel currentLogLevel;

// Символы для разных типов сообщений
#define LOG_SYMBOL_ERROR "❌"
#define LOG_SYMBOL_WARN  "⚠️ "
#define LOG_SYMBOL_INFO  "ℹ️ "
#define LOG_SYMBOL_DEBUG "🔧"
#define LOG_SYMBOL_SUCCESS "✅"
#define LOG_SYMBOL_SENSOR "📡"
#define LOG_SYMBOL_WIFI "📶"
#define LOG_SYMBOL_MQTT "📤"
#define LOG_SYMBOL_DATA "📊"
#define LOG_SYMBOL_SYSTEM "⚙️ "

// Цвета для консоли (ANSI escape codes)
#define COLOR_RESET   "\033[0m"
#define COLOR_RED     "\033[31m"
#define COLOR_GREEN   "\033[32m"
#define COLOR_YELLOW  "\033[33m"
#define COLOR_BLUE    "\033[34m"
#define COLOR_MAGENTA "\033[35m"
#define COLOR_CYAN    "\033[36m"
#define COLOR_WHITE   "\033[37m"
#define COLOR_BOLD    "\033[1m"

// Функции логгирования
void logPrintHeader(const char* title, const char* color = COLOR_CYAN);
void logPrintSeparator(const char* symbol = "─", int length = 50);
void logPrintBanner(const char* text);

// Основные функции логгирования
void logError(const char* format, ...);
void logWarn(const char* format, ...);
void logInfo(const char* format, ...);
void logDebug(const char* format, ...);
void logSuccess(const char* format, ...);

// Специализированные функции
void logSensor(const char* format, ...);
void logWiFi(const char* format, ...);
void logMQTT(const char* format, ...);
void logData(const char* format, ...);
void logSystem(const char* format, ...);

// Утилиты
void logUptime();
void logMemoryUsage();
void logWiFiStatus();

#endif // LOGGER_H 