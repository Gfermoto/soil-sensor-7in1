#include "../../include/jxct_config_vars.h"
#include "../../include/jxct_strings.h"
#include "../../include/jxct_ui_system.h"
#include "../../include/logger.h"
#include "../../include/web_routes.h"
#include "../wifi_manager.h"

// Структура для типобезопасности (предотвращение перепутывания min/max)
struct ValidationRange
{
    int minValue = 0;
    int maxValue = 0;
    
public:
    // Builder для предотвращения ошибок с параметрами
    struct Builder {
        int min = 0;
        int max = 0;
        Builder& minValue(int minValue) { min = minValue; return *this; }
        Builder& maxValue(int maxValue) { max = maxValue; return *this; }
        ValidationRange build() const {
            return {min, max};
        }
    };
    static Builder builder() { return {}; }
};

// Структура для веб-запроса (предотвращение перепутывания String параметров)
struct HttpRequest
{
    String method = "";
    String uri = "";
    String clientIP = "";
    
public:
    // Builder для предотвращения ошибок с параметрами
    struct Builder {
        String methodValue;
        String uriValue;
        String clientIPValue;
        Builder& method(const String& method) { methodValue = method; return *this; }
        Builder& uri(const String& uri) { uriValue = uri; return *this; }
        Builder& clientIP(const String& clientIP) { clientIPValue = clientIP; return *this; }
        HttpRequest build() const { // NOLINT(readability-convert-member-functions-to-static)
            HttpRequest result;
            result.method = methodValue;
            result.uri = uriValue;
            result.clientIP = clientIPValue;
            return result;
        }
    };
    static Builder builder() { return {}; }
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
    if (!validateInterval("mqtt_port", ValidationRange::builder().minValue(1).maxValue(65535).build(), "MQTT порт"))
    {
        return false;
    }
    if (!validateInterval("ntp_interval", ValidationRange::builder().minValue(10000).maxValue(86400000).build(), "NTP интервал"))
    {
        return false;
    }
    if (!validateInterval("sensor_read", ValidationRange::builder().minValue(1000).maxValue(300000).build(), "интервал чтения датчика"))
    {
        return false;
    }
    if (!validateInterval("mqtt_publish", ValidationRange::builder().minValue(1000).maxValue(3600000).build(), "интервал MQTT публикации"))
    {
        return false;
    }
    if (!validateInterval("thingspeak_interval", ValidationRange::builder().minValue(15000).maxValue(7200000).build(), "интервал ThingSpeak"))
    {
        return false;
    }

    logDebug("Валидация конфигурации прошла успешно");
    return true;
}

namespace {
void handleUploadError(const String& error)
{
    logErrorSafe("\1", error.c_str());

    const String html = generateErrorPage(400, "Ошибка загрузки файла: " + error);
    webServer.send(400, "text/html; charset=utf-8", html);
}
} // namespace

namespace
{
bool isFeatureAvailable(const String& feature)
{
    // Большинство функций недоступны в AP режиме
    if (currentWiFiMode == WiFiMode::AP)
    {
        // Разрешенные функции в AP режиме
        return feature == "main" || feature == "save" || feature == "status";
    }
    return true;
}

}  // namespace

// Перегрузка без параметров для обратной совместимости
bool isFeatureAvailable()
{
    return isFeatureAvailable("default");
}





/**
 * @brief Генерация HTML ответа с ошибкой валидации
 * @param errorMsg Сообщение об ошибке
 * @return HTML ответ с ошибкой
 */
static String generateValidationErrorResponse(const String& errorMsg) // NOLINT(misc-use-internal-linkage,readability-convert-member-functions-to-static)
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
static void handleCriticalError(const String& error) // NOLINT(misc-use-internal-linkage,readability-convert-member-functions-to-static)
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
static bool isRouteAvailable(const String& uri) // NOLINT(misc-use-internal-linkage,readability-convert-member-functions-to-static)
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
namespace
{
bool checkRouteAccess(const String& routeName, const String& icon) // NOLINT(bugprone-easily-swappable-parameters)
{
    if (!isRouteAvailable(webServer.uri()))
    {
        const String html = generateApModeUnavailablePage(routeName, icon);
        webServer.send(403, "text/html; charset=utf-8", html);
        return false;
    }
    return true;
}

}  // namespace
