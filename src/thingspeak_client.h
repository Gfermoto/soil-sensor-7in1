#ifndef THINGSPEAK_CLIENT_H
#define THINGSPEAK_CLIENT_H

#include <WiFiClient.h>
#include <Arduino.h>
#include <ThingSpeak.h>

extern WiFiClient espClient;
extern String thingSpeakLastPublish;
extern String thingSpeakLastError;

// Инициализация ThingSpeak клиента (передаём WiFiClient)
void setupThingSpeak(WiFiClient& client);

// Отправка данных в ThingSpeak (с учётом интервала)
bool sendDataToThingSpeak();

#endif  // THINGSPEAK_CLIENT_H