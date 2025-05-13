#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>
#include <Preferences.h>

// Пины
#define RX_PIN 16       // UART RX pin для Modbus
#define TX_PIN 17       // UART TX pin для Modbus
#define MAX485_DE_RE 5  // DE/RE pin для MAX485

// Константы
#define WIFI_AP_SSID "JXCT_SENSOR"
#define WIFI_AP_PASS "12345678"
#define CONFIGURATION_PORTAL_TIMEOUT 180 // в секундах
#define WIFI_RECONNECT_INTERVAL 30000   // в миллисекундах
#define SENSOR_READ_INTERVAL 10000      // в миллисекундах
#define DATA_PUBLISH_INTERVAL 60000     // в миллисекундах
#define BOOT_BUTTON_RESET_TIME 5000     // в миллисекундах
#define JXCT_MODBUS_ID 1                // Modbus ID датчика

// Структура для хранения настроек
struct Config {
    // WiFi настройки
    char ssid[32];
    char password[64];
    
    // MQTT настройки
    char mqttServer[64];
    uint16_t mqttPort;
    char mqttUser[32];
    char mqttPassword[64];
    char mqttTopicPrefix[32];
    char mqttDeviceName[32]; // Имя устройства для MQTT
    bool hassEnabled;        // Интеграция с Home Assistant
    
    // ThingSpeak настройки
    char thingSpeakApiKey[32];
    char thingSpeakChannelId[16]; // Channel ID
    
    // Сервисные настройки
    bool mqttEnabled;
    bool thingSpeakEnabled;
    
    // Настройка датчика
    uint8_t modbusId;
    bool useRealSensor;
    
    uint8_t mqttQos; // QoS для MQTT
    uint16_t thingspeakInterval; // Интервал публикации ThingSpeak (сек)
    char manufacturer[32]; // Производитель устройства
    char model[32];        // Модель устройства
    char swVersion[16];    // Версия ПО
};

extern Config config;
extern Preferences preferences;

// Функции для работы с настройками
void loadConfig();
void saveConfig();
void resetConfig();
bool isConfigValid();

#endif // CONFIG_H 