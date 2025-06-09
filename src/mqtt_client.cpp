/**
 * @file mqtt_client.cpp
 * @brief Взаимодействие с MQTT-брокером и Home Assistant
 * @details Реализация подключения, публикации данных, обработки команд и интеграции с Home Assistant через discovery.
 */
#include <Arduino.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "modbus_sensor.h"
#include "wifi_manager.h"
#include <WiFiClient.h>
#include "jxct_device_info.h"
#include "jxct_config_vars.h"
#include "jxct_format_utils.h"
#include <NTPClient.h>
extern NTPClient* timeClient;

WiFiClient espClient;
PubSubClient mqttClient(espClient);
bool mqttConnected = false;
String mqttLastError = "";

// Forward declarations
void mqttCallback(char* topic, byte* payload, unsigned int length);
void publishHomeAssistantConfig();

// Создаем уникальный ID клиента на основе MAC адреса
String getClientId() {
    uint8_t mac[6];
    WiFi.macAddress(mac);
    String clientId = "JXCT_";
    for (int i = 0; i < 6; i++) {
        if (mac[i] < 0x10) {
            clientId += "0";
        }
        clientId += String(mac[i], HEX);
    }
    return clientId;
}

// Возвращает имя клиента для MQTT
String getMqttClientName() {
    if (strlen(config.mqttDeviceName) > 0) {
        return String(config.mqttDeviceName);
    } else {
        return getClientId();
    }
}

String getStatusTopic() {
    return String(config.mqttTopicPrefix) + "/status";
}

String getCommandTopic() {
    return String(config.mqttTopicPrefix) + "/command";
}

void publishAvailability(bool online) {
    String topic = getStatusTopic();
    const char* payload = online ? "online" : "offline";
    Serial.printf("[publishAvailability] Публикация статуса: %s в топик %s\n", payload, topic.c_str());
    mqttClient.publish(topic.c_str(), payload, true);
}

void setupMQTT() {
    Serial.println("[КРИТИЧЕСКАЯ ОТЛАДКА] Инициализация MQTT");
    
    // Расширенная диагностика WiFi
    Serial.println("[WiFi Debug] Статус подключения:");
    Serial.printf("WiFi статус: %d\n", WiFi.status());
    Serial.printf("IP-адрес: %s\n", WiFi.localIP().toString().c_str());
    Serial.printf("Маска подсети: %s\n", WiFi.subnetMask().toString().c_str());
    Serial.printf("Шлюз: %s\n", WiFi.gatewayIP().toString().c_str());
    
    Serial.println("[MQTT Debug] Параметры:");
    Serial.printf("MQTT включен: %d\n", config.mqttEnabled);
    Serial.printf("Сервер: %s\n", config.mqttServer);
    Serial.printf("Порт: %d\n", config.mqttPort);
    Serial.printf("Префикс топика: %s\n", config.mqttTopicPrefix);
    Serial.printf("Пользователь: %s\n", config.mqttUser);
    
    if (!config.mqttEnabled || strlen(config.mqttServer) == 0) {
        Serial.println("[ОШИБКА] MQTT не может быть инициализирован");
        return;
    }
    
    mqttClient.setServer(config.mqttServer, config.mqttPort);
    mqttClient.setCallback(mqttCallback);
    mqttClient.setKeepAlive(30);
    mqttClient.setSocketTimeout(30);
    
    Serial.println("[MQTT] Инициализация завершена");
}

bool connectMQTT() {
    Serial.println("[КРИТИЧЕСКАЯ ОТЛАДКА] Попытка подключения к MQTT");
    
    // Проверка WiFi
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("[ОШИБКА] WiFi не подключен!");
        return false;
    }
    
    // Расширенная проверка параметров
    if (strlen(config.mqttServer) == 0) {
        Serial.println("[ОШИБКА] Не указан MQTT-сервер");
        return false;
    }
    
    // Попытка подключения с максимальной детализацией
    String clientId = getMqttClientName();
    Serial.printf("[MQTT] Сервер: %s\n", config.mqttServer);
    Serial.printf("[MQTT] Порт: %d\n", config.mqttPort);
    Serial.printf("[MQTT] ID клиента: %s\n", clientId.c_str());
    Serial.printf("[MQTT] Пользователь: %s\n", config.mqttUser);
    Serial.printf("[MQTT] Пароль: %s\n", config.mqttPassword);
    
    mqttClient.setServer(config.mqttServer, config.mqttPort);
    
    // Попытка подключения с максимально подробной информацией
    bool result = mqttClient.connect(
        clientId.c_str(),
        config.mqttUser,  // может быть пустым
        config.mqttPassword,  // может быть пустым
        (String(config.mqttTopicPrefix) + "/status").c_str(),
        1,  // QoS
        true,  // retain
        "offline"  // will message
    );
    
    Serial.printf("[MQTT] Результат подключения: %d\n", result);
    
    // Расшифровка кодов состояния
    int state = mqttClient.state();
    Serial.printf("[MQTT] Состояние клиента: %d - ", state);
    switch(state) {
        case -4: Serial.println("Тайм-аут подключения"); break;
        case -3: Serial.println("Соединение потеряно"); break;
        case -2: Serial.println("Ошибка подключения"); break;
        case -1: Serial.println("Отключено"); break;
        case 0: Serial.println("Подключено"); break;
        case 1: Serial.println("Неверный протокол"); break;
        case 2: Serial.println("Неверный ID клиента"); break;
        case 3: Serial.println("Сервер недоступен"); break;
        case 4: Serial.println("Неверные учетные данные"); break;
        case 5: Serial.println("Не авторизован"); break;
        default: Serial.println("Неизвестная ошибка"); break;
    }
    
    return result;
}

void handleMQTT() {
    if (!config.mqttEnabled) {
        return;
    }
    
    if (!mqttClient.connected()) {
        static unsigned long lastReconnectAttempt = 0;
        if (millis() - lastReconnectAttempt > 5000) {
            lastReconnectAttempt = millis();
            connectMQTT();
        }
    } else {
        mqttClient.loop();
    }
}

void publishSensorData() {
    if (!config.mqttEnabled || !mqttClient.connected() || !sensorData.valid) {
        return;
    }
    
    StaticJsonDocument<512> doc;
    doc["temperature"] = round(sensorData.temperature * 10) / 10.0;
    doc["humidity"] = round(sensorData.humidity * 10) / 10.0;
    doc["ec"] = (int)round(sensorData.ec);
    doc["ph"] = round(sensorData.ph * 10) / 10.0;
    doc["nitrogen"] = (int)round(sensorData.nitrogen);
    doc["phosphorus"] = (int)round(sensorData.phosphorus);
    doc["potassium"] = (int)round(sensorData.potassium);
    doc["timestamp"] = (long)(timeClient ? timeClient->getEpochTime() : 0);
    
    String jsonString;
    serializeJson(doc, jsonString);
    
    String topic = String(config.mqttTopicPrefix) + "/state";
    bool res = mqttClient.publish(topic.c_str(), jsonString.c_str(), true);
    
    if (res) {
        mqttLastError = "";
    } else {
        mqttLastError = "Ошибка публикации MQTT";
    }
}

void publishHomeAssistantConfig() {
    Serial.println("[publishHomeAssistantConfig] Публикация discovery-конфигов Home Assistant...");
    if (!config.mqttEnabled || !mqttClient.connected() || !config.hassEnabled) {
        Serial.println("[publishHomeAssistantConfig] Условия не выполнены, публикация отменена");
        return;
    }
    String deviceId = getDeviceId();
    // device info
    StaticJsonDocument<256> deviceInfo;
    deviceInfo["identifiers"] = deviceId;
    deviceInfo["manufacturer"] = DEVICE_MANUFACTURER;
    deviceInfo["model"] = DEVICE_MODEL;
    deviceInfo["sw_version"] = DEVICE_SW_VERSION;
    deviceInfo["name"] = deviceId;
    // Температура
    StaticJsonDocument<512> tempConfig;
    tempConfig["name"] = "JXCT Temperature";
    tempConfig["device_class"] = "temperature";
    tempConfig["state_topic"] = getDefaultTopic() + "/state";
    tempConfig["unit_of_measurement"] = "°C";
    tempConfig["value_template"] = "{{ value_json.temperature }}";
    tempConfig["unique_id"] = deviceId + "_temp";
    tempConfig["availability_topic"] = getDefaultTopic() + "/status";
    tempConfig["device"] = deviceInfo;
    // Влажность
    StaticJsonDocument<512> humConfig;
    humConfig["name"] = "JXCT Humidity";
    humConfig["device_class"] = "humidity";
    humConfig["state_topic"] = getDefaultTopic() + "/state";
    humConfig["unit_of_measurement"] = "%";
    humConfig["value_template"] = "{{ value_json.humidity }}";
    humConfig["unique_id"] = deviceId + "_hum";
    humConfig["availability_topic"] = getDefaultTopic() + "/status";
    humConfig["device"] = deviceInfo;
    // EC
    StaticJsonDocument<512> ecConfig;
    ecConfig["name"] = "JXCT EC";
    ecConfig["device_class"] = "conductivity";
    ecConfig["state_topic"] = getDefaultTopic() + "/state";
    ecConfig["unit_of_measurement"] = "µS/cm";
    ecConfig["value_template"] = "{{ value_json.ec }}";
    ecConfig["unique_id"] = deviceId + "_ec";
    ecConfig["availability_topic"] = getDefaultTopic() + "/status";
    ecConfig["device"] = deviceInfo;
    // pH
    StaticJsonDocument<512> phConfig;
    phConfig["name"] = "JXCT pH";
    phConfig["device_class"] = "ph";
    phConfig["state_topic"] = getDefaultTopic() + "/state";
    phConfig["unit_of_measurement"] = "pH";
    phConfig["value_template"] = "{{ value_json.ph }}";
    phConfig["unique_id"] = deviceId + "_ph";
    phConfig["availability_topic"] = getDefaultTopic() + "/status";
    phConfig["device"] = deviceInfo;
    // NPK (раздельно)
    StaticJsonDocument<512> nitrogenConfig;
    nitrogenConfig["name"] = "JXCT Nitrogen";
    nitrogenConfig["state_topic"] = getDefaultTopic() + "/state";
    nitrogenConfig["unit_of_measurement"] = "mg/kg";
    nitrogenConfig["value_template"] = "{{ value_json.nitrogen }}";
    nitrogenConfig["unique_id"] = deviceId + "_nitrogen";
    nitrogenConfig["availability_topic"] = getDefaultTopic() + "/status";
    nitrogenConfig["device"] = deviceInfo;
    StaticJsonDocument<512> phosphorusConfig;
    phosphorusConfig["name"] = "JXCT Phosphorus";
    phosphorusConfig["state_topic"] = getDefaultTopic() + "/state";
    phosphorusConfig["unit_of_measurement"] = "mg/kg";
    phosphorusConfig["value_template"] = "{{ value_json.phosphorus }}";
    phosphorusConfig["unique_id"] = deviceId + "_phosphorus";
    phosphorusConfig["availability_topic"] = getDefaultTopic() + "/status";
    phosphorusConfig["device"] = deviceInfo;
    StaticJsonDocument<512> potassiumConfig;
    potassiumConfig["name"] = "JXCT Potassium";
    potassiumConfig["state_topic"] = getDefaultTopic() + "/state";
    potassiumConfig["unit_of_measurement"] = "mg/kg";
    potassiumConfig["value_template"] = "{{ value_json.potassium }}";
    potassiumConfig["unique_id"] = deviceId + "_potassium";
    potassiumConfig["availability_topic"] = getDefaultTopic() + "/status";
    potassiumConfig["device"] = deviceInfo;
    // Сериализуем конфигурации в строки
    String tempConfigStr, humConfigStr, ecConfigStr, phConfigStr;
    String nitrogenConfigStr, phosphorusConfigStr, potassiumConfigStr;
    serializeJson(tempConfig, tempConfigStr);
    serializeJson(humConfig, humConfigStr);
    serializeJson(ecConfig, ecConfigStr);
    serializeJson(phConfig, phConfigStr);
    serializeJson(nitrogenConfig, nitrogenConfigStr);
    serializeJson(phosphorusConfig, phosphorusConfigStr);
    serializeJson(potassiumConfig, potassiumConfigStr);
    // Публикуем конфигурации
    mqttClient.publish(("homeassistant/sensor/" + deviceId + "_temperature/config").c_str(), tempConfigStr.c_str(), true);
    mqttClient.publish(("homeassistant/sensor/" + deviceId + "_humidity/config").c_str(), humConfigStr.c_str(), true);
    mqttClient.publish(("homeassistant/sensor/" + deviceId + "_ec/config").c_str(), ecConfigStr.c_str(), true);
    mqttClient.publish(("homeassistant/sensor/" + deviceId + "_ph/config").c_str(), phConfigStr.c_str(), true);
    mqttClient.publish(("homeassistant/sensor/" + deviceId + "_nitrogen/config").c_str(), nitrogenConfigStr.c_str(), true);
    mqttClient.publish(("homeassistant/sensor/" + deviceId + "_phosphorus/config").c_str(), phosphorusConfigStr.c_str(), true);
    mqttClient.publish(("homeassistant/sensor/" + deviceId + "_potassium/config").c_str(), potassiumConfigStr.c_str(), true);
    Serial.println("[publishHomeAssistantConfig] Конфигурация Home Assistant опубликована");
    mqttLastError = "";
}

void removeHomeAssistantConfig() {
    // Публикуем пустой payload с retain для удаления сенсоров из HA
    mqttClient.publish("homeassistant/sensor/jxct_temperature/config", "", true);
    mqttClient.publish("homeassistant/sensor/jxct_humidity/config", "", true);
    mqttClient.publish("homeassistant/sensor/jxct_ec/config", "", true);
    mqttClient.publish("homeassistant/sensor/jxct_ph/config", "", true);
    mqttClient.publish("homeassistant/sensor/jxct_nitrogen/config", "", true);
    mqttClient.publish("homeassistant/sensor/jxct_phosphorus/config", "", true);
    mqttClient.publish("homeassistant/sensor/jxct_potassium/config", "", true);
    Serial.println("[MQTT] Discovery-конфиги Home Assistant удалены");
    mqttLastError = "";
}

void handleMqttCommand(const String& cmd) {
    Serial.print("[MQTT] Получена команда: ");
    Serial.println(cmd);
    if (cmd == "reboot") {
        ESP.restart();
    } else if (cmd == "reset") {
        resetConfig();
        ESP.restart();
    } else if (cmd == "publish_test") {
        publishSensorData();
    } else {
        Serial.println("[MQTT] Неизвестная команда");
    }
}

void mqttCallback(char* topic, byte* payload, unsigned int length) {
    String t = String(topic);
    String message;
    for (unsigned int i = 0; i < length; i++) message += (char)payload[i];
    Serial.printf("[mqttCallback] Получено сообщение: %s = %s\n", t.c_str(), message.c_str());
    if (t == getCommandTopic()) {
        handleMqttCommand(message);
    }
} 