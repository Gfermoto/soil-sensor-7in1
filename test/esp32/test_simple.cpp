#include <unity.h>
#include <Arduino.h>

// Простые тесты для ESP32
void test_esp32_environment() {
    TEST_ASSERT_TRUE(true);
    TEST_ASSERT_EQUAL(240000000, F_CPU);
}

void test_arduino_functions() {
    // Тест базовых Arduino функций
    unsigned long start = millis();
    delay(10);
    unsigned long end = millis();
    TEST_ASSERT_GREATER_THAN(0, end - start);
}

void test_memory_operations() {
    // Тест операций с памятью
    int* test_array = new int[10];
    TEST_ASSERT_NOT_NULL(test_array);
    
    for (int i = 0; i < 10; i++) {
        test_array[i] = i;
        TEST_ASSERT_EQUAL(i, test_array[i]);
    }
    
    delete[] test_array;
}

void test_string_operations() {
    // Тест строковых операций
    String test_string = "JXCT Soil Sensor";
    TEST_ASSERT_EQUAL(16, test_string.length());
    TEST_ASSERT_TRUE(test_string.indexOf("Soil") >= 0);
}

void RUN_UNITY_TESTS() {
    UNITY_BEGIN();
    
    RUN_TEST(test_esp32_environment);
    RUN_TEST(test_arduino_functions);
    RUN_TEST(test_memory_operations);
    RUN_TEST(test_string_operations);
    
    UNITY_END();
}

// Главная функция для тестового режима
int main(int argc, char **argv) {
    // Инициализация Arduino в тестовом режиме
    init();
    
    RUN_UNITY_TESTS();
    return 0;
} 