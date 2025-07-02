#include "logger.h"
#include <cstdarg>
#include <cstdio>

LogLevel currentLogLevel = LOG_INFO;

static void noop(const char* /*format*/, ...) {}

void logPrintHeader(const char* title, const char* color)
{
    (void)title;
    (void)color;
}
void logPrintSeparator(const char* symbol, int length)
{
    (void)symbol;
    (void)length;
}
void logPrintBanner(const char* text)
{
    (void)text;
}

void logError(const char* format, ...)
{
    (void)format;
}
void logWarn(const char* format, ...)
{
    (void)format;
}
void logInfo(const char* format, ...)
{
    (void)format;
}
void logDebug(const char* format, ...)
{
    (void)format;
}
void logSuccess(const char* format, ...)
{
    (void)format;
}

void logSensor(const char* format, ...)
{
    (void)format;
}
void logWiFi(const char* format, ...)
{
    (void)format;
}
void logMQTT(const char* format, ...)
{
    (void)format;
}
void logData(const char* format, ...)
{
    (void)format;
}
void logSystem(const char* format, ...)
{
    (void)format;
}

void logUptime() {}
void logMemoryUsage() {}
void logWiFiStatus() {}
