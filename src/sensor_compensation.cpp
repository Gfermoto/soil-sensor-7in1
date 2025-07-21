#include "sensor_compensation.h"
#include <cmath>
#include <ctime>
#include "jxct_config_vars.h"

namespace
{
// --- коэффициенты ------------------------------------------------
struct SoilECCoeff
{
    float k;
};

constexpr float SOIL_EC_SAND = 0.15F;
constexpr float SOIL_EC_LOAM = 0.30F;
constexpr float SOIL_EC_PEAT = 0.10F;
constexpr float SOIL_EC_CLAY = 0.45F;
constexpr float SOIL_EC_SANDPEAT = 0.18F;
constexpr std::array<float, 5> SOIL_EC_VALUES = {SOIL_EC_SAND, SOIL_EC_LOAM, SOIL_EC_PEAT, SOIL_EC_CLAY, SOIL_EC_SANDPEAT};
constexpr std::array<float, 5> K_T_N = {0.0041F, 0.0038F, 0.0028F, 0.0032F, 0.0040F};
constexpr std::array<float, 5> K_T_P = {0.0053F, 0.0049F, 0.0035F, 0.0042F, 0.0051F};
constexpr std::array<float, 5> K_T_K = {0.0032F, 0.0029F, 0.0018F, 0.0024F, 0.0031F};
constexpr float K_H_N_A = 1.8F;
constexpr float K_H_N_B = 0.024F;
constexpr float K_H_P_A = 1.6F;
constexpr float K_H_P_B = 0.018F;
constexpr float K_H_K_A = 1.9F;
constexpr float K_H_K_B = 0.021F;
constexpr float REFERENCE_TEMP = 25.0F;
constexpr float COMPENSATION_BASE = 1.0F;
constexpr float COMPENSATION_DIV = 100.0F;

inline float k_h_N(float theta)
{
    return K_H_N_A - (K_H_N_B * theta);
}
inline float k_h_P(float theta)
{
    return K_H_P_A - (K_H_P_B * theta);
}
inline float k_h_K(float theta)
{
    return K_H_K_A - (K_H_K_B * theta);
}

// Внутренние функции — только для этой единицы трансляции
float correctEC_internal(float rawValue, float temperature,
                         float compensationFactor)  // NOLINT(bugprone-easily-swappable-parameters)
{
    const float tempDiff = temperature - REFERENCE_TEMP;
    const float compensation = COMPENSATION_BASE + (compensationFactor * tempDiff / COMPENSATION_DIV);
    return rawValue * compensation;
}

float correctPH_internal(float rawValue, float temperature,
                         float compensationFactor)  // NOLINT(bugprone-easily-swappable-parameters)
{
    const float tempDiff = temperature - REFERENCE_TEMP;
    const float compensation = COMPENSATION_BASE + (compensationFactor * tempDiff / COMPENSATION_DIV);
    return rawValue * compensation;
}

float correctNPK_internal(float rawValue, float temperature, float humidity,
                          float compensationFactor)  // NOLINT(bugprone-easily-swappable-parameters)
{
    const float referenceHumidity = 60.0F;
    const float tempDiff = temperature - REFERENCE_TEMP;
    const float humidityDiff = humidity - referenceHumidity;
    const float tempCompensation = COMPENSATION_BASE + (compensationFactor * tempDiff / COMPENSATION_DIV);
    const float humidityCompensation = COMPENSATION_BASE + (compensationFactor * humidityDiff / 1000.0F);
    return rawValue * tempCompensation * humidityCompensation;
}

// Строгая типизация для предотвращения ошибок
struct ECCompensationParams
{
    float rawValue = 0.0F;
    float temperature = 25.0F;
    float compensationFactor = 2.0F;
    struct Builder
    {
        float rawValue = 0.0F;
        float temperature = 25.0F;
        float compensationFactor = 2.0F;
        Builder& setRawValue(float value)
        {
            rawValue = value;
            return *this;
        }
        Builder& setTemperature(float temp)
        {
            temperature = temp;
            return *this;
        }
        Builder& setCompensationFactor(float factor)
        {
            compensationFactor = factor;
            return *this;
        }
        [[nodiscard]] ECCompensationParams build() const
        {
            ECCompensationParams result;
            result.rawValue = rawValue;
            result.temperature = temperature;
            result.compensationFactor = compensationFactor;
            return result;
        }
    };
    static Builder builder()
    {
        return {};
    }
};

float correctEC(const ECCompensationParams& params)  // NOLINT(misc-use-internal-linkage)
{
    return correctEC_internal(params.rawValue, params.temperature, params.compensationFactor);
}

float correctPH(const ECCompensationParams& params)  // NOLINT(misc-use-internal-linkage)
{
    return correctPH_internal(params.rawValue, params.temperature, params.compensationFactor);
}

float correctNPK(const ECCompensationParams& params)  // NOLINT(misc-use-internal-linkage)
{
    // Здесь rawValue — это NPK, temperature и humidity — из params
    return correctNPK_internal(params.rawValue, params.temperature, 60.0F, params.compensationFactor);
}

void correctNPK(const ECCompensationParams& params, NPKReferences& npk)  // NOLINT(misc-use-internal-linkage)
{
    if (params.temperature < 10.0F || params.temperature > 90.0F)
    {
        return;
    }
    const float tempFactorN = 1.0F - (0.02F * (params.temperature - 25.0F));
    const float tempFactorP = 1.0F - (0.015F * (params.temperature - 25.0F));
    const float tempFactorK = 1.0F - (0.02F * (params.temperature - 25.0F));
    npk.nitrogen *= tempFactorN;
    npk.phosphorus *= tempFactorP;
    npk.potassium *= tempFactorK;
}
}  // namespace

// --- ОБРАТНАЯ СОВМЕСТИМОСТЬ: старые сигнатуры (тонкие обёртки) ---
float correctEC(float ecRaw, float temperature,
                float compensationFactor)  // NOLINT(bugprone-easily-swappable-parameters,misc-use-internal-linkage)
{
    return correctEC(ECCompensationParams::builder()
                         .setRawValue(ecRaw)
                         .setTemperature(temperature)
                         .setCompensationFactor(compensationFactor)
                         .build());
}

float correctPH(float rawValue, float temperature,
                float compensationFactor)  // NOLINT(bugprone-easily-swappable-parameters,misc-use-internal-linkage)
{
    return correctPH(ECCompensationParams::builder()
                         .setRawValue(rawValue)
                         .setTemperature(temperature)
                         .setCompensationFactor(compensationFactor)
                         .build());
}

// Обёртка для обратной совместимости
float correctPH(float rawValue,
                float temperature)  // NOLINT(bugprone-easily-swappable-parameters,misc-use-internal-linkage)
{
    return correctPH(rawValue, temperature, 2.0F);
}

float correctNPK(
    float rawValue, float temperature, float humidity,
    float
        compensationFactor)  // NOLINT(bugprone-easily-swappable-parameters,misc-use-internal-linkage,misc-unused-parameters)
{
    return correctNPK(ECCompensationParams::builder()
                          .setRawValue(rawValue)
                          .setTemperature(temperature)
                          .setCompensationFactor(compensationFactor)
                          .build());
}

float correctEC(float ecRaw, const EnvironmentalConditions& env, SoilType soil)  // NOLINT(misc-use-internal-linkage)
{
    return correctEC(ECCompensationParams::builder()
                         .setRawValue(ecRaw)
                         .setTemperature(env.temperature)
                         .setCompensationFactor(2.0F)
                         .build());
}

void correctNPK(const EnvironmentalConditions& env, NPKReferences& npk,
                SoilType soil)  // NOLINT(misc-use-internal-linkage)
{
    correctNPK(ECCompensationParams::builder()
                   .setRawValue(0.0F)
                   .setTemperature(env.temperature)
                   .setCompensationFactor(2.0F)
                   .build(),
               npk);
}

float correctEC(float ecRaw, float temperature, float theta,
                SoilType soil)  // NOLINT(bugprone-easily-swappable-parameters,misc-use-internal-linkage)
{
    return correctEC(ecRaw, EnvironmentalConditions{temperature, theta}, soil);
}

void correctNPK(float temperature, float theta, SoilType soil,
                NPKReferences& npk)  // NOLINT(bugprone-easily-swappable-parameters,misc-use-internal-linkage)
{
    correctNPK(EnvironmentalConditions{temperature, theta}, npk, soil);
}
