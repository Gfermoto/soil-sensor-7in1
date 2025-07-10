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

// Структура для данных с датчиков
struct SensorData {
    float temperature;    // °C
    float humidity;       // %
    float ec;            // μS/cm
    float ph;            // pH
    float nitrogen;      // мг/кг
    float phosphorus;    // мг/кг
    float potassium;     // мг/кг
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
    String recommendations;
    String healthStatus;
    String scientificNotes;
};

class CropRecommendationEngine {
private:
    std::map<String, CropConfig> cropConfigs;
    
    void initializeCropConfigs();
    CropConfig applySeasonalAdjustments(const CropConfig& base, const String& season);
    CropConfig applyGrowingTypeAdjustments(const CropConfig& base, const String& growingType);
    String generateScientificRecommendations(const SensorData& data, const CropConfig& config, const String& cropType);
    String calculateSoilHealthStatus(const SensorData& data, const CropConfig& config);
    String generateScientificNotes(const SensorData& data, const CropConfig& config, const String& cropType);

public:
    CropRecommendationEngine();
    
    // Основной метод генерации рекомендаций
    RecommendationResult generateRecommendation(
        const SensorData& data, 
        const String& cropType, 
        const String& growingType = "soil",
        const String& season = "spring"
    );
    
    // Получение списка доступных культур
    std::vector<String> getAvailableCrops() const;
    
    // Получение конфигурации культуры
    CropConfig getCropConfig(const String& cropType) const;
    
    // Валидация данных с датчиков
    bool validateSensorData(const SensorData& data) const;
    
    // Получение научных данных о культуре
    String getCropScientificInfo(const String& cropType) const;
};

#endif // CROP_RECOMMENDATION_ENGINE_H
