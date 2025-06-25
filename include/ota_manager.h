#pragma once

#include <WiFiClient.h>

// Часовой для обнаружения перезаписи памяти
void checkGuard(const char* tag);

// Основные функции OTA-менеджера
const char* getOtaStatus();
void setupOTA(const char* manifestUrl, WiFiClient& client);
void triggerOtaCheck();    // только проверка манифеста
void triggerOtaInstall();  // немедленная установка доступного обновления
void handleOTA();          // периодическая проверка (авто-OTA)