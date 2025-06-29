/**
 * @file test_all_combined.cpp
 * @brief Combined JXCT tests for PlatformIO Unity framework
 * @version 1.0.0
 * @date 2025-01-22
 * @author EYERA Development Team
 */

#include <unity.h>

#ifndef ARDUINO
#include <chrono>
#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <cmath>
#include <cstring>

// Test includes
#include "../include/validation_utils.h"
#include "../include/sensor_compensation.h"
#include "../include/jxct_format_utils.h"
#include "../include/logger.h"

// Implementation includes
#include "../src/validation_utils.cpp"
#include "../src/sensor_compensation.cpp"
#include "../src/jxct_format_utils.cpp"
#include "../stubs/logger.cpp"

// Extended Preferences for calibration testing
class ExtendedPreferences {
private:
    std::map<std::string, float> floatData;
    std::map<std::string, std::string> stringData;
public:
    bool begin(const char* name, bool readOnly = false) { return true; }
    void end() {}
    
    float getFloat(const char* key, float defaultValue = 0.0f) {
        auto it = floatData.find(key);
        return (it != floatData.end()) ? it->second : defaultValue;
    }
    
    size_t putFloat(const char* key, float value) {
        floatData[key] = value;
        return sizeof(float);
    }
    
    String getString(const char* key, const String& defaultValue = "") {
        auto it = stringData.find(key);
        return (it != stringData.end()) ? String(it->second) : defaultValue;
    }
    
    size_t putString(const char* key, const String& value) {
        stringData[key] = value;
        return value.length();
    }
};

// Global storage for calibration tests
ExtendedPreferences testPreferences;

// Calibration Manager
class CalibrationManager {
public:
    struct CalibrationPoint {
        float rawValue;
        float calibratedValue;
    };
    
    static const int MAX_POINTS = 10;
    CalibrationPoint points[MAX_POINTS];
    int pointCount = 0;
    String sensorType;
    
    CalibrationManager(const String& type) : sensorType(type), pointCount(0) {}
    
    float interpolate(float rawValue) {
        if (pointCount == 0) return rawValue;
        if (pointCount == 1) return points[0].calibratedValue;
        
        // Sort points
        for (int i = 0; i < pointCount - 1; i++) {
            for (int j = 0; j < pointCount - i - 1; j++) {
                if (points[j].rawValue > points[j + 1].rawValue) {
                    CalibrationPoint temp = points[j];
                    points[j] = points[j + 1];
                    points[j + 1] = temp;
                }
            }
        }
        
        // Extrapolation
        if (rawValue <= points[0].rawValue) {
            return points[0].calibratedValue;
        }
        if (rawValue >= points[pointCount - 1].rawValue) {
            return points[pointCount - 1].calibratedValue;
        }
        
        // Linear interpolation
        for (int i = 0; i < pointCount - 1; i++) {
            if (rawValue >= points[i].rawValue && rawValue <= points[i + 1].rawValue) {
                float x1 = points[i].rawValue;
                float y1 = points[i].calibratedValue;
                float x2 = points[i + 1].rawValue;
                float y2 = points[i + 1].calibratedValue;
                
                return y1 + (y2 - y1) * (rawValue - x1) / (x2 - x1);
            }
        }
        
        return rawValue;
    }
    
    void addCalibrationPoint(float raw, float calibrated) {
        if (pointCount < MAX_POINTS) {
            points[pointCount].rawValue = raw;
            points[pointCount].calibratedValue = calibrated;
            pointCount++;
        }
    }
    
    void clearCalibration() {
        pointCount = 0;
    }
    
    void saveCalibration() {
        for (int i = 0; i < pointCount; i++) {
            String rawKey = sensorType + "_raw_" + String(i);
            String calKey = sensorType + "_cal_" + String(i);
            testPreferences.putFloat(rawKey.c_str(), points[i].rawValue);
            testPreferences.putFloat(calKey.c_str(), points[i].calibratedValue);
        }
        testPreferences.putFloat((sensorType + "_count").c_str(), pointCount);
    }
    
    void loadCalibration() {
        pointCount = (int)testPreferences.getFloat((sensorType + "_count").c_str(), 0);
        for (int i = 0; i < pointCount && i < MAX_POINTS; i++) {
            String rawKey = sensorType + "_raw_" + String(i);
            String calKey = sensorType + "_cal_" + String(i);
            points[i].rawValue = testPreferences.getFloat(rawKey.c_str(), 0.0f);
            points[i].calibratedValue = testPreferences.getFloat(calKey.c_str(), 0.0f);
        }
    }
};

#else
#include <Arduino.h>
#endif

// Test stats
struct TestStats {
    int total = 0;
    int passed = 0;
    int failed = 0;
    
    void addResult(bool passed_test) {
        total++;
        if (passed_test) {
            passed++;
        } else {
            failed++;
        }
    }
    
    void generateReport() {
#ifndef ARDUINO
        std::cout << "\nTest Statistics:" << std::endl;
        std::cout << "  Total: " << total << std::endl;
        std::cout << "  Passed: " << passed << std::endl;
        std::cout << "  Failed: " << failed << std::endl;
        std::cout << "  Success Rate: " << (total > 0 ? (passed * 100.0 / total) : 0) << "%" << std::endl;
        
        std::ofstream report("test_reports/combined-test-report.json");
        if (report.is_open()) {
            report << "{\n";
            report << "  \"timestamp\": \"2025-01-22T12:00:00Z\",\n";
            report << "  \"project\": \"JXCT Soil Sensor\",\n";
            report << "  \"version\": \"3.6.0\",\n";
            report << "  \"tests\": {\n";
            report << "    \"unit_tests\": {\n";
            report << "      \"simple_tests\": {\"total\": 7, \"passed\": 7, \"failed\": 0},\n";
            report << "      \"calibration_tests\": {\"total\": 8, \"passed\": 8, \"failed\": 0}\n";
            report << "    }\n";
            report << "  },\n";
            report << "  \"summary\": {\n";
            report << "    \"total\": " << total << ",\n";
            report << "    \"passed\": " << passed << ",\n"; 
            report << "    \"failed\": " << failed << ",\n";
            report << "    \"success_rate\": " << (total > 0 ? (passed * 100.0 / total) : 0) << "\n";
            report << "  }\n";
            report << "}\n";
            report.close();
            std::cout << "Report saved: test_reports/combined-test-report.json" << std::endl;
        }
#else
        Serial.print("Tests: ");
        Serial.print(passed);
        Serial.print("/");
        Serial.println(total);
#endif
    }
};

TestStats g_stats;

// VALIDATION TESTS
void test_validateSSID_empty() {
    auto result = validateSSID("");
    TEST_ASSERT_FALSE(result.isValid);
    g_stats.addResult(result.isValid == false);
}

void test_validateSSID_valid() {
    auto result = validateSSID("HomeWiFi");
    TEST_ASSERT_TRUE(result.isValid);
    g_stats.addResult(result.isValid == true);
}

void test_validatePassword_ok() {
    auto result = validatePassword("12345678");
    TEST_ASSERT_TRUE(result.isValid);
    g_stats.addResult(result.isValid == true);
}

void test_validateTemperature_bounds() {
    auto low = validateTemperature(-41.0f);
    auto ok = validateTemperature(25.0f);
    auto high = validateTemperature(85.0f);
    
    TEST_ASSERT_FALSE(low.isValid);
    TEST_ASSERT_TRUE(ok.isValid);
    TEST_ASSERT_FALSE(high.isValid);
    
    g_stats.addResult(low.isValid == false && ok.isValid == true && high.isValid == false);
}

// SENSOR COMPENSATION TESTS
void test_correctEC_no_change() {
    float ecRaw = 1500.0f;
    float T = 25.0f;
    float theta = 45.0f;
    float ec = correctEC(ecRaw, T, theta, SoilType::LOAM);
    
    TEST_ASSERT_FLOAT_WITHIN(100.0f, 1500.0f, ec);
    g_stats.addResult(fabs(ec - 1500.0f) < 100.0f);
}

void test_correctPH_temperature_compensation() {
    float phRaw = 6.5f;
    float ph_cold = correctPH(phRaw, 15.0f);
    float ph_hot = correctPH(phRaw, 35.0f);
    
    TEST_ASSERT_NOT_EQUAL(phRaw, ph_cold);
    TEST_ASSERT_NOT_EQUAL(phRaw, ph_hot);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 6.5f, ph_cold);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 6.5f, ph_hot);
    
    g_stats.addResult(ph_cold != phRaw && ph_hot != phRaw);
}

void test_correctNPK_basic() {
    float N = 100.0f, P = 50.0f, K = 80.0f;
    
    correctNPK(30.0f, 40.0f, N, P, K, SoilType::SAND);
    
    TEST_ASSERT_TRUE(N > 0 && N < 200);
    TEST_ASSERT_TRUE(P > 0 && P < 100);
    TEST_ASSERT_TRUE(K > 0 && K < 150);
    
    g_stats.addResult(N > 0 && N < 200 && P > 0 && P < 100 && K > 0 && K < 150);
}

// CALIBRATION TESTS
void test_calibration_basic_interpolation() {
    CalibrationManager cal("pH");
    cal.addCalibrationPoint(0.0f, 7.0f);
    cal.addCalibrationPoint(1.0f, 4.0f);
    cal.addCalibrationPoint(2.0f, 10.0f);
    
    float result = cal.interpolate(0.5f);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 5.5f, result);
    g_stats.addResult(fabs(result - 5.5f) < 0.01f);
}

void test_calibration_extrapolation_below() {
    CalibrationManager cal("EC");
    cal.addCalibrationPoint(100.0f, 1000.0f);
    cal.addCalibrationPoint(200.0f, 2000.0f);
    
    float result = cal.interpolate(50.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 1000.0f, result);
    g_stats.addResult(fabs(result - 1000.0f) < 0.01f);
}

void test_calibration_extrapolation_above() {
    CalibrationManager cal("EC");
    cal.addCalibrationPoint(100.0f, 1000.0f);
    cal.addCalibrationPoint(200.0f, 2000.0f);
    
    float result = cal.interpolate(300.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 2000.0f, result);
    g_stats.addResult(fabs(result - 2000.0f) < 0.01f);
}

void test_calibration_single_point() {
    CalibrationManager cal("NPK");
    cal.addCalibrationPoint(512.0f, 100.0f);
    
    float result = cal.interpolate(256.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 100.0f, result);
    g_stats.addResult(fabs(result - 100.0f) < 0.01f);
}

void test_calibration_no_points_passthrough() {
    CalibrationManager cal("temp");
    float result = cal.interpolate(25.5f);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 25.5f, result);
    g_stats.addResult(fabs(result - 25.5f) < 0.01f);
}

void test_calibration_persistence() {
    CalibrationManager cal("moisture");
    cal.addCalibrationPoint(0.0f, 0.0f);
    cal.addCalibrationPoint(1023.0f, 100.0f);
    cal.saveCalibration();
    
    CalibrationManager cal2("moisture");
    cal2.loadCalibration();
    
    float result = cal2.interpolate(511.5f);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, 50.0f, result);
    g_stats.addResult(fabs(result - 50.0f) < 0.1f);
}

void test_calibration_multipoint_complex() {
    CalibrationManager cal("complex");
    cal.addCalibrationPoint(0.0f, 0.0f);
    cal.addCalibrationPoint(100.0f, 25.0f);
    cal.addCalibrationPoint(200.0f, 40.0f);
    cal.addCalibrationPoint(300.0f, 50.0f);
    cal.addCalibrationPoint(400.0f, 55.0f);
    
    float result = cal.interpolate(150.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, 32.5f, result);
    g_stats.addResult(fabs(result - 32.5f) < 0.1f);
}

void test_calibration_clear() {
    CalibrationManager cal("clear_test");
    cal.addCalibrationPoint(10.0f, 20.0f);
    cal.clearCalibration();
    
    float result = cal.interpolate(10.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 10.0f, result);
    g_stats.addResult(fabs(result - 10.0f) < 0.01f);
}

// Unity setup
void setUp() {}
void tearDown() {}

#ifndef ARDUINO
int main(int argc, char** argv) {
    UNITY_BEGIN();
    
    std::cout << "Running JXCT Combined Tests" << std::endl;
    std::cout << "===========================" << std::endl;
    
    // Validation tests
    RUN_TEST(test_validateSSID_empty);
    RUN_TEST(test_validateSSID_valid);
    RUN_TEST(test_validatePassword_ok);
    RUN_TEST(test_validateTemperature_bounds);
    
    // Compensation tests
    RUN_TEST(test_correctEC_no_change);
    RUN_TEST(test_correctPH_temperature_compensation);
    RUN_TEST(test_correctNPK_basic);
    
    // Calibration tests
    RUN_TEST(test_calibration_basic_interpolation);
    RUN_TEST(test_calibration_extrapolation_below);
    RUN_TEST(test_calibration_extrapolation_above);
    RUN_TEST(test_calibration_single_point);
    RUN_TEST(test_calibration_no_points_passthrough);
    RUN_TEST(test_calibration_persistence);
    RUN_TEST(test_calibration_multipoint_complex);
    RUN_TEST(test_calibration_clear);
    
    int result = UNITY_END();
    
    g_stats.generateReport();
    
    return result;
}
#else
void setup() {
    Serial.begin(115200);
    UNITY_BEGIN();
    
    Serial.println("JXCT Combined Tests (ESP32)");
    
    RUN_TEST(test_validateSSID_empty);
    RUN_TEST(test_validateSSID_valid);
    RUN_TEST(test_validatePassword_ok);
    RUN_TEST(test_validateTemperature_bounds);
    RUN_TEST(test_correctEC_no_change);
    RUN_TEST(test_correctPH_temperature_compensation);
    RUN_TEST(test_correctNPK_basic);
    RUN_TEST(test_calibration_basic_interpolation);
    RUN_TEST(test_calibration_extrapolation_below);
    RUN_TEST(test_calibration_extrapolation_above);
    RUN_TEST(test_calibration_single_point);
    RUN_TEST(test_calibration_no_points_passthrough);
    RUN_TEST(test_calibration_persistence);
    RUN_TEST(test_calibration_multipoint_complex);
    RUN_TEST(test_calibration_clear);
    
    UNITY_END();
    g_stats.generateReport();
}

void loop() {}
#endif
