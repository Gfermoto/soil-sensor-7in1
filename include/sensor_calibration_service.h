#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>
#include <vector>

/**
 * @file sensor_calibration_service.h
 * @brief Система калибровки и поверки датчиков
 * @details Позволяет вводить показания датчика для эталонных растворов
 * и автоматически рассчитывать корректирующие коэффициенты
 */

// Структуры для калибровочных данных
struct CalibrationPoint {
    float expected;    // Ожидаемое значение (эталон)
    float measured;    // Измеренное значение датчика
};

struct PHCalibration {
    std::vector<CalibrationPoint> points;
    float coefficient_a;  // pH_corrected = a × pH_raw + b
    float coefficient_b;
    float r_squared;      // Коэффициент детерминации
    bool is_valid;
};

struct ECCalibration {
    std::vector<CalibrationPoint> points;
    float coefficient_c;  // EC_corrected = c × EC_raw + d
    float coefficient_d;
    float r_squared;
    bool is_valid;
};

struct NPKCalibration {
    float offset_nitrogen;    // NPK_corrected = NPK_raw + offset
    float offset_phosphorus;
    float offset_potassium;
    bool is_valid;
};

struct SensorCalibration {
    String sensor_id;
    String calibration_date;
    PHCalibration ph;
    ECCalibration ec;
    NPKCalibration npk;
    bool is_complete;
};

class SensorCalibrationService {
private:
    SensorCalibration current_calibration;
    
    // Расчёт коэффициентов линейной регрессии
    bool calculateLinearRegression(const std::vector<CalibrationPoint>& points, 
                                  float& slope, float& intercept, float& r_squared);
    
    // Применение калибровки к сырым данным
    float applyPHCalibration(float raw_ph);
    float applyECCalibration(float raw_ec);
    void applyNPKCalibration(float& nitrogen, float& phosphorus, float& potassium);

public:
    SensorCalibrationService();
    
    // Добавление калибровочных точек
    bool addPHCalibrationPoint(float expected, float measured);
    bool addECCalibrationPoint(float expected, float measured);
    bool setNPKCalibrationPoint(float measured_n, float measured_p, float measured_k);
    
    // Расчёт калибровочных коэффициентов
    bool calculatePHCalibration();
    bool calculateECCalibration();
    bool calculateNPKCalibration();
    
    // Применение калибровки к данным датчика
    void applyCalibration(float& ph, float& ec, float& nitrogen, float& phosphorus, float& potassium);
    
    // Сериализация/десериализация калибровки
    String exportCalibrationToJSON();
    bool importCalibrationFromJSON(const String& json_data);
    
    // Получение статуса калибровки
    bool isCalibrationComplete() const;
    String getCalibrationStatus() const;
    
    // Сброс калибровки
    void resetCalibration();
    
    // Валидация калибровки
    bool validateCalibration() const;
}; 