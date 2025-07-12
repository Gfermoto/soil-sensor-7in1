#pragma once

// Используем централизованное управление версией
#include "version.h"

// Firmware version constant (workaround for JXCT_VERSION macro issue)
extern const char* FIRMWARE_VERSION;

#ifdef TEST_BUILD
#include "../test/stubs/esp32_stubs.h"
#else
#include <Arduino.h>
#endif

String getDeviceId();
String getDefaultTopic();
