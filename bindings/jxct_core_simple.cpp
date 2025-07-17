/**
 * @file jxct_core_simple.cpp
 * @brief Упрощённые Python-обёртки для JXCT модулей
 * @details Использует Python C API без pybind11
 */

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <vector>
#include <string>
#include <map>

// Заглушки для desktop-сборки
#include "desktop_stubs.h"

// Включаем заголовки наших модулей
#include "validation_utils.h"
#include "business/crop_recommendation_engine.h"
#include "business/sensor_calibration_service.h"
#include "business/sensor_compensation_service.h"
#include "advanced_filters.h"
#include "calibration_manager.h"
#include "sensor_compensation.h"

// Глобальные переменные для хранения данных
static PyObject* g_sensor_data = nullptr;
static PyObject* g_validation_result = nullptr;

// Функция валидации данных сенсора
static PyObject* py_validate_sensor_data(PyObject* self, PyObject* args) {
    PyObject* data_dict;
    
    if (!PyArg_ParseTuple(args, "O", &data_dict)) {
        return NULL;
    }
    
    if (!PyDict_Check(data_dict)) {
        PyErr_SetString(PyExc_TypeError, "Expected dictionary");
        return NULL;
    }
    
    // Извлекаем данные из словаря
    PyObject* temp_obj = PyDict_GetItemString(data_dict, "temperature");
    PyObject* humidity_obj = PyDict_GetItemString(data_dict, "humidity");
    PyObject* ph_obj = PyDict_GetItemString(data_dict, "ph");
    PyObject* ec_obj = PyDict_GetItemString(data_dict, "ec");
    
    if (!temp_obj || !humidity_obj || !ph_obj || !ec_obj) {
        PyErr_SetString(PyExc_ValueError, "Missing required fields");
        return NULL;
    }
    
    float temperature = (float)PyFloat_AsDouble(temp_obj);
    float humidity = (float)PyFloat_AsDouble(humidity_obj);
    float ph = (float)PyFloat_AsDouble(ph_obj);
    float ec = (float)PyFloat_AsDouble(ec_obj);
    
    // Создаём C++ структуру данных
    SensorData cpp_data;
    cpp_data.temperature = temperature;
    cpp_data.humidity = humidity;
    cpp_data.ph = ph;
    cpp_data.ec = ec;
    cpp_data.nitrogen = 45.0;  // Значения по умолчанию
    cpp_data.phosphorus = 30.0;
    cpp_data.potassium = 25.0;
    cpp_data.valid = true;
    cpp_data.timestamp = 0;
    
    // Вызываем C++ функцию валидации
    auto result = validateFullSensorData(cpp_data);
    
    // Создаём результат для Python
    PyObject* result_dict = PyDict_New();
    PyDict_SetItemString(result_dict, "isValid", result.isValid ? Py_True : Py_False);
    
    // Добавляем ошибки
    PyObject* errors_list = PyList_New(0);
    for (const auto& error : result.errors) {
        PyList_Append(errors_list, PyUnicode_FromString(error.c_str()));
    }
    PyDict_SetItemString(result_dict, "errors", errors_list);
    
    // Добавляем предупреждения
    PyObject* warnings_list = PyList_New(0);
    for (const auto& warning : result.warnings) {
        PyList_Append(warnings_list, PyUnicode_FromString(warning.c_str()));
    }
    PyDict_SetItemString(result_dict, "warnings", warnings_list);
    
    return result_dict;
}

// Функция применения компенсации
static PyObject* py_apply_compensation(PyObject* self, PyObject* args) {
    PyObject* data_dict;
    int soil_type;
    
    if (!PyArg_ParseTuple(args, "Oi", &data_dict, &soil_type)) {
        return NULL;
    }
    
    if (!PyDict_Check(data_dict)) {
        PyErr_SetString(PyExc_TypeError, "Expected dictionary");
        return NULL;
    }
    
    // Извлекаем данные
    PyObject* temp_obj = PyDict_GetItemString(data_dict, "temperature");
    PyObject* humidity_obj = PyDict_GetItemString(data_dict, "humidity");
    PyObject* ph_obj = PyDict_GetItemString(data_dict, "ph");
    PyObject* ec_obj = PyDict_GetItemString(data_dict, "ec");
    
    float temperature = (float)PyFloat_AsDouble(temp_obj);
    float humidity = (float)PyFloat_AsDouble(humidity_obj);
    float ph = (float)PyFloat_AsDouble(ph_obj);
    float ec = (float)PyFloat_AsDouble(ec_obj);
    
    // Применяем компенсацию
    SoilType soil = static_cast<SoilType>(soil_type);
    float compensated_ec = correctEC(ec, soil, temperature, humidity);
    float compensated_ph = correctPH(temperature, ph);
    
    // Создаём результат
    PyObject* result_dict = PyDict_New();
    PyDict_SetItemString(result_dict, "temperature", PyFloat_FromDouble(temperature));
    PyDict_SetItemString(result_dict, "humidity", PyFloat_FromDouble(humidity));
    PyDict_SetItemString(result_dict, "ph", PyFloat_FromDouble(compensated_ph));
    PyDict_SetItemString(result_dict, "ec", PyFloat_FromDouble(compensated_ec));
    
    return result_dict;
}

// Функция получения рекомендаций
static PyObject* py_get_recommendations(PyObject* self, PyObject* args) {
    PyObject* data_dict;
    const char* crop_id;
    int soil_profile;
    int environment_type;
    
    if (!PyArg_ParseTuple(args, "Osii", &data_dict, &crop_id, &soil_profile, &environment_type)) {
        return NULL;
    }
    
    // Создаём движок рекомендаций
    CropRecommendationEngine engine;
    
    // Преобразуем типы
    SoilProfile soil = static_cast<SoilProfile>(soil_profile);
    EnvironmentType env = static_cast<EnvironmentType>(environment_type);
    
    // Получаем рекомендации (упрощённо)
    PyObject* result_dict = PyDict_New();
    PyDict_SetItemString(result_dict, "ph_action", PyUnicode_FromString("maintain"));
    PyDict_SetItemString(result_dict, "ph_value", PyFloat_FromDouble(6.5));
    PyDict_SetItemString(result_dict, "ec_action", PyUnicode_FromString("maintain"));
    PyDict_SetItemString(result_dict, "ec_value", PyFloat_FromDouble(1500.0));
    PyDict_SetItemString(result_dict, "watering", PyUnicode_FromString("maintain"));
    
    // NPK рекомендации
    PyObject* npk_dict = PyDict_New();
    PyDict_SetItemString(npk_dict, "nitrogen", PyUnicode_FromString("maintain"));
    PyDict_SetItemString(npk_dict, "phosphorus", PyUnicode_FromString("maintain"));
    PyDict_SetItemString(npk_dict, "potassium", PyUnicode_FromString("maintain"));
    PyDict_SetItemString(result_dict, "npk_recommendations", npk_dict);
    
    return result_dict;
}

// Методы модуля
static PyMethodDef JxctMethods[] = {
    {"validate_sensor_data", py_validate_sensor_data, METH_VARARGS, "Валидирует данные сенсора"},
    {"apply_compensation", py_apply_compensation, METH_VARARGS, "Применяет компенсацию к данным сенсора"},
    {"get_recommendations", py_get_recommendations, METH_VARARGS, "Получает агрономические рекомендации"},
    {NULL, NULL, 0, NULL}  // Sentinel
};

// Определение модуля
static struct PyModuleDef jxctmodule = {
    PyModuleDef_HEAD_INIT,
    "jxct_core",
    "JXCT Core Python Bindings",
    -1,
    JxctMethods
};

// Инициализация модуля
PyMODINIT_FUNC PyInit_jxct_core(void) {
    return PyModule_Create(&jxctmodule);
} 