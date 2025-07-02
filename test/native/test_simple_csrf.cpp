/**
 * @file test_simple_csrf.cpp
 * @brief Простой тест CSRF защиты без Unity
 * @date 2025-06-30
 * @author AI Assistant
 */

#include <Arduino.h>
#include <WebServer.h>
#include <cassert>
#include <iostream>
#include "../include/web/csrf_protection.h"

// Mock WebServer для тестирования
WebServer webServer(80);

int main()
{
    std::cout << "🔒 Простой тест CSRF защиты JXCT" << std::endl;
    std::cout << "=================================" << std::endl;

    try
    {
        // Инициализация CSRF защиты
        initCSRFProtection();
        std::cout << "✅ CSRF защита инициализирована" << std::endl;

        // Тест генерации токена
        String token1 = generateCSRFToken();
        String token2 = generateCSRFToken();

        assert(!token1.isEmpty());
        assert(!token2.isEmpty());
        assert(token1 != token2);
        assert(token1.length() >= 32);

        std::cout << "✅ Генерация CSRF токенов работает" << std::endl;

        // Тест валидации
        assert(validateCSRFToken(token1));
        assert(!validateCSRFToken("invalid"));
        assert(!validateCSRFToken(""));

        std::cout << "✅ Валидация CSRF токенов работает" << std::endl;

        // Тест HTML поля
        String field = getCSRFHiddenField();
        assert(field.indexOf("input") >= 0);
        assert(field.indexOf("type=\"hidden\"") >= 0);
        assert(field.indexOf("name=\"csrf_token\"") >= 0);

        std::cout << "✅ HTML поле CSRF генерируется корректно" << std::endl;

        std::cout << "\n🎉 ВСЕ ТЕСТЫ ПРОЙДЕНЫ УСПЕШНО!" << std::endl;
        return 0;
    }
    catch (const std::exception& e)
    {
        std::cerr << "❌ ОШИБКА: " << e.what() << std::endl;
        return 1;
    }
}
