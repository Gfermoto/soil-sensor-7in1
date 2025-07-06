/**
 * @file logger.cpp
 * @brief Реализация системы логгирования
 */

#include "../include/logger.h"
#include <cstdarg>
#include <cstdio>
#include <WiFi.h>

// Глобальная переменная для уровня логгирования
LogLevel currentLogLevel = LOG_DEBUG;

// Получение времени работы в читаемом формате
String getUptimeString()
{
    unsigned long milliseconds = millis();
    unsigned long seconds = milliseconds / 1000;
    unsigned long minutes = seconds / 60;
    unsigned long hours = minutes / 60;
    unsigned long days = hours / 24;

    seconds %= 60;
    minutes %= 60;
    hours %= 24;

    return String(days) + "д " + String(hours) + "ч " + String(minutes) + "м " + String(seconds) + "с";
}

// Безопасные helper функции для форматирования
String formatLogMessage(const char* format, ...)
{
    std::va_list args;
    va_start(args, format);
    
    // Буфер на стеке для небольших сообщений
    char buffer[512];
    int result = vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    
    if (result < 0) {
        return String("LOG FORMAT ERROR");
    }
    
    if (result < static_cast<int>(sizeof(buffer))) {
        return String(buffer);
    }
    
    // Если сообщение слишком длинное, обрезаем
    buffer[sizeof(buffer) - 1] = '\0';
    return String(buffer);
}

String formatLogMessage(const String& message)
{
    return message;
}

// Основные функции логгирования (String версии)
void logError(const String& message)
{
    if (currentLogLevel < LOG_ERROR)
    {
        return;
    }

    Serial.print(COLOR_RED);
    Serial.print(LOG_SYMBOL_ERROR " ");
    Serial.print(COLOR_RESET);
    Serial.println(message);
}

void logWarn(const String& message)
{
    if (currentLogLevel < LOG_WARN)
    {
        return;
    }

    Serial.print(COLOR_YELLOW);
    Serial.print(LOG_SYMBOL_WARN);
    Serial.print(COLOR_RESET);
    Serial.println(message);
}

void logInfo(const String& message)
{
    if (currentLogLevel < LOG_INFO)
    {
        return;
    }

    Serial.print(COLOR_BLUE);
    Serial.print(LOG_SYMBOL_INFO);
    Serial.print(COLOR_RESET);
    Serial.println(message);
}

void logDebug(const String& message)
{
    if (currentLogLevel < LOG_DEBUG)
    {
        return;
    }

    Serial.print(COLOR_CYAN);
    Serial.print(LOG_SYMBOL_DEBUG);
    Serial.print(COLOR_RESET);
    Serial.println(message);
}

void logSuccess(const String& message)
{
    if (currentLogLevel < LOG_INFO)
    {
        return;
    }

    Serial.print(COLOR_GREEN);
    Serial.print(LOG_SYMBOL_SUCCESS);
    Serial.print(COLOR_RESET);
    Serial.println(message);
}

void logSensor(const String& message)
{
    if (currentLogLevel < LOG_INFO)
    {
        return;
    }

    Serial.print(COLOR_MAGENTA);
    Serial.print(LOG_SYMBOL_SENSOR);
    Serial.print(COLOR_RESET);
    Serial.println(message);
}

void logWiFi(const String& message)
{
    if (currentLogLevel < LOG_INFO)
    {
        return;
    }

    Serial.print(COLOR_CYAN);
    Serial.print(LOG_SYMBOL_WIFI);
    Serial.print(COLOR_RESET);
    Serial.println(message);
}

void logMQTT(const String& message)
{
    if (currentLogLevel < LOG_INFO)
    {
        return;
    }

    Serial.print(COLOR_BLUE);
    Serial.print(LOG_SYMBOL_MQTT);
    Serial.print(COLOR_RESET);
    Serial.println(message);
}

void logHTTP(const String& message)
{
    if (currentLogLevel < LOG_INFO)
    {
        return;
    }

    Serial.print(COLOR_GREEN);
    Serial.print(LOG_SYMBOL_HTTP);
    Serial.print(COLOR_RESET);
    Serial.println(message);
}

void logSystem(const String& message)
{
    if (currentLogLevel < LOG_INFO)
    {
        return;
    }

    Serial.print(COLOR_WHITE);
    Serial.print("⚙️  ");
    Serial.print(COLOR_RESET);
    Serial.println(message);
}

void logData(const String& message)
{
    if (currentLogLevel < LOG_INFO)
    {
        return;
    }

    Serial.print(COLOR_YELLOW);
    Serial.print("📊 ");
    Serial.print(COLOR_RESET);
    Serial.println(message);
}

// Безопасные variadic функции (для обратной совместимости)
void logError(const char* format, ...)
{
    std::va_list args;
    va_start(args, format);
    
    char buffer[512];
    int result = vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    
    if (result >= 0) {
        logError(String(buffer));
    } else {
        logError(String("LOG FORMAT ERROR"));
    }
}

void logWarn(const char* format, ...)
{
    std::va_list args;
    va_start(args, format);
    
    char buffer[512];
    int result = vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    
    if (result >= 0) {
        logWarn(String(buffer));
    } else {
        logWarn(String("LOG FORMAT ERROR"));
    }
}

void logInfo(const char* format, ...)
{
    std::va_list args;
    va_start(args, format);
    
    char buffer[512];
    int result = vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    
    if (result >= 0) {
        logInfo(String(buffer));
    } else {
        logInfo(String("LOG FORMAT ERROR"));
    }
}

void logDebug(const char* format, ...)
{
    std::va_list args;
    va_start(args, format);
    
    char buffer[512];
    int result = vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    
    if (result >= 0) {
        logDebug(String(buffer));
    } else {
        logDebug(String("LOG FORMAT ERROR"));
    }
}

void logSuccess(const char* format, ...)
{
    std::va_list args;
    va_start(args, format);
    
    char buffer[512];
    int result = vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    
    if (result >= 0) {
        logSuccess(String(buffer));
    } else {
        logSuccess(String("LOG FORMAT ERROR"));
    }
}

void logSensor(const char* format, ...)
{
    std::va_list args;
    va_start(args, format);
    
    char buffer[512];
    int result = vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    
    if (result >= 0) {
        logSensor(String(buffer));
    } else {
        logSensor(String("LOG FORMAT ERROR"));
    }
}

void logWiFi(const char* format, ...)
{
    std::va_list args;
    va_start(args, format);
    
    char buffer[512];
    int result = vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    
    if (result >= 0) {
        logWiFi(String(buffer));
    } else {
        logWiFi(String("LOG FORMAT ERROR"));
    }
}

void logMQTT(const char* format, ...)
{
    std::va_list args;
    va_start(args, format);
    
    char buffer[512];
    int result = vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    
    if (result >= 0) {
        logMQTT(String(buffer));
    } else {
        logMQTT(String("LOG FORMAT ERROR"));
    }
}

void logHTTP(const char* format, ...)
{
    std::va_list args;
    va_start(args, format);
    
    char buffer[512];
    int result = vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    
    if (result >= 0) {
        logHTTP(String(buffer));
    } else {
        logHTTP(String("LOG FORMAT ERROR"));
    }
}

void logSystem(const char* format, ...)
{
    std::va_list args;
    va_start(args, format);
    
    char buffer[512];
    int result = vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    
    if (result >= 0) {
        logSystem(String(buffer));
    } else {
        logSystem(String("LOG FORMAT ERROR"));
    }
}

void logData(const char* format, ...)
{
    std::va_list args;
    va_start(args, format);
    
    char buffer[512];
    int result = vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    
    if (result >= 0) {
        logData(String(buffer));
    } else {
        logData(String("LOG FORMAT ERROR"));
    }
}

// Специальные функции
void logSeparator()
{
    Serial.println("────────────────────────────────────────────────────");
}

void logNewline()
{
    Serial.println();
}

void logMemoryUsage()
{
    if (currentLogLevel < LOG_DEBUG)
    {
        return;
    }

    Serial.print(COLOR_CYAN);
    Serial.print("🔧 Free heap: ");
    Serial.print(ESP.getFreeHeap());
    Serial.println(" bytes");
    Serial.print(COLOR_RESET);
}

void logSystemInfo()
{
    if (currentLogLevel < LOG_INFO)
    {
        return;
    }

    Serial.print(COLOR_BLUE);
    Serial.print("ℹ️  ESP32 Chip ID: ");
    Serial.println(ESP.getChipModel());
    Serial.print(COLOR_RESET);
}

void logUptime()
{
    logSystem("Время работы: %s", getUptimeString().c_str());
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

// Функции для работы с цветами
void setLogColor(LogColor color)
{
    Serial.print(getColorCode(color));
}

void resetLogColor()
{
    Serial.print(COLOR_RESET);
}

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
        default: return COLOR_RESET;
    }
}

// Утилиты
void printHeader(const String& title, LogColor color)
{
    Serial.print(getColorCode(color));
    Serial.print("═══ ");
    Serial.print(title);
    Serial.println(" ═══");
    Serial.print(COLOR_RESET);
}

void printSubHeader(const String& title, LogColor color)
{
    Serial.print(getColorCode(color));
    Serial.print("─── ");
    Serial.print(title);
    Serial.println(" ───");
    Serial.print(COLOR_RESET);
}

void printTimeStamp()
{
    Serial.print("[");
    Serial.print(millis());
    Serial.print("] ");
}
