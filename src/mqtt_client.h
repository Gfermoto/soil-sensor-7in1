#ifndef MQTT_CLIENT_H
#define MQTT_CLIENT_H

#include <Arduino.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

extern WiFiClient espClient;
extern PubSubClient mqttClient;
extern bool mqttConnected;
extern String mqttLastError;

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

// Обработка команд из MQTT
void handleMqttCommand(const String& cmd);

// Обработчик входящих MQTT сообщений
void mqttCallback(char* topic, byte* payload, unsigned int length);

#endif  // MQTT_CLIENT_H