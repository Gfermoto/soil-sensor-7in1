/**
 * @file wifi_manager.cpp
 * @brief Управление WiFi, веб-интерфейсом и индикацией
 * @details Реализация логики подключения к WiFi, работы в режимах AP/STA, веб-конфигурирования, управления светодиодом
 * и сервисных функций.
 */
#include "wifi_manager.h"
#include <NTPClient.h>
#include <array>
#include "jxct_config_vars.h"
#include "jxct_constants.h"
#include "jxct_device_info.h"
#include "jxct_format_utils.h"
#include "jxct_ui_system.h"  // 🎨 Единая система дизайна v2.3.1
#include "logger.h"
#include "modbus_sensor.h"
#include "mqtt_client.h"
#include "thingspeak_client.h"
#include "web/csrf_protection.h"
#include "web_routes.h"           // 🏗️ Модульная архитектура v2.4.5

// Константы
enum class WifiConstants : std::uint16_t  // NOLINT(performance-enum-size)
{
    RESET_BUTTON_PIN = 0,             // GPIO0 для кнопки сброса
    WIFI_RECONNECT_INTERVAL = 30000,  // Интервал между попытками переподключения (30 секунд)
    LED_FAST_BLINK_INTERVAL = 100,    // Интервал быстрого мигания светодиода (мс)
    LED_SLOW_BLINK_INTERVAL = 500,    // Интервал медленного мигания светодиода (мс)
    WIFI_MODE_DELAY = 100,            // Задержка при смене режима WiFi (мс)
    NTP_TIMEOUT_MS = 5000,            // Таймаут для NTP синхронизации (мс)
    RESET_BUTTON_HOLD_TIME = 5000,    // Время удержания кнопки сброса (мс)
    RESTART_DELAY_MS = 1000,          // Задержка перед перезагрузкой (мс)
    DNS_SERVER_PORT = 53,             // Порт DNS сервера
    MAC_ADDRESS_BUFFER_SIZE = 20      // Размер буфера для MAC адреса
};

// Глобальные переменные
bool wifiConnected = false;
WiFiMode currentWiFiMode = WiFiMode::AP;
WebServer webServer(DEFAULT_WEB_SERVER_PORT);  // Используем константу из jxct_constants.h

namespace
{
DNSServer dnsServer;
unsigned long ledLastToggle = 0;
bool ledState = false;
unsigned long ledBlinkInterval = 0;
bool ledFastBlink = false;
}  // namespace

extern NTPClient* timeClient;
extern WiFiUDP ntpUDP;

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
    ledBlinkInterval = static_cast<unsigned long>(WifiConstants::LED_FAST_BLINK_INTERVAL);
    ledFastBlink = true;
}

void updateLed()
{
    if (ledBlinkInterval > 0)
    {
        const unsigned long now = millis();
        if (now - ledLastToggle >= ledBlinkInterval)
        {
            ledLastToggle = now;
            ledState = !ledState;
            digitalWrite(STATUS_LED_PIN, ledState ? HIGH : LOW);
        }
    }
}

// HTML для навигации
String navHtml()  // NOLINT(misc-use-internal-linkage)
{
    String html = "<div class='nav'>";
    html += "<a href='/'>" UI_ICON_CONFIG " Настройки</a>";
    if (currentWiFiMode == WiFiMode::STA)
    {
        html += "<a href='/readings'>" UI_ICON_DATA " Данные</a>";
        html += "<a href='/calibration'>" UI_ICON_CALIBRATION " Калибр.</a>";
        html += "<a href='/intervals'>" UI_ICON_INTERVALS " Интерв.</a>";  // v2.3.0
        html += "<a href='/config_manager'>" UI_ICON_FOLDER " Конфиг</a>";  // v2.3.0
        html += "<a href='/updates'>🚀 ОТА</a>";
        html += "<a href='/service'>" UI_ICON_SERVICE " Сервис</a>";
    }
    html += "</div>";
    return html;
}

void setupWiFi()
{
    logPrintHeader("ИНИЦИАЛИЗАЦИЯ WiFi", LogColor::GREEN);

    pinMode(STATUS_LED_PIN, OUTPUT);
    setLedBlink(static_cast<unsigned long>(WifiConstants::LED_SLOW_BLINK_INTERVAL));

    // Сначала отключаем WiFi и очищаем настройки
    WiFi.disconnect(true);  // NOLINT(readability-static-accessed-through-instance)
    WiFi.mode(WIFI_OFF);    // NOLINT(readability-static-accessed-through-instance)
    delay(static_cast<unsigned long>(WifiConstants::WIFI_MODE_DELAY));

    loadConfig();

    logSystemSafe("\1", config.ssid);
    logDebugSafe("\1", strlen(config.password) > 0 ? "задан" : "не задан");

    if (strlen(config.ssid) > 0 && strlen(config.password) > 0)
    {
        logWiFi("Переход в режим STA (клиент)");
        startSTAMode();
        logPrintSeparator("─", DEFAULT_SEPARATOR_LENGTH);
        return;
    }

    logWiFi("Переход в режим AP (точка доступа)");
    startAPMode();
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
        if (WiFi.softAPgetStationNum() ==
                0 &&  // никого не подключено // NOLINT(readability-static-accessed-through-instance)
            millis() - lastStaRetry >=
                static_cast<unsigned long>(WifiConstants::WIFI_RECONNECT_INTERVAL) &&  // прошло ≥ интервала
            strlen(config.ssid) > 0 &&
            strlen(config.password) > 0)  // есть сохранённые уч. данные
        {
            lastStaRetry = millis();
            logWiFiSafe("AP режим: пробуем снова подключиться к WiFi \"%s\"", config.ssid);
            startSTAMode();  // если не получится, функция сама вернёт нас в AP
            return;          // ждём следующего цикла
        }

        if (WiFi.softAPgetStationNum() > 0)  // NOLINT(readability-static-accessed-through-instance)
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
        constexpr int MAX_RECONNECT_ATTEMPTS =
            3;  // Максимальное количество попыток переподключения перед переходом в AP

        if (WiFi.status() != WL_CONNECTED)  // NOLINT(readability-static-accessed-through-instance)
        {
            if (!wifiConnected ||
                (millis() - lastReconnectAttempt >= static_cast<unsigned long>(WifiConstants::WIFI_RECONNECT_INTERVAL)))
            {
                wifiConnected = false;
                setLedBlink(WIFI_RETRY_DELAY_MS);

                if (reconnectAttempts < MAX_RECONNECT_ATTEMPTS)
                {
                    logWarnSafe("\1", reconnectAttempts + 1, MAX_RECONNECT_ATTEMPTS);

                    WiFi.disconnect(true);  // NOLINT(readability-static-accessed-through-instance)
                    delay(static_cast<unsigned long>(WifiConstants::WIFI_MODE_DELAY));
                    WiFi.begin(config.ssid, config.password);  // NOLINT(readability-static-accessed-through-instance)

                    lastReconnectAttempt = millis();
                    reconnectAttempts++;
                }
                else
                {
                    logErrorSafe("\1", MAX_RECONNECT_ATTEMPTS);
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
                logSuccessSafe(
                    "\1", WiFi.localIP().toString().c_str());    // NOLINT(readability-static-accessed-through-instance)
                logSystemSafe("\1", WiFi.macAddress().c_str());  // NOLINT(readability-static-accessed-through-instance)
                logSystemSafe("\1", config.ssid);
                logSystemSafe("\1", WiFi.RSSI());  // NOLINT(readability-static-accessed-through-instance)
                // --- Первичная синхронизация времени NTP (блок до 5 сек) ---
                if (timeClient == nullptr)
                {
                    timeClient = new NTPClient(ntpUDP, "pool.ntp.org", 0, 3600000);
                    timeClient->begin();
                }
                if (timeClient != nullptr)
                {
                    const unsigned long ntpStart = millis();
                    while (!timeClient->forceUpdate() &&
                           millis() - ntpStart < static_cast<unsigned long>(WifiConstants::NTP_TIMEOUT_MS))
                    {
                        delay(static_cast<unsigned long>(WifiConstants::WIFI_MODE_DELAY));
                    }
                    logSystemSafe("\1", timeClient->isTimeSet() ? "OK" : "не удалось");
                }

                setupWebServer();
                return;
            }
        }
        webServer.handleClient();
    }
}

String getApSsid()  // NOLINT(misc-use-internal-linkage)
{
    std::array<uint8_t, 6> mac;
    WiFi.macAddress(mac.data());  // NOLINT(readability-static-accessed-through-instance)
    std::array<char, static_cast<size_t>(WifiConstants::MAC_ADDRESS_BUFFER_SIZE)> buf;
    snprintf(buf.data(), buf.size(), "jxct-%02X%02X%02X", mac[3], mac[4], mac[5]);
    for (int charIndex = 0; buf[charIndex]; ++charIndex)
    {
        buf[charIndex] = tolower(buf[charIndex]);
    }
    return String(buf.data());
}

void startAPMode()
{
    currentWiFiMode = WiFiMode::AP;
    WiFi.disconnect();   // NOLINT(readability-static-accessed-through-instance)
    WiFi.mode(WIFI_AP);  // NOLINT(readability-static-accessed-through-instance)
    const String apSsid = getApSsid();
    WiFi.softAP(apSsid.c_str(), JXCT_WIFI_AP_PASS);  // NOLINT(readability-static-accessed-through-instance)
    dnsServer.start(static_cast<uint16_t>(WifiConstants::DNS_SERVER_PORT), "*",
                    WiFi.softAPIP());  // NOLINT(readability-static-accessed-through-instance)
    setupWebServer();
    setLedBlink(static_cast<unsigned long>(WifiConstants::LED_SLOW_BLINK_INTERVAL));
    logWiFi("Режим точки доступа запущен");
    logSystemSafe("\1", apSsid.c_str());
    logSystemSafe("\1", WiFi.softAPIP().toString().c_str());  // NOLINT(readability-static-accessed-through-instance)
}

void startSTAMode()
{
    currentWiFiMode = WiFiMode::STA;
    WiFi.disconnect(
        true);  // Полное отключение с очисткой настроек // NOLINT(readability-static-accessed-through-instance)
    WiFi.mode(WIFI_STA);  // NOLINT(readability-static-accessed-through-instance)
    delay(static_cast<unsigned long>(WifiConstants::WIFI_MODE_DELAY));  // Даем время на применение режима

    const String hostname = getApSsid();
    WiFi.setHostname(hostname.c_str());  // NOLINT(readability-static-accessed-through-instance)

    if (strlen(config.ssid) > 0)
    {
        logWiFi("Подключение к WiFi...");
        WiFi.begin(
            config.ssid,
            config.password);  // Явно вызываем подключение // NOLINT(readability-static-accessed-through-instance)

        int attempts = 0;
        setLedBlink(WIFI_RETRY_DELAY_MS);
        const unsigned long startTime = millis();

        while (WiFi.status() != WL_CONNECTED &&
               attempts < WIFI_CONNECTION_ATTEMPTS &&  // NOLINT(readability-static-accessed-through-instance)
               (millis() - startTime) < WIFI_CONNECTION_TIMEOUT)
        {
            delay(WIFI_RETRY_DELAY_MS);
            updateLed();
            attempts++;
            logDebugSafe("\1", attempts, WIFI_CONNECTION_ATTEMPTS);

            // Проверяем кнопку сброса во время подключения
            if (checkResetButton())
            {
                logWarn("Обнаружено длительное нажатие кнопки во время подключения");
                startAPMode();
                return;
            }
        }

        if (WiFi.status() == WL_CONNECTED)  // NOLINT(readability-static-accessed-through-instance)
        {
            wifiConnected = true;
            setLedOn();
            logSuccessSafe("\1", config.ssid);
            logSystemSafe("\1",
                          WiFi.localIP().toString().c_str());  // NOLINT(readability-static-accessed-through-instance)
            logSystemSafe("\1", WiFi.macAddress().c_str());    // NOLINT(readability-static-accessed-through-instance)
            logSystemSafe("\1", hostname.c_str());
            logSystemSafe("\1", WiFi.RSSI());  // NOLINT(readability-static-accessed-through-instance)
            // --- Первичная синхронизация времени NTP (блок до 5 сек) ---
            if (timeClient == nullptr)
            {
                timeClient = new NTPClient(ntpUDP, "pool.ntp.org", 0, 3600000);
                timeClient->begin();
            }
            if (timeClient != nullptr)
            {
                const unsigned long ntpStart = millis();
                while (!timeClient->forceUpdate() &&
                       millis() - ntpStart < static_cast<unsigned long>(WifiConstants::NTP_TIMEOUT_MS))
                {
                    delay(static_cast<unsigned long>(WifiConstants::WIFI_MODE_DELAY));
                }
                logSystemSafe("\1", timeClient->isTimeSet() ? "OK" : "не удалось");
            }

            setupWebServer();
            return;
        }

        logErrorSafe("\1", attempts);
        startAPMode();
        return;
    }

    logWarn("SSID не задан, переход в AP");
    startAPMode();
}

bool checkResetButton()
{
    static unsigned long pressStart = 0;
    static bool wasPressed = false;
    const bool isPressed = digitalRead(static_cast<uint8_t>(WifiConstants::RESET_BUTTON_PIN)) == LOW;
    if (isPressed && !wasPressed)
    {
        pressStart = millis();
        wasPressed = true;
        setLedFastBlink();
        return false;
    }

    if (!isPressed && wasPressed)
    {
        wasPressed = false;
        setLedBlink(static_cast<unsigned long>(WifiConstants::LED_SLOW_BLINK_INTERVAL));
        return false;
    }

    if (isPressed && wasPressed)
    {
        if (millis() - pressStart >= static_cast<unsigned long>(WifiConstants::RESET_BUTTON_HOLD_TIME))
        {
            return true;
        }
    }
    return false;
}

void restartESP()
{
    logWarn("Перезагрузка ESP32...");
    delay(static_cast<unsigned long>(WifiConstants::RESTART_DELAY_MS));
    ESP.restart();
}

void handleStatus()  // NOLINT(misc-use-internal-linkage)
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
        html +=
            "<li>IP: " + WiFi.localIP().toString() + "</li>";  // NOLINT(readability-static-accessed-through-instance)
        html +=
            "<li>RSSI: " + String(WiFi.RSSI()) + " dBm</li>";  // NOLINT(readability-static-accessed-through-instance)
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
    logSuccessSafe("\1", currentWiFiMode == WiFiMode::AP ? "AP" : "STA");
    logSystem("Активные модули: main, data, config, service, ota, error_handlers");
    logSystem("📋 Полный набор маршрутов готов к использованию");
}
