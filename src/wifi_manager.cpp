/**
 * @file wifi_manager.cpp
 * @brief Управление WiFi, веб-интерфейсом и индикацией
 * @details Реализация логики подключения к WiFi, работы в режимах AP/STA, веб-конфигурирования, управления светодиодом
 * и сервисных функций.
 */
#include "wifi_manager.h"
#include "web_routes.h"      // 🏗️ Модульная архитектура v2.4.5
#include "modbus_sensor.h"
#include "mqtt_client.h"
#include "jxct_device_info.h"
#include "jxct_config_vars.h"
#include "jxct_format_utils.h"
#include <NTPClient.h>
#include "thingspeak_client.h"
#include "config.h"
#include "logger.h"
#include "jxct_ui_system.h"  // 🎨 Единая система дизайна v2.3.1

// Константы
#define RESET_BUTTON_PIN 0  // GPIO0 для кнопки сброса

// Глобальные переменные
bool wifiConnected = false;
WiFiMode currentWiFiMode = WiFiMode::AP;
WebServer webServer(80);
DNSServer dnsServer;

// Переменные для светодиода
unsigned long ledLastToggle = 0;
bool ledState = false;
unsigned long ledBlinkInterval = 0;
bool ledFastBlink = false;

extern NTPClient* timeClient;

// Объявление функций
void handleRoot();

void setLedOn()
{
    digitalWrite(STATUS_LED_PIN, HIGH);
    ledBlinkInterval = 0;
    ledFastBlink = false;
}

void setLedOff()
{
    digitalWrite(STATUS_LED_PIN, LOW);
    ledBlinkInterval = 0;
    ledFastBlink = false;
}

void setLedBlink(unsigned long interval)
{
    ledBlinkInterval = interval;
    ledFastBlink = false;
}

void setLedFastBlink()
{
    ledBlinkInterval = 100;
    ledFastBlink = true;
}

void updateLed()
{
    if (ledBlinkInterval > 0)
    {
        unsigned long now = millis();
        if (now - ledLastToggle >= ledBlinkInterval)
        {
            ledLastToggle = now;
            ledState = !ledState;
            digitalWrite(STATUS_LED_PIN, ledState ? HIGH : LOW);
        }
    }
}

// HTML для навигации
String navHtml()
{
    String html = "<div class='nav'>";
    html += "<a href='/'>" UI_ICON_CONFIG " Настройки</a>";
    if (currentWiFiMode == WiFiMode::STA)
    {
        html += "<a href='/readings'>" UI_ICON_DATA " Показания</a>";
        html += "<a href='/intervals'>" UI_ICON_INTERVALS " Интервалы</a>";  // v2.3.0
    
        html += "<a href='/config_manager'>" UI_ICON_FOLDER " Конфигурация</a>";  // v2.3.0
        html += "<a href='/service'>" UI_ICON_SERVICE " Сервис</a>";
    }
    html += "</div>";
    return html;
}

void setupWiFi()
{
    logPrintHeader("ИНИЦИАЛИЗАЦИЯ WiFi", COLOR_GREEN);

    pinMode(STATUS_LED_PIN, OUTPUT);
    setLedBlink(500);
    WiFi.mode(WIFI_AP_STA);
    loadConfig();

    logSystem("SSID: %s", config.ssid);
    logDebug("Password: %s", strlen(config.password) > 0 ? "задан" : "не задан");

    if (strlen(config.ssid) > 0 && strlen(config.password) > 0)
    {
        logWiFi("Переход в режим STA (клиент)");
        startSTAMode();
    }
    else
    {
        logWiFi("Переход в режим AP (точка доступа)");
        startAPMode();
    }
    logPrintSeparator("─", 60);
}

void handleWiFi()
{
    updateLed();
    if (currentWiFiMode == WiFiMode::AP)
    {
        dnsServer.processNextRequest();
        webServer.handleClient();
        if (WiFi.softAPgetStationNum() > 0)
        {
            setLedOn();
        }
        else
        {
            setLedBlink(500);
        }
    }
    else if (currentWiFiMode == WiFiMode::STA)
    {
        if (WiFi.status() != WL_CONNECTED)
        {
            wifiConnected = false;
            setLedBlink(500);
            logWarn("Потеряно соединение с WiFi, переход в AP");
            startAPMode();
        }
        else if (!wifiConnected)
        {
            wifiConnected = true;
            setLedOn();
            logSuccess("Подключено к WiFi, IP: %s", WiFi.localIP().toString().c_str());
        }
        webServer.handleClient();
    }
}

String getApSsid()
{
    uint8_t mac[6];
    WiFi.macAddress(mac);
    char buf[20];
    snprintf(buf, sizeof(buf), "jxct-%02X%02X%02X", mac[3], mac[4], mac[5]);
    for (int i = 0; buf[i]; ++i) buf[i] = tolower(buf[i]);
    return String(buf);
}

void startAPMode()
{
    currentWiFiMode = WiFiMode::AP;
    WiFi.disconnect();
    WiFi.mode(WIFI_AP);
    String apSsid = getApSsid();
    WiFi.softAP(apSsid.c_str(), WIFI_AP_PASS);
    dnsServer.start(53, "*", WiFi.softAPIP());
    setupWebServer();
    setLedBlink(500);
    logWiFi("Режим точки доступа запущен");
    logSystem("SSID: %s", apSsid.c_str());
    logSystem("IP адрес: %s", WiFi.softAPIP().toString().c_str());
}

void startSTAMode()
{
    currentWiFiMode = WiFiMode::STA;
    WiFi.mode(WIFI_STA);
    String hostname = getApSsid();
    WiFi.setHostname(hostname.c_str());
    if (strlen(config.ssid) > 0)
    {
        WiFi.begin(config.ssid, config.password);
        logWiFi("Подключение к WiFi...");
        int attempts = 0;
        setLedBlink(500);
        while (WiFi.status() != WL_CONNECTED && attempts < 20)
        {
            delay(500);
            updateLed();
            attempts++;
        }
        if (WiFi.status() == WL_CONNECTED)
        {
            wifiConnected = true;
            setLedOn();
            logSuccess("Подключено к WiFi: %s", config.ssid);
            logSystem("IP адрес: %s", WiFi.localIP().toString().c_str());
            setupWebServer();
        }
        else
        {
            logError("Не удалось подключиться к WiFi");
            startAPMode();
        }
    }
    else
    {
        logWarn("SSID не задан, переход в AP");
        startAPMode();
    }
}

bool checkResetButton()
{
    static unsigned long pressStart = 0;
    static bool wasPressed = false;
    bool isPressed = digitalRead(RESET_BUTTON_PIN) == LOW;
    if (isPressed && !wasPressed)
    {
        pressStart = millis();
        wasPressed = true;
        setLedFastBlink();
    }
    else if (!isPressed && wasPressed)
    {
        wasPressed = false;
        setLedBlink(500);
        return false;
    }
    else if (isPressed && wasPressed)
    {
        if (millis() - pressStart >= 5000)
        {
            return true;
        }
    }
    return false;
}

void restartESP()
{
    logWarn("Перезагрузка ESP32...");
    delay(1000);
    ESP.restart();
}

void handleStatus()
{
    String html = "<!DOCTYPE html><html><head><meta charset='UTF-8'>";
    html += "<title>" UI_ICON_STATUS " Статус JXCT</title>";
    html += "<style>" + String(getUnifiedCSS()) + "</style></head><body><div class='container'>";
    html += navHtml();
    html += "<h1>" UI_ICON_STATUS " Статус системы</h1>";
    html += "<div class='section'><h2>WiFi</h2><ul>";
    html += "<li>Режим: " + String(currentWiFiMode == WiFiMode::AP ? "Точка доступа" : "Клиент") + "</li>";
    if (currentWiFiMode == WiFiMode::STA && wifiConnected) {
        html += "<li>SSID: " + String(config.ssid) + "</li>";
        html += "<li>IP: " + WiFi.localIP().toString() + "</li>";
        html += "<li>RSSI: " + String(WiFi.RSSI()) + " dBm</li>";
    }
    html += "</ul></div>";
    html += "<div class='section'><h2>Система</h2><ul>";
    html += "<li>Версия: " + String(DEVICE_SW_VERSION) + "</li>";
    html += "<li>Время работы: " + String(millis() / 1000) + " сек</li>";
    html += "<li>Свободная память: " + String(ESP.getFreeHeap()) + " байт</li>";
    html += "</ul></div>";
    html += "</div>" + String(getToastHTML()) + "</body></html>";
    webServer.send(200, "text/html; charset=utf-8", html);
}

void setupWebServer()
{
    logInfo("🏗️ Настройка модульного веб-сервера v2.4.5...");
    
    // ============================================================================
    // МОДУЛЬНАЯ АРХИТЕКТУРА - Настройка всех маршрутов по группам
    // ============================================================================
    
    setupMainRoutes();     // Основные маршруты (/, /save, /status)
    setupDataRoutes();     // Данные датчика (/readings, /sensor_json, /api/sensor)
    setupConfigRoutes();   // Конфигурация (/intervals, /config_manager, /api/config/*)
    setupServiceRoutes();  // Сервис (/health, /service_status, /reset, /reboot, /ota)
    
    setupErrorHandlers();  // Обработчики ошибок (404, 500) - должны быть последними
    
    // ============================================================================
    // ЗАПУСК СЕРВЕРА
    // ============================================================================
    
    webServer.begin();
    logSuccess("🏗️ Модульный веб-сервер v2.4.5 запущен. Режим: %s", 
              currentWiFiMode == WiFiMode::AP ? "AP" : "STA");
    logSystem("✅ Активные модули: main_routes, data_routes, config_routes, service_routes, error_handlers");
    logSystem("📋 Полный набор маршрутов готов к использованию");
}

void handleRoot()
{
    String html = "<!DOCTYPE html><html><head><meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1.0'>";
    html += "<title>" UI_ICON_CONFIG " Настройки JXCT</title>";
    html += "<style>" + String(getUnifiedCSS()) + "</style></head><body><div class='container'>";
    html += navHtml();
    html += "<h1>" UI_ICON_CONFIG " Настройки JXCT</h1>";
    html += "<form action='/save' method='post'>";
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
            "<div style='color:#888;font-size:13px'>💡 Интервал публикации настраивается в разделе <a href='/intervals' style='color:#4CAF50'>Интервалы</a></div></div>";
        String realSensorChecked = config.flags.useRealSensor ? " checked" : "";
        html += "<div class='section'><h2>Датчик</h2>";
        html +=
            "<div class='form-group'><label for='real_sensor'>Реальный датчик:</label><input type='checkbox' "
            "id='real_sensor' name='real_sensor'" +
            realSensorChecked + "></div></div>";
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
    html += generateButton(ButtonType::PRIMARY, UI_ICON_SAVE, "Сохранить настройки") + "</form>";

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