#include "../../include/jxct_config_vars.h"
#include "../../include/jxct_ui_system.h"
#include "../../include/logger.h"
#include "../../include/web_routes.h"
#include "../../include/validation_utils.h"  // ✅ Валидация
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
            if (!checkCSRFSafety()) {
                logWarn("CSRF атака отклонена на /save от %s", 
                        webServer.client().remoteIP().toString().c_str());
                String html = generateErrorPage(403, "Forbidden: Недействительный CSRF токен");
                webServer.send(403, "text/html; charset=utf-8", html);
                return;
            }

            // ====== ВАЛИДАЦИЯ ЧЕРЕЗ validation_utils ======
            ValidationResult ssidRes = validateSSID(webServer.arg("ssid"));
            ValidationResult passRes = validatePassword(webServer.arg("password"));

            if (!ssidRes.isValid || !passRes.isValid)
            {
                String msg = !ssidRes.isValid ? ssidRes.message : passRes.message;
                String html = generateErrorPage(400, msg);
                webServer.send(400, "text/html; charset=utf-8", html);
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
                        String html = generateErrorPage(400, msg);
                        webServer.send(400, "text/html; charset=utf-8", html);
                        return;
                    }
                }
                if (webServer.hasArg("ts_enabled"))
                {
                    ValidationResult tsRes = validateThingSpeakAPIKey(webServer.arg("ts_api_key"));
                    if (!tsRes.isValid)
                    {
                        String html = generateErrorPage(400, tsRes.message);
                        webServer.send(400, "text/html; charset=utf-8", html);
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
                if (webServer.hasArg("env_type"))
                    config.environmentType = webServer.arg("env_type").toInt();
                else
                    config.environmentType = 0;
                // Сезонная поправка
                config.flags.seasonalAdjustEnabled = (uint8_t)webServer.hasArg("season_adj");

                // Обратная совместимость
                config.flags.isGreenhouse = (config.environmentType == 1);

                if (webServer.hasArg("latitude")) config.latitude = webServer.arg("latitude").toFloat();
                if (webServer.hasArg("longitude")) config.longitude = webServer.arg("longitude").toFloat();

                if (webServer.hasArg("crop"))
                    strlcpy(config.cropId, webServer.arg("crop").c_str(), sizeof(config.cropId));

                if (webServer.hasArg("soil_profile_sel"))
                {
                    config.soilProfile = webServer.arg("soil_profile_sel").toInt();
                }
                strlcpy(config.ntpServer, webServer.arg("ntp_server").c_str(), sizeof(config.ntpServer));
                config.ntpUpdateInterval = webServer.arg("ntp_interval").toInt();
                // Сохраняем пароль веб-интерфейса
                strlcpy(config.webPassword, webServer.arg("web_password").c_str(), sizeof(config.webPassword));

                if (webServer.hasArg("irrig_th")) config.irrigationSpikeThreshold = webServer.arg("irrig_th").toFloat();
                if (webServer.hasArg("irrig_hold")) config.irrigationHoldMinutes = webServer.arg("irrig_hold").toInt();

                logInfo("Конфигурация обновлена: MQTT=%s, ThingSpeak=%s, HASS=%s",
                        config.flags.mqttEnabled ? "ON" : "OFF", config.flags.thingSpeakEnabled ? "ON" : "OFF",
                        config.flags.hassEnabled ? "ON" : "OFF");
            }

            // Сохранение в NVS
            saveConfig();

            // Отправка страницы успеха
            String html = generateSuccessPage("Настройки сохранены",
                                              "Настройки сохранены успешно. Устройство перезагружается...", "/", 1);

            webServer.send(200, "text/html; charset=utf-8", html);

            logSuccess("Настройки сохранены успешно");
            delay(1000);
            ESP.restart();
        });

    // Статус (уже реализован в wifi_manager.cpp)
    webServer.on("/status", HTTP_GET, handleStatus);

    logDebug("Основные маршруты настроены: /, /save, /status");
}
