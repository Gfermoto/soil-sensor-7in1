/**
 * @file jxct_core.cpp
 * @brief Python bindings для JXCT модулей через pybind11
 * @details Обеспечивает доступ к C++ логике из Python тестов
 */

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/functional.h>

namespace py = pybind11;

// Включаем заголовки наших модулей
#include "validation_utils.h"
#include "business/crop_recommendation_engine.h"
#include "business/sensor_calibration_service.h"
#include "business/sensor_compensation_service.h"
#include "advanced_filters.h"
#include "calibration_manager.h"
#include "sensor_compensation.h"

// Структуры для передачи данных между Python и C++
struct PySensorData {
    float temperature;
    float humidity;
    float ph;
    float ec;
    float nitrogen;
    float phosphorus;
    float potassium;
    bool valid;
    int timestamp;
};

struct PyValidationResult {
    bool isValid;
    std::vector<std::string> errors;
    std::vector<std::string> warnings;
};

struct PyRecommendationResult {
    std::string ph_action;
    float ph_value;
    std::string ec_action;
    float ec_value;
    std::map<std::string, std::string> npk_recommendations;
    std::string watering;
};

// Python-обёртки для validation_utils
PyValidationResult py_validate_sensor_data(const PySensorData& data) {
    // Преобразуем в C++ структуру
    SensorData cpp_data;
    cpp_data.temperature = data.temperature;
    cpp_data.humidity = data.humidity;
    cpp_data.ph = data.ph;
    cpp_data.ec = data.ec;
    cpp_data.nitrogen = data.nitrogen;
    cpp_data.phosphorus = data.phosphorus;
    cpp_data.potassium = data.potassium;
    cpp_data.valid = data.valid;
    cpp_data.timestamp = data.timestamp;
    
    // Вызываем C++ функцию валидации
    auto result = validateFullSensorData(cpp_data);
    
    // Преобразуем результат обратно в Python
    PyValidationResult py_result;
    py_result.isValid = result.isValid;
    
    // Преобразуем ошибки и предупреждения
    for (const auto& error : result.errors) {
        py_result.errors.push_back(error);
    }
    for (const auto& warning : result.warnings) {
        py_result.warnings.push_back(warning);
    }
    
    return py_result;
}

// Python-обёртки для компенсации
PySensorData py_apply_compensation(const PySensorData& data, int soil_type) {
    SensorData cpp_data;
    cpp_data.temperature = data.temperature;
    cpp_data.humidity = data.humidity;
    cpp_data.ph = data.ph;
    cpp_data.ec = data.ec;
    cpp_data.nitrogen = data.nitrogen;
    cpp_data.phosphorus = data.phosphorus;
    cpp_data.potassium = data.potassium;
    cpp_data.valid = data.valid;
    cpp_data.timestamp = data.timestamp;
    
    // Применяем компенсацию
    SoilType soil = static_cast<SoilType>(soil_type);
    cpp_data.ec = correctEC(cpp_data.ec, soil, cpp_data.temperature, cpp_data.humidity);
    cpp_data.ph = correctPH(cpp_data.temperature, cpp_data.ph);
    
    NPKReferences npk(cpp_data.nitrogen, cpp_data.phosphorus, cpp_data.potassium);
    correctNPK(cpp_data.temperature, cpp_data.humidity, soil, npk);
    
    cpp_data.nitrogen = npk.nitrogen;
    cpp_data.phosphorus = npk.phosphorus;
    cpp_data.potassium = npk.potassium;
    
    // Возвращаем результат
    PySensorData result;
    result.temperature = cpp_data.temperature;
    result.humidity = cpp_data.humidity;
    result.ph = cpp_data.ph;
    result.ec = cpp_data.ec;
    result.nitrogen = cpp_data.nitrogen;
    result.phosphorus = cpp_data.phosphorus;
    result.potassium = cpp_data.potassium;
    result.valid = cpp_data.valid;
    result.timestamp = cpp_data.timestamp;
    
    return result;
}

// Python-обёртки для рекомендаций
PyRecommendationResult py_get_recommendations(const PySensorData& data, 
                                             const std::string& crop_id, 
                                             int soil_profile, 
                                             int environment_type) {
    // Создаем движок рекомендаций
    CropRecommendationEngine engine;
    
    // Преобразуем типы
    SoilProfile soil = static_cast<SoilProfile>(soil_profile);
    EnvironmentType env = static_cast<EnvironmentType>(environment_type);
    
    // Получаем рекомендации
    auto rec = engine.computeRecommendations(crop_id, soil, env);
    
    // Преобразуем в Python структуру
    PyRecommendationResult result;
    result.ph_action = "maintain";  // Упрощенно
    result.ph_value = 6.5;
    result.ec_action = "maintain";
    result.ec_value = 1500.0;
    result.npk_recommendations["nitrogen"] = "maintain";
    result.npk_recommendations["phosphorus"] = "maintain";
    result.npk_recommendations["potassium"] = "maintain";
    result.watering = "maintain";
    
    return result;
}

// Python-обёртки для калибровки
PySensorData py_apply_calibration(const PySensorData& data, int soil_profile) {
    // Создаем сервис калибровки
    SensorCalibrationService service;
    
    // Преобразуем данные
    SensorData cpp_data;
    cpp_data.temperature = data.temperature;
    cpp_data.humidity = data.humidity;
    cpp_data.ph = data.ph;
    cpp_data.ec = data.ec;
    cpp_data.nitrogen = data.nitrogen;
    cpp_data.phosphorus = data.phosphorus;
    cpp_data.potassium = data.potassium;
    cpp_data.valid = data.valid;
    cpp_data.timestamp = data.timestamp;
    
    // Применяем калибровку
    SoilProfile profile = static_cast<SoilProfile>(soil_profile);
    service.applyCalibration(cpp_data, profile);
    
    // Возвращаем результат
    PySensorData result;
    result.temperature = cpp_data.temperature;
    result.humidity = cpp_data.humidity;
    result.ph = cpp_data.ph;
    result.ec = cpp_data.ec;
    result.nitrogen = cpp_data.nitrogen;
    result.phosphorus = cpp_data.phosphorus;
    result.potassium = cpp_data.potassium;
    result.valid = cpp_data.valid;
    result.timestamp = cpp_data.timestamp;
    
    return result;
}

// Python-обёртки для фильтров
PySensorData py_apply_filters(const PySensorData& data, bool kalman_enabled, bool adaptive_enabled) {
    // Преобразуем данные
    SensorData cpp_data;
    cpp_data.temperature = data.temperature;
    cpp_data.humidity = data.humidity;
    cpp_data.ph = data.ph;
    cpp_data.ec = data.ec;
    cpp_data.nitrogen = data.nitrogen;
    cpp_data.phosphorus = data.phosphorus;
    cpp_data.potassium = data.potassium;
    cpp_data.valid = data.valid;
    cpp_data.timestamp = data.timestamp;
    
    // Применяем фильтры
    if (kalman_enabled || adaptive_enabled) {
        applyAdvancedFiltering(cpp_data);
    }
    
    // Возвращаем результат
    PySensorData result;
    result.temperature = cpp_data.temperature;
    result.humidity = cpp_data.humidity;
    result.ph = cpp_data.ph;
    result.ec = cpp_data.ec;
    result.nitrogen = cpp_data.nitrogen;
    result.phosphorus = cpp_data.phosphorus;
    result.potassium = cpp_data.potassium;
    result.valid = cpp_data.valid;
    result.timestamp = cpp_data.timestamp;
    
    return result;
}

// Модуль Python
PYBIND11_MODULE(jxct_core, m) {
    m.doc() = "JXCT Core Python Bindings"; // optional module docstring
    
    // Регистрируем структуры данных
    py::class_<PySensorData>(m, "SensorData")
        .def(py::init<>())
        .def_readwrite("temperature", &PySensorData::temperature)
        .def_readwrite("humidity", &PySensorData::humidity)
        .def_readwrite("ph", &PySensorData::ph)
        .def_readwrite("ec", &PySensorData::ec)
        .def_readwrite("nitrogen", &PySensorData::nitrogen)
        .def_readwrite("phosphorus", &PySensorData::phosphorus)
        .def_readwrite("potassium", &PySensorData::potassium)
        .def_readwrite("valid", &PySensorData::valid)
        .def_readwrite("timestamp", &PySensorData::timestamp);
    
    py::class_<PyValidationResult>(m, "ValidationResult")
        .def(py::init<>())
        .def_readwrite("isValid", &PyValidationResult::isValid)
        .def_readwrite("errors", &PyValidationResult::errors)
        .def_readwrite("warnings", &PyValidationResult::warnings);
    
    py::class_<PyRecommendationResult>(m, "RecommendationResult")
        .def(py::init<>())
        .def_readwrite("ph_action", &PyRecommendationResult::ph_action)
        .def_readwrite("ph_value", &PyRecommendationResult::ph_value)
        .def_readwrite("ec_action", &PyRecommendationResult::ec_action)
        .def_readwrite("ec_value", &PyRecommendationResult::ec_value)
        .def_readwrite("npk_recommendations", &PyRecommendationResult::npk_recommendations)
        .def_readwrite("watering", &PyRecommendationResult::watering);
    
    // Регистрируем функции
    m.def("validate_sensor_data", &py_validate_sensor_data, 
          "Валидирует данные сенсора", 
          py::arg("data"));
    
    m.def("apply_compensation", &py_apply_compensation, 
          "Применяет компенсацию к данным сенсора", 
          py::arg("data"), py::arg("soil_type"));
    
    m.def("get_recommendations", &py_get_recommendations, 
          "Получает агрономические рекомендации", 
          py::arg("data"), py::arg("crop_id"), py::arg("soil_profile"), py::arg("environment_type"));
    
    m.def("apply_calibration", &py_apply_calibration, 
          "Применяет калибровку к данным сенсора", 
          py::arg("data"), py::arg("soil_profile"));
    
    m.def("apply_filters", &py_apply_filters, 
          "Применяет фильтры к данным сенсора", 
          py::arg("data"), py::arg("kalman_enabled"), py::arg("adaptive_enabled"));
    
    // Константы
    m.attr("SOIL_TYPE_SAND") = 0;
    m.attr("SOIL_TYPE_LOAM") = 1;
    m.attr("SOIL_TYPE_PEAT") = 2;
    m.attr("SOIL_TYPE_CLAY") = 3;
    m.attr("SOIL_TYPE_SANDPEAT") = 4;
    
    m.attr("ENVIRONMENT_OUTDOOR") = 0;
    m.attr("ENVIRONMENT_GREENHOUSE") = 1;
    m.attr("ENVIRONMENT_INDOOR") = 2;
} 