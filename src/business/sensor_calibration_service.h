/**
 * @file sensor_calibration_service.h
 * @brief Сервис калибровки датчиков
 * @details Применяет калибровочные данные к показаниям датчиков
 */

#ifndef SENSOR_CALIBRATION_SERVICE_H
#define SENSOR_CALIBRATION_SERVICE_H

#include <map>
#include <vector>
#include <Arduino.h>
#include "../../include/sensor_compensation.h"
#include "../../include/business/ISensorCalibrationService.h"
#include "../../include/calibration_manager.h"
#include "../../include/validation_utils.h"

/**
 * @brief Точка калибровки
 *
 * Содержит пару значений: исходное и эталонное
 */
struct CalibrationPoint {
    float rawValue;     // Исходное значение датчика
    float referenceValue; // Эталонное значение

    CalibrationPoint() : rawValue(0), referenceValue(0) {}
    CalibrationPoint(float raw, float reference)
        : rawValue(raw), referenceValue(reference) {}
};

/**
 * @brief Калибровочная таблица
 *
 * Содержит набор точек калибровки для конкретного профиля почвы
 */
struct CalibrationTable {
    std::vector<CalibrationPoint> temperaturePoints;
    std::vector<CalibrationPoint> humidityPoints;
    std::vector<CalibrationPoint> ecPoints;
    std::vector<CalibrationPoint> phPoints;
    std::vector<CalibrationPoint> nitrogenPoints;
    std::vector<CalibrationPoint> phosphorusPoints;
    std::vector<CalibrationPoint> potassiumPoints;

    bool isValid;

    CalibrationTable() : isValid(false) {}
};

/**
 * @brief Сервис калибровки датчиков
 *
 * Реализует логику применения калибровочных данных к показаниям датчиков,
 * включая загрузку калибровочных таблиц и их применение.
 */
class SensorCalibrationService : public ISensorCalibrationService {
private:
    // Калибровочные таблицы для разных профилей почвы
    static std::map<SoilProfile, CalibrationTable> calibrationTables;

    // Менеджер калибровки (для совместимости с существующим кодом)
    // CalibrationManager& calibrationManager; // Убрано - используем namespace

    // Применение калибровки к значению с интерполяцией
    float applyCalibrationWithInterpolation(float rawValue,
                                          const std::vector<CalibrationPoint>& points) const;

    // Линейная интерполяция между двумя точками
    float linearInterpolation(float value, float x1, float y1, float x2, float y2) const;

    // Парсинг CSV данных калибровочной таблицы
    bool parseCalibrationCSV(const String& csvData, CalibrationTable& table);

    // Валидация калибровочных точек
    bool validateCalibrationPoints(const std::vector<CalibrationPoint>& points) const;

public:
    /**
     * @brief Конструктор
     */
    SensorCalibrationService();

    /**
     * @brief Деструктор
     */
    virtual ~SensorCalibrationService() = default;

    /**
     * @brief Применяет калибровку к данным датчика
     *
     * @param data Данные датчика для калибровки
     * @param profile Профиль почвы для выбора калибровочной таблицы
     */
    void applyCalibration(SensorData& data, SoilProfile profile) override;

    /**
     * @brief Применяет калибровку к отдельному значению
     *
     * @param rawValue Исходное значение
     * @param profile Профиль почвы
     * @return float Откалиброванное значение
     */
    float applySingleCalibration(float rawValue, SoilProfile profile) override;

    /**
     * @brief Проверяет валидность калибровочных данных
     *
     * @param data Данные датчика для проверки
     * @return true если данные валидны, false в противном случае
     */
    bool validateCalibrationData(const SensorData& data) override;

    /**
     * @brief Загружает калибровочную таблицу из CSV
     *
     * @param csvData CSV данные калибровочной таблицы
     * @param profile Профиль почвы
     * @return true если загрузка успешна, false в противном случае
     */
    bool loadCalibrationTable(const String& csvData, SoilProfile profile) override;

    /**
     * @brief Проверяет наличие калибровочной таблицы
     *
     * @param profile Профиль почвы
     * @return true если таблица загружена, false в противном случае
     */
    bool hasCalibrationTable(SoilProfile profile) const override;

    /**
     * @brief Очищает калибровочную таблицу
     *
     * @param profile Профиль почвы
     */
    void clearCalibrationTable(SoilProfile profile) override;

    /**
     * @brief Получает количество точек калибровки
     *
     * @param profile Профиль почвы
     * @param sensorType Тип датчика
     * @return size_t Количество точек калибровки
     */
    static size_t getCalibrationPointsCount(SoilProfile profile, const String& sensorType);

    /**
     * @brief Экспортирует калибровочную таблицу в CSV
     *
     * @param profile Профиль почвы
     * @return String CSV данные калибровочной таблицы
     */
    static String exportCalibrationTable(SoilProfile profile);

    // Методы для веб-интерфейса калибровки
    String getCalibrationStatus() const;
    bool isCalibrationComplete() const;
    bool addPHCalibrationPoint(float expected, float measured);
    bool addECCalibrationPoint(float expected, float measured);
    bool setNPKCalibrationPoint(float n, float p, float k);
    bool calculatePHCalibration();
    bool calculateECCalibration();
    String exportCalibrationToJSON();
    bool importCalibrationFromJSON(const String& jsonData);
    void resetCalibration();
};

#endif // SENSOR_CALIBRATION_SERVICE_H
 