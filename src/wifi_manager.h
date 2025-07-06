#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <Arduino.h>
#include <DNSServer.h>
#include <Preferences.h>
#include <WebServer.h>
#include <WiFi.h>

// Перечисление для режимов WiFi
enum class WiFiMode : std::uint8_t
{
    AP,  // Режим точки доступа
    STA  // Режим клиента
};

// Глобальные переменные
extern bool wifiConnected;
extern WiFiMode currentWiFiMode;

// Пин светодиода статуса
#define STATUS_LED_PIN 2

// Функции для управления светодиодом
void setLedOn();
void setLedOff();
void setLedBlink(unsigned long interval);
void setLedFastBlink();
void updateLed();

// Объявление функции настройки web-сервера
void setupWebServer();

// Инициализация WiFi
void setupWiFi();

// Обработка WiFi
void handleWiFi();

// Запуск режима точки доступа
void startAPMode();

// Запуск режима клиента
void startSTAMode();

// Проверка кнопки сброса
bool checkResetButton();

// Сброс конфигурации
void resetConfig();

// Перезапуск ESP32
void restartESP();

// Парсинг и применение конфигурации из JSON
bool parseAndApplyConfig(const String& jsonContent, String& error);

// HTML генераторы
String navHtml();
String getApSsid();

#endif  // WIFI_MANAGER_H
