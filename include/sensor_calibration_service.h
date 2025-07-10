#pragma once
#include <map>
#include "jxct_constants.h"
#include "calibration_manager.h"

class SensorCalibrationService {
public:
    SensorCalibrationService();
    void applyCalibration(SensorData& data, SoilProfile profile);
    float applySingleCalibration(float rawValue, SoilProfile profile);
    bool validateCalibrationData(const SensorData& data);
    bool loadCalibrationTable(const String& csvData, SoilProfile profile);
    bool hasCalibrationTable(SoilProfile profile) const;
    void clearCalibrationTable(SoilProfile profile);
    static size_t getCalibrationPointsCount(SoilProfile profile, const String& sensorType);
    static String exportCalibrationTable(SoilProfile profile);
    static std::map<SoilProfile, CalibrationTable> calibrationTables;
private:
    float applyCalibrationWithInterpolation(float rawValue, const std::vector<CalibrationPoint>& points);
}; 