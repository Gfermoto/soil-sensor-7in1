#include "sensor_compensation.h"
#include <cmath>
#include <ctime>
#include "jxct_config_vars.h"

// --- коэффициенты ------------------------------------------------
struct SoilECCoeff
{
    float k;
};

static constexpr std::array<SoilECCoeff, 5> SOIL_EC = {{
    {0.15F},  // SAND
    {0.30F},  // LOAM
    {0.10F},  // PEAT
    {0.45F},  // CLAY
    {0.18F}   // SANDPEAT
}};

static constexpr std::array<float, 5> k_t_N = {0.0041F, 0.0038F, 0.0028F, 0.0032F, 0.0040F};
static constexpr std::array<float, 5> k_t_P = {0.0053F, 0.0049F, 0.0035F, 0.0042F, 0.0051F};
static constexpr std::array<float, 5> k_t_K = {0.0032F, 0.0029F, 0.0018F, 0.0024F, 0.0031F};

// влажностные коэффициенты (λ-функции заменить не можем, считаем прямо)
namespace
{
inline float k_h_N(float theta)
{
    return 1.8F - (0.024F * theta);
}
inline float k_h_P(float theta)
{
    return 1.6F - (0.018F * theta);
}
inline float k_h_K(float theta)
{
    return 1.9F - (0.021F * theta);
}
}  // namespace

// Строгая типизация для предотвращения ошибок
struct ECCompensationParams
{
    float temperature;
    float moisture;
    SoilType soilType;

    ECCompensationParams(float temp, float moist, SoilType soil) : temperature(temp), moisture(moist), soilType(soil) {}  // NOLINT(bugprone-easily-swappable-parameters)
    
    // Статический метод-фабрика для безопасного создания
    static ECCompensationParams create(float temp, float moist, SoilType soil) {
        return {temp, moist, soil};
    }
};

// ------------------------------------------------------------------
// EC ----------------------------------------------------------------
float correctEC(float ecRaw, const ECCompensationParams& params)  // NOLINT(misc-use-internal-linkage)
{
    // Шаг 1. Температурная компенсация к 25 °C
    float ec25 = ecRaw / (1.0F + 0.021F * (params.temperature - 25.0F));

    // Шаг 2. Перевод к ECe (насыщенная паста)
    constexpr float THETA_SAT = 45.0F;  // %
    const float soilCoeff = SOIL_EC[static_cast<int>(params.soilType)].k;
    return ec25 * powf(THETA_SAT / params.moisture, 1.0F + soilCoeff);
}

// pH ----------------------------------------------------------------
float correctPH(float temperature, float phRaw)
{
    // только температурная поправка (-0.003·ΔT)
    return phRaw - (0.003F * (temperature - 25.0F));
}

// NPK ---------------------------------------------------------------
void correctNPK(const ECCompensationParams& params, NPKReferences& npk)  // NOLINT(misc-use-internal-linkage)
{
    if (params.moisture < 25.0F || params.moisture > 60.0F)
    {
        return;  // валидация – оставляем как есть
    }

    const int idx = static_cast<int>(params.soilType);

    // Температурная коррекция
    npk.nitrogen *= (1.0F - (k_t_N[idx] * (params.temperature - 25.0F)));
    npk.phosphorus *= (1.0F - (k_t_P[idx] * (params.temperature - 25.0F)));
    npk.potassium *= (1.0F - (k_t_K[idx] * (params.temperature - 25.0F)));

    // Влажностная коррекция
    npk.nitrogen *= k_h_N(params.moisture);
    npk.phosphorus *= k_h_P(params.moisture);
    npk.potassium *= k_h_K(params.moisture);
}

// ✅ ТИПОБЕЗОПАСНЫЕ ВЕРСИИ (предотвращают перепутывание параметров)
// ------------------------------------------------------------------
float correctEC(float ecRaw, const EnvironmentalConditions& env, SoilType soil)
{
    return correctEC(ecRaw, ECCompensationParams::create(env.temperature, env.moisture, soil));
}

void correctNPK(const EnvironmentalConditions& env, NPKReferences& npk, SoilType soil)
{
    correctNPK(ECCompensationParams::create(env.temperature, env.moisture, soil), npk);
}
// ------------------------------------------------------------------

// --- ОБРАТНАЯ СОВМЕСТИМОСТЬ: старые сигнатуры (тонкие обёртки) ---
float correctEC(float ecRaw, float temperature, float theta, SoilType soil)
{
    return correctEC(ecRaw, EnvironmentalConditions{temperature, theta}, soil);
}

void correctNPK(float temperature, float theta, SoilType soil, NPKReferences& npk)
{
    correctNPK(EnvironmentalConditions{temperature, theta}, npk, soil);
}
// ------------------------------------------------------------------
