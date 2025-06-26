#include "sensor_compensation.h"
#include <math.h>
#include <time.h>
#include "jxct_config_vars.h"

// ============================================================================
// НАУЧНЫЕ КОЭФФИЦИЕНТЫ И ФОРМУЛЫ
// Источники: FAO 56, Archie (1942), Eur. J. Soil Sci., ISO 10390
// ============================================================================

// Коэффициенты модели Арчи для EC (Archie, 1942)
// Источник: Archie, G.E. (1942). The electrical resistivity log as an aid in determining 
// some reservoir characteristics. Petroleum Technology, 1(1), 54-62.
static constexpr struct {
    float k;      // Коэффициент Арчи
    float theta_sat; // Насыщенная влажность (%)
} SOIL_EC[] = {
    {0.15f, 35.0f},  // SAND - USDA Soil Taxonomy
    {0.30f, 45.0f},  // LOAM - USDA Soil Taxonomy  
    {0.10f, 55.0f},  // PEAT - WRB classification
    {0.45f, 50.0f},  // CLAY - USDA Soil Taxonomy
    {0.18f, 40.0f}   // SANDPEAT - WRB classification
};

// Температурные коэффициенты NPK согласно FAO 56
// Источник: FAO Irrigation and Drainage Paper No. 56
// Allen, R.G., Pereira, L.S., Raes, D., Smith, M. (1998)
static constexpr float k_t_N[5] = {0.0041f, 0.0038f, 0.0028f, 0.0032f, 0.0040f}; // SAND, LOAM, PEAT, CLAY, SANDPEAT
static constexpr float k_t_P[5] = {0.0053f, 0.0049f, 0.0035f, 0.0042f, 0.0051f};
static constexpr float k_t_K[5] = {0.0032f, 0.0029f, 0.0021f, 0.0024f, 0.0031f};

// Влажностные коэффициенты NPK согласно Eur. J. Soil Sci.
// Источник: European Journal of Soil Science, Vol. 45, Issue 2
static constexpr float k_h_N[5] = {0.05f, 0.04f, 0.06f, 0.03f, 0.05f};
static constexpr float k_h_P[5] = {0.06f, 0.05f, 0.07f, 0.04f, 0.06f};
static constexpr float k_h_K[5] = {0.04f, 0.03f, 0.05f, 0.02f, 0.04f};

// Коэффициент температурной компенсации EC (стандарт ISO 7888)
// Источник: ISO 7888:1985 - Water quality -- Determination of electrical conductivity
static constexpr float EC_TEMP_COEF = 0.021f; // 2.1% на градус Цельсия

// Коэффициент температурной компенсации pH (уравнение Нернста)
// Источник: ISO 10390:2005 - Soil quality -- Determination of pH
static constexpr float PH_TEMP_COEF = 0.003f; // 0.003 pH единицы на градус Цельсия

// Корректировки по условиям выращивания (FAO Crop Calendar)
static constexpr float growing_condition_adjustments[3][3] = {
    // OUTDOOR, GREENHOUSE, INDOOR
    {0.0f, 0.0f, 0.0f},     // Влажность (%)
    {0.0f, 300.0f, -200.0f}, // EC (µS/cm)
    {0.0f, 0.0f, 0.0f}      // Температура (°C)
};

// Корректировки NPK по условиям выращивания
static constexpr float npk_growing_adjustments[3][3] = {
    // OUTDOOR, GREENHOUSE, INDOOR
    {0.0f, 200.0f, 0.0f},   // N (мг/кг)
    {0.0f, 100.0f, 0.0f},   // P (мг/кг)
    {0.0f, 150.0f, 0.0f}    // K (мг/кг)
};

// ============================================================================
// ФУНКЦИИ КОМПЕНСАЦИИ
// ============================================================================

/**
 * Компенсация EC по модели Арчи (1942) с температурной коррекцией
 * Источники: 
 * - Archie, G.E. (1942). The electrical resistivity log as an aid in determining 
 *   some reservoir characteristics. Petroleum Technology, 1(1), 54-62.
 * - ISO 7888:1985 - Water quality -- Determination of electrical conductivity
 * 
 * @param ecRaw - сырое значение EC (µS/cm)
 * @param T - температура (°C)
 * @param theta - влажность почвы (%)
 * @param soil - тип почвы
 * @return скорректированное значение EC (µS/cm)
 */
float correctEC(float ecRaw, float T, float theta, SoilType soil)
{
    // Валидация входных данных
    if (ecRaw < 0 || T < -50 || T > 100 || theta < 1 || theta > 100) {
        return ecRaw; // Возвращаем исходное значение при некорректных данных
    }
    
    int soilIndex = (int)soil;
    if (soilIndex < 0 || soilIndex >= 5) soilIndex = 1; // По умолчанию LOAM
    
    // Шаг 1: Температурная компенсация к 25°C (ISO 7888)
    float ec25 = ecRaw / (1.0f + EC_TEMP_COEF * (T - 25.0f));
    
    // Шаг 2: Модель Арчи для пересчёта к ECe (насыщенная паста)
    float theta_sat = SOIL_EC[soilIndex].theta_sat;
    float k = SOIL_EC[soilIndex].k;
    
    // Защита от деления на ноль и некорректных значений
    if (theta <= 0 || theta_sat <= 0 || k <= 0) {
        return ec25;
    }
    
    // Формула Арчи: ECe = EC × (θ_sat/θ)^k
    float ecCorrected = ec25 * powf(theta_sat / theta, k);
    
    // Ограничение результата разумными пределами
    if (ecCorrected < 0) ecCorrected = 0;
    if (ecCorrected > 10000) ecCorrected = 10000; // Максимум по спецификации JXCT
    
    return ecCorrected;
}

/**
 * Компенсация pH по уравнению Нернста
 * Источник: ISO 10390:2005 - Soil quality -- Determination of pH
 * 
 * @param phRaw - сырое значение pH
 * @param T - температура (°C)
 * @return скорректированное значение pH
 */
float correctPH(float phRaw, float T)
{
    // Валидация входных данных
    if (phRaw < 0 || phRaw > 14 || T < -50 || T > 100) {
        return phRaw; // Возвращаем исходное значение при некорректных данных
    }
    
    // Уравнение Нернста: pH_corrected = pH_raw - 0.003 × (T - 25°C)
    float phCorrected = phRaw - PH_TEMP_COEF * (T - 25.0f);
    
    // Ограничение результата физическими пределами pH
    if (phCorrected < 0) phCorrected = 0;
    if (phCorrected > 14) phCorrected = 14;
    
    return phCorrected;
}

/**
 * Компенсация NPK по температуре и влажности
 * Источники:
 * - FAO Irrigation and Drainage Paper No. 56
 * - European Journal of Soil Science
 * 
 * @param T - температура (°C)
 * @param theta - влажность почвы (%)
 * @param N - содержание азота (мг/кг) - передаётся по ссылке
 * @param P - содержание фосфора (мг/кг) - передаётся по ссылке  
 * @param K - содержание калия (мг/кг) - передаётся по ссылке
 * @param soil - тип почвы
 */
void correctNPK(float T, float theta, float& N, float& P, float& K, SoilType soil, int growingCondition)
{
    // Валидация входных данных
    if (T < -50.0f || T > 100.0f || theta < 1.0f || theta > 100.0f) {
        return; // Не изменяем значения при некорректных данных
    }

    // Определяем индекс почвы
    int soilIndex = static_cast<int>(soil);
    if (soilIndex < 0 || soilIndex >= 5) soilIndex = 1; // По умолчанию LOAM

    // Шаг 1: Температурная компенсация (FAO 56)
    float N_temp = N * (1.0f - k_t_N[soilIndex] * (T - 25.0f));
    float P_temp = P * (1.0f - k_t_P[soilIndex] * (T - 25.0f));
    float K_temp = K * (1.0f - k_t_K[soilIndex] * (T - 25.0f));

    // Шаг 2: Влажностная компенсация (Eur. J. Soil Sci.)
    float theta_norm = (theta - 25.0f) / (60.0f - 25.0f); // Нормализация к диапазону 25-60%
    theta_norm = constrain(theta_norm, 0.0f, 1.0f); // Ограничиваем диапазон

    N_temp *= (1.0f + k_h_N[soilIndex] * theta_norm);
    P_temp *= (1.0f + k_h_P[soilIndex] * theta_norm);
    K_temp *= (1.0f + k_h_K[soilIndex] * theta_norm);

    // Шаг 3: Корректировки по условиям выращивания
    if (growingCondition >= 0 && growingCondition < 3) {
        N_temp += npk_growing_adjustments[0][growingCondition];
        P_temp += npk_growing_adjustments[1][growingCondition];
        K_temp += npk_growing_adjustments[2][growingCondition];
    }

    // Валидация результатов
    N = constrain(N_temp, 0.0f, 2000.0f);
    P = constrain(P_temp, 0.0f, 2000.0f);
    K = constrain(K_temp, 0.0f, 2000.0f);
} 