/**
 * @file fake_sensor.cpp
 * @brief Эмуляция работы датчика для тестирования
 * @details Генерация тестовых данных для отладки и демонстрации работы системы без реального оборудования.
 */
#include "fake_sensor.h"
#include "modbus_sensor.h"
#include <Arduino.h>

void fakeSensorTask(void *pvParameters)
{
    for (;;)
    {
        sensorData.temperature = 22.0 + random(-50, 50) / 10.0;  // 17.0..27.0
        sensorData.humidity = 50.0 + random(-200, 200) / 10.0;   // 30..70
        sensorData.ec = 1000 + random(-200, 200);                // 800..1200
        sensorData.ph = 6.5 + random(-20, 20) / 10.0;            // 4.5..8.5
        sensorData.nitrogen = 30 + random(-10, 10);              // 20..40
        sensorData.phosphorus = 15 + random(-5, 5);              // 10..20
        sensorData.potassium = 20 + random(-5, 5);               // 15..25
        sensorData.valid = true;
        Serial.println("[fakeSensorTask] Сгенерированы тестовые данные датчика");
        vTaskDelay(10000 / portTICK_PERIOD_MS);  // каждые 10 секунд
    }
}

void startFakeSensorTask()
{
    xTaskCreate(fakeSensorTask, "FakeSensor", 4096, NULL, 1, NULL);
}