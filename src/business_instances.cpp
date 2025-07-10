/**
 * @file business_instances.cpp
 * @brief Определения глобальных экземпляров бизнес-сервисов
 */

#include "business/crop_recommendation_engine.h"
#include "business/sensor_calibration_service.h"
#include "business/sensor_compensation_service.h"

// Глобальные экземпляры бизнес-сервисов
CropRecommendationEngine gCropEngine;
SensorCalibrationService gCalibrationService;
SensorCompensationService gCompensationService; 