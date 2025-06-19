#pragma once

#include <WiFiClient.h>

// OTA 2.0 – безопасное HTTP-обновление с проверкой подписи и откатом.
// Публичные функции вызываются из main.cpp.

// Инициализация OTA менеджера. manifestUrl должен вести на JSON вида
// {"version":"2.7.1","url":"https://.../fw.bin","sha256":"<64-hex>"}
void setupOTA(const char* manifestUrl, WiFiClient& client);

// Вызывать в loop(). Самостоятельно обеспечивает не чаще 1р/час.
void handleOTA();

// Принудительный запуск проверки (сброс таймера)
void triggerOtaCheck();

// Принудительная установка найденного обновления
void triggerOtaInstall();

// Текстовый статус для UI / MQTT
const char* getOtaStatus(); 