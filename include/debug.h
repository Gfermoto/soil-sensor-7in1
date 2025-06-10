#ifndef DEBUG_H
#define DEBUG_H

#include <Arduino.h>

// Условная компиляция отладки
// В релизной сборке весь отладочный код будет исключен из прошивки
#ifdef DEBUG_MODE
  #define DEBUG_PRINT(x) Serial.print(x)
  #define DEBUG_PRINTLN(x) Serial.println(x)
  #define DEBUG_PRINTF(fmt, ...) Serial.printf(fmt, ##__VA_ARGS__)
  #define DEBUG_PRINT_VAR(name, value) Serial.printf("[DEBUG] %s = %s\n", name, String(value).c_str())
  #define DEBUG_PRINT_INT(name, value) Serial.printf("[DEBUG] %s = %d\n", name, value)
  #define DEBUG_PRINT_FLOAT(name, value) Serial.printf("[DEBUG] %s = %.2f\n", name, value)
#else
  // В релизной сборке эти макросы превращаются в пустые операции
  #define DEBUG_PRINT(x)
  #define DEBUG_PRINTLN(x)
  #define DEBUG_PRINTF(fmt, ...)
  #define DEBUG_PRINT_VAR(name, value)
  #define DEBUG_PRINT_INT(name, value)
  #define DEBUG_PRINT_FLOAT(name, value)
#endif

// Критические ошибки всегда выводятся (для отладки в продакшне)
#define ERROR_PRINT(x) Serial.print(x)
#define ERROR_PRINTLN(x) Serial.println(x)
#define ERROR_PRINTF(fmt, ...) Serial.printf(fmt, ##__VA_ARGS__)

// Информационные сообщения (можно отключить в критических случаях)
#ifdef INFO_MODE
  #define INFO_PRINT(x) Serial.print(x)
  #define INFO_PRINTLN(x) Serial.println(x)
  #define INFO_PRINTF(fmt, ...) Serial.printf(fmt, ##__VA_ARGS__)
#else
  #define INFO_PRINT(x)
  #define INFO_PRINTLN(x)
  #define INFO_PRINTF(fmt, ...)
#endif

#endif // DEBUG_H 