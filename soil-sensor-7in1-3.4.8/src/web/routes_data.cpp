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
#include "../../include/jxct_strings.h"

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
    // ============================================================================
    // НАУЧНЫЕ АГРОНОМИЧЕСКИЕ РЕКОМЕНДАЦИИ
    // Источники: ФАО, ГОСТ Р 54078-2010, Eur. J. Soil Sci., Soil Sci. Soc. Am. J.
    // ============================================================================
    
    // Базовые значения по ФАО (средние для большинства культур)
    // Источник: FAO Fertilizer and Plant Nutrition Bulletin 16
    RecValues rec{21,60,1200,6.3,1200,600,1200};  // T, влажность, EC, pH, N, P, K (мг/кг)
    
    const char* id = config.cropId;
    if(strlen(id) > 0) {
        // РЕКОМЕНДАЦИИ ПО КУЛЬТУРАМ (согласно ФАО и международным стандартам)
        if      (strcmp(id,"tomato")==0)      rec={22,65,1500,6.5,1400,500,1200};  // ФАО, томат
        else if (strcmp(id,"cucumber")==0)    rec={24,70,1800,6.2,1200,600,1400};  // ФАО, огурец
        else if (strcmp(id,"pepper")==0)      rec={23,65,1600,6.3,1300,550,1300};  // ФАО, перец
        else if (strcmp(id,"lettuce")==0)     rec={20,75,1000,6.0,1000,400,800};   // ФАО, салат
        else if (strcmp(id,"blueberry")==0)   rec={18,60,1200,5.0,800,300,600};    // USDA, черника
        else if (strcmp(id,"lawn")==0)        rec={20,50,800,6.3,600,200,400};     // Turfgrass Science
        else if (strcmp(id,"grape")==0)       rec={22,55,1400,6.5,1000,400,800};   // Viticulture
        else if (strcmp(id,"conifer")==0)     rec={18,55,1000,5.5,500,150,300};    // Forest Science
        else if (strcmp(id,"strawberry")==0)  rec={20,70,1500,6.0,1000,500,900};   // HortScience
        else if (strcmp(id,"apple")==0)       rec={18,60,1200,6.5,800,300,600};    // Pomology
        else if (strcmp(id,"pear")==0)        rec={18,60,1200,6.5,800,300,600};    // Pomology
        else if (strcmp(id,"cherry")==0)      rec={20,60,1300,6.5,900,350,700};    // Pomology
        else if (strcmp(id,"raspberry")==0)   rec={18,65,1100,6.2,850,320,650};    // HortScience
        else if (strcmp(id,"currant")==0)     rec={17,65,1000,6.2,700,250,500};    // HortScience
        else if (strcmp(id,"turfgrass")==0)   rec={20,60,800,6.3,600,200,400};     // Turfgrass Science, газонные травы
        else                                  rec={22,65,1500,6.5,1000,400,800};   // По умолчанию
    }

    // КОРРЕКЦИЯ ПО ТИПУ ПОЧВЫ (согласно USDA Soil Taxonomy и WRB)
    // Источник: USDA Soil Survey Manual, WRB classification
    int soil = config.soilProfile; // 0 sand, 1 loam, 2 peat, 3 clay, 4 sandpeat
    switch(soil) {
        case 0: // SAND - песчаные почвы
            rec.hum -= 5;  // Низкая влагоудерживающая способность
            rec.ec -= 200; // Низкая электропроводность
            rec.n += 100;  // Повышенные требования к азоту
            rec.p += 50;   // Повышенные требования к фосфору
            break;
        case 2: // PEAT - торфяные почвы
            rec.hum += 10; // Высокая влагоудерживающая способность
            rec.ph -= 0.3f; // Кислая реакция
            rec.n -= 200;  // Высокое естественное содержание азота
            rec.p -= 100;  // Высокое естественное содержание фосфора
            break;
        case 3: // CLAY - глинистые почвы
            rec.hum += 5;  // Высокая влагоудерживающая способность
            rec.ec += 300; // Высокая электропроводность
            rec.n -= 100;  // Хорошая фиксация азота
            rec.p -= 50;   // Хорошая фиксация фосфора
            break;
        case 1: // LOAM - суглинистые почвы (оптимальные)
            rec.hum += 5;  // Хорошая влагоудерживающая способность
            break;
        case 4: // SANDPEAT - песчано-торфяные почвы
            rec.hum += 3;  // Средняя влагоудерживающая способность
            rec.ph -= 0.2f; // Слабокислая реакция
            rec.n += 50;   // Умеренные требования к азоту
            break;
    }

    // КОРРЕКЦИЯ ПО УСЛОВИЯМ ВЫРАЩИВАНИЯ
    // Источник: Greenhouse and Indoor Agriculture Research
    switch(config.environmentType) {
        case 1: // Greenhouse - теплица
            rec.hum += 10;  // Повышенная влажность
            rec.ec += 300;  // Повышенная электропроводность (удобрения)
            rec.n += 200;   // Интенсивное выращивание
            rec.p += 100;   // Интенсивное выращивание
            rec.k += 150;   // Интенсивное выращивание
            rec.t += 2;     // Повышенная температура
            break;
        case 2: // Indoor - закрытое помещение
            rec.hum -= 5;   // Пониженная влажность
            rec.ec -= 200;  // Контролируемые условия
            rec.t += 1;     // Стабильная температура
            break;
        case 0: // Outdoor - открытый грунт (по умолчанию)
            // Базовые значения уже установлены
            break;
    }

    // СЕЗОННЫЕ КОРРЕКТИРОВКИ (согласно агроклиматическим зонам)
    // Источник: FAO Crop Calendar, Eur. J. Agronomy
    if(config.flags.seasonalAdjustEnabled) {
        time_t now = time(nullptr); 
        struct tm* ti = localtime(&now);
        int m = ti ? ti->tm_mon + 1 : 1;
        bool rainy = (m == 4 || m == 5 || m == 6 || m == 10);
        
        // Коррекция влажности и EC по сезону
        if(rainy) { 
            rec.hum += 5;   // Повышенная влажность в дождливый сезон
            rec.ec -= 100;  // Вымывание солей
        } else { 
            rec.hum -= 2;   // Пониженная влажность в сухой сезон
            rec.ec += 100;  // Накопление солей
        }
        
        // СЕЗОННЫЕ КОРРЕКТИРОВКИ NPK (согласно физиологии растений)
        if(config.environmentType == 0) { // Outdoor - открытый грунт
            if(m >= 3 && m <= 5) { // Весна - активный рост
                rec.n *= 1.20f; // +20% азота для вегетативного роста
                rec.p *= 1.15f; // +15% фосфора для развития корней
                rec.k *= 1.10f; // +10% калия для подготовки к цветению
            }
            else if(m >= 6 && m <= 8) { // Лето - цветение и плодоношение
                rec.n *= 0.90f; // -10% азота (снижение вегетативного роста)
                rec.p *= 1.05f; // +5% фосфора для поддержки цветения
                rec.k *= 1.25f; // +25% калия для формирования плодов
            }
            else if(m >= 9 && m <= 11) { // Осень - созревание и подготовка к зиме
                rec.n *= 0.80f; // -20% азота (подготовка к покою)
                rec.p *= 1.10f; // +10% фосфора для накопления питательных веществ
                rec.k *= 1.15f; // +15% калия для укрепления тканей
            }
            else { // Зима - период покоя
                rec.n *= 0.70f; // -30% азота (период покоя)
                rec.p *= 1.05f; // +5% фосфора (минимальная поддержка)
                rec.k *= 1.05f; // +5% калия (защита от стресса)
            }
        }
        else if(config.environmentType == 1) { // Greenhouse - теплица
            if(m >= 3 && m <= 5) { // Весна - интенсивный старт
                rec.n *= 1.25f; // +25% азота для интенсивного старта
                rec.p *= 1.20f; // +20% фосфора для активного корнеобразования
                rec.k *= 1.15f; // +15% калия для подготовки к цветению
            }
            else if(m >= 6 && m <= 8) { // Лето - поддержка роста
                rec.n *= 1.10f; // +10% азота для поддержки роста
                rec.p *= 1.10f; // +10% фосфора для поддержки цветения
                rec.k *= 1.30f; // +30% калия для формирования урожая
            }
            else if(m >= 9 && m <= 11) { // Осень - продление вегетации
                rec.n *= 1.15f; // +15% азота для продления вегетации
                rec.p *= 1.15f; // +15% фосфора для поддержки плодоношения
                rec.k *= 1.20f; // +20% калия для качества урожая
            }
            else { // Зима - минимальный рост
                rec.n *= 1.05f; // +5% азота для минимального роста
                rec.p *= 1.10f; // +10% фосфора для поддержки развития
                rec.k *= 1.15f; // +15% калия для стрессоустойчивости
            }
        }
    }

    // Корректировки по условиям выращивания (ФАО Crop Calendar)
    if (config.environmentType == 1) { // Теплица
        rec.t += 2.0f;      // +2°C для теплицы
        rec.hum += 5.0f;  // +5% влажность
        rec.ec += 200.0f;      // +200 µS/cm EC
        rec.n += 100.0f;       // +100 мг/кг N
        rec.p += 50.0f;        // +50 мг/кг P
        rec.k += 75.0f;        // +75 мг/кг K
    } else if (config.environmentType == 2) { // Закрытое помещение
        rec.t -= 1.0f;      // -1°C для комнаты
        rec.hum -= 5.0f;  // -5% влажность
        rec.ec -= 100.0f;      // -100 µS/cm EC
        rec.n -= 50.0f;        // -50 мг/кг N
        rec.p -= 25.0f;        // -25 мг/кг P
        rec.k -= 40.0f;        // -40 мг/кг K
    }

    // ФИНАЛЬНАЯ ВАЛИДАЦИЯ РЕЗУЛЬТАТОВ
    // Ограничение значений физическими пределами
    if(rec.t < 10) rec.t = 10;      if(rec.t > 35) rec.t = 35;
    if(rec.hum < 20) rec.hum = 20;  if(rec.hum > 80) rec.hum = 80;
    if(rec.ec < 200) rec.ec = 200;  if(rec.ec > 3000) rec.ec = 3000;
    if(rec.ph < 4.5) rec.ph = 4.5;  if(rec.ph > 8.5) rec.ph = 8.5;
    if(rec.n < 200) rec.n = 200;    if(rec.n > 2000) rec.n = 2000;
    if(rec.p < 100) rec.p = 100;    if(rec.p > 1000) rec.p = 1000;
    if(rec.k < 200) rec.k = 200;    if(rec.k > 2000) rec.k = 2000;

    return rec;
}

static void handleReadingsUpload()
{
    HTTPUpload& upload = webServer.upload();
    if (upload.status == UPLOAD_FILE_START)
    {
        CalibrationManager::init();
        const char* path = CalibrationManager::profileToFilename(SoilProfile::SAND); // custom.csv
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
        else if (profileStr == "clay") config.soilProfile = 3;

        saveConfig();
        logSuccess("Профиль почвы изменён на %s", profileStr.c_str());
    }
    webServer.sendHeader("Location", "/readings?toast=Профиль+сохранен", true);
    webServer.send(302, "text/plain", "Redirect");
}

static void sendSensorJson()
{
    // unified JSON response for sensor data
    logWebRequest("GET", webServer.uri(), webServer.client().remoteIP().toString());
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

    // Добавляем информацию о культуре и условиях выращивания
    doc["crop"] = config.cropId;
    doc["growing_condition"] = config.environmentType == 0 ? "outdoor" : 
                                config.environmentType == 1 ? "greenhouse" : "indoor";
    doc["soil_profile"] = config.soilProfile;

    // ---- Дополнительная информация ----
    // Сезон по текущему месяцу
    const char* seasonName = [](){
        // Проверяем инициализацию NTP
        if (timeClient == nullptr) {
            extern WiFiUDP ntpUDP;
            timeClient = new NTPClient(ntpUDP, "pool.ntp.org", 0, 3600000);
            timeClient->begin();
        }
        
        time_t now = timeClient ? (time_t)timeClient->getEpochTime() : time(nullptr);
        // если время < 2000-01-01 считаем, что NTP ещё не синхронизирован
        if (now < 946684800) {
            // Пробуем обновить NTP
            if (timeClient) {
                timeClient->forceUpdate();
                now = (time_t)timeClient->getEpochTime();
                if (now < 946684800) return "Н/Д";
            } else {
                return "Н/Д";
            }
        }
        struct tm* ti = localtime(&now);
        if (!ti) return "Н/Д";
        uint8_t m = ti->tm_mon + 1;
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
                         webServer.send(200, "text/html; charset=utf-8", generateApModeUnavailablePage("Показания", UI_ICON_DATA));
                         return;
                     }

                     String html = generatePageHeader("Показания датчика", UI_ICON_DATA);
                     html += navHtml();
                     html += "<h1>" UI_ICON_DATA " Показания датчика</h1>";
                     
                     // Информационная строка состояния
                     html += "<div id='statusInfo' style='margin:10px 0;font-size:16px;color:#333'></div>";
                     
                     // ======= ОБЪЯСНЕНИЕ ПРОЦЕССОВ =======
                     html += "<div class='section' style='background:#f8f9fa;padding:15px;border-radius:8px;margin:15px 0;'>";
                     html += "<h3>📋 Как работают показания</h3>";
                     html += "<div style='display:grid;grid-template-columns:1fr 1fr;gap:20px;font-size:14px;'>";
                     
                     // Левая колонка - компенсация
                     html += "<div>";
                     html += "<h4>🔧 Компенсация показаний</h4>";
                     html += "<ul style='margin:0;padding-left:20px;'>";
                     html += "<li><strong>RAW</strong> - сырые данные с датчика</li>";
                     html += "<li><strong>Компенс.</strong> - данные после математической компенсации:</li>";
                     html += "<ul style='margin:5px 0;padding-left:15px;'>";
                     html += "<li>🌡️ <strong>Температура:</strong> без изменений</li>";
                     html += "<li>💧 <strong>Влажность:</strong> без изменений</li>";
                     html += "<li>⚡ <strong>EC:</strong> температурная компенсация + модель Арчи (Archie, 1942)</li>";
                     html += "<li>⚗️ <strong>pH:</strong> температурная поправка по Нернсту (-0.003×ΔT)</li>";
                     html += "<li>🔴🟡🔵 <strong>NPK:</strong> коррекция по T, влажности и типу почвы (FAO 56 + Eur. J. Soil Sci.)</li>";
                     html += "</ul>";
                     html += "</ul>";
                     html += "</div>";
                     
                     // Правая колонка - рекомендации
                     html += "<div>";
                     html += "<h4>🎯 Рекомендации</h4>";
                     html += "<ul style='margin:0;padding-left:20px;'>";
                     html += "<li><strong>Базовые нормы</strong> для выбранной культуры</li>";
                     html += "<li><strong>Сезонные корректировки</strong> (весна/лето/осень/зима)</li>";
                     html += "<li><strong>Тип среды</strong> (открытый грунт/теплица/помещение)</li>";
                     html += "<li><strong>Цветовая индикация:</strong></li>";
                     html += "<ul style='margin:5px 0;padding-left:15px;'>";
                     html += "<li>🟢 <strong>Зеленый:</strong> в норме</li>";
                     html += "<li>🟡 <strong>Желтый:</strong> близко к границам</li>";
                     html += "<li>🟠 <strong>Оранжевый:</strong> отклонение >20%</li>";
                     html += "<li>🔴 <strong>Красный:</strong> критическое отклонение</li>";
                     html += "</ul>";
                     html += "</ul>";
                     html += "</div>";
                     
                     html += "</div>";
                     html += "</div>";
                     
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
                     html += "<tr><td>🔴 Азот (N), мг/кг</td><td><span id='n_raw'></span></td><td><span id='n'></span></td><td><span id='n_rec'></span><span id='n_season' class='season-adj'></span></td></tr>";
                     html += "<tr><td>🟡 Фосфор (P), мг/кг</td><td><span id='p_raw'></span></td><td><span id='p'></span></td><td><span id='p_rec'></span><span id='p_season' class='season-adj'></span></td></tr>";
                     html += "<tr><td>🔵 Калий (K), мг/кг</td><td><span id='k_raw'></span></td><td><span id='k'></span></td><td><span id='k_rec'></span><span id='k_season' class='season-adj'></span></td></tr>";
                     html += "</tbody></table></div>";
                     
                     // ======= КАЛИБРОВКА =======
                     bool csvPresent = CalibrationManager::hasTable(SoilProfile::SAND); // custom.csv

                     html += "<div class='section'><h2>⚙️ Калибровка датчика</h2>";
                     
                     // Статус калибровки
                     html += "<div style='background:#f8f9fa;padding:15px;border-radius:8px;margin:15px 0;'>";
                     html += "<h4>📊 Текущий статус калибровки</h4>";
                     if (!config.flags.calibrationEnabled) {
                         html += "<p style='color:#9E9E9E;margin:5px 0;'>❌ <strong>Компенсация выключена</strong> - используются только математические поправки</p>";
                     } else if (csvPresent) {
                         html += "<p style='color:#4CAF50;margin:5px 0;'>✅ <strong>CSV таблица загружена</strong> - применяется лабораторная калибровка + математическая компенсация</p>";
                     } else {
                         html += "<p style='color:#2196F3;margin:5px 0;'>⚠️ <strong>CSV таблица не загружена</strong> - применяется только математическая компенсация</p>";
                     }
                     html += "</div>";
                     
                     // Объяснение калибровочной таблицы
                     html += "<div style='background:#f8f9fa;border-left:4px solid #007bff;padding:10px;margin:10px 0;'>";
                     html += "<h4 style='margin:0 0 8px 0;color:#007bff;'>📊 <strong>Калибровочная таблица (CSV)</strong></h4>";
                     html += "<p style='margin:5px 0;font-size:14px;'>";
                     html += "Это файл с коэффициентами коррекции, полученными при поверке датчика в лаборатории. ";
                     html += "Содержит коррекции для всех параметров: <strong>температура, влажность, EC, pH, азот, фосфор, калий</strong>. ";
                     html += "Формат: <code>сырое_значение,коэффициент_коррекции</code>";
                     html += "</p>";
                     html += "<p style='margin:5px 0;font-size:14px;'>";
                     html += "💡 <strong>Применение:</strong> <code>скорректированное = сырое × коэффициент</code>";
                     html += "</p>";
                     html += "<p style='margin:5px 0;font-size:14px;'>";
                     html += "📄 <a href='/docs/examples/calibration_example.csv' target='_blank' style='color:#2196F3;'>Скачать пример CSV файла</a>";
                     html += "</p>";
                     html += "</div>";
                     
                     // Форма загрузки CSV
                     html += "<form action='/readings/upload' method='post' enctype='multipart/form-data' style='margin-top:15px;'>";
                     html += "<div class='form-group'><label for='calibration_csv'><strong>Загрузить CSV файл калибровки:</strong></label>";
                     html += "<input type='file' id='calibration_csv' name='calibration_csv' accept='.csv' required style='margin:5px 0;'>";
                     html += "<div style='font-size:12px;color:#666;margin:5px 0;'>Файл должен содержать пары значений: сырое_значение,коэффициент_коррекции</div>";
                     html += "</div>";
                     html += generateButton(ButtonType::PRIMARY, UI_ICON_UPLOAD, "Загрузить CSV", "");
                     html += "</form>";
                     
                     // Кнопка сброса CSV, если файл существует
                     if(csvPresent){
                         html += "<form action='/readings/csv_reset' method='post' style='margin-top:10px;'>";
                         html += generateButton(ButtonType::SECONDARY, "🗑️", "Удалить CSV таблицу", "");
                         html += "</form>";
                     }
                     html += "</div>";
                     
                     // ======= ДОПОЛНИТЕЛЬНАЯ ИНФОРМАЦИЯ =======
                     html += "<div class='section' style='background:#e8f5e8;padding:15px;border-radius:8px;margin:15px 0;'>";
                     html += "<h4>💡 Полезная информация</h4>";
                     html += "<ul style='margin:5px 0;padding-left:20px;font-size:14px;'>";
                     html += "<li><strong>Стрелки ↑↓</strong> показывают направление изменений после компенсации</li>";
                     html += "<li><strong>Сезонные корректировки</strong> учитывают потребности растений в разные периоды</li>";
                     html += "<li><strong>Валидность данных</strong> проверяется по диапазонам и логическим связям</li>";
                     html += "<li><strong>Интервал обновления:</strong> каждые 3 секунды</li>";
                     html += "</ul>";
                     html += "</div>";

                     html += "<style>";
                     html += ".season-adj { font-size: 0.8em; margin-left: 5px; }";
                     html += ".season-adj.up { color: #2ecc71; }";
                     html += ".season-adj.down { color: #e74c3c; }";
                     html += ".data{width:100%;border-collapse:collapse}.data th,.data td{border:1px solid #ccc;padding:6px;text-align:center}.data th{background:#f5f5f5}.green{color:#4CAF50}.yellow{color:#FFC107}.orange{color:#FF9800}.red{color:#F44336}";
                     html += "</style>";
                     
                     html += "<script>";
                     html += "function set(id,v){if(v!==undefined&&v!==null){document.getElementById(id).textContent=v;}}";
                     html += "function colorDelta(a,b){var diff=Math.abs(a-b)/b*100;if(diff>30)return 'red';if(diff>20)return 'orange';if(diff>10)return 'yellow';return '';}";
                     html += "function colorRange(v,min,max){var span=(max-min);if(span<=0)return '';if(v<min||v>max)return 'red';if(v<min+0.05*span||v>max-0.05*span)return 'orange';if(v<min+0.10*span||v>max-0.10*span)return 'yellow';return '';}";
                     html += "function applyColor(spanId,cls){var el=document.getElementById(spanId);if(!el)return;el.classList.remove('red','orange','yellow','green');if(cls){el.classList.add(cls);}else{el.classList.add('green');}}";
                     html += "var limits={temp:{min:-45,max:115},hum:{min:0,max:100},ec:{min:0,max:10000},ph:{min:3,max:9},n:{min:0,max:1999},p:{min:0,max:1999},k:{min:0,max:1999}};";
                     html += "function updateSensor(){";
                     html += "fetch('/sensor_json').then(r=>r.json()).then(d=>{";
                     html += "set('temp_raw',d.raw_temperature);";
                     html += "set('hum_raw',d.raw_humidity);";
                     html += "set('ec_raw',d.raw_ec);";
                     html += "set('ph_raw',d.raw_ph);";
                     html += "set('n_raw',d.raw_nitrogen);";
                     html += "set('p_raw',d.raw_phosphorus);";
                     html += "set('k_raw',d.raw_potassium);";
                     html += "set('temp_rec',d.rec_temperature);set('hum_rec',d.rec_humidity);set('ec_rec',d.rec_ec);set('ph_rec',d.rec_ph);set('n_rec',d.rec_nitrogen);set('p_rec',d.rec_phosphorus);set('k_rec',d.rec_potassium);";
                     // === Arrow indicators block ===
                     html += "const tol={temp:0.2,hum:0.5,ec:20,ph:0.05,n:5,p:3,k:3};";
                     html += "function arrowSign(base,val,thr){base=parseFloat(base);val=parseFloat(val);if(isNaN(base)||isNaN(val))return '';if(val>base+thr)return '↑ ';if(val<base-thr)return '↓ ';return '';};";
                     html += "function showWithArrow(id,sign,value){document.getElementById(id).textContent=sign+value;}";

                     // Compensated vs RAW arrows
                     html += "showWithArrow('temp', arrowSign(d.raw_temperature ,d.temperature ,tol.temp), d.temperature);";
                     html += "showWithArrow('hum',  arrowSign(d.raw_humidity    ,d.humidity    ,tol.hum ), d.humidity);";
                     html += "showWithArrow('ec',   arrowSign(d.raw_ec          ,d.ec          ,tol.ec  ), d.ec);";
                     html += "showWithArrow('ph',   arrowSign(d.raw_ph          ,d.ph          ,tol.ph  ), d.ph);";
                     html += "showWithArrow('n',    arrowSign(d.raw_nitrogen    ,d.nitrogen    ,tol.n   ), d.nitrogen);";
                     html += "showWithArrow('p',    arrowSign(d.raw_phosphorus  ,d.phosphorus  ,tol.p   ), d.phosphorus);";
                     html += "showWithArrow('k',    arrowSign(d.raw_potassium   ,d.potassium   ,tol.k   ), d.potassium);";

                     // Recommendation arrows (target vs current)
                     html += "showWithArrow('temp_rec', arrowSign(d.temperature ,d.rec_temperature ,tol.temp), d.rec_temperature);";
                     html += "showWithArrow('hum_rec',  arrowSign(d.humidity    ,d.rec_humidity    ,tol.hum ), d.rec_humidity);";
                     html += "showWithArrow('ec_rec',   arrowSign(d.ec          ,d.rec_ec          ,tol.ec  ), d.rec_ec);";
                     html += "showWithArrow('ph_rec',   arrowSign(d.ph          ,d.rec_ph          ,tol.ph  ), d.rec_ph);";
                     html += "showWithArrow('n_rec',    arrowSign(d.nitrogen    ,d.rec_nitrogen    ,tol.n   ), d.rec_nitrogen);";
                     html += "showWithArrow('p_rec',    arrowSign(d.phosphorus  ,d.rec_phosphorus  ,tol.p   ), d.rec_phosphorus);";
                     html += "showWithArrow('k_rec',    arrowSign(d.potassium   ,d.rec_potassium   ,tol.k   ), d.rec_potassium);";
                     // === End arrow indicators ===
                     
                     // Добавляем индикацию сезонных корректировок
                     html += "function updateSeasonalAdjustments(season) {";
                     html += "  const adjustments = {";
                     html += "    'Весна': { n: '+20%', p: '+15%', k: '+10%' },";
                     html += "    'Лето': { n: '-10%', p: '+5%', k: '+25%' },";
                     html += "    'Осень': { n: '-20%', p: '+10%', k: '+15%' },";
                     html += "    'Зима': { n: '-30%', p: '+5%', k: '+5%' }";
                     html += "  };";
                     html += "  const envType = " + String(config.environmentType) + ";";
                     html += "  if(envType === 1) {"; // Теплица
                     html += "    adjustments['Весна'] = { n: '+25%', p: '+20%', k: '+15%' };";
                     html += "    adjustments['Лето'] = { n: '+10%', p: '+10%', k: '+30%' };";
                     html += "    adjustments['Осень'] = { n: '+15%', p: '+15%', k: '+20%' };";
                     html += "    adjustments['Зима'] = { n: '+5%', p: '+10%', k: '+15%' };";
                     html += "  }";
                     html += "  const adj = adjustments[season] || { n: '', p: '', k: '' };";
                     html += "  ['n', 'p', 'k'].forEach(elem => {";
                     html += "    const span = document.getElementById(elem + '_season');";
                     html += "    if(span) {";
                     html += "      span.textContent = adj[elem] ? ` (${adj[elem]})` : '';";
                     html += "      span.className = 'season-adj ' + (adj[elem].startsWith('+') ? 'up' : 'down');";
                     html += "    }";
                     html += "  });";
                     html += "}";
                     
                     html += "var invalid = d.irrigation || d.alerts.length>0 || d.humidity<25 || d.temperature<5 || d.temperature>40;";
                     html += "var statusHtml = invalid ? '<span class=\\\"red\\\">Данные&nbsp;не&nbsp;валидны</span>' : '<span class=\\\"green\\\">Данные&nbsp;валидны</span>';";
                     html += "var seasonColor={'Лето':'green','Весна':'yellow','Осень':'yellow','Зима':'red','Н/Д':''}[d.season]||'';";
                     html += "var seasonHtml=seasonColor?(`<span class=\\\"${seasonColor}\\\">${d.season}</span>`):d.season;";
                     html += "document.getElementById('statusInfo').innerHTML=statusHtml+' | Сезон: '+seasonHtml;";
                     html += "updateSeasonalAdjustments(d.season);";
                     html += "var tvr=parseFloat(d.raw_temperature);applyColor('temp_raw',colorRange(tvr,limits.temp.min,limits.temp.max));";
                     html += "var hvr=parseFloat(d.raw_humidity);applyColor('hum_raw',colorRange(hvr,limits.hum.min,limits.hum.max));";
                     html += "var evr=parseFloat(d.raw_ec);applyColor('ec_raw',colorRange(evr,limits.ec.min,limits.ec.max));";
                     html += "var pvr=parseFloat(d.raw_ph);applyColor('ph_raw',colorRange(pvr,limits.ph.min,limits.ph.max));";
                     html += "var nvr=parseFloat(d.raw_nitrogen);applyColor('n_raw',colorRange(nvr,limits.n.min,limits.n.max));";
                     html += "var p2r=parseFloat(d.raw_phosphorus);applyColor('p_raw',colorRange(p2r,limits.p.min,limits.p.max));";
                     html += "var kvr=parseFloat(d.raw_potassium);applyColor('k_raw',colorRange(kvr,limits.k.min,limits.k.max));";
                     html += "['temp','hum','ec','ph','n','p','k'].forEach(function(id){var el=document.getElementById(id);if(el){el.classList.remove('red','orange','yellow','green');}});";
                     html += "var ct=parseFloat(d.temperature);";
                     html += "var ch=parseFloat(d.humidity);";
                     html += "var ce=parseFloat(d.ec);";
                     html += "var cph=parseFloat(d.ph);";
                     html += "var cn=parseFloat(d.nitrogen);";
                     html += "var cp=parseFloat(d.phosphorus);";
                     html += "var ck=parseFloat(d.potassium);";
                     html += "applyColor('temp_rec', colorDelta(ct, parseFloat(d.rec_temperature)));";
                     html += "applyColor('hum_rec',  colorDelta(ch, parseFloat(d.rec_humidity)));";
                     html += "applyColor('ec_rec',   colorDelta(ce, parseFloat(d.rec_ec)));";
                     html += "applyColor('ph_rec',   colorDelta(cph,parseFloat(d.rec_ph)));";
                     html += "applyColor('n_rec',    colorDelta(cn, parseFloat(d.rec_nitrogen)));";
                     html += "applyColor('p_rec',    colorDelta(cp, parseFloat(d.rec_phosphorus)));";
                     html += "applyColor('k_rec',    colorDelta(ck, parseFloat(d.rec_potassium)));";
                     html += "});";
                     html += "}";
                     html += "setInterval(updateSensor,3000);";
                     html += "updateSensor();";
                     html += "</script>";
                     
                     // API-ссылка внизу страницы
                     html += "<div style='margin-top:15px;font-size:14px;color:#555'><b>API:</b> <a href='" + String(API_SENSOR) + "' target='_blank'>" + String(API_SENSOR) + "</a> (JSON, +timestamp)</div>";
                     
                     html += generatePageFooter();
                     webServer.send(200, "text/html; charset=utf-8", html);
                 });

    // AJAX эндпоинт для обновления показаний
    webServer.on("/sensor_json", HTTP_GET, sendSensorJson);

    // Primary API v1 endpoint
    webServer.on(API_SENSOR, HTTP_GET, sendSensorJson);

    // Загрузка калибровочного CSV через вкладку
    webServer.on("/readings/upload", HTTP_POST, [](){}, handleReadingsUpload);

    // Сброс пользовательских CSV (удаляем все *.csv)
    webServer.on("/readings/csv_reset", HTTP_POST,
                 []() {
                     logWebRequest("POST","/readings/csv_reset", webServer.client().remoteIP().toString());
                     CalibrationManager::init();
                     bool removed = CalibrationManager::deleteTable(SoilProfile::SAND);
                     String toast = removed?"CSV+удален":"CSV+не+найден";
                     webServer.sendHeader("Location", String("/readings?toast=") + toast, true);
                     webServer.send(302,"text/plain","Redirect");
                 });

    // Форма для сохранения профиля
    webServer.on("/readings/profile", HTTP_POST, [](){}, handleProfileSave);

    // Обслуживание статических файлов из LittleFS
    webServer.on("/docs/examples/calibration_example.csv", HTTP_GET,
                 []() {
                     logWebRequest("GET", "/docs/examples/calibration_example.csv", webServer.client().remoteIP().toString());
                     
                     if (LittleFS.exists("/docs/examples/calibration_example.csv")) {
                         File file = LittleFS.open("/docs/examples/calibration_example.csv", "r");
                         if (file) {
                             webServer.sendHeader("Content-Type", "text/csv");
                             webServer.sendHeader("Content-Disposition", "attachment; filename=\"calibration_example.csv\"");
                             webServer.streamFile(file, "text/csv");
                             file.close();
                         } else {
                             webServer.send(404, "text/plain", "File not found");
                         }
                     } else {
                         // Если файл не найден, создаем его на лету
                         webServer.sendHeader("Content-Type", "text/csv");
                         webServer.sendHeader("Content-Disposition", "attachment; filename=\"calibration_example.csv\"");
                         String csvContent = "# Пример калибровочной таблицы для JXCT датчика\n";
                         csvContent += "# Формат: сырое_значение,коэффициент_коррекции\n";
                         csvContent += "# Коэффициент применяется как: скорректированное_значение = сырое_значение * коэффициент\n\n";
                         csvContent += "# Электропроводность (µS/cm) - может требовать коррекции\n";
                         csvContent += "0,1.000\n";
                         csvContent += "500,0.98\n";
                         csvContent += "1000,0.95\n";
                         csvContent += "1500,0.93\n";
                         csvContent += "2000,0.91\n";
                         csvContent += "3000,0.89\n";
                         csvContent += "5000,0.87\n\n";
                         csvContent += "# pH - может требовать коррекции\n";
                         csvContent += "3.0,1.000\n";
                         csvContent += "4.0,1.000\n";
                         csvContent += "5.0,1.000\n";
                         csvContent += "6.0,1.000\n";
                         csvContent += "7.0,1.000\n";
                         csvContent += "8.0,1.000\n";
                         csvContent += "9.0,1.000\n\n";
                         csvContent += "# Азот (мг/кг) - может требовать коррекции\n";
                         csvContent += "0,1.000\n";
                         csvContent += "100,0.95\n";
                         csvContent += "200,0.92\n";
                         csvContent += "500,0.89\n";
                         csvContent += "1000,0.87\n";
                         csvContent += "1500,0.85\n";
                         webServer.send(200, "text/csv", csvContent);
                     }
                 });

    // Deprecated alias удалён в v2.7.0

    logDebug("Маршруты данных настроены: /readings, /api/v1/sensor (json), /sensor_json [legacy]");
}

// Вспомогательная функция для получения SSID точки доступа
extern String getApSsid();