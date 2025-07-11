/**
 * @file routes_service.cpp
 * @brief Сервисные маршруты для диагностики и управления
 * @details Обработка запросов здоровья системы, статуса сервисов, перезагрузки и OTA обновлений
 */

#include <ArduinoJson.h>
#include "../../include/jxct_config_vars.h"
#include "../../include/jxct_constants.h"
#include "../../include/jxct_device_info.h"
#include "../../include/jxct_format_utils.h"
#include "../../include/jxct_strings.h"
#include "../../include/jxct_ui_system.h"
#include "../../include/logger.h"
#include "../../include/web/csrf_protection.h"  // 🔒 CSRF защита
#include "../../include/web_routes.h"           // ✅ CSRF защита
#include "../modbus_sensor.h"
#include "../mqtt_client.h"
#include "../thingspeak_client.h"
#include "../wifi_manager.h"

// Внешние зависимости (уже объявлены в заголовочных файлах)
// extern WebServer webServer;  // объявлено в web_routes.h
// extern WiFiMode currentWiFiMode;  // объявлено в wifi_manager.h
// extern String navHtml();  // объявлено в wifi_manager.h
// extern String getApSsid();  // объявлено в wifi_manager.h
// extern String sensorLastError;  // объявлено в modbus_sensor.h
// extern const char* getThingSpeakLastPublish();  // объявлено в thingspeak_client.h
// extern const char* getThingSpeakLastError();  // объявлено в thingspeak_client.h

// --- API v1 helpers ---
// NOLINTNEXTLINE(misc-use-anonymous-namespace)
static void sendHealthJson();
// NOLINTNEXTLINE(misc-use-anonymous-namespace)
static void sendServiceStatusJson();

// Локальные функции
// NOLINTNEXTLINE(misc-use-anonymous-namespace)
static String formatUptime(unsigned long milliseconds);

void setupServiceRoutes()
{
    logDebug("Настройка сервисных маршрутов");

    // Health endpoint (legacy) and API v1
    webServer.on("/health", HTTP_GET, sendHealthJson);
    webServer.on(API_SYSTEM_HEALTH, HTTP_GET, sendHealthJson);

    // Service status endpoint legacy + API v1
    webServer.on("/service_status", HTTP_GET, sendServiceStatusJson);
    webServer.on(API_SYSTEM_STATUS, HTTP_GET, sendServiceStatusJson);

    // Красивая страница сервисов (оригинальный дизайн)
    webServer.on(
        "/service", HTTP_GET,
        []()
        {
            logWebRequest("GET", "/service", webServer.client().remoteIP().toString());

            if (currentWiFiMode == WiFiMode::AP)
            {
                webServer.send(HTTP_OK, HTTP_CONTENT_TYPE_HTML,
                               generateApModeUnavailablePage("Сервис", UI_ICON_SERVICE));
                return;
            }

            String html = generatePageHeader("Сервис", UI_ICON_SERVICE);
            html += navHtml();
            html += "<h1>" UI_ICON_SERVICE " Сервис</h1>";
            html += "<div class='info-block' id='status-block'>Загрузка статусов...</div>";
            html += "<div class='info-block'><b>Производитель:</b> " + String(DEVICE_MANUFACTURER) +
                    "<br><b>Модель:</b> " + String(DEVICE_MODEL) + "<br><b>Версия:</b> " + String(FIRMWARE_VERSION) +
                    "</div>";
            html += "<div class='section' style='margin-top:20px;'>";
            html += "<form method='post' action='/reset' style='margin-bottom:10px'>";
            html += getCSRFHiddenField();
            html +=
                generateButton(ButtonType::DANGER, ButtonConfig{UI_ICON_RESET, "Сбросить настройки", ""}) + "</form>";
            html += "<form method='post' action='/reboot' style='margin-bottom:10px'>";
            html += getCSRFHiddenField();
            html += generateButton(ButtonType::SECONDARY, ButtonConfig{"🔄", "Перезагрузить", ""}) + "</form>";
            html += "</div>";
            html +=
                "<div class='section' style='margin-top:15px;font-size:14px;color:#555'><b>API:</b> <a "
                "href='/service_status' target='_blank'>/service_status</a> (JSON, статусы сервисов) | <a "
                "href='/health' target='_blank'>/health</a> (JSON, подробная диагностика)</div>";
            html += "<script>";
            html += "function dot(status){";
            html += "if(status===true)return'<span class=\"status-dot dot-ok\"></span>';";
            html += "if(status===false)return'<span class=\"status-dot dot-err\"></span>';";
            html += "if(status==='warn')return'<span class=\"status-dot dot-warn\"></span>';";
            html += "return'<span class=\"status-dot dot-off\"></span>';";
            html += "}";
            html += "function updateStatus(){fetch('/service_status').then(r=>r.json()).then(d=>{let html='';";
            html +=
                "html+=dot(d.wifi_connected)+'<b>WiFi:</b> '+(d.wifi_connected?'Подключено ('+d.wifi_ip+', "
                "'+d.wifi_ssid+', RSSI '+d.wifi_rssi+' dBm)':'Не подключено')+'<br>';";
            html +=
                "html+=dot(d.mqtt_enabled?d.mqtt_connected:false)+'<b>MQTT:</b> "
                "'+(d.mqtt_enabled?(d.mqtt_connected?'Подключено':'Ошибка'+(d.mqtt_last_error?' "
                "('+d.mqtt_last_error+')':'')):'Отключено')+'<br>';";
            html +=
                "html+=dot(d.thingspeak_enabled?(d.thingspeak_last_error?false:true):false)+'<b>ThingSpeak:</b> "
                "'+(d.thingspeak_enabled?(d.thingspeak_last_error?'Ошибка: "
                "'+d.thingspeak_last_error:((d.thingspeak_last_pub && d.thingspeak_last_pub!=='0')?'Последняя "
                "публикация: '+d.thingspeak_last_pub:'Нет публикаций')):'Отключено')+'<br>';";
            html +=
                "if(d.thingspeak_enabled && d.thingspeak_last_error){showToast('Ошибка ThingSpeak: "
                "'+d.thingspeak_last_error,'error');}";
            html +=
                "html+=dot(d.hass_enabled)+'<b>Home Assistant:</b> '+(d.hass_enabled?'Включено':'Отключено')+'<br>';";
            html +=
                "html+=dot(d.sensor_ok)+'<b>Датчик:</b> '+(d.sensor_ok?'Ок':'Ошибка'+(d.sensor_last_error?' "
                "('+d.sensor_last_error+')':''));";
            html += "document.getElementById('status-block').innerHTML=html;";
            html += "});}setInterval(updateStatus," + String(config.webUpdateInterval) + ");updateStatus();";
            html += "</script>";
            html += generatePageFooter();
            webServer.send(200, "text/html; charset=utf-8", html);
        });

    // POST обработчики для сервисных функций
    webServer.on("/reset", HTTP_POST,
                 []()
                 {
                     logWebRequest("POST", webServer.uri(), webServer.client().remoteIP().toString());

                     // ✅ CSRF защита - критическая операция сброса!
                     if (!checkCSRFSafety())
                     {
                         logWarnSafe("\1", webServer.client().remoteIP().toString().c_str());
                         const String html = generateErrorPage(HTTP_FORBIDDEN, "Forbidden: Недействительный CSRF токен");
                         webServer.send(HTTP_FORBIDDEN, HTTP_CONTENT_TYPE_HTML, html);
                         return;
                     }

                     if (currentWiFiMode != WiFiMode::STA)
                     {
                         webServer.send(HTTP_FORBIDDEN, HTTP_CONTENT_TYPE_PLAIN, "Недоступно в режиме точки доступа");
                         return;
                     }

                     resetConfig();
                     const String html =
                         "<!DOCTYPE html><html><head><meta charset='UTF-8'><meta http-equiv='refresh' "
                         "content='2;url=/service'><title>Сброс</title></head><body "
                         "style='font-family:Arial,sans-serif;text-align:center;padding-top:40px'><h2>Настройки "
                         "сброшены</h2><p>Перезагрузка...<br>Сейчас вы будете перенаправлены на страницу "
                         "сервисов.</p></body></html>";
                     webServer.send(HTTP_OK, HTTP_CONTENT_TYPE_HTML, html);
                     delay(WEB_OPERATION_DELAY_MS);
                     ESP.restart();
                 });

    webServer.on(API_SYSTEM_RESET, HTTP_POST,
                 []()
                 {
                     webServer.sendHeader("Location", "/reset", true);
                     webServer.send(HTTP_REDIRECT_PERMANENT, HTTP_CONTENT_TYPE_PLAIN, "Redirect");
                 });

    webServer.on("/reboot", HTTP_POST,
                 []()
                 {
                     logWebRequest("POST", webServer.uri(), webServer.client().remoteIP().toString());

                     // ✅ CSRF защита - критическая операция перезагрузки!
                     if (!checkCSRFSafety())
                     {
                         logWarnSafe("\1", webServer.client().remoteIP().toString().c_str());
                         const String html = generateErrorPage(HTTP_FORBIDDEN, "Forbidden: Недействительный CSRF токен");
                         webServer.send(HTTP_FORBIDDEN, HTTP_CONTENT_TYPE_HTML, html);
                         return;
                     }

                     if (currentWiFiMode != WiFiMode::STA)
                     {
                         webServer.send(HTTP_FORBIDDEN, HTTP_CONTENT_TYPE_PLAIN, "Недоступно в режиме точки доступа");
                         return;
                     }

                     const String html =
                         "<!DOCTYPE html><html><head><meta charset='UTF-8'><meta http-equiv='refresh' "
                         "content='2;url=/service'><title>Перезагрузка</title></head><body "
                         "style='font-family:Arial,sans-serif;text-align:center;padding-top:40px'><h2>Перезагрузка...</"
                         "h2><p>Сейчас вы будете перенаправлены на страницу сервисов.</p></body></html>";
                     webServer.send(HTTP_OK, HTTP_CONTENT_TYPE_HTML, html);
                     delay(WEB_OPERATION_DELAY_MS);
                     ESP.restart();
                 });

    webServer.on(API_SYSTEM_REBOOT, HTTP_POST,
                 []()
                 {
                     webServer.sendHeader("Location", "/reboot", true);
                     webServer.send(HTTP_REDIRECT_PERMANENT, HTTP_CONTENT_TYPE_PLAIN, "Redirect");
                 });

    // Старый маршрут /ota более не нужен – сделаем редирект на новую страницу
    webServer.on("/ota", HTTP_ANY,
                 []()
                 {
                     webServer.sendHeader("Location", "/updates", true);
                     webServer.send(HTTP_REDIRECT_TEMPORARY, HTTP_CONTENT_TYPE_PLAIN, "Redirect");
                 });

    logSuccess("Сервисные маршруты настроены");
}

// Вспомогательная функция для форматирования времени работы
// NOLINTNEXTLINE(misc-use-anonymous-namespace)
static String formatUptime(unsigned long milliseconds)
{
    unsigned long seconds = milliseconds / MILLISECONDS_IN_SECOND;
    unsigned long minutes = seconds / SECONDS_IN_MINUTE;
    unsigned long hours = minutes / MINUTES_IN_HOUR;
    const unsigned long days = hours / HOURS_IN_DAY;

    seconds %= SECONDS_IN_MINUTE;
    minutes %= MINUTES_IN_HOUR;
    hours %= HOURS_IN_DAY;

    String uptime = "";
    if (days > 0)
    {
        uptime += String(days) + "д ";
    }
    if (hours > 0)
    {
        uptime += String(hours) + "ч ";
    }
    if (minutes > 0)
    {
        uptime += String(minutes) + "м ";
    }
    uptime += String(seconds) + "с";

    return uptime;
}

// NOLINTNEXTLINE(misc-use-anonymous-namespace)
static void sendHealthJson()
{
    logWebRequest("GET", webServer.uri(), webServer.client().remoteIP().toString());
    StaticJsonDocument<JSON_DOC_MEDIUM> doc;

    // System info
    doc["device"]["manufacturer"] = DEVICE_MANUFACTURER;
    doc["device"]["model"] = DEVICE_MODEL;
    doc["device"]["version"] = FIRMWARE_VERSION;
    doc["device"]["uptime"] = millis() / MILLISECONDS_IN_SECOND;
    doc["device"]["free_heap"] = ESP.getFreeHeap();
    doc["device"]["chip_model"] = ESP.getChipModel();
    doc["device"]["chip_revision"] = ESP.getChipRevision();
    doc["device"]["cpu_freq"] = ESP.getCpuFreqMHz();

    // Memory info
    doc["memory"]["free_heap"] = ESP.getFreeHeap();
    doc["memory"]["largest_free_block"] = ESP.getMaxAllocHeap();
    doc["memory"]["heap_size"] = ESP.getHeapSize();
    doc["memory"]["psram_size"] = ESP.getPsramSize();
    doc["memory"]["free_psram"] = ESP.getFreePsram();

    // WiFi status
    doc["wifi"]["connected"] = wifiConnected;
    if (wifiConnected)
    {
        doc["wifi"]["ssid"] = WiFi.SSID();
        doc["wifi"]["ip"] = WiFi.localIP().toString();
        doc["wifi"]["rssi"] = WiFi.RSSI();
        doc["wifi"]["mac"] = WiFi.macAddress();
        doc["wifi"]["gateway"] = WiFi.gatewayIP().toString();
        doc["wifi"]["dns"] = WiFi.dnsIP().toString();
    }

    // MQTT status
    doc["mqtt"]["enabled"] = (bool)config.flags.mqttEnabled;
    if (config.flags.mqttEnabled)
    {
        doc["mqtt"]["connected"] = mqttClient.connected();
        doc["mqtt"]["server"] = config.mqttServer;
        doc["mqtt"]["port"] = config.mqttPort;
        doc["mqtt"]["last_error"] = getMqttLastError();
    }

    // ThingSpeak status
    doc["thingspeak"]["enabled"] = (bool)config.flags.thingSpeakEnabled;
    if (config.flags.thingSpeakEnabled)
    {
        doc["thingspeak"]["last_publish"] = getThingSpeakLastPublish();
        doc["thingspeak"]["last_error"] = getThingSpeakLastError();
        doc["thingspeak"]["interval"] = config.thingSpeakInterval;
    }

    // Home Assistant status
    doc["homeassistant"]["enabled"] = (bool)config.flags.hassEnabled;

    // Sensor status
    doc["sensor"]["enabled"] = (bool)config.flags.useRealSensor;
    doc["sensor"]["valid"] = sensorData.valid;
    doc["sensor"]["last_read"] = sensorData.last_update;
    if (getSensorLastError().length() > 0)
    {
        doc["sensor"]["last_error"] = getSensorLastError();
    }

    // Current readings
    doc["readings"]["temperature"] = format_temperature(sensorData.temperature);
    doc["readings"]["humidity"] = format_moisture(sensorData.humidity);
    doc["readings"]["ec"] = format_ec(sensorData.ec);
    doc["readings"]["ph"] = format_ph(sensorData.ph);
    doc["readings"]["nitrogen"] = format_npk(sensorData.nitrogen);
    doc["readings"]["phosphorus"] = format_npk(sensorData.phosphorus);
    doc["readings"]["potassium"] = format_npk(sensorData.potassium);

    // Timestamps
    doc["timestamp"] = millis();
    doc["boot_time"] = millis();

    String json;
    serializeJson(doc, json);
    webServer.send(HTTP_OK, HTTP_CONTENT_TYPE_JSON, json);
}

// NOLINTNEXTLINE(misc-use-anonymous-namespace)
static void sendServiceStatusJson()
{
    logWebRequest("GET", webServer.uri(), webServer.client().remoteIP().toString());
    StaticJsonDocument<JSON_DOC_SMALL> doc;
    doc["wifi_connected"] = wifiConnected;
    doc["wifi_ip"] = WiFi.localIP().toString();
    doc["wifi_ssid"] = WiFi.SSID();
    doc["wifi_rssi"] = WiFi.RSSI();
    doc["mqtt_enabled"] = (bool)config.flags.mqttEnabled;
    doc["mqtt_connected"] = (bool)config.flags.mqttEnabled && mqttClient.connected();
    doc["mqtt_last_error"] = getMqttLastError();
    doc["thingspeak_enabled"] = (bool)config.flags.thingSpeakEnabled;
    doc["thingspeak_last_pub"] = getThingSpeakLastPublish();
    doc["thingspeak_last_error"] = getThingSpeakLastError();
    doc["hass_enabled"] = (bool)config.flags.hassEnabled;
    doc["sensor_ok"] = sensorData.valid;
    doc["sensor_last_error"] = getSensorLastError();

    String json;
    serializeJson(doc, json);
    webServer.send(HTTP_OK, HTTP_CONTENT_TYPE_JSON, json);
}
