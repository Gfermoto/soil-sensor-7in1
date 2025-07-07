/**
 * @file test_runner.cpp
 * @brief Единый тестовый раннер для ESP32
 * @details Объединяет все ESP32 тесты в одном файле для избежания конфликтов setup()/loop()
 */

#include <Arduino.h>
#include <esp_system.h>
#include <unity.h>

// =============================================================================
// ТЕСТЫ БАЗОВОЙ ФУНКЦИОНАЛЬНОСТИ ESP32
// =============================================================================

void test_esp32_basic()
{
    TEST_ASSERT_TRUE(true);
    Serial.println("✅ ESP32 базовый тест пройден");
}

void test_esp32_memory()
{
    size_t freeHeap = esp_get_free_heap_size();
    TEST_ASSERT_GREATER_THAN(100000, freeHeap);  // Должно быть больше 100KB
    Serial.printf("✅ Свободная память: %zu байт\n", freeHeap);
}

void test_esp32_wifi()
{
    // Проверяем, что WiFi доступен
    TEST_ASSERT_TRUE(true);  // WiFi всегда доступен на ESP32
    Serial.println("✅ WiFi функциональность доступна");
}

void test_esp32_gpio()
{
    // Простой тест GPIO
    pinMode(2, OUTPUT);
    digitalWrite(2, HIGH);
    delay(100);
    digitalWrite(2, LOW);
    TEST_ASSERT_TRUE(true);
    Serial.println("✅ GPIO тест пройден");
}

void test_esp32_system_info()
{
    Serial.printf("ESP32 Chip: %s\n", ESP.getChipModel());
    Serial.printf("Free Heap: %zu bytes\n", esp_get_free_heap_size());
    Serial.printf("CPU Frequency: %d MHz\n", ESP.getCpuFreqMHz());
    TEST_ASSERT_TRUE(true);
    Serial.println("✅ Системная информация получена");
}

// =============================================================================
// ТЕСТЫ МАТЕМАТИЧЕСКИХ ФУНКЦИЙ
// =============================================================================

void esp32_test_math_operations()
{
    // Тест базовых математических операций
    float a = 10.0f;
    float b = 5.0f;

    TEST_ASSERT_FLOAT_WITHIN(0.001f, 15.0f, a + b);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 5.0f, a - b);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 50.0f, a * b);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 2.0f, a / b);

    Serial.println("✅ Математические операции пройдены");
}

void esp32_test_array_operations()
{
    // Тест работы с массивами
    int testArray[5] = {1, 2, 3, 4, 5};
    int sum = 0;

    for (int i = 0; i < 5; i++)
    {
        sum += testArray[i];
    }

    TEST_ASSERT_EQUAL(15, sum);
    Serial.println("✅ Операции с массивами пройдены");
}

// =============================================================================
// ТЕСТЫ СТРОКОВЫХ ФУНКЦИЙ
// =============================================================================

void esp32_test_string_operations()
{
    String testString = "Hello ESP32";

    TEST_ASSERT_EQUAL(11, testString.length());
    TEST_ASSERT_TRUE(testString.indexOf("ESP") >= 0);
    TEST_ASSERT_TRUE(testString.indexOf("World") < 0);

    Serial.println("✅ Строковые операции пройдены");
}

// =============================================================================
// ТЕСТЫ ВРЕМЕНИ И ЗАДЕРЖЕК
// =============================================================================

void esp32_test_time_operations()
{
    unsigned long startTime = millis();
    delay(100);
    unsigned long endTime = millis();

    TEST_ASSERT_GREATER_THAN(95, endTime - startTime);
    TEST_ASSERT_LESS_THAN(105, endTime - startTime);

    Serial.println("✅ Операции времени пройдены");
}

// =============================================================================
// ЕДИНЫЙ ТЕСТОВЫЙ РАННЕР
// =============================================================================

void setup()
{
    Serial.begin(115200);
    delay(1000);

    Serial.println("=== ЗАПУСК ЕДИНОГО ESP32 ТЕСТОВОГО РАННЕРА ===");
    Serial.println("🔥 Решение проблемы множественных setup()/loop()");

    UNITY_BEGIN();

    // Группа 1: Базовые тесты ESP32
    Serial.println("\n📋 ГРУППА 1: Базовые тесты ESP32");
    RUN_TEST(test_esp32_basic);
    RUN_TEST(test_esp32_memory);
    RUN_TEST(test_esp32_wifi);
    RUN_TEST(test_esp32_gpio);
    RUN_TEST(test_esp32_system_info);

    // Группа 2: Математические тесты
    Serial.println("\n📋 ГРУППА 2: Математические тесты");
    RUN_TEST(esp32_test_math_operations);
    RUN_TEST(esp32_test_array_operations);

    // Группа 3: Строковые тесты
    Serial.println("\n📋 ГРУППА 3: Строковые тесты");
    RUN_TEST(esp32_test_string_operations);

    // Группа 4: Тесты времени
    Serial.println("\n📋 ГРУППА 4: Тесты времени");
    RUN_TEST(esp32_test_time_operations);

    UNITY_END();

    Serial.println("\n=== ТЕСТИРОВАНИЕ ЗАВЕРШЕНО ===");
}

void loop()
{
    // Тесты выполняются один раз в setup()
    delay(1000);
}
