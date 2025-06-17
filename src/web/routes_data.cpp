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
#include <time.h>

extern NTPClient* timeClient;

// Объявления внешних функций
extern String navHtml();
extern String formatValue(float value, const char* unit, int precision);
extern String getApSsid();

// Буфер для загрузки файлов (калибровка через /readings)
static File uploadFile;
static SoilProfile uploadProfile = SoilProfile::SAND;

struct RecValues { float t, hum, ec, ph, n, p, k; };

static RecValues computeRecommendations()
{
    // 1. База по культуре или generic
    RecValues rec{21,60,1200,6.3,30,10,25};
    const char* id=config.cropId;
    if(strlen(id)>0){
        if      (strcmp(id,"tomato")==0)      rec={22,60,1500,6.5,40,10,30};
        else if (strcmp(id,"cucumber")==0)    rec={24,70,1800,6.2,35,12,28};
        else if (strcmp(id,"pepper")==0)      rec={23,65,1600,6.3,38,11,29};
        else if (strcmp(id,"lettuce")==0)     rec={20,75,1000,6.0,30,8,25};
        else if (strcmp(id,"blueberry")==0)   rec={18,65,1200,5.0,30,10,20};
        else if (strcmp(id,"lawn")==0)        rec={20,50,800,6.3,25,8,20};
        else if (strcmp(id,"grape")==0)       rec={22,55,1400,6.5,35,12,25};
        else if (strcmp(id,"conifer")==0)     rec={18,55,1000,5.5,25,8,15};
        else if (strcmp(id,"strawberry")==0)  rec={20,70,1500,6.0,35,10,25};
        else if (strcmp(id,"apple")==0)       rec={18,60,1200,6.5,25,10,20};
        else if (strcmp(id,"pear")==0)        rec={18,60,1200,6.5,25,10,20};
        else if (strcmp(id,"cherry")==0)      rec={20,60,1300,6.5,30,10,25};
        else if (strcmp(id,"raspberry")==0)   rec={18,65,1100,6.2,30,10,22};
        else if (strcmp(id,"currant")==0)     rec={17,65,1000,6.2,25,9,20};
    }

    // 2. Коррекция по soilProfile (влажность и pH)
    int soil=config.soilProfile; // 0 sand,1 loam,2 peat
    if(soil==0){ rec.hum+=-5; /* песок */ }
    else if(soil==2){ rec.hum+=10; rec.ph-=0.3f; }
    else if(soil==1){ rec.hum+=5; }

    // 3. Коррекция по environmentType
    switch(config.environmentType){
        case 1: // greenhouse
            rec.hum+=10; rec.ec+=300; rec.n+=5; rec.k+=5; rec.t+=2; break;
        case 2: // indoor
            rec.hum+=-5; rec.ec-=200; rec.t+=1; break;
    }

    // 4. Сезонная коррекция (только если включена)
    if(config.flags.seasonalAdjustEnabled){
        time_t now=time(nullptr); struct tm* ti=localtime(&now);
        int m=ti?ti->tm_mon+1:1;
        bool rainy=(m==4||m==5||m==6||m==10);
        if(rainy){ rec.hum+=5; rec.ec-=100; }
        else{ rec.hum+=-2; rec.ec+=100; }
    }

    return rec;
}

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
                     // Индикатор полива + информационный блок
                     html += "<div id='irrigBadge' style='display:none;margin:10px 0;font-size:18px;color:#2196F3'>💦 Полив!</div>";
                     html += "<div id='statusInfo' style='margin:10px 0;font-size:16px;color:#333'></div>";
                     // Заголовок 4-го столбца: выбранная культура или «Реком.»
                     String recHeader = "Реком.";
                     if (strlen(config.cropId) > 0)
                     {
                         const char* id = config.cropId;
                         if      (strcmp(id,"tomato")==0)      recHeader = "Томаты";
                         else if (strcmp(id,"cucumber")==0)    recHeader = "Огурцы";
                         else if (strcmp(id,"pepper")==0)      recHeader = "Перец";
                         else if (strcmp(id,"lettuce")==0)     recHeader = "Салат";
                         else if (strcmp(id,"blueberry")==0)   recHeader = "Голубика";
                         else if (strcmp(id,"lawn")==0)        recHeader = "Газон";
                         else if (strcmp(id,"grape")==0)       recHeader = "Виноград";
                         else if (strcmp(id,"conifer")==0)     recHeader = "Хвойные";
                         else if (strcmp(id,"strawberry")==0)  recHeader = "Клубника";
                         else if (strcmp(id,"apple")==0)       recHeader = "Яблоня";
                         else if (strcmp(id,"pear")==0)        recHeader = "Груша";
                         else if (strcmp(id,"cherry")==0)      recHeader = "Вишня";
                         else if (strcmp(id,"raspberry")==0)   recHeader = "Малина";
                         else if (strcmp(id,"currant")==0)     recHeader = "Смородина";
                     }

                     html += "<div class='section'><table class='data'><thead><tr><th></th><th>RAW</th><th>Компенс.</th><th>" + recHeader + "</th></tr></thead><tbody>";
                     html += "<tr><td>🌡️ Температура, °C</td><td><span id='temp_raw'></span></td><td><span id='temp'></span></td><td><span id='temp_rec'></span></td></tr>";
                     html += "<tr><td>💧 Влажность, %</td><td><span id='hum_raw'></span></td><td><span id='hum'></span></td><td><span id='hum_rec'></span></td></tr>";
                     html += "<tr><td>⚡ EC, µS/cm</td><td><span id='ec_raw'></span></td><td><span id='ec'></span></td><td><span id='ec_rec'></span></td></tr>";
                     html += "<tr><td>⚗️ pH</td><td><span id='ph_raw'></span></td><td><span id='ph'></span></td><td><span id='ph_rec'></span></td></tr>";
                     html += "<tr><td>🔴 Азот (N), мг/кг</td><td><span id='n_raw'></span></td><td><span id='n'></span></td><td><span id='n_rec'></span></td></tr>";
                     html += "<tr><td>🟡 Фосфор (P), мг/кг</td><td><span id='p_raw'></span></td><td><span id='p'></span></td><td><span id='p_rec'></span></td></tr>";
                     html += "<tr><td>🔵 Калий (K), мг/кг</td><td><span id='k_raw'></span></td><td><span id='k'></span></td><td><span id='k_rec'></span></td></tr>";
                     html += "</tbody></table></div>";
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
                     html += "set('temp_rec',d.rec_temperature);set('hum_rec',d.rec_humidity);set('ec_rec',d.rec_ec);set('ph_rec',d.rec_ph);set('n_rec',d.rec_nitrogen);set('p_rec',d.rec_phosphorus);set('k_rec',d.rec_potassium);";
                     html += "document.getElementById('irrigBadge').style.display = d.irrigation ? 'block' : 'none';";
                     html += "document.getElementById('statusInfo').textContent = 'Полив: ' + (d.irrigation ? 'Да' : 'Нет') + ' | Сезон: ' + d.season + (d.alerts ? (' | Отклонения: ' + d.alerts) : ' | Датчик в диапазоне измерений');";
                     html += "});";
                     html += "}";
                     html += "setInterval(updateSensor,3000);";
                     html += "updateSensor();";
                     html += "</script>";

                     // ======= Калибровка =======
                     html += "<div class='section'><h2>⚙️ Калибровка</h2>";
                     // ----- Форма загрузки CSV -----
                     html += "<form action='/readings/upload' method='post' enctype='multipart/form-data' style='margin-top:10px'>";
                     html += "<div class='section'><h3>Загрузить CSV</h3><input type='file' name='calibration_csv' accept='.csv' required></div>";
                     html += generateButton(ButtonType::PRIMARY, UI_ICON_UPLOAD, "Загрузить CSV", "");
                     html += "</form>";

                     // CSS для таблицы данных
                     html += "<style>.data{width:100%;border-collapse:collapse}.data th,.data td{border:1px solid #ccc;padding:6px;text-align:center}.data th{background:#f5f5f5}</style>";

                     // API-ссылка внизу страницы
                     html += "<div style='margin-top:15px;font-size:14px;color:#555'><b>API:</b> <a href='/api/sensor' target='_blank'>/api/sensor</a> (JSON, +timestamp)</div>";
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

                     StaticJsonDocument<512> doc;
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
                     doc["irrigation"] = sensorData.recentIrrigation;

                     RecValues rec = computeRecommendations();
                     doc["rec_temperature"] = format_temperature(rec.t);
                     doc["rec_humidity"] = format_moisture(rec.hum);
                     doc["rec_ec"] = format_ec(rec.ec);
                     doc["rec_ph"] = format_ph(rec.ph);
                     doc["rec_nitrogen"] = format_npk(rec.n);
                     doc["rec_phosphorus"] = format_npk(rec.p);
                     doc["rec_potassium"] = format_npk(rec.k);

                     // ---- Дополнительная информация ----
                     // Сезон по текущему месяцу
                     const char* seasonName = [](){
                         time_t now = timeClient ? (time_t)timeClient->getEpochTime() : time(nullptr);
                         struct tm* ti = localtime(&now);
                         uint8_t m = ti ? (ti->tm_mon + 1) : 1;
                         if (m==12 || m==1 || m==2) return "Зима";
                         if (m>=3 && m<=5)           return "Весна";
                         if (m>=6 && m<=8)           return "Лето";
                         return "Осень";
                     }();
                     doc["season"] = seasonName;

                     // Проверяем отклонения
                     String alerts="";
                     auto append=[&](const char* n){ if(alerts.length()) alerts += ", "; alerts += n; };
                     // Физические пределы датчика
                     if (sensorData.temperature < -45 || sensorData.temperature > 115) append("T");
                     if (sensorData.humidity    <   0 || sensorData.humidity    > 100) append("θ");
                     if (sensorData.ec          <   0 || sensorData.ec          > 10000) append("EC");
                     if (sensorData.ph          <   3 || sensorData.ph          > 9) append("pH");
                     if (sensorData.nitrogen    <   0 || sensorData.nitrogen    > 1999) append("N");
                     if (sensorData.phosphorus  <   0 || sensorData.phosphorus  > 1999) append("P");
                     if (sensorData.potassium   <   0 || sensorData.potassium   > 1999) append("K");
                     doc["alerts"] = alerts;

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