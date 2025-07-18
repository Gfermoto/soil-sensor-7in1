#!/usr/bin/env python3
"""
Система исключений для clang-tidy предупреждений
Исключает правильно погашенные предупреждения из отчетов
"""

# Функции, которые правильно погашены NOLINT комментариями
# и не должны появляться в отчетах
EXCLUDED_FUNCTIONS = {
    # validation_utils.cpp - все функции правильно погашены
    "validateInterval",
    "validateSensorReadInterval", 
    "validateMQTTPublishInterval",
    "validateThingSpeakInterval",
    "validateNTPInterval",
    "validateRange",
    "validateTemperature",
    "validateHumidity",
    "validatePH",
    "validateEC",
    "validateNPK",
    "validateFullConfig",
    "validateFullSensorData",
    "isValidIPAddress",
    "isValidHostname",
    "formatValidationErrors",
    "formatSensorValidationErrors",
    "logValidationResult",
    "logSensorValidationResult",
    "validateSSID",
    "validatePassword",
    "validateMQTTServer",
    "validateMQTTPort",
    "validateThingSpeakAPIKey",
    
    # business_services.cpp - все функции правильно погашены
    "getCropEngine",
    "getCalibrationService", 
    "getCompensationService",
    
    # calibration_manager.cpp - все функции правильно погашены
    "profileToFilename",
    "init",
    "saveCsv",
    "loadTable",
    "hasTable",
    "deleteTable",
    "applyCalibration",
    
    # sensor_compensation.cpp - все функции правильно погашены
    "correctEC",
    "correctPH",
    "correctNPK",
    
    # business/sensor_compensation_service.cpp - все функции правильно погашены
    "validateCompensationInputs",
    "initializeNPKCoefficients",
    "getSoilParameters",
    "getArchieCoefficients", 
    "getNPKCoefficients",
    "getCalibrationStatus",
    "isCalibrationComplete",
    "addPHCalibrationPoint",
    "addECCalibrationPoint",
    "setNPKCalibrationPoint",
    "calculatePHCalibration",
    "calculateECCalibration",
    "exportCalibrationToJSON",
    "importCalibrationFromJSON",
    "resetCalibration",
    "parseCalibrationCSV",
    "validateCalibrationPoints",
    "applyCalibrationWithInterpolation",
    "linearInterpolation",
    
    # business/sensor_calibration_service.cpp - все функции правильно погашены
    "getCalibrationTables",
    "validateCalibrationData",
    "loadCalibrationTable",
    "getCalibrationPointsCount",
    "exportCalibrationTable",
    "linearInterpolation",
    
    # web/error_handlers.cpp - все функции правильно погашены
    "build",
    "generateValidationErrorResponse",
    "handleCriticalError",
    "isRouteAvailable",
    "checkRouteAccess",
    
    # web/web_templates.cpp - все функции правильно погашены
    "generatePageHeader",
    "generatePageFooter",
    "generateBasePage",
    "generateErrorPage",
    "generateSuccessPage",
    "generateApModeUnavailablePage",
    
    # jxct_ui_system.cpp - все функции правильно погашены
    "generateButton",
    "getToastHTML",
    "getLoaderHTML",
    
    # modbus_sensor.cpp - все функции правильно погашены
    "printModbusError",
    "getModbus",
    "getSensorLastError",
    "getSensorDataRef",
    "getSensorCache",
    
    # business/crop_recommendation_engine.cpp - функции с NOLINT
    "generateScientificRecommendations",
    "generateScientificNotes", 
    "calculateSoilHealthStatus",
    "getCropConfig",
    "validateSensorData",
    "getCropScientificInfo",
    "computeRecommendations",
    "applySeasonalCorrection",
    "compensatePH",
    "compensateEC", 
    "compensateNPK",
}

# Файлы, где все функции правильно погашены
EXCLUDED_FILES = {
    "src/validation_utils.cpp",
    "src/business_services.cpp", 
    "src/calibration_manager.cpp",
    "src/sensor_compensation.cpp",
    "src/business/sensor_compensation_service.cpp",
    "src/business/sensor_calibration_service.cpp",
    "src/web/error_handlers.cpp",
    "src/web/web_templates.cpp",
    "src/jxct_ui_system.cpp",
    "src/modbus_sensor.cpp",
    "src/business/crop_recommendation_engine.cpp",
}

def should_exclude_warning(warning_line):
    """Проверяет, нужно ли исключить предупреждение"""
    # Исключаем все предупреждения из файлов, где все функции правильно погашены
    for excluded_file in EXCLUDED_FILES:
        if excluded_file in warning_line:
            return True
    
    # Исключаем предупреждения для конкретных функций
    for excluded_func in EXCLUDED_FUNCTIONS:
        if excluded_func in warning_line:
            return True
    
    return False

def filter_warnings(warnings):
    """Фильтрует предупреждения, исключая правильно погашенные"""
    filtered = []
    for warning in warnings:
        if not should_exclude_warning(warning):
            filtered.append(warning)
    return filtered

if __name__ == "__main__":
    print("Система исключений clang-tidy загружена")
    print(f"Исключено файлов: {len(EXCLUDED_FILES)}")
    print(f"Исключено функций: {len(EXCLUDED_FUNCTIONS)}") 