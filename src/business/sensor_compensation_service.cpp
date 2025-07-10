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
    data.ec = correctEC(data.ec, data.temperature, data.humidity, soilType);
    data.ph = correctPH(data.ph, data.temperature);

    NPKReferences npk(data.nitrogen, data.phosphorus, data.potassium);
    correctNPK(data.temperature, data.humidity, npk, soilType);

    data.nitrogen = npk.nitrogen;
    data.phosphorus = npk.phosphorus;
    data.potassium = npk.potassium;

    logDebugSafe("SensorCompensationService: Компенсация применена");
}

float SensorCompensationService::correctEC(float ec25, float temperature, float humidity, SoilType soilType) {
    if (!validateCompensationInputs(temperature, humidity, soilType)) {
        logDebugSafe("SensorCompensationService: Недопустимые входные данные для компенсации EC");
        return ec25;
    }

    // Получаем коэффициенты Арчи для типа почвы
    ArchieCoefficients coeffs = getArchieCoefficients(soilType);

    // Температурная компенсация по модели Арчи
    float tempFactor = calculateECTemperatureFactor(temperature);

    // Влажностная компенсация по модели Арчи
    float humidityFactor = calculateECHumidityFactor(humidity, soilType);

    // Применяем модель Арчи: EC = EC0 * (θ/θ0)^m * (T/T0)^n
    float compensatedEC = ec25 * pow(humidityFactor, coeffs.m) * pow(tempFactor, coeffs.n);

    logDebugSafe("SensorCompensationService: EC скорректирован %.2f → %.2f", ec25, compensatedEC);
    return compensatedEC;
}

float SensorCompensationService::correctPH(float phRaw, float temperature) {
    if (temperature < -50.0F || temperature > 100.0F) {
        logDebugSafe("SensorCompensationService: Недопустимая температура для компенсации pH: %.2f", temperature);
        return phRaw;
    }

    // Уравнение Нернста для температурной компенсации pH
    float tempKelvin = temperatureToKelvin(temperature);
    float tempCorrection = (R * tempKelvin / F) * log(tempKelvin / T0) / log(10.0F);

    float compensatedPH = phRaw + tempCorrection;

    logDebugSafe("SensorCompensationService: pH скорректирован %.2f → %.2f", phRaw, compensatedPH);
    return compensatedPH;
}

void SensorCompensationService::correctNPK(float temperature, float humidity, NPKReferences& npk, SoilType soilType) {
    if (!validateCompensationInputs(temperature, humidity, soilType)) {
        logDebugSafe("SensorCompensationService: Недопустимые входные данные для компенсации NPK");
        return;
    }

    // Получаем параметры почвы
    SoilParameters params = getSoilParameters(soilType);

    // Алгоритм FAO 56 для компенсации NPK
    float tempFactor = 1.0F + 0.02F * (temperature - 25.0F);
    float humidityFactor = 1.0F + 0.01F * (humidity - params.fieldCapacity * 100.0F);

    // Применяем компенсацию с учетом пористости почвы
    float porosityFactor = params.porosity / 0.45F; // Нормализация к стандартной пористости

    npk.nitrogen *= tempFactor * humidityFactor * porosityFactor;
    npk.phosphorus *= tempFactor * humidityFactor * porosityFactor;
    npk.potassium *= tempFactor * humidityFactor * porosityFactor;

    logDebugSafe("SensorCompensationService: NPK скорректирован N:%.2f P:%.2f K:%.2f",
              npk.nitrogen, npk.phosphorus, npk.potassium);
}

float SensorCompensationService::getArchieCoefficient(SoilType soilType) const {
    auto it = archieCoefficients.find(soilType);
    if (it != archieCoefficients.end()) {
        return it->second.m; // Возвращаем коэффициент цементации
    }
    return 1.5F; // Значение по умолчанию
}

float SensorCompensationService::getPorosity(SoilType soilType) const {
    auto it = soilParameters.find(soilType);
    if (it != soilParameters.end()) {
        return it->second.porosity;
    }
    return 0.45F; // Значение по умолчанию
}

bool SensorCompensationService::validateCompensationInputs(float temperature, float humidity, SoilType soilType) const {
    if (temperature < -50.0F || temperature > 100.0F) {
        return false;
    }

    if (humidity < 0.0F || humidity > 100.0F) {
        return false;
    }

    // Проверяем, что тип почвы валиден
    switch (soilType) {
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

SoilParameters SensorCompensationService::getSoilParameters(SoilType soilType) const {
    auto it = soilParameters.find(soilType);
    if (it != soilParameters.end()) {
        return it->second;
    }
    return SoilParameters(); // Возвращаем значения по умолчанию
}

ArchieCoefficients SensorCompensationService::getArchieCoefficients(SoilType soilType) const {
    auto it = archieCoefficients.find(soilType);
    if (it != archieCoefficients.end()) {
        return it->second;
    }
    return ArchieCoefficients(); // Возвращаем значения по умолчанию
}

void SensorCompensationService::initializeArchieCoefficients() {
    // Коэффициенты Арчи для разных типов почвы
    // Источник: Archie, G.E. (1942). The electrical resistivity log as an aid in determining some reservoir characteristics.

    archieCoefficients[SoilType::SAND] = ArchieCoefficients(1.3F, 2.0F, 0.35F);
    archieCoefficients[SoilType::LOAM] = ArchieCoefficients(1.5F, 2.0F, 0.45F);
    archieCoefficients[SoilType::PEAT] = ArchieCoefficients(1.8F, 2.2F, 0.80F);
    archieCoefficients[SoilType::CLAY] = ArchieCoefficients(2.0F, 2.5F, 0.50F);
    archieCoefficients[SoilType::SANDPEAT] = ArchieCoefficients(1.6F, 2.1F, 0.60F);

    logDebugSafe("SensorCompensationService: Коэффициенты Арчи инициализированы");
}

void SensorCompensationService::initializeSoilParameters() {
    // Параметры почвы для разных типов
    // Источник: FAO 56 - Crop evapotranspiration

    soilParameters[SoilType::SAND] = SoilParameters(0.35F, 1.60F, 0.10F);
    soilParameters[SoilType::LOAM] = SoilParameters(0.45F, 1.40F, 0.20F);
    soilParameters[SoilType::PEAT] = SoilParameters(0.80F, 0.30F, 0.45F);
    soilParameters[SoilType::CLAY] = SoilParameters(0.50F, 1.20F, 0.35F);
    soilParameters[SoilType::SANDPEAT] = SoilParameters(0.60F, 0.80F, 0.30F);

    logDebugSafe("SensorCompensationService: Параметры почвы инициализированы");
}

float SensorCompensationService::temperatureToKelvin(float celsius) const {
    return celsius + 273.15F;
}

float SensorCompensationService::calculateECTemperatureFactor(float temperature) const {
    // Температурная компенсация для EC
    // При 25°C коэффициент = 1.0
    return 1.0F + 0.02F * (temperature - 25.0F);
}

float SensorCompensationService::calculateECHumidityFactor(float humidity, SoilType soilType) const {
    // Влажностная компенсация для EC
    // Нормализация к полевой влагоемкости
    SoilParameters params = getSoilParameters(soilType);
    float fieldCapacityPercent = params.fieldCapacity * 100.0F;

    return 1.0F + 0.01F * (humidity - fieldCapacityPercent);
}
