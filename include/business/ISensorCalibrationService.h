/**
 * @file ISensorCalibrationService.h
 * @brief Интерфейс для сервиса калибровки датчиков
 * @details Определяет контракт для применения калибровочных данных
 */

#ifndef ISENSOR_CALIBRATION_SERVICE_H
#define ISENSOR_CALIBRATION_SERVICE_H

#include <Arduino.h>
#include "../sensor_compensation.h"
#include "../validation_utils.h"

/**
 * @brief Интерфейс для сервиса калибровки датчиков
 *
 * Определяет контракт для применения калибровочных данных к показаниям датчиков,
 * включая загрузку калибровочных таблиц и их применение.
 */
class ISensorCalibrationService
{
   public:
    virtual ~ISensorCalibrationService() = default;

    /**
     * @brief Применяет калибровку к данным датчика
     *
     * @param data Данные датчика для калибровки
     * @param profile Профиль почвы для выбора калибровочной таблицы
     */
    virtual void applyCalibration(SensorData& data, SoilProfile profile) = 0;

    /**
     * @brief Применяет калибровку к отдельному значению
     *
     * @param rawValue Исходное значение
     * @param profile Профиль почвы
     * @return float Откалиброванное значение
     */
    virtual float applySingleCalibration(float rawValue, SoilProfile profile) = 0;

    /**
     * @brief Проверяет валидность калибровочных данных
     *
     * @param data Данные датчика для проверки
     * @return true если данные валидны, false в противном случае
     */
    virtual bool validateCalibrationData(const SensorData& data) = 0;

    /**
     * @brief Загружает калибровочную таблицу из CSV
     *
     * @param csvData CSV данные калибровочной таблицы
     * @param profile Профиль почвы
     * @return true если загрузка успешна, false в противном случае
     */
    virtual bool loadCalibrationTable(const String& csvData, SoilProfile profile) = 0;

    /**
     * @brief Проверяет наличие калибровочной таблицы
     *
     * @param profile Профиль почвы
     * @return true если таблица загружена, false в противном случае
     */
    virtual bool hasCalibrationTable(SoilProfile profile) const = 0;

    /**
     * @brief Очищает калибровочную таблицу
     *
     * @param profile Профиль почвы
     */
    virtual void clearCalibrationTable(SoilProfile profile) = 0;
};

#endif  // ISENSOR_CALIBRATION_SERVICE_H
