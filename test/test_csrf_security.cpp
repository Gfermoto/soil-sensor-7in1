#include <cstring>
#include <string>
#include "simple_csrf.h"
#include "unity_simple.h"

// Заглушки логирования (чтобы не подтягивать logger.cpp)
extern "C"
{
    void logSuccess(const char*, ...) {}
    void logError(const char*, ...) {}
    void logWarn(const char*, ...) {}
    void logDebug(const char*, ...) {}
    void logInfo(const char*, ...) {}
    void logSystem(const char*, ...) {}
}

// Заглушки для Arduino и FS
#include <cstdint>
#include <cstring>
#include <string>

// Заглушки для LittleFS
namespace
{
bool littleFS_initialized = false;
bool littleFS_exists_result = false;
}  // namespace

// Заглушка для File
class File
{
   public:
    bool available()
    {
        return false;
    }
    std::string readStringUntil(char delimiter)
    {
        return "";
    }
    void close() {}
    size_t size()
    {
        return 0;
    }
    size_t write(uint8_t byte)
    {
        return 1;
    }
};

// Заглушка для Stream
class Stream
{
   public:
    virtual int available()
    {
        return 0;
    }
    virtual int read()
    {
        return -1;
    }
    virtual size_t write(uint8_t)
    {
        return 1;
    }
};

// Заглушки для LittleFS
class LittleFSClass
{
   public:
    bool begin(bool formatOnFailure)
    {
        littleFS_initialized = true;
        return true;
    }

    bool exists(const char* path)
    {
        return littleFS_exists_result;
    }

    bool mkdir(const char* path)
    {
        return true;
    }

    File open(const char* path, const char* mode)
    {
        static File dummyFile;
        return dummyFile;
    }

    bool remove(const char* path)
    {
        return true;
    }
};

LittleFSClass LittleFS;

// Заглушки для Arduino функций
bool isDigit(char c)
{
    return c >= '0' && c <= '9';
}
int random(int min, int max)
{
    return min;
}
// Убираем millis() - он уже есть в Arduino.h

// Заглушка для FS.h
#define FS_H
namespace fs
{
class FS
{
   public:
    virtual bool begin(bool formatOnFailure = false) = 0;
    virtual bool exists(const char* path) = 0;
    virtual bool mkdir(const char* path) = 0;
    virtual File open(const char* path, const char* mode) = 0;
    virtual bool remove(const char* path) = 0;
};
}  // namespace fs

#include "jxct_constants.h"
#include "validation_utils.h"
// Подключаем реализацию напрямую (без зависимости от logger.cpp)
#include "../src/sensor_compensation.cpp"
#include "../src/validation_utils.cpp"
#include "jxct_format_utils.h"
#include "sensor_compensation.h"

void setUp(void)
{
    // Настройка перед каждым тестом
}

void tearDown(void)
{
    // Очистка после каждого теста
}

// ============================================================================
// БАЗОВЫЕ ТЕСТЫ CSRF
// ============================================================================

void test_csrf_token_generation(void)
{
    // Тест генерации CSRF токена
    std::string token = generateCSRFToken();

    // Проверяем, что токен не пустой
    TEST_ASSERT_NOT_EQUAL(0, token.length());

    // Проверяем, что токен имеет разумную длину
    TEST_ASSERT_GREATER_THAN(10, token.length());

    // Проверяем, что токен содержит только hex символы
    for (char c : token)
    {
        TEST_ASSERT_TRUE((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f'));
    }
}

void test_csrf_token_validation(void)
{
    // Тест валидации CSRF токена
    std::string token = generateCSRFToken();

    // Валидный токен должен пройти проверку
    TEST_ASSERT_TRUE(validateCSRFToken(token));

    // Невалидный токен должен не пройти проверку
    TEST_ASSERT_FALSE(validateCSRFToken("invalid_token"));
    TEST_ASSERT_FALSE(validateCSRFToken(""));
}

void test_csrf_token_uniqueness(void)
{
    // Тест уникальности токенов
    std::string token1 = generateCSRFToken();
    std::string token2 = generateCSRFToken();

    // Токены должны быть разными
    TEST_ASSERT_NOT_EQUAL_STRING(token1.c_str(), token2.c_str());
}

// ============================================================================
// РАСШИРЕННЫЕ ТЕСТЫ CSRF БЕЗОПАСНОСТИ
// ============================================================================

void test_csrf_token_length_consistency(void)
{
    // Тест консистентности длины токенов
    std::string token1 = generateCSRFToken();
    std::string token2 = generateCSRFToken();
    std::string token3 = generateCSRFToken();

    // Все токены должны иметь одинаковую длину
    TEST_ASSERT_EQUAL(token1.length(), token2.length());
    TEST_ASSERT_EQUAL(token2.length(), token3.length());

    // Длина должна быть разумной (32 символа для MD5)
    TEST_ASSERT_EQUAL(32, token1.length());
}

void test_csrf_token_format_validation(void)
{
    // Тест валидации формата токена
    std::string validToken = generateCSRFToken();

    // Проверяем различные невалидные форматы
    TEST_ASSERT_FALSE(validateCSRFToken("1234567890123456789012345678901"));    // 31 символ
    TEST_ASSERT_FALSE(validateCSRFToken("123456789012345678901234567890123"));  // 33 символа
    TEST_ASSERT_FALSE(validateCSRFToken("1234567890123456789012345678901g"));   // содержит 'g'
    TEST_ASSERT_FALSE(validateCSRFToken("1234567890123456789012345678901G"));   // содержит 'G'
    TEST_ASSERT_FALSE(validateCSRFToken("1234567890123456789012345678901@"));   // содержит '@'
    TEST_ASSERT_FALSE(validateCSRFToken("1234567890123456789012345678901 "));   // содержит пробел
}

void test_csrf_token_case_sensitivity(void)
{
    // Тест чувствительности к регистру
    std::string token = generateCSRFToken();

    // Создаем версию в верхнем регистре
    std::string upperToken = token;
    for (char& c : upperToken)
    {
        if (c >= 'a' && c <= 'f')
        {
            c = c - 'a' + 'A';
        }
    }

    // Верхний регистр должен быть невалидным
    TEST_ASSERT_FALSE(validateCSRFToken(upperToken));
}

void test_csrf_token_null_termination(void)
{
    // Тест обработки null-терминации
    std::string token = generateCSRFToken();

    // Добавляем null-символ в середину
    std::string corruptedToken = token.substr(0, 16) + '\0' + token.substr(16);

    TEST_ASSERT_FALSE(validateCSRFToken(corruptedToken));
}

void test_csrf_token_special_characters(void)
{
    // Тест обработки специальных символов
    TEST_ASSERT_FALSE(validateCSRFToken("1234567890123456789012345678901\n"));    // newline
    TEST_ASSERT_FALSE(validateCSRFToken("1234567890123456789012345678901\r"));    // carriage return
    TEST_ASSERT_FALSE(validateCSRFToken("1234567890123456789012345678901\t"));    // tab
    TEST_ASSERT_FALSE(validateCSRFToken("1234567890123456789012345678901\x00"));  // null byte
}

void test_csrf_token_unicode_handling(void)
{
    // Тест обработки Unicode символов
    TEST_ASSERT_FALSE(validateCSRFToken("1234567890123456789012345678901ф"));   // кириллица
    TEST_ASSERT_FALSE(validateCSRFToken("1234567890123456789012345678901α"));   // греческий
    TEST_ASSERT_FALSE(validateCSRFToken("1234567890123456789012345678901😀"));  // эмодзи
}

void test_csrf_token_replay_attack(void)
{
    // Тест на replay атаку
    std::string token1 = generateCSRFToken();
    std::string token2 = generateCSRFToken();

    // Оба токена должны быть валидными, но разными
    TEST_ASSERT_TRUE(validateCSRFToken(token1));
    TEST_ASSERT_TRUE(validateCSRFToken(token2));
    TEST_ASSERT_NOT_EQUAL_STRING(token1.c_str(), token2.c_str());
}

void test_csrf_token_entropy(void)
{
    // Тест энтропии токенов (базовая проверка)
    std::string token1 = generateCSRFToken();
    std::string token2 = generateCSRFToken();
    std::string token3 = generateCSRFToken();

    // Подсчитываем количество одинаковых символов в одинаковых позициях
    int sameChars = 0;
    for (size_t i = 0; i < token1.length(); i++)
    {
        if (token1[i] == token2[i] && token2[i] == token3[i])
        {
            sameChars++;
        }
    }

    // Не должно быть слишком много одинаковых символов в одинаковых позициях
    // (это указывало бы на слабую энтропию)
    TEST_ASSERT_LESS_THAN(8, sameChars);  // Максимум 25% одинаковых позиций
}

void test_csrf_token_boundary_conditions(void)
{
    // Тест граничных условий
    TEST_ASSERT_FALSE(validateCSRFToken(""));                                  // Пустая строка
    TEST_ASSERT_FALSE(validateCSRFToken("a"));                                 // Один символ
    TEST_ASSERT_FALSE(validateCSRFToken("123456789012345678901234567890"));    // 30 символов
    TEST_ASSERT_FALSE(validateCSRFToken("12345678901234567890123456789012"));  // 32 символа, но не hex
}

void test_csrf_token_memory_safety(void)
{
    // Тест безопасности памяти (базовая проверка)
    for (int i = 0; i < 100; i++)
    {
        std::string token = generateCSRFToken();
        TEST_ASSERT_TRUE(validateCSRFToken(token));
        TEST_ASSERT_EQUAL(32, token.length());
    }
}

// ============================================================================
// ТЕСТЫ ВАЛИДАЦИИ
// ============================================================================

void test_validate_ssid_empty(void)
{
    ValidationResult res = validateSSID("");
    TEST_ASSERT_FALSE(res.isValid);
}

void test_validate_ssid_too_long(void)
{
    String ssid(33, 'a');
    ValidationResult res = validateSSID(ssid);
    TEST_ASSERT_FALSE(res.isValid);
}

void test_validate_ssid_valid(void)
{
    ValidationResult res = validateSSID("my_wifi");
    TEST_ASSERT_TRUE(res.isValid);
}

void test_validate_password_short(void)
{
    ValidationResult res = validatePassword("short");
    TEST_ASSERT_FALSE(res.isValid);
}

void test_validate_password_long(void)
{
    String pwd(64, 'p');
    ValidationResult res = validatePassword(pwd);
    TEST_ASSERT_FALSE(res.isValid);
}

void test_validate_password_valid(void)
{
    ValidationResult res = validatePassword("supersecret");
    TEST_ASSERT_TRUE(res.isValid);
}

void test_validate_mqtt_port_low(void)
{
    ValidationResult res = validateMQTTPort(0);
    TEST_ASSERT_FALSE(res.isValid);
}

void test_validate_mqtt_port_high(void)
{
    ValidationResult res = validateMQTTPort(70000);
    TEST_ASSERT_FALSE(res.isValid);
}

void test_validate_mqtt_port_valid(void)
{
    ValidationResult res = validateMQTTPort(1883);
    TEST_ASSERT_TRUE(res.isValid);
}

void test_validate_temperature_low(void)
{
    ValidationResult res = validateTemperature(SENSOR_TEMP_MIN - 1);
    TEST_ASSERT_FALSE(res.isValid);
}

void test_validate_temperature_high(void)
{
    ValidationResult res = validateTemperature(SENSOR_TEMP_MAX + 1);
    TEST_ASSERT_FALSE(res.isValid);
}

void test_validate_temperature_valid(void)
{
    ValidationResult res = validateTemperature(25.0F);
    TEST_ASSERT_TRUE(res.isValid);
}

void test_validate_humidity_low(void)
{
    ValidationResult res = validateHumidity(SENSOR_HUMIDITY_MIN - 1);
    TEST_ASSERT_FALSE(res.isValid);
}

void test_validate_humidity_high(void)
{
    ValidationResult res = validateHumidity(SENSOR_HUMIDITY_MAX + 1);
    TEST_ASSERT_FALSE(res.isValid);
}

void test_validate_humidity_valid(void)
{
    ValidationResult res = validateHumidity(60.0F);
    TEST_ASSERT_TRUE(res.isValid);
}

void test_validate_ph_low(void)
{
    ValidationResult res = validatePH(SENSOR_PH_MIN - 0.1F);
    TEST_ASSERT_FALSE(res.isValid);
}

void test_validate_ph_high(void)
{
    ValidationResult res = validatePH(SENSOR_PH_MAX + 0.1F);
    TEST_ASSERT_FALSE(res.isValid);
}

void test_validate_ph_valid(void)
{
    ValidationResult res = validatePH(6.5F);
    TEST_ASSERT_TRUE(res.isValid);
}

// ============================================================================
// ТЕСТЫ ФОРМАТИРОВАНИЯ
// ============================================================================

void test_format_temperature(void)
{
    std::string s = format_temperature(23.456f);
    TEST_ASSERT_EQUAL_STRING("23.5", s.c_str());
}

void test_format_ec(void)
{
    std::string s = format_ec(1234.7f);
    TEST_ASSERT_EQUAL_STRING("1235", s.c_str());
}

void test_format_ph(void)
{
    std::string s = format_ph(6.78f);
    TEST_ASSERT_EQUAL_STRING("6.8", s.c_str());
}

void test_format_value_prec0(void)
{
    String s = formatValue(25.4f, "°C", 0);
    TEST_ASSERT_EQUAL_STRING("25°C", s.c_str());
}

void test_format_value_prec2(void)
{
    String s = formatValue(3.1415f, "pH", 2);
    TEST_ASSERT_EQUAL_STRING("3.14pH", s.c_str());
}

// ============================================================================
// ТЕСТЫ КОМПЕНСАЦИИ
// ============================================================================

void test_correct_ph_baseline(void)
{
    float res = correctPH(7.0f, 25.0f);
    TEST_ASSERT_FLOAT_WITHIN(1e-5f, 7.0f, res);
}

void test_correct_ph_hot(void)
{
    float res = correctPH(7.0f, 35.0f);
    TEST_ASSERT_FLOAT_WITHIN(1e-4f, 6.97f, res);
}

void test_correct_ec_saturated(void)
{
    float res = correctEC(1000.0f, 25.0f, 45.0f, SoilType::LOAM);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 1000.0f, res);
}

void test_correct_ec_low_moisture(void)
{
    float res = correctEC(1000.0f, 25.0f, 30.0f, SoilType::LOAM);
    TEST_ASSERT_GREATER_THAN(1000.0f, res);
}

void test_correct_npk_out_of_range(void)
{
    float N = 100, P = 100, K = 100;
    correctNPK(25.0f, 20.0f, N, P, K, SoilType::LOAM);
    TEST_ASSERT_FLOAT_WITHIN(1e-5f, 100.0f, N);
    TEST_ASSERT_FLOAT_WITHIN(1e-5f, 100.0f, P);
    TEST_ASSERT_FLOAT_WITHIN(1e-5f, 100.0f, K);
}

void test_correct_npk_increase(void)
{
    float N = 100, P = 100, K = 100;
    correctNPK(25.0f, 30.0f, N, P, K, SoilType::LOAM);
    TEST_ASSERT_GREATER_THAN(100.0f, N);
    TEST_ASSERT_GREATER_THAN(100.0f, P);
    TEST_ASSERT_GREATER_THAN(100.0f, K);
}

// ============================================================================
// ГЛАВНАЯ ФУНКЦИЯ
// ============================================================================

int main(void)
{
    UNITY_BEGIN();

    // Базовые тесты CSRF
    RUN_TEST(test_csrf_token_generation);
    RUN_TEST(test_csrf_token_validation);
    RUN_TEST(test_csrf_token_uniqueness);

    // Расширенные тесты CSRF безопасности
    RUN_TEST(test_csrf_token_length_consistency);
    RUN_TEST(test_csrf_token_format_validation);
    RUN_TEST(test_csrf_token_case_sensitivity);
    RUN_TEST(test_csrf_token_null_termination);
    RUN_TEST(test_csrf_token_special_characters);
    RUN_TEST(test_csrf_token_unicode_handling);
    RUN_TEST(test_csrf_token_replay_attack);
    RUN_TEST(test_csrf_token_entropy);
    RUN_TEST(test_csrf_token_boundary_conditions);
    RUN_TEST(test_csrf_token_memory_safety);

    // Тесты валидации
    RUN_TEST(test_validate_ssid_empty);
    RUN_TEST(test_validate_ssid_too_long);
    RUN_TEST(test_validate_ssid_valid);
    RUN_TEST(test_validate_password_short);
    RUN_TEST(test_validate_password_long);
    RUN_TEST(test_validate_password_valid);
    RUN_TEST(test_validate_mqtt_port_low);
    RUN_TEST(test_validate_mqtt_port_high);
    RUN_TEST(test_validate_mqtt_port_valid);
    RUN_TEST(test_validate_temperature_low);
    RUN_TEST(test_validate_temperature_high);
    RUN_TEST(test_validate_temperature_valid);
    RUN_TEST(test_validate_humidity_low);
    RUN_TEST(test_validate_humidity_high);
    RUN_TEST(test_validate_humidity_valid);
    RUN_TEST(test_validate_ph_low);
    RUN_TEST(test_validate_ph_high);
    RUN_TEST(test_validate_ph_valid);

    // Тесты форматирования
    RUN_TEST(test_format_temperature);
    RUN_TEST(test_format_ec);
    RUN_TEST(test_format_ph);
    RUN_TEST(test_format_value_prec0);
    RUN_TEST(test_format_value_prec2);

    // Тесты компенсации
    RUN_TEST(test_correct_ph_baseline);
    RUN_TEST(test_correct_ph_hot);
    RUN_TEST(test_correct_ec_saturated);
    RUN_TEST(test_correct_ec_low_moisture);
    RUN_TEST(test_correct_npk_out_of_range);
    RUN_TEST(test_correct_npk_increase);

    return UNITY_END();
}
