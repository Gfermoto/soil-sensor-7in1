#pragma once

#ifdef TEST_BUILD
#include "../test/stubs/esp32_stubs.h"
#else
#include <Arduino.h>
#endif
#include <ArduinoJson.h>
#include <vector>
#include <map>
#include "modbus_sensor.h"  // Для структуры SensorData

// ============================================================================
// КОНСТАНТЫ СТАНДАРТНЫХ РАСТВОРОВ ДЛЯ КАЛИБРОВКИ
// ============================================================================

/**
 * @brief Стандартные буферные растворы для pH калибровки
 * 
 * Используются для точной калибровки pH датчиков
 * Максимум 6 стандартных растворов
 */
namespace StandardBuffers {
    // pH буферные растворы (стандартные)
    constexpr float PH_4_01 = 4.01F;    // Кислый буфер
    constexpr float PH_6_86 = 6.86F;    // Нейтральный буфер
    constexpr float PH_7_00 = 7.00F;    // Нейтральный буфер
    constexpr float PH_9_18 = 9.18F;    // Щелочной буфер
    constexpr float PH_10_01 = 10.01F;  // Высокий щелочной буфер
    
    // EC стандартные растворы (KCl)
    constexpr float EC_1413_US = 1413.0F;    // 1413 µS/cm
    constexpr float EC_1_413_MS = 1.413F;    // 1.413 mS/cm
    constexpr float EC_12_88_MS = 12.88F;    // 12.88 mS/cm
    constexpr float EC_0_1413_MS = 0.1413F;  // 0.1413 mS/cm
    
    // Максимальное количество калибровочных точек
    constexpr size_t MAX_PH_POINTS = 6;   // pH буферные растворы
    constexpr size_t MAX_EC_POINTS = 5;   // EC стандартные растворы
    constexpr size_t MAX_NPK_POINTS = 3;  // NPK калибровка (обычно 1-3 точки)
    constexpr size_t MAX_TOTAL_POINTS = MAX_PH_POINTS + MAX_EC_POINTS + MAX_NPK_POINTS;
}

/**
 * @file sensor_calibration_service.h
 * @brief Единая система калибровки и поверки датчиков
 * @details Объединяет функциональность простой и продвинутой калибровки
 * Позволяет вводить показания датчика для эталонных растворов
 * и автоматически рассчитывать корректирующие коэффициенты
 */

// ============================================================================
// СТРУКТУРЫ ДАННЫХ КАЛИБРОВКИ
// ============================================================================

/**
 * @brief Точка калибровки (универсальная)
 * 
 * Содержит пару значений: исходное и эталонное
 * Поддерживает как простую, так и продвинутую калибровку
 */
struct CalibrationPoint
{
    float rawValue;        // Исходное значение датчика
    float referenceValue;  // Эталонное значение

    // Конструкторы для обратной совместимости
    CalibrationPoint() : rawValue(0), referenceValue(0) {}
    CalibrationPoint(float raw, float reference) : rawValue(raw), referenceValue(reference) {}
    
    // Методы для обратной совместимости
    float getExpected() const { return referenceValue; }
    float getMeasured() const { return rawValue; }
    void setExpected(float value) { referenceValue = value; }
    void setMeasured(float value) { rawValue = value; }
    
    // Операторы присваивания для совместимости с STL
    CalibrationPoint& operator=(const CalibrationPoint& other) {
        if (this != &other) {
            rawValue = other.rawValue;
            referenceValue = other.referenceValue;
        }
        return *this;
    }
    
    CalibrationPoint& operator=(CalibrationPoint&& other) noexcept {
        if (this != &other) {
            rawValue = other.rawValue;
            referenceValue = other.referenceValue;
        }
        return *this;
    }
    
    // Конструктор копирования
    CalibrationPoint(const CalibrationPoint& other) : rawValue(other.rawValue), referenceValue(other.referenceValue) {}
    
    // Конструктор перемещения
    CalibrationPoint(CalibrationPoint&& other) noexcept : rawValue(other.rawValue), referenceValue(other.referenceValue) {}
};

/**
 * @brief Калибровка pH
 * 
 * Использует линейную регрессию: pH_corrected = a × pH_raw + b
 */
struct PHCalibration
{
    std::vector<CalibrationPoint> points;
    float coefficient_a;  // pH_corrected = a × pH_raw + b
    float coefficient_b;
    float r_squared;  // Коэффициент детерминации
    bool is_valid;

    PHCalibration() : coefficient_a(1.0f), coefficient_b(0.0f), r_squared(0.0f), is_valid(false) {}
};

/**
 * @brief Калибровка EC (электропроводность)
 * 
 * Использует линейную регрессию: EC_corrected = c × EC_raw + d
 */
struct ECCalibration
{
    std::vector<CalibrationPoint> points;
    float coefficient_c;  // EC_corrected = c × EC_raw + d
    float coefficient_d;
    float r_squared;
    bool is_valid;

    ECCalibration() : coefficient_c(1.0f), coefficient_d(0.0f), r_squared(0.0f), is_valid(false) {}
};

/**
 * @brief Калибровка NPK
 * 
 * Использует аддитивную коррекцию: NPK_corrected = NPK_raw + offset
 */
struct NPKCalibration
{
    float offset_nitrogen;    // NPK_corrected = NPK_raw + offset
    float offset_phosphorus;
    float offset_potassium;
    bool is_valid;

    NPKCalibration() : offset_nitrogen(0.0f), offset_phosphorus(0.0f), offset_potassium(0.0f), is_valid(false) {}
};

/**
 * @brief Калибровочная таблица для профилей почвы
 * 
 * Содержит набор точек калибровки для конкретного профиля почвы
 */
struct CalibrationTable
{
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
 * @brief Полная калибровка датчика
 * 
 * Содержит все калибровочные данные для одного датчика
 */
struct SensorCalibration
{
    String sensor_id;
    String calibration_date;
    PHCalibration ph;
    ECCalibration ec;
    NPKCalibration npk;
    bool is_complete;

    SensorCalibration() : is_complete(false) {}
};

// ============================================================================
// ОСНОВНОЙ КЛАСС КАЛИБРОВКИ
// ============================================================================

/**
 * @brief Единый сервис калибровки датчиков
 * 
 * Объединяет функциональность простой и продвинутой калибровки.
 * Поддерживает как точечную калибровку, так и калибровочные таблицы.
 */
class SensorCalibrationService
{
   private:
    // Текущая калибровка датчика
    SensorCalibration current_calibration;
    
    // Калибровочные таблицы для разных профилей почвы
    static std::map<int, CalibrationTable> calibrationTables;

    // ============================================================================
    // ПРИВАТНЫЕ МЕТОДЫ
    // ============================================================================

    // Расчёт коэффициентов линейной регрессии
    bool calculateLinearRegression(const std::vector<CalibrationPoint>& points, float& slope, float& intercept,
                                   float& r_squared);

    // Применение калибровки к сырым данным
    float applyPHCalibration(float raw_ph);
    float applyECCalibration(float raw_ec);
    void applyNPKCalibration(float& nitrogen, float& phosphorus, float& potassium);

    // Применение калибровки к значению с интерполяцией
    float applyCalibrationWithInterpolation(float rawValue, const std::vector<CalibrationPoint>& points) const;

    // Линейная интерполяция между двумя точками
    float linearInterpolation(float value, float x1, float y1, float x2, float y2) const;

    // Парсинг CSV данных калибровочной таблицы
    bool parseCalibrationCSV(const String& csvData, CalibrationTable& table);

    // Валидация калибровочных точек
    bool validateCalibrationPoints(const std::vector<CalibrationPoint>& points) const;

   public:
    // ============================================================================
    // КОНСТРУКТОР И ДЕСТРУКТОР
    // ============================================================================
    
    SensorCalibrationService();
    virtual ~SensorCalibrationService() = default;

    // ============================================================================
    // МЕТОДЫ ДОБАВЛЕНИЯ КАЛИБРОВОЧНЫХ ТОЧЕК
    // ============================================================================

    /**
     * @brief Добавляет точку калибровки pH
     * 
     * @param expected Ожидаемое значение pH
     * @param measured Измеренное значение pH
     * @return true если точка добавлена успешно
     */
    bool addPHCalibrationPoint(float expected, float measured);

    /**
     * @brief Добавляет точку калибровки EC
     * 
     * @param expected Ожидаемое значение EC
     * @param measured Измеренное значение EC
     * @return true если точка добавлена успешно
     */
    bool addECCalibrationPoint(float expected, float measured);

    /**
     * @brief Устанавливает точку калибровки NPK
     * 
     * @param measured_n Измеренное значение азота
     * @param measured_p Измеренное значение фосфора
     * @param measured_k Измеренное значение калия
     * @return true если точка установлена успешно
     */
    bool setNPKCalibrationPoint(float measured_n, float measured_p, float measured_k);

    // ============================================================================
    // МЕТОДЫ РАСЧЁТА КАЛИБРОВОЧНЫХ КОЭФФИЦИЕНТОВ
    // ============================================================================

    /**
     * @brief Рассчитывает коэффициенты калибровки pH
     * 
     * @return true если расчёт успешен
     */
    bool calculatePHCalibration();

    /**
     * @brief Рассчитывает коэффициенты калибровки EC
     * 
     * @return true если расчёт успешен
     */
    bool calculateECCalibration();

    /**
     * @brief Рассчитывает коэффициенты калибровки NPK
     * 
     * @return true если расчёт успешен
     */
    bool calculateNPKCalibration();

    // ============================================================================
    // МЕТОДЫ ПРИМЕНЕНИЯ КАЛИБРОВКИ
    // ============================================================================

    /**
     * @brief Применяет калибровку к данным датчика (простая версия)
     * 
     * @param ph pH для калибровки
     * @param ec EC для калибровки
     * @param nitrogen Азот для калибровки
     * @param phosphorus Фосфор для калибровки
     * @param potassium Калий для калибровки
     */
    void applyCalibration(float& ph, float& ec, float& nitrogen, float& phosphorus, float& potassium);

    /**
     * @brief Применяет калибровку к данным датчика (продвинутая версия)
     * 
     * @param data Данные датчика для калибровки
     * @param profile Профиль почвы для выбора калибровочной таблицы
     */
    void applyCalibration(SensorData& data, int profile);

    /**
     * @brief Применяет калибровку к отдельному значению
     * 
     * @param rawValue Исходное значение
     * @param profile Профиль почвы
     * @return float Откалиброванное значение
     */
    float applySingleCalibration(float rawValue, int profile);

    // ============================================================================
    // МЕТОДЫ СЕРИАЛИЗАЦИИ/ДЕСЕРИАЛИЗАЦИИ
    // ============================================================================

    /**
     * @brief Экспортирует калибровку в JSON
     * 
     * @return String JSON строка с калибровочными данными
     */
    String exportCalibrationToJSON();

    /**
     * @brief Импортирует калибровку из JSON
     * 
     * @param json_data JSON строка с калибровочными данными
     * @return true если импорт успешен
     */
    bool importCalibrationFromJSON(const String& json_data);

    // ============================================================================
    // МЕТОДЫ УПРАВЛЕНИЯ КАЛИБРОВОЧНЫМИ ТАБЛИЦАМИ
    // ============================================================================

    /**
     * @brief Загружает калибровочную таблицу из CSV
     * 
     * @param csvData CSV данные калибровочной таблицы
     * @param profile Профиль почвы
     * @return true если загрузка успешна
     */
    bool loadCalibrationTable(const String& csvData, int profile);

    /**
     * @brief Проверяет наличие калибровочной таблицы
     * 
     * @param profile Профиль почвы
     * @return true если таблица загружена
     */
    bool hasCalibrationTable(int profile) const;

    /**
     * @brief Очищает калибровочную таблицу
     * 
     * @param profile Профиль почвы
     */
    void clearCalibrationTable(int profile);

    /**
     * @brief Получает количество точек калибровки
     * 
     * @param profile Профиль почвы
     * @param sensorType Тип датчика
     * @return size_t Количество точек калибровки
     */
    static size_t getCalibrationPointsCount(int profile, const String& sensorType);

    /**
     * @brief Экспортирует калибровочную таблицу в CSV
     * 
     * @param profile Профиль почвы
     * @return String CSV данные калибровочной таблицы
     */
    static String exportCalibrationTable(int profile);

    // ============================================================================
    // МЕТОДЫ СТАТУСА И ВАЛИДАЦИИ
    // ============================================================================

    /**
     * @brief Получает статус калибровки
     * 
     * @return String Описание статуса калибровки
     */
    String getCalibrationStatus() const;

    /**
     * @brief Проверяет завершённость калибровки
     * 
     * @return true если калибровка завершена
     */
    bool isCalibrationComplete() const;

    /**
     * @brief Сбрасывает калибровку
     */
    void resetCalibration();

    /**
     * @brief Валидирует калибровку
     * 
     * @return true если калибровка валидна
     */
    bool validateCalibration() const;

    /**
     * @brief Проверяет валидность калибровочных данных
     * 
     * @param data Данные датчика для проверки
     * @return true если данные валидны
     */
    bool validateCalibrationData(const SensorData& data);
};

// ============================================================================
// ГЛОБАЛЬНЫЙ ЭКЗЕМПЛЯР
// ============================================================================

extern SensorCalibrationService gCalibrationService;
