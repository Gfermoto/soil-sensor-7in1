#pragma once

// ============================================================================
// ЦЕНТРАЛИЗОВАННОЕ УПРАВЛЕНИЕ ВЕРСИЕЙ JXCT
// ============================================================================
// Изменяйте версию ТОЛЬКО здесь - она автоматически обновится везде!

#define JXCT_VERSION_MAJOR 2
#define JXCT_VERSION_MINOR 4
#define JXCT_VERSION_PATCH 5

// Автоматическая генерация строки версии
#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#define JXCT_VERSION_STRING TOSTRING(JXCT_VERSION_MAJOR) "." TOSTRING(JXCT_VERSION_MINOR) "." TOSTRING(JXCT_VERSION_PATCH)

// Константы устройства
static const char DEVICE_MANUFACTURER[] = "Eyera";
static const char DEVICE_MODEL[] = "JXCT-7in1";
static const char DEVICE_SW_VERSION[] = JXCT_VERSION_STRING;

// Совместимость с существующим кодом
static const char* FIRMWARE_VERSION = JXCT_VERSION_STRING;

// Макросы для сравнения версий
#define JXCT_VERSION_CODE (JXCT_VERSION_MAJOR * 10000 + JXCT_VERSION_MINOR * 100 + JXCT_VERSION_PATCH)
#define JXCT_VERSION_AT_LEAST(major, minor, patch) (JXCT_VERSION_CODE >= ((major) * 10000 + (minor) * 100 + (patch)))

// Информация о сборке
#ifndef JXCT_BUILD_DATE
#define JXCT_BUILD_DATE __DATE__
#endif
#ifndef JXCT_BUILD_TIME
#define JXCT_BUILD_TIME __TIME__
#endif
#define JXCT_FULL_VERSION_STRING JXCT_VERSION_STRING " (built " JXCT_BUILD_DATE " " JXCT_BUILD_TIME ")" 