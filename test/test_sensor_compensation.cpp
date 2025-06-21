#include <unity.h>
#include "sensor_compensation.h"
#include <math.h>

static void test_correctEC()
{
    const float ecRaw = 1.5f;          // мС/см
    const float T = 20.0f;             // °C
    const float theta = 30.0f;         // %
    const float ec25   = ecRaw / (1.0f + 0.021f * (T - 25.0f));
    const float k      = 0.30f; // LOAM коэффициент
    const float expected = ec25 * powf(45.0f / theta, 1.0f + k);

    const float actual = correctEC(ecRaw, T, theta, SoilType::LOAM);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, expected, actual);
}

static void test_correctPH()
{
    const float phRaw = 6.80f;
    const float T = 30.0f;
    const float expected = phRaw - 0.003f * (T - 25.0f);
    const float actual = correctPH(phRaw, T);
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, expected, actual);
}

static void test_correctNPK()
{
    float N = 100.0f;
    float P = 50.0f;
    float K = 80.0f;

    const float T = 30.0f;
    const float theta = 40.0f;

    // Pre-computed ожидаемые значения
    float eN = N * (1.0f - 0.0032f * (T - 25.0f));
    float eP = P * (1.0f - 0.0042f * (T - 25.0f));
    float eK = K * (1.0f - 0.0024f * (T - 25.0f));

    eN *= (1.8f - 0.024f * theta);
    eP *= (1.6f - 0.018f * theta);
    eK *= (1.9f - 0.021f * theta);

    correctNPK(T, theta, N, P, K, SoilType::CLAY);

    TEST_ASSERT_FLOAT_WITHIN(0.01f, eN, N);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, eP, P);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, eK, K);
}

void setup()
{
    UNITY_BEGIN();
    RUN_TEST(test_correctEC);
    RUN_TEST(test_correctPH);
    RUN_TEST(test_correctNPK);
    UNITY_END();
}

void loop() {} 