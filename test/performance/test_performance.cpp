#include <unity.h>
#include <algorithm>
#include <chrono>
#include <future>
#include <iostream>
#include <random>
#include <string>
#include <thread>
#include <vector>

// Подключаем заголовки для тестирования
#include "csrf_protection.h"
#include "jxct_format_utils.h"
#include "logger.h"
#include "sensor_compensation.h"
#include "validation_utils.h"

// Константы для тестирования
const int SMALL_DATASET = 100;
const int MEDIUM_DATASET = 1000;
const int LARGE_DATASET = 10000;
const int PERFORMANCE_ITERATIONS = 1000;

// Глобальные переменные для тестирования
std::vector<std::string> performance_logs;
std::vector<double> execution_times;

// Генератор случайных данных
class DataGenerator
{
   private:
    std::random_device rd;
    std::mt19937 gen;
    std::uniform_real_distribution<float> temp_dist;
    std::uniform_real_distribution<float> humidity_dist;
    std::uniform_real_distribution<float> ph_dist;

   public:
    DataGenerator() : gen(rd()), temp_dist(-50.0f, 100.0f), humidity_dist(0.0f, 100.0f), ph_dist(0.0f, 14.0f) {}

    std::string generate_sensor_data()
    {
        float temp = temp_dist(gen);
        float humidity = humidity_dist(gen);
        float ph = ph_dist(gen);

        return R"({
            "temperature": )" +
               std::to_string(temp) + R"(,
            "humidity": )" +
               std::to_string(humidity) + R"(,
            "ph": )" +
               std::to_string(ph) + R"(,
            "nitrogen": )" +
               std::to_string(gen() % 100) + R"(,
            "phosphorus": )" +
               std::to_string(gen() % 100) + R"(,
            "potassium": )" +
               std::to_string(gen() % 100) + R"(,
            "timestamp": )" +
               std::to_string(std::time(nullptr)) + R"(
        })";
    }

    std::string generate_large_json(int size)
    {
        std::string json = "{";
        for (int i = 0; i < size; ++i)
        {
            if (i > 0) json += ",";
            json += "\"field" + std::to_string(i) + "\": " + std::to_string(gen() % 1000);
        }
        json += "}";
        return json;
    }
};

// Мок функции валидации
bool mock_validate_sensor_data(const std::string& data)
{
    // Симуляция работы валидации
    if (data.empty()) return false;
    if (data.find("temperature") == std::string::npos) return false;
    if (data.find("humidity") == std::string::npos) return false;

    // Имитация задержки обработки
    std::this_thread::sleep_for(std::chrono::microseconds(100));
    return true;
}

// Мок функции форматирования
std::string mock_format_json(const std::string& data)
{
    // Симуляция работы форматирования
    if (data.empty()) return "";

    // Имитация задержки обработки
    std::this_thread::sleep_for(std::chrono::microseconds(50));

    return "{\"formatted\": true, \"data\": " + data + "}";
}

// Мок функции CSRF защиты
std::string mock_generate_csrf_token()
{
    // Имитация задержки генерации
    std::this_thread::sleep_for(std::chrono::microseconds(10));
    return "csrf_token_" + std::to_string(std::time(nullptr));
}

bool mock_validate_csrf_token(const std::string& token)
{
    // Имитация задержки валидации
    std::this_thread::sleep_for(std::chrono::microseconds(5));
    return !token.empty() && token.find("csrf_token_") != std::string::npos;
}

// Мок функции компенсации
std::string mock_apply_compensation(const std::string& data)
{
    // Имитация задержки компенсации
    std::this_thread::sleep_for(std::chrono::microseconds(200));
    return "{\"compensated\": true, \"original\": " + data + "}";
}

// Утилиты для измерения производительности
class PerformanceTimer
{
   private:
    std::chrono::high_resolution_clock::time_point start_time;

   public:
    void start()
    {
        start_time = std::chrono::high_resolution_clock::now();
    }

    double stop()
    {
        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
        return duration.count() / 1000.0;  // Возвращаем в миллисекундах
    }
};

// Тест 1: Производительность валидации данных
void test_validation_performance()
{
    TEST_CASE("Тест производительности валидации данных");

    DataGenerator generator;
    PerformanceTimer timer;
    std::vector<double> times;

    // Генерируем тестовые данные
    std::vector<std::string> test_data;
    for (int i = 0; i < PERFORMANCE_ITERATIONS; ++i)
    {
        test_data.push_back(generator.generate_sensor_data());
    }

    // Тестируем производительность валидации
    timer.start();
    for (const auto& data : test_data)
    {
        bool result = mock_validate_sensor_data(data);
        TEST_ASSERT_TRUE(result);
    }
    double total_time = timer.stop();

    double avg_time = total_time / PERFORMANCE_ITERATIONS;
    times.push_back(avg_time);

    std::cout << "Validation Performance:" << std::endl;
    std::cout << "  Total time: " << total_time << "ms" << std::endl;
    std::cout << "  Average time per validation: " << avg_time << "ms" << std::endl;
    std::cout << "  Validations per second: " << (1000.0 / avg_time) << std::endl;

    // Проверяем, что производительность приемлемая (< 1ms на валидацию)
    TEST_ASSERT_TRUE(avg_time < 1.0);
}

// Тест 2: Производительность форматирования JSON
void test_json_formatting_performance()
{
    TEST_CASE("Тест производительности форматирования JSON");

    DataGenerator generator;
    PerformanceTimer timer;
    std::vector<double> times;

    // Тестируем разные размеры данных
    std::vector<int> sizes = {SMALL_DATASET, MEDIUM_DATASET, LARGE_DATASET};

    for (int size : sizes)
    {
        std::vector<std::string> test_data;
        for (int i = 0; i < 100; ++i)
        {  // 100 итераций для каждого размера
            test_data.push_back(generator.generate_large_json(size));
        }

        timer.start();
        for (const auto& data : test_data)
        {
            std::string result = mock_format_json(data);
            TEST_ASSERT_FALSE(result.empty());
        }
        double total_time = timer.stop();

        double avg_time = total_time / 100.0;
        times.push_back(avg_time);

        std::cout << "JSON Formatting Performance (" << size << " fields):" << std::endl;
        std::cout << "  Average time per format: " << avg_time << "ms" << std::endl;
        std::cout << "  Formats per second: " << (1000.0 / avg_time) << std::endl;

        // Проверяем, что производительность приемлемая
        TEST_ASSERT_TRUE(avg_time < 10.0);  // < 10ms на форматирование
    }
}

// Тест 3: Производительность CSRF операций
void test_csrf_performance()
{
    TEST_CASE("Тест производительности CSRF операций");

    PerformanceTimer timer;
    std::vector<std::string> tokens;

    // Тест генерации токенов
    timer.start();
    for (int i = 0; i < PERFORMANCE_ITERATIONS; ++i)
    {
        std::string token = mock_generate_csrf_token();
        tokens.push_back(token);
        TEST_ASSERT_FALSE(token.empty());
    }
    double generation_time = timer.stop();

    // Тест валидации токенов
    timer.start();
    for (const auto& token : tokens)
    {
        bool result = mock_validate_csrf_token(token);
        TEST_ASSERT_TRUE(result);
    }
    double validation_time = timer.stop();

    double avg_generation = generation_time / PERFORMANCE_ITERATIONS;
    double avg_validation = validation_time / PERFORMANCE_ITERATIONS;

    std::cout << "CSRF Performance:" << std::endl;
    std::cout << "  Token generation: " << avg_generation << "ms per token" << std::endl;
    std::cout << "  Token validation: " << avg_validation << "ms per token" << std::endl;
    std::cout << "  Generations per second: " << (1000.0 / avg_generation) << std::endl;
    std::cout << "  Validations per second: " << (1000.0 / avg_validation) << std::endl;

    // Проверяем производительность
    TEST_ASSERT_TRUE(avg_generation < 0.1);  // < 0.1ms на генерацию
    TEST_ASSERT_TRUE(avg_validation < 0.1);  // < 0.1ms на валидацию
}

// Тест 4: Производительность компенсации данных
void test_compensation_performance()
{
    TEST_CASE("Тест производительности компенсации данных");

    DataGenerator generator;
    PerformanceTimer timer;

    // Генерируем тестовые данные
    std::vector<std::string> test_data;
    for (int i = 0; i < 100; ++i)
    {  // Меньше итераций из-за большей задержки
        test_data.push_back(generator.generate_sensor_data());
    }

    timer.start();
    for (const auto& data : test_data)
    {
        std::string result = mock_apply_compensation(data);
        TEST_ASSERT_FALSE(result.empty());
        TEST_ASSERT_TRUE(result.find("compensated") != std::string::npos);
    }
    double total_time = timer.stop();

    double avg_time = total_time / 100.0;

    std::cout << "Compensation Performance:" << std::endl;
    std::cout << "  Average time per compensation: " << avg_time << "ms" << std::endl;
    std::cout << "  Compensations per second: " << (1000.0 / avg_time) << std::endl;

    // Проверяем производительность (компенсация может быть медленнее)
    TEST_ASSERT_TRUE(avg_time < 5.0);  // < 5ms на компенсацию
}

// Тест 5: Производительность полного пайплайна
void test_full_pipeline_performance()
{
    TEST_CASE("Тест производительности полного пайплайна обработки");

    DataGenerator generator;
    PerformanceTimer timer;

    // Генерируем тестовые данные
    std::vector<std::string> test_data;
    for (int i = 0; i < 100; ++i)
    {
        test_data.push_back(generator.generate_sensor_data());
    }

    timer.start();
    for (const auto& data : test_data)
    {
        // Полный пайплайн: валидация -> форматирование -> компенсация -> CSRF
        bool valid = mock_validate_sensor_data(data);
        TEST_ASSERT_TRUE(valid);

        std::string formatted = mock_format_json(data);
        TEST_ASSERT_FALSE(formatted.empty());

        std::string compensated = mock_apply_compensation(formatted);
        TEST_ASSERT_FALSE(compensated.empty());

        std::string csrf_token = mock_generate_csrf_token();
        bool csrf_valid = mock_validate_csrf_token(csrf_token);
        TEST_ASSERT_TRUE(csrf_valid);
    }
    double total_time = timer.stop();

    double avg_time = total_time / 100.0;

    std::cout << "Full Pipeline Performance:" << std::endl;
    std::cout << "  Average time per pipeline: " << avg_time << "ms" << std::endl;
    std::cout << "  Pipelines per second: " << (1000.0 / avg_time) << std::endl;

    // Проверяем производительность полного пайплайна
    TEST_ASSERT_TRUE(avg_time < 10.0);  // < 10ms на полный пайплайн
}

// Тест 6: Многопоточная производительность
void test_multithreaded_performance()
{
    TEST_CASE("Тест многопоточной производительности");

    DataGenerator generator;
    const int THREAD_COUNT = 4;
    const int OPERATIONS_PER_THREAD = 250;

    auto worker_function = [&generator](int thread_id)
    {
        std::vector<double> thread_times;

        for (int i = 0; i < OPERATIONS_PER_THREAD; ++i)
        {
            PerformanceTimer timer;
            timer.start();

            // Выполняем операции
            std::string data = generator.generate_sensor_data();
            bool valid = mock_validate_sensor_data(data);
            std::string formatted = mock_format_json(data);
            std::string compensated = mock_apply_compensation(formatted);
            std::string token = mock_generate_csrf_token();
            bool csrf_valid = mock_validate_csrf_token(token);

            double time = timer.stop();
            thread_times.push_back(time);

            TEST_ASSERT_TRUE(valid);
            TEST_ASSERT_FALSE(formatted.empty());
            TEST_ASSERT_FALSE(compensated.empty());
            TEST_ASSERT_TRUE(csrf_valid);
        }

        return thread_times;
    };

    // Запускаем потоки
    std::vector<std::future<std::vector<double>>> futures;
    auto start_time = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < THREAD_COUNT; ++i)
    {
        futures.push_back(std::async(std::launch::async, worker_function, i));
    }

    // Собираем результаты
    std::vector<double> all_times;
    for (auto& future : futures)
    {
        auto thread_times = future.get();
        all_times.insert(all_times.end(), thread_times.begin(), thread_times.end());
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    auto total_duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

    // Анализируем результаты
    double total_time = total_duration.count();
    double avg_time = std::accumulate(all_times.begin(), all_times.end(), 0.0) / all_times.size();

    std::cout << "Multithreaded Performance (" << THREAD_COUNT << " threads):" << std::endl;
    std::cout << "  Total operations: " << (THREAD_COUNT * OPERATIONS_PER_THREAD) << std::endl;
    std::cout << "  Total time: " << total_time << "ms" << std::endl;
    std::cout << "  Average time per operation: " << avg_time << "ms" << std::endl;
    std::cout << "  Operations per second: " << ((THREAD_COUNT * OPERATIONS_PER_THREAD * 1000.0) / total_time)
              << std::endl;

    // Проверяем производительность
    TEST_ASSERT_TRUE(avg_time < 5.0);  // < 5ms на операцию в многопоточном режиме
    TEST_ASSERT_EQUAL(THREAD_COUNT * OPERATIONS_PER_THREAD, all_times.size());
}

// Тест 7: Производительность при большой нагрузке
void test_high_load_performance()
{
    TEST_CASE("Тест производительности при большой нагрузке");

    DataGenerator generator;
    const int HIGH_LOAD_ITERATIONS = 5000;

    PerformanceTimer timer;
    std::vector<double> times;
    times.reserve(HIGH_LOAD_ITERATIONS);

    timer.start();
    for (int i = 0; i < HIGH_LOAD_ITERATIONS; ++i)
    {
        PerformanceTimer op_timer;
        op_timer.start();

        // Быстрые операции
        std::string data = generator.generate_sensor_data();
        bool valid = mock_validate_sensor_data(data);
        std::string formatted = mock_format_json(data);
        std::string token = mock_generate_csrf_token();
        bool csrf_valid = mock_validate_csrf_token(token);

        double op_time = op_timer.stop();
        times.push_back(op_time);

        TEST_ASSERT_TRUE(valid);
        TEST_ASSERT_FALSE(formatted.empty());
        TEST_ASSERT_TRUE(csrf_valid);
    }
    double total_time = timer.stop();

    // Анализируем производительность
    std::sort(times.begin(), times.end());
    double median_time = times[HIGH_LOAD_ITERATIONS / 2];
    double p95_time = times[static_cast<size_t>(HIGH_LOAD_ITERATIONS * 0.95)];
    double p99_time = times[static_cast<size_t>(HIGH_LOAD_ITERATIONS * 0.99)];

    std::cout << "High Load Performance:" << std::endl;
    std::cout << "  Total operations: " << HIGH_LOAD_ITERATIONS << std::endl;
    std::cout << "  Total time: " << total_time << "ms" << std::endl;
    std::cout << "  Median time: " << median_time << "ms" << std::endl;
    std::cout << "  95th percentile: " << p95_time << "ms" << std::endl;
    std::cout << "  99th percentile: " << p99_time << "ms" << std::endl;
    std::cout << "  Operations per second: " << ((HIGH_LOAD_ITERATIONS * 1000.0) / total_time) << std::endl;

    // Проверяем производительность
    TEST_ASSERT_TRUE(median_time < 2.0);  // Медиана < 2ms
    TEST_ASSERT_TRUE(p95_time < 5.0);     // 95% операций < 5ms
    TEST_ASSERT_TRUE(p99_time < 10.0);    // 99% операций < 10ms
}

// Тест 8: Производительность памяти
void test_memory_performance()
{
    TEST_CASE("Тест производительности памяти");

    DataGenerator generator;
    const int MEMORY_TEST_ITERATIONS = 1000;

    // Тест аллокации и освобождения памяти
    std::vector<std::string> large_strings;
    large_strings.reserve(MEMORY_TEST_ITERATIONS);

    PerformanceTimer timer;
    timer.start();

    for (int i = 0; i < MEMORY_TEST_ITERATIONS; ++i)
    {
        // Создаем большие строки
        std::string large_data = generator.generate_large_json(1000);
        large_strings.push_back(large_data);

        // Обрабатываем данные
        bool valid = mock_validate_sensor_data(large_data);
        std::string formatted = mock_format_json(large_data);

        TEST_ASSERT_TRUE(valid);
        TEST_ASSERT_FALSE(formatted.empty());
    }

    double allocation_time = timer.stop();

    // Освобождаем память
    timer.start();
    large_strings.clear();
    double deallocation_time = timer.stop();

    std::cout << "Memory Performance:" << std::endl;
    std::cout << "  Allocation time: " << allocation_time << "ms" << std::endl;
    std::cout << "  Deallocation time: " << deallocation_time << "ms" << std::endl;
    std::cout << "  Average allocation time: " << (allocation_time / MEMORY_TEST_ITERATIONS) << "ms" << std::endl;

    // Проверяем производительность памяти
    TEST_ASSERT_TRUE(allocation_time < 1000.0);   // < 1 секунды на аллокацию
    TEST_ASSERT_TRUE(deallocation_time < 100.0);  // < 100ms на освобождение
}

// Основная функция запуска тестов
int main()
{
    UNITY_BEGIN();

    std::cout << "=== Запуск тестов производительности ===" << std::endl;

    RUN_TEST(test_validation_performance);
    RUN_TEST(test_json_formatting_performance);
    RUN_TEST(test_csrf_performance);
    RUN_TEST(test_compensation_performance);
    RUN_TEST(test_full_pipeline_performance);
    RUN_TEST(test_multithreaded_performance);
    RUN_TEST(test_high_load_performance);
    RUN_TEST(test_memory_performance);

    std::cout << "=== Тесты производительности завершены ===" << std::endl;

    return UNITY_END();
}
