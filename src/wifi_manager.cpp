/**
 * @file wifi_manager.cpp
 * @brief Управление WiFi, веб-интерфейсом и индикацией
 * @details Реализация логики подключения к WiFi, работы в режимах AP/STA, веб-конфигурирования, управления светодиодом и сервисных функций.
 */
#include "wifi_manager.h"
#include "config.h"
#include "modbus_sensor.h"
#include "mqtt_client.h"

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

void setLedOn() {
    digitalWrite(STATUS_LED_PIN, HIGH);
    ledBlinkInterval = 0;
    ledFastBlink = false;
}

void setLedOff() {
    digitalWrite(STATUS_LED_PIN, LOW);
    ledBlinkInterval = 0;
    ledFastBlink = false;
}

void setLedBlink(unsigned long interval) {
    ledBlinkInterval = interval;
    ledFastBlink = false;
}

void setLedFastBlink() {
    ledBlinkInterval = 100;
    ledFastBlink = true;
}

void updateLed() {
    if (ledBlinkInterval > 0) {
        unsigned long now = millis();
        if (now - ledLastToggle >= ledBlinkInterval) {
            ledLastToggle = now;
            ledState = !ledState;
            digitalWrite(STATUS_LED_PIN, ledState ? HIGH : LOW);
        }
    }
}

// HTML для навигации
String navHtml() {
    return "<div class='nav'>"
           "<a href='/'>Настройки</a>"
           "<a href='/sensor'>Показания</a>"
           "<a href='/service'>Сервис</a>"
           "</div>";
}

void setupWiFi() {
    pinMode(STATUS_LED_PIN, OUTPUT);
    setLedBlink(500);
    WiFi.mode(WIFI_AP_STA);
    loadConfig();
    Serial.print("[setupWiFi] config.ssid: "); Serial.println(config.ssid);
    Serial.print("[setupWiFi] config.password: "); Serial.println(config.password);
    if (strlen(config.ssid) > 0 && strlen(config.password) > 0) {
        Serial.println("[setupWiFi] Переход в STA");
        startSTAMode();
    } else {
        Serial.println("[setupWiFi] Переход в AP");
        startAPMode();
    }
}

void handleWiFi() {
    updateLed();
    if (currentWiFiMode == WiFiMode::AP) {
        dnsServer.processNextRequest();
        webServer.handleClient();
        if (WiFi.softAPgetStationNum() > 0) {
            setLedOn();
        } else {
            setLedBlink(500);
        }
    } else if (currentWiFiMode == WiFiMode::STA) {
        if (WiFi.status() != WL_CONNECTED) {
            wifiConnected = false;
            setLedBlink(500);
            Serial.println("[handleWiFi] Потеряно соединение с WiFi, переход в AP");
            startAPMode();
        } else if (!wifiConnected) {
            wifiConnected = true;
            setLedOn();
            Serial.println("[handleWiFi] Подключено к WiFi");
            Serial.print("[handleWiFi] IP адрес: ");
            Serial.println(WiFi.localIP());
        }
        webServer.handleClient();
    }
}

void startAPMode() {
    currentWiFiMode = WiFiMode::AP;
    WiFi.disconnect();
    WiFi.mode(WIFI_AP);
    WiFi.softAP(WIFI_AP_SSID, WIFI_AP_PASS);
    dnsServer.start(53, "*", WiFi.softAPIP());
    setupWebServer();
    setLedBlink(500);
    Serial.println("[startAPMode] Режим точки доступа запущен");
    Serial.print("[startAPMode] IP адрес: ");
    Serial.println(WiFi.softAPIP());
}

void startSTAMode() {
    currentWiFiMode = WiFiMode::STA;
    WiFi.mode(WIFI_STA);
    if (strlen(config.ssid) > 0) {
        WiFi.begin(config.ssid, config.password);
        Serial.println("[startSTAMode] Подключение к WiFi...");
        int attempts = 0;
        setLedBlink(500);
        while (WiFi.status() != WL_CONNECTED && attempts < 20) {
            delay(500);
            updateLed();
            Serial.print(".");
            attempts++;
        }
        if (WiFi.status() == WL_CONNECTED) {
            wifiConnected = true;
            setLedOn();
            setupWebServer();
            Serial.println("\n[startSTAMode] Подключено к WiFi");
            Serial.print("[startSTAMode] IP адрес: ");
            Serial.println(WiFi.localIP());
        } else {
            Serial.println("\n[startSTAMode] Не удалось подключиться к WiFi, переход в AP");
            startAPMode();
        }
    } else {
        Serial.println("[startSTAMode] Нет SSID, переход в AP");
        startAPMode();
    }
}

bool checkResetButton() {
    static unsigned long pressStartTime = 0;
    static bool wasPressed = false;
    if (digitalRead(BOOT_BUTTON) == LOW) {
        if (!wasPressed) {
            pressStartTime = millis();
            wasPressed = true;
        }
        unsigned long held = millis() - pressStartTime;
        if (held > 3000) {
            wasPressed = false;
            return true;
        }
    } else {
        wasPressed = false;
    }
    return false;
}

void restartESP() {
    ESP.restart();
}

// Обработчик для статуса
void handleStatus() {
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
void setupWebServer() {
    // Главная страница — настройки Wi-Fi (и MQTT/ThingSpeak/HASS в STA)
    webServer.on("/", HTTP_GET, []() {
        String html = "<!DOCTYPE html><html><head><meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1.0'><title>Настройки JXCT</title><style>body{font-family:Arial,sans-serif;margin:0;padding:20px}.container{max-width:600px;margin:0 auto}h1{color:#333}.form-group{margin-bottom:15px}label{display:block;margin-bottom:5px}input[type=text],input[type=password]{width:100%;padding:8px;box-sizing:border-box}button{background:#4CAF50;color:white;padding:10px 15px;border:none;cursor:pointer}button:hover{background:#45a049}.section{margin-bottom:20px;padding:15px;border:1px solid #ddd;border-radius:5px}.nav{margin-bottom:20px}.nav a{margin-right:10px;text-decoration:none;color:#4CAF50;font-weight:bold}</style></head><body><div class='container'>";
        html += navHtml();
        html += "<h1>Настройки JXCT</h1><form action='/save' method='post'>";
        html += "<div class='section'><h2>WiFi настройки</h2>";
        html += "<div class='form-group'><label for='ssid'>SSID:</label><input type='text' id='ssid' name='ssid' value='" + String(config.ssid) + "' required></div>";
        html += "<div class='form-group'><label for='password'>Пароль:</label><input type='password' id='password' name='password' value='" + String(config.password) + "' required></div></div>";
        if (currentWiFiMode == WiFiMode::STA) {
            String mqttChecked = config.mqttEnabled ? " checked" : "";
            html += "<div class='section'><h2>MQTT настройки</h2>";
            html += "<div class='form-group'><label for='mqtt_enabled'>Включить MQTT:</label><input type='checkbox' id='mqtt_enabled' name='mqtt_enabled'" + mqttChecked + "></div>";
            html += "<div class='form-group'><label for='mqtt_server'>MQTT сервер:</label><input type='text' id='mqtt_server' name='mqtt_server' value='" + String(config.mqttServer) + "'></div>";
            html += "<div class='form-group'><label for='mqtt_port'>MQTT порт:</label><input type='text' id='mqtt_port' name='mqtt_port' value='" + String(config.mqttPort) + "'></div>";
            html += "<div class='form-group'><label for='mqtt_user'>MQTT пользователь:</label><input type='text' id='mqtt_user' name='mqtt_user' value='" + String(config.mqttUser) + "'></div>";
            html += "<div class='form-group'><label for='mqtt_password'>MQTT пароль:</label><input type='password' id='mqtt_password' name='mqtt_password' value='" + String(config.mqttPassword) + "'></div>";
            html += "<div class='form-group'><label for='mqtt_topic'>MQTT префикс топика:</label><input type='text' id='mqtt_topic' name='mqtt_topic' value='" + String(config.mqttTopicPrefix) + "'></div>";
            html += "<div class='form-group'><label for='mqtt_device_name'>Имя устройства:</label><input type='text' id='mqtt_device_name' name='mqtt_device_name' value='" + String(config.mqttDeviceName) + "'></div>";
            String hassChecked = config.hassEnabled ? " checked" : "";
            html += "<div class='form-group'><label for='hass_enabled'>Интеграция с Home Assistant:</label><input type='checkbox' id='hass_enabled' name='hass_enabled'" + hassChecked + "></div></div>";
            String tsChecked = config.thingSpeakEnabled ? " checked" : "";
            html += "<div class='section'><h2>ThingSpeak настройки</h2>";
            html += "<div class='form-group'><label for='ts_enabled'>Включить ThingSpeak:</label><input type='checkbox' id='ts_enabled' name='ts_enabled'" + tsChecked + "></div>";
            html += "<div class='form-group'><label for='ts_api_key'>API ключ:</label><input type='text' id='ts_api_key' name='ts_api_key' value='" + String(config.thingSpeakApiKey) + "'></div>";
            html += "<div class='form-group'><label for='ts_interval'>Интервал публикации (сек):</label><input type='number' id='ts_interval' name='ts_interval' min='15' max='3600' value='" + String(config.thingspeakInterval) + "'></div>";
            html += "<div class='form-group'><label for='ts_channel_id'>Channel ID:</label><input type='text' id='ts_channel_id' name='ts_channel_id' value='" + String(config.thingSpeakChannelId) + "'></div>";
            html += "<div style='color:#b00;font-size:13px'>Внимание: ThingSpeak разрешает публикацию не чаще 1 раза в 15 секунд!</div></div>";
            String realSensorChecked = config.useRealSensor ? " checked" : "";
            html += "<div class='section'><h2>Датчик</h2>";
            html += "<div class='form-group'><label for='real_sensor'>Реальный датчик:</label><input type='checkbox' id='real_sensor' name='real_sensor'" + realSensorChecked + "></div></div>";
        }
        html += "<button type='submit'>Сохранить настройки</button></form></div></body></html>";
        webServer.send(200, "text/html; charset=utf-8", html);
    });

    // Сохранение настроек
    webServer.on("/save", HTTP_POST, []() {
        strlcpy(config.ssid, webServer.arg("ssid").c_str(), sizeof(config.ssid));
        strlcpy(config.password, webServer.arg("password").c_str(), sizeof(config.password));
        Serial.print("[WEB /save] Сохраняю SSID: "); Serial.println(config.ssid);
        Serial.print("[WEB /save] Сохраняю PASSWORD: "); Serial.println(config.password);
        if (currentWiFiMode == WiFiMode::STA) {
            config.mqttEnabled = webServer.hasArg("mqtt_enabled");
            strlcpy(config.mqttServer, webServer.arg("mqtt_server").c_str(), sizeof(config.mqttServer));
            if (strlen(config.mqttServer) == 0) config.mqttEnabled = false;
            config.mqttPort = webServer.arg("mqtt_port").toInt();
            strlcpy(config.mqttUser, webServer.arg("mqtt_user").c_str(), sizeof(config.mqttUser));
            strlcpy(config.mqttPassword, webServer.arg("mqtt_password").c_str(), sizeof(config.mqttPassword));
            strlcpy(config.mqttTopicPrefix, webServer.arg("mqtt_topic").c_str(), sizeof(config.mqttTopicPrefix));
            strlcpy(config.mqttDeviceName, webServer.arg("mqtt_device_name").c_str(), sizeof(config.mqttDeviceName));
            config.hassEnabled = webServer.hasArg("hass_enabled");
            config.thingSpeakEnabled = webServer.hasArg("ts_enabled");
            strlcpy(config.thingSpeakApiKey, webServer.arg("ts_api_key").c_str(), sizeof(config.thingSpeakApiKey));
            config.mqttQos = webServer.arg("mqtt_qos").toInt();
            config.thingspeakInterval = webServer.arg("ts_interval").toInt();
            strlcpy(config.thingSpeakChannelId, webServer.arg("ts_channel_id").c_str(), sizeof(config.thingSpeakChannelId));
            config.useRealSensor = webServer.hasArg("real_sensor");
            Serial.print("[WEB /save] MQTT сервер: "); Serial.println(config.mqttServer);
            Serial.print("[WEB /save] MQTT порт: "); Serial.println(config.mqttPort);
            Serial.print("[WEB /save] MQTT пользователь: "); Serial.println(config.mqttUser);
            Serial.print("[WEB /save] MQTT пароль: "); Serial.println(config.mqttPassword);
            Serial.print("[WEB /save] MQTT топик: "); Serial.println(config.mqttTopicPrefix);
            Serial.print("[WEB /save] MQTT имя устройства: "); Serial.println(config.mqttDeviceName);
            Serial.print("[WEB /save] HASS: "); Serial.println(config.hassEnabled);
            Serial.print("[WEB /save] ThingSpeak: "); Serial.println(config.thingSpeakEnabled);
            Serial.print("[WEB /save] TS API: "); Serial.println(config.thingSpeakApiKey);
            Serial.print("[WEB /save] TS Interval: "); Serial.println(config.thingspeakInterval);
            Serial.print("[WEB /save] MQTT QoS: "); Serial.println(config.mqttQos);
            Serial.print("[WEB /save] useRealSensor: "); Serial.println(config.useRealSensor);
            Serial.print("[WEB /save] TS Channel ID: "); Serial.println(config.thingSpeakChannelId);
        }
        saveConfig();
        String html = "<!DOCTYPE html><html><head><meta charset='UTF-8'><meta http-equiv='refresh' content='2;url=/'><title>Сохранение</title></head><body style='font-family:Arial,sans-serif;text-align:center;padding-top:40px'><h2>Настройки сохранены</h2><p>Перезагрузка...<br>Сейчас вы будете перенаправлены на главную страницу.</p></body></html>";
        webServer.send(200, "text/html; charset=utf-8", html);
        delay(2000);
        ESP.restart();
    });

    // Вкладка показаний датчика (STA)
    webServer.on("/sensor", HTTP_GET, []() {
        if (currentWiFiMode != WiFiMode::STA) {
            webServer.send(403, "text/plain", "Недоступно в режиме точки доступа");
            return;
        }
        String html = "<!DOCTYPE html><html><head><meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1.0'><title>Показания датчика</title><style>body{font-family:Arial,sans-serif;margin:0;padding:20px}.container{max-width:600px;margin:0 auto}h1{color:#333}.nav{margin-bottom:20px}.nav a{margin-right:10px;text-decoration:none;color:#4CAF50;font-weight:bold}</style></head><body><div class='container'>";
        html += navHtml();
        html += "<h1>Показания датчика</h1>";
        html += "<ul>";
        html += "<li>Температура: " + String(sensorData.temperature) + " °C</li>";
        html += "<li>Влажность: " + String(sensorData.humidity) + " %</li>";
        html += "<li>EC: " + String(sensorData.ec) + " µS/cm</li>";
        html += "<li>pH: " + String(sensorData.ph) + "</li>";
        html += "<li>Азот: " + String(sensorData.nitrogen) + " мг/кг</li>";
        html += "<li>Фосфор: " + String(sensorData.phosphorus) + " мг/кг</li>";
        html += "<li>Калий: " + String(sensorData.potassium) + " мг/кг</li>";
        html += "</ul></div></body></html>";
        webServer.send(200, "text/html; charset=utf-8", html);
    });

    // Вкладка сервис (STA)
    webServer.on("/service", HTTP_GET, []() {
        if (currentWiFiMode != WiFiMode::STA) {
            webServer.send(403, "text/plain", "Недоступно в режиме точки доступа");
            return;
        }
        String html = "<!DOCTYPE html><html><head><meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1.0'><title>Сервис</title><style>body{font-family:Arial,sans-serif;margin:0;padding:20px}.container{max-width:600px;margin:0 auto}h1{color:#333}.nav{margin-bottom:20px}.nav a{margin-right:10px;text-decoration:none;color:#4CAF50;font-weight:bold}button{background:#4CAF50;color:white;padding:10px 15px;border:none;cursor:pointer;margin-right:10px}button:hover{background:#45a049}form{display:inline-block;margin-right:10px}.info-block{margin-top:20px;padding:10px 15px;background:#f7f7f7;border:1px solid #ddd;border-radius:5px}</style></head><body><div class='container'>";
        html += navHtml();
        html += "<h1>Сервис</h1>";
        // Блок статусов
        html += "<div class='info-block'><b>WiFi:</b> " + String(wifiConnected ? "Подключено (" + WiFi.localIP().toString() + ")" : "Не подключено") + "<br>";
        html += "<b>MQTT:</b> " + String((config.mqttEnabled && mqttClient.connected()) ? "Подключено" : "Не подключено") + "<br>";
        html += "<b>ThingSpeak:</b> " + String(config.thingSpeakEnabled ? "Включено" : "Выключено") + "<br>";
        html += "<b>Home Assistant:</b> " + String(config.hassEnabled ? "Включено" : "Выключено") + "</div>";
        // Блок справочной информации
        html += "<div class='info-block'><b>Производитель:</b> Eyera<br><b>Модель:</b> JXCT-7in1<br><b>Версия:</b> 1.0</div>";
        html += "<form method='post' action='/reset'><button type='submit'>Сбросить настройки</button></form>";
        html += "<form method='post' action='/reboot'><button type='submit'>Перезагрузить</button></form>";
        html += "<form method='post' action='/ota'><button type='submit'>OTA (заглушка)</button></form>";
        html += "</div></body></html>";
        webServer.send(200, "text/html; charset=utf-8", html);
    });

    // POST обработчики для сервисных функций
    webServer.on("/reset", HTTP_POST, []() {
        if (currentWiFiMode != WiFiMode::STA) {
            webServer.send(403, "text/plain", "Недоступно в режиме точки доступа");
            return;
        }
        resetConfig();
        String html = "<!DOCTYPE html><html><head><meta charset='UTF-8'><meta http-equiv='refresh' content='2;url=/service'><title>Сброс</title></head><body style='font-family:Arial,sans-serif;text-align:center;padding-top:40px'><h2>Настройки сброшены</h2><p>Перезагрузка...<br>Сейчас вы будете перенаправлены на страницу сервисов.</p></body></html>";
        webServer.send(200, "text/html; charset=utf-8", html);
        delay(2000);
        ESP.restart();
    });
    webServer.on("/reboot", HTTP_POST, []() {
        if (currentWiFiMode != WiFiMode::STA) {
            webServer.send(403, "text/plain", "Недоступно в режиме точки доступа");
            return;
        }
        String html = "<!DOCTYPE html><html><head><meta charset='UTF-8'><meta http-equiv='refresh' content='2;url=/service'><title>Перезагрузка</title></head><body style='font-family:Arial,sans-serif;text-align:center;padding-top:40px'><h2>Перезагрузка...</h2><p>Сейчас вы будете перенаправлены на страницу сервисов.</p></body></html>";
        webServer.send(200, "text/html; charset=utf-8", html);
        delay(2000);
        ESP.restart();
    });
    webServer.on("/ota", HTTP_POST, []() {
        if (currentWiFiMode != WiFiMode::STA) {
            webServer.send(403, "text/plain", "Недоступно в режиме точки доступа");
            return;
        }
        String html = "<!DOCTYPE html><html><head><meta charset='UTF-8'><meta http-equiv='refresh' content='2;url=/service'><title>OTA</title></head><body style='font-family:Arial,sans-serif;text-align:center;padding-top:40px'><h2>OTA пока не реализовано</h2><p>Сейчас вы будете перенаправлены на страницу сервисов.</p></body></html>";
        webServer.send(200, "text/html; charset=utf-8", html);
        delay(2000);
    });

    webServer.on("/status", HTTP_GET, handleStatus);
    webServer.begin();
}

// Сброс всех настроек (вызывает глобальный resetConfig из config.cpp)
// void resetConfig() {
//     ::resetConfig();
// }

// Удаляю функции setup() и loop() из этого файла
// void setup() { ... }
// void loop() { ... } 