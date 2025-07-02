#include "../../include/jxct_config_vars.h"
#include "../../include/jxct_strings.h"
#include "../../include/jxct_ui_system.h"
#include "../../include/logger.h"
#include "../../include/web_routes.h"
#include "../wifi_manager.h"

// Вспомогательная функция для валидации интервалов
bool validateInterval(const String& argName, int minValue, int maxValue, const String& description)
{
    if (webServer.hasArg(argName))
    {
        int value = webServer.arg(argName).toInt();
        if (value < minValue || value > maxValue)
        {
            logWarn("Валидация: некорректный %s: %d (допустимо %d-%d)", description.c_str(), value, minValue, maxValue);
            return false;
        }
    }
    return true;
}

void setupErrorHandlers()
{
    // Обработчик 404 - страница не найдена
    webServer.onNotFound(
        []()
        {
            String uri = webServer.uri();
            String method = webServer.method() == HTTP_GET ? "GET" : "POST";

            logWebRequest(method, uri, webServer.client().remoteIP().toString());

            logWarn("404 Not Found: %s %s", method.c_str(), uri.c_str());

            String html = generateErrorPage(404, "Страница не найдена");
            webServer.send(404, "text/html; charset=utf-8", html);
        });

    // Общий обработчик ошибок для внутренних ошибок сервера
    // Примечание: ESP32 WebServer не имеет встроенного onError,
    // поэтому ошибки 500 обрабатываются в конкретных маршрутах
}

bool validateConfigInput(bool checkRequired)
{
    // Валидация базовых полей
    if (checkRequired)
    {
        // SSID всегда обязательно
        if (!webServer.hasArg("ssid") || webServer.arg("ssid").length() == 0)
        {
            logWarn("Валидация: отсутствует SSID");
            return false;
        }

        // В STA режиме проверяем дополнительные поля
        if (currentWiFiMode == WiFiMode::STA)
        {
            // Если MQTT включен, проверяем обязательные поля
            if (webServer.hasArg("mqtt_enabled"))
            {
                if (!webServer.hasArg("mqtt_server") || webServer.arg("mqtt_server").length() == 0)
                {
                    logWarn("Валидация: MQTT включен, но отсутствует сервер");
                    return false;
                }
            }

            // Если ThingSpeak включен, проверяем API ключ
            if (webServer.hasArg("ts_enabled"))
            {
                if (!webServer.hasArg("ts_api_key") || webServer.arg("ts_api_key").length() == 0)
                {
                    logWarn("Валидация: ThingSpeak включен, но отсутствует API ключ");
                    return false;
                }
            }
        }
    }

    // Валидация форматов данных
    if (!validateInterval("mqtt_port", 1, 65535, "MQTT порт")) return false;
    if (!validateInterval("ntp_interval", 10000, 86400000, "NTP интервал")) return false;
    if (!validateInterval("sensor_read", 1000, 300000, "интервал чтения датчика")) return false;
    if (!validateInterval("mqtt_publish", 1000, 3600000, "интервал MQTT публикации")) return false;
    if (!validateInterval("thingspeak_interval", 15000, 7200000, "интервал ThingSpeak")) return false;

    logDebug("Валидация конфигурации прошла успешно");
    return true;
}

void handleUploadError(const String& error)
{
    logError("Ошибка загрузки файла: %s", error.c_str());

    String html = generateErrorPage(400, "Ошибка загрузки файла: " + error);
    webServer.send(400, "text/html; charset=utf-8", html);
}

bool isFeatureAvailable(const String& feature)
{
    // Большинство функций недоступны в AP режиме
    if (currentWiFiMode == WiFiMode::AP)
    {
        // Разрешенные функции в AP режиме
        if (feature == "main" || feature == "save" || feature == "status")
        {
            return true;
        }
        return false;
    }
    return true;
}

// Перегрузка без параметров для обратной совместимости
bool isFeatureAvailable()
{
    return isFeatureAvailable("default");
}

void logWebRequest(const String& method, const String& uri, const String& clientIP)
{
    // Логирование только важных запросов, исключаем служебные
    if (uri.startsWith("/sensor_json") || uri.startsWith(API_SENSOR))
    {
        // API запросы логируем на уровне DEBUG
        logDebug("%s %s from %s", method.c_str(), uri.c_str(), clientIP.c_str());
    }
    else
    {
        // Обычные запросы на уровне INFO
        logInfo("Web: %s %s from %s", method.c_str(), uri.c_str(), clientIP.c_str());
    }
}

/**
 * @brief Генерация HTML ответа с ошибкой валидации
 * @param errorMsg Сообщение об ошибке
 * @return HTML ответ с ошибкой
 */
String generateValidationErrorResponse(const String& errorMsg)
{
    String content = "<h1>" UI_ICON_CONFIG " Настройки JXCT</h1>";
    content += generateFormError(errorMsg);

    // Здесь можно добавить восстановление формы с введенными данными
    content +=
        "<p><a href='javascript:history.back()' style='color: #4CAF50; text-decoration: none;'>← Назад к форме</a></p>";

    return generateBasePage("Ошибка настроек", content, UI_ICON_CONFIG);
}

/**
 * @brief Обработка критических ошибок сервера
 * @param error Описание ошибки
 */
void handleCriticalError(const String& error)
{
    logError("Критическая ошибка веб-сервера: %s", error.c_str());

    String html = generateErrorPage(500, "Внутренняя ошибка сервера: " + error);
    webServer.send(500, "text/html; charset=utf-8", html);
}

/**
 * @brief Проверка доступности маршрута в текущем режиме
 * @param uri URI запроса
 * @return true если маршрут доступен
 */
bool isRouteAvailable(const String& uri)
{
    if (currentWiFiMode == WiFiMode::AP)
    {
        // В AP режиме доступны только базовые маршруты
        return (uri == "/" || uri == "/save" || uri == "/status" || uri == "/reboot");
    }
    return true;
}

/**
 * @brief Middleware для проверки доступности маршрута
 * Используется в каждом маршруте, который недоступен в AP режиме
 */
bool checkRouteAccess(const String& routeName, const String& icon)
{
    if (!isRouteAvailable(webServer.uri()))
    {
        String html = generateApModeUnavailablePage(routeName, icon);
        webServer.send(403, "text/html; charset=utf-8", html);
        return false;
    }
    return true;
}
