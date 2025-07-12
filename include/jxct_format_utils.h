#pragma once
#ifdef TEST_BUILD
#include "../test/stubs/esp32_stubs.h"
#else
#include "Arduino.h"
#endif
#include <string>

// Универсальная функция форматирования float
std::string formatFloat(float value, int precision, bool asInt = false);

std::string format_moisture(float value);     // "65.8"
std::string format_temperature(float value);  // "23.4"
std::string format_ec(float value);           // "1234"
std::string format_ph(float value);           // "6.7"
std::string format_npk(float value);          // "123"

// Универсальная функция форматирования для веб-интерфейса
String formatValue(float value, const char* unit, int precision = 2);
