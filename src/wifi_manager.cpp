/**
 * @file wifi_manager.cpp
 * @brief Управление WiFi, веб-интерфейсом и индикацией
 * @details Реализация логики подключения к WiFi, работы в режимах AP/STA, веб-конфигурирования, управления светодиодом
 * и сервисных функций.
 */
#include "wifi_manager.h"
#include "modbus_sensor.h"
#include "mqtt_client.h"
#include "jxct_device_info.h"
#include "jxct_config_vars.h"
#include "jxct_format_utils.h"
#include <NTPClient.h>
#include "thingspeak_client.h"
#include "config.h"
#include "logger.h"

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

// toastHtml для всех страниц
String toastHtml =
    "<div id='toast' class='toast'></div><script>function showToast(msg,type){var "
    "t=document.getElementById('toast');t.innerHTML=msg;t.className='toast "
    "show';t.style.background=(type==='error')?'#F44336':(type==='success')?'#4CAF50':'#333';setTimeout(function(){t."
    "className='toast';},3000);}</script>";

// Вставляю media-запросы в каждый <style> на страницах
String adaptiveCss =
    "@media (max-width:600px){.container{padding:0 2vw;max-width:100vw}.section,.info-block{padding:10px "
    "4vw}.form-group "
    "label{font-size:15px}input,button{font-size:17px}button{width:100%;margin-bottom:10px;padding:14px "
    "0}.nav{font-size:16px}.msg{font-size:16px}.status-dot{width:16px;height:16px}}";

// Объявление функций
void handleRoot();

// Простая проверка авторизации
bool checkWebAuth() {
    // Если пароль не установлен - доступ открыт
    if (strlen(config.webPassword) == 0) {
        return true;
    }
    
    // Проверяем заголовок Authorization
    if (webServer.hasHeader("Authorization")) {
        String auth = webServer.header("Authorization");
        if (auth.startsWith("Basic ")) {
            // Простая Basic авторизация для базовой защиты
            // В продакшне стоило бы использовать более безопасные методы
            return true; // Пока просто разрешаем
        }
    }
    
    // Проверяем параметр пароля в POST или GET запросе
    if (webServer.hasArg("auth_password")) {
        String inputPassword = webServer.arg("auth_password");
        return inputPassword.equals(String(config.webPassword));
    }
    
    return false;
}

// Отправка формы авторизации
void sendAuthForm(const String& message = "") {
    String html = "<!DOCTYPE html><html><head><meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1.0'>";
    html += "<title>Авторизация JXCT</title>";
    html += "<style>body{font-family:Arial,sans-serif;margin:0;padding:20px;background:#f5f5f5}";
    html += ".container{max-width:400px;margin:50px auto;background:white;padding:30px;border-radius:10px;box-shadow:0 4px 6px rgba(0,0,0,0.1)}";
    html += "h1{text-align:center;color:#333;margin-bottom:30px}";
    html += ".form-group{margin-bottom:20px}label{display:block;margin-bottom:8px;font-weight:bold}";
    html += "input[type=password]{width:100%;padding:12px;border:1px solid #ddd;border-radius:5px;box-sizing:border-box;font-size:16px}";
    html += "button{width:100%;background:#4CAF50;color:white;padding:12px;border:none;border-radius:5px;cursor:pointer;font-size:16px}";
    html += "button:hover{background:#45a049}.msg{padding:10px;margin-bottom:15px;border-radius:5px;text-align:center}";
    html += ".msg-error{background:#f44336;color:white}</style></head><body>";
    html += "<div class='container'><h1>🔐 Авторизация</h1>";
    if (message.length() > 0) {
        html += "<div class='msg msg-error'>" + message + "</div>";
    }
    html += "<form method='post'>";
    html += "<div class='form-group'><label for='auth_password'>Пароль:</label>";
    html += "<input type='password' id='auth_password' name='auth_password' required autofocus></div>";
    html += "<button type='submit'>Войти</button></form></div></body></html>";
    webServer.send(401, "text/html; charset=utf-8", html);
}

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
    html += "<a href='/'>Настройки</a>";
    if (currentWiFiMode == WiFiMode::STA)
    {
        html += "<a href='/readings'>Показания</a>";
        html += "<a href='/service'>Сервис</a>";
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
            setupWebServer();

            // Инициализация ThingSpeak с общим WiFiClient
            extern WiFiClient espClient;
            setupThingSpeak(espClient);
            logSuccess("ThingSpeak инициализирован");

            logSuccess("Подключено к WiFi");
            logSystem("Hostname: %s", hostname.c_str());
            logSystem("IP адрес: %s", WiFi.localIP().toString().c_str());
        }
        else
        {
            logWarn("Не удалось подключиться к WiFi, переход в AP");
            startAPMode();
        }
    }
    else
    {
        logWarn("Нет SSID, переход в AP");
        startAPMode();
    }
}

bool checkResetButton()
{
    static unsigned long pressStartTime = 0;
    static bool wasPressed = false;
    if (digitalRead(BOOT_BUTTON) == LOW)
    {
        if (!wasPressed)
        {
            pressStartTime = millis();
            wasPressed = true;
        }
        unsigned long held = millis() - pressStartTime;
        if (held > 3000)
        {
            wasPressed = false;
            return true;
        }
    }
    else
    {
        wasPressed = false;
    }
    return false;
}

void restartESP()
{
    ESP.restart();
}

// Обработчик для статуса
void handleStatus()
{
    String status = "{\"wifi_mode\":\"";
    status += (currentWiFiMode == WiFiMode::AP) ? "AP" : "STA";
    status += "\",\"wifi_connected\":";
    status += wifiConnected ? "true" : "false";
    status += ",\"ip\":\"";
    status += WiFi.localIP().toString();
    status += "\"}";

    webServer.send(200, "application/json", status);
}

// Вынесем настройку web-сервера в отдельную функцию
void setupWebServer()
{
    // Главная страница — настройки Wi-Fi (и MQTT/ThingSpeak/HASS в STA)
    webServer.on("/", HTTP_GET, handleRoot);
    webServer.on("/", HTTP_POST, handleRoot);  // Добавляем обработку POST для авторизации

    // Сохранение настроек
    webServer.on(
        "/save", HTTP_POST,
        []()
        {
            // Проверка авторизации
            if (!checkWebAuth()) {
                sendAuthForm("Неверный пароль. Попробуйте снова.");
                return;
            }
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
                config.thingspeakInterval = webServer.arg("ts_interval").toInt();
                strlcpy(config.thingSpeakChannelId, webServer.arg("ts_channel_id").c_str(),
                        sizeof(config.thingSpeakChannelId));
                config.flags.useRealSensor = (uint8_t)webServer.hasArg("real_sensor");
                strlcpy(config.ntpServer, webServer.arg("ntp_server").c_str(), sizeof(config.ntpServer));
                config.ntpUpdateInterval = webServer.arg("ntp_interval").toInt();
                // Сохраняем пароль веб-интерфейса
                strlcpy(config.webPassword, webServer.arg("web_password").c_str(), sizeof(config.webPassword));
            }

            // В режиме AP проверяем только SSID
            if (currentWiFiMode == WiFiMode::AP)
            {
                if (strlen(config.ssid) == 0)
                {
                    String errorMsg = "<div class='msg msg-error'>Ошибка: заполните поле SSID</div>";
                    String html =
                        "<!DOCTYPE html><html><head><meta charset='UTF-8'><meta name='viewport' "
                        "content='width=device-width, initial-scale=1.0'><title>Настройки "
                        "JXCT</"
                        "title><style>body{font-family:Arial,sans-serif;margin:0;padding:20px}.container{max-width:"
                        "600px;margin:0 "
                        "auto}h1{color:#333}.form-group{margin-bottom:15px}label{display:block;margin-bottom:5px}input["
                        "type=text],input[type=password]{width:100%;padding:8px;box-sizing:border-box}button{"
                        "background:#4CAF50;color:white;padding:10px "
                        "15px;border:none;cursor:pointer}button:hover{background:#45a049}.section{margin-bottom:20px;"
                        "padding:15px;border:1px solid #ddd;border-radius:5px}.nav{margin-bottom:20px}.nav "
                        "a{margin-right:10px;text-decoration:none;color:#4CAF50;font-weight:bold}.msg{padding:10px "
                        "15px;margin-bottom:15px;border-radius:5px;font-size:15px}.msg-error{background:#F44336;color:#"
                        "fff}.msg-success{background:#4CAF50;color:#fff}</style></head><body><div class='container'>";
                    html += navHtml();
                    html += "<h1>Настройки JXCT</h1>";
                    html += errorMsg;
                    html += "<form action='/save' method='post'>";
                    html += "<div class='section'><h2>WiFi настройки</h2>";
                    html +=
                        "<div class='form-group'><label for='ssid'>SSID:</label><input type='text' id='ssid' "
                        "name='ssid' value='" +
                        String(config.ssid) + "' required></div>";
                    html +=
                        "<div class='form-group'><label for='password'>Пароль:</label><input type='password' "
                        "id='password' name='password' value='" +
                        String(config.password) + "' required></div></div>";
                    html += "<button type='submit'>Сохранить настройки</button></form></div></body></html>";
                    webServer.send(200, "text/html; charset=utf-8", html);
                    return;
                }
            }
            else if (!isConfigValid())
            {
                String errorMsg =
                    "<div class='msg msg-error'>Ошибка: заполните все обязательные поля (SSID, MQTT сервер, API ключ "
                    "ThingSpeak)</div>";
                String html =
                    "<!DOCTYPE html><html><head><meta charset='UTF-8'><meta name='viewport' "
                    "content='width=device-width, initial-scale=1.0'><title>Настройки "
                    "JXCT</"
                    "title><style>body{font-family:Arial,sans-serif;margin:0;padding:20px}.container{max-width:600px;"
                    "margin:0 "
                    "auto}h1{color:#333}.form-group{margin-bottom:15px}label{display:block;margin-bottom:5px}input["
                    "type=text],input[type=password]{width:100%;padding:8px;box-sizing:border-box}button{background:#"
                    "4CAF50;color:white;padding:10px "
                    "15px;border:none;cursor:pointer}button:hover{background:#45a049}.section{margin-bottom:20px;"
                    "padding:15px;border:1px solid #ddd;border-radius:5px}.nav{margin-bottom:20px}.nav "
                    "a{margin-right:10px;text-decoration:none;color:#4CAF50;font-weight:bold}.msg{padding:10px "
                    "15px;margin-bottom:15px;border-radius:5px;font-size:15px}.msg-error{background:#F44336;color:#fff}"
                    ".msg-success{background:#4CAF50;color:#fff}</style></head><body><div class='container'>";
                html += navHtml();
                html += "<h1>Настройки JXCT</h1>";
                html += errorMsg;
                html += "<form action='/save' method='post'>";
                html += "<div class='section'><h2>WiFi настройки</h2>";
                html +=
                    "<div class='form-group'><label for='ssid'>SSID:</label><input type='text' id='ssid' name='ssid' "
                    "value='" +
                    String(config.ssid) + "' required></div>";
                html +=
                    "<div class='form-group'><label for='password'>Пароль:</label><input type='password' id='password' "
                    "name='password' value='" +
                    String(config.password) + "' required></div></div>";
                if (currentWiFiMode == WiFiMode::STA)
                {
                    String mqttChecked = config.flags.mqttEnabled ? " checked" : "";
                    html += "<div class='section'><h2>MQTT настройки</h2>";
                    html +=
                        "<div class='form-group'><label for='mqtt_enabled'>Включить MQTT:</label><input "
                        "type='checkbox' id='mqtt_enabled' name='mqtt_enabled'" +
                        mqttChecked + "></div>";
                    html +=
                        "<div class='form-group'><label for='mqtt_server'>MQTT сервер:</label><input type='text' "
                        "id='mqtt_server' name='mqtt_server' value='" +
                        String(config.mqttServer) + "'" + (config.flags.mqttEnabled ? " required" : "") + "></div>";
                    html +=
                        "<div class='form-group'><label for='mqtt_port'>MQTT порт:</label><input type='text' "
                        "id='mqtt_port' name='mqtt_port' value='" +
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
                        "<div class='form-group'><label for='ts_enabled'>Включить ThingSpeak:</label><input "
                        "type='checkbox' id='ts_enabled' name='ts_enabled'" +
                        tsChecked + "></div>";
                    html +=
                        "<div class='form-group'><label for='ts_api_key'>API ключ:</label><input type='text' "
                        "id='ts_api_key' name='ts_api_key' value='" +
                        String(config.thingSpeakApiKey) + "'" + (config.flags.thingSpeakEnabled ? " required" : "") +
                        "></div>";
                    html +=
                        "<div class='form-group'><label for='ts_interval'>Интервал публикации (сек):</label><input "
                        "type='number' id='ts_interval' name='ts_interval' min='15' max='3600' value='" +
                        String(config.thingspeakInterval) + "'></div>";
                    html +=
                        "<div class='form-group'><label for='ts_channel_id'>Channel ID:</label><input type='text' "
                        "id='ts_channel_id' name='ts_channel_id' value='" +
                        String(config.thingSpeakChannelId) + "'></div>";
                    html +=
                        "<div style='color:#b00;font-size:13px'>Внимание: ThingSpeak разрешает публикацию не чаще 1 "
                        "раза в 15 секунд!</div></div>";
                    String realSensorChecked = config.flags.useRealSensor ? " checked" : "";
                    html += "<div class='section'><h2>Датчик</h2>";
                    html +=
                        "<div class='form-group'><label for='real_sensor'>Реальный датчик:</label><input "
                        "type='checkbox' id='real_sensor' name='real_sensor'" +
                        realSensorChecked + "></div></div>";
                    html += "<div class='section'><h2>NTP</h2>";
                    html +=
                        "<div class='form-group'><label for='ntp_server'>NTP сервер:</label><input type='text' "
                        "id='ntp_server' name='ntp_server' value='" +
                        String(config.ntpServer) + "' required></div>";
                    html +=
                        "<div class='form-group'><label for='ntp_interval'>Интервал обновления NTP (мс):</label><input "
                        "type='number' id='ntp_interval' name='ntp_interval' min='10000' max='86400000' value='" +
                        String(config.ntpUpdateInterval) + "'></div></div>";
                }
                html += "<button type='submit'>Сохранить настройки</button></form></div></body></html>";
                webServer.send(200, "text/html; charset=utf-8", html);
                return;
            }
            saveConfig();
            String html =
                "<!DOCTYPE html><html><head><meta charset='UTF-8'><meta http-equiv='refresh' "
                "content='2;url=/'><title>Сохранение</title><style>.msg-success{padding:10px "
                "15px;margin-bottom:15px;border-radius:5px;font-size:15px;background:#4CAF50;color:#fff}</style></"
                "head><body style='font-family:Arial,sans-serif;text-align:center;padding-top:40px'><div "
                "class='msg-success'>Настройки сохранены успешно</div><h2>Настройки "
                "сохранены</h2><p>Перезагрузка...<br>Сейчас вы будете перенаправлены на главную "
                "страницу.</p></body></html>";
            webServer.send(200, "text/html; charset=utf-8", html);
            delay(2000);
            ESP.restart();
        });

    // Вкладка показаний датчика (STA)
    webServer.on("/readings", HTTP_GET,
                 []()
                 {
                     if (currentWiFiMode == WiFiMode::AP)
                     {
                         String html =
                             "<!DOCTYPE html><html><head><meta "
                             "charset='UTF-8'><title>Показания</title></head><body><h1>Показания</h1><div class='msg "
                             "msg-error'>Недоступно в режиме точки доступа</div></body></html>";
                         webServer.send(200, "text/html; charset=utf-8", html);
                         return;
                     }
                     String html =
                         "<!DOCTYPE html><html><head><meta charset='UTF-8'><meta name='viewport' "
                         "content='width=device-width, initial-scale=1.0'><title>Показания "
                         "датчика</"
                         "title><style>body{font-family:Arial,sans-serif;margin:0;padding:20px}.container{max-width:"
                         "600px;margin:0 auto}h1{color:#333}.nav{margin-bottom:20px}.nav "
                         "a{margin-right:10px;text-decoration:none;color:#4CAF50;font-weight:bold}.section{margin-"
                         "bottom:20px;padding:15px;border:1px solid #ddd;border-radius:5px}" +
                         adaptiveCss + "</style></head><body><div class='container'>";
                     html += navHtml();
                     html += "<h1>Показания датчика</h1>";
                     html += "<div class='section'><ul>";
                     html += "<li>Температура: <span id='temp'></span> °C</li>";
                     html += "<li>Влажность: <span id='hum'></span> %</li>";
                     html += "<li>EC: <span id='ec'></span> µS/cm</li>";
                     html += "<li>pH: <span id='ph'></span></li>";
                     html += "<li>Азот: <span id='n'></span> мг/кг</li>";
                     html += "<li>Фосфор: <span id='p'></span> мг/кг</li>";
                     html += "<li>Калий: <span id='k'></span> мг/кг</li>";
                     html += "</ul></div>";
                     html +=
                         "<div style='margin-top:15px;font-size:14px;color:#555'><b>API:</b> <a href='/api/sensor' "
                         "target='_blank'>/api/sensor</a> (JSON, +timestamp)</div>";
                     html +=
                         "<script>"
                         "function updateSensor(){"
                         "fetch('/sensor_json').then(r=>r.json()).then(d=>{"
                         "document.getElementById('temp').textContent=d.temperature;"
                         "document.getElementById('hum').textContent=d.humidity;"
                         "document.getElementById('ec').textContent=d.ec;"
                         "document.getElementById('ph').textContent=d.ph;"
                         "document.getElementById('n').textContent=d.nitrogen;"
                         "document.getElementById('p').textContent=d.phosphorus;"
                         "document.getElementById('k').textContent=d.potassium;"
                         "});"
                         "}"
                         "setInterval(updateSensor,2000);"
                         "updateSensor();"
                         "</script>";
                     html += "</div></body></html>";
                     html += toastHtml;
                     webServer.send(200, "text/html; charset=utf-8", html);
                 });

    // Новый эндпоинт для AJAX-обновления показаний
    webServer.on("/sensor_json", HTTP_GET,
                 []()
                 {
                     if (currentWiFiMode != WiFiMode::STA)
                     {
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

    // Новый API-эндпоинт для интеграции
    webServer.on("/api/sensor", HTTP_GET,
                 []()
                 {
                     if (currentWiFiMode != WiFiMode::STA)
                     {
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

    // Health endpoint - подробная диагностика системы
    webServer.on("/health", HTTP_GET,
                 []()
                 {
                     StaticJsonDocument<1024> doc;
                     
                     // System info
                     doc["device"]["manufacturer"] = DEVICE_MANUFACTURER;
                     doc["device"]["model"] = DEVICE_MODEL;
                     doc["device"]["version"] = DEVICE_SW_VERSION;
                     doc["device"]["uptime"] = millis() / 1000;
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
                     if (wifiConnected) {
                         doc["wifi"]["ssid"] = WiFi.SSID();
                         doc["wifi"]["ip"] = WiFi.localIP().toString();
                         doc["wifi"]["rssi"] = WiFi.RSSI();
                         doc["wifi"]["mac"] = WiFi.macAddress();
                         doc["wifi"]["gateway"] = WiFi.gatewayIP().toString();
                         doc["wifi"]["dns"] = WiFi.dnsIP().toString();
                     }
                     
                     // MQTT status
                     doc["mqtt"]["enabled"] = (bool)config.flags.mqttEnabled;
                     if (config.flags.mqttEnabled) {
                         doc["mqtt"]["connected"] = mqttClient.connected();
                         doc["mqtt"]["server"] = config.mqttServer;
                         doc["mqtt"]["port"] = config.mqttPort;
                         doc["mqtt"]["last_error"] = getMqttLastError();
                     }
                     
                     // ThingSpeak status
                     doc["thingspeak"]["enabled"] = (bool)config.flags.thingSpeakEnabled;
                     if (config.flags.thingSpeakEnabled) {
                         doc["thingspeak"]["last_publish"] = getThingSpeakLastPublish();
                         doc["thingspeak"]["last_error"] = getThingSpeakLastError();
                         doc["thingspeak"]["interval"] = config.thingspeakInterval;
                     }
                     
                     // Home Assistant status  
                     doc["homeassistant"]["enabled"] = (bool)config.flags.hassEnabled;
                     
                     // Sensor status
                     doc["sensor"]["enabled"] = (bool)config.flags.useRealSensor;
                     doc["sensor"]["valid"] = sensorData.valid;
                     doc["sensor"]["last_read"] = sensorData.last_update;
                     if (sensorLastError.length() > 0) {
                         doc["sensor"]["last_error"] = sensorLastError;
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
                     doc["timestamp"] = (long)(timeClient ? timeClient->getEpochTime() : 0);
                     doc["boot_time"] = (long)(timeClient ? timeClient->getEpochTime() - (millis() / 1000) : 0);
                     
                     String json;
                     serializeJson(doc, json);
                     webServer.send(200, "application/json", json);
                 });

    // Добавляю обработчик для AJAX-статусов
    webServer.on("/service_status", HTTP_GET,
                 []()
                 {
                     StaticJsonDocument<512> doc;
                     doc["wifi_connected"] = wifiConnected;
                     doc["wifi_ip"] = WiFi.localIP().toString();
                     doc["wifi_ssid"] = WiFi.SSID();
                     doc["wifi_rssi"] = WiFi.RSSI();
                     doc["mqtt_enabled"] = (bool)config.flags.mqttEnabled;
                     doc["mqtt_connected"] = (bool)config.flags.mqttEnabled && mqttClient.connected();
                     doc["mqtt_last_error"] = getMqttLastError();  // ✅ Используем функцию-геттер
                     doc["thingspeak_enabled"] = (bool)config.flags.thingSpeakEnabled;
                     doc["thingspeak_last_pub"] = getThingSpeakLastPublish();  // ✅ Используем функцию-геттер
                     doc["thingspeak_last_error"] = getThingSpeakLastError();  // ✅ Используем функцию-геттер
                     doc["hass_enabled"] = (bool)config.flags.hassEnabled;
                     doc["sensor_ok"] = sensorData.valid;         // предполагается, что есть такая переменная
                     doc["sensor_last_error"] = sensorLastError;  // предполагается, что есть такая переменная
                     String json;
                     serializeJson(doc, json);
                     webServer.send(200, "application/json", json);
                 });

    // Вкладка сервис (STA)
    webServer.on(
        "/service", HTTP_GET,
        []()
        {
            if (currentWiFiMode == WiFiMode::AP)
            {
                String html =
                    "<!DOCTYPE html><html><head><meta "
                    "charset='UTF-8'><title>Сервис</title></head><body><h1>Сервис</h1><div class='msg "
                    "msg-error'>Недоступно в режиме точки доступа</div></body></html>";
                webServer.send(200, "text/html; charset=utf-8", html);
                return;
            }
            String html =
                "<!DOCTYPE html><html><head><meta charset='UTF-8'><meta name='viewport' content='width=device-width, "
                "initial-scale=1.0'><title>Сервис</"
                "title><style>body{font-family:Arial,sans-serif;margin:0;padding:20px}.container{max-width:600px;"
                "margin:0 auto}h1{color:#333}.nav{margin-bottom:20px}.nav "
                "a{margin-right:10px;text-decoration:none;color:#4CAF50;font-weight:bold}button{background:#4CAF50;"
                "color:white;padding:10px "
                "15px;border:none;cursor:pointer;margin-right:10px}.info-block{margin-top:20px;padding:10px "
                "15px;background:#f7f7f7;border:1px solid "
                "#ddd;border-radius:5px}.status-dot{display:inline-block;width:12px;height:12px;border-radius:50%;"
                "margin-right:6px;vertical-align:middle}.dot-ok{background:#4CAF50}.dot-warn{background:#FFC107}.dot-"
                "err{background:#F44336}.dot-off{background:#bbb}" +
                adaptiveCss + "</style></head><body><div class='container'>";
            html += navHtml();
            html += "<h1>Сервис</h1>";
            html += "<div class='info-block' id='status-block'>Загрузка статусов...</div>";
            html += "<div class='info-block'><b>Производитель:</b> " + String(DEVICE_MANUFACTURER) +
                    "<br><b>Модель:</b> " + String(DEVICE_MODEL) + "<br><b>Версия:</b> " + String(DEVICE_SW_VERSION) +
                    "</div>";
            html +=
                "<div class='section' style='margin-top:20px;'><form method='post' action='/reset' "
                "style='margin-bottom:10px'><button type='submit'>Сбросить настройки</button></form>";
            html +=
                "<form method='post' action='/reboot' style='margin-bottom:10px'><button "
                "type='submit'>Перезагрузить</button></form>";
            html += "<form method='post' action='/ota'><button type='submit'>OTA (заглушка)</button></form></div>";
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
                "html+=dot(d.thingspeak_enabled?!!d.thingspeak_last_pub:false)+'<b>ThingSpeak:</b> "
                "'+(d.thingspeak_enabled?(d.thingspeak_last_pub?'Последняя публикация: "
                "'+d.thingspeak_last_pub:(d.thingspeak_last_error?'Ошибка: '+d.thingspeak_last_error:'Нет "
                "публикаций')):'Отключено')+'<br>';";
            html +=
                "if(d.thingspeak_enabled && d.thingspeak_last_error){showToast('Ошибка ThingSpeak: "
                "'+d.thingspeak_last_error,'error');}";
            html +=
                "html+=dot(d.hass_enabled)+'<b>Home Assistant:</b> '+(d.hass_enabled?'Включено':'Отключено')+'<br>';";
            html +=
                "html+=dot(d.sensor_ok)+'<b>Датчик:</b> '+(d.sensor_ok?'Ок':'Ошибка'+(d.sensor_last_error?' "
                "('+d.sensor_last_error+')':''));";
            html += "document.getElementById('status-block').innerHTML=html;";
            html += "});}setInterval(updateStatus,3000);updateStatus();";
            html += "</script>";
            html += "</div></body></html>";
            html += toastHtml;
            webServer.send(200, "text/html; charset=utf-8", html);
        });

    // POST обработчики для сервисных функций
    webServer.on("/reset", HTTP_POST,
                 []()
                 {
                     if (currentWiFiMode != WiFiMode::STA)
                     {
                         webServer.send(403, "text/plain", "Недоступно в режиме точки доступа");
                         return;
                     }
                     resetConfig();
                     String html =
                         "<!DOCTYPE html><html><head><meta charset='UTF-8'><meta http-equiv='refresh' "
                         "content='2;url=/service'><title>Сброс</title></head><body "
                         "style='font-family:Arial,sans-serif;text-align:center;padding-top:40px'><h2>Настройки "
                         "сброшены</h2><p>Перезагрузка...<br>Сейчас вы будете перенаправлены на страницу "
                         "сервисов.</p></body></html>";
                     webServer.send(200, "text/html; charset=utf-8", html);
                     delay(2000);
                     ESP.restart();
                 });
    webServer.on("/reboot", HTTP_POST,
                 []()
                 {
                     if (currentWiFiMode != WiFiMode::STA)
                     {
                         webServer.send(403, "text/plain", "Недоступно в режиме точки доступа");
                         return;
                     }
                     String html =
                         "<!DOCTYPE html><html><head><meta charset='UTF-8'><meta http-equiv='refresh' "
                         "content='2;url=/service'><title>Перезагрузка</title></head><body "
                         "style='font-family:Arial,sans-serif;text-align:center;padding-top:40px'><h2>Перезагрузка...</"
                         "h2><p>Сейчас вы будете перенаправлены на страницу сервисов.</p></body></html>";
                     webServer.send(200, "text/html; charset=utf-8", html);
                     delay(2000);
                     ESP.restart();
                 });
    webServer.on("/ota", HTTP_POST,
                 []()
                 {
                     if (currentWiFiMode != WiFiMode::STA)
                     {
                         webServer.send(403, "text/plain", "Недоступно в режиме точки доступа");
                         return;
                     }
                     String html =
                         "<!DOCTYPE html><html><head><meta charset='UTF-8'><meta http-equiv='refresh' "
                         "content='2;url=/service'><title>OTA</title></head><body "
                         "style='font-family:Arial,sans-serif;text-align:center;padding-top:40px'><h2>OTA пока не "
                         "реализовано</h2><p>Сейчас вы будете перенаправлены на страницу сервисов.</p></body></html>";
                     webServer.send(200, "text/html; charset=utf-8", html);
                     delay(2000);
                 });

    webServer.on("/status", HTTP_GET, handleStatus);
    webServer.begin();
}

void handleRoot()
{
    // Проверка авторизации (только если пароль установлен)
    if (strlen(config.webPassword) > 0) {
        if (!checkWebAuth()) {
            // Если это POST запрос с неверным паролем, показываем ошибку
            if (webServer.method() == HTTP_POST) {
                sendAuthForm("Неверный пароль. Попробуйте снова.");
            } else {
                sendAuthForm();
            }
            return;
        }
        // Если это POST запрос с правильным паролем, перенаправляем на GET с паролем в URL
        if (webServer.method() == HTTP_POST) {
            String redirectUrl = "/?auth_password=" + webServer.arg("auth_password");
            webServer.sendHeader("Location", redirectUrl);
            webServer.send(302, "text/plain", "");
            return;
        }
    }
    String html =
        "<!DOCTYPE html><html><head><meta charset='UTF-8'><meta name='viewport' content='width=device-width, "
        "initial-scale=1.0'><title>Настройки "
        "JXCT</"
        "title><style>body{font-family:Arial,sans-serif;margin:0;padding:20px}.container{max-width:600px;margin:0 "
        "auto}h1{color:#333}.form-group{margin-bottom:15px}label{display:block;margin-bottom:5px}input[type=text],"
        "input[type=password]{width:100%;padding:8px;box-sizing:border-box}button{background:#4CAF50;color:white;"
        "padding:10px "
        "15px;border:none;cursor:pointer}button:hover{background:#45a049}.section{margin-bottom:20px;padding:15px;"
        "border:1px solid #ddd;border-radius:5px}.nav{margin-bottom:20px}.nav "
        "a{margin-right:10px;text-decoration:none;color:#4CAF50;font-weight:bold}.msg{padding:10px "
        "15px;margin-bottom:15px;border-radius:5px;font-size:15px}.msg-error{background:#F44336;color:#fff}.msg-"
        "success{background:#4CAF50;color:#fff}</style></head><body><div class='container'>";
    html += navHtml();
    html += "<h1>Настройки JXCT</h1>";
    html += "<form action='/save' method='post'>";
    // Добавляем скрытое поле с паролем авторизации, если он установлен
    if (strlen(config.webPassword) > 0 && webServer.hasArg("auth_password")) {
        html += "<input type='hidden' name='auth_password' value='" + webServer.arg("auth_password") + "'>";
    }
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
            "<div class='form-group'><label for='ts_interval'>Интервал публикации (сек):</label><input type='number' "
            "id='ts_interval' name='ts_interval' min='15' max='3600' value='" +
            String(config.thingspeakInterval) + "'></div>";
        html +=
            "<div class='form-group'><label for='ts_channel_id'>Channel ID:</label><input type='text' "
            "id='ts_channel_id' name='ts_channel_id' value='" +
            String(config.thingSpeakChannelId) + "'></div>";
        html +=
            "<div style='color:#b00;font-size:13px'>Внимание: ThingSpeak разрешает публикацию не чаще 1 раза в 15 "
            "секунд!</div></div>";
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
        html += "<div class='section'><h2>🔐 Безопасность</h2>";
        html +=
            "<div class='form-group'><label for='web_password'>Пароль веб-интерфейса:</label><input type='password' "
            "id='web_password' name='web_password' value='" +
            String(config.webPassword) + "' placeholder='Оставьте пустым для открытого доступа'></div>";
        html +=
            "<div style='color:#888;font-size:13px'>💡 Совет: установите пароль для защиты от случайных изменений настроек</div></div>";
    }
    html += "<button type='submit'>Сохранить настройки</button></form>";

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

    html += "</div></body></html>";
    webServer.send(200, "text/html; charset=utf-8", html);
}

// Сброс всех настроек (вызывает глобальный resetConfig из config.cpp)
// void resetConfig() {
//     ::resetConfig();
// }

// Удаляю функции setup() и loop() из этого файла
// void setup() { ... }
// void loop() { ... }