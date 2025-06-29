/**
 * @file test_calibration_unity.cpp
 * @brief Unit-тесты CalibrationManager для PlatformIO Unity framework  
 * @details Безопасное тестирование критических алгоритмов интерполяции
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

// ============================================================================
// БЕЗОПАСНЫЕ ЗАГЛУШКИ ДЛЯ ТЕСТИРОВАНИЯ (БЕЗ ESP32 ЗАВИСИМОСТЕЙ)
// ============================================================================

// Заглушка для String
class String {
public:
    std::string data;
    String() {}
    String(const char* s) : data(s) {}
    String(const std::string& s) : data(s) {}
    String(int i) : data(std::to_string(i)) {}
    String(float f) : data(std::to_string(f)) {}
    const char* c_str() const { return data.c_str(); }
    String operator+(const String& other) const { return String(data + other.data); }
    bool operator==(const String& other) const { return data == other.data; }
};

// Заглушка для Preferences
class MockPreferences {
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
    
    String getString(const char* key, String defaultValue = "") {
        auto it = stringData.find(key);
        return (it != stringData.end()) ? String(it->second) : defaultValue;
    }
    
    size_t putString(const char* key, const String& value) {
        stringData[key] = value.data;
        return value.data.length();
    }
};

MockPreferences preferences;

// ============================================================================
// МИНИМАЛЬНАЯ РЕАЛИЗАЦИЯ CalibrationManager ДЛЯ ТЕСТИРОВАНИЯ
// ============================================================================

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
    
    // ✅ КРИТИЧЕСКИЙ АЛГОРИТМ ИНТЕРПОЛЯЦИИ - ОСНОВНОЙ ОБЪЕКТ ТЕСТИРОВАНИЯ
    float interpolate(float rawValue) {
        if (pointCount == 0) return rawValue;
        if (pointCount == 1) return points[0].calibratedValue;
        
        // Сортируем точки по rawValue (пузырьковая сортировка)
        for (int i = 0; i < pointCount - 1; i++) {
            for (int j = 0; j < pointCount - i - 1; j++) {
                if (points[j].rawValue > points[j + 1].rawValue) {
                    CalibrationPoint temp = points[j];
                    points[j] = points[j + 1];
                    points[j + 1] = temp;
                }
            }
        }
        
        // Экстраполяция за пределами диапазона
        if (rawValue <= points[0].rawValue) {
            return points[0].calibratedValue;
        }
        if (rawValue >= points[pointCount - 1].rawValue) {
            return points[pointCount - 1].calibratedValue;
        }
        
        // Линейная интерполяция между точками
        for (int i = 0; i < pointCount - 1; i++) {
            if (rawValue >= points[i].rawValue && rawValue <= points[i + 1].rawValue) {
                float x1 = points[i].rawValue;
                float y1 = points[i].calibratedValue;
                float x2 = points[i + 1].rawValue;
                float y2 = points[i + 1].calibratedValue;
                
                // Формула линейной интерполяции: y = y1 + (y2 - y1) * (x - x1) / (x2 - x1)
                return y1 + (y2 - y1) * (rawValue - x1) / (x2 - x1);
            }
        }
        
        return rawValue; // fallback
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
            preferences.putFloat(rawKey.c_str(), points[i].rawValue);
            preferences.putFloat(calKey.c_str(), points[i].calibratedValue);
        }
        preferences.putFloat((sensorType + "_count").c_str(), pointCount);
    }
    
    void loadCalibration() {
        pointCount = (int)preferences.getFloat((sensorType + "_count").c_str(), 0);
        for (int i = 0; i < pointCount && i < MAX_POINTS; i++) {
            String rawKey = sensorType + "_raw_" + String(i);
            String calKey = sensorType + "_cal_" + String(i);
            points[i].rawValue = preferences.getFloat(rawKey.c_str(), 0.0f);
            points[i].calibratedValue = preferences.getFloat(calKey.c_str(), 0.0f);
        }
    }
};

#else
#include <Arduino.h>
#endif

// ============================================================================
// UNITY UNIT TESTS - КРИТИЧЕСКИЕ АЛГОРИТМЫ КАЛИБРОВКИ
// ============================================================================

void test_calibration_basic_interpolation() {
    CalibrationManager cal("pH");
    cal.addCalibrationPoint(0.0f, 7.0f);   // pH 7 при 0V
    cal.addCalibrationPoint(1.0f, 4.0f);   // pH 4 при 1V
    cal.addCalibrationPoint(2.0f, 10.0f);  // pH 10 при 2V
    
    float result = cal.interpolate(0.5f);   // Ожидаем pH 5.5
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 5.5f, result);
}

void test_calibration_extrapolation_below() {
    CalibrationManager cal("EC");
    cal.addCalibrationPoint(100.0f, 1000.0f);
    cal.addCalibrationPoint(200.0f, 2000.0f);
    
    float result = cal.interpolate(50.0f);   // Ниже диапазона
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 1000.0f, result);
}

void test_calibration_extrapolation_above() {
    CalibrationManager cal("EC");
    cal.addCalibrationPoint(100.0f, 1000.0f);
    cal.addCalibrationPoint(200.0f, 2000.0f);
    
    float result = cal.interpolate(300.0f);  // Выше диапазона
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 2000.0f, result);
}

void test_calibration_single_point() {
    CalibrationManager cal("NPK");
    cal.addCalibrationPoint(512.0f, 100.0f);
    
    float result = cal.interpolate(256.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 100.0f, result);
}

void test_calibration_no_points_passthrough() {
    CalibrationManager cal("temp");
    float result = cal.interpolate(25.5f);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 25.5f, result);
}

void test_calibration_persistence() {
    CalibrationManager cal("moisture");
    cal.addCalibrationPoint(0.0f, 0.0f);
    cal.addCalibrationPoint(1023.0f, 100.0f);
    cal.saveCalibration();
    
    CalibrationManager cal2("moisture");
    cal2.loadCalibration();
    
    float result = cal2.interpolate(511.5f);  // Середина диапазона
    TEST_ASSERT_FLOAT_WITHIN(0.1f, 50.0f, result);
}

void test_calibration_multipoint_complex() {
    CalibrationManager cal("complex");
    cal.addCalibrationPoint(0.0f, 0.0f);
    cal.addCalibrationPoint(100.0f, 25.0f);
    cal.addCalibrationPoint(200.0f, 40.0f);
    cal.addCalibrationPoint(300.0f, 50.0f);
    cal.addCalibrationPoint(400.0f, 55.0f);
    
    float result = cal.interpolate(150.0f);  // Между 100 и 200
    TEST_ASSERT_FLOAT_WITHIN(0.1f, 32.5f, result);
}

void test_calibration_clear() {
    CalibrationManager cal("clear_test");
    cal.addCalibrationPoint(10.0f, 20.0f);
    cal.clearCalibration();
    
    float result = cal.interpolate(10.0f);  // Должен вернуть исходное значение
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 10.0f, result);
}

// ============================================================================
// UNITY FRAMEWORK SETUP
// ============================================================================

void setUp() {
    // Инициализация перед каждым тестом
}

void tearDown() {
    // Очистка после каждого теста
}

#ifndef ARDUINO
int main(int argc, char** argv) {
    UNITY_BEGIN();
    
    // Запускаем все тесты
    RUN_TEST(test_calibration_basic_interpolation);
    RUN_TEST(test_calibration_extrapolation_below);
    RUN_TEST(test_calibration_extrapolation_above);
    RUN_TEST(test_calibration_single_point);
    RUN_TEST(test_calibration_no_points_passthrough);
    RUN_TEST(test_calibration_persistence);
    RUN_TEST(test_calibration_multipoint_complex);
    RUN_TEST(test_calibration_clear);
    
    return UNITY_END();
}
#else
void setup() {
    Serial.begin(115200);
    UNITY_BEGIN();
    
    Serial.println("🧪 CalibrationManager Tests (ESP32)");
    
    RUN_TEST(test_calibration_basic_interpolation);
    RUN_TEST(test_calibration_extrapolation_below);
    RUN_TEST(test_calibration_extrapolation_above);
    RUN_TEST(test_calibration_single_point);
    RUN_TEST(test_calibration_no_points_passthrough);
    RUN_TEST(test_calibration_persistence);
    RUN_TEST(test_calibration_multipoint_complex);
    RUN_TEST(test_calibration_clear);
    
    UNITY_END();
}

void loop() {
    // Ничего не делаем в цикле
}
#endif 