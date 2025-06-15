#include "../../include/web_routes.h"
#include "../../include/logger.h"
#include "../../include/jxct_config_vars.h"
#include "../../include/jxct_ui_system.h"
#include "../wifi_manager.h"

void setupMainRoutes() {
    // Главная страница — настройки Wi-Fi (и MQTT/ThingSpeak/HASS в STA)
    webServer.on("/", HTTP_GET, handleRoot);
    webServer.on("/", HTTP_POST, handleRoot);  // Добавляем обработку POST для авторизации

    // Сохранение настроек
    webServer.on("/save", HTTP_POST, []() {
        logWebRequest("POST", "/save", webServer.client().remoteIP().toString());
        
        // Валидация входных данных
        if (!validateConfigInput(true)) {
            String errorMsg = "Заполните все обязательные поля";
            
            // В режиме AP проверяем только SSID
            if (currentWiFiMode == WiFiMode::AP && 
                (!webServer.hasArg("ssid") || webServer.arg("ssid").length() == 0)) {
                errorMsg = "Ошибка: заполните поле SSID";
            }
            
            String html = generateValidationErrorResponse(errorMsg);
            webServer.send(400, "text/html; charset=utf-8", html);
            return;
        }

        // Сохранение настроек в конфигурацию
        strlcpy(config.ssid, webServer.arg("ssid").c_str(), sizeof(config.ssid));
        strlcpy(config.password, webServer.arg("password").c_str(), sizeof(config.password));

        // Сохраняем остальные настройки только в режиме STA
        if (currentWiFiMode == WiFiMode::STA) {
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
            strlcpy(config.ntpServer, webServer.arg("ntp_server").c_str(), sizeof(config.ntpServer));
            config.ntpUpdateInterval = webServer.arg("ntp_interval").toInt();
            // Сохраняем пароль веб-интерфейса
            strlcpy(config.webPassword, webServer.arg("web_password").c_str(), sizeof(config.webPassword));
            
            logInfo("Конфигурация обновлена: MQTT=%s, ThingSpeak=%s, HASS=%s", 
                   config.flags.mqttEnabled ? "ON" : "OFF",
                   config.flags.thingSpeakEnabled ? "ON" : "OFF", 
                   config.flags.hassEnabled ? "ON" : "OFF");
        }

        // Сохранение в NVS
        saveConfig();
        
        // Отправка страницы успеха
        String html = generateSuccessPage(
            "Настройки сохранены", 
            "Настройки сохранены успешно. Устройство перезагружается...",
            "/",
            2
        );
        
        webServer.send(200, "text/html; charset=utf-8", html);
        
        logSuccess("Настройки сохранены успешно");
        delay(2000);
        ESP.restart();
    });

    // Статус (уже реализован в wifi_manager.cpp)
    webServer.on("/status", HTTP_GET, handleStatus);
    
    logDebug("Основные маршруты настроены: /, /save, /status");
}

 