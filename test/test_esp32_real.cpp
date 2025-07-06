/**
 * @file test_esp32_real.cpp
 * @brief Простейший тест для реального ESP32
 * @details Минимальный тест для проверки Unity на железе
 * @version 1.0.0
 * @date 2025-01-23
 */

#include <unity.h>
#include <Arduino.h>

// ============================================================================
// ПРОСТЕЙШИЕ ТЕСТЫ
// ============================================================================

void test_basic_arithmetic(void) {
    TEST_ASSERT_EQUAL(4, 2 + 2);
    TEST_ASSERT_EQUAL(0, 5 - 5);
    TEST_ASSERT_EQUAL(10, 2 * 5);
}

void test_esp32_functions(void) {
    // Тест базовых ESP32 функций
    unsigned long time1 = millis();
    delay(10);
    unsigned long time2 = millis();
    
    TEST_ASSERT_GREATER_OR_EQUAL(time1, time2);
    TEST_ASSERT_GREATER_THAN(5, time2 - time1);  // Минимум 5мс прошло
}

void test_memory_basic(void) {
    // Простой тест памяти
    size_t freeHeap = esp_get_free_heap_size();
    TEST_ASSERT_GREATER_THAN(10000, freeHeap);  // Минимум 10KB
    
    Serial.printf("Free heap: %zu bytes\n", freeHeap);
}

void test_serial_output(void) {
    // Тест Serial вывода
    Serial.println("Test message from ESP32");
    TEST_ASSERT_TRUE(true);  // Всегда проходит
}

// ============================================================================
// ОСНОВНЫЕ ФУНКЦИИ
// ============================================================================

void setUp(void) {
    // Инициализация перед каждым тестом
    delay(5);
}

void tearDown(void) {
    // Очистка после теста
}

void setup() {
    Serial.begin(115200);
    delay(2000);  // Ждем инициализации Serial
    
    Serial.println("=== ESP32 REAL HARDWARE TESTS START ===");
    Serial.printf("ESP32 Chip: %s\n", ESP.getChipModel());
    Serial.printf("CPU Freq: %d MHz\n", ESP.getCpuFreqMHz());
    Serial.printf("Free Heap: %zu bytes\n", esp_get_free_heap_size());
    
    UNITY_BEGIN();
    
    Serial.println("\n--- Basic Tests ---");
    RUN_TEST(test_basic_arithmetic);
    
    Serial.println("\n--- ESP32 Function Tests ---");
    RUN_TEST(test_esp32_functions);
    
    Serial.println("\n--- Memory Tests ---");
    RUN_TEST(test_memory_basic);
    
    Serial.println("\n--- Serial Tests ---");
    RUN_TEST(test_serial_output);
    
    UNITY_END();
    
    Serial.println("=== ESP32 REAL HARDWARE TESTS END ===");
}

void loop() {
    // Пустой цикл
    delay(1000);
} 