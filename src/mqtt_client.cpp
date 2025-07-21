/**
 * @file mqtt_client.cpp
 * @brief Взаимодействие с MQTT-брокером и Home Assistant
 * @details Реализация подключения, публикации данных, обработки команд и интеграции с Home Assistant через discovery.
 */
#include "mqtt_client.h"  // 🆕 Подключаем собственный заголовок, убираем дубли объявлений
#include <Arduino.h>
#include <ArduinoJson.h>
#include <NTPClient.h>
#include <PubSubClient.h>
#include <WiFiClient.h>
#include <array>
#include "debug.h"  // ✅ Добавляем систему условной компиляции
#include "jxct_config_vars.h"
#include "jxct_constants.h"  // ✅ Централизованные константы
#include "jxct_device_info.h"
#include "jxct_format_utils.h"
#include "logger.h"
#include "modbus_sensor.h"
#include "ota_manager.h"
#include "wifi_manager.h"
extern NTPClient* timeClient;

// Глобальные переменные (глобальное пространство имён)
WiFiClient espClient;                // NOLINT(misc-use-internal-linkage)
PubSubClient mqttClient(espClient);  // NOLINT(misc-use-internal-linkage)

namespace
{
// Forward declarations для всех внутренних функций
void publishAvailabilityInternal(bool online);
void setupMQTTInternal();
bool connectMQTTInternal();
void handleMQTTInternal();
void publishSensorDataInternal();
void publishHomeAssistantConfigInternal();
void removeHomeAssistantConfigInternal();
void handleMqttCommandInternal(const String& cmd);
void mqttCallbackInternal(const char* topic, const byte* payload, unsigned int length);
void invalidateHAConfigCacheInternal();

// Кэш для Home Assistant конфигураций
struct HomeAssistantConfigCache
{
    std::array<char, 512> tempConfig = {""};
    std::array<char, 512> humConfig = {""};
    std::array<char, 512> ecConfig = {""};
    std::array<char, 512> phConfig = {""};
    std::array<char, 512> nitrogenConfig = {""};
    std::array<char, 512> phosphorusConfig = {""};
    std::array<char, 512> potassiumConfig = {""};
    bool isValid = false;
    std::array<char, 32> cachedDeviceId = {""};
    std::array<char, 64> cachedTopicPrefix = {""};
} haConfigCache;

// Кэш для DNS запросов
struct DNSCache
{
    std::array<char, HOSTNAME_BUFFER_SIZE> hostname = {""};
    IPAddress cachedIP;
    unsigned long cacheTime;
    bool isValid;
} dnsCacheMqtt = {{""}, IPAddress(0, 0, 0, 0), 0, false};

// Кэш для топиков публикации
std::array<std::array<char, 64>, 7> pubTopicCache = {{}};
bool pubTopicCacheValid = false;

// Буфер для последней ошибки MQTT
std::array<char, 128> mqttLastErrorBuffer = {""};

// Буферы для идентификаторов и топиков
std::array<char, 32> clientIdBuffer = {""};
std::array<char, 128> statusTopicBuffer = {""};
std::array<char, 128> commandTopicBuffer = {""};
std::array<char, 128> otaStatusTopicBuffer = {""};
std::array<char, 128> otaCommandTopicBuffer = {""};

// Кэш JSON датчиков
std::array<char, 256> cachedSensorJson = {""};
unsigned long lastCachedSensorTime = 0;
bool sensorJsonCacheValid = false;

// Функция получения IP с кэшированием
IPAddress getCachedIP(const char* hostname)
{
    const unsigned long currentTime = millis();

    // Проверяем кэш
    if (dnsCacheMqtt.isValid && strcmp(dnsCacheMqtt.hostname.data(), hostname) == 0 &&
        (currentTime - dnsCacheMqtt.cacheTime < DNS_CACHE_TTL))
    {
        DEBUG_PRINTF("[DNS] Используем кэшированный IP %s для %s\n", dnsCacheMqtt.cachedIP.toString().c_str(),
                     hostname);
        return dnsCacheMqtt.cachedIP;
    }

    // DNS запрос
    IPAddress resolvedIP;
    if (WiFi.hostByName(hostname, resolvedIP) != 0)  // NOLINT(readability-static-accessed-through-instance)
    {
        // Кэшируем результат
        strlcpy(dnsCacheMqtt.hostname.data(), hostname, dnsCacheMqtt.hostname.size());
        dnsCacheMqtt.cachedIP = resolvedIP;
        dnsCacheMqtt.cacheTime = currentTime;
        dnsCacheMqtt.isValid = true;
        DEBUG_PRINTF("[DNS] Новый IP %s для %s кэширован\n", resolvedIP.toString().c_str(), hostname);
        return resolvedIP;
    }

    return IPAddress{0, 0, 0, 0};  // Ошибка резолвинга
}

String getClientId()
{
    static String clientId;
    if (clientId.length() == 0)
    {
        clientId = "JXCT_" + WiFi.macAddress();
        clientId.replace(":", "");
    }
    return clientId;
}

// Удалить функции доступа к переменным с внутренней связностью (getEspClient, getMqttClient)

// -----------------------------
// Вспомогательные функции OTA
// -----------------------------
const char* getOtaStatusTopic()
{
    if (otaStatusTopicBuffer[0] == '\0')
    {
        snprintf(otaStatusTopicBuffer.data(), otaStatusTopicBuffer.size(), "%s/ota/status", config.mqttTopicPrefix);
    }
    return otaStatusTopicBuffer.data();
}

const char* getOtaCommandTopic()
{
    if (otaCommandTopicBuffer[0] == '\0')
    {
        snprintf(otaCommandTopicBuffer.data(), otaCommandTopicBuffer.size(), "%s/ota/command", config.mqttTopicPrefix);
    }
    return otaCommandTopicBuffer.data();
}

// ✅ Оптимизированная функция getMqttClientName
const char* getMqttClientName()
{
    if (strlen(config.mqttDeviceName) > 0)
    {
        return config.mqttDeviceName;
    }
    return getClientId().c_str();
}

// ✅ Оптимизированная функция getStatusTopic с буфером
const char* getStatusTopic()
{
    if (statusTopicBuffer[0] == '\0')
    {  // Кэшируем результат
        snprintf(statusTopicBuffer.data(), statusTopicBuffer.size(), "%s/status", config.mqttTopicPrefix);
    }
    return statusTopicBuffer.data();
}

// ✅ Оптимизированная функция getCommandTopic с буфером
const char* getCommandTopic()
{
    if (commandTopicBuffer[0] == '\0')
    {  // Кэшируем результат
        snprintf(commandTopicBuffer.data(), commandTopicBuffer.size(), "%s/command", config.mqttTopicPrefix);
    }
    return commandTopicBuffer.data();
}

const char* getMqttLastError()
{
    return mqttLastErrorBuffer.data();
}

// Функции доступа к статическим переменным
WiFiClient& getEspClient()
{
    return espClient;
}
PubSubClient& getMqttClient()
{
    return mqttClient;
}

// Все функции с NOLINT(misc-use-internal-linkage) теперь здесь
void publishAvailabilityInternal(bool online)
{
    const char* topic = getStatusTopic();
    const char* payload = online ? "online" : "offline";
    DEBUG_PRINTF("[publishAvailability] Публикация статуса: %s в топик %s\n", payload, topic);
    mqttClient.publish(topic, payload, true);
}

void setupMQTTInternal()
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
    const IPAddress mqttServerIP = getCachedIP(config.mqttServer);
    if (mqttServerIP == IPAddress(0, 0, 0, 0))
    {
        ERROR_PRINTF("[DNS] Не удалось разрешить DNS для %s\n", config.mqttServer);
        strlcpy(mqttLastErrorBuffer.data(), "Ошибка DNS резолвинга", mqttLastErrorBuffer.size() - 1);
        mqttLastErrorBuffer[mqttLastErrorBuffer.size() - 1] = '\0';
        return;
    }

    DEBUG_PRINTF("[DNS] MQTT сервер %s -> %s\n", config.mqttServer, mqttServerIP.toString().c_str());
    mqttClient.setServer(mqttServerIP, config.mqttPort);  // Используем IP вместо hostname
    mqttClient.setCallback(mqttCallback);
    mqttClient.setKeepAlive(30);
    mqttClient.setSocketTimeout(30);

    INFO_PRINTLN("[MQTT] Инициализация завершена с DNS кэшированием");
}

bool connectMQTTInternal()
{
    DEBUG_PRINTLN("[КРИТИЧЕСКАЯ ОТЛАДКА] Попытка подключения к MQTT");

    // Проверка WiFi
    if (WiFi.status() != WL_CONNECTED)  // NOLINT(readability-static-accessed-through-instance)
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
    const char* const clientId = getMqttClientName();
    DEBUG_PRINTF("[MQTT] Сервер: %s\n", config.mqttServer);
    DEBUG_PRINTF("[MQTT] Порт: %d\n", config.mqttPort);
    DEBUG_PRINTF("[MQTT] ID клиента: %s\n", clientId);
    DEBUG_PRINTF("[MQTT] Пользователь: %s\n", config.mqttUser);
    DEBUG_PRINTF("[MQTT] Пароль: %s\n", config.mqttPassword);

    mqttClient.setServer(config.mqttServer, config.mqttPort);

    // Попытка подключения с максимально подробной информацией
    const bool result = mqttClient.connect(clientId,
                                           config.mqttUser,      // может быть пустым
                                           config.mqttPassword,  // может быть пустым
                                           getStatusTopic(),
                                           1,         // QoS
                                           true,      // retain
                                           "offline"  // will message
    );

    DEBUG_PRINTF("[MQTT] Результат подключения: %d\n", result);

    // Расшифровка кодов состояния
    const int state = mqttClient.state();
    DEBUG_PRINTF("[MQTT] Состояние клиента: %d - ", state);

    // Сохраняем ошибку в буфер для доступа извне
    switch (state)
    {
        case -4:
            DEBUG_PRINTLN("Тайм-аут подключения");
            strncpy(mqttLastErrorBuffer.data(), "Тайм-аут подключения", mqttLastErrorBuffer.size() - 1);
            mqttLastErrorBuffer[mqttLastErrorBuffer.size() - 1] = '\0';
            break;
        case -3:
            DEBUG_PRINTLN("Соединение потеряно");
            strncpy(mqttLastErrorBuffer.data(), "Соединение потеряно", mqttLastErrorBuffer.size() - 1);
            mqttLastErrorBuffer[mqttLastErrorBuffer.size() - 1] = '\0';
            break;
        case -2:
            DEBUG_PRINTLN("Ошибка подключения");
            strncpy(mqttLastErrorBuffer.data(), "Ошибка подключения", mqttLastErrorBuffer.size() - 1);
            mqttLastErrorBuffer[mqttLastErrorBuffer.size() - 1] = '\0';
            break;
        case -1:
            DEBUG_PRINTLN("Отключено");
            strncpy(mqttLastErrorBuffer.data(), "Отключено", mqttLastErrorBuffer.size() - 1);
            mqttLastErrorBuffer[mqttLastErrorBuffer.size() - 1] = '\0';
            break;
        case 0:
            DEBUG_PRINTLN("Подключено");
            strncpy(mqttLastErrorBuffer.data(), "Подключено", mqttLastErrorBuffer.size() - 1);
            mqttLastErrorBuffer[mqttLastErrorBuffer.size() - 1] = '\0';
            break;
        case 1:
            DEBUG_PRINTLN("Неверный протокол");
            strncpy(mqttLastErrorBuffer.data(), "Неверный протокол", mqttLastErrorBuffer.size() - 1);
            mqttLastErrorBuffer[mqttLastErrorBuffer.size() - 1] = '\0';
            break;
        case 2:
            DEBUG_PRINTLN("Неверный ID клиента");
            strncpy(mqttLastErrorBuffer.data(), "Неверный ID клиента", mqttLastErrorBuffer.size() - 1);
            mqttLastErrorBuffer[mqttLastErrorBuffer.size() - 1] = '\0';
            break;
        case 3:
            DEBUG_PRINTLN("Сервер недоступен");
            strncpy(mqttLastErrorBuffer.data(), "Сервер недоступен", mqttLastErrorBuffer.size() - 1);
            mqttLastErrorBuffer[mqttLastErrorBuffer.size() - 1] = '\0';
            break;
        case 4:
            DEBUG_PRINTLN("Неверные учетные данные");
            strncpy(mqttLastErrorBuffer.data(), "Неверные учетные данные", mqttLastErrorBuffer.size() - 1);
            mqttLastErrorBuffer[mqttLastErrorBuffer.size() - 1] = '\0';
            break;
        case 5:
            DEBUG_PRINTLN("Не авторизован");
            strncpy(mqttLastErrorBuffer.data(), "Не авторизован", mqttLastErrorBuffer.size() - 1);
            mqttLastErrorBuffer[mqttLastErrorBuffer.size() - 1] = '\0';
            break;
        default:
            DEBUG_PRINTLN("Неизвестная ошибка");
            strncpy(mqttLastErrorBuffer.data(), "Неизвестная ошибка", mqttLastErrorBuffer.size() - 1);
            mqttLastErrorBuffer[mqttLastErrorBuffer.size() - 1] = '\0';
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

        const char* otaCmdTopic = getOtaCommandTopic();
        mqttClient.subscribe(otaCmdTopic);
        DEBUG_PRINTF("[MQTT] Подписались на OTA команды: %s\n", otaCmdTopic);

        // Публикуем статус availability
        publishAvailabilityInternal(true);

        // Публикуем конфигурацию Home Assistant discovery если включено
        if (config.flags.hassEnabled)
        {
            publishHomeAssistantConfigInternal();
        }
    }

    return result;
}

void handleMQTTInternal()
{
    if (!config.flags.mqttEnabled)
    {
        return;
    }

    static bool wasConnected = false;
    const bool isConnected = mqttClient.connected();

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
            connectMQTTInternal();
        }
    }
    else
    {
        mqttClient.loop();

        // Публикуем статус OTA, если изменился (не чаще 5 сек)
        static std::array<char, 64> lastOtaStatus = {""};
        static unsigned long lastOtaPublish = 0;
        const unsigned long OTA_STATUS_INTERVAL = 5000;
        if (millis() - lastOtaPublish > OTA_STATUS_INTERVAL)
        {
            const char* cur = getOtaStatus();
            if (strcmp(cur, lastOtaStatus.data()) != 0)
            {
                mqttClient.publish(getOtaStatusTopic(), cur, true);
                strlcpy(lastOtaStatus.data(), cur, lastOtaStatus.size());
            }
            lastOtaPublish = millis();
        }
    }
}

// ДЕЛЬТА-ФИЛЬТР v2.2.1: Проверка необходимости публикации
bool shouldPublishMqtt()
{
    static int skipCounter = 0;

    // Первая публикация - всегда публикуем
    if (sensorData.last_mqtt_publish == 0)
    {
        DEBUG_PRINTLN("[MQTT DEBUG] Первая публикация - разрешено");
        return true;
    }

    // Принудительная публикация каждые N циклов (настраиваемо v2.3.0)
    if (++skipCounter >= config.forcePublishCycles)
    {
        skipCounter = 0;
        DEBUG_PRINTLN("[DELTA] Принудительная публикация (цикл)");
        return true;
    }

    DEBUG_PRINTF("[MQTT DEBUG] Проверка дельт: skipCounter=%d, forcePublishCycles=%d\n", skipCounter,
                 config.forcePublishCycles);

    // Проверяем дельта изменения
    bool hasSignificantChange = false;

    if (abs(sensorData.temperature - sensorData.prev_temperature) >= config.deltaTemperature)
    {
        DEBUG_PRINTF("[DELTA] Температура изменилась: %.1f -> %.1f (дельта=%.1f)\n", sensorData.prev_temperature,
                     sensorData.temperature, config.deltaTemperature);
        hasSignificantChange = true;
    }

    if (abs(sensorData.humidity - sensorData.prev_humidity) >= config.deltaHumidity)
    {
        DEBUG_PRINTF("[DELTA] Влажность изменилась: %.1f -> %.1f (дельта=%.1f)\n", sensorData.prev_humidity,
                     sensorData.humidity, config.deltaHumidity);
        hasSignificantChange = true;
    }

    if (abs(sensorData.ph - sensorData.prev_ph) >= config.deltaPh)
    {
        DEBUG_PRINTF("[DELTA] pH изменился: %.1f -> %.1f (дельта=%.1f)\n", sensorData.prev_ph, sensorData.ph,
                     config.deltaPh);
        hasSignificantChange = true;
    }

    if (abs(sensorData.ec - sensorData.prev_ec) >= config.deltaEc)
    {
        DEBUG_PRINTF("[DELTA] EC изменилась: %.0f -> %.0f (дельта=%.0f)\n", sensorData.prev_ec, sensorData.ec,
                     config.deltaEc);
        hasSignificantChange = true;
    }

    if (abs(sensorData.nitrogen - sensorData.prev_nitrogen) >= config.deltaNpk)
    {
        DEBUG_PRINTF("[DELTA] Азот изменился: %.0f -> %.0f (дельта=%.0f)\n", sensorData.prev_nitrogen,
                     sensorData.nitrogen, config.deltaNpk);
        hasSignificantChange = true;
    }

    if (abs(sensorData.phosphorus - sensorData.prev_phosphorus) >= config.deltaNpk)
    {
        DEBUG_PRINTF("[DELTA] Фосфор изменился: %.0f -> %.0f (дельта=%.0f)\n", sensorData.prev_phosphorus,
                     sensorData.phosphorus, config.deltaNpk);
        hasSignificantChange = true;
    }

    if (abs(sensorData.potassium - sensorData.prev_potassium) >= config.deltaNpk)
    {
        DEBUG_PRINTF("[DELTA] Калий изменился: %.0f -> %.0f (дельта=%.0f)\n", sensorData.prev_potassium,
                     sensorData.potassium, config.deltaNpk);
        hasSignificantChange = true;
    }

    if (hasSignificantChange)
    {
        skipCounter = 0;  // Сбрасываем счетчик при значимом изменении
        DEBUG_PRINTLN("[DELTA] Обнаружены значимые изменения - публикация разрешена");
    }
    else
    {
        DEBUG_PRINTLN("[DELTA] Изменения незначительные, пропускаем публикацию");
        DEBUG_PRINTF("[DELTA] Текущие значения: T=%.1f, H=%.1f, pH=%.1f, EC=%.0f, N=%.0f, P=%.0f, K=%.0f\n",
                     sensorData.temperature, sensorData.humidity, sensorData.ph, sensorData.ec, sensorData.nitrogen,
                     sensorData.phosphorus, sensorData.potassium);
        DEBUG_PRINTF("[DELTA] Предыдущие значения: T=%.1f, H=%.1f, pH=%.1f, EC=%.0f, N=%.0f, P=%.0f, K=%.0f\n",
                     sensorData.prev_temperature, sensorData.prev_humidity, sensorData.prev_ph, sensorData.prev_ec,
                     sensorData.prev_nitrogen, sensorData.prev_phosphorus, sensorData.prev_potassium);
    }

    return hasSignificantChange;
}

void publishSensorDataInternal()
{
    DEBUG_PRINTF("[MQTT DEBUG] mqttEnabled=%d, connected=%d, valid=%d\n", config.flags.mqttEnabled,
                 mqttClient.connected(), sensorData.valid);

    if (!config.flags.mqttEnabled || !mqttClient.connected() || !sensorData.valid)
    {
        DEBUG_PRINTLN("[MQTT DEBUG] Условия не выполнены, публикация отменена");
        return;
    }

    // ДЕЛЬТА-ФИЛЬТР v2.2.1: Проверяем необходимость публикации
    if (!shouldPublishMqtt())
    {
        DEBUG_PRINTLN("[MQTT DEBUG] Дельты не изменились, публикация отменена");
        return;
    }

    DEBUG_PRINTLN("[MQTT DEBUG] Начинаем публикацию данных...");

    // ✅ ОПТИМИЗАЦИЯ: Кэшируем JSON данных датчика
    const unsigned long currentTime = millis();
    bool needToRebuildJson = false;

    // Проверяем, нужно ли пересоздать JSON (данные обновились или кэш устарел)
    if (!sensorJsonCacheValid || (currentTime - lastCachedSensorTime > 1000) ||  // Кэш на 1 секунду
        (currentTime - sensorData.last_update < 100))                            // Свежие данные
    {
        needToRebuildJson = true;
    }

    if (needToRebuildJson)
    {
        // Пересоздаем JSON только при необходимости
        StaticJsonDocument<256> doc;  // ✅ Уменьшен размер с 512 до 256

        // ✅ ОПТИМИЗАЦИЯ 3.1: Сокращенные ключи для экономии трафика
        doc["t"] = round(sensorData.temperature * 10) / 10.0;                        // temperature → t (-10 байт)
        doc["h"] = round(sensorData.humidity * 10) / 10.0;                           // humidity → h (-7 байт)
        doc["e"] = (int)round(sensorData.ec);                                        // ec → e (стабильно)
        doc["p"] = round(sensorData.ph * 10) / 10.0;                                 // ph → p (стабильно)
        doc["n"] = (int)round(sensorData.nitrogen);                                  // nitrogen → n (-7 байт)
        doc["r"] = (int)round(sensorData.phosphorus);                                // phosphorus → r (-9 байт)
        doc["k"] = (int)round(sensorData.potassium);                                 // potassium → k (-8 байт)
        doc["ts"] = (long)(timeClient != nullptr ? timeClient->getEpochTime() : 0);  // timestamp → ts (-7 байт)

        // ✅ Кэшируем результат
        serializeJson(doc, cachedSensorJson.data(), cachedSensorJson.size());
        lastCachedSensorTime = currentTime;
        sensorJsonCacheValid = true;

        DEBUG_PRINTLN("[MQTT] Компактный JSON датчика пересоздан и закэширован");
    }

    // ✅ Кэшируем топик публикации
    static std::array<char, 128> stateTopicBuffer = {""};
    static bool stateTopicCached = false;
    if (!stateTopicCached)
    {
        snprintf(stateTopicBuffer.data(), stateTopicBuffer.size(), "%s/state", config.mqttTopicPrefix);
        stateTopicCached = true;
    }

    // Публикуем кэшированный JSON
    bool res = mqttClient.publish(stateTopicBuffer.data(), cachedSensorJson.data(), true);

    if (res)
    {
        mqttLastErrorBuffer.fill('\0');

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
        strlcpy(mqttLastErrorBuffer.data(), "Ошибка публикации MQTT", mqttLastErrorBuffer.size());
    }
}

void publishHomeAssistantConfigInternal()
{
    DEBUG_PRINTLN("[publishHomeAssistantConfig] Публикация discovery-конфигов Home Assistant...");
    if (!config.flags.mqttEnabled || !mqttClient.connected() || !config.flags.hassEnabled)
    {
        DEBUG_PRINTLN("[publishHomeAssistantConfig] Условия не выполнены, публикация отменена");
        return;
    }

    const String deviceIdStr = getDeviceId();
    const char* deviceId = deviceIdStr.c_str();

    // ✅ ОПТИМИЗАЦИЯ: Проверяем кэш конфигураций
    bool needToRebuildConfigs = false;
    if (!haConfigCache.isValid || strcmp(haConfigCache.cachedDeviceId.data(), deviceId) != 0 ||
        strcmp(haConfigCache.cachedTopicPrefix.data(), config.mqttTopicPrefix) != 0)
    {
        needToRebuildConfigs = true;
        DEBUG_PRINTLN("[HA] Кэш конфигураций устарел, пересоздаем...");
    }

    if (needToRebuildConfigs)
    {
        // Обновляем кэшированные значения
        strlcpy(haConfigCache.cachedDeviceId.data(), deviceId, haConfigCache.cachedDeviceId.size());
        strlcpy(haConfigCache.cachedTopicPrefix.data(), config.mqttTopicPrefix, haConfigCache.cachedTopicPrefix.size());

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
        serializeJson(tempConfig, haConfigCache.tempConfig.data(), haConfigCache.tempConfig.size());

        StaticJsonDocument<512> humConfig;
        humConfig["name"] = "JXCT Humidity";
        humConfig["device_class"] = "humidity";
        humConfig["state_topic"] = String(config.mqttTopicPrefix) + "/state";
        humConfig["unit_of_measurement"] = "%";
        humConfig["value_template"] = "{{ value_json.h }}";  // ✅ humidity → h
        humConfig["unique_id"] = String(deviceId) + "_hum";
        humConfig["availability_topic"] = String(config.mqttTopicPrefix) + "/status";
        humConfig["device"] = deviceInfo;
        serializeJson(humConfig, haConfigCache.humConfig.data(), haConfigCache.humConfig.size());

        StaticJsonDocument<512> ecConfig;
        ecConfig["name"] = "JXCT EC";
        ecConfig["device_class"] = "conductivity";
        ecConfig["state_topic"] = String(config.mqttTopicPrefix) + "/state";
        ecConfig["unit_of_measurement"] = "µS/cm";
        ecConfig["value_template"] = "{{ value_json.e }}";  // ✅ ec → e
        ecConfig["unique_id"] = String(deviceId) + "_ec";
        ecConfig["availability_topic"] = String(config.mqttTopicPrefix) + "/status";
        ecConfig["device"] = deviceInfo;
        serializeJson(ecConfig, haConfigCache.ecConfig.data(), haConfigCache.ecConfig.size());

        StaticJsonDocument<512> phConfig;
        phConfig["name"] = "JXCT pH";
        phConfig["device_class"] = "ph";
        phConfig["state_topic"] = String(config.mqttTopicPrefix) + "/state";
        phConfig["unit_of_measurement"] = "pH";
        phConfig["value_template"] = "{{ value_json.p }}";  // ✅ ph → p
        phConfig["unique_id"] = String(deviceId) + "_ph";
        phConfig["availability_topic"] = String(config.mqttTopicPrefix) + "/status";
        phConfig["device"] = deviceInfo;
        serializeJson(phConfig, haConfigCache.phConfig.data(), haConfigCache.phConfig.size());

        StaticJsonDocument<512> nitrogenConfig;
        nitrogenConfig["name"] = "JXCT Nitrogen";
        nitrogenConfig["state_topic"] = String(config.mqttTopicPrefix) + "/state";
        nitrogenConfig["unit_of_measurement"] = "mg/kg";
        nitrogenConfig["value_template"] = "{{ value_json.n }}";  // ✅ nitrogen → n
        nitrogenConfig["unique_id"] = String(deviceId) + "_nitrogen";
        nitrogenConfig["availability_topic"] = String(config.mqttTopicPrefix) + "/status";
        nitrogenConfig["device"] = deviceInfo;
        serializeJson(nitrogenConfig, haConfigCache.nitrogenConfig.data(), haConfigCache.nitrogenConfig.size());

        StaticJsonDocument<512> phosphorusConfig;
        phosphorusConfig["name"] = "JXCT Phosphorus";
        phosphorusConfig["state_topic"] = String(config.mqttTopicPrefix) + "/state";
        phosphorusConfig["unit_of_measurement"] = "mg/kg";
        phosphorusConfig["value_template"] = "{{ value_json.r }}";  // ✅ phosphorus → r
        phosphorusConfig["unique_id"] = String(deviceId) + "_phosphorus";
        phosphorusConfig["availability_topic"] = String(config.mqttTopicPrefix) + "/status";
        phosphorusConfig["device"] = deviceInfo;
        serializeJson(phosphorusConfig, haConfigCache.phosphorusConfig.data(), haConfigCache.phosphorusConfig.size());

        StaticJsonDocument<512> potassiumConfig;
        potassiumConfig["name"] = "JXCT Potassium";
        potassiumConfig["state_topic"] = String(config.mqttTopicPrefix) + "/state";
        potassiumConfig["unit_of_measurement"] = "mg/kg";
        potassiumConfig["value_template"] = "{{ value_json.k }}";  // ✅ potassium → k
        potassiumConfig["unique_id"] = String(deviceId) + "_potassium";
        potassiumConfig["availability_topic"] = String(config.mqttTopicPrefix) + "/status";
        potassiumConfig["device"] = deviceInfo;
        serializeJson(potassiumConfig, haConfigCache.potassiumConfig.data(), haConfigCache.potassiumConfig.size());

        // ✅ Кэшируем топики публикации
        snprintf(pubTopicCache[0].data(), pubTopicCache[0].size(), "homeassistant/sensor/%s_temperature/config",
                 deviceId);
        snprintf(pubTopicCache[1].data(), pubTopicCache[1].size(), "homeassistant/sensor/%s_humidity/config", deviceId);
        snprintf(pubTopicCache[2].data(), pubTopicCache[2].size(), "homeassistant/sensor/%s_ec/config", deviceId);
        snprintf(pubTopicCache[3].data(), pubTopicCache[3].size(), "homeassistant/sensor/%s_ph/config", deviceId);
        snprintf(pubTopicCache[4].data(), pubTopicCache[4].size(), "homeassistant/sensor/%s_nitrogen/config", deviceId);
        snprintf(pubTopicCache[5].data(), pubTopicCache[5].size(), "homeassistant/sensor/%s_phosphorus/config",
                 deviceId);
        snprintf(pubTopicCache[6].data(), pubTopicCache[6].size(), "homeassistant/sensor/%s_potassium/config",
                 deviceId);
        pubTopicCacheValid = true;

        haConfigCache.isValid = true;
        INFO_PRINTLN("[HA] Конфигурации созданы и закэшированы");
    }

    // ✅ Публикуем из кэша (супер быстро!)
    mqttClient.publish(pubTopicCache[0].data(), haConfigCache.tempConfig.data(), true);
    mqttClient.publish(pubTopicCache[1].data(), haConfigCache.humConfig.data(), true);
    mqttClient.publish(pubTopicCache[2].data(), haConfigCache.ecConfig.data(), true);
    mqttClient.publish(pubTopicCache[3].data(), haConfigCache.phConfig.data(), true);
    mqttClient.publish(pubTopicCache[4].data(), haConfigCache.nitrogenConfig.data(), true);
    mqttClient.publish(pubTopicCache[5].data(), haConfigCache.phosphorusConfig.data(), true);
    mqttClient.publish(pubTopicCache[6].data(), haConfigCache.potassiumConfig.data(), true);

    INFO_PRINTLN("[HA] Конфигурация Home Assistant опубликована из кэша");
    mqttLastErrorBuffer.fill('\0');
}

void removeHomeAssistantConfigInternal()
{
    const String deviceIdStr = getDeviceId();
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
    mqttLastErrorBuffer.fill('\0');
}

void handleMqttCommandInternal(const String& cmd)
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
        publishSensorDataInternal();
    }
    else if (cmd == "publish_discovery")
    {
        publishHomeAssistantConfigInternal();
    }
    else if (cmd == "remove_discovery")
    {
        removeHomeAssistantConfigInternal();
    }
    else if (cmd == "ota_check")
    {
        triggerOtaCheck();
        handleOTA();
    }
    else if (cmd == "ota_auto_on" || cmd == "ota_auto_off")
    {
        config.flags.autoOtaEnabled = (cmd == "ota_auto_on") ? 1 : 0;
        saveConfig();
        publishAvailabilityInternal(true);
    }
    else
    {
        DEBUG_PRINTLN("[MQTT] Неизвестная команда");
    }
}

void mqttCallbackInternal(const char* topic, const byte* payload, unsigned int length)
{
    const String topic_str = String(topic);
    const String message = String((char*)payload, length);

    if (topic_str == getCommandTopic())
    {
        handleMqttCommandInternal(message);
    }
}

void invalidateHAConfigCacheInternal()
{
    // Реализация инвалидации кэша Home Assistant конфигураций
    haConfigCache.isValid = false;
}

}  // namespace

// Глобальные обёртки для функций, объявленных в заголовке
void publishAvailability(bool online)
{
    publishAvailabilityInternal(online);
}

void setupMQTT()
{
    setupMQTTInternal();
}

bool connectMQTT()
{
    return connectMQTTInternal();
}

void handleMQTT()
{
    handleMQTTInternal();
}

void publishSensorData()
{
    publishSensorDataInternal();
}

void publishHomeAssistantConfig()
{
    publishHomeAssistantConfigInternal();
}

void removeHomeAssistantConfig()
{
    removeHomeAssistantConfigInternal();
}

void handleMqttCommand(const String& cmd)  // NOLINT(misc-use-internal-linkage)
{
    handleMqttCommandInternal(cmd);
}

void mqttCallback(const char* topic, const byte* payload, unsigned int length)  // NOLINT(misc-use-internal-linkage)
{
    mqttCallbackInternal(topic, payload, length);
}

void invalidateHAConfigCache()
{
    invalidateHAConfigCacheInternal();
}

const char* getMqttLastError()
{
    return mqttLastErrorBuffer.data();
}
