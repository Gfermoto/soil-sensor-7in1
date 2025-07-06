/**
 * @file wifi_manager.cpp
 * @brief Управление WiFi, веб-интерфейсом и индикацией
 * @details Реализация логики подключения к WiFi, работы в режимах AP/STA, веб-конфигурирования, управления светодиодом
 * и сервисных функций.
 */
#include "wifi_manager.h"
#include <NTPClient.h>
#include "jxct_config_vars.h"
#include "jxct_constants.h"
#include "jxct_device_info.h"
#include "jxct_format_utils.h"
#include "jxct_ui_system.h"  // 🎨 Единая система дизайна v2.3.1
#include "logger.h"
#include "modbus_sensor.h"
#include "mqtt_client.h"
#include "thingspeak_client.h"
#include "web/csrf_protection.h"  // 🔒 CSRF защита
#include "web_routes.h"           // 🏗️ Модульная архитектура v2.4.5

// Константы
#define RESET_BUTTON_PIN 0             // GPIO0 для кнопки сброса
#define WIFI_RECONNECT_INTERVAL 30000  // Интервал между попытками переподключения (30 секунд)

// Константы для светодиода и таймингов
#define LED_FAST_BLINK_INTERVAL 100  // Интервал быстрого мигания светодиода (мс)
#define LED_SLOW_BLINK_INTERVAL 500  // Интервал медленного мигания светодиода (мс)
#define WIFI_MODE_DELAY 100          // Задержка при смене режима WiFi (мс)
#define NTP_TIMEOUT_MS 5000          // Таймаут для NTP синхронизации (мс)
#define RESET_BUTTON_HOLD_TIME 5000  // Время удержания кнопки сброса (мс)
#define RESTART_DELAY_MS 1000        // Задержка перед перезагрузкой (мс)
#define DNS_PORT 53                  // Порт DNS сервера
#define MAC_ADDRESS_BUFFER_SIZE 20   // Размер буфера для MAC адреса

// Глобальные переменные
bool wifiConnected = false;
WiFiMode currentWiFiMode = WiFiMode::AP;
WebServer webServer(DEFAULT_WEB_SERVER_PORT);  // Используем константу из jxct_constants.h
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
    ledBlinkInterval = LED_FAST_BLINK_INTERVAL;
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
        html += "<a href='/updates'>🚀 Обновления</a>";
        html += "<a href='/service'>" UI_ICON_SERVICE " Сервис</a>";
    }
    html += "</div>";
    return html;
}

void setupWiFi()
{
    logPrintHeader("ИНИЦИАЛИЗАЦИЯ WiFi", LogColor::GREEN);

    pinMode(STATUS_LED_PIN, OUTPUT);
    setLedBlink(LED_SLOW_BLINK_INTERVAL);

    // Сначала отключаем WiFi и очищаем настройки
    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);
    delay(WIFI_MODE_DELAY);

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
    logPrintSeparator("─", DEFAULT_SEPARATOR_LENGTH);
}

void handleWiFi()
{
    updateLed();
    if (currentWiFiMode == WiFiMode::AP)
    {
        dnsServer.processNextRequest();
        webServer.handleClient();

        // Периодическая попытка вернуться в STA-режим, если точка доступа пуста
        static unsigned long lastStaRetry = 0;
        if (WiFi.softAPgetStationNum() == 0 &&                       // никого не подключено
            millis() - lastStaRetry >= WIFI_RECONNECT_INTERVAL &&    // прошло ≥ интервала
            strlen(config.ssid) > 0 && strlen(config.password) > 0)  // есть сохранённые уч. данные
        {
            lastStaRetry = millis();
            logWiFi("AP режим: пробуем снова подключиться к WiFi \"%s\"", config.ssid);
            startSTAMode();  // если не получится, функция сама вернёт нас в AP
            return;          // ждём следующего цикла
        }

        if (WiFi.softAPgetStationNum() > 0)
        {
            setLedOn();
        }
        else
        {
            setLedBlink(WIFI_RETRY_DELAY_MS);
        }
    }
    else if (currentWiFiMode == WiFiMode::STA)
    {
        static unsigned long lastReconnectAttempt = 0;
        static int reconnectAttempts = 0;
        const int MAX_RECONNECT_ATTEMPTS = 3;  // Максимальное количество попыток переподключения перед переходом в AP

        if (WiFi.status() != WL_CONNECTED)
        {
            if (!wifiConnected || (millis() - lastReconnectAttempt >= WIFI_RECONNECT_INTERVAL))
            {
                wifiConnected = false;
                setLedBlink(WIFI_RETRY_DELAY_MS);

                if (reconnectAttempts < MAX_RECONNECT_ATTEMPTS)
                {
                    logWarn("Потеряно соединение с WiFi, попытка переподключения %d из %d", reconnectAttempts + 1,
                            MAX_RECONNECT_ATTEMPTS);

                    WiFi.disconnect(true);
                    delay(WIFI_MODE_DELAY);
                    WiFi.begin(config.ssid, config.password);

                    lastReconnectAttempt = millis();
                    reconnectAttempts++;
                }
                else
                {
                    logError("Не удалось восстановить соединение после %d попыток, переход в AP",
                             MAX_RECONNECT_ATTEMPTS);
                    startAPMode();
                    reconnectAttempts = 0;  // Сбрасываем счетчик для следующей сессии
                }
            }
        }
        else
        {
            if (!wifiConnected)
            {
                wifiConnected = true;
                reconnectAttempts = 0;  // Сбрасываем счетчик при успешном подключении
                setLedOn();
                logSuccess("Подключено к WiFi, IP: %s", WiFi.localIP().toString().c_str());
            }
        }
        webServer.handleClient();
    }
}

String getApSsid()
{
    uint8_t mac[6];
    WiFi.macAddress(mac);
    char buf[MAC_ADDRESS_BUFFER_SIZE];
    snprintf(buf, sizeof(buf), "jxct-%02X%02X%02X", mac[3], mac[4], mac[5]);
    for (int i = 0; buf[i]; ++i) {
        buf[i] = tolower(buf[i]);
    }
    return String(buf);
}

void startAPMode()
{
    currentWiFiMode = WiFiMode::AP;
    WiFi.disconnect();
    WiFi.mode(WIFI_AP);
    String apSsid = getApSsid();
    WiFi.softAP(apSsid.c_str(), JXCT_WIFI_AP_PASS);
    dnsServer.start(DNS_PORT, "*", WiFi.softAPIP());
    setupWebServer();
    setLedBlink(LED_SLOW_BLINK_INTERVAL);
    logWiFi("Режим точки доступа запущен");
    logSystem("SSID: %s", apSsid.c_str());
    logSystem("IP адрес: %s", WiFi.softAPIP().toString().c_str());
}

void startSTAMode()
{
    currentWiFiMode = WiFiMode::STA;
    WiFi.disconnect(true);  // Полное отключение с очисткой настроек
    WiFi.mode(WIFI_STA);
    delay(WIFI_MODE_DELAY);  // Даем время на применение режима

    String hostname = getApSsid();
    WiFi.setHostname(hostname.c_str());

    if (strlen(config.ssid) > 0)
    {
        logWiFi("Подключение к WiFi...");
        WiFi.begin(config.ssid, config.password);  // Явно вызываем подключение

        int attempts = 0;
        setLedBlink(WIFI_RETRY_DELAY_MS);
        unsigned long startTime = millis();

        while (WiFi.status() != WL_CONNECTED && attempts < WIFI_CONNECTION_ATTEMPTS &&
               (millis() - startTime) < WIFI_CONNECTION_TIMEOUT)
        {
            delay(WIFI_RETRY_DELAY_MS);
            updateLed();
            attempts++;
            logDebug("Попытка подключения %d из %d", attempts, WIFI_CONNECTION_ATTEMPTS);

            // Проверяем кнопку сброса во время подключения
            if (checkResetButton())
            {
                logWarn("Обнаружено длительное нажатие кнопки во время подключения");
                startAPMode();
                return;
            }
        }

        if (WiFi.status() == WL_CONNECTED)
        {
            wifiConnected = true;
            setLedOn();
            logSuccess("Подключено к WiFi: %s", config.ssid);
            logSystem("IP адрес: %s", WiFi.localIP().toString().c_str());
            logSystem("MAC адрес: %s", WiFi.macAddress().c_str());
            logSystem("Hostname: %s", hostname.c_str());
            logSystem("RSSI: %d dBm", WiFi.RSSI());
            // --- Первичная синхронизация времени NTP (блок до 5 сек) ---
            if (timeClient == nullptr)
            {
                extern WiFiUDP ntpUDP;
                timeClient = new NTPClient(ntpUDP, "pool.ntp.org", 0, 3600000);
                timeClient->begin();
            }
            if (timeClient)
            {
                unsigned long ntpStart = millis();
                while (!timeClient->forceUpdate() && millis() - ntpStart < NTP_TIMEOUT_MS)
                {
                    delay(WIFI_MODE_DELAY);
                }
                logSystem("NTP синхронизация: %s", timeClient->isTimeSet() ? "OK" : "не удалось");
            }

            setupWebServer();
        }
        else
        {
            logError("Не удалось подключиться к WiFi после %d попыток", attempts);
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
        setLedBlink(LED_SLOW_BLINK_INTERVAL);
        return false;
    }
    else if (isPressed && wasPressed)
    {
        if (millis() - pressStart >= RESET_BUTTON_HOLD_TIME)
        {
            return true;
        }
    }
    return false;
}

void restartESP()
{
    logWarn("Перезагрузка ESP32...");
    delay(RESTART_DELAY_MS);
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
    if (currentWiFiMode == WiFiMode::STA && wifiConnected)
    {
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
    setupServiceRoutes();  // Сервис
    setupOtaRoutes();      // OTA (/updates, api)
    setupReportsRoutes();  // Отчёты тестирования (/api/reports/*, /reports)

    setupErrorHandlers();  // Обработчики ошибок (404, 500) - должны быть последними

    // ============================================================================
    // ЗАПУСК СЕРВЕРА
    // ============================================================================

    webServer.begin();
    logSuccess("🏗️ Модульный веб-сервер v2.4.5 запущен. Режим: %s", currentWiFiMode == WiFiMode::AP ? "AP" : "STA");
    logSystem("✅ Активные модули: main, data, config, service, ota, error_handlers");
    logSystem("📋 Полный набор маршрутов готов к использованию");
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
    html += generateButton(ButtonType::PRIMARY, UI_ICON_SAVE, "Сохранить настройки", "") + "</form>";

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
