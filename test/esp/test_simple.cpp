/**
 * @file test_simple.cpp
 * @brief Упрощённые тесты JXCT для первоначального запуска
 * @version 1.0.0
 * @date 2025-01-22
 * @author EYERA Development Team
 */

#include <unity.h>

#ifndef ARDUINO
#include <chrono>
#include <fstream>
#include <iostream>

// Включаем заголовочные файлы для тестирования
#include "../include/jxct_format_utils.h"
#include "../include/logger.h"
#include "../include/sensor_compensation.h"
#include "../include/validation_utils.h"

// Включаем реализации для тестирования
#include "../src/jxct_format_utils.cpp"
#include "../src/sensor_compensation.cpp"
#include "../src/validation_utils.cpp"
#include "../stubs/logger.cpp"
#else
#include <Arduino.h>
#endif

// Простая структура для сбора статистики
struct TestStats
{
    int total = 0;
    int passed = 0;
    int failed = 0;

    void addResult(bool passed)
    {
        total++;
        if (passed)
        {
            this->passed++;
        }
        else
        {
            this->failed++;
        }
    }

    void generateReport()
    {
#ifndef ARDUINO
        std::cout << "\n📊 Статистика тестов:" << std::endl;
        std::cout << "  Всего: " << total << std::endl;
        std::cout << "  Прошли: " << passed << std::endl;
        std::cout << "  Провалились: " << failed << std::endl;
        std::cout << "  Успешность: " << (total > 0 ? (passed * 100.0 / total) : 0) << "%" << std::endl;

        // Создаём простой JSON отчёт
        std::ofstream report("test_reports/simple-test-report.json");
        if (report.is_open())
        {
            report << "{\n";
            report << "  \"timestamp\": \"2025-01-22T12:00:00Z\",\n";
            report << "  \"project\": \"JXCT Soil Sensor\",\n";
            report << "  \"version\": \"3.6.0\",\n";
            report << "  \"summary\": {\n";
            report << "    \"total\": " << total << ",\n";
            report << "    \"passed\": " << passed << ",\n";
            report << "    \"failed\": " << failed << ",\n";
            report << "    \"success_rate\": " << (total > 0 ? (passed * 100.0 / total) : 0) << "\n";
            report << "  }\n";
            report << "}\n";
            report.close();
            std::cout << "📄 Отчёт сохранён: test_reports/simple-test-report.json" << std::endl;
        }
#else
        Serial.print("Tests: ");
        Serial.print(total);
        Serial.print(", Passed: ");
        Serial.print(passed);
        Serial.print(", Failed: ");
        Serial.println(failed);
#endif
    }
};

TestStats g_stats;

// ============================================================================
// UNIT TESTS - Юнит тесты
// ============================================================================

void test_validateSSID_empty()
{
    auto result = validateSSID("");
    TEST_ASSERT_FALSE(result.isValid);
    g_stats.addResult(result.isValid == false);
}

void test_validateSSID_valid()
{
    auto result = validateSSID("HomeWiFi");
    TEST_ASSERT_TRUE(result.isValid);
    g_stats.addResult(result.isValid == true);
}

void test_validateSSID_too_long()
{
    auto result = validateSSID("VeryLongSSIDNameThatExceedsThirtyTwoCharacters");
    TEST_ASSERT_FALSE(result.isValid);
    g_stats.addResult(result.isValid == false);
}

void test_validatePassword_short()
{
    auto result = validatePassword("1234567");  // 7 symbols
    TEST_ASSERT_FALSE(result.isValid);
    g_stats.addResult(result.isValid == false);
}

void test_validatePassword_ok()
{
    auto result = validatePassword("12345678");
    TEST_ASSERT_TRUE(result.isValid);
    g_stats.addResult(result.isValid == true);
}

void test_validatePassword_complex()
{
    auto result = validatePassword("MySecure123!@#");
    TEST_ASSERT_TRUE(result.isValid);
    g_stats.addResult(result.isValid == true);
}

void test_validateTemperature_bounds()
{
    auto low = validateTemperature(-41.0f);
    auto ok = validateTemperature(25.0f);
    auto high = validateTemperature(85.0f);

    TEST_ASSERT_FALSE(low.isValid);
    TEST_ASSERT_TRUE(ok.isValid);
    TEST_ASSERT_FALSE(high.isValid);

    g_stats.addResult(low.isValid == false && ok.isValid == true && high.isValid == false);
}

void test_validateTemperature_edge_cases()
{
    auto min_valid = validateTemperature(-40.0f);
    auto max_valid = validateTemperature(80.0f);
    auto zero = validateTemperature(0.0f);

    TEST_ASSERT_TRUE(min_valid.isValid);
    TEST_ASSERT_TRUE(max_valid.isValid);
    TEST_ASSERT_TRUE(zero.isValid);

    g_stats.addResult(min_valid.isValid && max_valid.isValid && zero.isValid);
}

// ============================================================================
// SENSOR COMPENSATION TESTS - Тесты компенсации датчиков
// ============================================================================

void test_correctEC_no_change()
{
    float ecRaw = 1500.0f;
    float T = 25.0f;
    float theta = 45.0f;
    float ec = correctEC(ecRaw, T, theta, SoilType::LOAM);

    // При стандартных условиях изменения должны быть минимальными
    TEST_ASSERT_FLOAT_WITHIN(100.0f, 1500.0f, ec);
    g_stats.addResult(fabs(ec - 1500.0f) < 100.0f);
}

void test_correctPH_temperature_compensation()
{
    float phRaw = 6.5f;
    float ph_cold = correctPH(phRaw, 15.0f);  // Холоднее
    float ph_hot = correctPH(phRaw, 35.0f);   // Теплее

    // pH должен корректироваться с температурой
    TEST_ASSERT_NOT_EQUAL(phRaw, ph_cold);
    TEST_ASSERT_NOT_EQUAL(phRaw, ph_hot);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 6.5f, ph_cold);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 6.5f, ph_hot);

    g_stats.addResult(ph_cold != phRaw && ph_hot != phRaw);
}

void test_correctNPK_basic()
{
    float N = 100.0f, P = 50.0f, K = 80.0f;
    float original_N = N, original_P = P, original_K = K;

    correctNPK(30.0f, 40.0f, N, P, K, SoilType::SAND);

    // Значения должны измениться, но остаться в разумных пределах
    TEST_ASSERT_TRUE(N > 0 && N < 200);
    TEST_ASSERT_TRUE(P > 0 && P < 100);
    TEST_ASSERT_TRUE(K > 0 && K < 150);

    g_stats.addResult(N > 0 && N < 200 && P > 0 && P < 100 && K > 0 && K < 150);
}

// ============================================================================
// PERFORMANCE TESTS - Простые тесты производительности
// ============================================================================

void test_validation_performance()
{
#ifndef ARDUINO
    auto start = std::chrono::high_resolution_clock::now();

    // Выполняем много валидаций
    for (int i = 0; i < 1000; ++i)
    {
        validateSSID("TestSSID");
        validatePassword("TestPassword123");
        validateTemperature(25.0f + i * 0.01f);
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    // Должно выполняться быстро (менее 100мс на 1000 итераций)
    TEST_ASSERT_TRUE(duration.count() < 100);
    g_stats.addResult(duration.count() < 100);

    std::cout << "⏱️ Производительность валидации: " << duration.count() << "ms для 1000 операций" << std::endl;
#else
    // На Arduino просто проверяем, что функции работают
    TEST_ASSERT_TRUE(validateSSID("Test").isValid);
    g_stats.addResult(true);
#endif
}

void test_compensation_performance()
{
#ifndef ARDUINO
    auto start = std::chrono::high_resolution_clock::now();

    // Выполняем много компенсаций
    for (int i = 0; i < 500; ++i)
    {
        float N = 100.0f, P = 50.0f, K = 80.0f;
        correctNPK(25.0f + i * 0.02f, 40.0f + i * 0.01f, N, P, K, SoilType::LOAM);
        correctEC(1500.0f + i, 25.0f + i * 0.02f, 40.0f, SoilType::SAND);
        correctPH(6.5f + i * 0.001f, 25.0f + i * 0.02f);
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    // Должно выполняться быстро (менее 50мс на 500 итераций)
    TEST_ASSERT_TRUE(duration.count() < 50);
    g_stats.addResult(duration.count() < 50);

    std::cout << "⏱️ Производительность компенсации: " << duration.count() << "ms для 500 операций" << std::endl;
#else
    // На Arduino просто проверяем базовую функциональность
    float N = 100.0f, P = 50.0f, K = 80.0f;
    correctNPK(25.0f, 40.0f, N, P, K, SoilType::LOAM);
    TEST_ASSERT_TRUE(N > 0);
    g_stats.addResult(true);
#endif
}

// ============================================================================
// MAIN TEST RUNNER
// ============================================================================

void setUp()
{
    // Инициализация перед каждым тестом
}

void tearDown()
{
    // Очистка после каждого теста
}

#ifndef ARDUINO
int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    std::cout << "🧪 Запуск упрощённого тестирования JXCT..." << std::endl;

    // Создаём директорию для отчётов
    system("mkdir -p test_reports 2>/dev/null || md test_reports >nul 2>&1");

    UNITY_BEGIN();

    // Unit tests - валидация
    RUN_TEST(test_validateSSID_empty);
    RUN_TEST(test_validateSSID_valid);
    RUN_TEST(test_validateSSID_too_long);
    RUN_TEST(test_validatePassword_short);
    RUN_TEST(test_validatePassword_ok);
    RUN_TEST(test_validatePassword_complex);
    RUN_TEST(test_validateTemperature_bounds);
    RUN_TEST(test_validateTemperature_edge_cases);

    // Sensor compensation tests
    RUN_TEST(test_correctEC_no_change);
    RUN_TEST(test_correctPH_temperature_compensation);
    RUN_TEST(test_correctNPK_basic);

    // Performance tests
    RUN_TEST(test_validation_performance);
    RUN_TEST(test_compensation_performance);

    int result = UNITY_END();

    // Генерируем отчёт
    g_stats.generateReport();

    std::cout << "\n✅ Тестирование завершено!" << std::endl;

    return result;
}

#else
// Arduino version
void setup()
{
    Serial.begin(115200);
    delay(2000);

    Serial.println("🧪 JXCT Simple Testing Started");

    UNITY_BEGIN();

    // Запускаем основные тесты на Arduino
    RUN_TEST(test_validateSSID_empty);
    RUN_TEST(test_validateSSID_valid);
    RUN_TEST(test_validatePassword_short);
    RUN_TEST(test_validatePassword_ok);
    RUN_TEST(test_validateTemperature_bounds);
    RUN_TEST(test_correctEC_no_change);
    RUN_TEST(test_correctNPK_basic);
    RUN_TEST(test_validation_performance);
    RUN_TEST(test_compensation_performance);

    UNITY_END();

    g_stats.generateReport();

    Serial.println("✅ Testing completed");
}

void loop()
{
    // Пустой цикл
}
#endif
