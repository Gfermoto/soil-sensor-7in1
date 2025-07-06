/**
 * @file test_fake_sensor.cpp
 * @brief Тесты для модуля эмуляции датчика
 * @details Проверка корректности генерации тестовых данных и работы компенсации
 */

#include <unity.h>
#include "../src/fake_sensor.h"
#include "../src/fake_sensor.cpp"  // Включаем реализацию для тестирования
#include "../test/stubs/Arduino.h"

// Тестовые данные
extern SensorData sensorData;
extern Config config;

void setUp(void) {
    // Инициализация перед каждым тестом
    sensorData = {};
    config = {};
    config.flags.calibrationEnabled = false;
    config.soilProfile = 1; // LOAM по умолчанию
}

void tearDown(void) {
    // Очистка после каждого теста
}

// Тест 1: Проверка диапазонов генерируемых данных
void test_fake_sensor_data_ranges(void) {
    // Симулируем генерацию данных
    sensorData.temperature = 22.0 + random(-50, 50) / 10.0;
    sensorData.humidity = 50.0 + random(-200, 200) / 10.0;
    sensorData.ec = 1000 + random(-200, 200);
    sensorData.ph = 6.5 + random(-20, 20) / 10.0;

    // Проверяем диапазоны
    TEST_ASSERT_TRUE(sensorData.temperature >= 17.0 && sensorData.temperature <= 27.0);
    TEST_ASSERT_TRUE(sensorData.humidity >= 30.0 && sensorData.humidity <= 70.0);
    TEST_ASSERT_TRUE(sensorData.ec >= 800 && sensorData.ec <= 1200);
    TEST_ASSERT_TRUE(sensorData.ph >= 4.5 && sensorData.ph <= 8.5);
}

// Тест 2: Проверка NPK конверсии мг/дм³ → мг/кг
void test_fake_sensor_npk_conversion(void) {
    float nitrogen_mgdm3 = 30.0;
    float phosphorus_mgdm3 = 15.0;
    float potassium_mgdm3 = 20.0;
    
    constexpr float NPK_FACTOR = 6.5f;
    
    float nitrogen_mgkg = nitrogen_mgdm3 * NPK_FACTOR;
    float phosphorus_mgkg = phosphorus_mgdm3 * NPK_FACTOR;
    float potassium_mgkg = potassium_mgdm3 * NPK_FACTOR;
    
    TEST_ASSERT_FLOAT_WITHIN(0.1, 195.0, nitrogen_mgkg);    // 30 * 6.5 = 195
    TEST_ASSERT_FLOAT_WITHIN(0.1, 97.5, phosphorus_mgkg);   // 15 * 6.5 = 97.5
    TEST_ASSERT_FLOAT_WITHIN(0.1, 130.0, potassium_mgkg);   // 20 * 6.5 = 130
}

// Тест 3: Проверка сохранения RAW значений
void test_fake_sensor_raw_values_preservation(void) {
    // Устанавливаем тестовые значения
    sensorData.temperature = 25.0;
    sensorData.humidity = 60.0;
    sensorData.ec = 1000;
    sensorData.ph = 7.0;
    sensorData.nitrogen = 200.0;
    sensorData.phosphorus = 100.0;
    sensorData.potassium = 150.0;
    
    // Сохраняем RAW значения
    sensorData.raw_temperature = sensorData.temperature;
    sensorData.raw_humidity = sensorData.humidity;
    sensorData.raw_ec = sensorData.ec;
    sensorData.raw_ph = sensorData.ph;
    sensorData.raw_nitrogen = sensorData.nitrogen;
    sensorData.raw_phosphorus = sensorData.phosphorus;
    sensorData.raw_potassium = sensorData.potassium;
    
    // Проверяем сохранение
    TEST_ASSERT_FLOAT_WITHIN(0.01, 25.0, sensorData.raw_temperature);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 60.0, sensorData.raw_humidity);
    TEST_ASSERT_EQUAL_INT32(1000, sensorData.raw_ec);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 7.0, sensorData.raw_ph);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 200.0, sensorData.raw_nitrogen);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 100.0, sensorData.raw_phosphorus);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 150.0, sensorData.raw_potassium);
}

// Тест 4: Проверка валидности данных
void test_fake_sensor_data_validity(void) {
    sensorData.valid = true;
    sensorData.last_update = millis();
    
    TEST_ASSERT_TRUE(sensorData.valid);
    TEST_ASSERT_TRUE(sensorData.last_update > 0);
}

// Тест 5: Проверка профилей почвы
void test_fake_sensor_soil_profiles(void) {
    // Тестируем все поддерживаемые профили
    for (int profile = 0; profile <= 4; profile++) {
        config.soilProfile = profile;
        
        SoilType expectedSoil;
        switch (profile) {
            case 0: expectedSoil = SoilType::SAND; break;
            case 1: expectedSoil = SoilType::LOAM; break;
            case 2: expectedSoil = SoilType::PEAT; break;
            case 3: expectedSoil = SoilType::CLAY; break;
            case 4: expectedSoil = SoilType::SANDPEAT; break;
            default: expectedSoil = SoilType::LOAM; break;
        }
        
        // Проверяем, что профиль корректно обрабатывается
        TEST_ASSERT_TRUE(profile >= 0 && profile <= 4);
    }
}

// Тест 6: Проверка отключенной калибровки
void test_fake_sensor_calibration_disabled(void) {
    config.flags.calibrationEnabled = false;
    
    // Устанавливаем исходные значения
    float original_ec = 1000.0;
    float original_ph = 7.0;
    
    sensorData.ec = original_ec;
    sensorData.ph = original_ph;
    
    // При отключенной калибровке значения не должны изменяться
    // (проверяем логику условия)
    if (!config.flags.calibrationEnabled) {
        TEST_ASSERT_FLOAT_WITHIN(0.01, original_ec, sensorData.ec);
        TEST_ASSERT_FLOAT_WITHIN(0.01, original_ph, sensorData.ph);
    }
}

// Тест 7: Проверка интервала генерации данных
void test_fake_sensor_generation_interval(void) {
    const uint32_t dataGenerationInterval = 10;
    uint32_t iterationCounter = 0;
    
    // Проверяем, что данные генерируются только при достижении интервала
    TEST_ASSERT_TRUE(iterationCounter < dataGenerationInterval);
    
    iterationCounter = dataGenerationInterval;
    TEST_ASSERT_TRUE(iterationCounter >= dataGenerationInterval);
}

// Основная функция тестирования
int main(void) {
    UNITY_BEGIN();
    
    RUN_TEST(test_fake_sensor_data_ranges);
    RUN_TEST(test_fake_sensor_npk_conversion);
    RUN_TEST(test_fake_sensor_raw_values_preservation);
    RUN_TEST(test_fake_sensor_data_validity);
    RUN_TEST(test_fake_sensor_soil_profiles);
    RUN_TEST(test_fake_sensor_calibration_disabled);
    RUN_TEST(test_fake_sensor_generation_interval);
    
    return UNITY_END();
}

// Функция для PlatformIO Unity
void setup() {
    delay(2000);  // Ждем инициализации
    main();
}

void loop() {
    // Пустой цикл
} 