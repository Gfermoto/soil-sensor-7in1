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

// Внутренние функции — только для этой единицы трансляции
float correctEC(float rawValue, float temperature, float compensationFactor)
{
    // Температурная компенсация EC
    const float referenceTemp = 25.0F;
    const float tempDiff = temperature - referenceTemp;
    const float compensation = 1.0F + (compensationFactor * tempDiff / 100.0F);
    return rawValue * compensation;
}

float correctPH(float rawValue, float temperature, float compensationFactor)
{
    // Температурная компенсация pH
    const float referenceTemp = 25.0F;
    const float tempDiff = temperature - referenceTemp;
    const float compensation = 1.0F + (compensationFactor * tempDiff / 100.0F);
    return rawValue * compensation;
}

float correctNPK(float rawValue, float temperature, float humidity, float compensationFactor)
{
    // Комплексная компенсация NPK с учётом температуры и влажности
    const float referenceTemp = 25.0F;
    const float referenceHumidity = 60.0F;
    const float tempDiff = temperature - referenceTemp;
    const float humidityDiff = humidity - referenceHumidity;
    const float tempCompensation = 1.0F + (compensationFactor * tempDiff / 100.0F);
    const float humidityCompensation = 1.0F + (compensationFactor * humidityDiff / 1000.0F);
    return rawValue * tempCompensation * humidityCompensation;
}
}  // namespace

// Строгая типизация для предотвращения ошибок
struct ECCompensationParams
{
    float rawValue;
    float temperature;
    float compensationFactor;
private:
    ECCompensationParams(float rawValue, float temperature, float compensationFactor)
        : rawValue(rawValue), temperature(temperature), compensationFactor(compensationFactor) {}
public:
    static ECCompensationParams fromValues(float rawValue, float temperature, float compensationFactor) {
        return ECCompensationParams(rawValue, temperature, compensationFactor);
    }
    // Builder для предотвращения ошибок с параметрами
    struct Builder {
        float rawValue = 0.0F;
        float temperature = 25.0F;
        float compensationFactor = 2.0F;
        Builder& setRawValue(float value) { rawValue = value; return *this; }
        Builder& setTemperature(float temp) { temperature = temp; return *this; }
        Builder& setCompensationFactor(float factor) { compensationFactor = factor; return *this; }
        ECCompensationParams build() const {
            return ECCompensationParams::fromValues(rawValue, temperature, compensationFactor);
        }
    };
    static Builder builder() { return {}; }
    static ECCompensationParams createWithRawValue(float rawValue, float temperature, float compensationFactor) {
        return ECCompensationParams::fromValues(rawValue, temperature, compensationFactor);
    }
    static ECCompensationParams createWithTemperature(float temperature, float rawValue, float compensationFactor) {
        return ECCompensationParams::fromValues(rawValue, temperature, compensationFactor);
    }
};

// ------------------------------------------------------------------
// EC ----------------------------------------------------------------
static float correctEC(float ecRaw, const ECCompensationParams& params)
{
    // ИСПРАВЛЕНО: простая температурная компенсация к 25 °C
    return ecRaw / (1.0F + 0.02F * (params.temperature - 25.0F));
}

// pH ----------------------------------------------------------------
float correctPH(float temperature, float phRaw)
{
    // ИСПРАВЛЕНО: стандартная температурная поправка (-0.003·ΔT)
    return phRaw - (0.003F * (temperature - 25.0F));
}

// NPK ---------------------------------------------------------------
static void correctNPK(const ECCompensationParams& params, NPKReferences& npk)
{
    if (params.temperature < 10.0F || params.temperature > 90.0F)
    {
        return;  // валидация – оставляем как есть
    }

    // ИСПРАВЛЕНО: простая температурная коррекция
    const float tempFactorN = 1.0F - (0.02F * (params.temperature - 25.0F));
    const float tempFactorP = 1.0F - (0.015F * (params.temperature - 25.0F));
    const float tempFactorK = 1.0F - (0.02F * (params.temperature - 25.0F));

    npk.nitrogen *= tempFactorN;
    npk.phosphorus *= tempFactorP;
    npk.potassium *= tempFactorK;
}

// ✅ ТИПОБЕЗОПАСНЫЕ ВЕРСИИ (предотвращают перепутывание параметров)
// ------------------------------------------------------------------
float correctEC(float ecRaw, const EnvironmentalConditions& env, SoilType soil)
{
    return correctEC(ecRaw, ECCompensationParams::builder()
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
