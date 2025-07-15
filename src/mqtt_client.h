#ifndef MQTT_CLIENT_H
#define MQTT_CLIENT_H

#ifdef TEST_BUILD
#include "esp32_stubs.h"
#elif defined(ESP32) || defined(ARDUINO)
#include <ArduinoJson.h>
#include <PubSubClient.h>
#include <WiFi.h>
#include "Arduino.h"
#else
#include "esp32_stubs.h"
#endif

// Функции доступа к MQTT клиентам
extern WiFiClient espClient;
extern PubSubClient mqttClient;
extern bool mqttConnected;

// ✅ Заменяем String на функцию-геттер для совместимости
const char* getMqttLastError();

// Инициализация MQTT клиента
void setupMQTT();

// Подключение к MQTT брокеру
bool connectMQTT();

// Обработка MQTT (вызывать в loop)
void handleMQTT();

// Публикация данных с датчика
void publishSensorData();

// Публикация конфигурации для Home Assistant
void publishHomeAssistantConfig();

// Публикация статуса устройства (online/offline)
void publishAvailability(bool online);

// Удаление discovery-конфигов Home Assistant
void removeHomeAssistantConfig();

// ✅ НОВОЕ: Инвалидация кэша Home Assistant конфигураций
void invalidateHAConfigCache();

// Обработка команд из MQTT
void handleMqttCommand(const String& cmd);

// Обработчик входящих MQTT сообщений
void mqttCallback(const char* topic, const byte* payload, unsigned int length);

#endif  // MQTT_CLIENT_H
