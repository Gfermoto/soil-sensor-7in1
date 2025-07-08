/**
 * @file routes_config.cpp
 * @brief Маршруты для управления конфигурацией
 * @details Обработка запросов настройки интервалов, управления конфигурацией и API конфигурации
 */

#include <ArduinoJson.h>
#include "../../include/jxct_config_vars.h"
#include "../../include/jxct_constants.h"
#include "../../include/jxct_device_info.h"
#include "../../include/jxct_strings.h"
#include "../../include/jxct_ui_system.h"
#include "../../include/logger.h"
#include "../../include/validation_utils.h"     // ✅ Валидация входных данных
#include "../../include/web/csrf_protection.h"  // 🔒 CSRF защита
#include "../../include/web_routes.h"
#include "../wifi_manager.h"

namespace {
String importedJson;
}  // namespace

// Объявление static функции
static void sendConfigExportJson();

void setupConfigRoutes()
{
    // Красивая страница интервалов и фильтров (оригинальный дизайн)
    webServer.on(
        "/intervals", HTTP_GET,
        []()
        {
            logWebRequest("GET", "/intervals", webServer.client().remoteIP().toString());

            if (currentWiFiMode == WiFiMode::AP)
            {
                webServer.send(HTTP_OK, HTTP_CONTENT_TYPE_HTML,
                               generateApModeUnavailablePage("Интервалы", UI_ICON_INTERVALS));
                return;
            }

            String html = generatePageHeader("Интервалы и фильтры", UI_ICON_INTERVALS);
            html += navHtml();
            html += "<h1>" UI_ICON_INTERVALS " Настройка интервалов и фильтров</h1>";
            html += "<form action='/save_intervals' method='post'>";
            html += getCSRFHiddenField();  // Добавляем CSRF токен

            html += "<div class='section'><h2>📊 Интервалы опроса и публикации</h2>";
            html += "<div class='form-group'><label for='sensor_interval'>Интервал опроса датчика (сек):</label>";
            html += "<input type='number' id='sensor_interval' name='sensor_interval' min='" +
                    String(CONFIG_SENSOR_INTERVAL_MIN_SEC) + "' max='" + String(CONFIG_SENSOR_INTERVAL_MAX_SEC) +
                    "' value='" + String(config.sensorReadInterval / CONVERSION_SEC_TO_MS) + "' required>";
            html += "<div class='help'>" + String(CONFIG_SENSOR_INTERVAL_MIN_SEC) + "-" +
                    String(CONFIG_SENSOR_INTERVAL_MAX_SEC) +
                    " сек. Текущее: " + String(config.sensorReadInterval / CONVERSION_SEC_TO_MS) +
                    " сек (по умолчанию: 1 сек)</div></div>";

            html += "<div class='form-group'><label for='mqtt_interval'>Интервал MQTT публикации (мин):</label>";
            html += "<input type='number' id='mqtt_interval' name='mqtt_interval' min='" +
                    String(CONFIG_MQTT_INTERVAL_MIN_MIN) + "' max='" + String(CONFIG_MQTT_INTERVAL_MAX_MIN) +
                    "' value='" + String(config.mqttPublishInterval / CONVERSION_MIN_TO_MS) + "' required>";
            html += "<div class='help'>" + String(CONFIG_MQTT_INTERVAL_MIN_MIN) + "-" +
                    String(CONFIG_MQTT_INTERVAL_MAX_MIN) +
                    " мин. Текущее: " + String(config.mqttPublishInterval / CONVERSION_MIN_TO_MS) + " мин</div></div>";

            html += "<div class='form-group'><label for='ts_interval'>Интервал ThingSpeak (мин):</label>";
            html += "<input type='number' id='ts_interval' name='ts_interval' min='" +
                    String(CONFIG_THINGSPEAK_INTERVAL_MIN_MIN) + "' max='" +
                    String(CONFIG_THINGSPEAK_INTERVAL_MAX_MIN) + "' value='" +
                    String(config.thingSpeakInterval / CONVERSION_MIN_TO_MS) + "' required>";
            html += "<div class='help'>" + String(CONFIG_THINGSPEAK_INTERVAL_MIN_MIN) + "-" +
                    String(CONFIG_THINGSPEAK_INTERVAL_MAX_MIN) +
                    " мин. Текущее: " + String(config.thingSpeakInterval / CONVERSION_MIN_TO_MS) + " мин</div></div>";

            html +=
                "<div class='form-group'><label for='web_interval'>Интервал обновления веб-интерфейса (сек):</label>";
            html += "<input type='number' id='web_interval' name='web_interval' min='" +
                    String(CONFIG_WEB_INTERVAL_MIN_SEC) + "' max='" + String(CONFIG_WEB_INTERVAL_MAX_SEC) +
                    "' value='" + String(config.webUpdateInterval / CONVERSION_SEC_TO_MS) + "' required>";
            html +=
                "<div class='help'>" + String(CONFIG_WEB_INTERVAL_MIN_SEC) + "-" + String(CONFIG_WEB_INTERVAL_MAX_SEC) +
                " сек. Текущее: " + String(config.webUpdateInterval / CONVERSION_SEC_TO_MS) + " сек</div></div></div>";

            html += "<div class='section'><h2>🎯 Пороги дельта-фильтра</h2>";
            html += "<div class='form-group'><label for='delta_temp'>Порог температуры (°C):</label>";
            html += "<input type='number' id='delta_temp' name='delta_temp' min='0.1' max='5.0' step='0.1' value='" +
                    String(config.deltaTemperature) + "' required>";
            html += "<div class='help'>0.1-5.0°C. Публикация при изменении более чем на это значение</div></div>";

            html += "<div class='form-group'><label for='delta_hum'>Порог влажности (%):</label>";
            html += "<input type='number' id='delta_hum' name='delta_hum' min='" + String(CONFIG_DELTA_HUMIDITY_MIN) +
                    "' max='" + String(CONFIG_DELTA_HUMIDITY_MAX) + "' step='" + String(CONFIG_STEP_HUMIDITY) +
                    "' value='" + String(config.deltaHumidity) + "' required>";
            html += "<div class='help'>" + String(CONFIG_DELTA_HUMIDITY_MIN) + "-" + String(CONFIG_DELTA_HUMIDITY_MAX) +
                    "%. Публикация при изменении более чем на это значение</div></div>";

            html += "<div class='form-group'><label for='delta_ph'>Порог pH:</label>";
            html += "<input type='number' id='delta_ph' name='delta_ph' min='" + String(CONFIG_DELTA_PH_MIN) +
                    "' max='" + String(CONFIG_DELTA_PH_MAX) + "' step='" + String(CONFIG_STEP_PH) + "' value='" +
                    String(config.deltaPh) + "' required>";
            html += "<div class='help'>" + String(CONFIG_DELTA_PH_MIN) + "-" + String(CONFIG_DELTA_PH_MAX) +
                    ". Публикация при изменении более чем на это значение</div></div>";

            html += "<div class='form-group'><label for='delta_ec'>Порог EC (µS/cm):</label>";
            html += "<input type='number' id='delta_ec' name='delta_ec' min='" + String(CONFIG_DELTA_EC_MIN) +
                    "' max='" + String(CONFIG_DELTA_EC_MAX) + "' value='" + String((int)config.deltaEc) + "' required>";
            html += "<div class='help'>" + String(CONFIG_DELTA_EC_MIN) + "-" + String(CONFIG_DELTA_EC_MAX) +
                    " µS/cm. Публикация при изменении более чем на это значение</div></div>";

            html += "<div class='form-group'><label for='delta_npk'>Порог NPK (mg/kg):</label>";
            html += "<input type='number' id='delta_npk' name='delta_npk' min='" + String(CONFIG_DELTA_NPK_MIN) +
                    "' max='" + String(CONFIG_DELTA_NPK_MAX) + "' value='" + String((int)config.deltaNpk) +
                    "' required>";
            html += "<div class='help'>" + String(CONFIG_DELTA_NPK_MIN) + "-" + String(CONFIG_DELTA_NPK_MAX) +
                    " mg/kg. Публикация при изменении более чем на это значение</div></div></div>";

            html += "<div class='section'><h2>📈 Скользящее среднее</h2>";
            html += "<div class='form-group'><label for='avg_window'>Размер окна усреднения:</label>";
            html += "<input type='number' id='avg_window' name='avg_window' min='" + String(CONFIG_AVG_WINDOW_MIN) +
                    "' max='" + String(CONFIG_AVG_WINDOW_MAX) + "' value='" + String(config.movingAverageWindow) +
                    "' required>";
            html += "<div class='help'>" + String(CONFIG_AVG_WINDOW_MIN) + "-" + String(CONFIG_AVG_WINDOW_MAX) +
                    " измерений. Больше = плавнее, но медленнее реакция</div></div>";

            html += "<div class='form-group'><label for='force_cycles'>Принудительная публикация (циклов):</label>";
            html += "<input type='number' id='force_cycles' name='force_cycles' min='" +
                    String(CONFIG_FORCE_CYCLES_MIN) + "' max='" + String(CONFIG_FORCE_CYCLES_MAX) + "' value='" +
                    String(config.forcePublishCycles) + "' required>";
            html += "<div class='help'>" + String(CONFIG_FORCE_CYCLES_MIN) + "-" + String(CONFIG_FORCE_CYCLES_MAX) +
                    " циклов. Публикация каждые N циклов даже без изменений</div></div>";

            // Новые настройки алгоритма и фильтра выбросов
            html += "<div class='form-group'><label for='filter_algo'>Алгоритм обработки данных:</label>";
            html += "<select id='filter_algo' name='filter_algo' required>";
            html += "<option value='0'" + String(config.filterAlgorithm == 0 ? " selected" : "") +
                    ">Среднее арифметическое</option>";
            html += "<option value='1'" + String(config.filterAlgorithm == 1 ? " selected" : "") +
                    ">Медианное значение</option>";
            html += "</select>";
            html += "<div class='help'>Среднее - быстрее, медиана - устойчивее к выбросам</div></div>";

            html += "<div class='form-group'><label for='outlier_filter'>Фильтр выбросов >2σ:</label>";
            html += "<select id='outlier_filter' name='outlier_filter' required>";
            html += "<option value='0'" + String(config.outlierFilterEnabled == 0 ? " selected" : "") +
                    ">Отключен</option>";
            html +=
                "<option value='1'" + String(config.outlierFilterEnabled == 1 ? " selected" : "") + ">Включен</option>";
            html += "</select>";
            html +=
                "<div class='help'>Автоматически отбрасывает измерения, отклоняющиеся более чем на 2 "
                "сигма</div></div></div>";

            html += generateButton(ButtonType::PRIMARY, ButtonConfig{UI_ICON_SAVE, "Сохранить настройки", ""});
            html += "</form>";
            html += generateButton(ButtonType::SECONDARY,
                                   ButtonConfig{UI_ICON_RESET, "Сбросить к умолчанию (1 сек + мин. фильтрация)", ""});
            html += "</form>";
            html += generatePageFooter();

            webServer.send(HTTP_OK, HTTP_CONTENT_TYPE_HTML, html);
        });

    // Обработчик сохранения настроек интервалов
    webServer.on(
        "/save_intervals", HTTP_POST,
        []()
        {
            logWebRequest("POST", "/save_intervals", webServer.client().remoteIP().toString());

            // ✅ CSRF защита
            if (!checkCSRFSafety())
            {
                logWarnSafe("\1", webServer.client().remoteIP().toString().c_str());
                const String html = generateErrorPage(HTTP_FORBIDDEN, "Forbidden: Недействительный CSRF токен");
                webServer.send(HTTP_FORBIDDEN, HTTP_CONTENT_TYPE_HTML, html);
                return;
            }

            if (currentWiFiMode == WiFiMode::AP)
            {
                webServer.send(HTTP_FORBIDDEN, HTTP_CONTENT_TYPE_PLAIN, "Недоступно в режиме точки доступа");
                return;
            }

            // ======= ВАЛИДАЦИЯ ВХОДНЫХ ДАННЫХ =======
            const unsigned long sensorMs = webServer.arg("sensor_interval").toInt() * CONVERSION_SEC_TO_MS;
            const unsigned long mqttMs = webServer.arg("mqtt_interval").toInt() * CONVERSION_MIN_TO_MS;
            const unsigned long tsMs = webServer.arg("ts_interval").toInt() * CONVERSION_MIN_TO_MS;
            const unsigned long webMs = webServer.arg("web_interval").toInt() * CONVERSION_SEC_TO_MS;

            const auto valSensor = validateSensorReadInterval(sensorMs);
            const auto valMqtt = validateMQTTPublishInterval(mqttMs);
            auto valTs = validateThingSpeakInterval(tsMs);

            if (!valSensor.isValid || !valMqtt.isValid || !valTs.isValid)
            {
                String errorMessage = "Ошибка валидации интервалов: ";
                if (!valSensor.isValid) {
                    errorMessage += valSensor.message;
                } else if (!valMqtt.isValid) {
                    errorMessage += valMqtt.message;
                } else {
                    errorMessage += valTs.message;
                }
                const String html = generateErrorPage(HTTP_BAD_REQUEST, errorMessage);
                webServer.send(HTTP_BAD_REQUEST, HTTP_CONTENT_TYPE_HTML, html);
                return;
            }

            // ======= СОХРАНЯЕМ НАСТРОЙКИ =======
            config.sensorReadInterval = sensorMs;
            config.mqttPublishInterval = mqttMs;
            config.thingSpeakInterval = tsMs;
            config.webUpdateInterval = webMs;

            // Сохраняем пороги дельта-фильтра
            config.deltaTemperature = webServer.arg("delta_temp").toFloat();
            config.deltaHumidity = webServer.arg("delta_hum").toFloat();
            config.deltaPh = webServer.arg("delta_ph").toFloat();
            config.deltaEc = webServer.arg("delta_ec").toFloat();
            config.deltaNpk = webServer.arg("delta_npk").toFloat();

            // Сохраняем настройки скользящего среднего
            config.movingAverageWindow = webServer.arg("avg_window").toInt();
            config.forcePublishCycles = webServer.arg("force_cycles").toInt();

            // Сохраняем новые настройки алгоритма и фильтра выбросов
            config.filterAlgorithm = webServer.arg("filter_algo").toInt();
            config.outlierFilterEnabled = webServer.arg("outlier_filter").toInt();

            // Сохраняем в NVS
            saveConfig();

            String html =
                "<!DOCTYPE html><html><head><meta charset='UTF-8'><meta http-equiv='refresh' "
                "content='3;url=/intervals'>";
            html += "<title>" UI_ICON_SUCCESS " Настройки сохранены</title>";
            html += "<style>" + String(getUnifiedCSS()) + "</style></head><body><div class='container'>";
            html += "<h1>" UI_ICON_SUCCESS " Настройки интервалов сохранены!</h1>";
            html += "<div class='msg msg-success'>" UI_ICON_SUCCESS " Новые настройки вступили в силу</div>";
            html += "<p><strong>Текущие интервалы:</strong><br>";
            html += "📊 Датчик: " + String(config.sensorReadInterval / CONVERSION_SEC_TO_MS) + " сек<br>";
            html += "📡 MQTT: " + String(config.mqttPublishInterval / CONVERSION_MIN_TO_MS) + " мин<br>";
            html += "📈 ThingSpeak: " + String(config.thingSpeakInterval / CONVERSION_MIN_TO_MS) + " мин</p>";
            html += "<p><em>Возврат к настройкам через 3 секунды...</em></p>";
            html += "</div>" + String(getToastHTML()) + "</body></html>";
            webServer.send(HTTP_OK, HTTP_CONTENT_TYPE_HTML, html);
        });

    // Сброс интервалов к умолчанию
    webServer.on("/reset_intervals", HTTP_GET,
                 []()
                 {
                     logWebRequest("GET", "/reset_intervals", webServer.client().remoteIP().toString());

                     if (currentWiFiMode == WiFiMode::AP)
                     {
                         webServer.send(HTTP_FORBIDDEN, HTTP_CONTENT_TYPE_PLAIN, "Недоступно в режиме точки доступа");
                         return;
                     }

                     // Сбрасываем к умолчанию (МИНИМАЛЬНАЯ ФИЛЬТРАЦИЯ + ЧАСТЫЙ MQTT)
                     config.sensorReadInterval = SENSOR_READ_INTERVAL;
                     config.mqttPublishInterval = MQTT_PUBLISH_INTERVAL;
                     config.thingSpeakInterval = THINGSPEAK_INTERVAL;
                     config.webUpdateInterval = WEB_UPDATE_INTERVAL;
                     config.deltaTemperature = DELTA_TEMPERATURE;       // 0.1°C
                     config.deltaHumidity = DELTA_HUMIDITY;             // 0.5%
                     config.deltaPh = DELTA_PH;                         // 0.01 pH
                     config.deltaEc = DELTA_EC;                         // 10 µS/cm
                     config.deltaNpk = DELTA_NPK;                       // 1 mg/kg
                     config.movingAverageWindow = 5;                    // минимальное окно
                     config.forcePublishCycles = FORCE_PUBLISH_CYCLES;  // каждые 5 циклов
                     config.filterAlgorithm = 0;                        // среднее
                     config.outlierFilterEnabled = 0;                   // отключен

                     saveConfig();

                     String html =
                         "<!DOCTYPE html><html><head><meta charset='UTF-8'><meta http-equiv='refresh' "
                         "content='2;url=/intervals'>";
                     html += "<title>" UI_ICON_RESET " Сброс настроек</title>";
                     html += "<style>" + String(getUnifiedCSS()) + "</style></head><body><div class='container'>";
                     html += "<h1>" UI_ICON_RESET " Настройки сброшены</h1>";
                     html += "<div class='msg msg-success'>" UI_ICON_SUCCESS
                             " Настройки интервалов возвращены к значениям по умолчанию</div>";
                     html += "<p><em>Возврат к настройкам через 2 секунды...</em></p>";
                     html += "</div>" + String(getToastHTML()) + "</body></html>";
                     webServer.send(HTTP_OK, HTTP_CONTENT_TYPE_HTML, html);
                 });

    // Страница управления конфигурацией
    webServer.on("/config_manager", HTTP_GET,
                 []()
                 {
                     logWebRequest("GET", "/config_manager", webServer.client().remoteIP().toString());

                     if (currentWiFiMode == WiFiMode::AP)
                     {
                         String html = "<!DOCTYPE html><html><head><meta charset='UTF-8'><title>" UI_ICON_FOLDER
                                       " Конфигурация</title>";
                         html += "<style>" + String(getUnifiedCSS()) + "</style></head><body><div class='container'>";
                         html += "<h1>" UI_ICON_FOLDER " Конфигурация</h1>";
                         html += "<div class='msg msg-error'>" UI_ICON_ERROR
                                 " Недоступно в режиме точки доступа</div></div></body></html>";
                         webServer.send(HTTP_OK, HTTP_CONTENT_TYPE_HTML, html);
                         return;
                     }

                     String html =
                         "<!DOCTYPE html><html><head><meta charset='UTF-8'><meta name='viewport' "
                         "content='width=device-width, initial-scale=1.0'>";
                     html += "<title>" UI_ICON_FOLDER " Управление конфигурацией JXCT</title>";
                     html += "<style>" + String(getUnifiedCSS()) + "</style></head><body><div class='container'>";
                     html += navHtml();
                     html += "<h1>" UI_ICON_FOLDER " Управление конфигурацией</h1>";

                     if (webServer.hasArg("import_ok"))
                     {
                         html += "<div class='msg msg-success'>✅ Конфигурация успешно импортирована и сохранена</div>";
                     }

                     html += "<div class='section'>";
                     html += "<h2>📤 Экспорт конфигурации</h2>";
                     html += "<p>Скачайте текущую конфигурацию в формате JSON (пароли заменены на заглушки):</p>";
                     html += generateButton(ButtonType::PRIMARY, ButtonConfig{"📥", "Скачать конфигурацию", ""});
                     html += "</div>";

                     html += "<div class='section'>";
                     html += "<h2>📥 Импорт конфигурации</h2>";
                     html += "<p>Загрузите файл конфигурации для восстановления настроек:</p>";
                     html += "<form action='/api/config/import' method='post' enctype='multipart/form-data'>";
                     html += getCSRFHiddenField();  // Добавляем CSRF токен
                     html += "<input type='file' name='config' accept='.json' required>";
                     html += generateButton(ButtonType::SECONDARY, ButtonConfig{"📤", "Загрузить конфигурацию", ""});
                     html += "</form>";
                     html += "</div>";

                     html += "</div>" + String(getToastHTML()) + "</body></html>";
                     webServer.send(HTTP_OK, HTTP_CONTENT_TYPE_HTML, html);
                 });

    // API v1 конфигурация
    webServer.on(API_CONFIG_EXPORT, HTTP_GET, [](){ sendConfigExportJson(); });

    // Импорт конфигурации через multipart/form-data (файл JSON)
    webServer.on(
        "/api/config/import", HTTP_POST,
        // Финальный обработчик после загрузки
        []()
        {
            logWebRequest("POST", "/api/config/import", webServer.client().remoteIP().toString());

            if (currentWiFiMode == WiFiMode::AP)
            {
                webServer.send(HTTP_FORBIDDEN, "application/json", R"({"error":"Недоступно в режиме AP"})");
                importedJson = "";
                return;
            }

            // ✅ CSRF защита - критическая операция импорта конфигурации!
            if (!checkCSRFSafety())
            {
                logWarnSafe("\1", webServer.client().remoteIP().toString().c_str());
                webServer.send(HTTP_FORBIDDEN, "application/json", R"({"error":"CSRF token invalid"})");
                importedJson = "";
                return;
            }

            // Парсим накопленный JSON
            StaticJsonDocument<CONFIG_JSON_DOC_SIZE> doc;
            DeserializationError err = deserializeJson(doc, importedJson);
            if (err)
            {
                const String resp = String("{\"error\":\"Ошибка JSON: ") + err.c_str() + "\"}";
                webServer.send(HTTP_BAD_REQUEST, "application/json", resp);
                importedJson = "";
                return;
            }

            // --- Применяем конфигурацию --- (минимальный набор, расширяйте по необходимости)
            if (doc.containsKey("wifi"))
            {
                JsonObject wifi = doc["wifi"];
                strlcpy(config.ssid, wifi["ssid"].as<const char*>(), sizeof(config.ssid));
                strlcpy(config.password, wifi["password"].as<const char*>(), sizeof(config.password));
            }
            if (doc.containsKey("mqtt"))
            {
                JsonObject mqtt = doc["mqtt"];
                config.flags.mqttEnabled = mqtt["enabled"].as<bool>(); // NOLINT(readability-misplaced-array-index)
                strlcpy(config.mqttServer, mqtt["server"].as<const char*>(), sizeof(config.mqttServer));
                config.mqttPort = mqtt["port"].as<int>(); // NOLINT(readability-misplaced-array-index)
                strlcpy(config.mqttUser, mqtt["user"].as<const char*>(), sizeof(config.mqttUser));
                strlcpy(config.mqttPassword, mqtt["password"].as<const char*>(), sizeof(config.mqttPassword));
            }

            // Сохраняем в NVS
            saveConfig();
            importedJson = "";

            // Отправляем 303 Redirect, чтобы браузер вернулся к менеджеру конфигурации
            webServer.sendHeader("Location", "/config_manager?import_ok=1", true);
            webServer.send(HTTP_SEE_OTHER, "text/plain", "Redirect");
        },
        // uploadHandler: накапливаем файл
        []()
        {
            HTTPUpload& upload = webServer.upload();
            if (upload.status == UPLOAD_FILE_START)
            {
                importedJson = "";
            }
            else if (upload.status == UPLOAD_FILE_WRITE)
            {
                importedJson += String((const char*)upload.buf, upload.currentSize);
            }
            else if (upload.status == UPLOAD_FILE_END)
            {
                // ничего, финальное действие в основном хендлере
            }
        });

    logDebug("Маршруты конфигурации настроены: /intervals, /config_manager, /api/v1/config/export");
}

// Определение функции вне namespace
static void sendConfigExportJson()
{
    logWebRequest("GET", webServer.uri(), webServer.client().remoteIP().toString());

    if (currentWiFiMode == WiFiMode::AP)
    {
        webServer.send(HTTP_FORBIDDEN, "application/json", "{\"error\":\"Недоступно в режиме точки доступа\"}");
        return;
    }

    StaticJsonDocument<CONFIG_JSON_ROOT_SIZE> root;

    // MQTT
    JsonObject mqtt = root.createNestedObject("mqtt");
    mqtt["enabled"] = (bool)config.flags.mqttEnabled; // NOLINT(readability-misplaced-array-index)
    mqtt["server"] = "YOUR_MQTT_SERVER_HERE"; // NOLINT(readability-misplaced-array-index)
    mqtt["port"] = config.mqttPort; // NOLINT(readability-misplaced-array-index)
    mqtt["user"] = "YOUR_MQTT_USER_HERE"; // NOLINT(readability-misplaced-array-index)
    mqtt["password"] = "YOUR_MQTT_PASSWORD_HERE"; // NOLINT(readability-misplaced-array-index)

    // ThingSpeak
    JsonObject thingSpeakJson = root.createNestedObject("thingspeak");
    thingSpeakJson["enabled"] = (bool)config.flags.thingSpeakEnabled; // NOLINT(readability-misplaced-array-index)
    thingSpeakJson["channel_id"] = "YOUR_CHANNEL_ID_HERE"; // NOLINT(readability-misplaced-array-index)
    thingSpeakJson["api_key"] = "YOUR_API_KEY_HERE"; // NOLINT(readability-misplaced-array-index)

    // Intervals
    JsonObject intervals = root.createNestedObject("intervals");
    intervals["sensor_read"] = config.sensorReadInterval; // NOLINT(readability-misplaced-array-index)
    intervals["mqtt_publish"] = config.mqttPublishInterval; // NOLINT(readability-misplaced-array-index)
    intervals["thingspeak"] = config.thingSpeakInterval; // NOLINT(readability-misplaced-array-index)
    intervals["web_update"] = config.webUpdateInterval; // NOLINT(readability-misplaced-array-index)

    // Filters
    JsonObject filters = root.createNestedObject("filters");
    filters["delta_temperature"] = config.deltaTemperature; // NOLINT(readability-misplaced-array-index)
    filters["delta_humidity"] = config.deltaHumidity; // NOLINT(readability-misplaced-array-index)
    filters["delta_ph"] = config.deltaPh; // NOLINT(readability-misplaced-array-index)
    filters["delta_ec"] = config.deltaEc; // NOLINT(readability-misplaced-array-index)
    filters["delta_npk"] = config.deltaNpk; // NOLINT(readability-misplaced-array-index)
    filters["moving_average_window"] = config.movingAverageWindow; // NOLINT(readability-misplaced-array-index)
    filters["force_publish_cycles"] = config.forcePublishCycles; // NOLINT(readability-misplaced-array-index)
    filters["filter_algorithm"] = config.filterAlgorithm; // NOLINT(readability-misplaced-array-index)
    filters["outlier_filter_enabled"] = config.outlierFilterEnabled; // NOLINT(readability-misplaced-array-index)

    // Device flags
    JsonObject device = root.createNestedObject("device");
    device["use_real_sensor"] = (bool)config.flags.useRealSensor; // NOLINT(readability-misplaced-array-index)
    device["hass_enabled"] = (bool)config.flags.hassEnabled; // NOLINT(readability-misplaced-array-index)

    root["export_timestamp"] = millis(); // NOLINT(readability-misplaced-array-index)

    String json;
    serializeJson(root, json);

    webServer.sendHeader(R"(Content-Disposition)", R"(attachment; filename="jxct_config_)" + String(millis()) + R"(.json")");
    webServer.send(HTTP_OK, "application/json", json);
}
