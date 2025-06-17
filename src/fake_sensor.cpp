/**
 * @file fake_sensor.cpp
 * @brief Эмуляция работы датчика для тестирования
 * @details Генерация тестовых данных для отладки и демонстрации работы системы без реального оборудования.
 */
#include "fake_sensor.h"
#include "modbus_sensor.h"
#include "sensor_compensation.h"
#include "jxct_config_vars.h"
#include "debug.h"  // ✅ Добавляем систему условной компиляции
#include <Arduino.h>

// ✅ Неблокирующая задача эмуляции датчика с оптимизированным циклом
void fakeSensorTask(void *pvParameters)
{
    const TickType_t taskDelay = 1000 / portTICK_PERIOD_MS;  // 1 секунда
    const uint32_t dataGenerationInterval = 10;              // Генерация данных каждые 10 итераций
    uint32_t iterationCounter = 0;

    for (;;)
    {
        // Генерируем данные только каждые 10 секунд
        if (iterationCounter >= dataGenerationInterval)
        {
            sensorData.temperature = 22.0 + random(-50, 50) / 10.0;  // 17.0..27.0
            sensorData.humidity = 50.0 + random(-200, 200) / 10.0;   // 30..70
            sensorData.ec = 1000 + random(-200, 200);                // 800..1200
            sensorData.ph = 6.5 + random(-20, 20) / 10.0;            // 4.5..8.5
            sensorData.nitrogen = 30 + random(-10, 10);              // 20..40
            sensorData.phosphorus = 15 + random(-5, 5);              // 10..20
            sensorData.potassium = 20 + random(-5, 5);               // 15..25
            sensorData.valid = true;
            sensorData.last_update = millis();  // ✅ Обновляем timestamp

            // RAW значения до компенсации
            sensorData.raw_temperature = sensorData.temperature;
            sensorData.raw_humidity = sensorData.humidity;
            sensorData.raw_ec = sensorData.ec;
            sensorData.raw_ph = sensorData.ph;
            sensorData.raw_nitrogen = sensorData.nitrogen;
            sensorData.raw_phosphorus = sensorData.phosphorus;
            sensorData.raw_potassium = sensorData.potassium;

            // Применяем компенсацию, если включена
            if (config.flags.calibrationEnabled)
            {
                SoilType soil;
                switch (config.soilProfile)
                {
                    case 0:  soil = SoilType::SAND; break;
                    case 1:  soil = SoilType::LOAM; break;
                    case 2:  soil = SoilType::PEAT; break;
                    case 3:  soil = SoilType::CLAY; break;
                    default: soil = SoilType::LOAM; break;
                }

                // 1. EC: температурная компенсация → затем модель Арчи
                float ec25 = sensorData.ec / (1.0f + 0.021f * (sensorData.temperature - 25.0f));
                sensorData.ec = correctEC(ec25,
                                          sensorData.temperature,
                                          sensorData.humidity,
                                          soil);

                // 2. pH: температурная поправка
                sensorData.ph = correctPH(sensorData.ph, sensorData.temperature);

                // 3. NPK: зависимость от T, θ и типа почвы
                correctNPK(sensorData.temperature,
                           sensorData.humidity,
                           sensorData.nitrogen,
                           sensorData.phosphorus,
                           sensorData.potassium,
                           soil);
            }

            DEBUG_PRINTLN("[fakeSensorTask] Сгенерированы тестовые данные датчика");
            iterationCounter = 0;  // Сброс счетчика
        }

        iterationCounter++;

        // ✅ Более частые, но короткие задержки для отзывчивости
        vTaskDelay(taskDelay);
    }
}

void startFakeSensorTask()
{
    xTaskCreate(fakeSensorTask, "FakeSensor", 4096, NULL, 1, NULL);
}