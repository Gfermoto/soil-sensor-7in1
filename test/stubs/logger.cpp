/**
 * @file logger.cpp
 * @brief Stub реализация логгера для тестирования
 */

#include "../../include/logger.h"
#include <iostream>
#include <string>

LogLevel currentLogLevel = LOG_DEBUG;

// Основные функции логгирования (безопасные альтернативы)
void logError(const char* message)
{
    if (currentLogLevel >= LOG_ERROR)
    {
        std::cout << "[ERROR] " << message << std::endl;
    }
}

void logError(const String& message)
{
    logError(message.c_str());
}

void logWarn(const char* message)
{
    if (currentLogLevel >= LOG_WARN)
    {
        std::cout << "[WARN] " << message << std::endl;
    }
}

void logWarn(const String& message)
{
    logWarn(message.c_str());
}

void logInfo(const char* message)
{
    if (currentLogLevel >= LOG_INFO)
    {
        std::cout << "[INFO] " << message << std::endl;
    }
}

void logInfo(const String& message)
{
    logInfo(message.c_str());
}

void logDebug(const char* message)
{
    if (currentLogLevel >= LOG_DEBUG)
    {
        std::cout << "[DEBUG] " << message << std::endl;
    }
}

void logDebug(const String& message)
{
    logDebug(message.c_str());
}

void logSuccess(const char* message)
{
    std::cout << "[SUCCESS] " << message << std::endl;
}

void logSuccess(const String& message)
{
    logSuccess(message.c_str());
}

// Специализированные функции (безопасные альтернативы)
void logSensor(const char* message)
{
    if (currentLogLevel >= LOG_INFO)
    {
        std::cout << "[SENSOR] " << message << std::endl;
    }
}

void logSensor(const String& message)
{
    logSensor(message.c_str());
}

void logWiFi(const char* message)
{
    if (currentLogLevel >= LOG_INFO)
    {
        std::cout << "[WiFi] " << message << std::endl;
    }
}

void logWiFi(const String& message)
{
    logWiFi(message.c_str());
}

void logMQTT(const char* message)
{
    if (currentLogLevel >= LOG_INFO)
    {
        std::cout << "[MQTT] " << message << std::endl;
    }
}

void logMQTT(const String& message)
{
    logMQTT(message.c_str());
}

void logData(const char* message)
{
    if (currentLogLevel >= LOG_INFO)
    {
        std::cout << "[DATA] " << message << std::endl;
    }
}

void logData(const String& message)
{
    logData(message.c_str());
}

void logSystem(const char* message)
{
    if (currentLogLevel >= LOG_INFO)
    {
        std::cout << "[SYSTEM] " << message << std::endl;
    }
}

void logSystem(const String& message)
{
    logSystem(message.c_str());
}

// Остальные функции (заглушки)
void logPrintHeader(const char* title, LogColor color)
{
    std::cout << "=== " << title << " ===" << std::endl;
}

void logPrintSeparator(const char* symbol, int length)
{
    for (int i = 0; i < length; i++)
    {
        std::cout << symbol;
    }
    std::cout << std::endl;
}

void logPrintBanner(const char* text)
{
    std::cout << "*** " << text << " ***" << std::endl;
}

void logUptime()
{
    std::cout << "[UPTIME] System uptime: 123 seconds" << std::endl;
}

void logMemoryUsage()
{
    std::cout << "[MEMORY] Free heap: 64KB" << std::endl;
}

void logWiFiStatus()
{
    std::cout << "[WiFi] Status: Connected" << std::endl;
}
