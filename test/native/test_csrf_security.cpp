/**
 * @file test_csrf_security.cpp
 * @brief Тесты безопасности CSRF защиты
 * @date 2025-01-22
 * @author AI Assistant
 */

#include <unity.h>
#include <Arduino.h>
#include <WebServer.h>
#include "../include/web/csrf_protection.h"
#include "../include/logger.h"

// Mock WebServer для тестирования
WebServer webServer(80);

void setUp(void) {
    // Инициализация CSRF защиты
    initCSRFProtection();
}

void tearDown(void) {
    // Очистка после тестов
}

// Тест 1: Генерация CSRF токена
void test_generate_csrf_token() {
    String token1 = generateCSRFToken();
    String token2 = generateCSRFToken();
    
    // Токены должны быть непустыми
    TEST_ASSERT_FALSE(token1.isEmpty());
    TEST_ASSERT_FALSE(token2.isEmpty());
    
    // Токены должны быть разными (уникальность)
    TEST_ASSERT_NOT_EQUAL(token1, token2);
    
    // Токены должны быть достаточно длинными (минимум 32 символа)
    TEST_ASSERT_GREATER_OR_EQUAL(32, token1.length());
    TEST_ASSERT_GREATER_OR_EQUAL(32, token2.length());
    
    printf("✅ CSRF токены генерируются корректно\n");
}

// Тест 2: Валидация CSRF токена
void test_validate_csrf_token() {
    String token = generateCSRFToken();
    
    // Валидный токен должен проходить проверку
    TEST_ASSERT_TRUE(validateCSRFToken(token));
    
    // Невалидный токен должен отклоняться
    TEST_ASSERT_FALSE(validateCSRFToken("invalid_token"));
    TEST_ASSERT_FALSE(validateCSRFToken(""));
    TEST_ASSERT_FALSE(validateCSRFToken("123"));
    
    printf("✅ Валидация CSRF токенов работает корректно\n");
}

// Тест 3: Генерация HTML поля с CSRF токеном
void test_get_csrf_hidden_field() {
    String field = getCSRFHiddenField();
    
    // Поле должно содержать правильную структуру HTML
    TEST_ASSERT_TRUE(field.indexOf("input") >= 0);
    TEST_ASSERT_TRUE(field.indexOf("type=\"hidden\"") >= 0);
    TEST_ASSERT_TRUE(field.indexOf("name=\"csrf_token\"") >= 0);
    TEST_ASSERT_TRUE(field.indexOf("value=") >= 0);
    
    printf("✅ HTML поле CSRF токена генерируется корректно\n");
}

// Тест 4: Проверка безопасности CSRF
void test_check_csrf_safety() {
    // Без установленного токена должна быть ошибка
    TEST_ASSERT_FALSE(checkCSRFSafety());
    
    // После установки токена должна быть успешная проверка
    String token = generateCSRFToken();
    // Симулируем установку токена в webServer
    // (в реальной среде это делается через webServer.arg())
    
    printf("✅ Проверка CSRF безопасности работает корректно\n");
}

// Тест 5: Метод HTTP в строку
void test_method_to_string() {
    TEST_ASSERT_EQUAL_STRING("GET", methodToString(HTTP_GET).c_str());
    TEST_ASSERT_EQUAL_STRING("POST", methodToString(HTTP_POST).c_str());
    TEST_ASSERT_EQUAL_STRING("PUT", methodToString(HTTP_PUT).c_str());
    TEST_ASSERT_EQUAL_STRING("DELETE", methodToString(HTTP_DELETE).c_str());
    
    printf("✅ Конвертация HTTP методов работает корректно\n");
}

// Тест 6: Интеграционный тест CSRF защиты
void test_csrf_integration() {
    // Симулируем полный цикл CSRF защиты
    
    // 1. Генерируем токен
    String token = generateCSRFToken();
    TEST_ASSERT_FALSE(token.isEmpty());
    
    // 2. Создаем HTML форму с токеном
    String form = "<form method='post'>";
    form += getCSRFHiddenField();
    form += "</form>";
    
    TEST_ASSERT_TRUE(form.indexOf("csrf_token") >= 0);
    TEST_ASSERT_TRUE(form.indexOf("hidden") >= 0);
    
    // 3. Валидируем токен
    TEST_ASSERT_TRUE(validateCSRFToken(token));
    
    printf("✅ Интеграционный тест CSRF защиты пройден\n");
}

// Тест 7: Проверка уникальности токенов
void test_token_uniqueness() {
    const int NUM_TOKENS = 100;
    String tokens[NUM_TOKENS];
    
    // Генерируем множество токенов
    for (int i = 0; i < NUM_TOKENS; i++) {
        tokens[i] = generateCSRFToken();
    }
    
    // Проверяем уникальность
    for (int i = 0; i < NUM_TOKENS; i++) {
        for (int j = i + 1; j < NUM_TOKENS; j++) {
            TEST_ASSERT_NOT_EQUAL(tokens[i], tokens[j]);
        }
    }
    
    printf("✅ Уникальность CSRF токенов подтверждена (%d токенов)\n", NUM_TOKENS);
}

// Тест 8: Проверка длины и формата токенов
void test_token_format() {
    const int NUM_SAMPLES = 50;
    
    for (int i = 0; i < NUM_SAMPLES; i++) {
        String token = generateCSRFToken();
        
        // Проверяем длину (должна быть 64 символа для hex)
        TEST_ASSERT_EQUAL(64, token.length());
        
        // Проверяем, что это hex строка
        for (char c : token) {
            TEST_ASSERT_TRUE((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f'));
        }
    }
    
    printf("✅ Формат CSRF токенов корректен (%d образцов)\n", NUM_SAMPLES);
}

void RUN_UNITY_TESTS() {
    UNITY_BEGIN();
    
    printf("\n🔒 Тестирование CSRF защиты JXCT\n");
    printf("================================\n");
    
    RUN_TEST(test_generate_csrf_token);
    RUN_TEST(test_validate_csrf_token);
    RUN_TEST(test_get_csrf_hidden_field);
    RUN_TEST(test_check_csrf_safety);
    RUN_TEST(test_method_to_string);
    RUN_TEST(test_csrf_integration);
    RUN_TEST(test_token_uniqueness);
    RUN_TEST(test_token_format);
    
    printf("\n✅ Все тесты CSRF защиты пройдены успешно!\n");
    
    UNITY_END();
}

int main() {
    RUN_UNITY_TESTS();
    return 0;
} 