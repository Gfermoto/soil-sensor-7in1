/**
 * @file test_format_utils.cpp
 * @brief Unit-тесты для утилит форматирования JXCT (ESP32 совместимые)
 * @version 1.0.0
 * @date 2025-07-02
 * @author JXCT Development Team
 */

#include <unity.h>
#include <Arduino.h>

// Включаем исходные файлы для тестирования
#include "../src/jxct_format_utils.cpp"
#include "../src/validation_utils.cpp"

// ============================================================================
// ТЕСТЫ ФОРМАТИРОВАНИЯ ДАННЫХ ДАТЧИКОВ
// ============================================================================

void test_format_temperature_value() {
    // Тест форматирования температуры
    float temp = 25.5f;
    String formatted = formatTemperature(temp);
    
    TEST_ASSERT_TRUE(formatted.length() > 0);
    TEST_ASSERT_TRUE(formatted.indexOf("25.5") >= 0);
    TEST_ASSERT_TRUE(formatted.indexOf("°C") >= 0);
}

void test_format_ec_value() {
    // Тест форматирования электропроводности
    int ec = 1500;
    String formatted = formatEC(ec);
    
    TEST_ASSERT_TRUE(formatted.length() > 0);
    TEST_ASSERT_TRUE(formatted.indexOf("1500") >= 0);
    TEST_ASSERT_TRUE(formatted.indexOf("µS/cm") >= 0);
}

void test_format_ph_value() {
    // Тест форматирования pH
    float ph = 6.8f;
    String formatted = formatPH(ph);
    
    TEST_ASSERT_TRUE(formatted.length() > 0);
    TEST_ASSERT_TRUE(formatted.indexOf("6.8") >= 0);
    TEST_ASSERT_TRUE(formatted.indexOf("pH") >= 0);
}

void test_format_npk_values() {
    // Тест форматирования NPK значений
    float N = 120.5f, P = 45.2f, K = 89.7f;
    String formatted = formatNPK(N, P, K);
    
    TEST_ASSERT_TRUE(formatted.length() > 0);
    TEST_ASSERT_TRUE(formatted.indexOf("120.5") >= 0);
    TEST_ASSERT_TRUE(formatted.indexOf("45.2") >= 0);
    TEST_ASSERT_TRUE(formatted.indexOf("89.7") >= 0);
    TEST_ASSERT_TRUE(formatted.indexOf("mg/kg") >= 0);
}

void test_format_moisture_value() {
    // Тест форматирования влажности
    float moisture = 65.3f;
    String formatted = formatMoisture(moisture);
    
    TEST_ASSERT_TRUE(formatted.length() > 0);
    TEST_ASSERT_TRUE(formatted.indexOf("65.3") >= 0);
    TEST_ASSERT_TRUE(formatted.indexOf("%") >= 0);
}

// ============================================================================
// ТЕСТЫ JSON ФОРМАТИРОВАНИЯ
// ============================================================================

void test_format_json_sensor_data() {
    // Тест JSON форматирования данных датчиков
    SensorData data;
    data.temperature = 25.5f;
    data.ec = 1500;
    data.ph = 6.8f;
    data.moisture = 65.3f;
    data.N = 120.5f;
    data.P = 45.2f;
    data.K = 89.7f;
    
    String json = formatSensorDataJSON(data);
    
    TEST_ASSERT_TRUE(json.length() > 0);
    TEST_ASSERT_TRUE(json.indexOf("temperature") >= 0);
    TEST_ASSERT_TRUE(json.indexOf("25.5") >= 0);
    TEST_ASSERT_TRUE(json.indexOf("ec") >= 0);
    TEST_ASSERT_TRUE(json.indexOf("1500") >= 0);
    TEST_ASSERT_TRUE(json.indexOf("ph") >= 0);
    TEST_ASSERT_TRUE(json.indexOf("6.8") >= 0);
}

void test_format_json_status() {
    // Тест JSON форматирования статуса
    String status = "OK";
    int uptime = 3600;
    String json = formatStatusJSON(status, uptime);
    
    TEST_ASSERT_TRUE(json.length() > 0);
    TEST_ASSERT_TRUE(json.indexOf("status") >= 0);
    TEST_ASSERT_TRUE(json.indexOf("OK") >= 0);
    TEST_ASSERT_TRUE(json.indexOf("uptime") >= 0);
    TEST_ASSERT_TRUE(json.indexOf("3600") >= 0);
}

void test_format_json_error() {
    // Тест JSON форматирования ошибки
    String error = "Sensor timeout";
    int code = 500;
    String json = formatErrorJSON(error, code);
    
    TEST_ASSERT_TRUE(json.length() > 0);
    TEST_ASSERT_TRUE(json.indexOf("error") >= 0);
    TEST_ASSERT_TRUE(json.indexOf("Sensor timeout") >= 0);
    TEST_ASSERT_TRUE(json.indexOf("code") >= 0);
    TEST_ASSERT_TRUE(json.indexOf("500") >= 0);
}

// ============================================================================
// ТЕСТЫ HTML ФОРМАТИРОВАНИЯ
// ============================================================================

void test_format_html_table_row() {
    // Тест форматирования строки HTML таблицы
    String name = "Temperature";
    String value = "25.5°C";
    String unit = "Celsius";
    
    String html = formatHTMLTableRow(name, value, unit);
    
    TEST_ASSERT_TRUE(html.length() > 0);
    TEST_ASSERT_TRUE(html.indexOf("<tr>") >= 0);
    TEST_ASSERT_TRUE(html.indexOf("</tr>") >= 0);
    TEST_ASSERT_TRUE(html.indexOf("Temperature") >= 0);
    TEST_ASSERT_TRUE(html.indexOf("25.5°C") >= 0);
}

void test_format_html_sensor_card() {
    // Тест форматирования HTML карточки датчика
    String title = "Soil Sensor";
    String status = "Online";
    String lastUpdate = "2025-07-02 12:30:15";
    
    String html = formatHTMLSensorCard(title, status, lastUpdate);
    
    TEST_ASSERT_TRUE(html.length() > 0);
    TEST_ASSERT_TRUE(html.indexOf("card") >= 0);
    TEST_ASSERT_TRUE(html.indexOf("Soil Sensor") >= 0);
    TEST_ASSERT_TRUE(html.indexOf("Online") >= 0);
    TEST_ASSERT_TRUE(html.indexOf("2025-07-02") >= 0);
}

// ============================================================================
// ТЕСТЫ ВАЛИДАЦИИ ФОРМАТИРОВАНИЯ
// ============================================================================

void test_validate_number_format() {
    // Тест валидации числового формата
    TEST_ASSERT_TRUE(isValidNumberFormat("123.45"));
    TEST_ASSERT_TRUE(isValidNumberFormat("0"));
    TEST_ASSERT_TRUE(isValidNumberFormat("-25.5"));
    TEST_ASSERT_FALSE(isValidNumberFormat("abc"));
    TEST_ASSERT_FALSE(isValidNumberFormat("12.34.56"));
    TEST_ASSERT_FALSE(isValidNumberFormat(""));
}

void test_validate_json_format() {
    // Тест валидации JSON формата
    String validJSON = "{\"temperature\": 25.5, \"status\": \"OK\"}";
    String invalidJSON = "{temperature: 25.5, status: OK}";
    
    TEST_ASSERT_TRUE(isValidJSONFormat(validJSON));
    TEST_ASSERT_FALSE(isValidJSONFormat(invalidJSON));
    TEST_ASSERT_FALSE(isValidJSONFormat(""));
}

void test_validate_html_format() {
    // Тест валидации HTML формата
    String validHTML = "<div class=\"test\">Content</div>";
    String invalidHTML = "<div class=\"test\">Content";
    
    TEST_ASSERT_TRUE(isValidHTMLFormat(validHTML));
    TEST_ASSERT_FALSE(isValidHTMLFormat(invalidHTML));
    TEST_ASSERT_FALSE(isValidHTMLFormat(""));
}

// ============================================================================
// ТЕСТЫ ПРОИЗВОДИТЕЛЬНОСТИ ФОРМАТИРОВАНИЯ
// ============================================================================

void test_format_performance() {
    // Тест производительности форматирования
    unsigned long start = millis();
    
    // Форматируем много значений
    for (int i = 0; i < 100; ++i) {
        float temp = 20.0f + i * 0.1f;
        String formatted = formatTemperature(temp);
        TEST_ASSERT_TRUE(formatted.length() > 0);
    }
    
    unsigned long duration = millis() - start;
    
    // Должно выполняться быстро (менее 1000мс на 100 операций)
    TEST_ASSERT_TRUE(duration < 1000);
    
    Serial.print("⏱️ Производительность форматирования: ");
    Serial.print(duration);
    Serial.println("мс для 100 операций");
}

void test_json_format_performance() {
    // Тест производительности JSON форматирования
    unsigned long start = millis();
    
    SensorData data;
    data.temperature = 25.5f;
    data.ec = 1500;
    data.ph = 6.8f;
    data.moisture = 65.3f;
    data.N = 120.5f;
    data.P = 45.2f;
    data.K = 89.7f;
    
    // Форматируем много JSON объектов
    for (int i = 0; i < 50; ++i) {
        data.temperature += 0.1f;
        String json = formatSensorDataJSON(data);
        TEST_ASSERT_TRUE(json.length() > 0);
    }
    
    unsigned long duration = millis() - start;
    
    // Должно выполняться быстро (менее 2000мс на 50 операций)
    TEST_ASSERT_TRUE(duration < 2000);
    
    Serial.print("⏱️ Производительность JSON: ");
    Serial.print(duration);
    Serial.println("мс для 50 операций");
}

// ============================================================================
// ОСНОВНАЯ ФУНКЦИЯ ТЕСТИРОВАНИЯ
// ============================================================================

void setUp() {
    // Инициализация перед каждым тестом
}

void tearDown() {
    // Очистка после каждого теста
}

#ifdef ARDUINO
void setup() {
    Serial.begin(115200);
    delay(2000);
    
    Serial.println("🧪 JXCT Format Utils Testing Started");
    Serial.println("=====================================");
    
    UNITY_BEGIN();
    
    // Тесты форматирования данных
    RUN_TEST(test_format_temperature_value);
    RUN_TEST(test_format_ec_value);
    RUN_TEST(test_format_ph_value);
    RUN_TEST(test_format_npk_values);
    RUN_TEST(test_format_moisture_value);
    
    // Тесты JSON форматирования
    RUN_TEST(test_format_json_sensor_data);
    RUN_TEST(test_format_json_status);
    RUN_TEST(test_format_json_error);
    
    // Тесты HTML форматирования
    RUN_TEST(test_format_html_table_row);
    RUN_TEST(test_format_html_sensor_card);
    
    // Тесты валидации форматирования
    RUN_TEST(test_validate_number_format);
    RUN_TEST(test_validate_json_format);
    RUN_TEST(test_validate_html_format);
    
    // Тесты производительности
    RUN_TEST(test_format_performance);
    RUN_TEST(test_json_format_performance);
    
    UNITY_END();
    
    Serial.println("✅ Format Utils Testing completed");
    Serial.println("Добавлено +15 unit-тестов для утилит форматирования");
}

void loop() {
    // Пустой цикл
}

#else
// Native версия для компиляции в среде разработки
int main() {
    std::cout << "🧪 JXCT Format Utils Testing (Native)" << std::endl;
    
    UNITY_BEGIN();
    
    // Основные тесты
    RUN_TEST(test_format_temperature_value);
    RUN_TEST(test_format_json_sensor_data);
    RUN_TEST(test_validate_number_format);
    RUN_TEST(test_format_performance);
    
    return UNITY_END();
}
#endif 