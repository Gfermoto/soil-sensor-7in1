/**
 * @file business_services.h
 * @brief Доступ к бизнес-сервисам
 * @details Предоставляет глобальные экземпляры бизнес-сервисов для использования в других модулях
 */

#ifndef BUSINESS_SERVICES_H
#define BUSINESS_SERVICES_H

#include "business/ICropRecommendationEngine.h"
#include "business/ISensorCalibrationService.h"
#include "business/ISensorCompensationService.h"

// Глобальные экземпляры бизнес-сервисов
extern ICropRecommendationEngine& getCropEngine();
extern ISensorCalibrationService& getCalibrationService();
extern ISensorCompensationService& getCompensationService();

#endif  // BUSINESS_SERVICES_H
