#pragma once

static const char DEVICE_MANUFACTURER[] = "Eyera";
static const char DEVICE_MODEL[] = "JXCT-7in1";
static const char DEVICE_SW_VERSION[] = "2.0";

#include <Arduino.h>

String getDeviceId();
String getDefaultTopic();