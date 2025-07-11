/**
 * @file csrf_protection.cpp
 * @brief Безопасная CSRF защита для веб-интерфейса JXCT
 * @date 2025-01-22
 * @author AI Assistant (Tech Debt Reduction Plan - Stage 1.2)
 *
 * ВАЖНО: Реализация НЕ нарушает существующий функционал
 * Добавляет дополнительную защиту без поломки API
 */

#include "../../include/web/csrf_protection.h"
#include <Arduino.h>
#include <WiFi.h>
#include "../../include/logger.h"

// ============================================================================
// ГЛОБАЛЬНЫЕ ПЕРЕМЕННЫЕ ДЛЯ CSRF
// ============================================================================

namespace
{
String currentCSRFToken = "";
unsigned long tokenGeneratedTime = 0;
}  // namespace
const unsigned long CSRF_TOKEN_LIFETIME = 30 * 60 * 1000;  // 30 минут в миллисекундах

// ============================================================================
// БЕЗОПАСНАЯ ГЕНЕРАЦИЯ CSRF ТОКЕНОВ
// ============================================================================

String generateCSRFToken() // NOLINT(misc-use-internal-linkage)
{
    // Используем различные источники энтропии для безопасности
    String token = "";

    // Компоненты для генерации токена:
    const unsigned long currentTime = millis();
    const uint32_t freeHeap = ESP.getFreeHeap();
    const uint32_t chipId = ESP.getEfuseMac();

    // Добавляем MAC адрес для уникальности
    String macAddr = WiFi.macAddress();
    macAddr.replace(":", "");

    // Генерируем токен из различных источников
    token = String(currentTime, HEX) + String(freeHeap, HEX) + String(chipId, HEX) +
            macAddr.substring(6);  // Последние 6 символов MAC

    // Сохраняем токен и время генерации
    currentCSRFToken = token;
    tokenGeneratedTime = currentTime;

    logDebugSafe("\1", token.c_str(), currentTime);

    return token;
}

bool validateCSRFToken(const String& token) // NOLINT(misc-use-internal-linkage)
{
    // Если токен пустой или не инициализирован
    if (token.isEmpty() || currentCSRFToken.isEmpty())
    {
        logWarn("CSRF валидация: пустой токен");
        return false;
    }

    // Проверяем время жизни токена
    const unsigned long currentTime = millis();
    if (currentTime - tokenGeneratedTime > CSRF_TOKEN_LIFETIME)
    {
        logWarnSafe("\1", currentTime, tokenGeneratedTime);
        currentCSRFToken = "";
        return false;
    }

    // Проверяем совпадение токенов
    const bool isValid = (token == currentCSRFToken);

    if (isValid)
    {
        logDebug("CSRF токен валиден");
    }
    else
    {
        logWarnSafe("\1", currentCSRFToken.c_str(), token.c_str());
    }

    return isValid;
}

String getCSRFHiddenField() // NOLINT(misc-use-internal-linkage)
{
    // Генерируем новый токен если текущий пустой или истек
    if (currentCSRFToken.isEmpty() || (millis() - tokenGeneratedTime) > CSRF_TOKEN_LIFETIME)
    {
        generateCSRFToken();
    }

    return R"(<input type="hidden" name="csrf_token" value=")" + currentCSRFToken + R"(">)";
}

bool checkCSRFSafety()
{
    // GET запросы всегда безопасны (идемпотентные)
    const HTTPMethod method = webServer.method();
    if (method == HTTP_GET || method == HTTP_HEAD || method == HTTP_OPTIONS)
    {
        return true;
    }

    // POST, PUT, DELETE требуют проверки CSRF
    String csrfToken = "";

    // Ищем токен в POST параметрах
    if (webServer.hasArg("csrf_token"))
    {
        csrfToken = webServer.arg("csrf_token");
    }

    // Если токен не найден в POST, ищем в заголовках
    if (csrfToken.isEmpty() && webServer.hasHeader("X-CSRF-Token"))
    {
        csrfToken = webServer.header("X-CSRF-Token");
    }

    // Логируем попытку доступа
    const String clientIP = webServer.client().remoteIP().toString();
    const String uri = webServer.uri();

    if (csrfToken.isEmpty())
    {
        logWarnSafe("\1", methodToString(method).c_str(), uri.c_str(), clientIP.c_str());
        return false;
    }

    const bool isValid = validateCSRFToken(csrfToken);

    if (!isValid)
    {
        logWarnSafe("\1", methodToString(method).c_str(), uri.c_str(), clientIP.c_str());
    }

    return isValid;
}

// ============================================================================
// ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ
// ============================================================================

String methodToString(HTTPMethod method)  // NOLINT(misc-use-internal-linkage)
{
    switch (method)
    {
        case HTTP_GET:
            return "GET";
        case HTTP_POST:
            return "POST";
        case HTTP_PUT:
            return "PUT";
        case HTTP_DELETE:
            return "DELETE";
        case HTTP_PATCH:
            return "PATCH";
        case HTTP_HEAD:
            return "HEAD";
        case HTTP_OPTIONS:
            return "OPTIONS";
        default:
            return "UNKNOWN";
    }
}

// ============================================================================
// ИНИЦИАЛИЗАЦИЯ CSRF ЗАЩИТЫ
// ============================================================================

void initCSRFProtection()
{
    logInfo("Инициализация CSRF защиты");

    // Генерируем первоначальный токен
    generateCSRFToken();

    logSuccessSafe("\1", CSRF_TOKEN_LIFETIME / 60000);
}
