#include <Arduino.h>
#include <unity.h>

#include "jxct_format_utils.h"

#ifndef ARDUINO
#include "../src/jxct_format_utils.cpp"
#endif

void setUp() {}
void tearDown() {}

// ============================================================================
// ТЕСТЫ УНИВЕРСАЛЬНОГО ФОРМАТИРОВАНИЯ FLOAT
// ============================================================================

void test_formatFloat_as_int()
{
    auto result = formatFloat(25.7F, 0, true);
    TEST_ASSERT_EQUAL_STRING("26", result.c_str());
}

void test_formatFloat_as_float_0_precision()
{
    auto result = formatFloat(25.7F, 0, false);
    TEST_ASSERT_EQUAL_STRING("26", result.c_str());
}

void test_formatFloat_as_float_1_precision()
{
    auto result = formatFloat(25.7F, 1, false);
    TEST_ASSERT_EQUAL_STRING("25.7", result.c_str());
}

void test_formatFloat_as_float_2_precision()
{
    auto result = formatFloat(25.789F, 2, false);
    TEST_ASSERT_EQUAL_STRING("25.79", result.c_str());
}

void test_formatFloat_negative()
{
    auto result = formatFloat(-15.5F, 1, false);
    TEST_ASSERT_EQUAL_STRING("-15.5", result.c_str());
}

void test_formatFloat_zero()
{
    auto result = formatFloat(0.0F, 1, false);
    TEST_ASSERT_EQUAL_STRING("0.0", result.c_str());
}

void test_formatFloat_rounding()
{
    auto result = formatFloat(25.999F, 2, false);
    TEST_ASSERT_EQUAL_STRING("26.00", result.c_str());
}

// ============================================================================
// ТЕСТЫ СПЕЦИАЛИЗИРОВАННОГО ФОРМАТИРОВАНИЯ
// ============================================================================

void test_format_moisture()
{
    auto result = format_moisture(45.7F);
    TEST_ASSERT_EQUAL_STRING("45.7", result.c_str());
}

void test_format_temperature()
{
    auto result = format_temperature(23.5F);
    TEST_ASSERT_EQUAL_STRING("23.5", result.c_str());
}

void test_format_ec()
{
    auto result = format_ec(1500.7F);
    TEST_ASSERT_EQUAL_STRING("1501", result.c_str());  // Округление
}

void test_format_ph()
{
    auto result = format_ph(6.8F);
    TEST_ASSERT_EQUAL_STRING("6.8", result.c_str());
}

void test_format_npk()
{
    auto result = format_npk(125.7F);
    TEST_ASSERT_EQUAL_STRING("126", result.c_str());  // Округление
}

// ============================================================================
// ТЕСТЫ ВЕБ-ФОРМАТИРОВАНИЯ
// ============================================================================

void test_formatValue_precision_0()
{
    auto result = formatValue(25.7F, "°C", 0);
    TEST_ASSERT_EQUAL_STRING("26°C", result.c_str());
}

void test_formatValue_precision_1()
{
    auto result = formatValue(25.7F, "°C", 1);
    TEST_ASSERT_EQUAL_STRING("25.7°C", result.c_str());
}

void test_formatValue_precision_2()
{
    auto result = formatValue(25.789F, "°C", 2);
    TEST_ASSERT_EQUAL_STRING("25.79°C", result.c_str());
}

void test_formatValue_precision_3()
{
    auto result = formatValue(25.7891F, "°C", 3);
    TEST_ASSERT_EQUAL_STRING("25.789°C", result.c_str());
}

void test_formatValue_precision_default()
{
    auto result = formatValue(25.789F, "°C", 5);          // Неизвестная точность
    TEST_ASSERT_EQUAL_STRING("25.79°C", result.c_str());  // Должна быть точность 2
}

void test_formatValue_negative()
{
    auto result = formatValue(-15.5F, "°C", 1);
    TEST_ASSERT_EQUAL_STRING("-15.5°C", result.c_str());
}

void test_formatValue_zero()
{
    auto result = formatValue(0.0F, "°C", 1);
    TEST_ASSERT_EQUAL_STRING("0.0°C", result.c_str());
}

void test_formatValue_empty_unit()
{
    auto result = formatValue(25.7F, "", 1);
    TEST_ASSERT_EQUAL_STRING("25.7", result.c_str());
}

void test_formatValue_different_units()
{
    auto result1 = formatValue(60.0F, "%", 1);
    auto result2 = formatValue(1500.0F, "µS/cm", 0);
    auto result3 = formatValue(6.5F, "pH", 1);

    TEST_ASSERT_EQUAL_STRING("60.0%", result1.c_str());
    TEST_ASSERT_EQUAL_STRING("1500µS/cm", result2.c_str());
    TEST_ASSERT_EQUAL_STRING("6.5pH", result3.c_str());
}

// ============================================================================
// ТЕСТЫ ГРАНИЧНЫХ ЗНАЧЕНИЙ
// ============================================================================

void test_formatFloat_very_small()
{
    auto result = formatFloat(0.001F, 3, false);
    TEST_ASSERT_EQUAL_STRING("0.001", result.c_str());
}

void test_formatFloat_very_large()
{
    auto result = formatFloat(9999.9F, 1, false);
    TEST_ASSERT_EQUAL_STRING("9999.9", result.c_str());
}

void test_formatValue_large_number()
{
    auto result = formatValue(9999.9F, "units", 1);
    TEST_ASSERT_EQUAL_STRING("9999.9units", result.c_str());
}

// ============================================================================
// ТЕСТЫ ОКРУГЛЕНИЯ
// ============================================================================

void test_formatFloat_rounding_up()
{
    auto result = formatFloat(25.51F, 0, false);
    TEST_ASSERT_EQUAL_STRING("26", result.c_str());
}

void test_formatFloat_rounding_down()
{
    auto result = formatFloat(25.49F, 0, false);
    TEST_ASSERT_EQUAL_STRING("25", result.c_str());
}

void test_formatValue_rounding()
{
    auto result = formatValue(25.51F, "°C", 0);
    TEST_ASSERT_EQUAL_STRING("26°C", result.c_str());
}
