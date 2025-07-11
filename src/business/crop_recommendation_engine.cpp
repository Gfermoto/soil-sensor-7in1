/**
 * @file crop_recommendation_engine.cpp
 * @brief Реализация движка рекомендаций по культурам
 * @details Вычисляет агрономические рекомендации на основе типа культуры и условий
 */

#include "crop_recommendation_engine.h"
#include "../../include/jxct_config_vars.h"
#include "../../include/logger.h"
#include <ctime>

namespace {
// Прототипы внутренних функций компенсации
float compensatePHInternal(float pHRawValue, float temperatureValue, float moistureValue);
float compensateECInternal(float ECRawValue, float temperatureValue);
float compensateNPKInternal(float NPKRawValue, float temperatureValue, float moistureValue);
} // namespace


CropRecommendationEngine::CropRecommendationEngine() {
    initializeCropConfigs();
}

void CropRecommendationEngine::initializeCropConfigs() {
    // Базовые значения (generic) - научно обоснованные
    cropConfigs["generic"] = CropConfig(
        22.0F, 70.0F, 1500.0F, 6.5F,  // температура, влажность, EC, pH
        150.0F, 60.0F, 200.0F         // N, P, K (мг/кг) - ВАЛИДИРОВАНО: научные агрономические нормы
    );

    // ТОМАТЫ (Solanum lycopersicum) - [Источник: University of Florida IFAS Extension, B. Santos, 2019]
    cropConfigs["tomato"] = CropConfig(
        24.0F, 75.0F, 2000.0F, 6.5F,  // pH 6.0-6.8, EC 1.5-3.0 mS/cm
        200.0F, 80.0F, 300.0F         // N: 150-250, P: 50-100, K: 200-400 мг/кг
    );

    // ОГУРЦЫ (Cucumis sativus) - [Источник: USDA Natural Resources Conservation Service, 2020]
    cropConfigs["cucumber"] = CropConfig(
        26.0F, 80.0F, 1800.0F, 6.5F,  // pH 6.0-7.0, EC 1.2-2.5 mS/cm
        160.0F, 60.0F, 225.0F         // N: 120-200, P: 40-80, K: 150-300 мг/кг
    );

    // ПЕРЕЦ (Capsicum annuum) - [Источник: Cornell University Cooperative Extension, 2022]
    cropConfigs["pepper"] = CropConfig(
        27.0F, 75.0F, 2100.0F, 6.5F,  // pH 6.0-7.0, EC 1.4-2.8 mS/cm
        140.0F, 50.0F, 250.0F         // N: 100-180, P: 30-70, K: 150-350 мг/кг
    );

    // САЛАТ (Lactuca sativa) - [Источник: University of California Agriculture and Natural Resources, 2018]
    cropConfigs["lettuce"] = CropConfig(
        18.0F, 85.0F, 1500.0F, 6.5F,  // pH 6.0-7.0, EC 1.0-2.0 mS/cm
        115.0F, 35.0F, 175.0F         // N: 80-150, P: 20-50, K: 100-250 мг/кг
    );

    // ЧЕРНИКА (Vaccinium corymbosum) - [Источник: Michigan State University Extension, A. Schilder, 2021]
    cropConfigs["blueberry"] = CropConfig(
        20.0F, 75.0F, 1200.0F, 5.0F,  // pH 4.5-5.5, EC 0.8-1.5 mS/cm
        75.0F, 30.0F, 60.0F           // N: 50-100, P: 20-40, K: 40-80 мг/кг
    );

    // ГАЗОН (Lawn) - научные данные Turfgrass Science
    cropConfigs["lawn"] = CropConfig(
        20.0F, 60.0F, 1000.0F, 6.5F,  // pH 6.0-7.0, EC 0.8-1.5 mS/cm
        100.0F, 40.0F, 80.0F          // N: 80-120, P: 30-50, K: 60-100 мг/кг
    );

    // ВИНОГРАД (Vitis vinifera) - научные данные American Journal of Enology
    cropConfigs["grape"] = CropConfig(
        24.0F, 65.0F, 1500.0F, 6.5F,  // pH 6.0-7.5, EC 1.0-2.0 mS/cm
        120.0F, 50.0F, 150.0F         // N: 100-140, P: 40-60, K: 120-180 мг/кг
    );

    // ХВОЙНЫЕ (Conifer) - научные данные Forest Science
    cropConfigs["conifer"] = CropConfig(
        18.0F, 65.0F, 1000.0F, 5.8F,  // pH 5.5-6.5, EC 0.5-1.2 mS/cm
        60.0F, 25.0F, 50.0F           // N: 50-70, P: 20-30, K: 40-60 мг/кг
    );

    // КЛУБНИКА (Fragaria × ananassa) - научные данные HortScience
    cropConfigs["strawberry"] = CropConfig(
        22.0F, 80.0F, 1600.0F, 6.0F,  // pH 5.5-6.5, EC 1.2-2.0 mS/cm
        130.0F, 55.0F, 150.0F         // N: 110-150, P: 45-65, K: 130-170 мг/кг
    );

    // ЯБЛОНИ (Malus domestica) - научные данные Journal of Horticultural Science
    cropConfigs["apple"] = CropConfig(
        20.0F, 70.0F, 1200.0F, 6.5F,  // pH 6.0-7.0, EC 1.0-1.8 mS/cm
        110.0F, 45.0F, 130.0F         // N: 90-130, P: 35-55, K: 110-150 мг/кг
    );
    cropConfigs["pear"] = cropConfigs["apple"]; // Используем ту же конфигурацию

    // ВИШНЯ (Prunus avium) - научные данные HortScience
    cropConfigs["cherry"] = CropConfig(
        22.0F, 70.0F, 1300.0F, 6.5F,  // pH 6.0-7.0, EC 1.0-1.8 mS/cm
        120.0F, 50.0F, 140.0F         // N: 100-140, P: 40-60, K: 120-160 мг/кг
    );

    // МАЛИНА (Rubus idaeus) - научные данные Acta Horticulturae
    cropConfigs["raspberry"] = CropConfig(
        20.0F, 75.0F, 1100.0F, 6.0F,  // pH 5.5-6.5, EC 0.8-1.5 mS/cm
        100.0F, 40.0F, 120.0F         // N: 80-120, P: 30-50, K: 100-140 мг/кг
    );

    // СМОРОДИНА (Ribes spp.) - научные данные HortScience
    cropConfigs["currant"] = CropConfig(
        18.0F, 75.0F, 1000.0F, 6.0F,  // pH 5.5-6.5, EC 0.8-1.3 mS/cm
        80.0F, 35.0F, 100.0F          // N: 70-90, P: 30-40, K: 90-110 мг/кг
    );
}



// Структура для параметров рекомендаций
struct RecommendationParams
{
    const SensorData& data;
    const String& cropType;
    const String& growingType;
    const String& season;
    const String& soilType;
private:
    RecommendationParams(const SensorData& data, const String& cropType, const String& growingType, 
                        const String& season, const String& soilType) // NOLINT(bugprone-easily-swappable-parameters)
        : data(data), cropType(cropType), growingType(growingType), season(season), soilType(soilType) {}
public:
    static RecommendationParams fromValues(const SensorData& data, const String& cropType, const String& growingType, 
                                          const String& season, const String& soilType) { // NOLINT(bugprone-easily-swappable-parameters)
        return RecommendationParams(data, cropType, growingType, season, soilType);
    }
    // Builder для предотвращения ошибок с параметрами
    struct Builder {
        SensorData sensorData;
        String crop;
        String growing;
        String seasonType;
        String soil;
        Builder& data(const SensorData& sensorDataValue) { sensorData = sensorDataValue; return *this; }
        Builder& cropType(const String& cropTypeValue) { crop = cropTypeValue; return *this; }
        Builder& growingType(const String& growingTypeValue) { growing = growingTypeValue; return *this; }
        Builder& season(const String& seasonValue) { seasonType = seasonValue; return *this; }
        Builder& soilType(const String& soilTypeValue) { soil = soilTypeValue; return *this; }
        RecommendationParams build() const {
            return RecommendationParams::fromValues(sensorData, crop, growing, seasonType, soil);
        }
    };
    static Builder builder() { return {}; }
};

RecommendationResult CropRecommendationEngine::generateRecommendation(
    const SensorData& data, 
    const String& cropType, 
    const String& growingType,
    const String& season,
    const String& soilType) { // NOLINT(bugprone-easily-swappable-parameters)
    
    const RecommendationParams params = RecommendationParams::builder()
        .data(data)
        .cropType(cropType)
        .growingType(growingType)
        .season(season)
        .soilType(soilType)
        .build();
    
    // Валидация входных данных
    if (params.data.temperature < 0.0F || params.data.temperature > 50.0F) {
        // Логирование ошибки валидации
        Serial.println("ОШИБКА: Температура вне диапазона 0-50°C");
    }
    if (params.data.humidity < 10.0F || params.data.humidity > 90.0F) {
        Serial.println("ОШИБКА: Влажность вне диапазона 10-90%");
    }
    
    // Компенсация показаний датчиков [Источники: SSSA Journal, 2008; Advances in Agronomy, 2014; Journal of Soil Science, 2020]
    SensorData compensatedData = params.data;
    compensatedData.ph = compensatePH(CropCompensationParams::builder()
        .setRawValue(params.data.ph)
        .setTemperature(params.data.temperature)
        .setMoisture(params.data.humidity)
        .build());
    compensatedData.ec = compensateEC(CropECCompensationParams::builder()
        .setRawValue(params.data.ec)
        .setTemperature(params.data.temperature)
        .build());
    compensatedData.nitrogen = compensateNPK(CropCompensationParams::builder()
        .setRawValue(params.data.nitrogen)
        .setTemperature(params.data.temperature)
        .setMoisture(params.data.humidity)
        .build());
    compensatedData.phosphorus = compensateNPK(CropCompensationParams::builder()
        .setRawValue(params.data.phosphorus)
        .setTemperature(params.data.temperature)
        .setMoisture(params.data.humidity)
        .build());
    compensatedData.potassium = compensateNPK(CropCompensationParams::builder()
        .setRawValue(params.data.potassium)
        .setTemperature(params.data.temperature)
        .setMoisture(params.data.humidity)
        .build());
    
    RecommendationResult result;
    result.cropType = params.cropType;
    result.growingType = params.growingType;
    result.season = params.season;
    result.soilType = params.soilType;  // Добавляем тип почвы в результат
    
    // Получаем базовую конфигурацию для культуры
    auto configIterator = cropConfigs.find(params.cropType);
    if (configIterator == cropConfigs.end()) {
        configIterator = cropConfigs.find("generic");
    }
    CropConfig baseConfig = configIterator->second;
    
    // Применяем сезонные корректировки
    CropConfig adjustedConfig = applySeasonalAdjustments(baseConfig, params.season);
    
    // Применяем корректировки для типа выращивания
    adjustedConfig = applyGrowingTypeAdjustments(adjustedConfig, params.growingType);
    
    // Применяем корректировки для типа почвы
    adjustedConfig = applySoilTypeAdjustments(adjustedConfig, params.soilType);
    
    // Генерируем рекомендации на основе компенсированных данных
    result.recommendations = generateScientificRecommendations(compensatedData, adjustedConfig, params.cropType, params.soilType);
    
    // Рассчитываем общий статус здоровья почвы
    result.healthStatus = calculateSoilHealthStatus(compensatedData, adjustedConfig);
    
    // Добавляем научные комментарии
    result.scientificNotes = generateScientificNotes(compensatedData, adjustedConfig, params.cropType, params.soilType);
    
    return result;
}

} // namespace

namespace {
CropConfig applySeasonalAdjustments(const CropConfig& base, const String& season) {
    CropConfig adjusted = base;
    
    if (season == "spring") {
        // Весна: активный рост, потребность в азоте [Источник: FAO Fertilizer and Plant Nutrition Bulletin No. 19, FAO, 2008]
        adjusted.temperature += 0.0F;
        adjusted.humidity += 0.0F;
        adjusted.ec += 0.0F;
        adjusted.nitrogen *= 1.15F;    // +15% для активного роста
        adjusted.phosphorus *= 1.10F;  // +10% для развития корней
        adjusted.potassium *= 1.12F;   // +12% для устойчивости
    } else if (season == "summer") {
        // Лето: жаркий период, потребность в калии [Источник: FAO Fertilizer and Plant Nutrition Bulletin No. 19, FAO, 2008]
        adjusted.temperature += 2.0F;
        adjusted.humidity -= 5.0F;
        adjusted.ec += 200.0F;
        adjusted.nitrogen *= 1.08F;    // +8% для вегетации
        adjusted.phosphorus *= 1.05F;  // +5% стабильно
        adjusted.potassium *= 1.18F;   // +18% для жаростойкости
    } else if (season == "autumn") {
        // Осень: подготовка к зиме, потребность в фосфоре [Источник: FAO Fertilizer and Plant Nutrition Bulletin No. 19, FAO, 2008]
        adjusted.temperature -= 1.0F;
        adjusted.humidity += 5.0F;
        adjusted.ec -= 100.0F;
        adjusted.nitrogen *= 1.02F;    // +2% минимально
        adjusted.phosphorus *= 1.12F;  // +12% для подготовки к зиме
        adjusted.potassium *= 1.15F;   // +15% для морозостойкости
    } else if (season == "winter") {
        // Зима: период покоя, сниженные потребности [Источник: FAO Fertilizer and Plant Nutrition Bulletin No. 19, FAO, 2008]
        adjusted.temperature -= 3.0F;
        adjusted.humidity += 10.0F;
        adjusted.ec -= 200.0F;
        adjusted.nitrogen *= 0.85F;    // -15% период покоя
        adjusted.phosphorus *= 1.08F;  // +8% для корневой системы
        adjusted.potassium *= 1.10F;   // +10% для устойчивости
    }
    
    return adjusted;
}

CropConfig applyGrowingTypeAdjustments(const CropConfig& base, const String& growingType) {
    CropConfig adjusted = base;
    
    if (growingType == "greenhouse") {
        // Теплица: контролируемая среда, интенсивное выращивание [Источник: Protected Cultivation Guidelines, USDA, 2015]
        adjusted.temperature += 3.0F;
        adjusted.humidity += 10.0F;
        adjusted.ec += 300.0F;  // Более интенсивное питание
        adjusted.nitrogen *= 1.25F;  // +25% интенсивное выращивание
        adjusted.phosphorus *= 1.20F; // +20% развитие корней
        adjusted.potassium *= 1.22F;  // +22% качество плодов
    } else if (growingType == "hydroponics") {
        // Гидропоника: точный контроль питательных веществ [Источник: Hydroponic Crop Production, Acta Horticulturae, 2018]
        adjusted.ec += 500.0F;  // Высокая концентрация питательных веществ
        adjusted.nitrogen *= 1.40F;  // +40% точное питание
        adjusted.phosphorus *= 1.30F; // +30% доступность
        adjusted.potassium *= 1.35F;  // +35% качество
    } else if (growingType == "aeroponics") {
        // Аэропоника: максимальная эффективность [Источник: Aeroponic Systems, Journal of Agricultural Engineering, 2019]
        adjusted.ec += 400.0F;
        adjusted.nitrogen *= 1.35F;  // +35% эффективность
        adjusted.phosphorus *= 1.25F; // +25% развитие
        adjusted.potassium *= 1.30F;  // +30% качество
    } else if (growingType == "organic") {
        // Органическое выращивание: естественные процессы [Источник: Organic Farming Guidelines, IFOAM, 2020]
        adjusted.ec -= 200.0F;  // Более низкая концентрация солей
        adjusted.nitrogen *= 0.85F;  // -15% органический азот
        adjusted.phosphorus *= 0.90F; // -10% медленное высвобождение
        adjusted.potassium *= 0.88F;  // -12% органический калий
    }
    
    return adjusted;
}

CropConfig applySoilTypeAdjustments(const CropConfig& base, const String& soilType) {
    CropConfig adjusted = base;
    
    if (soilType == "sand") {
        // Песчаная почва: плохое удержание влаги и питательных веществ [Источник: Soil Fertility Manual, International Plant Nutrition Institute, 2020]
        adjusted.humidity -= 5.0F;
        adjusted.ec -= 200.0F;
        adjusted.nitrogen *= 1.25F;    // +25% вымывание
        adjusted.phosphorus *= 1.15F;  // +15% связывание
        adjusted.potassium *= 1.20F;   // +20% вымывание
    } else if (soilType == "loam") {
        // Суглинистая почва: оптимальные условия - без изменений
    } else if (soilType == "clay") {
        // Глинистая почва: хорошее удержание, но плохая аэрация [Источник: Soil Fertility Manual, International Plant Nutrition Institute, 2020]
        adjusted.humidity += 10.0F;
        adjusted.ec -= 400.0F;
        adjusted.nitrogen *= 0.90F;    // -10% удержание
        adjusted.phosphorus *= 0.85F;  // -15% связывание
        adjusted.potassium *= 0.92F;   // -8% удержание
    } else if (soilType == "peat") {
        // Торфяная почва: кислая, богатая органическим веществом [Источник: Soil Fertility Manual, International Plant Nutrition Institute, 2020]
        adjusted.humidity += 10.0F;
        adjusted.ec -= 100.0F;
        adjusted.ph -= 0.5F;
        adjusted.nitrogen *= 1.15F;    // +15% органический азот
        adjusted.phosphorus *= 1.10F;  // +10% доступность
        adjusted.potassium *= 1.05F;   // +5% стабильно
    } else if (soilType == "sandpeat") {
        // Песчано-торфяная смесь: компромисс [Источник: Soil Fertility Manual, International Plant Nutrition Institute, 2020]
        adjusted.humidity += 2.0F;
        adjusted.ec -= 50.0F;
        adjusted.ph -= 0.2F;
        adjusted.nitrogen *= 1.10F;    // +10% умеренное вымывание
        adjusted.phosphorus *= 1.05F;  // +5% умеренное связывание
        adjusted.potassium *= 1.02F;   // +2% минимальная корректировка
    }
    
    return adjusted;
}

String CropRecommendationEngine::generateScientificRecommendations(
    const SensorData& data, 
    const CropConfig& config, 
    const String& cropType,
    const String& soilType) {
    
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
    
    // Рекомендации по влажности с учетом типа почвы
    if (data.humidity < config.humidity - 10.0F) {
        recommendations += "💧 Влажность низкая. Рекомендуется: ";
        if (soilType == "sand") {
            recommendations += "частый полив малыми дозами (песок быстро дренирует)";
        } else if (soilType == "clay") {
            recommendations += "глубокий полив с интервалами (глина удерживает влагу)";
        } else if (cropType == "lettuce" || cropType == "cucumber") {
            recommendations += "увеличить полив, использовать туманообразование";
        } else {
            recommendations += "увеличить полив, мульчирование почвы";
        }
        recommendations += "\n";
    } else if (data.humidity > config.humidity + 10.0F) {
        recommendations += "💧 Влажность высокая. Рекомендуется: ";
        if (soilType == "clay") {
            recommendations += "улучшить дренаж, уменьшить полив (глина медленно дренирует)";
        } else {
            recommendations += "улучшить вентиляцию, уменьшить полив, профилактика грибковых заболеваний";
        }
        recommendations += "\n";
    }
    
    // Рекомендации по EC с учетом типа почвы
    if (data.ec < config.ec - 500.0F) {
        recommendations += "⚡ EC низкий (недостаток питательных веществ). Рекомендуется: ";
        if (soilType == "sand") {
            recommendations += "частое внесение удобрений малыми дозами (песок быстро вымывает)";
        } else {
            recommendations += "внести комплексное удобрение, увеличить концентрацию питательного раствора";
        }
        recommendations += "\n";
    } else if (data.ec > config.ec + 500.0F) {
        recommendations += "⚠️ EC высокий (риск засоления). Рекомендуется: ";
        if (soilType == "clay") {
            recommendations += "промывка почвы, гипсование (глина склонна к засолению)";
        } else {
            recommendations += "промывка почвы, снижение концентрации удобрений, использование гипса";
        }
        recommendations += "\n";
    }
    
    // Рекомендации по pH с учетом типа почвы
    if (data.ph < config.ph - 0.5F) {
        recommendations += "🧪 pH кислый. Рекомендуется: ";
        if (cropType == "blueberry") {
            recommendations += "pH подходит для черники, но контролируйте другие культуры";
        } else if (soilType == "peat") {
            recommendations += "внести известь, доломитовую муку (торф кислый)";
        } else {
            recommendations += "внести известь, доломитовую муку, древесную золу";
        }
        recommendations += "\n";
    } else if (data.ph > config.ph + 0.5F) {
        recommendations += "🧪 pH щелочной. Рекомендуется: ";
        if (soilType == "clay") {
            recommendations += "внести серу, кислые удобрения (глина склонна к щелочности)";
        } else {
            recommendations += "внести серу, торф, кислые удобрения";
        }
        recommendations += "\n";
    }
    
    // Рекомендации по NPK с учетом типа почвы
    if (data.nitrogen < config.nitrogen - 20.0F) {
        recommendations += "🌱 Азот (N) дефицитен. Рекомендуется: ";
        if (soilType == "sand") {
            recommendations += "частое внесение азотных удобрений (песок быстро вымывает азот)";
        } else if (soilType == "peat") {
            recommendations += "органические азотные удобрения (торф богат органикой)";
        } else {
            recommendations += "внести азотные удобрения (мочевина, аммиачная селитра), органические удобрения";
        }
        recommendations += "\n";
    }
    
    if (data.phosphorus < config.phosphorus - 15.0F) {
        recommendations += "🌱 Фосфор (P) дефицитен. Рекомендуется: ";
        if (soilType == "clay") {
            recommendations += "внести фосфорные удобрения с органическими (глина связывает фосфор)";
        } else {
            recommendations += "внести фосфорные удобрения (суперфосфат), костную муку";
        }
        recommendations += "\n";
    }
    
    if (data.potassium < config.potassium - 20.0F) {
        recommendations += "🌱 Калий (K) дефицитен. Рекомендуется: ";
        if (soilType == "sand") {
            recommendations += "частое внесение калийных удобрений (песок быстро вымывает калий)";
        } else {
            recommendations += "внести калийные удобрения (хлористый калий), древесную золу";
        }
        recommendations += "\n";
    }
    
    // Специфические рекомендации для культур с учетом типа почвы
    if (cropType == "tomato") {
        if (data.ec > 2500.0F) {
            recommendations += "🍅 Для томатов: высокий EC может вызвать вершинную гниль. Увеличьте кальций";
            if (soilType == "clay") {
                recommendations += ", улучшите дренаж";
            }
            recommendations += "\n";
        }
    } else if (cropType == "blueberry") {
        if (data.ph > 5.5F) {
            recommendations += "🫐 Для черники: pH слишком высокий. Внесите серу или кислый торф";
            if (soilType == "clay") {
                recommendations += ", добавьте торф для подкисления";
            }
            recommendations += "\n";
        }
    }
    
    if (recommendations.isEmpty()) {
        recommendations = "✅ Все параметры в оптимальном диапазоне. Продолжайте текущий уход.";
    }
    
    return recommendations;
}

String CropRecommendationEngine::generateScientificNotes(const SensorData& /*data*/, const CropConfig& /*config*/, const String& cropType, const String& soilType) {
    String notes = "📊 Научные данные:\n";
    
    // Общие научные принципы
    notes += "• Оптимальный диапазон pH для большинства культур: 6.0-7.0\n";
    notes += "• EC 1.0-2.5 mS/cm подходит для большинства овощных культур\n";
    notes += "• Соотношение N:P:K варьируется по фазам роста\n";
    
    // Данные по типу почвы
    notes += "\n🌍 Характеристики почвы (" + soilType + "):\n";
    if (soilType == "sand") {
        notes += "• Песок: быстрый дренаж, низкая влагоемкость\n";
        notes += "• Требует частого полива и внесения удобрений\n";
        notes += "• Коэффициент Арчи: m=1.3, n=2.0\n";
    } else if (soilType == "clay") {
        notes += "• Глина: медленный дренаж, высокая влагоемкость\n";
        notes += "• Склонна к засолению и уплотнению\n";
        notes += "• Коэффициент Арчи: m=2.0, n=2.5\n";
    } else if (soilType == "peat") {
        notes += "• Торф: высокая влагоемкость, кислая реакция\n";
        notes += "• Богат органикой, требует известкования\n";
        notes += "• Коэффициент Арчи: m=1.8, n=2.2\n";
    } else if (soilType == "loam") {
        notes += "• Суглинок: сбалансированные свойства\n";
        notes += "• Оптимален для большинства культур\n";
        notes += "• Коэффициент Арчи: m=1.5, n=2.0\n";
    } else if (soilType == "sandpeat") {
        notes += "• Песчано-торфяной: промежуточные свойства\n";
        notes += "• Подходит для газонов и декоративных культур\n";
        notes += "• Коэффициент Арчи: m=1.6, n=2.1\n";
    }
    
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

String CropRecommendationEngine::calculateSoilHealthStatus(const SensorData& data, const CropConfig& config) {
    int score = 100;
    
    // Оценка по температуре
    float tempDiff = abs(data.temperature - config.temperature);
    if (tempDiff > 10.0F) {
        score -= 30;
    } else if (tempDiff > 5.0F) {
        score -= 15;
    }
    
    // Оценка по влажности
    float humDiff = abs(data.humidity - config.humidity);
    if (humDiff > 20.0F) {
        score -= 25;
    } else if (humDiff > 10.0F) {
        score -= 10;
    }
    
    // Оценка по EC
    float ecDiff = abs(data.ec - config.ec);
    if (ecDiff > 1000.0F) {
        score -= 20;
    } else if (ecDiff > 500.0F) {
        score -= 10;
    }
    
    // Оценка по pH
    float phDiff = abs(data.ph - config.ph);
    if (phDiff > 1.0F) {
        score -= 15;
    } else if (phDiff > 0.5F) {
        score -= 5;
    }
    
    // Оценка по NPK
    if (data.nitrogen < config.nitrogen - 30.0F) {
        score -= 10;
    }
    if (data.phosphorus < config.phosphorus - 20.0F) {
        score -= 10;
    }
    if (data.potassium < config.potassium - 30.0F) {
        score -= 10;
    }
    
    if (score >= 80) {
        return "Отличное";
    }
    if (score >= 60) {
        return "Хорошее";
    }
    if (score >= 40) {
        return "Удовлетворительное";
    }
    return "Требует внимания";
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
    return {}; // Пустая конфигурация
}

bool CropRecommendationEngine::validateSensorData(const SensorData& data) const {
    // Проверка диапазонов значений
    if (data.temperature < -50.0F || data.temperature > 80.0F) {
        return false;
    }
    if (data.humidity < 0.0F || data.humidity > 100.0F) {
        return false;
    }
    if (data.ec < 0.0F || data.ec > 10000.0F) {
        return false;
    }
    if (data.ph < 0.0F || data.ph > 14.0F) {
        return false;
    }
    if (data.nitrogen < 0.0F || data.nitrogen > 1000.0F) {
        return false;
    }
    if (data.phosphorus < 0.0F || data.phosphorus > 1000.0F) {
        return false;
    }
    if (data.potassium < 0.0F || data.potassium > 1000.0F) {
        return false;
    }
    
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

// Реализация интерфейса ICropRecommendationEngine
RecValues CropRecommendationEngine::computeRecommendations(const String& cropId,
                                                         const SoilProfile& soilProfile,
                                                         const EnvironmentType& envType) {
    RecValues rec = {0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F};
    
    // Получаем базовую конфигурацию культуры
    auto it = cropConfigs.find(cropId);
    if (it == cropConfigs.end()) {
        it = cropConfigs.find("generic");
    }
    
    if (it != cropConfigs.end()) {
        const CropConfig& config = it->second;
        rec.t = config.temperature;
        rec.hum = config.humidity;
        rec.ec = config.ec;
        rec.ph = config.ph;
        rec.n = config.nitrogen;
        rec.p = config.phosphorus;
        rec.k = config.potassium;
    }
    
    return rec;
}

void CropRecommendationEngine::applySeasonalCorrection(RecValues& rec,
                                   Season season,
                                   bool isGreenhouse) {
    // Простая реализация сезонных корректировок
    switch (season) {
        case Season::SPRING:
            rec.n *= 1.1F;  // +10% азота весной
            break;
        case Season::SUMMER:
            rec.k *= 1.15F; // +15% калия летом
            break;
        case Season::AUTUMN:
            rec.p *= 1.1F;  // +10% фосфора осенью
            break;
        case Season::WINTER:
            rec.ec *= 0.9F; // -10% EC зимой
            break;
    }
    
    // Дополнительные корректировки для теплицы
    if (isGreenhouse) {
        rec.hum *= 1.1F;  // +10% влажности в теплице
        rec.t += 2.0F;    // +2°C в теплице
    }
}

// Обёртки для функций с легко перепутываемыми параметрами
float CropRecommendationEngine::compensatePH(const CropCompensationParams& params) {
    return compensatePHInternal(params.rawValue, params.temperature, params.moisture);
}

float CropRecommendationEngine::compensateEC(const CropECCompensationParams& params) {
    return compensateECInternal(params.rawValue, params.temperature);
}

float CropRecommendationEngine::compensateNPK(const CropCompensationParams& params) {
    return compensateNPKInternal(params.rawValue, params.temperature, params.moisture);
}

// Обратная совместимость
float CropRecommendationEngine::compensatePH(float pHRawValue, float temperatureValue, float moistureValue) {
    return compensatePHInternal(pHRawValue, temperatureValue, moistureValue);
}

float CropRecommendationEngine::compensateEC(float ECRawValue, float temperatureValue) {
    return compensateECInternal(ECRawValue, temperatureValue);
}

float CropRecommendationEngine::compensateNPK(float NPKRawValue, float temperatureValue, float moistureValue) {
    return compensateNPKInternal(NPKRawValue, temperatureValue, moistureValue);
}

} // namespace


