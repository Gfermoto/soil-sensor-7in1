#include "sensor_compensation.h"
#include <math.h>

// Коэффициенты температурной компенсации (по умолчанию)
constexpr float EC_TEMP_COEFF = 0.019f;   // 1.9%/°C
constexpr float PH_TEMP_COEFF = -0.03f;    // -0.03 pH/°C

float compensateEcByTemperature(float ecRaw, float temperature)
{
    // Стандартная температура 25°C
    float compensated = ecRaw / (1.0f + EC_TEMP_COEFF * (temperature - 25.0f));
    return compensated;
}

float compensatePhByTemperature(float phRaw, float temperature)
{
    float compensated = phRaw + PH_TEMP_COEFF * (temperature - 25.0f);
    return compensated;
}

float compensateNpkByMoisture(float npkRaw, float moisturePercent)
{
    // Простейшая линейная коррекция: снижение показаний на 0.2% за каждый % влажности выше 30
    if (moisturePercent <= 30.0f) return npkRaw;
    float factor = 1.0f - 0.002f * (moisturePercent - 30.0f);
    return npkRaw * factor;
}

float compensateNpkByPh(float npkRaw, float ph)
{
    // Эмпирическая кривая: максимум при pH 6.5
    float factor = 1.0f;
    if (ph < 6.5f)
        factor = 1.0f - (6.5f - ph) * 0.05f;  // -5% на каждые 0.1 ниже 6.5
    else if (ph > 6.5f)
        factor = 1.0f - (ph - 6.5f) * 0.03f;  // -3% на каждые 0.1 выше 6.5
    if (factor < 0.0f) factor = 0.0f;
    return npkRaw * factor;
}

float compensateNpkByEc(float npkRaw, float ec)
{
    // Снижение доступности питательных веществ при высокой EC
    if (ec < 1000.0f) return npkRaw;
    float factor = 1.0f - 0.0001f * (ec - 1000.0f);  // -1% на каждые 100 µS выше 1000
    if (factor < 0.0f) factor = 0.0f;
    return npkRaw * factor;
} 