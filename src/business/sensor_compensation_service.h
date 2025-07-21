/**
 * @file sensor_compensation_service.h
 * @brief Сервис компенсации датчиков
 * @details Применяет научные алгоритмы компенсации к показаниям датчиков
 */

#ifndef SENSOR_COMPENSATION_SERVICE_H
#define SENSOR_COMPENSATION_SERVICE_H

#include <Arduino.h>
#include <map>
#include "../../include/business/ISensorCompensationService.h"
#include "../../include/sensor_compensation.h"
#include "../../include/validation_utils.h"

// ===================== КОНСТАНТЫ ДЛЯ КОМПЕНСАЦИИ =====================
constexpr float UNIVERSAL_GAS_CONSTANT = 8.314F;
constexpr float FARADAY_CONSTANT = 96485.0F;
constexpr float STANDARD_TEMP_K = 298.15F;
constexpr float DEFAULT_POROSITY = 0.45F;
constexpr float DEFAULT_CEMENTATION = 1.5F;
constexpr float DEFAULT_SATURATION = 2.0F;
constexpr float DEFAULT_EPSILON_N = 0.01F;
constexpr float DEFAULT_EPSILON_P = 0.008F;
constexpr float DEFAULT_EPSILON_K = 0.012F;
constexpr float DEFAULT_DELTA_N = 0.0041F;
constexpr float DEFAULT_DELTA_P = 0.0053F;
constexpr float DEFAULT_DELTA_K = 0.0032F;
constexpr float NPK_SAND_EPSILON_N = 0.011F;
constexpr float NPK_SAND_EPSILON_P = 0.009F;
constexpr float NPK_SAND_EPSILON_K = 0.013F;
constexpr float NPK_LOAM_DELTA_N = 0.0039F;
constexpr float NPK_LOAM_DELTA_P = 0.0050F;
constexpr float NPK_LOAM_DELTA_K = 0.0030F;
constexpr float NPK_LOAM_EPSILON_N = 0.010F;
constexpr float NPK_LOAM_EPSILON_P = 0.008F;
constexpr float NPK_LOAM_EPSILON_K = 0.012F;
constexpr float NPK_CLAY_DELTA_N = 0.0033F;
constexpr float NPK_CLAY_DELTA_P = 0.0043F;
constexpr float NPK_CLAY_DELTA_K = 0.0025F;
constexpr float NPK_CLAY_EPSILON_N = 0.009F;
constexpr float NPK_CLAY_EPSILON_P = 0.007F;
constexpr float NPK_CLAY_EPSILON_K = 0.011F;
constexpr float NPK_PEAT_DELTA_N = 0.0029F;
constexpr float NPK_PEAT_DELTA_P = 0.0036F;
constexpr float NPK_PEAT_DELTA_K = 0.0019F;
constexpr float NPK_PEAT_EPSILON_N = 0.013F;
constexpr float NPK_PEAT_EPSILON_P = 0.010F;
constexpr float NPK_PEAT_EPSILON_K = 0.016F;
constexpr float NPK_SANDPEAT_DELTA_N = 0.0041F;
constexpr float NPK_SANDPEAT_DELTA_P = 0.0052F;
constexpr float NPK_SANDPEAT_DELTA_K = 0.0032F;
constexpr float NPK_SANDPEAT_EPSILON_N = 0.011F;
constexpr float NPK_SANDPEAT_EPSILON_P = 0.009F;
constexpr float NPK_SANDPEAT_EPSILON_K = 0.013F;
// ===================== КОНСТАНТЫ ДЛЯ КОМПЕНСАЦИИ =====================

/**
 * @brief Коэффициенты Арчи для разных типов почвы
 *
 * Содержит коэффициенты для модели Арчи (1942)
 */
struct ArchieCoefficients
{
    float m;  // Коэффициент цементации
    float n;  // Коэффициент насыщенности
    float a;  // Коэффициент пористости

    ArchieCoefficients() : m(DEFAULT_CEMENTATION), n(DEFAULT_SATURATION), a(DEFAULT_POROSITY) {}
    ArchieCoefficients(float cementation, float saturation, float porosity) : m(cementation), n(saturation), a(porosity)
    {
    }
};

/**
 * @brief Параметры почвы
 *
 * Содержит физические параметры почвы
 */
struct SoilParameters
{
    float porosity;       // Пористость
    float bulkDensity;    // Объемная плотность
    float fieldCapacity;  // Полевая влагоемкость

    SoilParameters() : porosity(DEFAULT_POROSITY), bulkDensity(1.40F), fieldCapacity(0.20F) {}
    SoilParameters(float por, float density, float capacity)
        : porosity(por), bulkDensity(density), fieldCapacity(capacity)
    {
    }
};

/**
 * @brief Коэффициенты NPK для разных типов почвы
 *
 * Содержит температурные и влажностные коэффициенты для NPK
 * Источник: [Delgado et al. (2020). DOI:10.1007/s42729-020-00215-4]
 */
struct NPKCoefficients
{
    float delta_N, delta_P, delta_K;        // Температурные коэффициенты
    float epsilon_N, epsilon_P, epsilon_K;  // Влажностные коэффициенты

    NPKCoefficients()
        : delta_N(DEFAULT_DELTA_N), delta_P(DEFAULT_DELTA_P), delta_K(DEFAULT_DELTA_K), epsilon_N(DEFAULT_EPSILON_N), epsilon_P(DEFAULT_EPSILON_P), epsilon_K(DEFAULT_EPSILON_K)
    {
    }
    NPKCoefficients(float dN, float dP, float dK, float eN, float eP, float eK)
        : delta_N(dN), delta_P(dP), delta_K(dK), epsilon_N(eN), epsilon_P(eP), epsilon_K(eK)
    {
    }
};

/**
 * @brief Сервис компенсации датчиков
 *
 * Реализует научные алгоритмы компенсации:
 * - Модель Арчи для EC
 * - Уравнение Нернста для pH
 * - Алгоритм FAO 56 для NPK
 */
class SensorCompensationService : public ISensorCompensationService
{
   private:
    // Коэффициенты Арчи для разных типов почвы
    std::map<SoilType, ArchieCoefficients> archieCoefficients;

    // Параметры почвы для разных типов
    std::map<SoilType, SoilParameters> soilParameters;

    // Коэффициенты NPK для разных типов почвы
    std::map<SoilType, NPKCoefficients> npkCoefficients;

    // Константы для расчетов
    static constexpr float R = UNIVERSAL_GAS_CONSTANT;    // Универсальная газовая постоянная (Дж/(моль·К))
    static constexpr float F = FARADAY_CONSTANT;  // Постоянная Фарадея (Кл/моль)
    static constexpr float T0 = STANDARD_TEMP_K;  // Стандартная температура (25°C в Кельвинах)

    // Инициализация коэффициентов Арчи
    void initializeArchieCoefficients();

    // Инициализация параметров почвы
    void initializeSoilParameters();

    // Инициализация коэффициентов NPK
    void initializeNPKCoefficients();

    // Расчет температуры в Кельвинах
    static float temperatureToKelvin(float celsius);

    // Расчет коэффициента температурной компенсации для EC
    static float calculateECTemperatureFactor(float temperature);

    // Расчет коэффициента влажностной компенсации для EC
    float calculateECHumidityFactor(float humidity, SoilType soilType) const;

   public:
    /**
     * @brief Конструктор
     *
     * Инициализирует коэффициенты и параметры
     */
    SensorCompensationService();

    /**
     * @brief Деструктор
     */
    virtual ~SensorCompensationService() = default;

    /**
     * @brief Применяет компенсацию к данным датчика
     *
     * @param data Данные датчика для компенсации
     * @param soilType Тип почвы для выбора коэффициентов
     */
    void applyCompensation(SensorData& data, SoilType soilType) override;

    /**
     * @brief Компенсирует EC по модели Арчи
     *
     * @param ec25 EC при 25°C
     * @param soilType Тип почвы
     * @param temperature Температура почвы
     * @param humidity Влажность почвы
     * @return float Скомпенсированное значение EC
     */
    float correctEC(float ec25, SoilType soilType, float temperature, float humidity) override;

    /**
     * @brief Компенсирует pH по уравнению Нернста
     *
     * @param phRaw Исходное значение pH
     * @param temperature Температура почвы
     * @return float Скомпенсированное значение pH
     */
    float correctPH(float temperature, float phRaw) override;

    /**
     * @brief Компенсирует NPK по алгоритму FAO 56
     *
     * @param temperature Температура почвы
     * @param humidity Влажность почвы
     * @param npk NPK значения для компенсации
     * @param soilType Тип почвы
     */
    void correctNPK(float temperature, float humidity, SoilType soilType, NPKReferences& npk) override;

    /**
     * @brief Получает коэффициент Арчи для типа почвы
     *
     * @param soilType Тип почвы
     * @return float Коэффициент Арчи
     */
    float getArchieCoefficient(SoilType soilType) const override;

    /**
     * @brief Получает пористость для типа почвы
     *
     * @param soilType Тип почвы
     * @return float Пористость почвы
     */
    float getPorosity(SoilType soilType) const override;

    /**
     * @brief Проверяет валидность входных данных для компенсации
     *
     * @param soilType Тип почвы
     * @param temperature Температура
     * @param humidity Влажность
     * @return true если данные валидны, false в противном случае
     */
    bool validateCompensationInputs(SoilType soilType, float humidity, float temperature) const override;

    /**
     * @brief Получает параметры почвы для типа
     *
     * @param soilType Тип почвы
     * @return SoilParameters Параметры почвы
     */
    SoilParameters getSoilParameters(SoilType soilType) const;

    /**
     * @brief Получает коэффициенты Арчи для типа почвы
     *
     * @param soilType Тип почвы
     * @return ArchieCoefficients Коэффициенты Арчи
     */
    ArchieCoefficients getArchieCoefficients(SoilType soilType) const;

    /**
     * @brief Получает коэффициенты NPK для типа почвы
     *
     * @param soilType Тип почвы
     * @return NPKCoefficients Коэффициенты NPK
     */
    NPKCoefficients getNPKCoefficients(SoilType soilType) const;
};

#endif  // SENSOR_COMPENSATION_SERVICE_H
