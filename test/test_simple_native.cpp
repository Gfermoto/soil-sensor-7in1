#include <unity.h>
#include <iostream>

// Необходимые заглушки для Unity
void setUp() {}
void tearDown() {}

// Простой тест для проверки native environment
void test_native_environment()
{
    // Проверка native environment
    int result = 2 + 2;
    TEST_ASSERT_EQUAL(4, result);
    std::cout << "✅ Native environment работает корректно" << std::endl;
}

// Тест математических операций
void test_math_operations()
{
    // Тест математических операций
    float a = 10.5f;
    float b = 5.2f;
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 15.7f, a + b);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 5.3f, a - b);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 54.6f, a * b);
    std::cout << "✅ Математические операции работают корректно" << std::endl;
}

// Тест строковых операций
void test_string_operations()
{
    // Тест строковых операций
    std::string test_string = "JXCT Test";
    TEST_ASSERT_EQUAL(9, test_string.length());
    TEST_ASSERT_TRUE(test_string.find("JXCT") != std::string::npos);
    TEST_ASSERT_TRUE(test_string.find("Test") != std::string::npos);
    std::cout << "✅ Строковые операции работают корректно" << std::endl;
}

int main()
{
    UNITY_BEGIN();
    std::cout << "=== Запуск простых native тестов ===" << std::endl;
    RUN_TEST(test_native_environment);
    RUN_TEST(test_math_operations);
    RUN_TEST(test_string_operations);
    std::cout << "=== Native тесты завершены ===" << std::endl;
    return UNITY_END();
}
