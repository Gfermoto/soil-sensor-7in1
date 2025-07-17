#include "esp32_stubs.h"

// Реализация заглушек Arduino функций
unsigned long millis()
{
    return 0;
}

unsigned long micros()
{
    return 0;
}

void delay(unsigned long ms)
{
    // Ничего не делаем в тестах
}

void delayMicroseconds(unsigned long us)
{
    // Ничего не делаем в тестах
}

void digitalWrite(uint8_t pin, uint8_t state)
{
    // Ничего не делаем в тестах
}

int digitalRead(uint8_t pin)
{
    return 0;
}

void pinMode(uint8_t pin, uint8_t mode)
{
    // Ничего не делаем в тестах
}

int analogRead(uint8_t pin)
{
    return 0;
}

void analogWrite(uint8_t pin, int value)
{
    // Ничего не делаем в тестах
}

void yield()
{
    // Ничего не делаем в тестах
}

void esp_restart()
{
    // Ничего не делаем в тестах
}

// Создаем экземпляры глобальных объектов
SerialClass Serial;
HardwareSerial Serial2(2);
HardwareSerial Serial3(3);
WiFiClass WiFi;
WebServer server(80);
LittleFSClass LittleFS;
ModbusMaster modbus;
PubSubClient mqtt;
HTTPClient http;
JsonDocument doc;
JsonObject obj;
JsonArray arr;
EEPROMClass EEPROM;
