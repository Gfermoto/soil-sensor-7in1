/**
 * @file logger.cpp
 * @brief Реализация системы логгирования
 */

#include "logger.h"
#include <Arduino.h>
#include <WiFi.h>
#include <cstdarg>

// Глобальная переменная уровня логгирования
LogLevel currentLogLevel = LOG_INFO;

// Получение времени работы в читаемом формате
String getUptimeString()
{
    unsigned long totalSeconds = millis() / 1000;
    unsigned long hours = totalSeconds / 3600;
    unsigned long minutes = (totalSeconds % 3600) / 60;
    unsigned long seconds = totalSeconds % 60;

    return String(hours) + ":" + (minutes < 10 ? "0" : "") + String(minutes) + ":" + (seconds < 10 ? "0" : "") +
           String(seconds);
}

// Функция для получения ANSI цвета из enum
const char* getColorCode(LogColor color)
{
    switch (color)
    {
        case LogColor::RED: return COLOR_RED;
        case LogColor::GREEN: return COLOR_GREEN;
        case LogColor::YELLOW: return COLOR_YELLOW;
        case LogColor::BLUE: return COLOR_BLUE;
        case LogColor::MAGENTA: return COLOR_MAGENTA;
        case LogColor::CYAN: return COLOR_CYAN;
        case LogColor::WHITE: return COLOR_WHITE;
        case LogColor::CYAN_DEFAULT: return COLOR_CYAN;
        default: return COLOR_CYAN;
    }
}

// Вывод заголовка секции
void logPrintHeader(const char* title, LogColor color)
{
    Serial.println();
    Serial.print(getColorCode(color));
    Serial.print(COLOR_BOLD);
    logPrintSeparator("═", 60);
    Serial.printf("  %s\n", title);
    logPrintSeparator("═", 60);
    Serial.print(COLOR_RESET);
}

// Вывод разделителя
void logPrintSeparator(const char* symbol, int length)
{
    for (int i = 0; i < length; i++)
    {
        Serial.print(symbol);
    }
    Serial.println();
}

// Вывод баннера
void logPrintBanner(const char* text)
{
    Serial.println();
    Serial.print(COLOR_CYAN);
    Serial.print(COLOR_BOLD);
    logPrintSeparator("*", 60);
    Serial.printf("  %s\n", text);
    logPrintSeparator("*", 60);
    Serial.print(COLOR_RESET);
    Serial.println();
}

// Основные функции логгирования
void logError(const char* format, ...)
{
    if (currentLogLevel < LOG_ERROR) {
        return;
    }

    Serial.print(COLOR_RED);
    Serial.print(LOG_SYMBOL_ERROR " ");
    Serial.print(COLOR_RESET);

    std::va_list args;
    va_start(args, format);
    char buffer[256];
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    Serial.println(buffer);
}

void logWarn(const char* format, ...)
{
    if (currentLogLevel < LOG_WARN) {
        return;
    }

    Serial.print(COLOR_YELLOW);
    Serial.print(LOG_SYMBOL_WARN);
    Serial.print(COLOR_RESET);

    std::va_list args;
    va_start(args, format);
    char buffer[256];
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    Serial.println(buffer);
}

void logInfo(const char* format, ...)
{
    if (currentLogLevel < LOG_INFO) {
        return;
    }

    Serial.print(COLOR_BLUE);
    Serial.print(LOG_SYMBOL_INFO);
    Serial.print(COLOR_RESET);

    std::va_list args;
    va_start(args, format);
    char buffer[256];
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    Serial.println(buffer);
}

void logDebug(const char* format, ...)
{
    if (currentLogLevel < LOG_DEBUG) {
        return;
    }

    Serial.print(COLOR_MAGENTA);
    Serial.print(LOG_SYMBOL_DEBUG " ");
    Serial.print(COLOR_RESET);

    std::va_list args;
    va_start(args, format);
    char buffer[256];
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    Serial.println(buffer);
}

void logSuccess(const char* format, ...)
{
    Serial.print(COLOR_GREEN);
    Serial.print(LOG_SYMBOL_SUCCESS " ");
    Serial.print(COLOR_RESET);

    std::va_list args;
    va_start(args, format);
    char buffer[256];
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    Serial.println(buffer);
}

// Специализированные функции
void logSensor(const char* format, ...)
{
    if (currentLogLevel < LOG_INFO) {
        return;
    }

    Serial.print(COLOR_CYAN);
    Serial.print(LOG_SYMBOL_SENSOR " ");
    Serial.print(COLOR_RESET);

    std::va_list args;
    va_start(args, format);
    char buffer[256];
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    Serial.println(buffer);
}

void logWiFi(const char* format, ...)
{
    if (currentLogLevel < LOG_INFO) {
        return;
    }

    Serial.print(COLOR_GREEN);
    Serial.print(LOG_SYMBOL_WIFI " ");
    Serial.print(COLOR_RESET);

    std::va_list args;
    va_start(args, format);
    char buffer[256];
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    Serial.println(buffer);
}

void logMQTT(const char* format, ...)
{
    if (currentLogLevel < LOG_INFO) {
        return;
    }

    Serial.print(COLOR_MAGENTA);
    Serial.print(LOG_SYMBOL_MQTT " ");
    Serial.print(COLOR_RESET);

    std::va_list args;
    va_start(args, format);
    char buffer[256];
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    Serial.println(buffer);
}

void logData(const char* format, ...)
{
    if (currentLogLevel < LOG_INFO) {
        return;
    }

    Serial.print(COLOR_YELLOW);
    Serial.print(LOG_SYMBOL_DATA " ");
    Serial.print(COLOR_RESET);

    std::va_list args;
    va_start(args, format);
    char buffer[256];
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    Serial.println(buffer);
}

void logSystem(const char* format, ...)
{
    if (currentLogLevel < LOG_INFO) {
        return;
    }

    Serial.print(COLOR_WHITE);
    Serial.print(LOG_SYMBOL_SYSTEM);
    Serial.print(COLOR_RESET);

    std::va_list args;
    va_start(args, format);
    char buffer[256];
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    Serial.println(buffer);
}

// Утилиты
void logUptime()
{
    logSystem("Время работы: %s", getUptimeString().c_str());
}

void logMemoryUsage()
{
    logSystem("Свободная память: %d байт", ESP.getFreeHeap());
}

void logWiFiStatus()
{
    if (WiFi.status() == WL_CONNECTED)
    {
        logWiFi("Подключен к %s, IP: %s, RSSI: %d dBm", WiFi.SSID().c_str(), WiFi.localIP().toString().c_str(),
                WiFi.RSSI());
    }
    else
    {
        logWarn("WiFi не подключен (статус: %d)", WiFi.status());
    }
}
