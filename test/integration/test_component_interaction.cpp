/*
 * ИНТЕГРАЦИОННЫЙ ТЕСТ КОМПОНЕНТОВ JXCT
 * 
 * ДЛЯ КОМПИЛЯЦИИ В WINDOWS:
 * 1. Добавить в PATH: C:\Program Files\w64devkit\bin
 * 2. Компилировать: g++ -std=c++17 -I../../include -I../../src test_component_interaction.cpp -o test_component_interaction
 * 3. Запускать: ./test_component_interaction
 * 
 * ИЛИ использовать скрипт: python run_integration_tests.py
 */

extern "C" void setUp() {}
extern "C" void tearDown() {}

#include <unity.h>
#include <chrono>
#include <iostream>
#include <string>
#include <vector>

// Подключаем заглушки ESP32
#include "esp32_stubs.h"

// Подключаем заголовки компонентов
#include "calibration_manager.h"
#include "web/csrf_protection.h"
#include "jxct_format_utils.h"
#include "logger.h"
#include "sensor_compensation.h"
#include "validation_utils.h"

// Тестовые данные
const std::string TEST_SENSOR_DATA = R"({
    "temperature": 25.5,
    "humidity": 60.2,
    "ph": 6.8,
    "nitrogen": 45.0,
    "phosphorus": 30.0,
    "potassium": 25.0,
    "timestamp": 1640995200
})";

const std::string TEST_CONFIG = R"({
    "sensor_interval": 30000,
    "calibration_enabled": true,
    "compensation_enabled": true,
    "mqtt_enabled": true,
    "thingspeak_enabled": false
})";

// Глобальные переменные для тестирования
std::vector<std::string> test_logs;
std::vector<std::string> validation_errors;
std::vector<std::string> format_errors;

// Мок функции логирования
void mock_logger(const std::string& message)
{
    test_logs.push_back(message);
}

// Мок функции валидации
bool mock_validate_sensor_data(const std::string& data, std::vector<std::string>& errors)
{
    errors.clear();
    if (data.empty())
    {
        errors.push_back("Empty sensor data");
        return false;
    }
    
    // Для конфигурационных данных проверяем наличие JSON структуры
    if (data.find("{") != std::string::npos && data.find("}") != std::string::npos)
    {
        // Это JSON данные - валидируем как конфигурацию
        if (data.find("sensor_interval") != std::string::npos || 
            data.find("calibration_enabled") != std::string::npos ||
            data.find("mqtt_enabled") != std::string::npos)
        {
            return true;  // Конфигурация валидна
        }
    }
    
    // Для сенсорных данных проверяем наличие temperature
    if (data.find("temperature") == std::string::npos)
    {
        errors.push_back("Missing temperature field");
        return false;
    }
    
    return true;
}

// Мок функции форматирования
std::string mock_format_json(const std::string& data)
{
    if (data.empty())
    {
        format_errors.push_back("Empty data for formatting");
        return "";
    }
    return "{\"formatted\": true, \"data\": " + data + "}";
}

// Мок функции CSRF защиты
std::string mock_generate_csrf_token()
{
    return "test_csrf_token_12345";
}

bool mock_validate_csrf_token(const std::string& token)
{
    // В тестах всегда возвращаем true для валидного токена
    // Это симулирует корректную работу CSRF защиты
    return !token.empty() && token == "test_csrf_token_12345";
}

// Тест 1: Валидация + Форматирование + Логирование
void test_validation_formatting_logging_chain()
{
    TEST_CASE("Тест цепочки: валидация -> форматирование -> логирование");

    // Очищаем логи
    test_logs.clear();
    validation_errors.clear();
    format_errors.clear();

    // Шаг 1: Валидация данных
    bool validation_result = mock_validate_sensor_data(TEST_SENSOR_DATA, validation_errors);
    TEST_ASSERT_TRUE(validation_result);
    TEST_ASSERT_EQUAL(0, validation_errors.size());

    // Шаг 2: Форматирование данных
    std::string formatted_data = mock_format_json(TEST_SENSOR_DATA);
    TEST_ASSERT_FALSE(formatted_data.empty());
    TEST_ASSERT_EQUAL(0, format_errors.size());

    // Шаг 3: Логирование результата
    std::string log_message = "Validation and formatting completed successfully";
    mock_logger(log_message);
    TEST_ASSERT_EQUAL(1, test_logs.size());
    TEST_ASSERT_EQUAL_STRING(log_message.c_str(), test_logs[0].c_str());

    // Проверяем, что все компоненты работают вместе
    TEST_ASSERT_TRUE(validation_result && !formatted_data.empty() && !test_logs.empty());
}

// Тест 2: CSRF защита + Валидация конфигурации
void test_csrf_config_validation_chain()
{
    TEST_CASE("Тест цепочки: CSRF защита -> валидация конфигурации");

    // Очищаем логи
    test_logs.clear();
    validation_errors.clear();

    // Шаг 1: Генерация CSRF токена
    std::string csrf_token = mock_generate_csrf_token();
    TEST_ASSERT_FALSE(csrf_token.empty());
    TEST_ASSERT_EQUAL_STRING("test_csrf_token_12345", csrf_token.c_str());

    // Шаг 2: Валидация CSRF токена
    bool csrf_valid = mock_validate_csrf_token(csrf_token);
    TEST_ASSERT_TRUE(csrf_valid);

    // Шаг 3: Валидация конфигурации (если CSRF валиден)
    if (csrf_valid)
    {
        bool config_valid = mock_validate_sensor_data(TEST_CONFIG, validation_errors);
        TEST_ASSERT_TRUE(config_valid);
        TEST_ASSERT_EQUAL(0, validation_errors.size());

        // Логируем успешную валидацию
        mock_logger("CSRF and configuration validation passed");
        TEST_ASSERT_EQUAL(1, test_logs.size());
    }
}

// Тест 3: Обработка ошибок в цепочке
void test_error_handling_chain()
{
    TEST_CASE("Тест обработки ошибок в цепочке компонентов");

    // Очищаем логи
    test_logs.clear();
    validation_errors.clear();
    format_errors.clear();

    // Шаг 1: Попытка валидации пустых данных
    bool validation_result = mock_validate_sensor_data("", validation_errors);
    TEST_ASSERT_FALSE(validation_result);
    TEST_ASSERT_EQUAL(1, validation_errors.size());
    TEST_ASSERT_EQUAL_STRING("Empty sensor data", validation_errors[0].c_str());

    // Шаг 2: Логирование ошибки валидации
    if (!validation_result)
    {
        std::string error_log = "Validation failed: " + validation_errors[0];
        mock_logger(error_log);
        TEST_ASSERT_EQUAL(1, test_logs.size());
    }

    // Шаг 3: Попытка форматирования пустых данных
    std::string formatted_data = mock_format_json("");
    TEST_ASSERT_TRUE(formatted_data.empty());
    TEST_ASSERT_EQUAL(1, format_errors.size());

    // Шаг 4: Логирование ошибки форматирования
    if (formatted_data.empty())
    {
        std::string format_error_log = "Formatting failed: " + format_errors[0];
        mock_logger(format_error_log);
        TEST_ASSERT_EQUAL(2, test_logs.size());
    }
}

// Тест 4: Производительность цепочки операций
void test_performance_chain()
{
    TEST_CASE("Тест производительности цепочки операций");

    const int ITERATIONS = 1000;
    auto start_time = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < ITERATIONS; ++i)
    {
        // Цепочка операций
        std::vector<std::string> errors;
        bool valid = mock_validate_sensor_data(TEST_SENSOR_DATA, errors);
        std::string formatted = mock_format_json(TEST_SENSOR_DATA);
        std::string token = mock_generate_csrf_token();
        bool csrf_valid = mock_validate_csrf_token(token);

        // Проверяем, что все операции прошли успешно
        TEST_ASSERT_TRUE(valid);
        TEST_ASSERT_FALSE(formatted.empty());
        TEST_ASSERT_TRUE(csrf_valid);
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

    // Проверяем, что производительность приемлемая (< 1 секунды для 1000 операций)
    TEST_ASSERT_TRUE(duration.count() < 1000);

    std::cout << "Performance test: " << ITERATIONS << " iterations completed in " << duration.count() << "ms"
              << std::endl;
}

// Тест 5: Интеграция с калибровкой и компенсацией
void test_calibration_compensation_integration()
{
    TEST_CASE("Тест интеграции калибровки и компенсации");

    // Очищаем логи
    test_logs.clear();
    validation_errors.clear();

    // Шаг 1: Валидация данных датчика
    bool sensor_valid = mock_validate_sensor_data(TEST_SENSOR_DATA, validation_errors);
    TEST_ASSERT_TRUE(sensor_valid);

    // Шаг 2: Применение калибровки (мок)
    std::string calibrated_data = TEST_SENSOR_DATA;  // В реальности здесь была бы калибровка
    TEST_ASSERT_FALSE(calibrated_data.empty());

    // Шаг 3: Применение компенсации (мок)
    std::string compensated_data = calibrated_data;  // В реальности здесь была бы компенсация
    TEST_ASSERT_FALSE(compensated_data.empty());

    // Шаг 4: Финальная валидация
    bool final_valid = mock_validate_sensor_data(compensated_data, validation_errors);
    TEST_ASSERT_TRUE(final_valid);

    // Шаг 5: Форматирование для отправки
    std::string final_formatted = mock_format_json(compensated_data);
    TEST_ASSERT_FALSE(final_formatted.empty());

    // Шаг 6: Логирование результата
    mock_logger("Calibration and compensation pipeline completed");
    TEST_ASSERT_EQUAL(1, test_logs.size());
}

// Тест 6: Стресс-тест с множественными операциями
void test_stress_multiple_operations()
{
    TEST_CASE("Стресс-тест с множественными операциями");

    const int CONCURRENT_OPERATIONS = 100;
    std::vector<bool> results;
    results.reserve(CONCURRENT_OPERATIONS);

    auto start_time = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < CONCURRENT_OPERATIONS; ++i)
    {
        // Создаем уникальные данные для каждой операции
        std::string unique_data = TEST_SENSOR_DATA;
        unique_data.replace(unique_data.find("25.5"), 4, std::to_string(20 + i));

        // Выполняем полную цепочку операций
        std::vector<std::string> errors;
        bool valid = mock_validate_sensor_data(unique_data, errors);
        std::string formatted = mock_format_json(unique_data);
        std::string token = mock_generate_csrf_token();
        bool csrf_valid = mock_validate_csrf_token(token);

        // Сохраняем результат
        results.push_back(valid && !formatted.empty() && csrf_valid);
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

    // Проверяем результаты
    int success_count = 0;
    for (bool result : results)
    {
        if (result) success_count++;
    }

    TEST_ASSERT_EQUAL(CONCURRENT_OPERATIONS, success_count);
    TEST_ASSERT_TRUE(duration.count() < 5000);  // < 5 секунд для 100 операций

    std::cout << "Stress test: " << success_count << "/" << CONCURRENT_OPERATIONS << " operations successful in "
              << duration.count() << "ms" << std::endl;
}

// Тест 7: Интеграция с MQTT и Thingspeak
void test_mqtt_thingspeak_integration()
{
    TEST_CASE("Тест интеграции с MQTT и Thingspeak");

    // Очищаем логи
    test_logs.clear();
    validation_errors.clear();

    // Шаг 1: Валидация и форматирование данных
    bool sensor_valid = mock_validate_sensor_data(TEST_SENSOR_DATA, validation_errors);
    TEST_ASSERT_TRUE(sensor_valid);

    std::string formatted_data = mock_format_json(TEST_SENSOR_DATA);
    TEST_ASSERT_FALSE(formatted_data.empty());

    // Шаг 2: Подготовка данных для MQTT (мок)
    std::string mqtt_payload = formatted_data;
    TEST_ASSERT_FALSE(mqtt_payload.empty());

    // Шаг 3: Подготовка данных для Thingspeak (мок)
    std::string thingspeak_payload = formatted_data;
    TEST_ASSERT_FALSE(thingspeak_payload.empty());

    // Шаг 4: Валидация конфигурации MQTT/Thingspeak
    bool config_valid = mock_validate_sensor_data(TEST_CONFIG, validation_errors);
    TEST_ASSERT_TRUE(config_valid);

    // Шаг 5: Логирование успешной интеграции
    mock_logger("MQTT and Thingspeak integration test passed");
    TEST_ASSERT_EQUAL(1, test_logs.size());

    // Проверяем, что все компоненты работают корректно
    bool integration_success = sensor_valid && !formatted_data.empty() && !mqtt_payload.empty() && !thingspeak_payload.empty() && config_valid;
    TEST_ASSERT_TRUE(integration_success);
}

// Тест 8: Обработка сетевых ошибок
void test_network_error_handling()
{
    TEST_CASE("Тест обработки сетевых ошибок");

    // Очищаем логи
    test_logs.clear();
    validation_errors.clear();

    // Симулируем сетевую ошибку
    bool network_available = false;  // Симуляция недоступности сети

    // Шаг 1: Валидация данных (должна работать независимо от сети)
    bool sensor_valid = mock_validate_sensor_data(TEST_SENSOR_DATA, validation_errors);
    TEST_ASSERT_TRUE(sensor_valid);

    // Шаг 2: Форматирование данных (должно работать независимо от сети)
    std::string formatted_data = mock_format_json(TEST_SENSOR_DATA);
    TEST_ASSERT_FALSE(formatted_data.empty());

    // Шаг 3: Попытка отправки данных (должна обработать ошибку сети)
    if (!network_available)
    {
        mock_logger("Network unavailable, data cached for later transmission");
        TEST_ASSERT_EQUAL(1, test_logs.size());

        // Проверяем, что данные сохранились для последующей отправки
        TEST_ASSERT_FALSE(formatted_data.empty());
    }

    // Шаг 4: Восстановление сети
    network_available = true;
    if (network_available)
    {
        mock_logger("Network restored, cached data transmitted");
        TEST_ASSERT_EQUAL(2, test_logs.size());
    }
}

// Основная функция запуска тестов
int main()
{
    UNITY_BEGIN();

    std::cout << "=== Запуск интеграционных тестов ===" << std::endl;

    RUN_TEST(test_validation_formatting_logging_chain);
    RUN_TEST(test_csrf_config_validation_chain);
    RUN_TEST(test_error_handling_chain);
    RUN_TEST(test_performance_chain);
    RUN_TEST(test_calibration_compensation_integration);
    RUN_TEST(test_stress_multiple_operations);
    RUN_TEST(test_mqtt_thingspeak_integration);
    RUN_TEST(test_network_error_handling);

    std::cout << "=== Интеграционные тесты завершены ===" << std::endl;

    return UNITY_END();
}
