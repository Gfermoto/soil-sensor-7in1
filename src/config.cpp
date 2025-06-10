/**
 * @file config.cpp
 * @brief Работа с конфигурацией устройства
 * @details Загрузка, сохранение, сброс и валидация настроек устройства через NVS (Preferences).
 */
#include <WiFi.h>
#include "config.h"
#include "jxct_device_info.h"
#include "jxct_config_vars.h"
#include "debug.h"  // ✅ Добавляем систему условной компиляции
#include "logger.h"

String getDeviceId()
{
    uint8_t mac[6];
    WiFi.macAddress(mac);
    char buf[32];
    snprintf(buf, sizeof(buf), "%s_%02X%02X%02X", DEVICE_MODEL, mac[3], mac[4], mac[5]);
    return String(buf);
}

String getDefaultTopic()
{
    uint8_t mac[6];
    WiFi.macAddress(mac);
    char buf[32];
    snprintf(buf, sizeof(buf), "jxct_%02X%02X%02X", mac[3], mac[4], mac[5]);
    return String(buf);
}

Config config;
Preferences preferences;

void loadConfig()
{
    preferences.begin("jxct-sensor", false);

    // WiFi настройки
    preferences.getString("ssid", config.ssid, sizeof(config.ssid));
    preferences.getString("password", config.password, sizeof(config.password));

    // MQTT настройки
    preferences.getString("mqttServer", config.mqttServer, sizeof(config.mqttServer));
    config.mqttPort = preferences.getUShort("mqttPort", 1883);
    preferences.getString("mqttUser", config.mqttUser, sizeof(config.mqttUser));
    preferences.getString("mqttPassword", config.mqttPassword, sizeof(config.mqttPassword));
    preferences.getString("mqttTopicPrefix", config.mqttTopicPrefix, sizeof(config.mqttTopicPrefix));
    preferences.getString("mqttDeviceName", config.mqttDeviceName, sizeof(config.mqttDeviceName));
    // Битовые поля boolean флагов
    config.flags.hassEnabled = preferences.getBool("hassEnabled", false);
    config.flags.useRealSensor = preferences.getBool("useRealSensor", false);
    config.flags.mqttEnabled = preferences.getBool("mqttEnabled", false);
    config.flags.thingSpeakEnabled = preferences.getBool("tsEnabled", false);
    
    config.mqttQos = preferences.getUChar("mqttQos", 0);
    config.thingspeakInterval = preferences.getUShort("tsInterval", 60);
    preferences.getString("manufacturer", config.manufacturer, sizeof(config.manufacturer));
    preferences.getString("model", config.model, sizeof(config.model));
    preferences.getString("swVersion", config.swVersion, sizeof(config.swVersion));

    // ThingSpeak настройки
    preferences.getString("tsApiKey", config.thingSpeakApiKey, sizeof(config.thingSpeakApiKey));
    preferences.getString("tsChannelId", config.thingSpeakChannelId, sizeof(config.thingSpeakChannelId));

    // Настройка датчика
    config.modbusId = preferences.getUChar("modbusId", JXCT_MODBUS_ID);

    // Безопасность веб-интерфейса
    preferences.getString("webPassword", config.webPassword, sizeof(config.webPassword));

    // NTP настройки
    String ntpServer = preferences.getString("ntpServer", "pool.ntp.org");
    ntpServer.toCharArray(config.ntpServer, sizeof(config.ntpServer));
    config.ntpUpdateInterval = preferences.getUInt("ntpUpdateInterval", 60000);

    preferences.end();
    // Значения по умолчанию для новых полей
    if (strlen(config.mqttDeviceName) == 0)
        strlcpy(config.mqttDeviceName, "JXCT_Device", sizeof(config.mqttDeviceName));
    if (strlen(config.thingSpeakChannelId) == 0)
        strlcpy(config.thingSpeakChannelId, "", sizeof(config.thingSpeakChannelId));
    if (strlen(config.mqttTopicPrefix) == 0)
        strlcpy(config.mqttTopicPrefix, getDefaultTopic().c_str(), sizeof(config.mqttTopicPrefix));

    logSuccess("Конфигурация загружена");
    logDebug("SSID: %s, MQTT: %s:%d, ThingSpeak: %s", config.ssid, config.mqttServer, config.mqttPort,
             config.flags.thingSpeakEnabled ? "включен" : "выключен");
}

void saveConfig()
{
    preferences.begin("jxct-sensor", false);

    // WiFi настройки
    preferences.putString("ssid", config.ssid);
    preferences.putString("password", config.password);

    // MQTT настройки
    preferences.putString("mqttServer", config.mqttServer);
    preferences.putUShort("mqttPort", config.mqttPort);
    preferences.putString("mqttUser", config.mqttUser);
    preferences.putString("mqttPassword", config.mqttPassword);
    preferences.putString("mqttTopicPrefix", config.mqttTopicPrefix);
    preferences.putString("mqttDeviceName", config.mqttDeviceName);
    
    // Битовые поля boolean флагов
    preferences.putBool("hassEnabled", config.flags.hassEnabled);
    preferences.putBool("useRealSensor", config.flags.useRealSensor);
    preferences.putBool("mqttEnabled", config.flags.mqttEnabled);
    preferences.putBool("tsEnabled", config.flags.thingSpeakEnabled);
    
    preferences.putUChar("mqttQos", config.mqttQos);
    preferences.putUShort("tsInterval", config.thingspeakInterval);
    preferences.putString("manufacturer", config.manufacturer);
    preferences.putString("model", config.model);
    preferences.putString("swVersion", config.swVersion);

    // ThingSpeak настройки
    preferences.putString("tsApiKey", config.thingSpeakApiKey);
    preferences.putString("tsChannelId", config.thingSpeakChannelId);

    // Настройка датчика
    preferences.putUChar("modbusId", config.modbusId);

    // Безопасность веб-интерфейса
    preferences.putString("webPassword", config.webPassword);

    // NTP настройки
    preferences.putString("ntpServer", config.ntpServer);
    preferences.putUInt("ntpUpdateInterval", config.ntpUpdateInterval);

    preferences.end();
    
    // ✅ Инвалидируем кэш MQTT конфигураций при изменении настроек
    extern void invalidateHAConfigCache();
    invalidateHAConfigCache();
    
    logSuccess("Конфигурация сохранена");
}

void resetConfig()
{
    logWarn("Сброс конфигурации...");
    preferences.begin("jxct-sensor", false);
    preferences.clear();
    preferences.end();
    // ✅ Безопасный сброс структуры config без memset для упакованных структур
    // Wi-Fi
    config.ssid[0] = '\0';
    config.password[0] = '\0';
    // MQTT
    config.mqttPort = 1883;
    config.modbusId = JXCT_MODBUS_ID;
    strlcpy(config.mqttTopicPrefix, getDefaultTopic().c_str(), sizeof(config.mqttTopicPrefix));
    strlcpy(config.mqttDeviceName, "JXCT_Device", sizeof(config.mqttDeviceName));
    
    // ✅ Явный сброс битовых полей
    config.flags.mqttEnabled = 0;
    config.flags.thingSpeakEnabled = 0;
    config.flags.hassEnabled = 0;
    config.flags.useRealSensor = 0;
    config.flags.reserved = 0;
    
    // ✅ Очистка всех строковых полей
    strlcpy(config.thingSpeakChannelId, "", sizeof(config.thingSpeakChannelId));
    strlcpy(config.mqttServer, "", sizeof(config.mqttServer));
    strlcpy(config.mqttUser, "", sizeof(config.mqttUser));
    strlcpy(config.mqttPassword, "", sizeof(config.mqttPassword));
    strlcpy(config.thingSpeakApiKey, "", sizeof(config.thingSpeakApiKey));
    strlcpy(config.manufacturer, "", sizeof(config.manufacturer));
    strlcpy(config.model, "", sizeof(config.model));
    strlcpy(config.swVersion, "", sizeof(config.swVersion));
    
    // ✅ Сброс числовых полей
    config.mqttQos = 0;
    config.thingspeakInterval = 60;
    
    // ✅ Безопасность веб-интерфейса
    strlcpy(config.webPassword, "", sizeof(config.webPassword));
    
    // NTP
    strlcpy(config.ntpServer, "pool.ntp.org", sizeof(config.ntpServer));
    config.ntpUpdateInterval = 60000;

    logSuccess("Все настройки сброшены к значениям по умолчанию");
    DEBUG_PRINT("[resetConfig] config.thingspeakInterval: ");
    DEBUG_PRINTLN(config.thingspeakInterval);
    DEBUG_PRINT("[resetConfig] config.manufacturer: ");
    DEBUG_PRINTLN(config.manufacturer);
    DEBUG_PRINT("[resetConfig] config.model: ");
    DEBUG_PRINTLN(config.model);
    DEBUG_PRINT("[resetConfig] config.swVersion: ");
    DEBUG_PRINTLN(config.swVersion);
    DEBUG_PRINT("[resetConfig] config.ntpServer: ");
    DEBUG_PRINTLN(config.ntpServer);
    DEBUG_PRINT("[resetConfig] config.ntpUpdateInterval: ");
    DEBUG_PRINTLN(config.ntpUpdateInterval);
}

bool isConfigValid()
{
    // Проверяем минимально необходимые настройки
    if (strlen(config.ssid) == 0)
    {
        return false;
    }

    // Если MQTT включен, проверяем настройки MQTT
    if (config.flags.mqttEnabled && strlen(config.mqttServer) == 0)
    {
        return false;
    }

    // Если ThingSpeak включен, проверяем API ключ
    if (config.flags.thingSpeakEnabled && strlen(config.thingSpeakApiKey) == 0)
    {
        return false;
    }

    return true;
}