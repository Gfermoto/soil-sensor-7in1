/**
 * @file crop_recommendation_engine.h
 * @brief Реализация движка рекомендаций по культурам
 * @details Вычисляет агрономические рекомендации на основе типа культуры и условий
 */

#ifndef CROP_RECOMMENDATION_ENGINE_H
#define CROP_RECOMMENDATION_ENGINE_H

#include <Arduino.h>
#include <map>
#include <vector>
#include "business/ICropRecommendationEngine.h"

// Используем структуру SensorData из modbus_sensor.h
#include "../modbus_sensor.h"

// Структуры для устранения проблемы с легко перепутываемыми параметрами
struct CropCompensationParams {
    float rawValue;
    float temperature;
    float moisture;
    
private:
    CropCompensationParams(float raw, float temp, float moist) : rawValue(raw), temperature(temp), moisture(moist) {}
public:
    struct Builder {
        float rawValue = 0.0F;
        float temperature = 25.0F;
        float moisture = 60.0F;
        Builder& setRawValue(float value) { rawValue = value; return *this; }
        Builder& setTemperature(float temp) { temperature = temp; return *this; }
        Builder& setMoisture(float moist) { moisture = moist; return *this; }
        CropCompensationParams build() const {
            return CropCompensationParams(rawValue, temperature, moisture);
        }
    };
    static Builder builder() { return {}; }
};

struct CropECCompensationParams {
    float rawValue;
    float temperature;
    
private:
    CropECCompensationParams(float raw, float temp) : rawValue(raw), temperature(temp) {}
public:
    struct Builder {
        float rawValue = 0.0F;
        float temperature = 25.0F;
        Builder& setRawValue(float value) { rawValue = value; return *this; }
        Builder& setTemperature(float temp) { temperature = temp; return *this; }
        CropECCompensationParams build() const {
            return CropECCompensationParams(rawValue, temperature);
        }
    };
    static Builder builder() { return {}; }
};

// Структура конфигурации культуры
struct CropConfig {
    float temperature;   // °C
    float humidity;      // %
    float ec;           // μS/cm
    float ph;           // pH
    float nitrogen;     // мг/кг
    float phosphorus;   // мг/кг
    float potassium;    // мг/кг
    
    CropConfig() : temperature(0), humidity(0), ec(0), ph(0), 
                   nitrogen(0), phosphorus(0), potassium(0) {}
    
    CropConfig(float t, float h, float e, float p, float n, float ph, float k)
        : temperature(t), humidity(h), ec(e), ph(p), 
          nitrogen(n), phosphorus(ph), potassium(k) {}
};

// Структура результата рекомендаций
struct RecommendationResult {
    String cropType;
    String growingType;
    String season;
    String soilType;      // Добавляем тип почвы
    String recommendations;
    String healthStatus;
    String scientificNotes;
};

class CropRecommendationEngine : public ICropRecommendationEngine {
private:
    std::map<String, CropConfig> cropConfigs;
    
    // Коэффициенты компенсации датчиков [Источник: SSSA Journal, 2008; Advances in Agronomy, 2014]
    const float pH_alpha = -0.01F;  // Температурный коэффициент для pH
    const float pH_beta = 0.005F;   // Влажностный коэффициент для pH
    const float EC_gamma = 0.02F;   // Температурный коэффициент для EC
    const float NPK_delta = 0.03F;  // Температурный коэффициент для NPK
    const float NPK_epsilon = 0.01F; // Влажностный коэффициент для NPK
    
    // Функции компенсации датчиков
    float compensatePH(float pH_raw, float temperature, float moisture);
    float compensateEC(float EC_raw, float temperature);
    float compensateNPK(float NPK_raw, float temperature, float moisture);
    
    // Внутренние функции компенсации с Builder паттерном
    float compensatePH(const CropCompensationParams& params);
    float compensateEC(const CropECCompensationParams& params);
    float compensateNPK(const CropCompensationParams& params);
    
    void initializeCropConfigs();
    CropConfig applySeasonalAdjustments(const CropConfig& base, const String& season);
    CropConfig applyGrowingTypeAdjustments(const CropConfig& base, const String& growingType);
    CropConfig applySoilTypeAdjustments(const CropConfig& base, const String& soilType);  // Добавляем метод
    String generateScientificRecommendations(const SensorData& data, const CropConfig& config, const String& cropType, const String& soilType);
    String calculateSoilHealthStatus(const SensorData& data, const CropConfig& config);
    String generateScientificNotes(const SensorData& data, const CropConfig& config, const String& cropType, const String& soilType);

public:
    CropRecommendationEngine();
    
    // Основной метод генерации рекомендаций
    RecommendationResult generateRecommendation(
        const SensorData& data, 
        const String& cropType, 
        const String& growingType = "soil",
        const String& season = "spring",
        const String& soilType = "loam"  // Добавляем тип почвы
    );
    
    // Получение списка доступных культур
    std::vector<String> getAvailableCrops() const;
    
    // Получение конфигурации культуры
    CropConfig getCropConfig(const String& cropType) const;
    
    // Валидация данных с датчиков
    bool validateSensorData(const SensorData& data) const;
    
    // Получение научных данных о культуре
    String getCropScientificInfo(const String& cropType) const;
    
    // Реализация интерфейса ICropRecommendationEngine
    RecValues computeRecommendations(const String& cropId,
                                   const SoilProfile& soilProfile,
                                   const EnvironmentType& envType) override;
    
    void applySeasonalCorrection(RecValues& rec,
                               Season season,
                               bool isGreenhouse) override;
};

#endif // CROP_RECOMMENDATION_ENGINE_H
