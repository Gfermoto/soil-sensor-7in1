/**
 * @file routes_data.cpp
 * @brief Маршруты для работы с данными датчика
 * @details Обработка запросов показаний датчика, JSON API и веб-интерфейса данных
 */

#include "../../include/web_routes.h"
#include "../../include/logger.h"
#include "../../include/jxct_config_vars.h"
#include "../../include/jxct_ui_system.h"
#include "../../include/jxct_format_utils.h"
#include "../wifi_manager.h"
#include "../modbus_sensor.h"
#include <ArduinoJson.h>
#include <NTPClient.h>

extern NTPClient* timeClient;

// Объявления внешних функций
extern String navHtml();
extern String formatValue(float value, const char* unit, int precision);
extern String getApSsid();

void setupDataRoutes() {
    // Красивая страница показаний с иконками (оригинальный дизайн)
    webServer.on("/readings", HTTP_GET, []() {
        logWebRequest("GET", "/readings", webServer.client().remoteIP().toString());
        
        if (currentWiFiMode == WiFiMode::AP) {
            String html = "<!DOCTYPE html><html><head><meta charset='UTF-8'>";
            html += "<title>" UI_ICON_DATA " Показания</title>";
            html += "<style>" + String(getUnifiedCSS()) + "</style></head><body><div class='container'>";
            html += "<h1>" UI_ICON_DATA " Показания</h1>";
            html += "<div class='msg msg-error'>" UI_ICON_ERROR " Недоступно в режиме точки доступа</div></div></body></html>";
            webServer.send(200, "text/html; charset=utf-8", html);
            return;
        }
        
        String html = "<!DOCTYPE html><html><head><meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1.0'>";
        html += "<title>" UI_ICON_DATA " Показания датчика JXCT</title>";
        html += "<style>" + String(getUnifiedCSS()) + "</style></head><body><div class='container'>";
        html += navHtml();
        html += "<h1>" UI_ICON_DATA " Показания датчика</h1>";
        html += "<div class='section'><ul>";
        html += "<li>🌡️ Температура: <span id='temp'></span> °C</li>";
        html += "<li>💧 Влажность: <span id='hum'></span> %</li>";
        html += "<li>⚡ EC: <span id='ec'></span> µS/cm</li>";
        html += "<li>⚗️ pH: <span id='ph'></span></li>";
        html += "<li>🔴 Азот (N): <span id='n'></span> мг/кг</li>";
        html += "<li>🟡 Фосфор (P): <span id='p'></span> мг/кг</li>";
        html += "<li>🔵 Калий (K): <span id='k'></span> мг/кг</li>";
        html += "</ul></div>";
        html += "<div style='margin-top:15px;font-size:14px;color:#555'><b>API:</b> <a href='/api/sensor' target='_blank'>/api/sensor</a> (JSON, +timestamp)</div>";
        html += "<script>";
        html += "function updateSensor(){";
        html += "fetch('/sensor_json').then(r=>r.json()).then(d=>{";
        html += "document.getElementById('temp').textContent=d.temperature;";
        html += "document.getElementById('hum').textContent=d.humidity;";
        html += "document.getElementById('ec').textContent=d.ec;";
        html += "document.getElementById('ph').textContent=d.ph;";
        html += "document.getElementById('n').textContent=d.nitrogen;";
        html += "document.getElementById('p').textContent=d.phosphorus;";
        html += "document.getElementById('k').textContent=d.potassium;";
        html += "});";
        html += "}";
        html += "setInterval(updateSensor,3000);";
        html += "updateSensor();";
        html += "</script>";
        html += "</div>" + String(getToastHTML()) + "</body></html>";
        webServer.send(200, "text/html; charset=utf-8", html);
    });

    // AJAX эндпоинт для обновления показаний
    webServer.on("/sensor_json", HTTP_GET, []() {
        logWebRequest("GET", "/sensor_json", webServer.client().remoteIP().toString());
        
        if (currentWiFiMode != WiFiMode::STA) {
            webServer.send(403, "application/json", "{\"error\":\"AP mode\"}");
            return;
        }
        
        StaticJsonDocument<256> doc;
        doc["temperature"] = format_temperature(sensorData.temperature);
        doc["humidity"] = format_moisture(sensorData.humidity);
        doc["ec"] = format_ec(sensorData.ec);
        doc["ph"] = format_ph(sensorData.ph);
        doc["nitrogen"] = format_npk(sensorData.nitrogen);
        doc["phosphorus"] = format_npk(sensorData.phosphorus);
        doc["potassium"] = format_npk(sensorData.potassium);
        doc["timestamp"] = (long)(timeClient ? timeClient->getEpochTime() : 0);
        
        String json;
        serializeJson(doc, json);
        webServer.send(200, "application/json", json);
    });

    // API эндпоинт для интеграции
    webServer.on("/api/sensor", HTTP_GET, []() {
        logWebRequest("GET", "/api/sensor", webServer.client().remoteIP().toString());
        
        if (currentWiFiMode != WiFiMode::STA) {
            webServer.send(403, "application/json", "{\"error\":\"AP mode\"}");
            return;
        }
        
        StaticJsonDocument<256> doc;
        doc["temperature"] = format_temperature(sensorData.temperature);
        doc["humidity"] = format_moisture(sensorData.humidity);
        doc["ec"] = format_ec(sensorData.ec);
        doc["ph"] = format_ph(sensorData.ph);
        doc["nitrogen"] = format_npk(sensorData.nitrogen);
        doc["phosphorus"] = format_npk(sensorData.phosphorus);
        doc["potassium"] = format_npk(sensorData.potassium);
        doc["timestamp"] = (long)(timeClient ? timeClient->getEpochTime() : 0);
        
        String json;
        serializeJson(doc, json);
        webServer.send(200, "application/json", json);
    });
    
    logDebug("Маршруты данных настроены: /readings, /sensor_json, /api/sensor");
}

// Вспомогательная функция для получения SSID точки доступа
extern String getApSsid(); 