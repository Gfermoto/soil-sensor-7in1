/**
 * @file business_instances.cpp
 * @brief Определения глобальных экземпляров бизнес-сервисов
 */

#include "business/crop_recommendation_engine.h"
#include "../include/sensor_calibration_service.h"  // Единый сервис калибровки
#include "business/sensor_compensation_service.h"

// Глобальные экземпляры бизнес-сервисов
CropRecommendationEngine gCropEngine;            // NOLINT(misc-use-internal-linkage)
// SensorCalibrationService gCalibrationService;    // Убрано - объявлен в unified_calibration_service.cpp
SensorCompensationService gCompensationService;  // NOLINT(misc-use-internal-linkage)
