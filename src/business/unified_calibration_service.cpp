/**
 * @file unified_calibration_service.cpp
 * @brief Единая реализация сервиса калибровки датчиков
 * @details Объединяет функциональность простой и продвинутой калибровки
 * Реализует алгоритмы линейной регрессии, интерполяции и валидации
 */

#include "../../include/sensor_calibration_service.h"
#include "../../include/logger.h"
#include "../../include/validation_utils.h"
#include <cmath>
#include <algorithm>

// ============================================================================
// ГЛОБАЛЬНЫЕ ПЕРЕМЕННЫЕ
// ============================================================================

// Глобальный экземпляр сервиса калибровки
SensorCalibrationService gCalibrationService;

// Статические переменные класса
// NOLINTNEXTLINE(misc-use-internal-linkage)
std::map<int, CalibrationTable> SensorCalibrationService::calibrationTables;

// ============================================================================
// КОНСТРУКТОР И ИНИЦИАЛИЗАЦИЯ
// ============================================================================

SensorCalibrationService::SensorCalibrationService()
{
    logDebug("Инициализация единого сервиса калибровки");
    
    // Инициализация текущей калибровки
    current_calibration.sensor_id = "JXCT_UNIFIED";
    current_calibration.calibration_date = "";
    current_calibration.is_complete = false;
    
    // Очистка калибровочных таблиц
    calibrationTables.clear();
    
    logDebug("Сервис калибровки инициализирован");
}

// ============================================================================
// МЕТОДЫ ДОБАВЛЕНИЯ КАЛИБРОВОЧНЫХ ТОЧЕК
// ============================================================================

bool SensorCalibrationService::addPHCalibrationPoint(float expected, float measured)
{
    // Валидация входных данных
    if (expected < 0.0F || expected > 14.0F || measured < 0.0F || measured > 14.0F) {
        logWarn("Недопустимые значения pH калибровки: expected=" + String(expected) + ", measured=" + String(measured));
        return false;
    }
    
    // Добавление точки калибровки
    CalibrationPoint point(measured, expected);
    current_calibration.ph.points.push_back(point);
    
    // Сброс валидности - нужно пересчитать коэффициенты
    current_calibration.ph.is_valid = false;
    
    logDebug("Добавлена точка pH калибровки: " + String(expected) + " -> " + String(measured));
    return true;
}

bool SensorCalibrationService::addECCalibrationPoint(float expected, float measured)
{
    // Валидация входных данных
    if (expected < 0.0F || measured < 0.0F) {
        logWarn("Недопустимые значения EC калибровки: expected=" + String(expected) + ", measured=" + String(measured));
        return false;
    }
    
    // Добавление точки калибровки
    CalibrationPoint point(measured, expected);
    current_calibration.ec.points.push_back(point);
    
    // Сброс валидности - нужно пересчитать коэффициенты
    current_calibration.ec.is_valid = false;
    
    logDebug("Добавлена точка EC калибровки: " + String(expected) + " -> " + String(measured));
    return true;
}

bool SensorCalibrationService::setNPKCalibrationPoint(float measured_n, float measured_p, float measured_k)
{
    // Валидация входных данных
    if (measured_n < 0.0F || measured_p < 0.0F || measured_k < 0.0F) {
        logWarn("Недопустимые значения NPK калибровки: N=" + String(measured_n) + ", P=" + String(measured_p) + ", K=" + String(measured_k));
        return false;
    }
    
    // Установка смещений NPK (предполагаем, что это нулевая точка)
    current_calibration.npk.offset_nitrogen = -measured_n;
    current_calibration.npk.offset_phosphorus = -measured_p;
    current_calibration.npk.offset_potassium = -measured_k;
    current_calibration.npk.is_valid = true;
    
    logDebug("Установлена точка NPK калибровки: N=" + String(measured_n) + ", P=" + String(measured_p) + ", K=" + String(measured_k));
    return true;
}

// ============================================================================
// МЕТОДЫ РАСЧЁТА КАЛИБРОВОЧНЫХ КОЭФФИЦИЕНТОВ
// ============================================================================

// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
bool SensorCalibrationService::calculateLinearRegression(const std::vector<CalibrationPoint>& points, 
                                                        float& slope, float& intercept, float& r_squared)
{
    if (points.size() < 2) {
        logWarn("Недостаточно точек для линейной регрессии (минимум 2)");
        return false;
    }
    
    // Расчёт средних значений
    float sum_x = 0.0F;
    float sum_y = 0.0F;
    float sum_xy = 0.0F;
    float sum_x2 = 0.0F;
    float sum_y2 = 0.0F;
    const size_t num_points = points.size();
    
    for (const auto& point : points) {
        float x = point.rawValue;
        float y = point.referenceValue;
        
        sum_x += x;
        sum_y += y;
        sum_xy += x * y;
        sum_x2 += x * x;
        sum_y2 += y * y;
    }
    
    float mean_x = sum_x / num_points;
    float mean_y = sum_y / num_points;
    
    // Расчёт коэффициентов линейной регрессии
    float numerator = (num_points * sum_xy) - (sum_x * sum_y);
    float denominator = (num_points * sum_x2) - (sum_x * sum_x);
    
    if (fabs(denominator) < 1e-10F) {
        logWarn("Деление на ноль в линейной регрессии");
        return false;
    }
    
    slope = numerator / denominator;
    intercept = mean_y - slope * mean_x;
    
    // Расчёт коэффициента детерминации R²
    float ss_tot = sum_y2 - ((sum_y * sum_y) / num_points);
    float ss_res = 0.0F;
    
    for (const auto& point : points) {
        float y_pred = slope * point.rawValue + intercept;
        float residual = point.referenceValue - y_pred;
        ss_res += residual * residual;
    }
    
    r_squared = 1.0F - (ss_res / ss_tot);
    
    logDebug("Линейная регрессия: slope=" + String(slope) + ", intercept=" + String(intercept) + ", R²=" + String(r_squared));
    return true;
}

bool SensorCalibrationService::calculatePHCalibration()
{
    if (current_calibration.ph.points.size() < 2) {
        logWarn("Недостаточно точек для расчёта pH калибровки");
        return false;
    }
    
    bool success = calculateLinearRegression(current_calibration.ph.points, 
                                           current_calibration.ph.coefficient_a,
                                           current_calibration.ph.coefficient_b,
                                           current_calibration.ph.r_squared);
    
    if (success) {
        current_calibration.ph.is_valid = true;
        logInfo("pH калибровка рассчитана: a=" + String(current_calibration.ph.coefficient_a) + 
                ", b=" + String(current_calibration.ph.coefficient_b) + 
                ", R²=" + String(current_calibration.ph.r_squared));
    }
    
    return success;
}

bool SensorCalibrationService::calculateECCalibration()
{
    if (current_calibration.ec.points.size() < 2) {
        logWarn("Недостаточно точек для расчёта EC калибровки");
        return false;
    }
    
    bool success = calculateLinearRegression(current_calibration.ec.points, 
                                           current_calibration.ec.coefficient_c,
                                           current_calibration.ec.coefficient_d,
                                           current_calibration.ec.r_squared);
    
    if (success) {
        current_calibration.ec.is_valid = true;
        logInfo("EC калибровка рассчитана: c=" + String(current_calibration.ec.coefficient_c) + 
                ", d=" + String(current_calibration.ec.coefficient_d) + 
                ", R²=" + String(current_calibration.ec.r_squared));
    }
    
    return success;
}

// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
bool SensorCalibrationService::calculateNPKCalibration()
{
    // NPK калибровка уже установлена в setNPKCalibrationPoint
    if (current_calibration.npk.is_valid) {
        logInfo("NPK калибровка уже рассчитана");
        return true;
    }
    
    logWarn("NPK калибровка не установлена");
    return false;
}

// ============================================================================
// МЕТОДЫ ПРИМЕНЕНИЯ КАЛИБРОВКИ
// ============================================================================

// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
float SensorCalibrationService::applyPHCalibration(float raw_ph)
{
    if (!current_calibration.ph.is_valid) {
        logDebug("pH калибровка не применена (не валидна)");
        return raw_ph;
    }
    
    float calibrated_ph = current_calibration.ph.coefficient_a * raw_ph + current_calibration.ph.coefficient_b;
    
    // Ограничение диапазона pH
    calibrated_ph = std::max(0.0F, std::min(14.0F, calibrated_ph));
    
    logDebug("pH калибровка: " + String(raw_ph) + " -> " + String(calibrated_ph));
    return calibrated_ph;
}

// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
float SensorCalibrationService::applyECCalibration(float raw_ec)
{
    if (!current_calibration.ec.is_valid) {
        logDebug("EC калибровка не применена (не валидна)");
        return raw_ec;
    }
    
    float calibrated_ec = current_calibration.ec.coefficient_c * raw_ec + current_calibration.ec.coefficient_d;
    
    // Ограничение диапазона EC
    calibrated_ec = std::max(0.0F, calibrated_ec);
    
    logDebug("EC калибровка: " + String(raw_ec) + " -> " + String(calibrated_ec));
    return calibrated_ec;
}

// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
void SensorCalibrationService::applyNPKCalibration(float& nitrogen, float& phosphorus, float& potassium)
{
    if (!current_calibration.npk.is_valid) {
        logDebug("NPK калибровка не применена (не валидна)");
        return;
    }
    
    nitrogen += current_calibration.npk.offset_nitrogen;
    phosphorus += current_calibration.npk.offset_phosphorus;
    potassium += current_calibration.npk.offset_potassium;
    
    // Ограничение диапазона NPK
    nitrogen = std::max(0.0F, nitrogen);
    phosphorus = std::max(0.0F, phosphorus);
    potassium = std::max(0.0F, potassium);
    
    logDebug("NPK калибровка применена");
}

void SensorCalibrationService::applyCalibration(float& ph_value, float& ec_value, float& nitrogen, float& phosphorus, float& potassium)
{
    // Применение калибровки к каждому параметру
    ph_value = applyPHCalibration(ph_value);
    ec_value = applyECCalibration(ec_value);
    applyNPKCalibration(nitrogen, phosphorus, potassium);
    
    logDebug("Калибровка применена ко всем параметрам");
}

void SensorCalibrationService::applyCalibration(SensorData& data, int profile)
{
    // Сначала применяем точечную калибровку
    data.ph = applyPHCalibration(data.ph);
    data.ec = applyECCalibration(data.ec);
    applyNPKCalibration(data.nitrogen, data.phosphorus, data.potassium);
    
    // Затем применяем калибровочную таблицу для профиля почвы
    if (hasCalibrationTable(profile)) {
        const auto& table = calibrationTables[profile];
        
        if (table.isValid) {
            data.temperature = applyCalibrationWithInterpolation(data.temperature, table.temperaturePoints);
            data.humidity = applyCalibrationWithInterpolation(data.humidity, table.humidityPoints);
            data.ec = applyCalibrationWithInterpolation(data.ec, table.ecPoints);
            data.ph = applyCalibrationWithInterpolation(data.ph, table.phPoints);
            data.nitrogen = applyCalibrationWithInterpolation(data.nitrogen, table.nitrogenPoints);
            data.phosphorus = applyCalibrationWithInterpolation(data.phosphorus, table.phosphorusPoints);
            data.potassium = applyCalibrationWithInterpolation(data.potassium, table.potassiumPoints);
        }
    }
    
    logDebug("Продвинутая калибровка применена для профиля " + String(profile));
}

// NOLINTNEXTLINE(readability-make-member-function-const)
float SensorCalibrationService::applySingleCalibration(float rawValue, int profile)
{
    // Простая интерполяция по калибровочной таблице
    if (hasCalibrationTable(profile)) {
        const auto& table = calibrationTables[profile];
        if (table.isValid) {
            // Используем EC точки как универсальные (можно расширить)
            return applyCalibrationWithInterpolation(rawValue, table.ecPoints);
        }
    }
    
    return rawValue;
}

// ============================================================================
// МЕТОДЫ ИНТЕРПОЛЯЦИИ
// ============================================================================

float SensorCalibrationService::applyCalibrationWithInterpolation(float rawValue, const std::vector<CalibrationPoint>& points) const
{
    if (points.empty()) {
        return rawValue;
    }
    
    if (points.size() == 1) {
        return points[0].referenceValue;
    }
    
    // Сортировка точек по rawValue
    std::vector<CalibrationPoint> sorted_points = points;
    std::sort(sorted_points.begin(), sorted_points.end(), 
              [](const CalibrationPoint& a, const CalibrationPoint& b) {
                  return a.rawValue < b.rawValue;
              });
    
    // Экстраполяция за пределами диапазона
    if (rawValue <= sorted_points.front().rawValue) {
        return sorted_points.front().referenceValue;
    }
    if (rawValue >= sorted_points.back().rawValue) {
        return sorted_points.back().referenceValue;
    }
    
    // Линейная интерполяция между точками
    for (size_t i = 0; i < sorted_points.size() - 1; ++i) {
        if (rawValue >= sorted_points[i].rawValue && rawValue <= sorted_points[i + 1].rawValue) {
            return linearInterpolation(rawValue, 
                                     sorted_points[i].rawValue, sorted_points[i].referenceValue,
                                     sorted_points[i + 1].rawValue, sorted_points[i + 1].referenceValue);
        }
    }
    
    return rawValue;  // fallback
}

float SensorCalibrationService::linearInterpolation(float value, float x1_value, float y1_value, float x2_value, float y2_value) const
{
    if (fabs(x2_value - x1_value) < 1e-10F) {
        return y1_value;  // Избегаем деления на ноль
    }
    
    return y1_value + (((y2_value - y1_value) * (value - x1_value)) / (x2_value - x1_value));
}

// ============================================================================
// МЕТОДЫ УПРАВЛЕНИЯ КАЛИБРОВОЧНЫМИ ТАБЛИЦАМИ
// ============================================================================

bool SensorCalibrationService::loadCalibrationTable(const String& csvData, int profile)
{
    CalibrationTable table;
    
    if (!parseCalibrationCSV(csvData, table)) {
        logWarn("Ошибка парсинга CSV калибровочной таблицы для профиля " + String(profile));
        return false;
    }
    
    calibrationTables[profile] = table;
    logInfo("Калибровочная таблица загружена для профиля " + String(profile));
    return true;
}

// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
bool SensorCalibrationService::hasCalibrationTable(int profile) const
{
    return calibrationTables.find(profile) != calibrationTables.end() && 
           calibrationTables.at(profile).isValid;
}

// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
void SensorCalibrationService::clearCalibrationTable(int profile)
{
    calibrationTables.erase(profile);
    logInfo("Калибровочная таблица очищена для профиля " + String(profile));
}

size_t SensorCalibrationService::getCalibrationPointsCount(int profile, const String& sensorType)
{
    auto table_iter = calibrationTables.find(profile);
    if (table_iter == calibrationTables.end() || !table_iter->second.isValid) {
        return 0;
    }
    
    const auto& table = table_iter->second;
    
    if (sensorType == "temperature") {
        return table.temperaturePoints.size();
    }
    if (sensorType == "humidity") {
        return table.humidityPoints.size();
    }
    if (sensorType == "ec") {
        return table.ecPoints.size();
    }
    if (sensorType == "ph") {
        return table.phPoints.size();
    }
    if (sensorType == "nitrogen") {
        return table.nitrogenPoints.size();
    }
    if (sensorType == "phosphorus") {
        return table.phosphorusPoints.size();
    }
    if (sensorType == "potassium") {
        return table.potassiumPoints.size();
    }
    
    return 0;
}

String SensorCalibrationService::exportCalibrationTable(int profile)
{
    auto table_iter = calibrationTables.find(profile);
    if (table_iter == calibrationTables.end() || !table_iter->second.isValid) {
        return "";
    }
    
    const auto& table = table_iter->second;
    String csv = "# Калибровочная таблица для профиля " + String(profile) + "\n";
    csv += "# Формат: raw_value,reference_value\n";
    
    // Экспорт всех типов точек
    for (const auto& point : table.temperaturePoints) {
        csv += "temperature," + String(point.rawValue) + "," + String(point.referenceValue) + "\n";
    }
    for (const auto& point : table.humidityPoints) {
        csv += "humidity," + String(point.rawValue) + "," + String(point.referenceValue) + "\n";
    }
    for (const auto& point : table.ecPoints) {
        csv += "ec," + String(point.rawValue) + "," + String(point.referenceValue) + "\n";
    }
    for (const auto& point : table.phPoints) {
        csv += "ph," + String(point.rawValue) + "," + String(point.referenceValue) + "\n";
    }
    for (const auto& point : table.nitrogenPoints) {
        csv += "nitrogen," + String(point.rawValue) + "," + String(point.referenceValue) + "\n";
    }
    for (const auto& point : table.phosphorusPoints) {
        csv += "phosphorus," + String(point.rawValue) + "," + String(point.referenceValue) + "\n";
    }
    for (const auto& point : table.potassiumPoints) {
        csv += "potassium," + String(point.rawValue) + "," + String(point.referenceValue) + "\n";
    }
    
    return csv;
}

// ============================================================================
// МЕТОДЫ ПАРСИНГА И ВАЛИДАЦИИ
// ============================================================================

bool SensorCalibrationService::parseCalibrationCSV(const String& csvData, CalibrationTable& table)
{
    // Простая реализация парсинга CSV
    // В реальной версии можно использовать более сложный парсер
    
    String lines[100];  // Максимум 100 строк
    int lineCount = 0;
    
    // Разбиваем на строки
    int start = 0;
    int end = csvData.indexOf('\n');
    
    while (end >= 0 && lineCount < 100) {
        lines[lineCount++] = csvData.substring(start, end);
        start = end + 1;
        end = csvData.indexOf('\n', start);
    }
    
    if (lineCount < 100) {
        lines[lineCount++] = csvData.substring(start);
    }
    
    // Парсим каждую строку
    for (int i = 0; i < lineCount; ++i) {
        String line = lines[i];
        line.trim();
        
        // Пропускаем комментарии и пустые строки
        if (line.startsWith("#") || line.length() == 0) {
            continue;
        }
        
        // Парсим CSV строку
        int comma1 = line.indexOf(',');
        int comma2 = line.indexOf(',', comma1 + 1);
        
        if (comma1 >= 0 && comma2 >= 0) {
            String sensorType = line.substring(0, comma1);
            float rawValue = line.substring(comma1 + 1, comma2).toFloat();
            float referenceValue = line.substring(comma2 + 1).toFloat();
            
            CalibrationPoint point(rawValue, referenceValue);
            
            // Добавляем точку в соответствующий вектор
            if (sensorType == "temperature") {
                table.temperaturePoints.push_back(point);
            } else if (sensorType == "humidity") {
                table.humidityPoints.push_back(point);
            } else if (sensorType == "ec") {
                table.ecPoints.push_back(point);
            } else if (sensorType == "ph") {
                table.phPoints.push_back(point);
            } else if (sensorType == "nitrogen") {
                table.nitrogenPoints.push_back(point);
            } else if (sensorType == "phosphorus") {
                table.phosphorusPoints.push_back(point);
            } else if (sensorType == "potassium") {
                table.potassiumPoints.push_back(point);
            }
        }
    }
    
    // Проверяем валидность таблицы
    table.isValid = validateCalibrationPoints(table.temperaturePoints) ||
                   validateCalibrationPoints(table.humidityPoints) ||
                   validateCalibrationPoints(table.ecPoints) ||
                   validateCalibrationPoints(table.phPoints) ||
                   validateCalibrationPoints(table.nitrogenPoints) ||
                   validateCalibrationPoints(table.phosphorusPoints) ||
                   validateCalibrationPoints(table.potassiumPoints);
    
    return table.isValid;
}

// NOLINTNEXTLINE(readability-simplify-boolean-expr)
bool SensorCalibrationService::validateCalibrationPoints(const std::vector<CalibrationPoint>& points) const
{
    if (points.size() < 2) {
        return false;  // Нужно минимум 2 точки для интерполяции
    }
    
    // Проверяем, что все точки имеют валидные значения
    for (const auto& point : points) {
        if (std::isnan(point.rawValue) || std::isnan(point.referenceValue) ||
            std::isinf(point.rawValue) || std::isinf(point.referenceValue)) {
            return false;
        }
    }
    
    return true;  // NOLINT(readability-simplify-boolean-expr) - Все проверки пройдены успешно
}

// ============================================================================
// МЕТОДЫ СЕРИАЛИЗАЦИИ/ДЕСЕРИАЛИЗАЦИИ
// ============================================================================

String SensorCalibrationService::exportCalibrationToJSON()
{
    // Рассчитываем необходимый размер для JSON документа
    // Основная структура: sensor_id, calibration_date, is_complete
    // pH: is_valid, coefficient_a, coefficient_b, r_squared + массив точек (до 20 точек)
    // EC: is_valid, coefficient_c, coefficient_d, r_squared + массив точек (до 20 точек)  
    // NPK: is_valid, offset_nitrogen, offset_phosphorus, offset_potassium
    // Каждая точка: expected, measured (2 поля)
    // Безопасный размер: 4KB для поддержки до 50 калибровочных точек
    DynamicJsonDocument doc(4096);
    
    // Основная информация
    doc["sensor_id"] = current_calibration.sensor_id;
    doc["calibration_date"] = current_calibration.calibration_date;
    doc["is_complete"] = current_calibration.is_complete;
    
    // pH калибровка
    JsonObject ph_obj = doc.createNestedObject("ph");
    ph_obj["is_valid"] = current_calibration.ph.is_valid;
    ph_obj["coefficient_a"] = current_calibration.ph.coefficient_a;
    ph_obj["coefficient_b"] = current_calibration.ph.coefficient_b;
    ph_obj["r_squared"] = current_calibration.ph.r_squared;
    
    JsonArray ph_points = ph_obj.createNestedArray("points");
    for (const auto& point : current_calibration.ph.points) {
        JsonObject point_obj = ph_points.createNestedObject();
        point_obj["expected"] = point.referenceValue;
        point_obj["measured"] = point.rawValue;
    }
    
    // EC калибровка
    JsonObject ec_obj = doc.createNestedObject("ec");
    ec_obj["is_valid"] = current_calibration.ec.is_valid;
    ec_obj["coefficient_c"] = current_calibration.ec.coefficient_c;
    ec_obj["coefficient_d"] = current_calibration.ec.coefficient_d;
    ec_obj["r_squared"] = current_calibration.ec.r_squared;
    
    JsonArray ec_points = ec_obj.createNestedArray("points");
    for (const auto& point : current_calibration.ec.points) {
        JsonObject point_obj = ec_points.createNestedObject();
        point_obj["expected"] = point.referenceValue;
        point_obj["measured"] = point.rawValue;
    }
    
    // NPK калибровка
    JsonObject npk_obj = doc.createNestedObject("npk");
    npk_obj["is_valid"] = current_calibration.npk.is_valid;
    npk_obj["offset_nitrogen"] = current_calibration.npk.offset_nitrogen;
    npk_obj["offset_phosphorus"] = current_calibration.npk.offset_phosphorus;
    npk_obj["offset_potassium"] = current_calibration.npk.offset_potassium;
    
    String jsonString;
    serializeJson(doc, jsonString);
    return jsonString;
}

bool SensorCalibrationService::importCalibrationFromJSON(const String& json_data)
{
    // Рассчитываем необходимый размер для JSON документа
    // Аналогично exportCalibrationToJSON - 4KB для поддержки до 50 калибровочных точек
    DynamicJsonDocument doc(4096);
    DeserializationError error = deserializeJson(doc, json_data);
    
    if (error) {
        logWarn("Ошибка парсинга JSON калибровки: " + String(error.c_str()));
        return false;
    }
    
    // Импорт основной информации
    current_calibration.sensor_id = doc["sensor_id"] | "JXCT_UNIFIED";
    current_calibration.calibration_date = doc["calibration_date"] | "";
    current_calibration.is_complete = doc["is_complete"] | false;
    
    // Импорт pH калибровки
    if (doc.containsKey("ph")) {
        JsonObject ph_obj = doc["ph"];
        current_calibration.ph.is_valid = ph_obj["is_valid"] | false;
        current_calibration.ph.coefficient_a = ph_obj["coefficient_a"] | 1.0F;
        current_calibration.ph.coefficient_b = ph_obj["coefficient_b"] | 0.0F;
        current_calibration.ph.r_squared = ph_obj["r_squared"] | 0.0F;
        
        current_calibration.ph.points.clear();
        JsonArray ph_points = ph_obj["points"];
        for (JsonObject point_obj : ph_points) {
            CalibrationPoint point(point_obj["measured"] | 0.0F, point_obj["expected"] | 0.0F);
            current_calibration.ph.points.push_back(point);
        }
    }
    
    // Импорт EC калибровки
    if (doc.containsKey("ec")) {
        JsonObject ec_obj = doc["ec"];
        current_calibration.ec.is_valid = ec_obj["is_valid"] | false;
        current_calibration.ec.coefficient_c = ec_obj["coefficient_c"] | 1.0F;
        current_calibration.ec.coefficient_d = ec_obj["coefficient_d"] | 0.0F;
        current_calibration.ec.r_squared = ec_obj["r_squared"] | 0.0F;
        
        current_calibration.ec.points.clear();
        JsonArray ec_points = ec_obj["points"];
        for (JsonObject point_obj : ec_points) {
            CalibrationPoint point(point_obj["measured"] | 0.0F, point_obj["expected"] | 0.0F);
            current_calibration.ec.points.push_back(point);
        }
    }
    
    // Импорт NPK калибровки
    if (doc.containsKey("npk")) {
        JsonObject npk_obj = doc["npk"];
        current_calibration.npk.is_valid = npk_obj["is_valid"] | false;
        current_calibration.npk.offset_nitrogen = npk_obj["offset_nitrogen"] | 0.0F;
        current_calibration.npk.offset_phosphorus = npk_obj["offset_phosphorus"] | 0.0F;
        current_calibration.npk.offset_potassium = npk_obj["offset_potassium"] | 0.0F;
    }
    
    logInfo("Калибровка импортирована из JSON");
    return true;
}

// ============================================================================
// МЕТОДЫ СТАТУСА И ВАЛИДАЦИИ
// ============================================================================

String SensorCalibrationService::getCalibrationStatus() const
{
    String status = "Статус калибровки:\n";
    
    // pH статус
    status += "pH: ";
    // NOLINTNEXTLINE(bugprone-branch-clone)
    if (current_calibration.ph.is_valid) {
        status += "✅ Валидна (R²=" + String(current_calibration.ph.r_squared, 3) + 
                  ", точек: " + String(current_calibration.ph.points.size()) + ")";
    } else {
        status += "❌ Не валидна (точек: " + String(current_calibration.ph.points.size()) + ")";
    }
    status += "\n";
    
    // EC статус
    status += "EC: ";
    // NOLINTNEXTLINE(bugprone-branch-clone)
    if (current_calibration.ec.is_valid) {
        status += "✅ Валидна (R²=" + String(current_calibration.ec.r_squared, 3) + 
                  ", точек: " + String(current_calibration.ec.points.size()) + ")";
    } else {
        status += "❌ Не валидна (точек: " + String(current_calibration.ec.points.size()) + ")";
    }
    status += "\n";
    
    // NPK статус
    status += "NPK: ";
    if (current_calibration.npk.is_valid) {
        status += "✅ Валидна";
    } else {
        status += "❌ Не валидна";
    }
    status += "\n";
    
    // Общий статус
    status += "Общий статус: ";
    if (current_calibration.is_complete) {
        status += "✅ Калибровка завершена";
    } else {
        status += "❌ Калибровка не завершена";
    }
    
    return status;
}

bool SensorCalibrationService::isCalibrationComplete() const
{
    return current_calibration.is_complete;
}

void SensorCalibrationService::resetCalibration()
{
    // Сброс текущей калибровки
    current_calibration.ph.points.clear();
    current_calibration.ph.is_valid = false;
    current_calibration.ph.coefficient_a = 1.0F;
    current_calibration.ph.coefficient_b = 0.0F;
    current_calibration.ph.r_squared = 0.0F;
    
    current_calibration.ec.points.clear();
    current_calibration.ec.is_valid = false;
    current_calibration.ec.coefficient_c = 1.0F;
    current_calibration.ec.coefficient_d = 0.0F;
    current_calibration.ec.r_squared = 0.0F;
    
    current_calibration.npk.is_valid = false;
    current_calibration.npk.offset_nitrogen = 0.0F;
    current_calibration.npk.offset_phosphorus = 0.0F;
    current_calibration.npk.offset_potassium = 0.0F;
    
    current_calibration.is_complete = false;
    
    logInfo("Калибровка сброшена");
}

// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
bool SensorCalibrationService::validateCalibration() const
{
    // Проверяем валидность всех компонентов калибровки
    bool ph_valid = current_calibration.ph.is_valid && current_calibration.ph.r_squared > 0.8F;
    bool ec_valid = current_calibration.ec.is_valid && current_calibration.ec.r_squared > 0.8F;
    bool npk_valid = current_calibration.npk.is_valid;
    
    // Калибровка считается валидной, если хотя бы один компонент валиден
    return ph_valid || ec_valid || npk_valid;
}

bool SensorCalibrationService::validateCalibrationData(const SensorData& data)
{
    // Простая валидация данных датчика
    if (data.temperature < -50.0F || data.temperature > 100.0F) {
        return false;
    }
    if (data.humidity < 0.0F || data.humidity > 100.0F) {
        return false;
    }
    if (data.ec < 0.0F || data.ec > 10000.0F) {
        return false;
    }
    if (data.ph < 0.0F || data.ph > 14.0F) {
        return false;
    }
    if (data.nitrogen < 0.0F || data.nitrogen > 2000.0F) {
        return false;
    }
    if (data.phosphorus < 0.0F || data.phosphorus > 2000.0F) {
        return false;
    }
    if (data.potassium < 0.0F || data.potassium > 2000.0F) {
        return false;
    }
    
    return true;
} 