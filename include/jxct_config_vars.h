#pragma once

// Аппаратные пины
#define RX_PIN 16
#define TX_PIN 17
#define MAX485_DE 4  // Data Enable
#define MAX485_RE 5  // Receive Enable
#define BOOT_BUTTON 0
#define STATUS_LED_PIN 2

// Интервалы (мс)
#define SENSOR_READ_INTERVAL 5000
#define DATA_PUBLISH_INTERVAL 15000

// Modbus ID по умолчанию
#define JXCT_MODBUS_ID 1

// Константы для точки доступа
#define WIFI_AP_SSID "JXCT-Setup"
#define WIFI_AP_PASS "12345678"

// Экспорт глобальной конфигурации
#include <Preferences.h>
struct Config
{
    char ssid[32];
    char password[32];
    char mqttServer[64];
    uint16_t mqttPort;
    char mqttUser[32];
    char mqttPassword[32];
    char mqttTopicPrefix[64];
    char mqttDeviceName[32];
    bool hassEnabled;
    bool useRealSensor;
    uint8_t mqttQos;
    uint16_t thingspeakInterval;
    char manufacturer[32];
    char model[32];
    char swVersion[16];
    char thingSpeakApiKey[32];
    char thingSpeakChannelId[16];
    bool mqttEnabled;
    bool thingSpeakEnabled;
    uint8_t modbusId;
    char ntpServer[64];
    uint32_t ntpUpdateInterval;
};
extern Config config;
extern Preferences preferences;

// Объявления функций работы с конфигом
void loadConfig();
void saveConfig();
void resetConfig();
bool isConfigValid();