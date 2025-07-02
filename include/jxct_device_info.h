#pragma once

// Используем централизованное управление версией
#include "version.h"

// Firmware version constant (workaround for JXCT_VERSION macro issue)
extern const char* FIRMWARE_VERSION;

#include <Arduino.h>

String getDeviceId();
String getDefaultTopic();
