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
    PEAT = 2
};

// ------------------ Температурная компенсация ------------------

/**
 * @brief Коррекция EC по температуре (классическая формула)
 * @param ecRaw Сырые показания EC (µS/cm)
 * @param temperature Температура °C
 * @return Компенсированное значение EC
 */
float compensateEcByTemperature(float ecRaw, float temperature);

/**
 * @brief Коррекция pH по температуре (Nernst equation упрощённо)
 */
float compensatePhByTemperature(float phRaw, float temperature);

// ------------------ Влажностная компенсация ------------------

/**
 * @brief Коррекция NPK по влажности
 */
float compensateNpkByMoisture(float npkRaw, float moisturePercent);

// ------------------ pH-зависимые коэффициенты ------------------

/**
 * @brief Коррекция NPK по pH (эмпирическая зависимость)
 */
float compensateNpkByPh(float npkRaw, float ph);

// ------------------ EC-зависимая коррекция ------------------

/**
 * @brief Коррекция NPK по EC
 */
float compensateNpkByEc(float npkRaw, float ec); 