/**
 * @file logger.h
 * @brief Система логгирования с красивым форматированием
 */

#ifndef LOGGER_H
#define LOGGER_H

#include <Arduino.h>

// Уровни логгирования
enum LogLevel : std::uint8_t
{
    LOG_ERROR = 0,
    LOG_WARN = 1,
    LOG_INFO = 2,
    LOG_DEBUG = 3
};

// Цвета для заголовков логгирования
enum class LogColor : std::uint8_t
{
    RED = 0,
    GREEN = 1,
    YELLOW = 2,
    BLUE = 3,
    MAGENTA = 4,
    CYAN = 5,
    WHITE = 6,
    CYAN_DEFAULT = 7  // Уникальное значение
};

// Текущий уровень логгирования (можно менять)
extern LogLevel currentLogLevel;

// Символы для разных типов сообщений
#define LOG_SYMBOL_ERROR "❌"
#define LOG_SYMBOL_WARN "⚠️ "
#define LOG_SYMBOL_INFO "ℹ️ "
#define LOG_SYMBOL_DEBUG "🔧"
#define LOG_SYMBOL_SUCCESS "✅"
#define LOG_SYMBOL_SENSOR "📡"
#define LOG_SYMBOL_WIFI "📶"
#define LOG_SYMBOL_MQTT "🌐"
#define LOG_SYMBOL_HTTP "🌍"

// Цвета ANSI для терминала
#define COLOR_RED "\033[31m"
#define COLOR_GREEN "\033[32m"
#define COLOR_YELLOW "\033[33m"
#define COLOR_BLUE "\033[34m"
#define COLOR_MAGENTA "\033[35m"
#define COLOR_CYAN "\033[36m"
#define COLOR_WHITE "\033[37m"
#define COLOR_RESET "\033[0m"

// Безопасные helper функции для форматирования
String formatLogMessage(const char* format, ...);
String formatLogMessage(const String& message);

// Основные функции логгирования (String версии)
void logError(const String& message);
void logWarn(const String& message);
void logInfo(const String& message);
void logDebug(const String& message);
void logSuccess(const String& message);
void logSensor(const String& message);
void logWiFi(const String& message);
void logMQTT(const String& message);
void logHTTP(const String& message);
void logSystem(const String& message);
void logData(const String& message);

// Безопасные variadic функции (для обратной совместимости)
void logError(const char* format, ...) __attribute__((format(printf, 1, 2)));
void logWarn(const char* format, ...) __attribute__((format(printf, 1, 2)));
void logInfo(const char* format, ...) __attribute__((format(printf, 1, 2)));
void logDebug(const char* format, ...) __attribute__((format(printf, 1, 2)));
void logSuccess(const char* format, ...) __attribute__((format(printf, 1, 2)));
void logSensor(const char* format, ...) __attribute__((format(printf, 1, 2)));
void logWiFi(const char* format, ...) __attribute__((format(printf, 1, 2)));
void logMQTT(const char* format, ...) __attribute__((format(printf, 1, 2)));
void logHTTP(const char* format, ...) __attribute__((format(printf, 1, 2)));
void logSystem(const char* format, ...) __attribute__((format(printf, 1, 2)));
void logData(const char* format, ...) __attribute__((format(printf, 1, 2)));

// Специальные функции
void logSeparator();
void logNewline();
void logMemoryUsage();
void logSystemInfo();
void logUptime();
void logWiFiStatus();

// Функции для работы с цветами
void setLogColor(LogColor color);
void resetLogColor();
const char* getColorCode(LogColor color);

// Утилиты
void printHeader(const String& title, LogColor color = LogColor::CYAN_DEFAULT);
void printSubHeader(const String& title, LogColor color = LogColor::BLUE);
void printTimeStamp();

// Алиасы для обратной совместимости
#define logPrintHeader printHeader
#define logPrintSeparator(symbol, length) logSeparator()
#define logPrintBanner(text) printHeader(String(text), LogColor::CYAN)

#endif // LOGGER_H
