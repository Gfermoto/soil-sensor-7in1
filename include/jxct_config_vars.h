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

// Оптимизированная упакованная структура конфигурации
struct __attribute__((packed)) Config
{
    // WiFi настройки
    char ssid[32];
    char password[32];         // Сократил с 64 до 32 (обычно достаточно)
    
    // MQTT настройки
    char mqttServer[48];       // Сократил с 64 до 48 байт
    uint16_t mqttPort;
    char mqttUser[24];         // Сократил с 32 до 24 байт
    char mqttPassword[24];     // Сократил с 32 до 24 байт
    char mqttTopicPrefix[48];  // Сократил с 64 до 48 байт
    char mqttDeviceName[24];   // Сократил с 32 до 24 байт
    uint8_t mqttQos;
    
    // ThingSpeak настройки
    char thingSpeakApiKey[24]; // Сократил с 32 до 24 байт
    char thingSpeakChannelId[12]; // Сократил с 16 до 12 байт
    uint16_t thingspeakInterval;
    
    // Информация о устройстве
    char manufacturer[24];     // Сократил с 32 до 24 байт
    char model[24];           // Сократил с 32 до 24 байт
    char swVersion[12];       // Сократил с 16 до 12 байт
    
    // NTP настройки
    char ntpServer[48];       // Сократил с 64 до 48 байт
    uint32_t ntpUpdateInterval;
    
    // Датчик настройки
    uint8_t modbusId;
    
    // Безопасность веб-интерфейса
    char webPassword[24];     // Пароль для доступа к веб-интерфейсу
    
    // Битовые поля для boolean флагов (экономия 4 байта)
    struct __attribute__((packed)) {
        uint8_t hassEnabled       : 1;  // 1 бит вместо 1 байта
        uint8_t useRealSensor     : 1;  // 1 бит вместо 1 байта
        uint8_t mqttEnabled       : 1;  // 1 бит вместо 1 байта
        uint8_t thingSpeakEnabled : 1;  // 1 бит вместо 1 байта
        uint8_t reserved          : 4;  // Резерв для будущих флагов
    } flags;
};

extern Config config;
extern Preferences preferences;

// Объявления функций работы с конфигом
void loadConfig();
void saveConfig();
void resetConfig();
bool isConfigValid();