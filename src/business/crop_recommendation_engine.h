/**
 * @file crop_recommendation_engine.h
 * @brief Реализация движка рекомендаций по культурам
 * @details Вычисляет агрономические рекомендации на основе типа культуры и условий
 */

#ifndef CROP_RECOMMENDATION_ENGINE_H
#define CROP_RECOMMENDATION_ENGINE_H

#include <map>
#include <Arduino.h>
#include "../../include/business/ICropRecommendationEngine.h"
#include "../../include/jxct_constants.h"

/**
 * @brief Конфигурация культуры
 *
 * Содержит рекомендуемые значения для конкретной культуры
 */
struct CropConfig {
    float temp;  // Температура (°C)
    float hum;   // Влажность (%)
    float ec;    // Электропроводность (μS/cm)
    float ph;    // pH
    float n;     // Азот (mg/kg)
    float p;     // Фосфор (mg/kg)
    float k;     // Калий (mg/kg)

    CropConfig() : temp(0), hum(0), ec(0), ph(0), n(0), p(0), k(0) {}
    CropConfig(float t, float h, float e, float ph_val, float nitrogen, float phosphorus, float potassium)
        : temp(t), hum(h), ec(e), ph(ph_val), n(nitrogen), p(phosphorus), k(potassium) {}
};

/**
 * @brief Сезонная корректировка
 *
 * Содержит коэффициенты корректировки для разных сезонов
 */
struct SeasonalAdjustment {
    float n_factor;  // Коэффициент для азота
    float p_factor;  // Коэффициент для фосфора
    float k_factor;  // Коэффициент для калия
    float temp_adjust;  // Корректировка температуры
    float hum_adjust;   // Корректировка влажности

    SeasonalAdjustment() : n_factor(1.0F), p_factor(1.0F), k_factor(1.0F), temp_adjust(0.0F), hum_adjust(0.0F) {}
    SeasonalAdjustment(float n, float p, float k, float temp, float hum)
        : n_factor(n), p_factor(p), k_factor(k), temp_adjust(temp), hum_adjust(hum) {}
};

/**
 * @brief Движок рекомендаций по культурам
 *
 * Реализует логику вычисления агрономических рекомендаций
 * на основе типа культуры, профиля почвы и условий окружающей среды.
 */
class CropRecommendationEngine : public ICropRecommendationEngine {
private:
    // Конфигурации культур
    std::map<String, CropConfig> cropConfigs;

    // Сезонные корректировки
    std::map<Season, SeasonalAdjustment> seasonalAdjustments;

    // Инициализация конфигураций культур
    void initializeCropConfigs();

    // Инициализация сезонных корректировок
    void initializeSeasonalAdjustments();

    // Применение корректировки по профилю почвы
    void applySoilProfileCorrection(RecValues& rec, SoilProfile soilProfile);

    // Применение корректировки по типу окружающей среды
    void applyEnvironmentCorrection(RecValues& rec, EnvironmentType envType);

    // Получение текущего сезона
    Season getCurrentSeason() const;

public:
    /**
     * @brief Конструктор
     *
     * Инициализирует конфигурации культур и сезонные корректировки
     */
    CropRecommendationEngine();

    /**
     * @brief Деструктор
     */
    virtual ~CropRecommendationEngine() = default;

    /**
     * @brief Вычисляет рекомендации для указанной культуры
     *
     * @param cropId Идентификатор культуры
     * @param soilProfile Профиль почвы
     * @param envType Тип окружающей среды
     * @return RecValues Структура с рекомендуемыми значениями
     */
    RecValues computeRecommendations(const String& cropId,
                                   const SoilProfile& soilProfile,
                                   const EnvironmentType& envType) override;

    /**
     * @brief Применяет сезонные корректировки к рекомендациям
     *
     * @param rec Рекомендации для корректировки
     * @param season Текущий сезон
     * @param isGreenhouse Признак теплицы
     */
    void applySeasonalCorrection(RecValues& rec,
                               Season season,
                               bool isGreenhouse) override;

    /**
     * @brief Получает конфигурацию для культуры
     *
     * @param cropId Идентификатор культуры
     * @return CropConfig Конфигурация культуры или базовые значения
     */
    CropConfig getCropConfig(const String& cropId) const;

    /**
     * @brief Проверяет наличие конфигурации для культуры
     *
     * @param cropId Идентификатор культуры
     * @return true если конфигурация существует, false в противном случае
     */
    bool hasCropConfig(const String& cropId) const;
};

#endif // CROP_RECOMMENDATION_ENGINE_H
