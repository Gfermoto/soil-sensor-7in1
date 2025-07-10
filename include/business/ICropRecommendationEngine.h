/**
 * @file ICropRecommendationEngine.h
 * @brief Интерфейс для движка рекомендаций по культурам
 * @details Определяет контракт для вычисления агрономических рекомендаций
 */

#ifndef ICROP_RECOMMENDATION_ENGINE_H
#define ICROP_RECOMMENDATION_ENGINE_H

#include <Arduino.h>
#include "../sensor_compensation.h"  // Для SoilProfile

// Структура для рекомендаций
struct RecValues {
    float t;      // Температура
    float hum;    // Влажность
    float ec;     // Электропроводность
    float ph;     // pH
    float n;      // Азот
    float p;      // Фосфор
    float k;      // Калий
};

// Перечисления для типов
enum class Season {
    SPRING,
    SUMMER,
    AUTUMN,
    WINTER
};

enum class EnvironmentType {
    OUTDOOR = 0,
    GREENHOUSE = 1,
    INDOOR = 2
};

// SoilProfile определен в sensor_compensation.h

/**
 * @brief Интерфейс для движка рекомендаций по культурам
 *
 * Определяет контракт для вычисления агрономических рекомендаций
 * на основе типа культуры, профиля почвы и условий окружающей среды.
 */
class ICropRecommendationEngine {
public:
    virtual ~ICropRecommendationEngine() = default;

    /**
     * @brief Вычисляет рекомендации для указанной культуры
     *
     * @param cropId Идентификатор культуры (tomato, cucumber, etc.)
     * @param soilProfile Профиль почвы (SAND, LOAM, CLAY, PEAT, SANDPEAT)
     * @param envType Тип окружающей среды (OUTDOOR, GREENHOUSE, INDOOR)
     * @return RecValues Структура с рекомендуемыми значениями
     */
    virtual RecValues computeRecommendations(const String& cropId,
                                           const SoilProfile& soilProfile,
                                           const EnvironmentType& envType) = 0;

    /**
     * @brief Применяет сезонные корректировки к рекомендациям
     *
     * @param rec Рекомендации для корректировки
     * @param season Текущий сезон
     * @param isGreenhouse Признак теплицы
     */
    virtual void applySeasonalCorrection(RecValues& rec,
                                       Season season,
                                       bool isGreenhouse) = 0;
};

#endif // ICROP_RECOMMENDATION_ENGINE_H
