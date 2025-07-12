/**
 * @file csrf_protection.h
 * @brief Заголовочный файл для CSRF защиты
 * @date 2025-01-22
 * @author AI Assistant
 */

#ifndef CSRF_PROTECTION_H
#define CSRF_PROTECTION_H

#ifdef TEST_BUILD
#include "esp32_stubs.h"
#elif defined(ESP32) || defined(ARDUINO)
#include "Arduino.h"
#include <WebServer.h>
#else
#include "esp32_stubs.h"
#endif

// Объявление webServer (определен в wifi_manager.cpp)
extern WebServer webServer;

// Функции CSRF защиты
String generateCSRFToken();
bool validateCSRFToken(const String& token);
String getCSRFHiddenField();
bool checkCSRFSafety();
void initCSRFProtection();

// Вспомогательные функции
String methodToString(HTTPMethod method);

#endif  // CSRF_PROTECTION_H
