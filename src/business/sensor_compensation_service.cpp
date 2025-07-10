/**
 * @file sensor_compensation_service.cpp
 * @brief Реализация сервиса компенсации датчиков
 * @details Применяет научные алгоритмы компенсации к показаниям датчиков
 */

#include "sensor_compensation_service.h"
#include "../../include/jxct_constants.h"
#include "../../include/logger.h"
#include "../../include/sensor_compensation.h"
#include <cmath>

SensorCompensationService::SensorCompensationService() {
    logDebugSafe("SensorCompensationService: Инициализация сервиса компенсации");
    initializeArchieCoefficients();
    initializeSoilParameters();
}

void SensorCompensationService::applyCompensation(SensorData& data, SoilType soilType) {
    logDebugSafe("SensorCompensationService: Применение компенсации для типа почвы %d", static_cast<int>(soilType));

    // Применяем компенсацию к каждому параметру
    data.ec = correctEC(data.ec, soilType, data.temperature, data.humidity);
    data.ph = correctPH(data.ph, data.temperature); // pH, затем температура

    NPKReferences npk(data.nitrogen, data.phosphorus, data.potassium);
    correctNPK(data.temperature, data.humidity, soilType, npk);

    data.nitrogen = npk.nitrogen;
    data.phosphorus = npk.phosphorus;
    data.potassium = npk.potassium;

    logDebugSafe("SensorCompensationService: Компенсация применена");
}

float SensorCompensationService::correctEC(float ec25_param, SoilType soilType_param, float temperature_param, float humidity_param) {
    if (!validateCompensationInputs(soilType_param, humidity_param, temperature_param)) {
        logDebugSafe("SensorCompensationService: Недопустимые входные данные для компенсации EC");
        return ec25_param;
    }

    // Получаем коэффициенты Арчи для типа почвы
    const ArchieCoefficients coeffs = getArchieCoefficients(soilType_param);

    // Температурная компенсация по модели Арчи
    const float tempFactor = SensorCompensationService::calculateECTemperatureFactor(temperature_param);

    // Влажностная компенсация по модели Арчи
    const float humidityFactor = SensorCompensationService::calculateECHumidityFactor(humidity_param, soilType_param);

    // Применяем модель Арчи: EC = EC0 * (θ/θ0)^m * (T/T0)^n
    float compensatedEC = ec25_param * pow(humidityFactor, coeffs.m) * pow(tempFactor, coeffs.n);

    logDebugSafe("SensorCompensationService: EC скорректирован %.2f → %.2f", ec25_param, compensatedEC);
    return compensatedEC;
}

// @param phRaw_param - исходное значение pH (первый параметр)
// @param temperature_param - температура почвы (второй параметр)
float SensorCompensationService::correctPH(float phRaw_param, float temperature_param) // NOLINT(bugprone-easily-swappable-parameters)
{
    if (temperature_param < -50.0F || temperature_param > 100.0F) {
        logDebugSafe("SensorCompensationService: Недопустимая температура для компенсации pH: %.2f", temperature_param);
        return phRaw_param;
    }

    // Уравнение Нернста для температурной компенсации pH
    const float tempKelvin = SensorCompensationService::temperatureToKelvin(temperature_param);
    const float tempCorrection = ((R * tempKelvin) / F) * (log(tempKelvin / T0) / log(10.0F));

    const float compensatedPH = phRaw_param + tempCorrection;

    logDebugSafe("SensorCompensationService: pH скорректирован %.2f → %.2f", phRaw_param, compensatedPH);
    return compensatedPH;
}

void SensorCompensationService::correctNPK(float temperature, float humidity, SoilType soilType, NPKReferences& npk) {
    if (!validateCompensationInputs(soilType, humidity, temperature)) {
        logDebugSafe("SensorCompensationService: Недопустимые входные данные для компенсации NPK");
        return;
    }

    // Получаем параметры почвы
    const SoilParameters params = getSoilParameters(soilType);

    // Алгоритм FAO 56 для компенсации NPK
    const float tempFactor = 1.0F + (0.02F * (temperature - 25.0F));
    const float humidityFactor = 1.0F + (0.01F * (humidity - (params.fieldCapacity * 100.0F)));

    // Применяем компенсацию с учетом пористости почвы
    const float porosityFactor = params.porosity / 0.45F; // Нормализация к стандартной пористости

    npk.nitrogen *= (((tempFactor * humidityFactor) * porosityFactor));
    npk.phosphorus *= (((tempFactor * humidityFactor) * porosityFactor));
    npk.potassium *= (((tempFactor * humidityFactor) * porosityFactor));

    logDebugSafe("SensorCompensationService: NPK скорректирован N:%.2f P:%.2f K:%.2f",
              npk.nitrogen, npk.phosphorus, npk.potassium);
}

float SensorCompensationService::getArchieCoefficient(SoilType soilType) const {
    auto iter = archieCoefficients.find(soilType);
    if (iter != archieCoefficients.end()) {
        return iter->second.m; // Возвращаем коэффициент цементации
    }
    return 1.5F; // Значение по умолчанию
}

float SensorCompensationService::getPorosity(SoilType soilType) const {
    auto iter = soilParameters.find(soilType);
    if (iter != soilParameters.end()) {
        return iter->second.porosity;
    }
    return 0.45F; // Значение по умолчанию
}

// @param soilType_param - тип почвы (первый параметр)
// @param humidity_param - влажность (второй параметр) 
// @param temperature_param - температура (третий параметр)
// ВНИМАНИЕ: порядок параметров важен! soilType, humidity, temperature
bool SensorCompensationService::validateCompensationInputs(SoilType soilType_param, float humidity_param, float temperature_param) const // NOLINT(bugprone-easily-swappable-parameters)
{
    if (temperature_param < -50.0F || temperature_param > 100.0F) {
        return false;
    }

    if (humidity_param < 0.0F || humidity_param > 100.0F) {
        return false;
    }

    // Проверяем, что тип почвы валиден
    switch (soilType_param) {
        case SoilType::SAND:
        case SoilType::LOAM:
        case SoilType::PEAT:
        case SoilType::CLAY:
        case SoilType::SANDPEAT:
            return true;
        default:
            return false;
    }
}

namespace {
void initializeArchieCoefficients(std::map<SoilType, ArchieCoefficients>& archieCoefficients) {
    // Коэффициенты Арчи для разных типов почвы
    // Источник: Archie, G.E. (1942). The electrical resistivity log as an aid in determining some reservoir characteristics.

    archieCoefficients[SoilType::SAND] = ArchieCoefficients(1.3F, 2.0F, 0.35F);
    archieCoefficients[SoilType::LOAM] = ArchieCoefficients(1.5F, 2.0F, 0.45F);
    archieCoefficients[SoilType::PEAT] = ArchieCoefficients(1.8F, 2.2F, 0.80F);
    archieCoefficients[SoilType::CLAY] = ArchieCoefficients(2.0F, 2.5F, 0.50F);
    archieCoefficients[SoilType::SANDPEAT] = ArchieCoefficients(1.6F, 2.1F, 0.60F);

    logDebugSafe("SensorCompensationService: Коэффициенты Арчи инициализированы");
}
} // end anonymous namespace

void SensorCompensationService::initializeArchieCoefficients() {
    ::initializeArchieCoefficients(archieCoefficients);
}

namespace {
void initializeSoilParameters(std::map<SoilType, SoilParameters>& soilParameters) {
    // Параметры почвы для разных типов
    // Источник: FAO 56 - Crop evapotranspiration

    soilParameters[SoilType::SAND] = SoilParameters(0.35F, 1.60F, 0.10F);
    soilParameters[SoilType::LOAM] = SoilParameters(0.45F, 1.40F, 0.20F);
    soilParameters[SoilType::PEAT] = SoilParameters(0.80F, 0.30F, 0.45F);
    soilParameters[SoilType::CLAY] = SoilParameters(0.50F, 1.20F, 0.35F);
    soilParameters[SoilType::SANDPEAT] = SoilParameters(0.60F, 0.80F, 0.30F);

    logDebugSafe("SensorCompensationService: Параметры почвы инициализированы");
}
} // end anonymous namespace

void SensorCompensationService::initializeSoilParameters() {
    ::initializeSoilParameters(soilParameters);
}

SoilParameters SensorCompensationService::getSoilParameters(SoilType soilType) const { // NOLINT(readability-convert-member-functions-to-static)
    auto iter = soilParameters.find(soilType);
    if (iter != soilParameters.end()) {
        return iter->second;
    }
    return {}; // Возвращаем значения по умолчанию
}

ArchieCoefficients SensorCompensationService::getArchieCoefficients(SoilType soilType) const { // NOLINT(readability-convert-member-functions-to-static)
    auto iter = archieCoefficients.find(soilType);
    if (iter != archieCoefficients.end()) {
        return iter->second;
    }
    return {}; // Возвращаем значения по умолчанию
}

float SensorCompensationService::temperatureToKelvin(float celsius) {
    return celsius + 273.15F;
}

float SensorCompensationService::calculateECTemperatureFactor(float temperature) {
    // Температурная компенсация для EC
    // При 25°C коэффициент = 1.0
    return 1.0F + (0.02F * (temperature - 25.0F));
}

float SensorCompensationService::calculateECHumidityFactor(float humidity, SoilType soilType) const {
    // Влажностная компенсация для EC
    // Нормализация к полевой влагоемкости
    const SoilParameters params = getSoilParameters(soilType);
    const float fieldCapacityPercent = params.fieldCapacity * 100.0F;

    return 1.0F + (0.01F * (humidity - fieldCapacityPercent));
}
