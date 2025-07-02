#ifndef THINGSPEAK_CLIENT_H
#define THINGSPEAK_CLIENT_H

#include <Arduino.h>
#include <ThingSpeak.h>
#include <WiFiClient.h>

extern WiFiClient espClient;

// ✅ Заменяем String на функции-геттеры для совместимости
const char* getThingSpeakLastPublish();
const char* getThingSpeakLastError();

// Инициализация ThingSpeak клиента (передаём WiFiClient)
void setupThingSpeak(WiFiClient& client);

// Отправка данных в ThingSpeak (с учётом интервала)
bool sendDataToThingSpeak();

#endif  // THINGSPEAK_CLIENT_H
