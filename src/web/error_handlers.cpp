#include "../../include/jxct_config_vars.h"
#include "../../include/jxct_strings.h"
#include "../../include/jxct_ui_system.h"
#include "../../include/logger.h"
#include "../../include/web_routes.h"
#include "../wifi_manager.h"

// Структура для типобезопасности (предотвращение перепутывания min/max)
struct ValidationRange
{
    int minValue;
    int maxValue;

    ValidationRange(int minValue, int maxValue) : minValue(minValue), maxValue(maxValue) {}
};

// Структура для веб-запроса (предотвращение перепутывания String параметров)
struct HttpRequest
{
    String method;
    String uri;
    String clientIP;

    HttpRequest(const String& methodValue, const String& uriValue, const String& clientIPValue)
        : method(methodValue), uri(uriValue), clientIP(clientIPValue)
    {
    }
};

// Вспомогательные функции для валидации интервалов — скрыты во внутреннем безымянном пространстве имён,
// чтобы ограничить область видимости текущим файлом и устранить предупреждение clang-tidy
namespace
{

// ✅ Типобезопасная версия (предотвращает перепутывание min/max)
bool validateInterval(const String& argName, const ValidationRange& range, const String& description)
{
    if (webServer.hasArg(argName))
    {
        const int value = webServer.arg(argName).toInt();
        if (value < range.minValue || value > range.maxValue)
        {
            logWarnSafe("\1", description.c_str(), value, range.minValue, range.maxValue);
            return false;
        }
    }
    return true;
}

}  // namespace

void setupErrorHandlers()
{
    // Обработчик 404 - страница не найдена
    webServer.onNotFound(
        []()
        {
            const String uri = webServer.uri();
            const String method = webServer.method() == HTTP_GET ? "GET" : "POST";

            logWebRequest(method, uri, webServer.client().remoteIP().toString());

            logWarnSafe("\1", method.c_str(), uri.c_str());

            const String html = generateErrorPage(404, "Страница не найдена");
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
    if (!validateInterval("mqtt_port", ValidationRange(1, 65535), "MQTT порт"))
    {
        return false;
    }
    if (!validateInterval("ntp_interval", ValidationRange(10000, 86400000), "NTP интервал"))
    {
        return false;
    }
    if (!validateInterval("sensor_read", ValidationRange(1000, 300000), "интервал чтения датчика"))
    {
        return false;
    }
    if (!validateInterval("mqtt_publish", ValidationRange(1000, 3600000), "интервал MQTT публикации"))
    {
        return false;
    }
    if (!validateInterval("thingspeak_interval", ValidationRange(15000, 7200000), "интервал ThingSpeak"))
    {
        return false;
    }

    logDebug("Валидация конфигурации прошла успешно");
    return true;
}

void handleUploadError(const String& error)
{
    logErrorSafe("\1", error.c_str());

    const String html = generateErrorPage(400, "Ошибка загрузки файла: " + error);
    webServer.send(400, "text/html; charset=utf-8", html);
}

// NOLINTNEXTLINE(misc-use-anonymous-namespace)
static bool isFeatureAvailable(const String& feature)
{
    // Большинство функций недоступны в AP режиме
    if (currentWiFiMode == WiFiMode::AP)
    {
        // Разрешенные функции в AP режиме
        return feature == "main" || feature == "save" || feature == "status";
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
        logDebugSafe("\1", method.c_str(), uri.c_str(), clientIP.c_str());
    }
    else
    {
        // Обычные запросы на уровне INFO
        logInfoSafe("\1", method.c_str(), uri.c_str(), clientIP.c_str());
    }
}

// NOLINTNEXTLINE(misc-use-anonymous-namespace)
static void logWebRequest(const HttpRequest& request)
{
    logWebRequest(request.method, request.uri, request.clientIP);
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
    logErrorSafe("\1", error.c_str());

    const String html = generateErrorPage(500, "Внутренняя ошибка сервера: " + error);
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
        const String html = generateApModeUnavailablePage(routeName, icon);
        webServer.send(403, "text/html; charset=utf-8", html);
        return false;
    }
    return true;
}
