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
    // Базовые значения (generic) - научно обоснованные
    cropConfigs["generic"] = CropConfig(
        22.0F, 70.0F, 1500.0F, 6.5F,  // температура, влажность, EC, pH
        100.0F, 50.0F, 120.0F         // N, P, K (мг/кг)
    );

    // ТОМАТЫ (Solanum lycopersicum) - научные данные FAO, HortScience
    cropConfigs["tomato"] = CropConfig(
        24.0F, 75.0F, 2000.0F, 6.5F,  // pH 6.0-6.8, EC 1.5-3.0 mS/cm
        150.0F, 75.0F, 200.0F         // N: 120-180, P: 60-90, K: 150-250
    );

    // ОГУРЦЫ (Cucumis sativus) - научные данные Cucurbit Genetics, HortScience
    cropConfigs["cucumber"] = CropConfig(
        26.0F, 80.0F, 1800.0F, 6.5F,  // pH 6.0-7.0, EC 1.2-2.5 mS/cm
        125.0F, 65.0F, 150.0F         // N: 100-150, P: 50-80, K: 120-180
    );

    // ПЕРЕЦ (Capsicum annuum) - научные данные Scientia Horticulturae
    cropConfigs["pepper"] = CropConfig(
        27.0F, 75.0F, 2100.0F, 6.5F,  // pH 6.0-7.0, EC 1.4-2.8 mS/cm
        155.0F, 75.0F, 175.0F         // N: 130-180, P: 60-90, K: 150-200
    );

    // САЛАТ (Lactuca sativa) - научные данные Journal of Vegetable Science
    cropConfigs["lettuce"] = CropConfig(
        18.0F, 85.0F, 1500.0F, 6.5F,  // pH 6.0-7.0, EC 1.0-2.0 mS/cm
        100.0F, 50.0F, 100.0F         // N: 80-120, P: 40-60, K: 80-120
    );

    // ЧЕРНИКА (Vaccinium corymbosum) - научные данные HortScience
    cropConfigs["blueberry"] = CropConfig(
        20.0F, 75.0F, 1200.0F, 5.0F,  // pH 4.5-5.5, EC 0.8-1.5 mS/cm
        100.0F, 40.0F, 80.0F          // N: 80-120, P: 30-50, K: 60-100
    );

    // ГАЗОН (Lawn) - научные данные Turfgrass Science
    cropConfigs["lawn"] = CropConfig(
        20.0F, 60.0F, 1000.0F, 6.5F,  // pH 6.0-7.0, EC 0.8-1.5 mS/cm
        80.0F, 40.0F, 60.0F           // N: 60-100, P: 30-50, K: 50-80
    );

    // ВИНОГРАД (Vitis vinifera) - научные данные American Journal of Enology
    cropConfigs["grape"] = CropConfig(
        24.0F, 65.0F, 1500.0F, 6.5F,  // pH 6.0-7.5, EC 1.0-2.0 mS/cm
        125.0F, 65.0F, 150.0F         // N: 100-150, P: 50-80, K: 120-180
    );

    // ХВОЙНЫЕ (Conifer) - научные данные Forest Science
    cropConfigs["conifer"] = CropConfig(
        18.0F, 65.0F, 1000.0F, 5.8F,  // pH 5.5-6.5, EC 0.5-1.2 mS/cm
        60.0F, 30.0F, 40.0F           // N: 40-80, P: 20-40, K: 30-50
    );

    // КЛУБНИКА (Fragaria × ananassa) - научные данные HortScience
    cropConfigs["strawberry"] = CropConfig(
        22.0F, 80.0F, 1600.0F, 6.0F,  // pH 5.5-6.5, EC 1.2-2.0 mS/cm
        120.0F, 60.0F, 140.0F         // N: 100-140, P: 50-70, K: 120-160
    );

    // ЯБЛОНИ (Malus domestica) - научные данные Journal of Horticultural Science
    cropConfigs["apple"] = CropConfig(
        20.0F, 70.0F, 1200.0F, 6.5F,  // pH 6.0-7.0, EC 1.0-1.8 mS/cm
        100.0F, 50.0F, 120.0F         // N: 80-120, P: 40-60, K: 100-140
    );
    cropConfigs["pear"] = cropConfigs["apple"]; // Используем ту же конфигурацию

    // ВИШНЯ (Prunus avium) - научные данные HortScience
    cropConfigs["cherry"] = CropConfig(
        22.0F, 70.0F, 1300.0F, 6.5F,  // pH 6.0-7.0, EC 1.0-1.8 mS/cm
        110.0F, 55.0F, 130.0F         // N: 90-130, P: 45-65, K: 110-150
    );

    // МАЛИНА (Rubus idaeus) - научные данные Acta Horticulturae
    cropConfigs["raspberry"] = CropConfig(
        20.0F, 75.0F, 1100.0F, 6.0F,  // pH 5.5-6.5, EC 0.8-1.5 mS/cm
        90.0F, 45.0F, 100.0F          // N: 70-110, P: 35-55, K: 80-120
    );

    // СМОРОДИНА (Ribes spp.) - научные данные HortScience
    cropConfigs["currant"] = CropConfig(
        18.0F, 75.0F, 1000.0F, 6.0F,  // pH 5.5-6.5, EC 0.8-1.3 mS/cm
        80.0F, 40.0F, 90.0F           // N: 60-100, P: 30-50, K: 70-110
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

RecommendationResult CropRecommendationEngine::generateRecommendation(
    const SensorData& data, 
    const String& cropType, 
    const String& growingType,
    const String& season) {
    
    RecommendationResult result;
    result.cropType = cropType;
    result.growingType = growingType;
    result.season = season;
    
    // Получаем базовую конфигурацию для культуры
    auto it = cropConfigs.find(cropType);
    if (it == cropConfigs.end()) {
        it = cropConfigs.find("generic");
    }
    CropConfig baseConfig = it->second;
    
    // Применяем сезонные корректировки
    CropConfig adjustedConfig = applySeasonalAdjustments(baseConfig, season);
    
    // Применяем корректировки для типа выращивания
    adjustedConfig = applyGrowingTypeAdjustments(adjustedConfig, growingType);
    
    // Генерируем рекомендации на основе научных данных
    result.recommendations = generateScientificRecommendations(data, adjustedConfig, cropType);
    
    // Рассчитываем общий статус здоровья почвы
    result.healthStatus = calculateSoilHealthStatus(data, adjustedConfig);
    
    // Добавляем научные комментарии
    result.scientificNotes = generateScientificNotes(data, adjustedConfig, cropType);
    
    return result;
}

CropConfig CropRecommendationEngine::applySeasonalAdjustments(const CropConfig& base, const String& season) {
    CropConfig adjusted = base;
    
    if (season == "spring") {
        // Весна: умеренные температуры, повышенная влажность
        adjusted.temperature += 2.0F;
        adjusted.humidity += 5.0F;
        adjusted.nitrogen += 10.0F;  // Активизация роста
    } else if (season == "summer") {
        // Лето: высокие температуры, контроль влажности
        adjusted.temperature += 4.0F;
        adjusted.humidity -= 5.0F;
        adjusted.potassium += 15.0F;  // Устойчивость к стрессу
    } else if (season == "autumn") {
        // Осень: снижение температуры, подготовка к зиме
        adjusted.temperature -= 3.0F;
        adjusted.humidity += 3.0F;
        adjusted.phosphorus += 10.0F;  // Развитие корневой системы
    } else if (season == "winter") {
        // Зима: низкие температуры, защита от холода
        adjusted.temperature -= 8.0F;
        adjusted.humidity -= 10.0F;
        adjusted.ec -= 200.0F;  // Снижение солевого стресса
    }
    
    return adjusted;
}

CropConfig CropRecommendationEngine::applyGrowingTypeAdjustments(const CropConfig& base, const String& growingType) {
    CropConfig adjusted = base;
    
    if (growingType == "greenhouse") {
        // Теплица: контролируемая среда
        adjusted.temperature += 3.0F;
        adjusted.humidity += 10.0F;
        adjusted.ec += 300.0F;  // Более интенсивное питание
        adjusted.nitrogen += 20.0F;
        adjusted.phosphorus += 10.0F;
        adjusted.potassium += 15.0F;
    } else if (growingType == "hydroponics") {
        // Гидропоника: точный контроль питательных веществ
        adjusted.ec += 500.0F;  // Высокая концентрация питательных веществ
        adjusted.nitrogen += 30.0F;
        adjusted.phosphorus += 15.0F;
        adjusted.potassium += 25.0F;
    } else if (growingType == "aeroponics") {
        // Аэропоника: максимальная эффективность
        adjusted.ec += 400.0F;
        adjusted.nitrogen += 25.0F;
        adjusted.phosphorus += 12.0F;
        adjusted.potassium += 20.0F;
    } else if (growingType == "organic") {
        // Органическое выращивание: естественные процессы
        adjusted.ec -= 200.0F;  // Более низкая концентрация солей
        adjusted.nitrogen -= 10.0F;  // Медленное высвобождение
        adjusted.phosphorus -= 5.0F;
        adjusted.potassium -= 8.0F;
    }
    
    return adjusted;
}

String CropRecommendationEngine::generateScientificRecommendations(
    const SensorData& data, 
    const CropConfig& config, 
    const String& cropType) {
    
    String recommendations = "";
    
    // Температурные рекомендации
    if (data.temperature < config.temperature - 5.0F) {
        recommendations += "🌡️ Температура ниже оптимальной. Рекомендуется: ";
        if (cropType == "tomato" || cropType == "pepper") {
            recommendations += "увеличить обогрев, использовать мульчирование";
        } else {
            recommendations += "укрыть растения, добавить обогрев";
        }
        recommendations += "\n";
    } else if (data.temperature > config.temperature + 5.0F) {
        recommendations += "🌡️ Температура выше оптимальной. Рекомендуется: ";
        recommendations += "увеличить вентиляцию, притенение, полив";
        recommendations += "\n";
    }
    
    // Рекомендации по влажности
    if (data.humidity < config.humidity - 10.0F) {
        recommendations += "💧 Влажность низкая. Рекомендуется: ";
        if (cropType == "lettuce" || cropType == "cucumber") {
            recommendations += "увеличить полив, использовать туманообразование";
        } else {
            recommendations += "увеличить полив, мульчирование почвы";
        }
        recommendations += "\n";
    } else if (data.humidity > config.humidity + 10.0F) {
        recommendations += "💧 Влажность высокая. Рекомендуется: ";
        recommendations += "улучшить вентиляцию, уменьшить полив, профилактика грибковых заболеваний";
        recommendations += "\n";
    }
    
    // Рекомендации по EC (электропроводности)
    if (data.ec < config.ec - 500.0F) {
        recommendations += "⚡ EC низкий (недостаток питательных веществ). Рекомендуется: ";
        recommendations += "внести комплексное удобрение, увеличить концентрацию питательного раствора";
        recommendations += "\n";
    } else if (data.ec > config.ec + 500.0F) {
        recommendations += "⚠️ EC высокий (риск засоления). Рекомендуется: ";
        recommendations += "промывка почвы, снижение концентрации удобрений, использование гипса";
        recommendations += "\n";
    }
    
    // Рекомендации по pH
    if (data.ph < config.ph - 0.5F) {
        recommendations += "🧪 pH кислый. Рекомендуется: ";
        if (cropType == "blueberry") {
            recommendations += "pH подходит для черники, но контролируйте другие культуры";
        } else {
            recommendations += "внести известь, доломитовую муку, древесную золу";
        }
        recommendations += "\n";
    } else if (data.ph > config.ph + 0.5F) {
        recommendations += "🧪 pH щелочной. Рекомендуется: ";
        recommendations += "внести серу, торф, кислые удобрения";
        recommendations += "\n";
    }
    
    // Рекомендации по NPK
    if (data.nitrogen < config.nitrogen - 20.0F) {
        recommendations += "🌱 Азот (N) дефицитен. Рекомендуется: ";
        recommendations += "внести азотные удобрения (мочевина, аммиачная селитра), органические удобрения";
        recommendations += "\n";
    }
    
    if (data.phosphorus < config.phosphorus - 15.0F) {
        recommendations += "🌱 Фосфор (P) дефицитен. Рекомендуется: ";
        recommendations += "внести фосфорные удобрения (суперфосфат), костную муку";
        recommendations += "\n";
    }
    
    if (data.potassium < config.potassium - 20.0F) {
        recommendations += "🌱 Калий (K) дефицитен. Рекомендуется: ";
        recommendations += "внести калийные удобрения (хлористый калий), древесную золу";
        recommendations += "\n";
    }
    
    // Специфические рекомендации для культур
    if (cropType == "tomato") {
        if (data.ec > 2500.0F) {
            recommendations += "🍅 Для томатов: высокий EC может вызвать вершинную гниль. Увеличьте кальций";
            recommendations += "\n";
        }
    } else if (cropType == "blueberry") {
        if (data.ph > 5.5F) {
            recommendations += "🫐 Для черники: pH слишком высокий. Внесите серу или кислый торф";
            recommendations += "\n";
        }
    }
    
    if (recommendations.isEmpty()) {
        recommendations = "✅ Все параметры в оптимальном диапазоне. Продолжайте текущий уход.";
    }
    
    return recommendations;
}

String CropRecommendationEngine::calculateSoilHealthStatus(const SensorData& data, const CropConfig& config) {
    int score = 100;
    
    // Оценка по температуре
    float tempDiff = abs(data.temperature - config.temperature);
    if (tempDiff > 10.0F) score -= 30;
    else if (tempDiff > 5.0F) score -= 15;
    
    // Оценка по влажности
    float humDiff = abs(data.humidity - config.humidity);
    if (humDiff > 20.0F) score -= 25;
    else if (humDiff > 10.0F) score -= 10;
    
    // Оценка по EC
    float ecDiff = abs(data.ec - config.ec);
    if (ecDiff > 1000.0F) score -= 20;
    else if (ecDiff > 500.0F) score -= 10;
    
    // Оценка по pH
    float phDiff = abs(data.ph - config.ph);
    if (phDiff > 1.0F) score -= 15;
    else if (phDiff > 0.5F) score -= 5;
    
    // Оценка по NPK
    if (data.nitrogen < config.nitrogen - 30.0F) score -= 10;
    if (data.phosphorus < config.phosphorus - 20.0F) score -= 10;
    if (data.potassium < config.potassium - 30.0F) score -= 10;
    
    if (score >= 80) return "Отличное";
    else if (score >= 60) return "Хорошее";
    else if (score >= 40) return "Удовлетворительное";
    else return "Требует внимания";
}

String CropRecommendationEngine::generateScientificNotes(const SensorData& data, const CropConfig& config, const String& cropType) {
    String notes = "📊 Научные данные:\n";
    
    // Общие научные принципы
    notes += "• Оптимальный диапазон pH для большинства культур: 6.0-7.0\n";
    notes += "• EC 1.0-2.5 mS/cm подходит для большинства овощных культур\n";
    notes += "• Соотношение N:P:K варьируется по фазам роста\n";
    
    // Специфические данные для культур
    if (cropType == "tomato") {
        notes += "• Томаты: чувствительны к засолению (EC > 3.0 mS/cm)\n";
        notes += "• Кальций важен для предотвращения вершинной гнили\n";
    } else if (cropType == "blueberry") {
        notes += "• Черника: требует кислую почву (pH 4.5-5.5)\n";
        notes += "• Не переносит известь и высокий pH\n";
    } else if (cropType == "lettuce") {
        notes += "• Салат: быстрорастущая культура, требует частого полива\n";
        notes += "• Чувствителен к засухе и высоким температурам\n";
    }
    
    // Сезонные рекомендации
    notes += "\n🌱 Сезонные особенности:\n";
    notes += "• Весна: активизация роста, увеличение азота\n";
    notes += "• Лето: контроль температуры, увеличение калия\n";
    notes += "• Осень: подготовка к зиме, развитие корней\n";
    notes += "• Зима: защита от холода, снижение полива\n";
    
    return notes;
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

void CropRecommendationEngine::applySoilProfileCorrection(RecValues& rec, SoilProfile soilProfile) {
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

void CropRecommendationEngine::applyEnvironmentCorrection(RecValues& rec, EnvironmentType envType) {
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

std::vector<String> CropRecommendationEngine::getAvailableCrops() const {
    std::vector<String> crops;
    for (const auto& pair : cropConfigs) {
        crops.push_back(pair.first);
    }
    return crops;
}

CropConfig CropRecommendationEngine::getCropConfig(const String& cropType) const {
    auto it = cropConfigs.find(cropType);
    if (it != cropConfigs.end()) {
        return it->second;
    }
    // Возвращаем generic конфигурацию если культура не найдена
    auto genericIt = cropConfigs.find("generic");
    if (genericIt != cropConfigs.end()) {
        return genericIt->second;
    }
    return CropConfig(); // Пустая конфигурация
}

bool CropRecommendationEngine::validateSensorData(const SensorData& data) const {
    // Проверка диапазонов значений
    if (data.temperature < -50.0F || data.temperature > 80.0F) return false;
    if (data.humidity < 0.0F || data.humidity > 100.0F) return false;
    if (data.ec < 0.0F || data.ec > 10000.0F) return false;
    if (data.ph < 0.0F || data.ph > 14.0F) return false;
    if (data.nitrogen < 0.0F || data.nitrogen > 1000.0F) return false;
    if (data.phosphorus < 0.0F || data.phosphorus > 1000.0F) return false;
    if (data.potassium < 0.0F || data.potassium > 1000.0F) return false;
    
    return true;
}

String CropRecommendationEngine::getCropScientificInfo(const String& cropType) const {
    String info = "📚 Научная информация о культуре: " + cropType + "\n\n";
    
    if (cropType == "tomato") {
        info += "🍅 ТОМАТ (Solanum lycopersicum)\n";
        info += "• Семейство: Пасленовые (Solanaceae)\n";
        info += "• Происхождение: Южная Америка\n";
        info += "• Оптимальная температура: 20-30°C\n";
        info += "• pH: 6.0-6.8 (слабокислая почва)\n";
        info += "• EC: 1.5-3.0 mS/cm\n";
        info += "• Чувствителен к: засолению, вершинной гнили\n";
        info += "• Особенности: требует кальций для предотвращения гнили\n\n";
        
        info += "📖 Научные источники:\n";
        info += "• FAO Crop Calendar Database\n";
        info += "• HortScience Journal (томаты в теплицах)\n";
        info += "• Journal of Plant Nutrition (питание томатов)\n";
        
    } else if (cropType == "cucumber") {
        info += "🥒 ОГУРЕЦ (Cucumis sativus)\n";
        info += "• Семейство: Тыквенные (Cucurbitaceae)\n";
        info += "• Происхождение: Индия\n";
        info += "• Оптимальная температура: 22-30°C\n";
        info += "• pH: 6.0-7.0\n";
        info += "• EC: 1.2-2.5 mS/cm\n";
        info += "• Чувствителен к: засухе, переувлажнению\n";
        info += "• Особенности: требует частого полива\n\n";
        
        info += "📖 Научные источники:\n";
        info += "• Cucurbit Genetics Cooperative\n";
        info += "• HortScience (огурцы в гидропонике)\n";
        
    } else if (cropType == "blueberry") {
        info += "🫐 ЧЕРНИКА (Vaccinium corymbosum)\n";
        info += "• Семейство: Вересковые (Ericaceae)\n";
        info += "• Происхождение: Северная Америка\n";
        info += "• Оптимальная температура: 18-25°C\n";
        info += "• pH: 4.5-5.5 (кислая почва)\n";
        info += "• EC: 0.8-1.5 mS/cm\n";
        info += "• Чувствителен к: щелочной почве, извести\n";
        info += "• Особенности: требует кислую почву, микоризу\n\n";
        
        info += "📖 Научные источники:\n";
        info += "• HortScience (выращивание черники)\n";
        info += "• Journal of Small Fruit & Viticulture\n";
        
    } else if (cropType == "lettuce") {
        info += "🥬 САЛАТ (Lactuca sativa)\n";
        info += "• Семейство: Астровые (Asteraceae)\n";
        info += "• Происхождение: Средиземноморье\n";
        info += "• Оптимальная температура: 15-25°C\n";
        info += "• pH: 6.0-7.0\n";
        info += "• EC: 1.0-2.0 mS/cm\n";
        info += "• Чувствителен к: высоким температурам, засухе\n";
        info += "• Особенности: быстрорастущая культура\n\n";
        
        info += "📖 Научные источники:\n";
        info += "• Journal of Vegetable Science\n";
        info += "• Acta Horticulturae (салат в теплицах)\n";
        
    } else {
        info += "🌱 Общая информация:\n";
        info += "• Большинство культур предпочитают pH 6.0-7.0\n";
        info += "• EC 1.0-2.5 mS/cm оптимален для овощных культур\n";
        info += "• Соотношение N:P:K зависит от фазы роста\n";
        info += "• Температура влияет на скорость роста и качество\n\n";
        
        info += "📖 Общие научные источники:\n";
        info += "• USDA Plant Database\n";
        info += "• FAO Crop Production Guidelines\n";
        info += "• Soil Science Society of America\n";
    }
    
    return info;
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
