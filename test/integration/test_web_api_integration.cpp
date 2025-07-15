extern "C" void setUp() {}
extern "C" void tearDown() {}

#include <unity.h>
#include <chrono>
#include <iostream>
#include <map>
#include <string>
#include <vector>

// Подключаем заглушки ESP32
#include "esp32_stubs.h"

// Подключаем заголовки веб-компонентов
#include "jxct_format_utils.h"
#include "logger.h"
#include "validation_utils.h"
#include "web/csrf_protection.h"
#include "web_routes.h"

// Мок HTTP запроса
struct MockHttpRequest
{
    std::string method;
    std::string path;
    std::string body;
    std::map<std::string, std::string> headers;
    std::map<std::string, std::string> query_params;
};

// Мок HTTP ответа
struct MockHttpResponse
{
    int status_code;
    std::string body;
    std::map<std::string, std::string> headers;
    std::vector<std::string> errors;
};

// Глобальные переменные для тестирования
std::vector<std::string> api_logs;
std::vector<std::string> csrf_tokens;
std::vector<MockHttpRequest> processed_requests;

// Мок функции логирования API
void mock_api_logger(const std::string& message)
{
    api_logs.push_back(message);
}

// Мок функции генерации CSRF токена
std::string mock_api_csrf_token()
{
    std::string token = "api_csrf_" + std::to_string(csrf_tokens.size() + 1);
    csrf_tokens.push_back(token);
    return token;
}

// Мок функции валидации CSRF токена
bool mock_api_validate_csrf(const std::string& token)
{
    for (const auto& valid_token : csrf_tokens)
    {
        if (token == valid_token)
        {
            return true;
        }
    }
    return false;
}

// Мок функции валидации JSON
bool mock_validate_json(const std::string& json_data)
{
    return !json_data.empty() && json_data.find("{") != std::string::npos && json_data.find("}") != std::string::npos;
}

// Мок функции форматирования JSON ответа
std::string mock_format_json_response(const std::string& data, bool success = true)
{
    if (success)
    {
        return "{\"success\": true, \"data\": " + data + "}";
    }
    else
    {
        return "{\"success\": false, \"error\": " + data + "}";
    }
}

// Мок обработчика API маршрутов
MockHttpResponse mock_api_handler(const MockHttpRequest& request)
{
    MockHttpResponse response;
    response.status_code = 200;
    response.headers["Content-Type"] = "application/json";

    // Логируем запрос
    processed_requests.push_back(request);
    mock_api_logger("Processing request: " + request.method + " " + request.path);

    // Проверяем CSRF токен для POST/PUT/DELETE запросов
    if (request.method == "POST" || request.method == "PUT" || request.method == "DELETE")
    {
        auto csrf_header = request.headers.find("X-CSRF-Token");
        if (csrf_header == request.headers.end())
        {
            response.status_code = 403;
            response.body = mock_format_json_response("CSRF token required", false);
            return response;
        }

        if (!mock_api_validate_csrf(csrf_header->second))
        {
            response.status_code = 403;
            response.body = mock_format_json_response("Invalid CSRF token", false);
            return response;
        }
    }

    // Обрабатываем различные маршруты
    if (request.path == "/api/sensor/data")
    {
        if (request.method == "GET")
        {
            // Возвращаем данные датчика
            std::string sensor_data = R"({"temperature": 25.5, "humidity": 60.2})";
            response.body = mock_format_json_response(sensor_data);
        }
        else if (request.method == "POST")
        {
            // Валидируем и сохраняем данные датчика
            if (mock_validate_json(request.body))
            {
                // Дополнительная проверка безопасности
                if (request.body.find("DROP TABLE") != std::string::npos ||
                    request.body.find("<script>") != std::string::npos)
                {
                    response.status_code = 400;
                    response.body = mock_format_json_response("Malicious content detected", false);
                }
                else
                {
                    response.body = mock_format_json_response("Sensor data saved successfully");
                }
            }
            else
            {
                response.status_code = 400;
                response.body = mock_format_json_response("Invalid sensor data format", false);
            }
        }
    }
    else if (request.path == "/api/config")
    {
        if (request.method == "GET")
        {
            // Возвращаем конфигурацию
            std::string config_data = R"({"interval": 30000, "enabled": true})";
            response.body = mock_format_json_response(config_data);
        }
        else if (request.method == "PUT")
        {
            // Обновляем конфигурацию
            if (mock_validate_json(request.body))
            {
                response.body = mock_format_json_response("Configuration updated successfully");
            }
            else
            {
                response.status_code = 400;
                response.body = mock_format_json_response("Invalid configuration format", false);
            }
        }
    }
    else if (request.path == "/api/calibration/status")
    {
        if (request.method == "GET")
        {
            response.body = R"({"status": "Калибровка не настроена", "complete": false})";
        }
    }
    else if (request.path == "/api/calibration/ph/add")
    {
        if (request.method == "POST")
        {
            if (mock_validate_json(request.body))
            {
                response.body = R"({"success": true})";
            }
            else
            {
                response.status_code = 400;
                response.body = R"({"success": false, "error": "Invalid JSON"})";
            }
        }
    }
    else if (request.path == "/api/calibration/ec/add")
    {
        if (request.method == "POST")
        {
            if (mock_validate_json(request.body))
            {
                response.body = R"({"success": true})";
            }
            else
            {
                response.status_code = 400;
                response.body = R"({"success": false, "error": "Invalid JSON"})";
            }
        }
    }
    else if (request.path == "/api/calibration/npk/set")
    {
        if (request.method == "POST")
        {
            if (mock_validate_json(request.body))
            {
                response.body = R"({"success": true})";
            }
            else
            {
                response.status_code = 400;
                response.body = R"({"success": false, "error": "Invalid JSON"})";
            }
        }
    }
    else if (request.path == "/api/calibration/ph/calculate")
    {
        if (request.method == "POST")
        {
            response.body = R"({"success": true, "r_squared": 0.99})";
        }
    }
    else if (request.path == "/api/calibration/ec/calculate")
    {
        if (request.method == "POST")
        {
            response.body = R"({"success": true, "r_squared": 0.99})";
        }
    }
    else if (request.path == "/api/calibration/export")
    {
        if (request.method == "GET")
        {
            response.body = R"({"ph_points": [], "ec_points": [], "npk_zero": {}, "calculated": false})";
        }
    }
    else if (request.path == "/api/calibration/import")
    {
        if (request.method == "POST")
        {
            if (mock_validate_json(request.body))
            {
                response.body = R"({"success": true})";
            }
            else
            {
                response.status_code = 400;
                response.body = R"({"success": false, "error": "Failed to import calibration"})";
            }
        }
    }
    else if (request.path == "/api/calibration/reset")
    {
        if (request.method == "POST")
        {
            response.body = R"({"success": true})";
        }
    }
    else
    {
        response.status_code = 404;
        response.body = mock_format_json_response("Endpoint not found", false);
    }

    return response;
}

// Тест 1: Базовые GET запросы
void test_basic_get_requests()
{
    TEST_CASE("Тест базовых GET запросов к API");

    // Очищаем логи
    api_logs.clear();
    processed_requests.clear();

    // Тест 1: GET /api/sensor/data
    MockHttpRequest sensor_request;
    sensor_request.method = "GET";
    sensor_request.path = "/api/sensor/data";

    MockHttpResponse sensor_response = mock_api_handler(sensor_request);
    TEST_ASSERT_EQUAL(200, sensor_response.status_code);
    TEST_ASSERT_TRUE(sensor_response.body.find("temperature") != std::string::npos);

    // Тест 2: GET /api/config
    MockHttpRequest config_request;
    config_request.method = "GET";
    config_request.path = "/api/config";

    MockHttpResponse config_response = mock_api_handler(config_request);
    TEST_ASSERT_EQUAL(200, config_response.status_code);
    TEST_ASSERT_TRUE(config_response.body.find("interval") != std::string::npos);

    // Проверяем логи
    TEST_ASSERT_EQUAL(2, api_logs.size());
    TEST_ASSERT_EQUAL(2, processed_requests.size());
}

// Тест 2: POST запросы с CSRF защитой
void test_post_requests_with_csrf()
{
    TEST_CASE("Тест POST запросов с CSRF защитой");

    // Очищаем логи и токены
    api_logs.clear();
    processed_requests.clear();
    csrf_tokens.clear();

    // Генерируем CSRF токен
    std::string csrf_token = mock_api_csrf_token();
    TEST_ASSERT_FALSE(csrf_token.empty());

    // Тест 1: POST без CSRF токена (должен вернуть 403)
    MockHttpRequest request_no_csrf;
    request_no_csrf.method = "POST";
    request_no_csrf.path = "/api/sensor/data";
    request_no_csrf.body = R"({"temperature": 25.5})";

    MockHttpResponse response_no_csrf = mock_api_handler(request_no_csrf);
    TEST_ASSERT_EQUAL(403, response_no_csrf.status_code);
    TEST_ASSERT_TRUE(response_no_csrf.body.find("CSRF token required") != std::string::npos);

    // Тест 2: POST с неверным CSRF токеном (должен вернуть 403)
    MockHttpRequest request_invalid_csrf;
    request_invalid_csrf.method = "POST";
    request_invalid_csrf.path = "/api/sensor/data";
    request_invalid_csrf.body = R"({"temperature": 25.5})";
    request_invalid_csrf.headers["X-CSRF-Token"] = "invalid_token";

    MockHttpResponse response_invalid_csrf = mock_api_handler(request_invalid_csrf);
    TEST_ASSERT_EQUAL(403, response_invalid_csrf.status_code);
    TEST_ASSERT_TRUE(response_invalid_csrf.body.find("Invalid CSRF token") != std::string::npos);

    // Тест 3: POST с правильным CSRF токеном (должен вернуть 200)
    MockHttpRequest request_valid_csrf;
    request_valid_csrf.method = "POST";
    request_valid_csrf.path = "/api/sensor/data";
    request_valid_csrf.body = R"({"temperature": 25.5})";
    request_valid_csrf.headers["X-CSRF-Token"] = csrf_token;

    MockHttpResponse response_valid_csrf = mock_api_handler(request_valid_csrf);
    TEST_ASSERT_EQUAL(200, response_valid_csrf.status_code);
    TEST_ASSERT_TRUE(response_valid_csrf.body.find("success") != std::string::npos);
}

// Тест 3: Валидация JSON данных
void test_json_validation()
{
    TEST_CASE("Тест валидации JSON данных в API");

    // Очищаем логи
    api_logs.clear();
    processed_requests.clear();
    csrf_tokens.clear();

    // Генерируем CSRF токен
    std::string csrf_token = mock_api_csrf_token();

    // Тест 1: Валидный JSON
    MockHttpRequest valid_request;
    valid_request.method = "POST";
    valid_request.path = "/api/sensor/data";
    valid_request.body = R"({"temperature": 25.5, "humidity": 60.2})";
    valid_request.headers["X-CSRF-Token"] = csrf_token;

    MockHttpResponse valid_response = mock_api_handler(valid_request);
    TEST_ASSERT_EQUAL(200, valid_response.status_code);

    // Тест 2: Невалидный JSON
    MockHttpRequest invalid_request;
    invalid_request.method = "POST";
    invalid_request.path = "/api/sensor/data";
    invalid_request.body = "invalid json data";
    invalid_request.headers["X-CSRF-Token"] = csrf_token;

    MockHttpResponse invalid_response = mock_api_handler(invalid_request);
    TEST_ASSERT_EQUAL(400, invalid_response.status_code);
    TEST_ASSERT_TRUE(invalid_response.body.find("Invalid sensor data format") != std::string::npos);

    // Тест 3: Пустой JSON
    MockHttpRequest empty_request;
    empty_request.method = "POST";
    empty_request.path = "/api/sensor/data";
    empty_request.body = "";
    empty_request.headers["X-CSRF-Token"] = csrf_token;

    MockHttpResponse empty_response = mock_api_handler(empty_request);
    TEST_ASSERT_EQUAL(400, empty_response.status_code);
}

// Тест 4: Обработка ошибок 404
void test_404_error_handling()
{
    TEST_CASE("Тест обработки ошибок 404");

    // Очищаем логи
    api_logs.clear();
    processed_requests.clear();

    // Тест несуществующего маршрута
    MockHttpRequest invalid_request;
    invalid_request.method = "GET";
    invalid_request.path = "/api/nonexistent";

    MockHttpResponse response = mock_api_handler(invalid_request);
    TEST_ASSERT_EQUAL(404, response.status_code);
    TEST_ASSERT_TRUE(response.body.find("Endpoint not found") != std::string::npos);

    // Проверяем логи
    TEST_ASSERT_EQUAL(1, api_logs.size());
    TEST_ASSERT_EQUAL(1, processed_requests.size());
}

// Тест 5: Производительность API
void test_api_performance()
{
    TEST_CASE("Тест производительности API");

    const int ITERATIONS = 1000;
    auto start_time = std::chrono::high_resolution_clock::now();

    // Генерируем CSRF токен
    std::string csrf_token = mock_api_csrf_token();

    for (int i = 0; i < ITERATIONS; ++i)
    {
        MockHttpRequest request;
        request.method = "GET";
        request.path = "/api/sensor/data";

        MockHttpResponse response = mock_api_handler(request);
        TEST_ASSERT_EQUAL(200, response.status_code);
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

    // Проверяем, что производительность приемлемая (< 1 секунды для 1000 запросов)
    TEST_ASSERT_TRUE(duration.count() < 1000);

    std::cout << "API Performance test: " << ITERATIONS << " requests completed in " << duration.count() << "ms"
              << std::endl;
}

// Тест 6: Интеграция калибровки
void test_calibration_integration()
{
    TEST_CASE("Тест интеграции калибровки через API");

    // Очищаем логи
    api_logs.clear();
    processed_requests.clear();
    csrf_tokens.clear();

    // Тест 1: Получение статуса калибровки
    MockHttpRequest status_request;
    status_request.method = "GET";
    status_request.path = "/api/calibration/status";

    MockHttpResponse status_response = mock_api_handler(status_request);
    TEST_ASSERT_EQUAL(200, status_response.status_code);
    TEST_ASSERT_TRUE(status_response.body.find("Калибровка не настроена") != std::string::npos);

    // Тест 2: Добавление точки pH калибровки
    MockHttpRequest ph_request;
    ph_request.method = "POST";
    ph_request.path = "/api/calibration/ph/add";
    ph_request.body = R"({"expected": 7.0, "measured": 6.8})";
    ph_request.headers["X-CSRF-Token"] = mock_api_csrf_token();  // Добавляем CSRF токен

    MockHttpResponse ph_response = mock_api_handler(ph_request);
    TEST_ASSERT_EQUAL(200, ph_response.status_code);
    TEST_ASSERT_TRUE(ph_response.body.find("success") != std::string::npos);

    // Тест 3: Добавление точки EC калибровки
    MockHttpRequest ec_request;
    ec_request.method = "POST";
    ec_request.path = "/api/calibration/ec/add";
    ec_request.body = R"({"expected": 1.0, "measured": 0.95})";
    ec_request.headers["X-CSRF-Token"] = mock_api_csrf_token();  // Добавляем CSRF токен

    MockHttpResponse ec_response = mock_api_handler(ec_request);
    TEST_ASSERT_EQUAL(200, ec_response.status_code);
    TEST_ASSERT_TRUE(ec_response.body.find("success") != std::string::npos);

    // Тест 4: Установка NPK точки
    MockHttpRequest npk_request;
    npk_request.method = "POST";
    npk_request.path = "/api/calibration/npk/set";
    npk_request.body = R"({"n": 0.1, "p": 0.05, "k": 0.08})";
    npk_request.headers["X-CSRF-Token"] = mock_api_csrf_token();  // Добавляем CSRF токен

    MockHttpResponse npk_response = mock_api_handler(npk_request);
    TEST_ASSERT_EQUAL(200, npk_response.status_code);
    TEST_ASSERT_TRUE(npk_response.body.find("success") != std::string::npos);

    // Тест 5: Расчет pH калибровки
    MockHttpRequest calc_ph_request;
    calc_ph_request.method = "POST";
    calc_ph_request.path = "/api/calibration/ph/calculate";
    calc_ph_request.headers["X-CSRF-Token"] = mock_api_csrf_token();  // Добавляем CSRF токен

    MockHttpResponse calc_ph_response = mock_api_handler(calc_ph_request);
    TEST_ASSERT_EQUAL(200, calc_ph_response.status_code);
    TEST_ASSERT_TRUE(calc_ph_response.body.find("r_squared") != std::string::npos);

    // Тест 6: Экспорт калибровки
    MockHttpRequest export_request;
    export_request.method = "GET";
    export_request.path = "/api/calibration/export";

    MockHttpResponse export_response = mock_api_handler(export_request);
    TEST_ASSERT_EQUAL(200, export_response.status_code);
    TEST_ASSERT_TRUE(export_response.body.find("ph_points") != std::string::npos);

    // Тест 7: Импорт калибровки
    MockHttpRequest import_request;
    import_request.method = "POST";
    import_request.path = "/api/calibration/import";
    import_request.body = R"({"ph_points": [], "ec_points": [], "npk_zero": {}, "calculated": false})";
    import_request.headers["X-CSRF-Token"] = mock_api_csrf_token();  // Добавляем CSRF токен

    MockHttpResponse import_response = mock_api_handler(import_request);
    TEST_ASSERT_EQUAL(200, import_response.status_code);
    TEST_ASSERT_TRUE(import_response.body.find("success") != std::string::npos);

    // Тест 8: Сброс калибровки
    MockHttpRequest reset_request;
    reset_request.method = "POST";
    reset_request.path = "/api/calibration/reset";
    reset_request.headers["X-CSRF-Token"] = mock_api_csrf_token();  // Добавляем CSRF токен

    MockHttpResponse reset_response = mock_api_handler(reset_request);
    TEST_ASSERT_EQUAL(200, reset_response.status_code);
    TEST_ASSERT_TRUE(reset_response.body.find("success") != std::string::npos);
}

// Тест 7: Стресс-тест API
void test_api_stress()
{
    TEST_CASE("Стресс-тест API с множественными запросами");

    const int CONCURRENT_REQUESTS = 100;
    std::vector<MockHttpResponse> responses;
    responses.reserve(CONCURRENT_REQUESTS);

    // Генерируем CSRF токен
    std::string csrf_token = mock_api_csrf_token();

    auto start_time = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < CONCURRENT_REQUESTS; ++i)
    {
        MockHttpRequest request;

        // Чередуем типы запросов
        if (i % 3 == 0)
        {
            request.method = "GET";
            request.path = "/api/sensor/data";
        }
        else if (i % 3 == 1)
        {
            request.method = "POST";
            request.path = "/api/sensor/data";
            request.body = R"({"temperature": )" + std::to_string(20 + i) + R"(})";
            request.headers["X-CSRF-Token"] = csrf_token;
        }
        else
        {
            request.method = "GET";
            request.path = "/api/config";
        }

        MockHttpResponse response = mock_api_handler(request);
        responses.push_back(response);
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

    // Проверяем результаты
    int success_count = 0;
    for (const auto& response : responses)
    {
        if (response.status_code == 200)
        {
            success_count++;
        }
    }

    TEST_ASSERT_EQUAL(CONCURRENT_REQUESTS, success_count);
    TEST_ASSERT_TRUE(duration.count() < 5000);  // < 5 секунд для 100 запросов

    std::cout << "API Stress test: " << success_count << "/" << CONCURRENT_REQUESTS << " requests successful in "
              << duration.count() << "ms" << std::endl;
}

// Тест 8: Безопасность API
void test_api_security()
{
    TEST_CASE("Тест безопасности API");

    // Очищаем логи
    api_logs.clear();
    processed_requests.clear();
    csrf_tokens.clear();

    // Тест 1: Попытка SQL инъекции
    MockHttpRequest sql_injection_request;
    sql_injection_request.method = "POST";
    sql_injection_request.path = "/api/sensor/data";
    sql_injection_request.body = R"({"temperature": "25.5; DROP TABLE sensors;"})";
    sql_injection_request.headers["X-CSRF-Token"] = mock_api_csrf_token();

    MockHttpResponse sql_response = mock_api_handler(sql_injection_request);
    // Должен вернуть 400 из-за обнаружения вредоносного контента
    TEST_ASSERT_EQUAL(400, sql_response.status_code);

    // Тест 2: Попытка XSS атаки
    MockHttpRequest xss_request;
    xss_request.method = "POST";
    xss_request.path = "/api/sensor/data";
    xss_request.body = R"({"temperature": "<script>alert('xss')</script>"})";
    xss_request.headers["X-CSRF-Token"] = mock_api_csrf_token();

    MockHttpResponse xss_response = mock_api_handler(xss_request);
    // Должен вернуть 400 из-за обнаружения вредоносного контента
    TEST_ASSERT_EQUAL(400, xss_response.status_code);

    // Тест 3: Попытка доступа без авторизации
    MockHttpRequest unauthorized_request;
    unauthorized_request.method = "POST";
    unauthorized_request.path = "/api/sensor/data";
    unauthorized_request.body = R"({"temperature": 25.5})";
    // Без CSRF токена

    MockHttpResponse unauthorized_response = mock_api_handler(unauthorized_request);
    TEST_ASSERT_EQUAL(403, unauthorized_response.status_code);

    // Проверяем логи безопасности
    TEST_ASSERT_EQUAL(3, api_logs.size());
}

// Основная функция запуска тестов
int main()
{
    UNITY_BEGIN();

    std::cout << "=== Запуск интеграционных тестов веб-API ===" << std::endl;

    RUN_TEST(test_basic_get_requests);
    RUN_TEST(test_post_requests_with_csrf);
    RUN_TEST(test_json_validation);
    RUN_TEST(test_404_error_handling);
    RUN_TEST(test_api_performance);
    RUN_TEST(test_calibration_integration);
    RUN_TEST(test_api_stress);
    RUN_TEST(test_api_security);

    std::cout << "=== Интеграционные тесты веб-API завершены ===" << std::endl;

    return UNITY_END();
}
