#include "../../include/web_routes.h"
#include "../../include/logger.h"
#include "../../include/jxct_config_vars.h"
#include "../../include/jxct_ui_system.h"
#include "../wifi_manager.h"

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
    if (webServer.hasArg("mqtt_port"))
    {
        int port = webServer.arg("mqtt_port").toInt();
        if (port < 1 || port > 65535)
        {
            logWarn("Валидация: некорректный MQTT порт: %d", port);
            return false;
        }
    }

    if (webServer.hasArg("ntp_interval"))
    {
        int interval = webServer.arg("ntp_interval").toInt();
        if (interval < 10000 || interval > 86400000)
        {  // от 10 сек до 24 часов
            logWarn("Валидация: некорректный NTP интервал: %d", interval);
            return false;
        }
    }

    // Валидация интервалов датчика
    if (webServer.hasArg("sensor_read"))
    {
        int interval = webServer.arg("sensor_read").toInt();
        if (interval < 1000 || interval > 300000)
        {  // от 1 сек до 5 мин
            logWarn("Валидация: некорректный интервал чтения датчика: %d", interval);
            return false;
        }
    }

    if (webServer.hasArg("mqtt_publish"))
    {
        int interval = webServer.arg("mqtt_publish").toInt();
        if (interval < 1000 || interval > 3600000)
        {  // от 1 сек до 1 часа
            logWarn("Валидация: некорректный интервал MQTT публикации: %d", interval);
            return false;
        }
    }

    if (webServer.hasArg("thingspeak_interval"))
    {
        int interval = webServer.arg("thingspeak_interval").toInt();
        if (interval < 15000 || interval > 7200000)
        {  // от 15 сек до 2 часов (лимит ThingSpeak)
            logWarn("Валидация: некорректный интервал ThingSpeak: %d", interval);
            return false;
        }
    }

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
    if (uri.startsWith("/sensor_json") || uri.startsWith("/api/sensor"))
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
 * @param formData Данные формы для восстановления
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