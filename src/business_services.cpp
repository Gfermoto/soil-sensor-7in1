/**
 * @file business_services.cpp
 * @brief Реализация доступа к бизнес-сервисам
 */

#include "business_services.h"
#include "business/crop_recommendation_engine.h"
#include "business/sensor_calibration_service.h"
#include "business/sensor_compensation_service.h"

// Глобальные экземпляры бизнес-сервисов (объявлены в main.cpp)
extern CropRecommendationEngine gCropEngine;
extern SensorCalibrationService gCalibrationService;
extern SensorCompensationService gCompensationService;

ICropRecommendationEngine& getCropEngine() {
    return gCropEngine;
}

ISensorCalibrationService& getCalibrationService() {
    return gCalibrationService;
}

ISensorCompensationService& getCompensationService() {
    return gCompensationService;
} 