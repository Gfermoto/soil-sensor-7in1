#include "sensor_compensation.h"
#include <cmath>
#include <ctime>
#include "jxct_config_vars.h"

namespace {
// --- коэффициенты ------------------------------------------------
struct SoilECCoeff
{
    float k;
};

constexpr std::array<SoilECCoeff, 5> SOIL_EC = {{
    {0.15F},  // SAND
    {0.30F},  // LOAM
    {0.10F},  // PEAT
    {0.45F},  // CLAY
    {0.18F}   // SANDPEAT
}};

constexpr std::array<float, 5> k_t_N = {0.0041F, 0.0038F, 0.0028F, 0.0032F, 0.0040F};
constexpr std::array<float, 5> k_t_P = {0.0053F, 0.0049F, 0.0035F, 0.0042F, 0.0051F};
constexpr std::array<float, 5> k_t_K = {0.0032F, 0.0029F, 0.0018F, 0.0024F, 0.0031F};

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

// Внутренние функции — только для этой единицы трансляции
float correctEC_internal(float rawValue, float temperature, float compensationFactor) // NOLINT(bugprone-easily-swappable-parameters)
{
    const float referenceTemp = 25.0F;
    const float tempDiff = temperature - referenceTemp;
    const float compensation = 1.0F + (compensationFactor * tempDiff / 100.0F);
    return rawValue * compensation;
}

float correctPH_internal(float rawValue, float temperature, float compensationFactor) // NOLINT(bugprone-easily-swappable-parameters)
{
    const float referenceTemp = 25.0F;
    const float tempDiff = temperature - referenceTemp;
    const float compensation = 1.0F + (compensationFactor * tempDiff / 100.0F);
    return rawValue * compensation;
}

float correctNPK_internal(float rawValue, float temperature, float humidity, float compensationFactor) // NOLINT(bugprone-easily-swappable-parameters)
{
    const float referenceTemp = 25.0F;
    const float referenceHumidity = 60.0F;
    const float tempDiff = temperature - referenceTemp;
    const float humidityDiff = humidity - referenceHumidity;
    const float tempCompensation = 1.0F + (compensationFactor * tempDiff / 100.0F);
    const float humidityCompensation = 1.0F + (compensationFactor * humidityDiff / 1000.0F);
    return rawValue * tempCompensation * humidityCompensation;
}

// Строгая типизация для предотвращения ошибок
struct ECCompensationParams
{
    float rawValue;
    float temperature;
    float compensationFactor;
    ECCompensationParams() : rawValue(0.0F), temperature(25.0F), compensationFactor(2.0F) {}
    struct Builder {
        float rawValue = 0.0F;
        float temperature = 25.0F;
        float compensationFactor = 2.0F;
        Builder& setRawValue(float value) { rawValue = value; return *this; }
        Builder& setTemperature(float temp) { temperature = temp; return *this; }
        Builder& setCompensationFactor(float factor) { compensationFactor = factor; return *this; }
        ECCompensationParams build() const {
            ECCompensationParams result;
            result.rawValue = rawValue;
            result.temperature = temperature;
            result.compensationFactor = compensationFactor;
            return result;
        }
    };
    static Builder builder() { return {}; }
};

float correctEC(const ECCompensationParams& params)
{
    return correctEC_internal(params.rawValue, params.temperature, params.compensationFactor);
}

float correctPH(const ECCompensationParams& params)
{
    return correctPH_internal(params.rawValue, params.temperature, params.compensationFactor);
}

float correctNPK(const ECCompensationParams& params)
{
    // Здесь rawValue — это NPK, temperature и humidity — из params
    return correctNPK_internal(params.rawValue, params.temperature, 60.0F, params.compensationFactor);
}

void correctNPK(const ECCompensationParams& params, NPKReferences& npk)
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
} // namespace

// --- ОБРАТНАЯ СОВМЕСТИМОСТЬ: старые сигнатуры (тонкие обёртки) ---
float correctEC(float ecRaw, float temperature, float compensationFactor)
{
    return correctEC(ECCompensationParams::builder()
        .setRawValue(ecRaw)
        .setTemperature(temperature)
        .setCompensationFactor(compensationFactor)
        .build());
}

float correctPH(float rawValue, float temperature, float compensationFactor)
{
    return correctPH(ECCompensationParams::builder()
        .setRawValue(rawValue)
        .setTemperature(temperature)
        .setCompensationFactor(compensationFactor)
        .build());
}

// Обёртка для обратной совместимости
float correctPH(float rawValue, float temperature)
{
    return correctPH(rawValue, temperature, 2.0F);
}

float correctNPK(float rawValue, float temperature, float humidity, float compensationFactor)
{
    return correctNPK(ECCompensationParams::builder()
        .setRawValue(rawValue)
        .setTemperature(temperature)
        .setCompensationFactor(compensationFactor)
        .build());
}

float correctEC(float ecRaw, const EnvironmentalConditions& env, SoilType soil)
{
    return correctEC(ECCompensationParams::builder()
        .setRawValue(ecRaw)
        .setTemperature(env.temperature)
        .setCompensationFactor(2.0F)
        .build());
}

void correctNPK(const EnvironmentalConditions& env, NPKReferences& npk, SoilType soil)
{
    correctNPK(ECCompensationParams::builder()
        .setRawValue(0.0F)
        .setTemperature(env.temperature)
        .setCompensationFactor(2.0F)
        .build(), npk);
}

float correctEC(float ecRaw, float temperature, float theta, SoilType soil)
{
    return correctEC(ecRaw, EnvironmentalConditions{temperature, theta}, soil);
}

void correctNPK(float temperature, float theta, SoilType soil, NPKReferences& npk)
{
    correctNPK(EnvironmentalConditions{temperature, theta}, npk, soil);
}
