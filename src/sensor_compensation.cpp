#include "sensor_compensation.h"
#include <math.h>
#include <time.h>
#include "jxct_config_vars.h"

// --- коэффициенты ------------------------------------------------
static constexpr struct
{
    float k;
} SOIL_EC[] = {
    {0.15f},  // SAND
    {0.30f},  // LOAM
    {0.10f},  // PEAT
    {0.45f},  // CLAY
    {0.18f}   // SANDPEAT
};

static constexpr float k_t_N[5] = {0.0041f, 0.0038f, 0.0028f, 0.0032f, 0.0040f};
static constexpr float k_t_P[5] = {0.0053f, 0.0049f, 0.0035f, 0.0042f, 0.0051f};
static constexpr float k_t_K[5] = {0.0032f, 0.0029f, 0.0018f, 0.0024f, 0.0031f};

// влажностные коэффициенты (λ-функции заменить не можем, считаем прямо)
static inline float k_h_N(float th)
{
    return 1.8f - 0.024f * th;
}
static inline float k_h_P(float th)
{
    return 1.6f - 0.018f * th;
}
static inline float k_h_K(float th)
{
    return 1.9f - 0.021f * th;
}

// ------------------------------------------------------------------
// EC ----------------------------------------------------------------
float correctEC(float ecRaw, float T, float theta, SoilType soil)
{
    // Шаг 1. Температурная компенсация к 25 °C
    float ec25 = ecRaw / (1.0f + 0.021f * (T - 25.0f));

    // Шаг 2. Перевод к ECe (насыщенная паста)
    constexpr float THETA_SAT = 45.0f;  // %
    float k = SOIL_EC[(int)soil].k;
    return ec25 * powf(THETA_SAT / theta, 1.0f + k);
}

// pH ----------------------------------------------------------------
float correctPH(float phRaw, float T)
{
    // только температурная поправка (-0.003·ΔT)
    return phRaw - 0.003f * (T - 25.0f);
}

// NPK ---------------------------------------------------------------
void correctNPK(float T, float theta, float& N, float& P, float& K, SoilType soil)
{
    if (theta < 25.0f || theta > 60.0f) return;  // валидация – оставляем как есть

    int idx = (int)soil;

    // Температурная коррекция
    N *= (1.0f - k_t_N[idx] * (T - 25.0f));
    P *= (1.0f - k_t_P[idx] * (T - 25.0f));
    K *= (1.0f - k_t_K[idx] * (T - 25.0f));

    // Влажностная коррекция
    N *= k_h_N(theta);
    P *= k_h_P(theta);
    K *= k_h_K(theta);
}
// ------------------------------------------------------------------
