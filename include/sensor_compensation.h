#pragma once

#ifdef TEST_BUILD
#include "../test/stubs/esp32_stubs.h"
#else
#include "Arduino.h"
#endif

/**
 * @file sensor_compensation.h
 * @brief Алгоритмы коррекции показаний датчиков
 * @details Этот модуль содержит функции для температурной,
 * влажностной, pH-зависимой и EC-зависимой компенсации.
 */

// Профили почвы
enum class SoilProfile : uint8_t
{
    SAND = 0,
    LOAM = 1,
    PEAT = 2,
    CLAY = 3,
    SANDPEAT = 4
};

// Типы почвы
enum class SoilType : std::uint8_t
{
    SAND,
    LOAM,
    PEAT,
    CLAY,
    SANDPEAT
};

// Структуры для типобезопасности (предотвращение перепутывания параметров)
struct EnvironmentalConditions
{
    float temperature;  // °C
    float moisture;     // % влажности почвы (theta)
};

struct NPKReferences
{
    float nitrogen;    // мг/кг
    float phosphorus;  // мг/кг
    float potassium;   // мг/кг

    NPKReferences() : nitrogen(0), phosphorus(0), potassium(0) {}
    NPKReferences(float n, float p, float k) : nitrogen(n), phosphorus(p), potassium(k) {}
};

// (устаревшие функции компенсации удалены)

float correctEC(float ecRaw, float T, float theta, SoilType soil);
float correctPH(float T, float phRaw);
void correctNPK(float T, float theta, SoilType soil, NPKReferences& npk);

// ✅ Новые типобезопасные версии (предотвращают перепутывание параметров)
float correctEC(float ecRaw, const EnvironmentalConditions& env, SoilType soil);
void correctNPK(const EnvironmentalConditions& env, NPKReferences& npk, SoilType soil);
