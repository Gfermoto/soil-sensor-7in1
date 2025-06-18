#include <Arduino.h>
#include <unity.h>

#include "validation_utils.h"
#include "sensor_compensation.h"
#ifndef ARDUINO
#include "../src/validation_utils.cpp"
#include "../src/sensor_compensation.cpp"
#include "../stubs/logger.cpp"
#endif

void setUp() {}
void tearDown() {}

void test_validateSSID_empty() {
    auto res = validateSSID("");
    TEST_ASSERT_FALSE(res.isValid);
}

void test_validateSSID_valid() {
    auto res = validateSSID("HomeWiFi");
    TEST_ASSERT_TRUE(res.isValid);
}

void test_validatePassword_short() {
    auto res = validatePassword("1234567"); // 7 symbols
    TEST_ASSERT_FALSE(res.isValid);
}

void test_validatePassword_ok() {
    auto res = validatePassword("12345678");
    TEST_ASSERT_TRUE(res.isValid);
}

void test_validateTemperature_bounds() {
    auto low = validateTemperature(-41.0f);
    auto ok = validateTemperature(25.0f);
    auto high = validateTemperature(85.0f);
    TEST_ASSERT_FALSE(low.isValid);
    TEST_ASSERT_TRUE(ok.isValid);
    TEST_ASSERT_FALSE(high.isValid);
}

void test_correctEC_no_change() {
    float ecRaw = 1500.0f;
    float T = 25.0f;
    float theta = 45.0f;
    float ec = correctEC(ecRaw, T, theta, SoilType::LOAM);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, 1500.0f, ec);
}

void test_correctPH_colder() {
    float phRaw = 6.5f;
    float ph = correctPH(phRaw, 15.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 6.53f, ph); // +0.03
}

void test_correctNPK_warmer() {
    float N=100, P=50, K=80;
    const float T = 35.0f;     // +10 °C
    const float theta = 40.0f; // 40 %

    // Расчёт ожидаемых коэффициентов (из sensor_compensation.cpp)
    const float k_t_N = 0.0041f;
    const float k_t_P = 0.0053f;
    const float k_t_K = 0.0032f;

    auto k_h_N = [](float th){ return 1.8f - 0.024f * th; };
    auto k_h_P = [](float th){ return 1.6f - 0.018f * th; };
    auto k_h_K = [](float th){ return 1.9f - 0.021f * th; };

    float expN = N * (1.0f - k_t_N * (T - 25.0f)) * k_h_N(theta);
    float expP = P * (1.0f - k_t_P * (T - 25.0f)) * k_h_P(theta);
    float expK = K * (1.0f - k_t_K * (T - 25.0f)) * k_h_K(theta);

    correctNPK(T, theta, N, P, K, SoilType::SAND);

    TEST_ASSERT_FLOAT_WITHIN(0.1f, expN, N);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, expP, P);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, expK, K);
}

// Arduino-style entry point (device tests)
#ifdef ARDUINO
void setup() {
    Serial.begin(115200);
    delay(2000);
    UNITY_BEGIN();
    RUN_TEST(test_validateSSID_empty);
    RUN_TEST(test_validateSSID_valid);
    RUN_TEST(test_validatePassword_short);
    RUN_TEST(test_validatePassword_ok);
    RUN_TEST(test_validateTemperature_bounds);
    RUN_TEST(test_correctEC_no_change);
    RUN_TEST(test_correctPH_colder);
    RUN_TEST(test_correctNPK_warmer);
    UNITY_END();
}

void loop() {}
#else // Native PC tests
#include <unity.h>
int main(int argc, char **argv) {
    (void) argc; (void) argv;
    UNITY_BEGIN();
    RUN_TEST(test_validateSSID_empty);
    RUN_TEST(test_validateSSID_valid);
    RUN_TEST(test_validatePassword_short);
    RUN_TEST(test_validatePassword_ok);
    RUN_TEST(test_validateTemperature_bounds);
    RUN_TEST(test_correctEC_no_change);
    RUN_TEST(test_correctPH_colder);
    RUN_TEST(test_correctNPK_warmer);
    return UNITY_END();
}
#endif // ARDUINO 