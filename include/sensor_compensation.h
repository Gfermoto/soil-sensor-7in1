#pragma once

#include <Arduino.h>

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
enum class SoilType { SAND, LOAM, PEAT, CLAY, SANDPEAT };

// (устаревшие функции компенсации удалены)

float correctEC(float ecRaw, float T, float theta, SoilType soil);
float correctPH(float phRaw, float T);
void  correctNPK(float T, float theta,
                 float& N, float& P, float& K, SoilType soil); 