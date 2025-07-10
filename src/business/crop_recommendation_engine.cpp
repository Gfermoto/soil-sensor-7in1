/**
 * @file crop_recommendation_engine.cpp
 * @brief Реализация движка рекомендаций по культурам
 * @details Вычисляет агрономические рекомендации на основе типа культуры и условий
 */

#include "crop_recommendation_engine.h"
#include "../../include/jxct_config_vars.h"
#include "../../include/logger.h"
#include <ctime>

CropRecommendationEngine::CropRecommendationEngine() {
    initializeCropConfigs();
    initializeSeasonalAdjustments();
}

void CropRecommendationEngine::initializeCropConfigs() {
    // Базовые значения (generic)
    cropConfigs["generic"] = CropConfig(
        TEST_DATA_TEMP_BASE + 1, TEST_DATA_HUM_BASE, TEST_DATA_EC_BASE, TEST_DATA_PH_BASE,
        TEST_DATA_NPK_BASE + 5, TEST_DATA_NPK_BASE - 15, TEST_DATA_NPK_BASE
    );

    // Томаты
    cropConfigs["tomato"] = CropConfig(
        TEST_DATA_TEMP_BASE + 2, TEST_DATA_HUM_BASE, TEST_DATA_EC_BASE + 300, TEST_DATA_PH_BASE + 0.2F,
        TEST_DATA_NPK_BASE + 15, TEST_DATA_NPK_BASE - 15, TEST_DATA_NPK_BASE + 5
    );

    // Огурцы
    cropConfigs["cucumber"] = CropConfig(
        TEST_DATA_TEMP_BASE + 4, TEST_DATA_HUM_BASE + 10, TEST_DATA_EC_BASE + 600, TEST_DATA_PH_BASE - 0.1F,
        TEST_DATA_NPK_BASE + 10, TEST_DATA_NPK_BASE - 13, TEST_DATA_NPK_BASE + 3
    );

    // Перец
    cropConfigs["pepper"] = CropConfig(
        TEST_DATA_TEMP_BASE + 3, TEST_DATA_HUM_BASE + 5, TEST_DATA_EC_BASE + 400, TEST_DATA_PH_BASE,
        TEST_DATA_NPK_BASE + 13, TEST_DATA_NPK_BASE - 14, TEST_DATA_NPK_BASE + 4
    );

    // Салат
    cropConfigs["lettuce"] = CropConfig(
        TEST_DATA_TEMP_BASE, TEST_DATA_HUM_BASE + 15, TEST_DATA_EC_BASE - 200, TEST_DATA_PH_BASE - 0.3F,
        TEST_DATA_NPK_BASE + 5, TEST_DATA_NPK_BASE - 17, TEST_DATA_NPK_BASE
    );

    // Черника
    cropConfigs["blueberry"] = CropConfig(
        TEST_DATA_TEMP_BASE - 2, TEST_DATA_HUM_BASE + 5, TEST_DATA_EC_BASE, TEST_DATA_PH_BASE - 1.3F,
        TEST_DATA_NPK_BASE + 5, TEST_DATA_NPK_BASE - 15, TEST_DATA_NPK_BASE - 5
    );

    // Газон
    cropConfigs["lawn"] = CropConfig(
        TEST_DATA_TEMP_BASE, TEST_DATA_HUM_BASE - 10, TEST_DATA_EC_BASE - 400, TEST_DATA_PH_BASE,
        TEST_DATA_NPK_BASE, TEST_DATA_NPK_BASE - 17, TEST_DATA_NPK_BASE - 5
    );

    // Виноград
    cropConfigs["grape"] = CropConfig(
        TEST_DATA_TEMP_BASE + 2, TEST_DATA_HUM_BASE - 5, TEST_DATA_EC_BASE + 200, TEST_DATA_PH_BASE + 0.2F,
        TEST_DATA_NPK_BASE + 10, TEST_DATA_NPK_BASE - 13, TEST_DATA_NPK_BASE
    );

    // Хвойные
    cropConfigs["conifer"] = CropConfig(
        TEST_DATA_TEMP_BASE - 2, TEST_DATA_HUM_BASE - 5, TEST_DATA_EC_BASE - 200, TEST_DATA_PH_BASE - 0.8F,
        TEST_DATA_NPK_BASE, TEST_DATA_NPK_BASE - 17, TEST_DATA_NPK_BASE - 10
    );

    // Клубника
    cropConfigs["strawberry"] = CropConfig(
        TEST_DATA_TEMP_BASE, TEST_DATA_HUM_BASE + 10, TEST_DATA_EC_BASE + 300, TEST_DATA_PH_BASE - 0.3F,
        TEST_DATA_NPK_BASE + 10, TEST_DATA_NPK_BASE - 15, TEST_DATA_NPK_BASE
    );

    // Яблоки и груши
    cropConfigs["apple"] = CropConfig(
        TEST_DATA_TEMP_BASE - 2, TEST_DATA_HUM_BASE, TEST_DATA_EC_BASE, TEST_DATA_PH_BASE + 0.2F,
        TEST_DATA_NPK_BASE, TEST_DATA_NPK_BASE - 15, TEST_DATA_NPK_BASE - 5
    );
    cropConfigs["pear"] = cropConfigs["apple"]; // Используем ту же конфигурацию

    // Вишня
    cropConfigs["cherry"] = CropConfig(
        TEST_DATA_TEMP_BASE, TEST_DATA_HUM_BASE, TEST_DATA_EC_BASE + 100, TEST_DATA_PH_BASE + 0.2F,
        TEST_DATA_NPK_BASE + 5, TEST_DATA_NPK_BASE - 15, TEST_DATA_NPK_BASE
    );

    // Малина
    cropConfigs["raspberry"] = CropConfig(
        TEST_DATA_TEMP_BASE - 2, TEST_DATA_HUM_BASE + 5, TEST_DATA_EC_BASE - 100, TEST_DATA_PH_BASE - 0.1F,
        TEST_DATA_NPK_BASE + 5, TEST_DATA_NPK_BASE - 15, TEST_DATA_NPK_BASE - 3
    );

    // Смородина
    cropConfigs["currant"] = CropConfig(
        TEST_DATA_TEMP_BASE - 3, TEST_DATA_HUM_BASE + 5, TEST_DATA_EC_BASE - 200, TEST_DATA_PH_BASE - 0.1F,
        TEST_DATA_NPK_BASE, TEST_DATA_NPK_BASE - 16, TEST_DATA_NPK_BASE - 5
    );
}

void CropRecommendationEngine::initializeSeasonalAdjustments() {
    // Весенние корректировки
    seasonalAdjustments[Season::SPRING] = SeasonalAdjustment(
        TEST_DATA_NPK_INCREASE_N,  // +20% для азота
        TEST_DATA_NPK_INCREASE_P,  // +15% для фосфора
        TEST_DATA_NPK_INCREASE_K,  // +10% для калия
        0.0F,  // без корректировки температуры
        0.0F   // без корректировки влажности
    );

    // Летние корректировки
    seasonalAdjustments[Season::SUMMER] = SeasonalAdjustment(
        TEST_DATA_NPK_DECREASE_N,  // -10% для азота
        1.10F,                     // +10% для фосфора (теплица)
        TEST_DATA_NPK_DECREASE_K,  // +30% для калия (теплица)
        0.0F,  // без корректировки температуры
        0.0F   // без корректировки влажности
    );

    // Осенние корректировки
    seasonalAdjustments[Season::AUTUMN] = SeasonalAdjustment(
        TEST_DATA_NPK_INCREASE_N,  // +15% для азота (теплица)
        TEST_DATA_NPK_INCREASE_P,  // +15% для фосфора (теплица)
        TEST_DATA_NPK_INCREASE_K,  // +20% для калия (теплица)
        0.0F,  // без корректировки температуры
        0.0F   // без корректировки влажности
    );

    // Зимние корректировки
    seasonalAdjustments[Season::WINTER] = SeasonalAdjustment(
        TEST_DATA_NPK_DECREASE_N,  // +5% для азота (теплица)
        1.10F,                     // +10% для фосфора (теплица)
        TEST_DATA_NPK_DECREASE_K,  // +15% для калия (теплица)
        0.0F,  // без корректировки температуры
        0.0F   // без корректировки влажности
    );
}

RecValues CropRecommendationEngine::computeRecommendations(const String& cropId,
                                                         const SoilProfile& soilProfile,
                                                         const EnvironmentType& envType) {
    // Получаем базовую конфигурацию для культуры
    const CropConfig config = getCropConfig(cropId);

    // Создаем структуру рекомендаций
    RecValues rec;
    rec.t = config.temp;
    rec.hum = config.hum;
    rec.ec = config.ec;
    rec.ph = config.ph;
    rec.n = config.n;
    rec.p = config.p;
    rec.k = config.k;

    // Применяем корректировки по профилю почвы
    applySoilProfileCorrection(rec, soilProfile);

    // Применяем корректировки по типу окружающей среды
    applyEnvironmentCorrection(rec, envType);

    // Применяем сезонные корректировки
    const Season currentSeason = getCurrentSeason();
    const bool isGreenhouse = (envType == EnvironmentType::GREENHOUSE);
    applySeasonalCorrection(rec, currentSeason, isGreenhouse);

    logDebugSafe("Рекомендации для культуры %s: T=%.1f, H=%.1f, EC=%.0f, pH=%.1f, N=%.0f, P=%.0f, K=%.0f",
                 cropId.c_str(), rec.t, rec.hum, rec.ec, rec.ph, rec.n, rec.p, rec.k);

    return rec;
}

void CropRecommendationEngine::applySeasonalCorrection(RecValues& rec,
                                                     Season season,
                                                     bool isGreenhouse) {
    auto iterator = seasonalAdjustments.find(season);
    if (iterator == seasonalAdjustments.end()) {
        return; // Нет корректировки для данного сезона
    }

    const SeasonalAdjustment& adjustment = iterator->second;

    // Применяем корректировки NPK
    rec.n *= adjustment.n_factor;
    rec.p *= adjustment.p_factor;
    rec.k *= adjustment.k_factor;

    // Применяем корректировки температуры и влажности
    rec.t += adjustment.temp_adjust;
    rec.hum += adjustment.hum_adjust;

    // Дополнительные корректировки для теплицы
    if (isGreenhouse) {
        switch (season) {
            case Season::SUMMER:
                rec.p *= 1.10F;  // +10% для фосфора
                rec.k *= TEST_DATA_NPK_DECREASE_K;  // +30% для калия
                break;
            case Season::AUTUMN:
                rec.n *= TEST_DATA_NPK_INCREASE_N;  // +15% для азота
                rec.p *= TEST_DATA_NPK_INCREASE_P;  // +15% для фосфора
                rec.k *= TEST_DATA_NPK_INCREASE_K;  // +20% для калия
                break;
            case Season::WINTER:
                rec.n *= TEST_DATA_NPK_DECREASE_N;  // +5% для азота
                rec.p *= 1.10F;  // +10% для фосфора
                rec.k *= TEST_DATA_NPK_DECREASE_K;  // +15% для калия
                break;
            default:
                break;
        }
    } else {
        // Корректировки для открытого грунта
        switch (season) {
            case Season::SUMMER:
                rec.p *= 1.05F;  // +5% для фосфора
                rec.k *= TEST_DATA_NPK_DECREASE_K;  // +25% для калия
                break;
            case Season::AUTUMN:
                rec.n *= TEST_DATA_NPK_DECREASE_N;  // -20% для азота
                rec.p *= 1.10F;  // +10% для фосфора
                rec.k *= TEST_DATA_NPK_DECREASE_K;  // +15% для калия
                break;
            case Season::WINTER:
                rec.n *= TEST_DATA_NPK_DECREASE_N;  // -30% для азота
                rec.p *= 1.05F;  // +5% для фосфора
                rec.k *= TEST_DATA_NPK_DECREASE_K;  // +5% для калия
                break;
            default:
                break;
        }
    }
}

namespace {
void applySoilProfileCorrection(RecValues& rec, SoilProfile soilProfile) {
    switch (soilProfile) {
        case SoilProfile::SAND:
            rec.hum += -5;  // Песок - снижаем влажность
            break;
        case SoilProfile::PEAT:
            rec.hum += TEST_DATA_HUM_VARIATION;  // Торф - увеличиваем влажность
            rec.ph -= 0.3F;  // Снижаем pH
            break;
        case SoilProfile::CLAY:
        case SoilProfile::LOAM:
            rec.hum += 5;  // Глина или суглинок - увеличиваем влажность
            break;
        case SoilProfile::SANDPEAT:
            // Песчано-торфяной - промежуточные значения
            rec.hum += 0;
            rec.ph -= 0.1F;
            break;
        default:
            break;
    }
}

void applyEnvironmentCorrection(RecValues& rec, EnvironmentType envType) {
    switch (envType) {
        case EnvironmentType::GREENHOUSE:
            rec.hum += TEST_DATA_HUM_VARIATION;  // +10%
            rec.ec += TEST_DATA_EC_VARIATION;    // +500
            rec.n += 5;
            rec.k += 5;
            rec.t += 2;
            break;
        case EnvironmentType::INDOOR:
            rec.hum += -5;  // -5%
            rec.ec -= TEST_DATA_EC_VARIATION_SMALL;  // -200
            rec.t += 1;
            break;
        case EnvironmentType::OUTDOOR:
        default:
            // Без дополнительной корректировки
            break;
    }
}
} // end anonymous namespace

CropConfig CropRecommendationEngine::getCropConfig(const String& cropId) const { // NOLINT(readability-convert-member-functions-to-static)
    auto configIter = cropConfigs.find(cropId);
    if (configIter != cropConfigs.end()) {
        return configIter->second;
    }

    // Возвращаем базовую конфигурацию если культура не найдена
    logDebugSafe("Культура %s не найдена, используем базовую конфигурацию", cropId.c_str());
    return cropConfigs.at("generic");
}

Season CropRecommendationEngine::getCurrentSeason() {
    // Получаем текущее время
    time_t now = time(nullptr);
    struct tm* timeInfo = localtime(&now);

    if (timeInfo == nullptr) {
        return Season::SPRING; // По умолчанию весна
    }

    int month = timeInfo->tm_mon + 1; // tm_mon начинается с 0

    // Определяем сезон по месяцу
    if (month >= 3 && month <= 5) {
        return Season::SPRING;
    }
    if (month >= 6 && month <= 8) {
        return Season::SUMMER;
    }
    if (month >= 9 && month <= 11) {
        return Season::AUTUMN;
    }
    return Season::WINTER;
}

bool CropRecommendationEngine::hasCropConfig(const String& cropId) const {
    return cropConfigs.find(cropId) != cropConfigs.end();
}
