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
#include "jxct_constants.h"  // ✅ Централизованные константы
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

// ✅ НОВОЕ: Кэш Home Assistant конфигураций
struct HomeAssistantConfigCache {
    char tempConfig[512];
    char humConfig[512];
    char ecConfig[512];
    char phConfig[512];
    char nitrogenConfig[512];
    char phosphorusConfig[512];
    char potassiumConfig[512];
    bool isValid;
    char cachedDeviceId[32];
    char cachedTopicPrefix[64];
} haConfigCache = {"", "", "", "", "", "", "", false, "", ""};

// ✅ НОВОЕ: Кэш топиков публикации
static char pubTopicCache[7][128] = {"", "", "", "", "", "", ""};
static bool pubTopicCacheValid = false;

// ✅ НОВОЕ: Кэш данных датчика JSON
static char cachedSensorJson[256] = "";
static unsigned long lastCachedSensorTime = 0;
static bool sensorJsonCacheValid = false;

// ✅ ОПТИМИЗАЦИЯ 3.3: DNS кэширование для избежания повторных запросов
struct DNSCache {
    char hostname[HOSTNAME_BUFFER_SIZE];
    IPAddress cachedIP;
    unsigned long cacheTime;
    bool isValid;
} dnsCacheMqtt = {"", IPAddress(0, 0, 0, 0), 0, false};

// Forward declarations
void mqttCallback(char* topic, byte* payload, unsigned int length);
void publishHomeAssistantConfig();
void invalidateHAConfigCache();  // ✅ НОВОЕ: Инвалидация кэша
IPAddress getCachedIP(const char* hostname);  // ✅ НОВОЕ: Forward declaration для DNS кэша

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

    // ✅ ОПТИМИЗАЦИЯ 3.3: Используем кэшированный DNS резолвинг
    IPAddress mqttServerIP = getCachedIP(config.mqttServer);
    if (mqttServerIP == IPAddress(0, 0, 0, 0)) {
        ERROR_PRINTF("[DNS] Не удалось разрешить DNS для %s\n", config.mqttServer);
        strlcpy(mqttLastErrorBuffer, "Ошибка DNS резолвинга", sizeof(mqttLastErrorBuffer));
        return;
    }

    DEBUG_PRINTF("[DNS] MQTT сервер %s -> %s\n", config.mqttServer, mqttServerIP.toString().c_str());
    mqttClient.setServer(mqttServerIP, config.mqttPort);  // Используем IP вместо hostname
    mqttClient.setCallback(mqttCallback);
    mqttClient.setKeepAlive(30);
    mqttClient.setSocketTimeout(30);

    INFO_PRINTLN("[MQTT] Инициализация завершена с DNS кэшированием");
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

// ДЕЛЬТА-ФИЛЬТР v2.2.1: Проверка необходимости публикации
bool shouldPublishMqtt()
{
    static int skipCounter = 0;
    
    // Первая публикация - всегда публикуем
    if (sensorData.last_mqtt_publish == 0) {
        return true;
    }
    
    // Принудительная публикация каждые N циклов (настраиваемо v2.3.0)
    if (++skipCounter >= config.forcePublishCycles) {
        skipCounter = 0;
        DEBUG_PRINTLN("[DELTA] Принудительная публикация (цикл)");
        return true;
    }
    
    // Проверяем дельта изменения
    bool hasSignificantChange = false;
    
    if (abs(sensorData.temperature - sensorData.prev_temperature) >= config.deltaTemperature) {
        DEBUG_PRINTF("[DELTA] Температура изменилась: %.1f -> %.1f\n", sensorData.prev_temperature, sensorData.temperature);
        hasSignificantChange = true;
    }
    
    if (abs(sensorData.humidity - sensorData.prev_humidity) >= config.deltaHumidity) {
        DEBUG_PRINTF("[DELTA] Влажность изменилась: %.1f -> %.1f\n", sensorData.prev_humidity, sensorData.humidity);
        hasSignificantChange = true;
    }
    
    if (abs(sensorData.ph - sensorData.prev_ph) >= config.deltaPh) {
        DEBUG_PRINTF("[DELTA] pH изменился: %.1f -> %.1f\n", sensorData.prev_ph, sensorData.ph);
        hasSignificantChange = true;
    }
    
    if (abs(sensorData.ec - sensorData.prev_ec) >= config.deltaEc) {
        DEBUG_PRINTF("[DELTA] EC изменилась: %.0f -> %.0f\n", sensorData.prev_ec, sensorData.ec);
        hasSignificantChange = true;
    }
    
    if (abs(sensorData.nitrogen - sensorData.prev_nitrogen) >= config.deltaNpk) {
        DEBUG_PRINTF("[DELTA] Азот изменился: %.0f -> %.0f\n", sensorData.prev_nitrogen, sensorData.nitrogen);
        hasSignificantChange = true;
    }
    
    if (abs(sensorData.phosphorus - sensorData.prev_phosphorus) >= config.deltaNpk) {
        DEBUG_PRINTF("[DELTA] Фосфор изменился: %.0f -> %.0f\n", sensorData.prev_phosphorus, sensorData.phosphorus);
        hasSignificantChange = true;
    }
    
    if (abs(sensorData.potassium - sensorData.prev_potassium) >= config.deltaNpk) {
        DEBUG_PRINTF("[DELTA] Калий изменился: %.0f -> %.0f\n", sensorData.prev_potassium, sensorData.potassium);
        hasSignificantChange = true;
    }
    
    if (hasSignificantChange) {
        skipCounter = 0; // Сбрасываем счетчик при значимом изменении
    } else {
        DEBUG_PRINTLN("[DELTA] Изменения незначительные, пропускаем публикацию");
    }
    
    return hasSignificantChange;
}

void publishSensorData()
{
    if (!config.flags.mqttEnabled || !mqttClient.connected() || !sensorData.valid)
    {
        return;
    }
    
    // ДЕЛЬТА-ФИЛЬТР v2.2.1: Проверяем необходимость публикации
    if (!shouldPublishMqtt()) {
        return;
    }

    // ✅ ОПТИМИЗАЦИЯ: Кэшируем JSON данных датчика
    unsigned long currentTime = millis();
    bool needToRebuildJson = false;
    
    // Проверяем, нужно ли пересоздать JSON (данные обновились или кэш устарел)
    if (!sensorJsonCacheValid || 
        (currentTime - lastCachedSensorTime > 1000) ||  // Кэш на 1 секунду
        (currentTime - sensorData.last_update < 100))   // Свежие данные
    {
        needToRebuildJson = true;
    }
    
    if (needToRebuildJson)
    {
        // Пересоздаем JSON только при необходимости
        StaticJsonDocument<256> doc;  // ✅ Уменьшен размер с 512 до 256
        
        // ✅ ОПТИМИЗАЦИЯ 3.1: Сокращенные ключи для экономии трафика
        doc["t"] = round(sensorData.temperature * 10) / 10.0;    // temperature → t (-10 байт)
        doc["h"] = round(sensorData.humidity * 10) / 10.0;       // humidity → h (-7 байт)  
        doc["e"] = (int)round(sensorData.ec);                    // ec → e (стабильно)
        doc["p"] = round(sensorData.ph * 10) / 10.0;            // ph → p (стабильно)
        doc["n"] = (int)round(sensorData.nitrogen);             // nitrogen → n (-7 байт)
        doc["r"] = (int)round(sensorData.phosphorus);           // phosphorus → r (-9 байт) 
        doc["k"] = (int)round(sensorData.potassium);            // potassium → k (-8 байт)
        doc["ts"] = (long)(timeClient ? timeClient->getEpochTime() : 0);  // timestamp → ts (-7 байт)

        // ✅ Кэшируем результат
        serializeJson(doc, cachedSensorJson, sizeof(cachedSensorJson));
        lastCachedSensorTime = currentTime;
        sensorJsonCacheValid = true;
        
        DEBUG_PRINTLN("[MQTT] Компактный JSON датчика пересоздан и закэширован");
    }

    // ✅ Кэшируем топик публикации
    static char stateTopicBuffer[128] = "";
    static bool stateTopicCached = false;
    if (!stateTopicCached)
    {
        snprintf(stateTopicBuffer, sizeof(stateTopicBuffer), "%s/state", config.mqttTopicPrefix);
        stateTopicCached = true;
    }

    // Публикуем кэшированный JSON
    bool res = mqttClient.publish(stateTopicBuffer, cachedSensorJson, true);

    if (res)
    {
        strcpy(mqttLastErrorBuffer, "");
        
        // ДЕЛЬТА-ФИЛЬТР v2.2.1: Сохраняем текущие значения как предыдущие
        sensorData.prev_temperature = sensorData.temperature;
        sensorData.prev_humidity = sensorData.humidity;
        sensorData.prev_ec = sensorData.ec;
        sensorData.prev_ph = sensorData.ph;
        sensorData.prev_nitrogen = sensorData.nitrogen;
        sensorData.prev_phosphorus = sensorData.phosphorus;
        sensorData.prev_potassium = sensorData.potassium;
        sensorData.last_mqtt_publish = millis();
        
        DEBUG_PRINTLN("[MQTT] Данные опубликованы, предыдущие значения обновлены");
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
    
    // ✅ ОПТИМИЗАЦИЯ: Проверяем кэш конфигураций
    bool needToRebuildConfigs = false;
    if (!haConfigCache.isValid ||
        strcmp(haConfigCache.cachedDeviceId, deviceId) != 0 ||
        strcmp(haConfigCache.cachedTopicPrefix, config.mqttTopicPrefix) != 0)
    {
        needToRebuildConfigs = true;
        DEBUG_PRINTLN("[HA] Кэш конфигураций устарел, пересоздаем...");
    }
    
    if (needToRebuildConfigs)
    {
        // Обновляем кэшированные значения
        strlcpy(haConfigCache.cachedDeviceId, deviceId, sizeof(haConfigCache.cachedDeviceId));
        strlcpy(haConfigCache.cachedTopicPrefix, config.mqttTopicPrefix, sizeof(haConfigCache.cachedTopicPrefix));
        
        // ✅ Создаем JSON конфигурации один раз и кэшируем их
        StaticJsonDocument<256> deviceInfo;
        deviceInfo["identifiers"] = deviceId;
        deviceInfo["manufacturer"] = DEVICE_MANUFACTURER;
        deviceInfo["model"] = DEVICE_MODEL;
        deviceInfo["sw_version"] = DEVICE_SW_VERSION;
        deviceInfo["name"] = deviceId;
        
        // Создаем все конфигурации и сразу сериализуем в кэш
        StaticJsonDocument<512> tempConfig;
        tempConfig["name"] = "JXCT Temperature";
        tempConfig["device_class"] = "temperature";
        tempConfig["state_topic"] = String(config.mqttTopicPrefix) + "/state";
        tempConfig["unit_of_measurement"] = "°C";
        tempConfig["value_template"] = "{{ value_json.t }}";  // ✅ temperature → t
        tempConfig["unique_id"] = String(deviceId) + "_temp";
        tempConfig["availability_topic"] = String(config.mqttTopicPrefix) + "/status";
        tempConfig["device"] = deviceInfo;
        serializeJson(tempConfig, haConfigCache.tempConfig, sizeof(haConfigCache.tempConfig));
        
        StaticJsonDocument<512> humConfig;
        humConfig["name"] = "JXCT Humidity";
        humConfig["device_class"] = "humidity";
        humConfig["state_topic"] = String(config.mqttTopicPrefix) + "/state";
        humConfig["unit_of_measurement"] = "%";
        humConfig["value_template"] = "{{ value_json.h }}";  // ✅ humidity → h
        humConfig["unique_id"] = String(deviceId) + "_hum";
        humConfig["availability_topic"] = String(config.mqttTopicPrefix) + "/status";
        humConfig["device"] = deviceInfo;
        serializeJson(humConfig, haConfigCache.humConfig, sizeof(haConfigCache.humConfig));
        
        StaticJsonDocument<512> ecConfig;
        ecConfig["name"] = "JXCT EC";
        ecConfig["device_class"] = "conductivity";
        ecConfig["state_topic"] = String(config.mqttTopicPrefix) + "/state";
        ecConfig["unit_of_measurement"] = "µS/cm";
        ecConfig["value_template"] = "{{ value_json.e }}";  // ✅ ec → e
        ecConfig["unique_id"] = String(deviceId) + "_ec";
        ecConfig["availability_topic"] = String(config.mqttTopicPrefix) + "/status";
        ecConfig["device"] = deviceInfo;
        serializeJson(ecConfig, haConfigCache.ecConfig, sizeof(haConfigCache.ecConfig));
        
        StaticJsonDocument<512> phConfig;
        phConfig["name"] = "JXCT pH";
        phConfig["device_class"] = "ph";
        phConfig["state_topic"] = String(config.mqttTopicPrefix) + "/state";
        phConfig["unit_of_measurement"] = "pH";
        phConfig["value_template"] = "{{ value_json.p }}";  // ✅ ph → p
        phConfig["unique_id"] = String(deviceId) + "_ph";
        phConfig["availability_topic"] = String(config.mqttTopicPrefix) + "/status";
        phConfig["device"] = deviceInfo;
        serializeJson(phConfig, haConfigCache.phConfig, sizeof(haConfigCache.phConfig));
        
        StaticJsonDocument<512> nitrogenConfig;
        nitrogenConfig["name"] = "JXCT Nitrogen";
        nitrogenConfig["state_topic"] = String(config.mqttTopicPrefix) + "/state";
        nitrogenConfig["unit_of_measurement"] = "mg/kg";
        nitrogenConfig["value_template"] = "{{ value_json.n }}";  // ✅ nitrogen → n
        nitrogenConfig["unique_id"] = String(deviceId) + "_nitrogen";
        nitrogenConfig["availability_topic"] = String(config.mqttTopicPrefix) + "/status";
        nitrogenConfig["device"] = deviceInfo;
        serializeJson(nitrogenConfig, haConfigCache.nitrogenConfig, sizeof(haConfigCache.nitrogenConfig));
        
        StaticJsonDocument<512> phosphorusConfig;
        phosphorusConfig["name"] = "JXCT Phosphorus";
        phosphorusConfig["state_topic"] = String(config.mqttTopicPrefix) + "/state";
        phosphorusConfig["unit_of_measurement"] = "mg/kg";
        phosphorusConfig["value_template"] = "{{ value_json.r }}";  // ✅ phosphorus → r
        phosphorusConfig["unique_id"] = String(deviceId) + "_phosphorus";
        phosphorusConfig["availability_topic"] = String(config.mqttTopicPrefix) + "/status";
        phosphorusConfig["device"] = deviceInfo;
        serializeJson(phosphorusConfig, haConfigCache.phosphorusConfig, sizeof(haConfigCache.phosphorusConfig));
        
        StaticJsonDocument<512> potassiumConfig;
        potassiumConfig["name"] = "JXCT Potassium";
        potassiumConfig["state_topic"] = String(config.mqttTopicPrefix) + "/state";
        potassiumConfig["unit_of_measurement"] = "mg/kg";
        potassiumConfig["value_template"] = "{{ value_json.k }}";  // ✅ potassium → k
        potassiumConfig["unique_id"] = String(deviceId) + "_potassium";
        potassiumConfig["availability_topic"] = String(config.mqttTopicPrefix) + "/status";
        potassiumConfig["device"] = deviceInfo;
        serializeJson(potassiumConfig, haConfigCache.potassiumConfig, sizeof(haConfigCache.potassiumConfig));
        
        // ✅ Кэшируем топики публикации
        snprintf(pubTopicCache[0], sizeof(pubTopicCache[0]), "homeassistant/sensor/%s_temperature/config", deviceId);
        snprintf(pubTopicCache[1], sizeof(pubTopicCache[1]), "homeassistant/sensor/%s_humidity/config", deviceId);
        snprintf(pubTopicCache[2], sizeof(pubTopicCache[2]), "homeassistant/sensor/%s_ec/config", deviceId);
        snprintf(pubTopicCache[3], sizeof(pubTopicCache[3]), "homeassistant/sensor/%s_ph/config", deviceId);
        snprintf(pubTopicCache[4], sizeof(pubTopicCache[4]), "homeassistant/sensor/%s_nitrogen/config", deviceId);
        snprintf(pubTopicCache[5], sizeof(pubTopicCache[5]), "homeassistant/sensor/%s_phosphorus/config", deviceId);
        snprintf(pubTopicCache[6], sizeof(pubTopicCache[6]), "homeassistant/sensor/%s_potassium/config", deviceId);
        pubTopicCacheValid = true;
        
        haConfigCache.isValid = true;
        INFO_PRINTLN("[HA] Конфигурации созданы и закэшированы");
    }
    
    // ✅ Публикуем из кэша (супер быстро!)
    mqttClient.publish(pubTopicCache[0], haConfigCache.tempConfig, true);
    mqttClient.publish(pubTopicCache[1], haConfigCache.humConfig, true);
    mqttClient.publish(pubTopicCache[2], haConfigCache.ecConfig, true);
    mqttClient.publish(pubTopicCache[3], haConfigCache.phConfig, true);
    mqttClient.publish(pubTopicCache[4], haConfigCache.nitrogenConfig, true);
    mqttClient.publish(pubTopicCache[5], haConfigCache.phosphorusConfig, true);
    mqttClient.publish(pubTopicCache[6], haConfigCache.potassiumConfig, true);
    
    INFO_PRINTLN("[HA] Конфигурация Home Assistant опубликована из кэша");
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

void invalidateHAConfigCache()
{
    // Реализация инвалидации кэша Home Assistant конфигураций
    haConfigCache.isValid = false;
    strcpy(haConfigCache.cachedDeviceId, "");
    strcpy(haConfigCache.cachedTopicPrefix, "");
    strcpy(haConfigCache.tempConfig, "");
    strcpy(haConfigCache.humConfig, "");
    strcpy(haConfigCache.ecConfig, "");
    strcpy(haConfigCache.phConfig, "");
    strcpy(haConfigCache.nitrogenConfig, "");
    strcpy(haConfigCache.phosphorusConfig, "");
    strcpy(haConfigCache.potassiumConfig, "");
    INFO_PRINTLN("[MQTT] Кэш Home Assistant конфигураций инвалидирован");
    strcpy(mqttLastErrorBuffer, "");
}

// Функция получения IP с кэшированием
IPAddress getCachedIP(const char* hostname) {
    unsigned long currentTime = millis();
    
    // Проверяем кэш
    if (dnsCacheMqtt.isValid && 
        strcmp(dnsCacheMqtt.hostname, hostname) == 0 &&
        (currentTime - dnsCacheMqtt.cacheTime < DNS_CACHE_TTL)) {
        DEBUG_PRINTF("[DNS] Используем кэшированный IP %s для %s\n", 
                     dnsCacheMqtt.cachedIP.toString().c_str(), hostname);
        return dnsCacheMqtt.cachedIP;
    }
    
    // DNS запрос
    IPAddress resolvedIP;
    if (WiFi.hostByName(hostname, resolvedIP)) {
        // Кэшируем результат
        strlcpy(dnsCacheMqtt.hostname, hostname, sizeof(dnsCacheMqtt.hostname));
        dnsCacheMqtt.cachedIP = resolvedIP;
        dnsCacheMqtt.cacheTime = currentTime;
        dnsCacheMqtt.isValid = true;
        DEBUG_PRINTF("[DNS] Новый IP %s для %s кэширован\n", 
                     resolvedIP.toString().c_str(), hostname);
        return resolvedIP;
    }
    
    return IPAddress(0, 0, 0, 0);  // Ошибка резолвинга
}