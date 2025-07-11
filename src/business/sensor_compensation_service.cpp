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
    initializeNPKCoefficients();
}

void SensorCompensationService::applyCompensation(SensorData& data, SoilType soilType) {
    logDebugSafe("SensorCompensationService: Применение компенсации для типа почвы %d", static_cast<int>(soilType));

    // Применяем компенсацию к каждому параметру
    data.ec = correctEC(data.ec, soilType, data.temperature, data.humidity);
    data.ph = correctPH(data.temperature, data.ph); // ИСПРАВЛЕНО: температура, затем pH

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
    const float tempFactor = calculateECTemperatureFactor(temperature_param);
    
    // Влажностная компенсация по модели Арчи
    const float humidityFactor = calculateECHumidityFactor(humidity_param, soilType_param);
    
    // Применяем модель Арчи: EC = EC0 × (θ/θ0)^m × (T/T0)^n
    float compensatedEC = ec25_param * pow(humidityFactor, coeffs.m) * pow(tempFactor, coeffs.n);
    
    logDebugSafe("SensorCompensationService: EC скорректирован %.2f → %.2f (m=%.2f, n=%.2f, ΔT=%.1f°C)", 
                 ec25_param, compensatedEC, coeffs.m, coeffs.n, temperature_param - 25.0F);
    return compensatedEC;
}

// @param temperature_param - температура почвы (первый параметр)
// @param phRaw_param - исходное значение pH (второй параметр)
float SensorCompensationService::correctPH(float temperature_param, float phRaw_param) // NOLINT(bugprone-easily-swappable-parameters)
{
    if (temperature_param < -50.0F || temperature_param > 100.0F) {
        logDebugSafe("SensorCompensationService: Недопустимая температура для компенсации pH: %.2f", temperature_param);
        return phRaw_param;
    }

    // ИСПРАВЛЕНО: Простая температурная поправка -0.003 pH/°C
    // Это стандартная поправка для большинства pH датчиков
    const float tempCorrection = -0.003F * (temperature_param - 25.0F);
    const float compensatedPH = phRaw_param + tempCorrection;

    logDebugSafe("SensorCompensationService: pH скорректирован %.2f → %.2f (ΔT=%.1f°C)", 
                 phRaw_param, compensatedPH, temperature_param - 25.0F);
    return compensatedPH;
}

void SensorCompensationService::correctNPK(float temperature, float humidity, SoilType soilType, NPKReferences& npk) {
    if (!validateCompensationInputs(soilType, humidity, temperature)) {
        logDebugSafe("SensorCompensationService: Недопустимые входные данные для компенсации NPK");
        return;
    }

    // Получаем коэффициенты NPK для типа почвы
    const NPKCoefficients coeffs = getNPKCoefficients(soilType);
    
    // Температурная компенсация: N_comp = N_raw × e^(δ(T-20))
    const float tempFactorN = exp(coeffs.delta_N * (temperature - 20.0F));
    const float tempFactorP = exp(coeffs.delta_P * (temperature - 20.0F));
    const float tempFactorK = exp(coeffs.delta_K * (temperature - 20.0F));
    
    // Влажностная компенсация: (1 + ε(M-30))
    const float moistureFactorN = 1.0F + (coeffs.epsilon_N * (humidity - 30.0F));
    const float moistureFactorP = 1.0F + (coeffs.epsilon_P * (humidity - 30.0F));
    const float moistureFactorK = 1.0F + (coeffs.epsilon_K * (humidity - 30.0F));
    
    // Применяем полную компенсацию: N_comp = N_raw × e^(δ(T-20)) × (1 + ε(M-30))
    npk.nitrogen *= tempFactorN * moistureFactorN;
    npk.phosphorus *= tempFactorP * moistureFactorP;
    npk.potassium *= tempFactorK * moistureFactorK;

    logDebugSafe("SensorCompensationService: NPK скорректирован N:%.2f P:%.2f K:%.2f (δN=%.4f, εN=%.3f, ΔT=%.1f°C)",
                 npk.nitrogen, npk.phosphorus, npk.potassium, coeffs.delta_N, coeffs.epsilon_N, temperature - 20.0F);
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

void SensorCompensationService::initializeNPKCoefficients() {
    // Коэффициенты NPK для разных типов почвы
    // Источник: [Delgado et al. (2020). DOI:10.1007/s42729-020-00215-4]
    
    npkCoefficients[SoilType::SAND] = NPKCoefficients(0.0041F, 0.0053F, 0.0032F, 0.01F, 0.008F, 0.012F);
    npkCoefficients[SoilType::LOAM] = NPKCoefficients(0.0038F, 0.0049F, 0.0029F, 0.009F, 0.007F, 0.011F);
    npkCoefficients[SoilType::CLAY] = NPKCoefficients(0.0032F, 0.0042F, 0.0024F, 0.008F, 0.006F, 0.010F);
    npkCoefficients[SoilType::PEAT] = NPKCoefficients(0.0028F, 0.0035F, 0.0018F, 0.012F, 0.009F, 0.015F);
    npkCoefficients[SoilType::SANDPEAT] = NPKCoefficients(0.0040F, 0.0051F, 0.0031F, 0.010F, 0.008F, 0.012F);
    
    logDebugSafe("SensorCompensationService: Коэффициенты NPK инициализированы");
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

NPKCoefficients SensorCompensationService::getNPKCoefficients(SoilType soilType) const { // NOLINT(readability-convert-member-functions-to-static)
    auto iter = npkCoefficients.find(soilType);
    if (iter != npkCoefficients.end()) {
        return iter->second;
    }
    return {}; // Возвращаем значения по умолчанию
}

float SensorCompensationService::temperatureToKelvin(float celsius) {
    return celsius + 273.15F;
}

float SensorCompensationService::calculateECTemperatureFactor(float temperature) {
    // Температурная компенсация для EC согласно стандартам (USDA, Hanna, Horiba)
    // EC25 = ECt / [1 + 0.02 × (t - 25)]
    return 1.0F / (1.0F + 0.02F * (temperature - 25.0F));
}

float SensorCompensationService::calculateECHumidityFactor(float humidity, SoilType soilType) const {
    // Влажностная компенсация для EC по модели Арчи
    // Нормализация к полевой влагоемкости
    const SoilParameters params = getSoilParameters(soilType);
    const float fieldCapacityPercent = params.fieldCapacity * 100.0F;

    return 1.0F + (0.01F * (humidity - fieldCapacityPercent));
}
