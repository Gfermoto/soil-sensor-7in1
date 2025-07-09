/**
 * @file fake_sensor.cpp
 * @brief Эмуляция работы датчика для тестирования
 * @details Генерация тестовых данных для отладки и демонстрации работы системы без реального оборудования.
 */
#include "fake_sensor.h"
#include <Arduino.h>
#include "debug.h"  // ✅ Добавляем систему условной компиляции
#include "jxct_config_vars.h"
#include "modbus_sensor.h"
#include "sensor_compensation.h"

namespace
{
void fakeSensorTask(void* pvParameters)
{
    (void)pvParameters;                                      // Suppress unused parameter warning
    const TickType_t taskDelay = 1000 / portTICK_PERIOD_MS;  // 1 секунда
    const uint32_t dataGenerationInterval = 10;              // Генерация данных каждые 10 итераций
    uint32_t iterationCounter = 0;

    for (;;)
    {
        // Генерируем данные только каждые 10 секунд
        if (iterationCounter >= dataGenerationInterval)
        {
            sensorData.temperature = 22.0F + static_cast<float>(random(-50, 50)) / 10.0F;  // 17.0..27.0
            sensorData.humidity = 50.0F + static_cast<float>(random(-200, 200)) / 10.0F;   // 30..70
            sensorData.ec = 1000 + static_cast<float>(random(-200, 200));                  // 800..1200
            sensorData.ph = 6.5F + static_cast<float>(random(-20, 20)) / 10.0F;            // 4.5..8.5

            // NPK в мг/кг (как в реальном датчике JXCT)
            // Генерируем данные в диапазоне реального датчика 0-2000 мг/кг
            const float nitrogen_mgkg = 800.0F + static_cast<float>(random(-300, 300));   // 500..1100
            const float phosphorus_mgkg = 400.0F + static_cast<float>(random(-200, 200));  // 200..600
            const float potassium_mgkg = 800.0F + static_cast<float>(random(-300, 300));   // 500..1100

            // Данные уже в мг/кг, конверсия не нужна
            NPKReferences npk{nitrogen_mgkg, phosphorus_mgkg, potassium_mgkg};

            sensorData.valid = true;
            sensorData.last_update = millis();  // ✅ Обновляем timestamp

            // RAW значения до компенсации
            sensorData.raw_temperature = sensorData.temperature;
            sensorData.raw_humidity = sensorData.humidity;
            sensorData.raw_ec = sensorData.ec;
            sensorData.raw_ph = sensorData.ph;
            // RAW до компенсации, но уже в правильных единицах (мг/кг)
            sensorData.raw_nitrogen = npk.nitrogen;
            sensorData.raw_phosphorus = npk.phosphorus;
            sensorData.raw_potassium = npk.potassium;

            // КРИТИЧЕСКОЕ ИСПРАВЛЕНИЕ: всегда сохраняем NPK данные в sensorData
            sensorData.nitrogen = npk.nitrogen;
            sensorData.phosphorus = npk.phosphorus;
            sensorData.potassium = npk.potassium;

            // Применяем компенсацию, если включена
            if (config.flags.calibrationEnabled)
            {
                SoilType soil;
                switch (config.soilProfile)
                {
                    case 0:
                        soil = SoilType::SAND;
                        break;
                    case 1:
                        soil = SoilType::LOAM;
                        break;
                    case 2:
                        soil = SoilType::PEAT;
                        break;
                    case 3:
                        soil = SoilType::CLAY;
                        break;
                    case 4:
                        soil = SoilType::SANDPEAT;
                        break;
                    default:
                        soil = SoilType::LOAM;
                        break;
                }

                // 1. EC: единоразовая температурная + влажностная компенсация
                sensorData.ec = correctEC(sensorData.ec, sensorData.temperature, sensorData.humidity, soil);

                // 2. pH: температурная поправка
                sensorData.ph = correctPH(sensorData.ph, sensorData.temperature);

                // 3. NPK: зависимость от T, θ и типа почвы
                correctNPK(sensorData.temperature, sensorData.humidity, npk, soil);
                
                // Сохраняем скорректированные NPK данные в sensorData
                sensorData.nitrogen = npk.nitrogen;
                sensorData.phosphorus = npk.phosphorus;
                sensorData.potassium = npk.potassium;
            }

            DEBUG_PRINTLN("[fakeSensorTask] Сгенерированы тестовые данные датчика");
            iterationCounter = 0;  // Сброс счетчика
        }

        iterationCounter++;

        // ✅ Более частые, но короткие задержки для отзывчивости
        vTaskDelay(taskDelay);
    }
}

// forward declaration; реализация глобальная ниже
void startFakeSensorTask();
}  // anonymous namespace

// Определение-обёртка с внешним связыванием
void startFakeSensorTask()
{
    xTaskCreate(fakeSensorTask, "FakeSensor", 4096, nullptr, 1, nullptr);
}
