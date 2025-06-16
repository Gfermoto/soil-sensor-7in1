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
#include "calibration_manager.h"
#include <ArduinoJson.h>
#include <NTPClient.h>
#include <LittleFS.h>

extern NTPClient* timeClient;

// Объявления внешних функций
extern String navHtml();
extern String formatValue(float value, const char* unit, int precision);
extern String getApSsid();

// Буфер для загрузки файлов (калибровка через /readings)
static File uploadFile;
static SoilProfile uploadProfile = SoilProfile::SAND;

static void handleReadingsUpload()
{
    HTTPUpload& upload = webServer.upload();
    if (upload.status == UPLOAD_FILE_START)
    {
        String profileStr = webServer.arg("soil_profile");
        if (profileStr == "sand") uploadProfile = SoilProfile::SAND;
        else if (profileStr == "loam") uploadProfile = SoilProfile::LOAM;
        else if (profileStr == "peat") uploadProfile = SoilProfile::PEAT;

        CalibrationManager::init();
        const char* path = CalibrationManager::profileToFilename(uploadProfile);
        uploadFile = LittleFS.open(path, "w");
        if (!uploadFile)
        {
            logError("Не удалось создать файл %s", path);
        }
    }
    else if (upload.status == UPLOAD_FILE_WRITE)
    {
        if (uploadFile)
        {
            uploadFile.write(upload.buf, upload.currentSize);
        }
    }
    else if (upload.status == UPLOAD_FILE_END)
    {
        if (uploadFile)
        {
            uploadFile.close();
            logSuccess("Файл калибровки загружен (%u байт)", upload.totalSize);
        }
        webServer.sendHeader("Location", "/readings?toast=Калибровка+загружена", true);
        webServer.send(302, "text/plain", "Redirect");
    }
}

static void handleProfileSave()
{
    if (webServer.hasArg("soil_profile"))
    {
        String profileStr = webServer.arg("soil_profile");
        if (profileStr == "sand") config.soilProfile = 0;
        else if (profileStr == "loam") config.soilProfile = 1;
        else if (profileStr == "peat") config.soilProfile = 2;

        saveConfig();
        logSuccess("Профиль почвы изменён на %s", profileStr.c_str());
    }
    webServer.sendHeader("Location", "/readings?toast=Профиль+сохранен", true);
    webServer.send(302, "text/plain", "Redirect");
}

void setupDataRoutes()
{
    // Красивая страница показаний с иконками (оригинальный дизайн)
    webServer.on("/readings", HTTP_GET,
                 []()
                 {
                     logWebRequest("GET", "/readings", webServer.client().remoteIP().toString());

                     if (currentWiFiMode == WiFiMode::AP)
                     {
                         String html = "<!DOCTYPE html><html><head><meta charset='UTF-8'>";
                         html += "<title>" UI_ICON_DATA " Показания</title>";
                         html += "<style>" + String(getUnifiedCSS()) + "</style></head><body><div class='container'>";
                         html += "<h1>" UI_ICON_DATA " Показания</h1>";
                         html += "<div class='msg msg-error'>" UI_ICON_ERROR
                                 " Недоступно в режиме точки доступа</div></div></body></html>";
                         webServer.send(200, "text/html; charset=utf-8", html);
                         return;
                     }

                     String html =
                         "<!DOCTYPE html><html><head><meta charset='UTF-8'><meta name='viewport' "
                         "content='width=device-width, initial-scale=1.0'>";
                     html += "<title>" UI_ICON_DATA " Показания датчика JXCT</title>";
                     html += "<style>" + String(getUnifiedCSS()) + "</style></head><body><div class='container'>";
                     html += navHtml();
                     html += "<h1>" UI_ICON_DATA " Показания датчика</h1>";
                     // Таблица данных "После / До"
                     html += "<div class='section'><table class='data'><thead><tr><th></th><th>После</th><th>До</th></tr></thead><tbody>";
                     html += "<tr><td>🌡️ Температура, °C</td><td><span id='temp'></span></td><td><span id='temp_raw'></span></td></tr>";
                     html += "<tr><td>💧 Влажность, %</td><td><span id='hum'></span></td><td><span id='hum_raw'></span></td></tr>";
                     html += "<tr><td>⚡ EC, µS/cm</td><td><span id='ec'></span></td><td><span id='ec_raw'></span></td></tr>";
                     html += "<tr><td>⚗️ pH</td><td><span id='ph'></span></td><td><span id='ph_raw'></span></td></tr>";
                     html += "<tr><td>🔴 Азот (N), мг/кг</td><td><span id='n'></span></td><td><span id='n_raw'></span></td></tr>";
                     html += "<tr><td>🟡 Фосфор (P), мг/кг</td><td><span id='p'></span></td><td><span id='p_raw'></span></td></tr>";
                     html += "<tr><td>🔵 Калий (K), мг/кг</td><td><span id='k'></span></td><td><span id='k_raw'></span></td></tr>";
                     html += "</tbody></table></div>";
                     html +=
                         "<div style='margin-top:15px;font-size:14px;color:#555'><b>API:</b> <a href='/api/sensor' "
                         "target='_blank'>/api/sensor</a> (JSON, +timestamp)</div>";
                     html += "<script>";
                     html += "function set(id,v){if(v!==undefined&&v!==null){document.getElementById(id).textContent=v;}}";
                     html += "function updateSensor(){";
                     html += "fetch('/sensor_json').then(r=>r.json()).then(d=>{";
                     html += "set('temp',d.temperature);";
                     html += "set('hum',d.humidity);";
                     html += "set('ec',d.ec);";
                     html += "set('ph',d.ph);";
                     html += "set('n',d.nitrogen);";
                     html += "set('p',d.phosphorus);";
                     html += "set('k',d.potassium);";
                     html += "set('temp_raw',d.raw_temperature);";
                     html += "set('hum_raw',d.raw_humidity);";
                     html += "set('ec_raw',d.raw_ec);";
                     html += "set('ph_raw',d.raw_ph);";
                     html += "set('n_raw',d.raw_nitrogen);";
                     html += "set('p_raw',d.raw_phosphorus);";
                     html += "set('k_raw',d.raw_potassium);";
                     html += "});";
                     html += "}";
                     html += "setInterval(updateSensor,3000);";
                     html += "updateSensor();";
                     html += "</script>";

                     // ======= Калибровка =======
                     html += "<div class='section'><h2>⚙️ Калибровка</h2>";
                     // ----- Форма выбора профиля -----
                     html += "<form action='/readings/profile' method='post'>";
                     html += "<div class='section'><h3>Профиль почвы</h3>";
                     html += "<select name='soil_profile' id='soil_sel'>";
                     html += "<option value='sand'" + String(config.soilProfile==0?" selected":"") + ">Песок</option>";
                     html += "<option value='loam'" + String(config.soilProfile==1?" selected":"") + ">Суглинок</option>";
                     html += "<option value='peat'" + String(config.soilProfile==2?" selected":"") + ">Торф</option>";
                     html += "</select></div>";
                     html += generateButton(ButtonType::SECONDARY, UI_ICON_SAVE, "Сохранить профиль", "");
                     html += "</form>";

                     // ----- Форма загрузки CSV -----
                     html += "<form action='/readings/upload' method='post' enctype='multipart/form-data' style='margin-top:10px'>";
                     html += "<input type='hidden' name='soil_profile' id='upload_hidden' value='sand'>";
                     html += "<div class='section'><h3>Загрузить CSV</h3><input type='file' name='calibration_csv' accept='.csv' required></div>";
                     html += generateButton(ButtonType::PRIMARY, UI_ICON_UPLOAD, "Загрузить CSV", "");
                     html += "</form>";

                     // JS синхронизации селекта с hidden upload
                     html += "<script>document.getElementById('soil_sel').addEventListener('change',e=>{document.getElementById('upload_hidden').value=e.target.value;});</script>";

                     // CSS для таблицы данных
                     html += "<style>.data{width:100%;border-collapse:collapse}.data th,.data td{border:1px solid #ccc;padding:6px;text-align:center}.data th{background:#f5f5f5}</style>";

                     html += "</div>" + String(getToastHTML()) + "</body></html>";
                     webServer.send(200, "text/html; charset=utf-8", html);
                 });

    // AJAX эндпоинт для обновления показаний
    webServer.on("/sensor_json", HTTP_GET,
                 []()
                 {
                     logWebRequest("GET", "/sensor_json", webServer.client().remoteIP().toString());

                     if (currentWiFiMode != WiFiMode::STA)
                     {
                         webServer.send(403, "application/json", "{\"error\":\"AP mode\"}");
                         return;
                     }

                     StaticJsonDocument<384> doc;
                     doc["temperature"] = format_temperature(sensorData.temperature);
                     doc["humidity"] = format_moisture(sensorData.humidity);
                     doc["ec"] = format_ec(sensorData.ec);
                     doc["ph"] = format_ph(sensorData.ph);
                     doc["nitrogen"] = format_npk(sensorData.nitrogen);
                     doc["phosphorus"] = format_npk(sensorData.phosphorus);
                     doc["potassium"] = format_npk(sensorData.potassium);
                     doc["raw_temperature"] = format_temperature(sensorData.raw_temperature);
                     doc["raw_humidity"] = format_moisture(sensorData.raw_humidity);
                     doc["raw_ec"] = format_ec(sensorData.raw_ec);
                     doc["raw_ph"] = format_ph(sensorData.raw_ph);
                     doc["raw_nitrogen"] = format_npk(sensorData.raw_nitrogen);
                     doc["raw_phosphorus"] = format_npk(sensorData.raw_phosphorus);
                     doc["raw_potassium"] = format_npk(sensorData.raw_potassium);
                     doc["timestamp"] = (long)(timeClient ? timeClient->getEpochTime() : 0);

                     String json;
                     serializeJson(doc, json);
                     webServer.send(200, "application/json", json);
                 });

    // API эндпоинт для интеграции
    webServer.on("/api/sensor", HTTP_GET,
                 []()
                 {
                     logWebRequest("GET", "/api/sensor", webServer.client().remoteIP().toString());

                     if (currentWiFiMode != WiFiMode::STA)
                     {
                         webServer.send(403, "application/json", "{\"error\":\"AP mode\"}");
                         return;
                     }

                     StaticJsonDocument<384> doc;
                     doc["temperature"] = format_temperature(sensorData.temperature);
                     doc["humidity"] = format_moisture(sensorData.humidity);
                     doc["ec"] = format_ec(sensorData.ec);
                     doc["ph"] = format_ph(sensorData.ph);
                     doc["nitrogen"] = format_npk(sensorData.nitrogen);
                     doc["phosphorus"] = format_npk(sensorData.phosphorus);
                     doc["potassium"] = format_npk(sensorData.potassium);
                     doc["raw_temperature"] = format_temperature(sensorData.raw_temperature);
                     doc["raw_humidity"] = format_moisture(sensorData.raw_humidity);
                     doc["raw_ec"] = format_ec(sensorData.raw_ec);
                     doc["raw_ph"] = format_ph(sensorData.raw_ph);
                     doc["raw_nitrogen"] = format_npk(sensorData.raw_nitrogen);
                     doc["raw_phosphorus"] = format_npk(sensorData.raw_phosphorus);
                     doc["raw_potassium"] = format_npk(sensorData.raw_potassium);
                     doc["timestamp"] = (long)(timeClient ? timeClient->getEpochTime() : 0);

                     String json;
                     serializeJson(doc, json);
                     webServer.send(200, "application/json", json);
                 });

    // Загрузка калибровочного CSV через вкладку
    webServer.on("/readings/upload", HTTP_POST, [](){}, handleReadingsUpload);

    // Форма для сохранения профиля
    webServer.on("/readings/profile", HTTP_POST, [](){}, handleProfileSave);

    logDebug("Маршруты данных настроены: /readings, /sensor_json, /api/sensor");
}

// Вспомогательная функция для получения SSID точки доступа
extern String getApSsid();