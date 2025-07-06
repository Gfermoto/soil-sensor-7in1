#include "sensor_compensation.h"
#include <math.h>
#include <time.h>
#include "jxct_config_vars.h"

// --- коэффициенты ------------------------------------------------
static constexpr struct
{
    float k;
} SOIL_EC[] = {
    {0.15F},  // SAND
    {0.30F},  // LOAM
    {0.10F},  // PEAT
    {0.45F},  // CLAY
    {0.18F}   // SANDPEAT
};

static constexpr float k_t_N[5] = {0.0041F, 0.0038F, 0.0028F, 0.0032F, 0.0040F};
static constexpr float k_t_P[5] = {0.0053F, 0.0049F, 0.0035F, 0.0042F, 0.0051F};
static constexpr float k_t_K[5] = {0.0032F, 0.0029F, 0.0018F, 0.0024F, 0.0031F};

// влажностные коэффициенты (λ-функции заменить не можем, считаем прямо)
namespace {
inline float k_h_N(float theta)
{
    return 1.8F - 0.024F * theta;
}
inline float k_h_P(float theta)
{
    return 1.6F - 0.018F * theta;
}
inline float k_h_K(float theta)
{
    return 1.9F - 0.021F * theta;
}
}

// ------------------------------------------------------------------
// EC ----------------------------------------------------------------
float correctEC(float ecRaw, float T, float theta, SoilType soil)
{
    // Шаг 1. Температурная компенсация к 25 °C
    float ec25 = ecRaw / (1.0F + 0.021F * (T - 25.0F));

    // Шаг 2. Перевод к ECe (насыщенная паста)
    constexpr float THETA_SAT = 45.0F;  // %
    float k = SOIL_EC[(int)soil].k;
    return ec25 * powf(THETA_SAT / theta, 1.0F + k);
}

// pH ----------------------------------------------------------------
float correctPH(float phRaw, float T)
{
    // только температурная поправка (-0.003·ΔT)
    return phRaw - 0.003F * (T - 25.0F);
}

// NPK ---------------------------------------------------------------
void correctNPK(float T, float theta, float& N, float& P, float& K, SoilType soil)
{
    if (theta < 25.0F || theta > 60.0F)
    {
        return;  // валидация – оставляем как есть
    }

    int idx = (int)soil;

    // Температурная коррекция
    N *= (1.0F - k_t_N[idx] * (T - 25.0F));
    P *= (1.0F - k_t_P[idx] * (T - 25.0F));
    K *= (1.0F - k_t_K[idx] * (T - 25.0F));

    // Влажностная коррекция
    N *= k_h_N(theta);
    P *= k_h_P(theta);
    K *= k_h_K(theta);
}

// ✅ ТИПОБЕЗОПАСНЫЕ ВЕРСИИ (предотвращают перепутывание параметров)
// ------------------------------------------------------------------
float correctEC(float ecRaw, const EnvironmentalConditions& env, SoilType soil)
{
    return correctEC(ecRaw, env.temperature, env.moisture, soil);
}

void correctNPK(const EnvironmentalConditions& env, NPKReferences& npk, SoilType soil)
{
    correctNPK(env.temperature, env.moisture, npk.nitrogen, npk.phosphorus, npk.potassium, soil);
}
// ------------------------------------------------------------------
