/**
 * @file csrf_protection.h
 * @brief Заголовочный файл для CSRF защиты
 * @date 2025-01-22
 * @author AI Assistant
 */

#ifndef CSRF_PROTECTION_H
#define CSRF_PROTECTION_H

#include <Arduino.h>
#include <WebServer.h>

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
