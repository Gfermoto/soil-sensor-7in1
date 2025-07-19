/**
 * @file ISensorCompensationService.h
 * @brief Интерфейс для сервиса компенсации датчиков
 * @details Определяет контракт для применения научных алгоритмов компенсации
 */

#ifndef ISENSOR_COMPENSATION_SERVICE_H
#define ISENSOR_COMPENSATION_SERVICE_H

#include <Arduino.h>
#include "../sensor_compensation.h"
#include "../validation_utils.h"

/**
 * @brief Интерфейс для сервиса компенсации датчиков
 *
 * Определяет контракт для применения научных алгоритмов компенсации
 * к показаниям датчиков (модель Арчи, уравнение Нернста, FAO 56).
 */
class ISensorCompensationService
{
   public:
    virtual ~ISensorCompensationService() = default;

    /**
     * @brief Применяет компенсацию к данным датчика
     *
     * @param data Данные датчика для компенсации
     * @param soilType Тип почвы для выбора коэффициентов
     */
    virtual void applyCompensation(SensorData& data, SoilType soilType) = 0;

    /**
     * @brief Компенсирует EC по модели Арчи
     *
     * Применяет температурную и влажностную компенсацию к электропроводности
     * на основе модели Арчи (1942).
     *
     * @param ec25 EC при 25°C
     * @param soilType Тип почвы
     * @param temperature Температура почвы
     * @param humidity Влажность почвы
     * @return float Скомпенсированное значение EC
     */
    virtual float correctEC(float ec25, SoilType soilType, float temperature, float humidity) = 0;

    /**
     * @brief Компенсирует pH по уравнению Нернста
     *
     * Применяет температурную компенсацию к pH на основе уравнения Нернста.
     *
     * @param temperature Температура почвы
     * @param phRaw Исходное значение pH
     * @return float Скомпенсированное значение pH
     */
    virtual float correctPH(float temperature, float phRaw) = 0;

    /**
     * @brief Компенсирует NPK по алгоритму FAO 56
     *
     * Применяет температурную и влажностную компенсацию к NPK значениям
     * на основе алгоритма FAO 56.
     *
     * @param temperature Температура почвы
     * @param humidity Влажность почвы
     * @param soilType Тип почвы
     * @param npk NPK значения для компенсации
     */
    virtual void correctNPK(float temperature, float humidity, SoilType soilType, NPKReferences& npk) = 0;

    /**
     * @brief Получает коэффициент Арчи для типа почвы
     *
     * @param soilType Тип почвы
     * @return float Коэффициент Арчи
     */
    virtual float getArchieCoefficient(SoilType soilType) const = 0;

    /**
     * @brief Получает пористость для типа почвы
     *
     * @param soilType Тип почвы
     * @return float Пористость почвы
     */
    virtual float getPorosity(SoilType soilType) const = 0;

    /**
     * @brief Проверяет валидность входных данных для компенсации
     *
     * @param soilType Тип почвы
     * @param temperature Температура
     * @param humidity Влажность
     * @return true если данные валидны, false в противном случае
     */
    virtual bool validateCompensationInputs(SoilType soilType, float humidity, float temperature) const = 0;

    /**
     * @brief Инициализирует коэффициенты Арчи
     */
    virtual void initializeArchieCoefficients() = 0;

    /**
     * @brief Инициализирует параметры почвы
     */
    virtual void initializeSoilParameters() = 0;

    /**
     * @brief Инициализирует коэффициенты NPK
     */
    virtual void initializeNPKCoefficients() = 0;
};

#endif  // ISENSOR_COMPENSATION_SERVICE_H
