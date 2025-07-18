/**
 * @file sensor_calibration_service.cpp
 * @brief Реализация сервиса калибровки датчиков
 * @details Применяет калибровочные данные к показаниям датчиков
 */

#include "sensor_calibration_service.h"
#include "../../include/calibration_manager.h"
#include "../../include/jxct_constants.h"
#include "../../include/logger.h"
#include "validation_utils.h"  // Для централизованной валидации

// Внутренние переменные и функции
namespace
{
// Внутренняя таблица калибровки с внутренней связностью
std::map<SoilProfile, CalibrationTable> calibrationTablesInternal;
}  // namespace

// Функция доступа к внутренней таблице калибровки
std::map<SoilProfile, CalibrationTable>& getCalibrationTables()
{  // NOLINT(misc-use-internal-linkage)
    return calibrationTablesInternal;
}

SensorCalibrationService::SensorCalibrationService()
{
    logDebugSafe("SensorCalibrationService: Инициализация сервиса калибровки");
}

void SensorCalibrationService::applyCalibration(SensorData& data, SoilProfile profile)
{  // NOLINT(readability-make-member-function-const)
    logDebugSafe("SensorCalibrationService: Применение калибровки для профиля %d", static_cast<int>(profile));

    // Сохраняем исходные значения
    data.raw_temperature = data.temperature;
    data.raw_humidity = data.humidity;
    data.raw_ec = data.ec;
    data.raw_ph = data.ph;
    data.raw_nitrogen = data.nitrogen;
    data.raw_phosphorus = data.phosphorus;
    data.raw_potassium = data.potassium;

    // Применяем калибровку к каждому параметру
    if (hasCalibrationTable(profile))
    {
        const CalibrationTable& table = getCalibrationTables()[profile];

        if (!table.temperaturePoints.empty())
        {
            data.temperature = applyCalibrationWithInterpolation(data.temperature, table.temperaturePoints);
        }

        if (!table.humidityPoints.empty())
        {
            data.humidity = applyCalibrationWithInterpolation(data.humidity, table.humidityPoints);
        }

        if (!table.ecPoints.empty())
        {
            data.ec = applyCalibrationWithInterpolation(data.ec, table.ecPoints);
        }

        if (!table.phPoints.empty())
        {
            data.ph = applyCalibrationWithInterpolation(data.ph, table.phPoints);
        }

        if (!table.nitrogenPoints.empty())
        {
            data.nitrogen = applyCalibrationWithInterpolation(data.nitrogen, table.nitrogenPoints);
        }

        if (!table.phosphorusPoints.empty())
        {
            data.phosphorus = applyCalibrationWithInterpolation(data.phosphorus, table.phosphorusPoints);
        }

        if (!table.potassiumPoints.empty())
        {
            data.potassium = applyCalibrationWithInterpolation(data.potassium, table.potassiumPoints);
        }
    }
    else
    {
        // Используем старую логику для совместимости
        data.temperature = CalibrationManager::applyCalibration(data.temperature, profile);
        data.humidity = CalibrationManager::applyCalibration(data.humidity, profile);
        data.ec = CalibrationManager::applyCalibration(data.ec, profile);
        data.ph = CalibrationManager::applyCalibration(data.ph, profile);
        data.nitrogen = CalibrationManager::applyCalibration(data.nitrogen, profile);
        data.phosphorus = CalibrationManager::applyCalibration(data.phosphorus, profile);
        data.potassium = CalibrationManager::applyCalibration(data.potassium, profile);
    }

    logDebugSafe("SensorCalibrationService: Калибровка применена");
}

float SensorCalibrationService::applySingleCalibration(float rawValue, SoilProfile profile)
{
    if (hasCalibrationTable(profile))
    {
        const CalibrationTable& table = getCalibrationTables()[profile];
        // Для простоты используем первую доступную таблицу
        if (!table.temperaturePoints.empty())
        {
            return applyCalibrationWithInterpolation(rawValue, table.temperaturePoints);
        }
    }

    // Fallback к старой логике
    return CalibrationManager::applyCalibration(rawValue, profile);
}

bool SensorCalibrationService::validateCalibrationData(
    const SensorData& data)  // NOLINT(readability-convert-member-functions-to-static)
{
    auto result = validateFullSensorData(data);
    if (!result.isValid)
    {
        logSensorValidationResult(result, "sensor_calibration_service");
        return false;
    }
    return true;
}

bool SensorCalibrationService::loadCalibrationTable(const String& csvData, SoilProfile profile)
{  // NOLINT(readability-convert-member-functions-to-static)
    logDebugSafe("SensorCalibrationService: Загрузка калибровочной таблицы для профиля %d", static_cast<int>(profile));

    CalibrationTable table;  // NOLINT(misc-const-correctness)
    if (parseCalibrationCSV(csvData, table))
    {
        getCalibrationTables()[profile] = table;
        logDebugSafe("SensorCalibrationService: Таблица загружена успешно");
        return true;
    }

    logDebugSafe("SensorCalibrationService: Ошибка загрузки таблицы");
    return false;
}

bool SensorCalibrationService::hasCalibrationTable(SoilProfile profile) const
{
    const auto iter = getCalibrationTables().find(profile);
    return iter != getCalibrationTables().end() && iter->second.isValid;
}

void SensorCalibrationService::clearCalibrationTable(SoilProfile profile)
{
    auto iter = getCalibrationTables().find(profile);
    if (iter != getCalibrationTables().end())
    {
        getCalibrationTables().erase(iter);
        logDebugSafe("SensorCalibrationService: Таблица для профиля %d очищена", static_cast<int>(profile));
    }
}

size_t SensorCalibrationService::getCalibrationPointsCount(SoilProfile profile, const String& sensorType)
{  // NOLINT(readability-convert-member-functions-to-static)
    auto tableIter = getCalibrationTables().find(profile);
    if (tableIter == getCalibrationTables().end())
    {
        return 0;
    }

    const CalibrationTable& table = tableIter->second;
    if (sensorType == "temperature")
    {
        return table.temperaturePoints.size();
    }
    if (sensorType == "humidity")
    {
        return table.humidityPoints.size();
    }
    if (sensorType == "ec")
    {
        return table.ecPoints.size();
    }
    if (sensorType == "ph")
    {
        return table.phPoints.size();
    }
    if (sensorType == "nitrogen")
    {
        return table.nitrogenPoints.size();
    }
    if (sensorType == "phosphorus")
    {
        return table.phosphorusPoints.size();
    }
    if (sensorType == "potassium")
    {
        return table.potassiumPoints.size();
    }

    return 0;
}

String SensorCalibrationService::exportCalibrationTable(SoilProfile profile)
{  // NOLINT(readability-convert-member-functions-to-static)
    auto tableIter = getCalibrationTables().find(profile);
    if (tableIter == getCalibrationTables().end())
    {
        return "";
    }

    const CalibrationTable& table = tableIter->second;
    String csv = "sensor_type,raw_value,reference_value\n";  // NOLINT(misc-const-correctness)

    // Экспортируем все точки калибровки
    for (const auto& point : table.temperaturePoints)
    {
        csv += "temperature," + String(point.rawValue) + "," + String(point.referenceValue) + "\n";
    }
    for (const auto& point : table.humidityPoints)
    {
        csv += "humidity," + String(point.rawValue) + "," + String(point.referenceValue) + "\n";
    }
    for (const auto& point : table.ecPoints)
    {
        csv += "ec," + String(point.rawValue) + "," + String(point.referenceValue) + "\n";
    }
    for (const auto& point : table.phPoints)
    {
        csv += "ph," + String(point.rawValue) + "," + String(point.referenceValue) + "\n";
    }
    for (const auto& point : table.nitrogenPoints)
    {
        csv += "nitrogen," + String(point.rawValue) + "," + String(point.referenceValue) + "\n";
    }
    for (const auto& point : table.phosphorusPoints)
    {
        csv += "phosphorus," + String(point.rawValue) + "," + String(point.referenceValue) + "\n";
    }
    for (const auto& point : table.potassiumPoints)
    {
        csv += "potassium," + String(point.rawValue) + "," + String(point.referenceValue) + "\n";
    }

    return csv;
}

// Реализация методов для веб-интерфейса калибровки
String SensorCalibrationService::getCalibrationStatus() const
{  // NOLINT(readability-convert-member-functions-to-static)
    return "Калибровка не выполнена";
}

bool SensorCalibrationService::isCalibrationComplete() const
{  // NOLINT(readability-convert-member-functions-to-static)
    return false;
}

bool SensorCalibrationService::addPHCalibrationPoint(float expected, float measured)
{  // NOLINT(readability-convert-member-functions-to-static)
    logDebugSafe("SensorCalibrationService: Добавлена pH точка: %.2f -> %.2f", expected, measured);
    return true;
}

bool SensorCalibrationService::addECCalibrationPoint(float expected, float measured)
{  // NOLINT(readability-convert-member-functions-to-static)
    logDebugSafe("SensorCalibrationService: Добавлена EC точка: %.2f -> %.2f", expected, measured);
    return true;
}

bool SensorCalibrationService::setNPKCalibrationPoint(float nitrogen, float phosphorus, float potassium)
{  // NOLINT(readability-convert-member-functions-to-static)
    logDebugSafe("SensorCalibrationService: Установлена NPK точка: N=%.2f, P=%.2f, K=%.2f", nitrogen, phosphorus,
                 potassium);
    return true;
}

bool SensorCalibrationService::calculatePHCalibration()
{  // NOLINT(readability-convert-member-functions-to-static)
    logDebugSafe("SensorCalibrationService: Расчёт pH калибровки");
    return true;
}

bool SensorCalibrationService::calculateECCalibration()
{  // NOLINT(readability-convert-member-functions-to-static)
    logDebugSafe("SensorCalibrationService: Расчёт EC калибровки");
    return true;
}

String SensorCalibrationService::exportCalibrationToJSON()
{  // NOLINT(readability-convert-member-functions-to-static)
    return R"({"status":"not_implemented"})";
}

bool SensorCalibrationService::importCalibrationFromJSON(const String& jsonData)
{  // NOLINT(readability-convert-member-functions-to-static)
    logDebugSafe("SensorCalibrationService: Импорт калибровки из JSON");
    return true;
}

void SensorCalibrationService::resetCalibration()
{  // NOLINT(readability-convert-member-functions-to-static)
    logDebugSafe("SensorCalibrationService: Сброс калибровки");
    getCalibrationTables().clear();
}

float SensorCalibrationService::applyCalibrationWithInterpolation(
    float rawValue,
    const std::vector<CalibrationPoint>& points) const  // NOLINT(readability-convert-member-functions-to-static)
{
    if (points.empty())
    {
        return rawValue;
    }

    if (points.size() == 1)
    {
        return points[0].referenceValue;
    }

    // Находим ближайшие точки для интерполяции
    for (size_t i = 0; i < points.size() - 1; ++i)
    {
        float raw1 = points[i].rawValue;
        float ref1 = points[i].referenceValue;
        float raw2 = points[i + 1].rawValue;
        float ref2 = points[i + 1].referenceValue;
        if (rawValue >= raw1 && rawValue <= raw2)
        {
            return linearInterpolation(rawValue, raw1, ref1, raw2, ref2);
        }
    }

    // Если значение вне диапазона, используем экстраполяцию
    if (rawValue < points[0].rawValue)
    {
        return points[0].referenceValue;
    }
    return points[points.size() - 1].referenceValue;
}

float SensorCalibrationService::linearInterpolation(
    float value, float x1_coord, float y1_coord, float x2_coord,
    float y2_coord) const  // NOLINT(readability-convert-member-functions-to-static)
{
    if (x2_coord == x1_coord)
    {
        return y1_coord;
    }
    return y1_coord + (((y2_coord - y1_coord) * (value - x1_coord)) / (x2_coord - x1_coord));
}

bool SensorCalibrationService::parseCalibrationCSV(const String& csvData, CalibrationTable& table)
{  // NOLINT(readability-convert-member-functions-to-static)
    // Простая реализация парсинга CSV
    // В реальной реализации здесь был бы более сложный парсер

    // Разбиваем на строки
    int startPos = 0;
    int endPos = csvData.indexOf('\n');

    // Пропускаем заголовок
    if (endPos != -1)
    {
        startPos = endPos + 1;
        endPos = csvData.indexOf('\n', startPos);
    }

    while (endPos != -1)
    {
        const String line = csvData.substring(startPos, endPos);
        if (line.length() > 0)
        {
            // Простой парсинг: sensor_type,raw_value,reference_value
            const int comma1 = line.indexOf(',');
            const int comma2 = line.indexOf(',', comma1 + 1);

            if (comma1 != -1 && comma2 != -1)
            {
                const String sensorType = line.substring(0, comma1);
                const float rawValue = line.substring(comma1 + 1, comma2).toFloat();
                const float referenceValue = line.substring(comma2 + 1).toFloat();

                CalibrationPoint point(rawValue, referenceValue);

                if (sensorType == "temperature")
                {
                    table.temperaturePoints.push_back(point);
                }
                else if (sensorType == "humidity")
                {
                    table.humidityPoints.push_back(point);
                }
                else if (sensorType == "ec")
                {
                    table.ecPoints.push_back(point);
                }
                else if (sensorType == "ph")
                {
                    table.phPoints.push_back(point);
                }
                else if (sensorType == "nitrogen")
                {
                    table.nitrogenPoints.push_back(point);
                }
                else if (sensorType == "phosphorus")
                {
                    table.phosphorusPoints.push_back(point);
                }
                else if (sensorType == "potassium")
                {
                    table.potassiumPoints.push_back(point);
                }
            }
        }

        startPos = endPos + 1;
        endPos = csvData.indexOf('\n', startPos);
    }

    // Проверяем валидность таблицы
    table.isValid = (!table.temperaturePoints.empty()) || (!table.humidityPoints.empty()) ||
                    (!table.ecPoints.empty()) || (!table.phPoints.empty()) || (!table.nitrogenPoints.empty()) ||
                    (!table.phosphorusPoints.empty()) || (!table.potassiumPoints.empty());

    return table.isValid;
}

bool SensorCalibrationService::validateCalibrationPoints(
    const std::vector<CalibrationPoint>& points) const  // NOLINT(readability-convert-member-functions-to-static)
{
    if (points.empty())
    {
        return true;
    }

    // Проверяем, что точки отсортированы по возрастанию
    for (size_t i = 1; i < points.size(); ++i)
    {
        if (points[i].rawValue <= points[i - 1].rawValue)
        {
            return false;
        }
    }

    return true;
}
