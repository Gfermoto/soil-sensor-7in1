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
#include "logger.h"
#include "debug.h"  // ✅ Добавляем систему условной компиляции
#include <NTPClient.h>
extern NTPClient* timeClient;

WiFiClient espClient;
PubSubClient mqttClient(espClient);
bool mqttConnected = false;

// ✅ Заменяем String на статический буфер
static char mqttLastErrorBuffer[128] = "";
const char* getMqttLastError() { return mqttLastErrorBuffer; }

// ✅ Статические буферы для топиков и ID
static char clientIdBuffer[32] = "";
static char statusTopicBuffer[128] = "";
static char commandTopicBuffer[128] = "";

// Forward declarations
void mqttCallback(char* topic, byte* payload, unsigned int length);
void publishHomeAssistantConfig();

// ✅ Оптимизированная функция getClientId с буфером
const char* getClientId()
{
    if (clientIdBuffer[0] == '\0') {  // Кэшируем результат
        uint8_t mac[6];
        WiFi.macAddress(mac);
        snprintf(clientIdBuffer, sizeof(clientIdBuffer), "JXCT_%02X%02X%02X%02X%02X%02X", 
                 mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    }
    return clientIdBuffer;
}

// ✅ Оптимизированная функция getMqttClientName
const char* getMqttClientName()
{
    if (strlen(config.mqttDeviceName) > 0)
    {
        return config.mqttDeviceName;
    }
    else
    {
        return getClientId();
    }
}

// ✅ Оптимизированная функция getStatusTopic с буфером
const char* getStatusTopic()
{
    if (statusTopicBuffer[0] == '\0') {  // Кэшируем результат
        snprintf(statusTopicBuffer, sizeof(statusTopicBuffer), "%s/status", config.mqttTopicPrefix);
    }
    return statusTopicBuffer;
}

// ✅ Оптимизированная функция getCommandTopic с буфером
const char* getCommandTopic()
{
    if (commandTopicBuffer[0] == '\0') {  // Кэшируем результат
        snprintf(commandTopicBuffer, sizeof(commandTopicBuffer), "%s/command", config.mqttTopicPrefix);
    }
    return commandTopicBuffer;
}

void publishAvailability(bool online)
{
    const char* topic = getStatusTopic();
    const char* payload = online ? "online" : "offline";
    DEBUG_PRINTF("[publishAvailability] Публикация статуса: %s в топик %s\n", payload, topic);
    mqttClient.publish(topic, payload, true);
}

void setupMQTT()
{
    DEBUG_PRINTLN("[КРИТИЧЕСКАЯ ОТЛАДКА] Инициализация MQTT");

    // Расширенная диагностика WiFi
    DEBUG_PRINTLN("[WiFi Debug] Статус подключения:");
    DEBUG_PRINTF("WiFi статус: %d\n", WiFi.status());
    DEBUG_PRINTF("IP-адрес: %s\n", WiFi.localIP().toString().c_str());
    DEBUG_PRINTF("Маска подсети: %s\n", WiFi.subnetMask().toString().c_str());
    DEBUG_PRINTF("Шлюз: %s\n", WiFi.gatewayIP().toString().c_str());

    DEBUG_PRINTLN("[MQTT Debug] Параметры:");
    DEBUG_PRINTF("MQTT включен: %d\n", config.flags.mqttEnabled);
    DEBUG_PRINTF("Сервер: %s\n", config.mqttServer);
    DEBUG_PRINTF("Порт: %d\n", config.mqttPort);
    DEBUG_PRINTF("Префикс топика: %s\n", config.mqttTopicPrefix);
    DEBUG_PRINTF("Пользователь: %s\n", config.mqttUser);

    if (!config.flags.mqttEnabled || strlen(config.mqttServer) == 0)
    {
        ERROR_PRINTLN("[ОШИБКА] MQTT не может быть инициализирован");
        return;
    }

    mqttClient.setServer(config.mqttServer, config.mqttPort);
    mqttClient.setCallback(mqttCallback);
    mqttClient.setKeepAlive(30);
    mqttClient.setSocketTimeout(30);

    INFO_PRINTLN("[MQTT] Инициализация завершена");
}

bool connectMQTT()
{
    DEBUG_PRINTLN("[КРИТИЧЕСКАЯ ОТЛАДКА] Попытка подключения к MQTT");

    // Проверка WiFi
    if (WiFi.status() != WL_CONNECTED)
    {
        ERROR_PRINTLN("[ОШИБКА] WiFi не подключен!");
        return false;
    }

    // Расширенная проверка параметров
    if (strlen(config.mqttServer) == 0)
    {
        ERROR_PRINTLN("[ОШИБКА] Не указан MQTT-сервер");
        return false;
    }

    // Попытка подключения с максимальной детализацией
    const char* clientId = getMqttClientName();
    DEBUG_PRINTF("[MQTT] Сервер: %s\n", config.mqttServer);
    DEBUG_PRINTF("[MQTT] Порт: %d\n", config.mqttPort);
    DEBUG_PRINTF("[MQTT] ID клиента: %s\n", clientId);
    DEBUG_PRINTF("[MQTT] Пользователь: %s\n", config.mqttUser);
    DEBUG_PRINTF("[MQTT] Пароль: %s\n", config.mqttPassword);

    mqttClient.setServer(config.mqttServer, config.mqttPort);

    // Попытка подключения с максимально подробной информацией
    bool result = mqttClient.connect(clientId,
                                     config.mqttUser,      // может быть пустым
                                     config.mqttPassword,  // может быть пустым
                                     getStatusTopic(),
                                     1,         // QoS
                                     true,      // retain
                                     "offline"  // will message
    );

    DEBUG_PRINTF("[MQTT] Результат подключения: %d\n", result);

    // Расшифровка кодов состояния
    int state = mqttClient.state();
    DEBUG_PRINTF("[MQTT] Состояние клиента: %d - ", state);
    
    // Сохраняем ошибку в буфер для доступа извне
    switch (state)
    {
        case -4:
            DEBUG_PRINTLN("Тайм-аут подключения");
            strncpy(mqttLastErrorBuffer, "Тайм-аут подключения", sizeof(mqttLastErrorBuffer)-1);
            break;
        case -3:
            DEBUG_PRINTLN("Соединение потеряно");
            strncpy(mqttLastErrorBuffer, "Соединение потеряно", sizeof(mqttLastErrorBuffer)-1);
            break;
        case -2:
            DEBUG_PRINTLN("Ошибка подключения");
            strncpy(mqttLastErrorBuffer, "Ошибка подключения", sizeof(mqttLastErrorBuffer)-1);
            break;
        case -1:
            DEBUG_PRINTLN("Отключено");
            strncpy(mqttLastErrorBuffer, "Отключено", sizeof(mqttLastErrorBuffer)-1);
            break;
        case 0:
            DEBUG_PRINTLN("Подключено");
            strncpy(mqttLastErrorBuffer, "Подключено", sizeof(mqttLastErrorBuffer)-1);
            break;
        case 1:
            DEBUG_PRINTLN("Неверный протокол");
            strncpy(mqttLastErrorBuffer, "Неверный протокол", sizeof(mqttLastErrorBuffer)-1);
            break;
        case 2:
            DEBUG_PRINTLN("Неверный ID клиента");
            strncpy(mqttLastErrorBuffer, "Неверный ID клиента", sizeof(mqttLastErrorBuffer)-1);
            break;
        case 3:
            DEBUG_PRINTLN("Сервер недоступен");
            strncpy(mqttLastErrorBuffer, "Сервер недоступен", sizeof(mqttLastErrorBuffer)-1);
            break;
        case 4:
            DEBUG_PRINTLN("Неверные учетные данные");
            strncpy(mqttLastErrorBuffer, "Неверные учетные данные", sizeof(mqttLastErrorBuffer)-1);
            break;
        case 5:
            DEBUG_PRINTLN("Не авторизован");
            strncpy(mqttLastErrorBuffer, "Не авторизован", sizeof(mqttLastErrorBuffer)-1);
            break;
        default:
            DEBUG_PRINTLN("Неизвестная ошибка");
            strncpy(mqttLastErrorBuffer, "Неизвестная ошибка", sizeof(mqttLastErrorBuffer)-1);
            break;
    }

    // Если подключились успешно
    if (result)
    {
        INFO_PRINTLN("[MQTT] Подключение успешно!");

        // Подписываемся на топик команд
        const char* commandTopic = getCommandTopic();
        mqttClient.subscribe(commandTopic);
        DEBUG_PRINTF("[MQTT] Подписались на топик команд: %s\n", commandTopic);

        // Публикуем статус availability
        publishAvailability(true);

        // Публикуем конфигурацию Home Assistant discovery если включено
        if (config.flags.hassEnabled)
        {
            publishHomeAssistantConfig();
        }
    }

    return result;
}

void handleMQTT()
{
    if (!config.flags.mqttEnabled)
    {
        return;
    }

    static bool wasConnected = false;
    bool isConnected = mqttClient.connected();

    // Логирование изменений состояния подключения
    if (wasConnected && !isConnected)
    {
        logWarn("MQTT подключение потеряно!");
    }
    else if (!wasConnected && isConnected)
    {
        logSuccess("MQTT переподключение успешно");
    }
    wasConnected = isConnected;

    if (!isConnected)
    {
        static unsigned long lastReconnectAttempt = 0;
        if (millis() - lastReconnectAttempt > 5000)
        {
            lastReconnectAttempt = millis();
            logMQTT("Попытка переподключения...");
            connectMQTT();
        }
    }
    else
    {
        mqttClient.loop();
    }
}

void publishSensorData()
{
    if (!config.flags.mqttEnabled || !mqttClient.connected() || !sensorData.valid)
    {
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

    char stateTopicBuffer[128];
    snprintf(stateTopicBuffer, sizeof(stateTopicBuffer), "%s/state", config.mqttTopicPrefix);
    bool res = mqttClient.publish(stateTopicBuffer, jsonString.c_str(), true);

    if (res)
    {
        strcpy(mqttLastErrorBuffer, "");
    }
    else
    {
        strcpy(mqttLastErrorBuffer, "Ошибка публикации MQTT");
    }
}

void publishHomeAssistantConfig()
{
    DEBUG_PRINTLN("[publishHomeAssistantConfig] Публикация discovery-конфигов Home Assistant...");
    if (!config.flags.mqttEnabled || !mqttClient.connected() || !config.flags.hassEnabled)
    {
        DEBUG_PRINTLN("[publishHomeAssistantConfig] Условия не выполнены, публикация отменена");
        return;
    }
    String deviceIdStr = getDeviceId();
    const char* deviceId = deviceIdStr.c_str();
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
    tempConfig["state_topic"] = String(config.mqttTopicPrefix) + "/state";
    tempConfig["unit_of_measurement"] = "°C";
    tempConfig["value_template"] = "{{ value_json.temperature }}";
    tempConfig["unique_id"] = String(deviceId) + "_temp";
    tempConfig["availability_topic"] = String(config.mqttTopicPrefix) + "/status";
    tempConfig["device"] = deviceInfo;
    // Влажность
    StaticJsonDocument<512> humConfig;
    humConfig["name"] = "JXCT Humidity";
    humConfig["device_class"] = "humidity";
    humConfig["state_topic"] = String(config.mqttTopicPrefix) + "/state";
    humConfig["unit_of_measurement"] = "%";
    humConfig["value_template"] = "{{ value_json.humidity }}";
    humConfig["unique_id"] = String(deviceId) + "_hum";
    humConfig["availability_topic"] = String(config.mqttTopicPrefix) + "/status";
    humConfig["device"] = deviceInfo;
    // EC
    StaticJsonDocument<512> ecConfig;
    ecConfig["name"] = "JXCT EC";
    ecConfig["device_class"] = "conductivity";
    ecConfig["state_topic"] = String(config.mqttTopicPrefix) + "/state";
    ecConfig["unit_of_measurement"] = "µS/cm";
    ecConfig["value_template"] = "{{ value_json.ec }}";
    ecConfig["unique_id"] = String(deviceId) + "_ec";
    ecConfig["availability_topic"] = String(config.mqttTopicPrefix) + "/status";
    ecConfig["device"] = deviceInfo;
    // pH
    StaticJsonDocument<512> phConfig;
    phConfig["name"] = "JXCT pH";
    phConfig["device_class"] = "ph";
    phConfig["state_topic"] = String(config.mqttTopicPrefix) + "/state";
    phConfig["unit_of_measurement"] = "pH";
    phConfig["value_template"] = "{{ value_json.ph }}";
    phConfig["unique_id"] = String(deviceId) + "_ph";
    phConfig["availability_topic"] = String(config.mqttTopicPrefix) + "/status";
    phConfig["device"] = deviceInfo;
    // NPK (раздельно)
    StaticJsonDocument<512> nitrogenConfig;
    nitrogenConfig["name"] = "JXCT Nitrogen";
    nitrogenConfig["state_topic"] = String(config.mqttTopicPrefix) + "/state";
    nitrogenConfig["unit_of_measurement"] = "mg/kg";
    nitrogenConfig["value_template"] = "{{ value_json.nitrogen }}";
    nitrogenConfig["unique_id"] = String(deviceId) + "_nitrogen";
    nitrogenConfig["availability_topic"] = String(config.mqttTopicPrefix) + "/status";
    nitrogenConfig["device"] = deviceInfo;
    StaticJsonDocument<512> phosphorusConfig;
    phosphorusConfig["name"] = "JXCT Phosphorus";
    phosphorusConfig["state_topic"] = String(config.mqttTopicPrefix) + "/state";
    phosphorusConfig["unit_of_measurement"] = "mg/kg";
    phosphorusConfig["value_template"] = "{{ value_json.phosphorus }}";
    phosphorusConfig["unique_id"] = String(deviceId) + "_phosphorus";
    phosphorusConfig["availability_topic"] = String(config.mqttTopicPrefix) + "/status";
    phosphorusConfig["device"] = deviceInfo;
    StaticJsonDocument<512> potassiumConfig;
    potassiumConfig["name"] = "JXCT Potassium";
    potassiumConfig["state_topic"] = String(config.mqttTopicPrefix) + "/state";
    potassiumConfig["unit_of_measurement"] = "mg/kg";
    potassiumConfig["value_template"] = "{{ value_json.potassium }}";
    potassiumConfig["unique_id"] = String(deviceId) + "_potassium";
    potassiumConfig["availability_topic"] = String(config.mqttTopicPrefix) + "/status";
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
    mqttClient.publish(("homeassistant/sensor/" + String(deviceId) + "_temperature/config").c_str(), tempConfigStr.c_str(),
                       true);
    mqttClient.publish(("homeassistant/sensor/" + String(deviceId) + "_humidity/config").c_str(), humConfigStr.c_str(), true);
    mqttClient.publish(("homeassistant/sensor/" + String(deviceId) + "_ec/config").c_str(), ecConfigStr.c_str(), true);
    mqttClient.publish(("homeassistant/sensor/" + String(deviceId) + "_ph/config").c_str(), phConfigStr.c_str(), true);
    mqttClient.publish(("homeassistant/sensor/" + String(deviceId) + "_nitrogen/config").c_str(), nitrogenConfigStr.c_str(),
                       true);
    mqttClient.publish(("homeassistant/sensor/" + String(deviceId) + "_phosphorus/config").c_str(), phosphorusConfigStr.c_str(),
                       true);
    mqttClient.publish(("homeassistant/sensor/" + String(deviceId) + "_potassium/config").c_str(), potassiumConfigStr.c_str(),
                       true);
    INFO_PRINTLN("[publishHomeAssistantConfig] Конфигурация Home Assistant опубликована");
    strcpy(mqttLastErrorBuffer, "");
}

void removeHomeAssistantConfig()
{
    String deviceIdStr = getDeviceId();
    const char* deviceId = deviceIdStr.c_str();
    // Публикуем пустой payload с retain для удаления сенсоров из HA
    mqttClient.publish(("homeassistant/sensor/" + String(deviceId) + "_temperature/config").c_str(), "", true);
    mqttClient.publish(("homeassistant/sensor/" + String(deviceId) + "_humidity/config").c_str(), "", true);
    mqttClient.publish(("homeassistant/sensor/" + String(deviceId) + "_ec/config").c_str(), "", true);
    mqttClient.publish(("homeassistant/sensor/" + String(deviceId) + "_ph/config").c_str(), "", true);
    mqttClient.publish(("homeassistant/sensor/" + String(deviceId) + "_nitrogen/config").c_str(), "", true);
    mqttClient.publish(("homeassistant/sensor/" + String(deviceId) + "_phosphorus/config").c_str(), "", true);
    mqttClient.publish(("homeassistant/sensor/" + String(deviceId) + "_potassium/config").c_str(), "", true);
    INFO_PRINTLN("[MQTT] Discovery-конфиги Home Assistant удалены");
    strcpy(mqttLastErrorBuffer, "");
}

void handleMqttCommand(const String& cmd)
{
    DEBUG_PRINT("[MQTT] Получена команда: ");
    DEBUG_PRINTLN(cmd);
    if (cmd == "reboot")
    {
        ESP.restart();
    }
    else if (cmd == "reset")
    {
        resetConfig();
        ESP.restart();
    }
    else if (cmd == "publish_test")
    {
        publishSensorData();
    }
    else if (cmd == "publish_discovery")
    {
        publishHomeAssistantConfig();
    }
    else if (cmd == "remove_discovery")
    {
        removeHomeAssistantConfig();
    }
    else
    {
        DEBUG_PRINTLN("[MQTT] Неизвестная команда");
    }
}

void mqttCallback(char* topic, byte* payload, unsigned int length)
{
    String t = String(topic);
    String message;
    for (unsigned int i = 0; i < length; i++) message += (char)payload[i];
    DEBUG_PRINTF("[mqttCallback] Получено сообщение: %s = %s\n", t.c_str(), message.c_str());
    if (t == getCommandTopic())
    {
        handleMqttCommand(message);
    }
}