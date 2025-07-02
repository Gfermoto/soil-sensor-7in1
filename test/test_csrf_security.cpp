#include "unity_simple.h"
#include "simple_csrf.h"
#include <string>
#include <cstring>

// Заглушки логирования (чтобы не подтягивать logger.cpp)
extern "C" {
void logSuccess(const char*, ...) {}
void logError(const char*, ...) {}
void logWarn(const char*, ...) {}
void logDebug(const char*, ...) {}
void logInfo(const char*, ...) {}
void logSystem(const char*, ...) {}
}

#include "validation_utils.h"
#include "jxct_constants.h"
// Подключаем реализацию напрямую (без зависимости от logger.cpp)
#include "../src/validation_utils.cpp"
#include "jxct_format_utils.h"
#include "sensor_compensation.h"
#include "../src/sensor_compensation.cpp"

void setUp(void) {
    // Настройка перед каждым тестом
}

void tearDown(void) {
    // Очистка после каждого теста
}

void test_csrf_token_generation(void) {
    // Тест генерации CSRF токена
    std::string token = generateCSRFToken();
    
    // Проверяем, что токен не пустой
    TEST_ASSERT_NOT_EQUAL(0, token.length());
    
    // Проверяем, что токен имеет разумную длину
    TEST_ASSERT_GREATER_THAN(10, token.length());
    
    // Проверяем, что токен содержит только hex символы
    for (char c : token) {
        TEST_ASSERT_TRUE((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f'));
    }
}

void test_csrf_token_validation(void) {
    // Тест валидации CSRF токена
    std::string token = generateCSRFToken();
    
    // Валидный токен должен пройти проверку
    TEST_ASSERT_TRUE(validateCSRFToken(token));
    
    // Невалидный токен должен не пройти проверку
    TEST_ASSERT_FALSE(validateCSRFToken("invalid_token"));
    TEST_ASSERT_FALSE(validateCSRFToken(""));
}

void test_csrf_token_uniqueness(void) {
    // Тест уникальности токенов
    std::string token1 = generateCSRFToken();
    std::string token2 = generateCSRFToken();
    
    // Токены должны быть разными
    TEST_ASSERT_NOT_EQUAL_STRING(token1.c_str(), token2.c_str());
}

// ---------------- ValidationUtils Tests ----------------

void test_validate_ssid_empty(void) {
    ValidationResult res = validateSSID("");
    TEST_ASSERT_FALSE(res.isValid);
}

void test_validate_ssid_too_long(void) {
    String ssid(33, 'a');
    ValidationResult res = validateSSID(ssid);
    TEST_ASSERT_FALSE(res.isValid);
}

void test_validate_ssid_valid(void) {
    ValidationResult res = validateSSID("my_wifi");
    TEST_ASSERT_TRUE(res.isValid);
}

void test_validate_password_short(void) {
    ValidationResult res = validatePassword("short");
    TEST_ASSERT_FALSE(res.isValid);
}

void test_validate_password_long(void) {
    String pwd(64, 'p');
    ValidationResult res = validatePassword(pwd);
    TEST_ASSERT_FALSE(res.isValid);
}

void test_validate_password_valid(void) {
    ValidationResult res = validatePassword("supersecret");
    TEST_ASSERT_TRUE(res.isValid);
}

void test_validate_mqtt_port_low(void) {
    ValidationResult res = validateMQTTPort(0);
    TEST_ASSERT_FALSE(res.isValid);
}

void test_validate_mqtt_port_high(void) {
    ValidationResult res = validateMQTTPort(70000);
    TEST_ASSERT_FALSE(res.isValid);
}

void test_validate_mqtt_port_valid(void) {
    ValidationResult res = validateMQTTPort(1883);
    TEST_ASSERT_TRUE(res.isValid);
}

void test_validate_temperature_low(void) {
    ValidationResult res = validateTemperature(SENSOR_TEMP_MIN - 1);
    TEST_ASSERT_FALSE(res.isValid);
}

void test_validate_temperature_high(void) {
    ValidationResult res = validateTemperature(SENSOR_TEMP_MAX + 1);
    TEST_ASSERT_FALSE(res.isValid);
}

void test_validate_temperature_valid(void) {
    ValidationResult res = validateTemperature(25.0f);
    TEST_ASSERT_TRUE(res.isValid);
}

// ---------------- FormatUtils Tests ----------------
void test_format_temperature(void){std::string s=format_temperature(23.456f);TEST_ASSERT_EQUAL_STRING("23.5",s.c_str());}
void test_format_ec(void){std::string s=format_ec(1234.7f);TEST_ASSERT_EQUAL_STRING("1235",s.c_str());}
void test_format_ph(void){std::string s=format_ph(6.78f);TEST_ASSERT_EQUAL_STRING("6.8",s.c_str());}
void test_format_value_prec0(void){String s=formatValue(25.4f,"°C",0);TEST_ASSERT_EQUAL_STRING("25°C",s.c_str());}
void test_format_value_prec2(void){String s=formatValue(3.1415f,"pH",2);TEST_ASSERT_EQUAL_STRING("3.14pH",s.c_str());}
// include impl
#include "../src/jxct_format_utils.cpp"

void test_correct_ph_baseline(void){float res=correctPH(7.0f,25.0f);TEST_ASSERT_FLOAT_WITHIN(1e-5f,7.0f,res);} 
void test_correct_ph_hot(void){float res=correctPH(7.0f,35.0f);TEST_ASSERT_FLOAT_WITHIN(1e-4f,6.97f,res);} 
void test_correct_ec_saturated(void){float res=correctEC(1000.0f,25.0f,45.0f,SoilType::LOAM);TEST_ASSERT_FLOAT_WITHIN(0.01f,1000.0f,res);} 
void test_correct_ec_low_moisture(void){float res=correctEC(1000.0f,25.0f,30.0f,SoilType::LOAM);TEST_ASSERT_GREATER_THAN(1000.0f,res);} 
void test_correct_npk_out_of_range(void){float N=100,P=100,K=100;correctNPK(25.0f,20.0f,N,P,K,SoilType::LOAM);TEST_ASSERT_FLOAT_WITHIN(1e-5f,100.0f,N);TEST_ASSERT_FLOAT_WITHIN(1e-5f,100.0f,P);TEST_ASSERT_FLOAT_WITHIN(1e-5f,100.0f,K);} 
void test_correct_npk_increase(void){float N=100,P=100,K=100;correctNPK(25.0f,30.0f,N,P,K,SoilType::LOAM);TEST_ASSERT_GREATER_THAN(100.0f,N);TEST_ASSERT_GREATER_THAN(100.0f,P);TEST_ASSERT_GREATER_THAN(100.0f,K);} 

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_csrf_token_generation);
    RUN_TEST(test_csrf_token_validation);
    RUN_TEST(test_csrf_token_uniqueness);
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
    RUN_TEST(test_format_temperature);
    RUN_TEST(test_format_ec);
    RUN_TEST(test_format_ph);
    RUN_TEST(test_format_value_prec0);
    RUN_TEST(test_format_value_prec2);
    RUN_TEST(test_correct_ph_baseline);
    RUN_TEST(test_correct_ph_hot);
    RUN_TEST(test_correct_ec_saturated);
    RUN_TEST(test_correct_ec_low_moisture);
    RUN_TEST(test_correct_npk_out_of_range);
    RUN_TEST(test_correct_npk_increase);
    UNITY_END();
} 