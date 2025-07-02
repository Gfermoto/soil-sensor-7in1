#include <Arduino.h>
#include <unity.h>

// Подключаем все тесты
extern void test_simple();
extern void test_calibration();
extern void test_all_combined();

void setup()
{
    // Инициализация Unity
    UNITY_BEGIN();

    // Запускаем тесты
    test_simple();
    test_calibration();
    test_all_combined();

    // Завершаем тестирование
    UNITY_END();
}

void loop()
{
    // Тесты выполняются один раз в setup()
    delay(1000);
}
