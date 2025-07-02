#include <Arduino.h>
#include <unity.h>

#include "validation_utils.h"
#include "jxct_config_vars.h"
#include "jxct_constants.h"

#ifndef ARDUINO
#include "../src/validation_utils.cpp"
#include "../stubs/logger.cpp"
#endif

void setUp() {}
void tearDown() {}

// ============================================================================
// ТЕСТЫ ВАЛИДАЦИИ КОНФИГУРАЦИИ
// ============================================================================

void test_validateSSID_empty()
{
    auto res = validateSSID("");
    TEST_ASSERT_FALSE(res.isValid);
    TEST_ASSERT_EQUAL_STRING("SSID не может быть пустым", res.message.c_str());
}

void test_validateSSID_valid()
{
    auto res = validateSSID("HomeWiFi");
    TEST_ASSERT_TRUE(res.isValid);
    TEST_ASSERT_EQUAL_STRING("", res.message.c_str());
}

void test_validateSSID_too_long()
{
    auto res = validateSSID("ThisIsAVeryLongSSIDThatExceedsTheMaximumLength");
    TEST_ASSERT_FALSE(res.isValid);
    TEST_ASSERT_EQUAL_STRING("SSID слишком длинный (максимум 32 символа)", res.message.c_str());
}

void test_validateSSID_boundary()
{
    // 32 символа - граничное значение
    String longSSID = "12345678901234567890123456789012";
    auto res = validateSSID(longSSID);
    TEST_ASSERT_TRUE(res.isValid);
}

void test_validatePassword_empty()
{
    auto res = validatePassword("");
    TEST_ASSERT_TRUE(res.isValid); // Пустой пароль допустим
}

void test_validatePassword_short()
{
    auto res = validatePassword("1234567");  // 7 символов
    TEST_ASSERT_FALSE(res.isValid);
    TEST_ASSERT_EQUAL_STRING("Пароль должен содержать минимум 8 символов", res.message.c_str());
}

void test_validatePassword_valid()
{
    auto res = validatePassword("12345678");
    TEST_ASSERT_TRUE(res.isValid);
}

void test_validatePassword_too_long()
{
    String longPassword = "a";
    for (int i = 0; i < 64; i++) longPassword += "a";
    auto res = validatePassword(longPassword);
    TEST_ASSERT_FALSE(res.isValid);
    TEST_ASSERT_EQUAL_STRING("Пароль слишком длинный (максимум 63 символа)", res.message.c_str());
}

void test_validateMQTTServer_empty()
{
    auto res = validateMQTTServer("");
    TEST_ASSERT_FALSE(res.isValid);
    TEST_ASSERT_EQUAL_STRING("MQTT сервер не может быть пустым", res.message.c_str());
}

void test_validateMQTTServer_valid()
{
    auto res = validateMQTTServer("mqtt.example.com");
    TEST_ASSERT_TRUE(res.isValid);
}

void test_validateMQTTServer_with_spaces()
{
    auto res = validateMQTTServer("mqtt example com");
    TEST_ASSERT_FALSE(res.isValid);
    TEST_ASSERT_EQUAL_STRING("MQTT сервер содержит недопустимые символы", res.message.c_str());
}

void test_validateMQTTPort_valid()
{
    auto res = validateMQTTPort(1883);
    TEST_ASSERT_TRUE(res.isValid);
}

void test_validateMQTTPort_invalid_low()
{
    auto res = validateMQTTPort(0);
    TEST_ASSERT_FALSE(res.isValid);
    TEST_ASSERT_EQUAL_STRING("MQTT порт должен быть в диапазоне 1-65535", res.message.c_str());
}

void test_validateMQTTPort_invalid_high()
{
    auto res = validateMQTTPort(70000);
    TEST_ASSERT_FALSE(res.isValid);
}

void test_validateThingSpeakAPIKey_empty()
{
    auto res = validateThingSpeakAPIKey("");
    TEST_ASSERT_FALSE(res.isValid);
    TEST_ASSERT_EQUAL_STRING("ThingSpeak API ключ не может быть пустым", res.message.c_str());
}

void test_validateThingSpeakAPIKey_wrong_length()
{
    auto res = validateThingSpeakAPIKey("123456789012345"); // 15 символов
    TEST_ASSERT_FALSE(res.isValid);
    TEST_ASSERT_EQUAL_STRING("ThingSpeak API ключ должен содержать 16 символов", res.message.c_str());
}

void test_validateThingSpeakAPIKey_valid()
{
    auto res = validateThingSpeakAPIKey("1234567890123456"); // 16 символов
    TEST_ASSERT_TRUE(res.isValid);
}

void test_validateThingSpeakAPIKey_invalid_chars()
{
    auto res = validateThingSpeakAPIKey("123456789012345@"); // содержит @
    TEST_ASSERT_FALSE(res.isValid);
    TEST_ASSERT_EQUAL_STRING("ThingSpeak API ключ содержит недопустимые символы", res.message.c_str());
}

// ============================================================================
// ТЕСТЫ ВАЛИДАЦИИ ИНТЕРВАЛОВ
// ============================================================================

void test_validateInterval_valid()
{
    auto res = validateInterval(5000, 1000, 10000, "Тестовый интервал");
    TEST_ASSERT_TRUE(res.isValid);
}

void test_validateInterval_too_low()
{
    auto res = validateInterval(500, 1000, 10000, "Тестовый интервал");
    TEST_ASSERT_FALSE(res.isValid);
    TEST_ASSERT_TRUE(res.message.indexOf("должен быть в диапазоне") >= 0);
}

void test_validateInterval_too_high()
{
    auto res = validateInterval(15000, 1000, 10000, "Тестовый интервал");
    TEST_ASSERT_FALSE(res.isValid);
}

void test_validateSensorReadInterval()
{
    auto res = validateSensorReadInterval(5000);
    TEST_ASSERT_TRUE(res.isValid);
}

void test_validateMQTTPublishInterval()
{
    auto res = validateMQTTPublishInterval(30000);
    TEST_ASSERT_TRUE(res.isValid);
}

void test_validateThingSpeakInterval()
{
    auto res = validateThingSpeakInterval(60000);
    TEST_ASSERT_TRUE(res.isValid);
}

void test_validateNTPInterval()
{
    auto res = validateNTPInterval(3600000); // 1 час
    TEST_ASSERT_TRUE(res.isValid);
}

// ============================================================================
// ТЕСТЫ ВАЛИДАЦИИ ДАННЫХ ДАТЧИКА
// ============================================================================

void test_validateRange_valid()
{
    auto res = validateRange(25.0F, 0.0F, 50.0F, "Тестовое значение");
    TEST_ASSERT_TRUE(res.isValid);
}

void test_validateRange_too_low()
{
    auto res = validateRange(-5.0F, 0.0F, 50.0F, "Тестовое значение");
    TEST_ASSERT_FALSE(res.isValid);
    TEST_ASSERT_TRUE(res.message.indexOf("вне допустимого диапазона") >= 0);
}

void test_validateRange_too_high()
{
    auto res = validateRange(55.0F, 0.0F, 50.0F, "Тестовое значение");
    TEST_ASSERT_FALSE(res.isValid);
}

void test_validateTemperature_valid()
{
    auto res = validateTemperature(25.0F);
    TEST_ASSERT_TRUE(res.isValid);
}

void test_validateTemperature_too_low()
{
    auto res = validateTemperature(-50.0F);
    TEST_ASSERT_FALSE(res.isValid);
}

void test_validateTemperature_too_high()
{
    auto res = validateTemperature(120.0F);
    TEST_ASSERT_FALSE(res.isValid);
}

void test_validateHumidity_valid()
{
    auto res = validateHumidity(60.0F);
    TEST_ASSERT_TRUE(res.isValid);
}

void test_validateHumidity_invalid()
{
    auto res = validateHumidity(150.0F);
    TEST_ASSERT_FALSE(res.isValid);
}

void test_validatePH_valid()
{
    auto res = validatePH(6.5F);
    TEST_ASSERT_TRUE(res.isValid);
}

void test_validatePH_invalid()
{
    auto res = validatePH(12.0F);
    TEST_ASSERT_FALSE(res.isValid);
}

void test_validateEC_valid()
{
    auto res = validateEC(1500.0F);
    TEST_ASSERT_TRUE(res.isValid);
}

void test_validateEC_invalid()
{
    auto res = validateEC(-100.0F);
    TEST_ASSERT_FALSE(res.isValid);
}

void test_validateNPK_valid()
{
    auto res = validateNPK(100.0F, "Азот");
    TEST_ASSERT_TRUE(res.isValid);
}

void test_validateNPK_invalid()
{
    auto res = validateNPK(3000.0F, "Азот");
    TEST_ASSERT_FALSE(res.isValid);
    TEST_ASSERT_TRUE(res.message.indexOf("Азот") >= 0);
}

// ============================================================================
// ТЕСТЫ КОМПЛЕКСНОЙ ВАЛИДАЦИИ
// ============================================================================

void test_validateFullConfig_valid()
{
    ConfigData config = {};
    strcpy(config.ssid, "TestWiFi");
    strcpy(config.password, "12345678");
    config.mqttEnabled = false;
    
    auto res = validateFullConfig(config, false);
    TEST_ASSERT_TRUE(res.isValid);
    TEST_ASSERT_EQUAL(0, res.errors.size());
}

void test_validateFullConfig_invalid_ssid()
{
    ConfigData config = {};
    strcpy(config.ssid, ""); // Пустой SSID
    strcpy(config.password, "12345678");
    config.mqttEnabled = false;
    
    auto res = validateFullConfig(config, false);
    TEST_ASSERT_FALSE(res.isValid);
    TEST_ASSERT_EQUAL(1, res.errors.size());
    TEST_ASSERT_EQUAL_STRING("ssid", res.errors[0].field.c_str());
}

void test_validateFullConfig_invalid_password()
{
    ConfigData config = {};
    strcpy(config.ssid, "TestWiFi");
    strcpy(config.password, "123"); // Слишком короткий
    config.mqttEnabled = false;
    
    auto res = validateFullConfig(config, false);
    TEST_ASSERT_FALSE(res.isValid);
    TEST_ASSERT_EQUAL(1, res.errors.size());
    TEST_ASSERT_EQUAL_STRING("password", res.errors[0].field.c_str());
}

void test_validateFullConfig_mqtt_enabled_invalid()
{
    ConfigData config = {};
    strcpy(config.ssid, "TestWiFi");
    strcpy(config.password, "12345678");
    config.mqttEnabled = true;
    strcpy(config.mqttServer, ""); // Пустой сервер
    
    auto res = validateFullConfig(config, true);
    TEST_ASSERT_FALSE(res.isValid);
    TEST_ASSERT_EQUAL(1, res.errors.size());
    TEST_ASSERT_EQUAL_STRING("mqtt_server", res.errors[0].field.c_str());
}

// ============================================================================
// ТЕСТЫ ВСПОМОГАТЕЛЬНЫХ ФУНКЦИЙ
// ============================================================================

void test_isValidIPAddress_valid()
{
    TEST_ASSERT_TRUE(isValidIPAddress("192.168.1.1"));
    TEST_ASSERT_TRUE(isValidIPAddress("10.0.0.1"));
    TEST_ASSERT_TRUE(isValidIPAddress("172.16.0.1"));
}

void test_isValidIPAddress_invalid()
{
    TEST_ASSERT_FALSE(isValidIPAddress("256.1.2.3"));
    TEST_ASSERT_FALSE(isValidIPAddress("1.2.3.256"));
    TEST_ASSERT_FALSE(isValidIPAddress("192.168.1"));
    TEST_ASSERT_FALSE(isValidIPAddress("192.168.1.1.1"));
    TEST_ASSERT_FALSE(isValidIPAddress("abc.def.ghi.jkl"));
}

void test_isValidHostname_valid()
{
    TEST_ASSERT_TRUE(isValidHostname("example.com"));
    TEST_ASSERT_TRUE(isValidHostname("mqtt.example.com"));
    TEST_ASSERT_TRUE(isValidHostname("localhost"));
}

void test_isValidHostname_invalid()
{
    TEST_ASSERT_FALSE(isValidHostname(""));
    TEST_ASSERT_FALSE(isValidHostname("example..com"));
    TEST_ASSERT_FALSE(isValidHostname(".example.com"));
    TEST_ASSERT_FALSE(isValidHostname("example.com."));
}

// ============================================================================
// ТЕСТЫ ФОРМАТИРОВАНИЯ ОШИБОК
// ============================================================================

void test_formatValidationErrors()
{
    ConfigValidationResult result;
    result.isValid = false;
    result.errors.push_back({"ssid", "SSID не может быть пустым"});
    result.errors.push_back({"password", "Пароль слишком короткий"});
    
    String formatted = formatValidationErrors(result);
    TEST_ASSERT_TRUE(formatted.indexOf("ssid") >= 0);
    TEST_ASSERT_TRUE(formatted.indexOf("password") >= 0);
    TEST_ASSERT_TRUE(formatted.indexOf("SSID не может быть пустым") >= 0);
}

void test_formatSensorValidationErrors()
{
    SensorValidationResult result;
    result.isValid = false;
    result.errors.push_back({"temperature", "Температура вне диапазона"});
    result.errors.push_back({"humidity", "Влажность вне диапазона"});
    
    String formatted = formatSensorValidationErrors(result);
    TEST_ASSERT_TRUE(formatted.indexOf("temperature") >= 0);
    TEST_ASSERT_TRUE(formatted.indexOf("humidity") >= 0);
}

// Arduino-style entry point (device tests)
#ifdef ARDUINO
void setup()
{
    Serial.begin(115200);
    delay(2000);
    UNITY_BEGIN();
    
    // Тесты валидации конфигурации
    RUN_TEST(test_validateSSID_empty);
    RUN_TEST(test_validateSSID_valid);
    RUN_TEST(test_validateSSID_too_long);
    RUN_TEST(test_validateSSID_boundary);
    RUN_TEST(test_validatePassword_empty);
    RUN_TEST(test_validatePassword_short);
    RUN_TEST(test_validatePassword_valid);
    RUN_TEST(test_validatePassword_too_long);
    RUN_TEST(test_validateMQTTServer_empty);
    RUN_TEST(test_validateMQTTServer_valid);
    RUN_TEST(test_validateMQTTServer_with_spaces);
    RUN_TEST(test_validateMQTTPort_valid);
    RUN_TEST(test_validateMQTTPort_invalid_low);
    RUN_TEST(test_validateMQTTPort_invalid_high);
    RUN_TEST(test_validateThingSpeakAPIKey_empty);
    RUN_TEST(test_validateThingSpeakAPIKey_wrong_length);
    RUN_TEST(test_validateThingSpeakAPIKey_valid);
    RUN_TEST(test_validateThingSpeakAPIKey_invalid_chars);
    
    // Тесты валидации интервалов
    RUN_TEST(test_validateInterval_valid);
    RUN_TEST(test_validateInterval_too_low);
    RUN_TEST(test_validateInterval_too_high);
    RUN_TEST(test_validateSensorReadInterval);
    RUN_TEST(test_validateMQTTPublishInterval);
    RUN_TEST(test_validateThingSpeakInterval);
    RUN_TEST(test_validateNTPInterval);
    
    // Тесты валидации данных датчика
    RUN_TEST(test_validateRange_valid);
    RUN_TEST(test_validateRange_too_low);
    RUN_TEST(test_validateRange_too_high);
    RUN_TEST(test_validateTemperature_valid);
    RUN_TEST(test_validateTemperature_too_low);
    RUN_TEST(test_validateTemperature_too_high);
    RUN_TEST(test_validateHumidity_valid);
    RUN_TEST(test_validateHumidity_invalid);
    RUN_TEST(test_validatePH_valid);
    RUN_TEST(test_validatePH_invalid);
    RUN_TEST(test_validateEC_valid);
    RUN_TEST(test_validateEC_invalid);
    RUN_TEST(test_validateNPK_valid);
    RUN_TEST(test_validateNPK_invalid);
    
    // Тесты комплексной валидации
    RUN_TEST(test_validateFullConfig_valid);
    RUN_TEST(test_validateFullConfig_invalid_ssid);
    RUN_TEST(test_validateFullConfig_invalid_password);
    RUN_TEST(test_validateFullConfig_mqtt_enabled_invalid);
    
    // Тесты вспомогательных функций
    RUN_TEST(test_isValidIPAddress_valid);
    RUN_TEST(test_isValidIPAddress_invalid);
    RUN_TEST(test_isValidHostname_valid);
    RUN_TEST(test_isValidHostname_invalid);
    
    // Тесты форматирования ошибок
    RUN_TEST(test_formatValidationErrors);
    RUN_TEST(test_formatSensorValidationErrors);
    
    UNITY_END();
}

void loop() {}
#else  // Native PC tests
int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;
    UNITY_BEGIN();
    
    // Тесты валидации конфигурации
    RUN_TEST(test_validateSSID_empty);
    RUN_TEST(test_validateSSID_valid);
    RUN_TEST(test_validateSSID_too_long);
    RUN_TEST(test_validateSSID_boundary);
    RUN_TEST(test_validatePassword_empty);
    RUN_TEST(test_validatePassword_short);
    RUN_TEST(test_validatePassword_valid);
    RUN_TEST(test_validatePassword_too_long);
    RUN_TEST(test_validateMQTTServer_empty);
    RUN_TEST(test_validateMQTTServer_valid);
    RUN_TEST(test_validateMQTTServer_with_spaces);
    RUN_TEST(test_validateMQTTPort_valid);
    RUN_TEST(test_validateMQTTPort_invalid_low);
    RUN_TEST(test_validateMQTTPort_invalid_high);
    RUN_TEST(test_validateThingSpeakAPIKey_empty);
    RUN_TEST(test_validateThingSpeakAPIKey_wrong_length);
    RUN_TEST(test_validateThingSpeakAPIKey_valid);
    RUN_TEST(test_validateThingSpeakAPIKey_invalid_chars);
    
    // Тесты валидации интервалов
    RUN_TEST(test_validateInterval_valid);
    RUN_TEST(test_validateInterval_too_low);
    RUN_TEST(test_validateInterval_too_high);
    RUN_TEST(test_validateSensorReadInterval);
    RUN_TEST(test_validateMQTTPublishInterval);
    RUN_TEST(test_validateThingSpeakInterval);
    RUN_TEST(test_validateNTPInterval);
    
    // Тесты валидации данных датчика
    RUN_TEST(test_validateRange_valid);
    RUN_TEST(test_validateRange_too_low);
    RUN_TEST(test_validateRange_too_high);
    RUN_TEST(test_validateTemperature_valid);
    RUN_TEST(test_validateTemperature_too_low);
    RUN_TEST(test_validateTemperature_too_high);
    RUN_TEST(test_validateHumidity_valid);
    RUN_TEST(test_validateHumidity_invalid);
    RUN_TEST(test_validatePH_valid);
    RUN_TEST(test_validatePH_invalid);
    RUN_TEST(test_validateEC_valid);
    RUN_TEST(test_validateEC_invalid);
    RUN_TEST(test_validateNPK_valid);
    RUN_TEST(test_validateNPK_invalid);
    
    // Тесты комплексной валидации
    RUN_TEST(test_validateFullConfig_valid);
    RUN_TEST(test_validateFullConfig_invalid_ssid);
    RUN_TEST(test_validateFullConfig_invalid_password);
    RUN_TEST(test_validateFullConfig_mqtt_enabled_invalid);
    
    // Тесты вспомогательных функций
    RUN_TEST(test_isValidIPAddress_valid);
    RUN_TEST(test_isValidIPAddress_invalid);
    RUN_TEST(test_isValidHostname_valid);
    RUN_TEST(test_isValidHostname_invalid);
    
    // Тесты форматирования ошибок
    RUN_TEST(test_formatValidationErrors);
    RUN_TEST(test_formatSensorValidationErrors);
    
    return UNITY_END();
}
#endif  // ARDUINO 