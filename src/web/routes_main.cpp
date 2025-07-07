#include "../../include/jxct_config_vars.h"
#include "../../include/jxct_constants.h"
#include "../../include/jxct_ui_system.h"
#include "../../include/logger.h"
#include "../../include/validation_utils.h"  // ✅ Валидация
#include "../../include/web_routes.h"
#include "../wifi_manager.h"

void setupMainRoutes()
{
    // Главная страница — настройки Wi-Fi (и MQTT/ThingSpeak/HASS в STA)
    webServer.on("/", HTTP_GET, handleRoot);
    webServer.on("/", HTTP_POST, handleRoot);  // Добавляем обработку POST для авторизации

    // Сохранение настроек
    webServer.on(
        "/save", HTTP_POST,
        []()
        {
            logWebRequest("POST", "/save", webServer.client().remoteIP().toString());

            // CSRF защита (безопасная - не ломает API)
            if (!checkCSRFSafety())
            {
                logWarnSafe("\1", webServer.client().remoteIP().toString().c_str());
                const String html = generateErrorPage(HTTP_FORBIDDEN, "Forbidden: Недействительный CSRF токен");
                webServer.send(HTTP_FORBIDDEN, HTTP_CONTENT_TYPE_HTML, html);
                return;
            }

            // ====== ВАЛИДАЦИЯ ЧЕРЕЗ validation_utils ======
            const ValidationResult ssidRes = validateSSID(webServer.arg("ssid"));
            const ValidationResult passRes = validatePassword(webServer.arg("password"));

            if (!ssidRes.isValid || !passRes.isValid)
            {
                String msg = !ssidRes.isValid ? ssidRes.message : passRes.message;
                String html = generateErrorPage(HTTP_BAD_REQUEST, msg);
                webServer.send(HTTP_BAD_REQUEST, HTTP_CONTENT_TYPE_HTML, html);
                return;
            }

            if (currentWiFiMode == WiFiMode::STA)
            {
                if (webServer.hasArg("mqtt_enabled"))
                {
                    ValidationResult hostRes = validateMQTTServer(webServer.arg("mqtt_server"));
                    ValidationResult portRes = validateMQTTPort(webServer.arg("mqtt_port").toInt());
                    if (!hostRes.isValid || !portRes.isValid)
                    {
                        String msg = !hostRes.isValid ? hostRes.message : portRes.message;
                        String html = generateErrorPage(HTTP_BAD_REQUEST, msg);
                        webServer.send(HTTP_BAD_REQUEST, HTTP_CONTENT_TYPE_HTML, html);
                        return;
                    }
                }
                if (webServer.hasArg("ts_enabled"))
                {
                    ValidationResult tsRes = validateThingSpeakAPIKey(webServer.arg("ts_api_key"));
                    if (!tsRes.isValid)
                    {
                        String html = generateErrorPage(HTTP_BAD_REQUEST, tsRes.message);
                        webServer.send(HTTP_BAD_REQUEST, HTTP_CONTENT_TYPE_HTML, html);
                        return;
                    }
                }
            }

            // Сохранение настроек в конфигурацию
            strlcpy(config.ssid, webServer.arg("ssid").c_str(), sizeof(config.ssid));
            strlcpy(config.password, webServer.arg("password").c_str(), sizeof(config.password));

            // Сохраняем остальные настройки только в режиме STA
            if (currentWiFiMode == WiFiMode::STA)
            {
                // ✅ Явное приведение bool для битовых полей
                config.flags.mqttEnabled = (uint8_t)webServer.hasArg("mqtt_enabled");
                strlcpy(config.mqttServer, webServer.arg("mqtt_server").c_str(), sizeof(config.mqttServer));
                config.mqttPort = webServer.arg("mqtt_port").toInt();
                strlcpy(config.mqttUser, webServer.arg("mqtt_user").c_str(), sizeof(config.mqttUser));
                strlcpy(config.mqttPassword, webServer.arg("mqtt_password").c_str(), sizeof(config.mqttPassword));
                config.flags.hassEnabled = (uint8_t)webServer.hasArg("hass_enabled");
                config.flags.thingSpeakEnabled = (uint8_t)webServer.hasArg("ts_enabled");
                strlcpy(config.thingSpeakApiKey, webServer.arg("ts_api_key").c_str(), sizeof(config.thingSpeakApiKey));
                config.mqttQos = webServer.arg("mqtt_qos").toInt();
                strlcpy(config.thingSpeakChannelId, webServer.arg("ts_channel_id").c_str(),
                        sizeof(config.thingSpeakChannelId));
                config.flags.useRealSensor = (uint8_t)webServer.hasArg("real_sensor");
                config.flags.calibrationEnabled = (uint8_t)webServer.hasArg("cal_enabled");
                // Тип среды выращивания v2.6.1
                if (webServer.hasArg("env_type")) {
                    config.environmentType = webServer.arg("env_type").toInt();
                } else {
                    config.environmentType = 0;
                }
                // Сезонная поправка
                config.flags.seasonalAdjustEnabled = (uint8_t)webServer.hasArg("season_adj");

                // Обратная совместимость
                config.flags.isGreenhouse = (config.environmentType == 1);

                if (webServer.hasArg("latitude")) {
                    config.latitude = webServer.arg("latitude").toFloat();
                }
                if (webServer.hasArg("longitude")) {
                    config.longitude = webServer.arg("longitude").toFloat();
                }

                if (webServer.hasArg("crop")) {
                    strlcpy(config.cropId, webServer.arg("crop").c_str(), sizeof(config.cropId));
                }

                if (webServer.hasArg("soil_profile_sel"))
                {
                    config.soilProfile = webServer.arg("soil_profile_sel").toInt();
                }
                strlcpy(config.ntpServer, webServer.arg("ntp_server").c_str(), sizeof(config.ntpServer));
                config.ntpUpdateInterval = webServer.arg("ntp_interval").toInt();
                // Сохраняем пароль веб-интерфейса
                strlcpy(config.webPassword, webServer.arg("web_password").c_str(), sizeof(config.webPassword));

                if (webServer.hasArg("irrig_th")) {
                    config.irrigationSpikeThreshold = webServer.arg("irrig_th").toFloat();
                }
                if (webServer.hasArg("irrig_hold")) {
                    config.irrigationHoldMinutes = webServer.arg("irrig_hold").toInt();
                }

                logInfoSafe("\1",
                        config.flags.mqttEnabled ? "ON" : "OFF", config.flags.thingSpeakEnabled ? "ON" : "OFF",
                        config.flags.hassEnabled ? "ON" : "OFF");
            }

            // Сохранение в NVS
            saveConfig();

            // Отправка страницы успеха
            String html = generateSuccessPage("Настройки сохранены",
                                              "Настройки сохранены успешно. Устройство перезагружается...", "/", 1);

            webServer.send(HTTP_OK, HTTP_CONTENT_TYPE_HTML, html);

            logSuccess("Настройки сохранены успешно");
            delay(1000);
            ESP.restart();
        });

    // Статус (уже реализован в wifi_manager.cpp)
    webServer.on("/status", HTTP_GET, handleStatus);

    logDebug("Основные маршруты настроены: /, /save, /status");
}

void handleRoot()
{
    String html =
        "<!DOCTYPE html><html><head><meta charset='UTF-8'><meta name='viewport' content='width=device-width, "
        "initial-scale=1.0'>";
    html += "<title>" UI_ICON_CONFIG " Настройки JXCT</title>";
    html += "<style>" + String(getUnifiedCSS()) + "</style></head><body><div class='container'>";
    html += navHtml();
    html += "<h1>" UI_ICON_CONFIG " Настройки JXCT</h1>";
    html += "<form action='/save' method='post'>";
    html += getCSRFHiddenField();  // Добавляем CSRF токен
    html += "<div class='section'><h2>WiFi настройки</h2>";
    html += "<div class='form-group'><label for='ssid'>SSID:</label><input type='text' id='ssid' name='ssid' value='" +
            String(config.ssid) + "' required></div>";
    html +=
        "<div class='form-group'><label for='password'>Пароль:</label><input type='password' id='password' "
        "name='password' value='" +
        String(config.password) + "' required></div></div>";

    // Показываем остальные настройки только в режиме STA
    if (currentWiFiMode == WiFiMode::STA)
    {
        String mqttChecked = config.flags.mqttEnabled ? " checked" : "";
        html += "<div class='section'><h2>MQTT настройки</h2>";
        html +=
            "<div class='form-group'><label for='mqtt_enabled'>Включить MQTT:</label><input type='checkbox' "
            "id='mqtt_enabled' name='mqtt_enabled'" +
            mqttChecked + "></div>";
        html +=
            "<div class='form-group'><label for='mqtt_server'>MQTT сервер:</label><input type='text' id='mqtt_server' "
            "name='mqtt_server' value='" +
            String(config.mqttServer) + "'" + (config.flags.mqttEnabled ? " required" : "") + "></div>";
        html +=
            "<div class='form-group'><label for='mqtt_port'>MQTT порт:</label><input type='text' id='mqtt_port' "
            "name='mqtt_port' value='" +
            String(config.mqttPort) + "'></div>";
        html +=
            "<div class='form-group'><label for='mqtt_user'>MQTT пользователь:</label><input type='text' "
            "id='mqtt_user' name='mqtt_user' value='" +
            String(config.mqttUser) + "'></div>";
        html +=
            "<div class='form-group'><label for='mqtt_password'>MQTT пароль:</label><input type='password' "
            "id='mqtt_password' name='mqtt_password' value='" +
            String(config.mqttPassword) + "'></div>";
        String hassChecked = config.flags.hassEnabled ? " checked" : "";
        html +=
            "<div class='form-group'><label for='hass_enabled'>Интеграция с Home Assistant:</label><input "
            "type='checkbox' id='hass_enabled' name='hass_enabled'" +
            hassChecked + "></div></div>";
        String tsChecked = config.flags.thingSpeakEnabled ? " checked" : "";
        html += "<div class='section'><h2>ThingSpeak настройки</h2>";
        html +=
            "<div class='form-group'><label for='ts_enabled'>Включить ThingSpeak:</label><input type='checkbox' "
            "id='ts_enabled' name='ts_enabled'" +
            tsChecked + "></div>";
        html +=
            "<div class='form-group'><label for='ts_api_key'>API ключ:</label><input type='text' id='ts_api_key' "
            "name='ts_api_key' value='" +
            String(config.thingSpeakApiKey) + "'" + (config.flags.thingSpeakEnabled ? " required" : "") + "></div>";
        html +=
            "<div class='form-group'><label for='ts_channel_id'>Channel ID:</label><input type='text' "
            "id='ts_channel_id' name='ts_channel_id' value='" +
            String(config.thingSpeakChannelId) + "'></div>";
        html +=
            "<div style='color:#888;font-size:13px'>💡 Интервал публикации настраивается в разделе <a "
            "href='/intervals' style='color:#4CAF50'>Интервалы</a></div></div>";
        String realSensorChecked = config.flags.useRealSensor ? " checked" : "";
        html += "<div class='section'><h2>Датчик</h2>";
        html +=
            "<div class='form-group'><label for='real_sensor'>Реальный датчик:</label><input type='checkbox' "
            "id='real_sensor' name='real_sensor'" +
            realSensorChecked + "></div>";

        // ----------------- ⚙️ Компенсация датчиков -----------------
        html += "<div class='section'><h2>⚙️ Компенсация датчиков</h2>";
        String calibChecked = config.flags.calibrationEnabled ? " checked" : "";
        html +=
            "<div class='form-group'><label for='cal_enabled'>Включить компенсацию:</label><input type='checkbox' "
            "id='cal_enabled' name='cal_enabled'" +
            calibChecked + "></div>";
        html +=
            "<div class='form-group'><label for='irrig_th'>Порог ∆влажности (%):</label><input type='number' "
            "step='0.1' id='irrig_th' name='irrig_th' value='" +
            String(config.irrigationSpikeThreshold, 1) + "'></div>";
        html +=
            "<div class='form-group'><label for='irrig_hold'>Удержание (мин):</label><input type='number' "
            "id='irrig_hold' name='irrig_hold' value='" +
            String(config.irrigationHoldMinutes) + "'></div>";
        html += "</div>";  // конец секции компенсации

        // ----------------- 🌱 Агрорекомендации -----------------
        html += "<div class='section'><h2>🌱 Агрорекомендации</h2>";
        // Координаты
        html +=
            "<div class='form-group'><label for='latitude'>Широта:</label><input type='number' step='0.0001' "
            "id='latitude' name='latitude' value='" +
            String(config.latitude, 4) + "'></div>";
        html +=
            "<div class='form-group'><label for='longitude'>Долгота:</label><input type='number' step='0.0001' "
            "id='longitude' name='longitude' value='" +
            String(config.longitude, 4) + "'></div>";
        // Культура
        html += "<div class='form-group'><label for='crop'>Культура:</label><select id='crop' name='crop'>";
        html +=
            String("<option value='none'") + (strcmp(config.cropId, "none") == 0 ? " selected" : "") + ">нет</option>";
        html += String("<option value='tomato'") + (strcmp(config.cropId, "tomato") == 0 ? " selected" : "") +
                ">Томат</option>";
        html += String("<option value='cucumber'") + (strcmp(config.cropId, "cucumber") == 0 ? " selected" : "") +
                ">Огурец</option>";
        html += String("<option value='pepper'") + (strcmp(config.cropId, "pepper") == 0 ? " selected" : "") +
                ">Перец</option>";
        html += String("<option value='lettuce'") + (strcmp(config.cropId, "lettuce") == 0 ? " selected" : "") +
                ">Салат</option>";
        html += String("<option value='strawberry'") + (strcmp(config.cropId, "strawberry") == 0 ? " selected" : "") +
                ">Клубника</option>";
        html += String("<option value='apple'") + (strcmp(config.cropId, "apple") == 0 ? " selected" : "") +
                ">Яблоня</option>";
        html += String("<option value='pear'") + (strcmp(config.cropId, "pear") == 0 ? " selected" : "") +
                ">Груша</option>";
        html += String("<option value='cherry'") + (strcmp(config.cropId, "cherry") == 0 ? " selected" : "") +
                ">Вишня/Черешня</option>";
        html += String("<option value='raspberry'") + (strcmp(config.cropId, "raspberry") == 0 ? " selected" : "") +
                ">Малина</option>";
        html += String("<option value='currant'") + (strcmp(config.cropId, "currant") == 0 ? " selected" : "") +
                ">Смородина</option>";
        html += String("<option value='blueberry'") + (strcmp(config.cropId, "blueberry") == 0 ? " selected" : "") +
                ">Голубика</option>";
        html += String("<option value='lawn'") + (strcmp(config.cropId, "lawn") == 0 ? " selected" : "") +
                ">Газон</option>";
        html += String("<option value='grape'") + (strcmp(config.cropId, "grape") == 0 ? " selected" : "") +
                ">Виноград</option>";
        html += String("<option value='conifer'") + (strcmp(config.cropId, "conifer") == 0 ? " selected" : "") +
                ">Хвойные деревья</option>";
        html += "</select></div>";
        // Тип среды выращивания v2.6.1
        String selectedEnvOutdoor = config.environmentType == 0 ? " selected" : "";
        String selectedEnvGreenhouse = config.environmentType == 1 ? " selected" : "";
        String selectedEnvIndoor = config.environmentType == 2 ? " selected" : "";
        html += "<div class='form-group'><label for='env_type'>Среда:</label><select id='env_type' name='env_type'>";
        html += String("<option value='0'") + selectedEnvOutdoor + ">Открытый грунт</option>";
        html += String("<option value='1'") + selectedEnvGreenhouse + ">Теплица</option>";
        html += String("<option value='2'") + selectedEnvIndoor + ">Комнатная</option></select></div>";

        // Сезонные коэффициенты
        String seasonalChecked = config.flags.seasonalAdjustEnabled ? " checked" : "";
        html +=
            "<div class='form-group'><label for='season_adj'>Учитывать сезонность:</label><input type='checkbox' "
            "id='season_adj' name='season_adj'" +
            seasonalChecked + "></div>";

        // Профиль почвы
        const char* selectedSand = config.soilProfile == 0 ? " selected" : "";
        const char* selectedLoam = config.soilProfile == 1 ? " selected" : "";
        const char* selectedPeat = config.soilProfile == 2 ? " selected" : "";
        const char* selectedClay = config.soilProfile == 3 ? " selected" : "";
        const char* selectedSandPeat = config.soilProfile == 4 ? " selected" : "";
        html +=
            "<div class='form-group'><label for='soil_profile_sel'>Профиль почвы:</label><select id='soil_profile_sel' "
            "name='soil_profile_sel'>";
        html += String("<option value='0'") + selectedSand + ">Песок</option>";
        html += String("<option value='1'") + selectedLoam + ">Суглинок</option>";
        html += String("<option value='2'") + selectedPeat + ">Торф</option>";
        html += String("<option value='3'") + selectedClay + ">Глина</option>";
        html += String("<option value='4'") + selectedSandPeat + ">Песчано-торфяной</option>";
        html += "</select></div>";

        html += "</div>";  // конец секции агрорекомендаций

        html += "</div>";  // конец секции датчика
        html += "<div class='section'><h2>NTP</h2>";
        html +=
            "<div class='form-group'><label for='ntp_server'>NTP сервер:</label><input type='text' id='ntp_server' "
            "name='ntp_server' value='" +
            String(config.ntpServer) + "' required></div>";
        html +=
            "<div class='form-group'><label for='ntp_interval'>Интервал обновления NTP (мс):</label><input "
            "type='number' id='ntp_interval' name='ntp_interval' min='10000' max='86400000' value='" +
            String(config.ntpUpdateInterval) + "'></div></div>";
    }
    html += generateButton(ButtonType::PRIMARY, ButtonConfig{UI_ICON_SAVE, "Сохранить настройки", ""}) + "</form>";

    // Добавляем JavaScript для динамического изменения обязательных полей
    if (currentWiFiMode == WiFiMode::STA)
    {
        html += "<script>";
        html += "document.getElementById('mqtt_enabled').addEventListener('change', function() {";
        html += "  document.getElementById('mqtt_server').required = this.checked;";
        html += "});";
        html += "document.getElementById('ts_enabled').addEventListener('change', function() {";
        html += "  document.getElementById('ts_api_key').required = this.checked;";
        html += "});";
        html += "</script>";
    }

    html += "</div>" + String(getToastHTML()) + "</body></html>";
    webServer.send(200, "text/html; charset=utf-8", html);
}
