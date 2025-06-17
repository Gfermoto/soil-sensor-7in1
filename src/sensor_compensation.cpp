#include "sensor_compensation.h"
#include "jxct_config_vars.h"
#include <math.h>
#include <time.h>

// Коэффициенты температурной компенсации (по умолчанию)
#define DEFAULT_EC_TEMP_COEFF 0.019f

// Значения по умолчанию для разных сред
static constexpr float EC_TEMP_COEFF_OUTDOOR_DRY = 0.022f;
static constexpr float EC_TEMP_COEFF_OUTDOOR_RAINY = 0.015f;
static constexpr float EC_TEMP_COEFF_GREENHOUSE = 0.021f;
static constexpr float EC_TEMP_COEFF_INDOOR = 0.020f;

constexpr float PH_TEMP_COEFF = -0.03f;    // -0.03 pH/°C

static float selectEcCoeff();

float compensateEcByTemperature(float ecRaw, float temperature)
{
    float k = selectEcCoeff();
    float compensated = ecRaw / (1.0f + k * (temperature - 25.0f));
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

static uint8_t getCurrentMonth()
{
    time_t now = time(nullptr);
    struct tm* tinfo = localtime(&now);
    if (!tinfo) return 1; // январь по умолчанию
    return tinfo->tm_mon + 1; // 1-12
}

static float selectEcCoeff()
{
    // По умолчанию
    float coeff = DEFAULT_EC_TEMP_COEFF;

    switch (config.environmentType)
    {
        case 1: // greenhouse
            coeff = EC_TEMP_COEFF_GREENHOUSE;
            break;
        case 2: // indoor
            coeff = EC_TEMP_COEFF_INDOOR;
            break;
        case 0: // outdoor
        default:
        {
            if (config.flags.seasonalAdjustEnabled)
            {
                uint8_t m = getCurrentMonth();
                bool rainy = (m == 4 || m == 5 || m == 6 || m == 10);
                coeff = rainy ? EC_TEMP_COEFF_OUTDOOR_RAINY : EC_TEMP_COEFF_OUTDOOR_DRY;
            }
            else
            {
                coeff = DEFAULT_EC_TEMP_COEFF;
            }
            break;
        }
    }
    return coeff;
}

float compensateEc(float ecRaw, float temperature, float moisturePercent)
{
    // 1. Температурная компенсация (универсальна для всех сред)
    float ecTemp = compensateEcByTemperature(ecRaw, temperature);

    // 2. Влажностная компенсация
    //   • Не используется для indoor (environmentType == 2)
    //   • Применяется только если влажность > 20 % во избежание деления на маленькие числа
    if (config.environmentType == 2) // indoor
    {
        return ecTemp; // без θ-коррекции
    }

    if (moisturePercent > 20.0f)
    {
        return ecTemp * (45.0f / moisturePercent);
    }

    return ecTemp;
} 