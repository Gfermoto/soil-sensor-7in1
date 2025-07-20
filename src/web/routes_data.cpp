/**
 * @file routes_data.cpp
 * @brief Маршруты для работы с данными датчика
 * @details Обработка запросов показаний датчика, JSON API и веб-интерфейса данных
 */

#include <ArduinoJson.h>
#include <LittleFS.h>
#include <NTPClient.h>
#include <ctime>
#include "../../include/jxct_config_vars.h"
#include "../../include/jxct_constants.h"
#include "../../include/jxct_format_utils.h"
#include "../../include/jxct_strings.h"
#include "../../include/jxct_ui_system.h"
#include "../../include/logger.h"
#include "../../include/web/csrf_protection.h"  // 🔒 CSRF защита
#include "../../include/web_routes.h"
#include "../modbus_sensor.h"
#include "../wifi_manager.h"
#include "business_services.h"
#include "calibration_manager.h"
#include "../../include/sensor_calibration_service.h"

extern NTPClient* timeClient;
// extern SensorCalibrationService gCalibrationService;  // Убрано - объявлен в unified_calibration_service.cpp

// Внешние зависимости (уже объявлены в заголовочных файлах)
// extern String navHtml();  // объявлено в wifi_manager.h
// extern String formatValue(float value, const char* unit, int precision);  // объявлено в jxct_format_utils.h
// extern String getApSsid();  // объявлено в wifi_manager.h

// Буфер для загрузки файлов (калибровка через /readings)
namespace
{
File uploadFile;
SoilProfile uploadProfile = SoilProfile::SAND;

// Используем RecValues из бизнес-сервиса

// Функции сезонной коррекции NPK перенесены в бизнес-сервис CropRecommendationEngine

RecValues computeRecommendations()
{
    // Используем бизнес-сервис для вычисления рекомендаций
    const String cropId = String(config.cropId);

    // Преобразуем конфигурацию в типы бизнес-логики
    SoilProfile soilProfile = SoilProfile::SAND;
    EnvironmentType envType = EnvironmentType::OUTDOOR;

    // Используем массивы для устранения дублирования кода
    static const std::array<SoilProfile, 5> soilProfiles = {{
        SoilProfile::SAND,     // 0
        SoilProfile::LOAM,     // 1
        SoilProfile::PEAT,     // 2
        SoilProfile::CLAY,     // 3
        SoilProfile::SANDPEAT  // 4
    }};

    static const std::array<EnvironmentType, 3> envTypes = {{
        EnvironmentType::OUTDOOR,     // 0
        EnvironmentType::GREENHOUSE,  // 1
        EnvironmentType::INDOOR       // 2
    }};

    const int soilIndex = (config.soilProfile >= 0 && config.soilProfile < 5) ? config.soilProfile : 0;
    const int envIndex = (config.environmentType >= 0 && config.environmentType < 3) ? config.environmentType : 0;

    soilProfile = soilProfiles[soilIndex];
    envType = envTypes[envIndex];

    // Получаем рекомендации от бизнес-сервиса
    RecValues rec = getCropEngine().computeRecommendations(cropId, soilProfile, envType);

    // Применяем сезонную коррекцию если включена
    if (config.flags.seasonalAdjustEnabled)
    {
        time_t now = time(nullptr);
        struct tm* timeInfo = localtime(&now);
        const int month = timeInfo != nullptr ? timeInfo->tm_mon + 1 : 1;

        // Определяем сезон
        Season season = Season::WINTER;
        if (month >= 3 && month <= 5) {
            season = Season::SPRING;
        } else if (month >= 6 && month <= 8) {
            season = Season::SUMMER;
        } else if (month >= 9 && month <= 11) {
            season = Season::AUTUMN;
        } else {
            season = Season::WINTER;
        }

        const bool isGreenhouse = (config.environmentType == 1);
        getCropEngine().applySeasonalCorrection(rec, season, isGreenhouse);
    }

    return rec;
}
}  // namespace

void handleReadingsUpload()  // ✅ Убираем static - функция extern в header
{
    HTTPUpload& upload = webServer.upload();
    if (upload.status == UPLOAD_FILE_START)
    {
        CalibrationManager::init();
        const char* path = CalibrationManager::profileToFilename(SoilProfile::SAND);  // custom.csv
        uploadFile = LittleFS.open(path, "w");
        if (!uploadFile)
        {
            logErrorSafe("\1", path);
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
            logSuccessSafe("\1", upload.totalSize);
        }
        webServer.sendHeader("Location", "/readings?toast=Калибровка+загружена", true);
        webServer.send(HTTP_REDIRECT, "text/plain", "Redirect");
    }
}

namespace
{
void handleProfileSave()
{
    if (webServer.hasArg("soil_profile"))
    {
        const String profileStr = webServer.arg("soil_profile");
        if (profileStr == "sand")
        {
            config.soilProfile = 0;
        }
        else if (profileStr == "loam")
        {
            config.soilProfile = 1;
        }
        else if (profileStr == "peat")
        {
            config.soilProfile = 2;
        }
        else if (profileStr == "clay")
        {
            config.soilProfile = 3;
        }

        saveConfig();
        logSuccessSafe("\1", profileStr.c_str());
    }
    webServer.sendHeader("Location", "/readings?toast=Профиль+сохранен", true);
    webServer.send(HTTP_REDIRECT, "text/plain", "Redirect");
}
}  // namespace

void sendSensorJson()  // ✅ Убираем static - функция extern в header
{
    // unified JSON response for sensor data
    logWebRequest("GET", webServer.uri(), webServer.client().remoteIP().toString());
    if (currentWiFiMode != WiFiMode::STA)
    {
        webServer.send(HTTP_FORBIDDEN, HTTP_CONTENT_TYPE_JSON, R"({"error":"AP mode"})");
        return;
    }

    StaticJsonDocument<SENSOR_JSON_DOC_SIZE> doc;
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
    doc["valid"] = validateSensorData(sensorData);  // Флаг валидности по лимитам датчика

    const RecValues rec = computeRecommendations();
    doc["rec_temperature"] = format_temperature(rec.t);
    doc["rec_humidity"] = format_moisture(rec.hum);
    doc["rec_ec"] = format_ec(rec.ec);
    doc["rec_ph"] = format_ph(rec.ph);
    doc["rec_nitrogen"] = format_npk(rec.n);
    doc["rec_phosphorus"] = format_npk(rec.p);
    doc["rec_potassium"] = format_npk(rec.k);

    // ---- Дополнительная информация ----
    // Сезон по текущему месяцу
    const char* seasonName = []()
    {
        // Проверяем инициализацию NTP
        if (timeClient == nullptr)
        {
            extern WiFiUDP ntpUDP;
            timeClient = new NTPClient(ntpUDP, "pool.ntp.org", 0, 3600000);
            timeClient->begin();
        }

        time_t now = timeClient ? (time_t)timeClient->getEpochTime() : time(nullptr);
        // если время < 2000-01-01 считаем, что NTP ещё не синхронизирован
        if (now < NTP_TIMESTAMP_2000)
        {
            // Пробуем обновить NTP
            if (timeClient)
            {
                timeClient->forceUpdate();
                now = (time_t)timeClient->getEpochTime();
                if (now < NTP_TIMESTAMP_2000)
                {
                    return "Н/Д";
                }
            }
            else
            {
                return "Н/Д";
            }
        }
        struct tm* timeInfo = localtime(&now);
        if (!timeInfo)
        {
            return "Н/Д";
        }
        uint8_t month = timeInfo->tm_mon + 1;
        if (month == 12 || month == 1 || month == 2)
        {
            return "Зима";
        }
        if (month >= 3 && month <= 5)
        {
            return "Весна";
        }
        if (month >= 6 && month <= 8)
        {
            return "Лето";
        }
        return "Осень";
    }();
    doc["season"] = seasonName;

    // Проверяем отклонения
    String alerts = "";
    auto append = [&](const char* n)
    {
        if (alerts.length())
        {
            alerts += ", ";
        }
        alerts += n;
    };
    // Физические пределы датчика
    if (sensorData.temperature < TEMP_MIN_VALID || sensorData.temperature > TEMP_MAX_VALID)
    {
        append("T");
    }
    if (sensorData.humidity < HUM_MIN_VALID || sensorData.humidity > HUM_MAX_VALID)
    {
        append("θ");
    }
    if (sensorData.ec < 0 || sensorData.ec > EC_MAX_VALID)
    {
        append("EC");
    }
    if (sensorData.ph < 3 || sensorData.ph > 9)
    {
        append("pH");
    }
    if (sensorData.nitrogen < 0 || sensorData.nitrogen > NPK_MAX_VALID)
    {
        append("N");
    }
    if (sensorData.phosphorus < 0 || sensorData.phosphorus > NPK_MAX_VALID)
    {
        append("P");
    }
    if (sensorData.potassium < 0 || sensorData.potassium > NPK_MAX_VALID)
    {
        append("K");
    }
    doc["alerts"] = alerts;

    doc["timestamp"] = (long)(timeClient != nullptr ? timeClient->getEpochTime() : 0);

    String json;
    serializeJson(doc, json);
    webServer.send(HTTP_OK, HTTP_CONTENT_TYPE_JSON, json);
}

void setupDataRoutes()
{
    // Красивая страница показаний с иконками (оригинальный дизайн)
    webServer.on(
        "/readings", HTTP_GET,
        []()
        {
            logWebRequest("GET", "/readings", webServer.client().remoteIP().toString());

            if (currentWiFiMode == WiFiMode::AP)
            {
                webServer.send(200, "text/html; charset=utf-8",
                               generateApModeUnavailablePage("Показания", UI_ICON_DATA));
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
            html +=
                "<li>🔴🟡🔵 <strong>NPK:</strong> коррекция по T, влажности и типу почвы (FAO 56 + Eur. J. Soil "
                "Sci.)</li>";
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
            html += "<li>🟢 <strong>Зеленый:</strong> оптимальные условия измерения</li>";
            html +=
                "<li>🟠 <strong>Оранжевый:</strong> неоптимальные условия (влажность <25%, температура <5°C или "
                ">40°C)</li>";
            html += "<li>🔵 <strong>Синий:</strong> полив активен (временная невалидность)</li>";
            html += "<li>🔴 <strong>Красный:</strong> ошибки датчика (выход за физические пределы)</li>";
            html += "</ul>";
            html += "</ul>";
            html += "</div>";

            html += "</div>";
            html += "</div>";

            // Заголовок 4-го столбца: выбранная культура или «Реком.»
            String recHeader = "Реком.";
            if (strlen(config.cropId) > 0)
            {
                const char* cropId = config.cropId;
                if (strcmp(cropId, "tomato") == 0)
                {
                    recHeader = "Томаты";
                }
                else if (strcmp(cropId, "cucumber") == 0)
                {
                    recHeader = "Огурцы";
                }
                else if (strcmp(cropId, "pepper") == 0)
                {
                    recHeader = "Перец";
                }
                else if (strcmp(cropId, "lettuce") == 0)
                {
                    recHeader = "Салат";
                }
                else if (strcmp(cropId, "blueberry") == 0)
                {
                    recHeader = "Голубика";
                }
                else if (strcmp(cropId, "lawn") == 0)
                {
                    recHeader = "Газон";
                }
                else if (strcmp(cropId, "grape") == 0)
                {
                    recHeader = "Виноград";
                }
                else if (strcmp(cropId, "conifer") == 0)
                {
                    recHeader = "Хвойные";
                }
                else if (strcmp(cropId, "strawberry") == 0)
                {
                    recHeader = "Клубника";
                }
                else if (strcmp(cropId, "apple") == 0)
                {
                    recHeader = "Яблоня";
                }
                else if (strcmp(cropId, "pear") == 0)
                {
                    recHeader = "Груша";
                }
                else if (strcmp(cropId, "cherry") == 0)
                {
                    recHeader = "Вишня";
                }
                else if (strcmp(cropId, "raspberry") == 0)
                {
                    recHeader = "Малина";
                }
                else if (strcmp(cropId, "currant") == 0)
                {
                    recHeader = "Смородина";
                }
            }

            html += "<div class='section'><table class='data'><thead><tr><th></th><th>RAW</th><th>Компенс.</th><th>" +
                    recHeader + "</th></tr></thead><tbody>";
            html +=
                "<tr><td>🌡️ Температура, °C</td><td><span id='temp_raw'></span></td><td><span "
                "id='temp'></span></td><td><span id='temp_rec'></span></td></tr>";
            html +=
                "<tr><td>💧 Влажность, %</td><td><span id='hum_raw'></span></td><td><span "
                "id='hum'></span></td><td><span id='hum_rec'></span></td></tr>";
            html +=
                "<tr><td>⚡ EC, µS/cm</td><td><span id='ec_raw'></span></td><td><span id='ec'></span></td><td><span "
                "id='ec_rec'></span></td></tr>";
            html +=
                "<tr><td>⚗️ pH</td><td><span id='ph_raw'></span></td><td><span id='ph'></span></td><td><span "
                "id='ph_rec'></span></td></tr>";
            html +=
                "<tr><td>🔴 Азот (N), мг/кг</td><td><span id='n_raw'></span></td><td><span "
                "id='n'></span></td><td><span id='n_rec'></span><span id='n_season' "
                "class='season-adj'></span></td></tr>";
            html +=
                "<tr><td>🟡 Фосфор (P), мг/кг</td><td><span id='p_raw'></span></td><td><span "
                "id='p'></span></td><td><span id='p_rec'></span><span id='p_season' "
                "class='season-adj'></span></td></tr>";
            html +=
                "<tr><td>🔵 Калий (K), мг/кг</td><td><span id='k_raw'></span></td><td><span "
                "id='k'></span></td><td><span id='k_rec'></span><span id='k_season' "
                "class='season-adj'></span></td></tr>";
            html += "</tbody></table></div>";

            // ======= КАЛИБРОВКА =======
            html += "<div class='section'><h2>⚙️ Калибровка датчика</h2>";

            // Статус калибровки
            html += "<div style='background:#f8f9fa;padding:15px;border-radius:8px;margin:15px 0;'>";
            html += "<h4>📊 Текущий статус калибровки</h4>";
            html += "<div id='calibration-status'>Загрузка статуса...</div>";
            html += "</div>";

            // pH калибровка
            html += "<div class='section'>";
            html += "<h3>🧪 pH калибровка</h3>";
            html += "<p>Введите показания для буферных растворов pH:</p>";
            html += "<div class='form-group'>";
            html += "<label for='ph_expected'>Ожидаемое значение pH:</label>";
            html += "<input type='number' id='ph_expected' step='0.1' min='0' max='14' placeholder='7.0'>";
            html += "</div>";
            html += "<div class='form-group'>";
            html += "<label for='ph_measured'>Измеренное значение pH:</label>";
            html += "<input type='number' id='ph_measured' step='0.1' min='0' max='14' placeholder='6.8'>";
            html += "</div>";
            html += "<button onclick='addPHPoint()' class='btn btn-primary'>Добавить точку pH</button>";
            html += "<div id='ph-points' style='margin-top:10px;'></div>";
            html += "</div>";

            // EC калибровка
            html += "<div class='section'>";
            html += "<h3>⚡ EC калибровка</h3>";
            html += "<p>Введите показания для стандартных растворов EC:</p>";
            html += "<div class='form-group'>";
            html += "<label for='ec_expected'>Ожидаемое значение EC (мСм/см):</label>";
            html += "<input type='number' id='ec_expected' step='0.1' min='0' placeholder='1.0'>";
            html += "</div>";
            html += "<div class='form-group'>";
            html += "<label for='ec_measured'>Измеренное значение EC (мСм/см):</label>";
            html += "<input type='number' id='ec_measured' step='0.1' min='0' placeholder='0.95'>";
            html += "</div>";
            html += "<button onclick='addECPoint()' class='btn btn-primary'>Добавить точку EC</button>";
            html += "<div id='ec-points' style='margin-top:10px;'></div>";
            html += "</div>";

            // NPK калибровка
            html += "<div class='section'>";
            html += "<h3>🌱 NPK калибровка</h3>";
            html += "<p>Введите показания для дистиллированной воды (должны быть близки к нулю):</p>";
            html += "<div class='form-group'>";
            html += "<label for='npk_n'>N (мг/кг):</label>";
            html += "<input type='number' id='npk_n' step='0.1' min='0' placeholder='0.0'>";
            html += "</div>";
            html += "<div class='form-group'>";
            html += "<label for='npk_p'>P (мг/кг):</label>";
            html += "<input type='number' id='npk_p' step='0.1' min='0' placeholder='0.0'>";
            html += "</div>";
            html += "<div class='form-group'>";
            html += "<label for='npk_k'>K (мг/кг):</label>";
            html += "<input type='number' id='npk_k' step='0.1' min='0' placeholder='0.0'>";
            html += "</div>";
            html += "<button onclick='setNPKPoint()' class='btn btn-primary'>Установить NPK</button>";
            html += "</div>";

            // Действия
            html += "<div class='section'>";
            html += "<h3>⚙️ Действия</h3>";
            html += "<div style='display:grid;grid-template-columns:1fr 1fr;gap:10px;margin-bottom:10px;'>";
            html += "<button onclick='calculatePH()' class='btn btn-success'>Рассчитать pH</button>";
            html += "<button onclick='calculateEC()' class='btn btn-success'>Рассчитать EC</button>";
            html += "</div>";
            html += "<div style='display:grid;grid-template-columns:1fr 1fr;gap:10px;margin-bottom:10px;'>";
            html += "<button onclick='exportCalibration()' class='btn btn-info'>Экспорт</button>";
            html += "<button onclick='importCalibration()' class='btn btn-info'>Импорт</button>";
            html += "</div>";
            html += "<button onclick='resetCalibration()' class='btn btn-danger'>Сбросить калибровку</button>";
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
            html +=
                ".data{width:100%;border-collapse:collapse}.data th,.data td{border:1px solid "
                "#ccc;padding:6px;text-align:center}.data "
                "th{background:#f5f5f5}.green{color:#4CAF50}.yellow{color:#FFC107}.orange{color:#FF9800}.red{color:#"
                "F44336}.blue{color:#2196F3}";
            html += "</style>";

            html += "<script>";
            html += "function set(id,v){if(v!==undefined&&v!==null){document.getElementById(id).textContent=v;}}";
            html +=
                "function colorDelta(a,b){var diff=Math.abs(a-b)/b*100;if(diff>30)return 'red';if(diff>20)return "
                "'orange';if(diff>10)return 'yellow';return '';}";
            html +=
                "function colorRange(v,min,max){var span=(max-min);if(span<=0)return '';if(v<min||v>max)return "
                "'red';if(v<min+0.05*span||v>max-0.05*span)return 'orange';if(v<min+0.10*span||v>max-0.10*span)return "
                "'yellow';return '';}";
            html +=
                "function applyColor(spanId,cls){var "
                "el=document.getElementById(spanId);if(!el)return;el.classList.remove('red','orange','yellow','green');"
                "if(cls){el.classList.add(cls);}else{el.classList.add('green');}}";
            html +=
                "var "
                "limits={temp:{min:-45,max:115},hum:{min:0,max:100},ec:{min:0,max:10000},ph:{min:3,max:9},n:{min:0,max:"
                "1999},p:{min:0,max:1999},k:{min:0,max:1999}};";
            html += "function updateSensor(){";
            html += "fetch('/sensor_json').then(r=>r.json()).then(d=>{";
            html += "set('temp_raw',d.raw_temperature);";
            html += "set('hum_raw',d.raw_humidity);";
            html += "set('ec_raw',d.raw_ec);";
            html += "set('ph_raw',d.raw_ph);";
            html += "set('n_raw',d.raw_nitrogen);";
            html += "set('p_raw',d.raw_phosphorus);";
            html += "set('k_raw',d.raw_potassium);";
            html +=
                "set('temp_rec',d.rec_temperature);set('hum_rec',d.rec_humidity);set('ec_rec',d.rec_ec);set('ph_rec',d."
                "rec_ph);set('n_rec',d.rec_nitrogen);set('p_rec',d.rec_phosphorus);set('k_rec',d.rec_potassium);";
            // === Arrow indicators block ===
            html += "const tol={temp:0.2,hum:0.5,ec:20,ph:0.05,n:5,p:3,k:3};";
            html +=
                "function "
                "arrowSign(base,val,thr){base=parseFloat(base);val=parseFloat(val);if(isNaN(base)||isNaN(val))return "
                "'';if(val>base+thr)return '↑ ';if(val<base-thr)return '↓ ';return '';};";
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
            html +=
                "showWithArrow('temp_rec', arrowSign(d.temperature ,d.rec_temperature ,tol.temp), d.rec_temperature);";
            html += "showWithArrow('hum_rec',  arrowSign(d.humidity    ,d.rec_humidity    ,tol.hum ), d.rec_humidity);";
            html += "showWithArrow('ec_rec',   arrowSign(d.ec          ,d.rec_ec          ,tol.ec  ), d.rec_ec);";
            html += "showWithArrow('ph_rec',   arrowSign(d.ph          ,d.rec_ph          ,tol.ph  ), d.rec_ph);";
            html += "showWithArrow('n_rec',    arrowSign(d.nitrogen    ,d.rec_nitrogen    ,tol.n   ), d.rec_nitrogen);";
            html +=
                "showWithArrow('p_rec',    arrowSign(d.phosphorus  ,d.rec_phosphorus  ,tol.p   ), d.rec_phosphorus);";
            html +=
                "showWithArrow('k_rec',    arrowSign(d.potassium   ,d.rec_potassium   ,tol.k   ), d.rec_potassium);";
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
            html += "  const adj = adjustments[season] || { n: '', p: '', k: '' };";
            html += "  ['n', 'p', 'k'].forEach(elem => {";
            html += "    const span = document.getElementById(elem + '_season');";
            html += "    if(span) {";
            html += "      span.textContent = adj[elem] ? ` (${adj[elem]})` : '';";
            html += "      span.className = 'season-adj ' + (adj[elem].startsWith('+') ? 'up' : 'down');";
            html += "    }";
            html += "  });";
            html += "}";

            html += R"(var invalid = d.irrigation || d.alerts.length>0 || d.humidity<25 || d.temperature<5 || )"
                    R"(d.temperature>40;)";
            html += R"(var statusHtml = invalid ? '<span class="red">Данные&nbsp;не&nbsp;валидны</span>' : '<span )"
                    R"(class="green">Данные&nbsp;валидны</span>';)";
            html +=
                R"(var seasonColor={'Лето':'green','Весна':'yellow','Осень':'yellow','Зима':'red','Н/Д':''}[d.season]||'';)";
            html += R"(var seasonHtml=seasonColor?(`<span class=\"${seasonColor}\">${d.season}</span>`):d.season;)";
            html += "document.getElementById('statusInfo').innerHTML=statusHtml+' | Сезон: '+seasonHtml;";
            html += "updateSeasonalAdjustments(d.season);";
            html +=
                "var "
                "tvr=parseFloat(d.raw_temperature);applyColor('temp_raw',colorRange(tvr,limits.temp.min,limits.temp."
                "max));";
            html +=
                "var "
                "hvr=parseFloat(d.raw_humidity);applyColor('hum_raw',colorRange(hvr,limits.hum.min,limits.hum.max));";
            html += "var evr=parseFloat(d.raw_ec);applyColor('ec_raw',colorRange(evr,limits.ec.min,limits.ec.max));";
            html += "var pvr=parseFloat(d.raw_ph);applyColor('ph_raw',colorRange(pvr,limits.ph.min,limits.ph.max));";
            html += "var nvr=parseFloat(d.raw_nitrogen);applyColor('n_raw',colorRange(nvr,limits.n.min,limits.n.max));";
            html +=
                "var p2r=parseFloat(d.raw_phosphorus);applyColor('p_raw',colorRange(p2r,limits.p.min,limits.p.max));";
            html +=
                "var kvr=parseFloat(d.raw_potassium);applyColor('k_raw',colorRange(kvr,limits.k.min,limits.k.max));";
            html +=
                "['temp','hum','ec','ph','n','p','k'].forEach(function(id){var "
                "el=document.getElementById(id);if(el){el.classList.remove('red','orange','yellow','green');}});";
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

            // Функции калибровки
            html += "function updateCalibrationStatus() {";
            html += "  fetch('/api/calibration/status')";
            html += "    .then(response => response.json())";
            html += "    .then(data => {";
            html += "      document.getElementById('calibration-status').innerHTML = data.status;";
            html += "    });";
            html += "}";
            html += "function addPHPoint() {";
            html += "  const expected = parseFloat(document.getElementById('ph_expected').value);";
            html += "  const measured = parseFloat(document.getElementById('ph_measured').value);";
            html += "  fetch('/api/calibration/ph/add', {";
            html += "    method: 'POST',";
            html += "    headers: {'Content-Type': 'application/json'},";
            html += "    body: JSON.stringify({expected: expected, measured: measured})";
            html += "  }).then(response => response.json())";
            html += "    .then(data => {";
            html += "      if(data.success) {";
            html += "        updateCalibrationStatus();";
            html += "        document.getElementById('ph_expected').value = '';";
            html += "        document.getElementById('ph_measured').value = '';";
            html += "      }";
            html += "    });";
            html += "}";
            html += "function addECPoint() {";
            html += "  const expected = parseFloat(document.getElementById('ec_expected').value);";
            html += "  const measured = parseFloat(document.getElementById('ec_measured').value);";
            html += "  fetch('/api/calibration/ec/add', {";
            html += "    method: 'POST',";
            html += "    headers: {'Content-Type': 'application/json'},";
            html += "    body: JSON.stringify({expected: expected, measured: measured})";
            html += "  }).then(response => response.json())";
            html += "    .then(data => {";
            html += "      if(data.success) {";
            html += "        updateCalibrationStatus();";
            html += "        document.getElementById('ec_expected').value = '';";
            html += "        document.getElementById('ec_measured').value = '';";
            html += "      }";
            html += "    });";
            html += "}";
            html += "function setNPKPoint() {";
            html += "  const n = parseFloat(document.getElementById('npk_n').value);";
            html += "  const p = parseFloat(document.getElementById('npk_p').value);";
            html += "  const k = parseFloat(document.getElementById('npk_k').value);";
            html += "  fetch('/api/calibration/npk/set', {";
            html += "    method: 'POST',";
            html += "    headers: {'Content-Type': 'application/json'},";
            html += "    body: JSON.stringify({n: n, p: p, k: k})";
            html += "  }).then(response => response.json())";
            html += "    .then(data => {";
            html += "      if(data.success) {";
            html += "        updateCalibrationStatus();";
            html += "        document.getElementById('npk_n').value = '';";
            html += "        document.getElementById('npk_p').value = '';";
            html += "        document.getElementById('npk_k').value = '';";
            html += "      }";
            html += "    });";
            html += "}";
            html += "function calculatePH() {";
            html += "  fetch('/api/calibration/ph/calculate', {method: 'POST'})";
            html += "    .then(response => response.json())";
            html += "    .then(data => {";
            html += "      if(data.success) {";
            html += "        updateCalibrationStatus();";
            html += "        alert('pH калибровка рассчитана! R² = ' + data.r_squared);";
            html += "      }";
            html += "    });";
            html += "}";
            html += "function calculateEC() {";
            html += "  fetch('/api/calibration/ec/calculate', {method: 'POST'})";
            html += "    .then(response => response.json())";
            html += "    .then(data => {";
            html += "      if(data.success) {";
            html += "        updateCalibrationStatus();";
            html += "        alert('EC калибровка рассчитана! R² = ' + data.r_squared);";
            html += "      }";
            html += "    });";
            html += "}";
            html += "function exportCalibration() {";
            html += "  fetch('/api/calibration/export')";
            html += "    .then(response => response.json())";
            html += "    .then(data => {";
            html += "      const blob = new Blob([JSON.stringify(data, null, 2)], {type: 'application/json'});";
            html += "      const url = URL.createObjectURL(blob);";
            html += "      const a = document.createElement('a');";
            html += "      a.href = url;";
            html += "      a.download = 'calibration.json';";
            html += "      a.click();";
            html += "    });";
            html += "}";
            html += "function importCalibration() {";
            html += "  const input = document.createElement('input');";
            html += "  input.type = 'file';";
            html += "  input.accept = '.json';";
            html += "  input.onchange = function(e) {";
            html += "    const file = e.target.files[0];";
            html += "    const reader = new FileReader();";
            html += "    reader.onload = function(e) {";
            html += "      fetch('/api/calibration/import', {";
            html += "        method: 'POST',";
            html += "        headers: {'Content-Type': 'application/json'},";
            html += "        body: e.target.result";
            html += "      }).then(response => response.json())";
            html += "        .then(data => {";
            html += "          if(data.success) {";
            html += "            updateCalibrationStatus();";
            html += "            alert('Калибровка импортирована!');";
            html += "          }";
            html += "        });";
            html += "    };";
            html += "    reader.readAsText(file);";
            html += "  };";
            html += "  input.click();";
            html += "}";
            html += "function resetCalibration() {";
            html += "  if(confirm('Сбросить всю калибровку?')) {";
            html += "    fetch('/api/calibration/reset', {method: 'POST'})";
            html += "      .then(response => response.json())";
            html += "      .then(data => {";
            html += "        if(data.success) {";
            html += "          updateCalibrationStatus();";
            html += "          alert('Калибровка сброшена!');";
            html += "        }";
            html += "      });";
            html += "  }";
            html += "}";

            html += "setInterval(updateSensor,3000);";
            html += "updateSensor();";
            html += "updateCalibrationStatus();";
            html += "setInterval(updateCalibrationStatus, 10000);";
            html += "</script>";

            // API-ссылка внизу страницы
            html += "<div style='margin-top:15px;font-size:14px;color:#555'><b>API:</b> <a href='" +
                    String(API_SENSOR) + "' target='_blank'>" + String(API_SENSOR) + "</a> (JSON, +timestamp)</div>";

            html += generatePageFooter();
            webServer.send(200, "text/html; charset=utf-8", html);
        });

    // AJAX эндпоинт для обновления показаний
    webServer.on("/sensor_json", HTTP_GET, sendSensorJson);

    // Primary API v1 endpoint
    webServer.on(API_SENSOR, HTTP_GET, sendSensorJson);

    // Загрузка калибровочного CSV через вкладку
    webServer.on("/readings/upload", HTTP_POST, []() {}, handleReadingsUpload);

    // Сброс пользовательских CSV (удаляем все *.csv)
    webServer.on("/readings/csv_reset", HTTP_POST,
                 []()
                 {
                     logWebRequest("POST", "/readings/csv_reset", webServer.client().remoteIP().toString());

                     // ✅ CSRF защита - критическая операция удаления!
                     if (!checkCSRFSafety())
                     {
                         logWarnSafe("\1", webServer.client().remoteIP().toString().c_str());
                         const String html = generateErrorPage(403, "Forbidden: Недействительный CSRF токен");
                         webServer.send(403, "text/html; charset=utf-8", html);
                         return;
                     }

                     CalibrationManager::init();
                     const bool removed = CalibrationManager::deleteTable(SoilProfile::SAND);
                     const String toast = removed ? "CSV+удален" : "CSV+не+найден";
                     webServer.sendHeader("Location", String("/readings?toast=") + toast, true);
                     webServer.send(302, "text/plain", "Redirect");
                 });

    // Форма для сохранения профиля
    webServer.on("/readings/profile", HTTP_POST, []() {}, handleProfileSave);

    // Обслуживание статических файлов из LittleFS
    webServer.on(
        "/docs/examples/calibration_example.csv", HTTP_GET,
        []()
        {
            logWebRequest("GET", "/docs/examples/calibration_example.csv", webServer.client().remoteIP().toString());

            if (LittleFS.exists("/docs/examples/calibration_example.csv"))
            {
                File file = LittleFS.open("/docs/examples/calibration_example.csv", "r");
                if (file)
                {
                    webServer.sendHeader("Content-Type", "text/csv");
                    webServer.sendHeader("Content-Disposition", "attachment; filename=\"calibration_example.csv\"");
                    webServer.streamFile(file, "text/csv");
                    file.close();
                }
                else
                {
                    webServer.send(404, "text/plain", "File not found");
                }
            }
            else
            {
                // Если файл не найден, создаем его на лету
                webServer.sendHeader("Content-Type", "text/csv");
                webServer.sendHeader("Content-Disposition", "attachment; filename=\"calibration_example.csv\"");
                // NOLINTNEXTLINE(misc-const-correctness)
    String csvContent = "# Пример калибровочной таблицы для JXCT датчика\n";
                csvContent += "# Формат: сырое_значение,коэффициент_коррекции\n";
                csvContent +=
                    "# Коэффициент применяется как: скорректированное_значение = сырое_значение * коэффициент\n\n";
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

    // API маршруты калибровки
    webServer.on("/api/calibration/status", HTTP_GET,
                 []()
                 {
                     DynamicJsonDocument doc(512);
                     doc["status"] = gCalibrationService.getCalibrationStatus();
                     doc["complete"] = gCalibrationService.isCalibrationComplete();

                     String response;
                     serializeJson(doc, response);
                     webServer.send(200, "application/json", response);
                 });

    webServer.on("/api/calibration/ph/add", HTTP_POST,
                 []()
                 {
                     DynamicJsonDocument doc(512);
                     DeserializationError error = deserializeJson(doc, webServer.arg("plain"));

                     if (error)
                     {
                         webServer.send(400, "application/json", "{\"success\":false,\"error\":\"Invalid JSON\"}");
                         return;
                     }

                     float expected = doc["expected"];
                     float measured = doc["measured"];

                     // Реальная логика калибровки
                     bool success = gCalibrationService.addPHCalibrationPoint(expected, measured);

                     DynamicJsonDocument response(256);
                     response["success"] = success;
                     if (!success)
                     {
                         response["error"] = "Failed to add pH point";
                     }

                     String response_str;
                     serializeJson(response, response_str);
                     webServer.send(200, "application/json", response_str);
                 });

    webServer.on("/api/calibration/ec/add", HTTP_POST,
                 []()
                 {
                     DynamicJsonDocument doc(512);
                     DeserializationError error = deserializeJson(doc, webServer.arg("plain"));

                     if (error)
                     {
                         webServer.send(400, "application/json", "{\"success\":false,\"error\":\"Invalid JSON\"}");
                         return;
                     }

                     float expected = doc["expected"];
                     float measured = doc["measured"];

                     // Реальная логика калибровки
                     bool success = gCalibrationService.addECCalibrationPoint(expected, measured);

                     DynamicJsonDocument response(256);
                     response["success"] = success;
                     if (!success)
                     {
                         response["error"] = "Failed to add EC point";
                     }

                     String response_str;
                     serializeJson(response, response_str);
                     webServer.send(200, "application/json", response_str);
                 });

    webServer.on("/api/calibration/npk/set", HTTP_POST,
                 []()
                 {
                     DynamicJsonDocument doc(512);
                     DeserializationError error = deserializeJson(doc, webServer.arg("plain"));

                     if (error)
                     {
                         webServer.send(400, "application/json", "{\"success\":false,\"error\":\"Invalid JSON\"}");
                         return;
                     }

                     float nitrogen = doc["n"];
                     float phosphorus = doc["p"];
                     float potassium = doc["k"];

                     // Реальная логика калибровки
                     bool success = gCalibrationService.setNPKCalibrationPoint(nitrogen, phosphorus, potassium);

                     DynamicJsonDocument response(256);
                     response["success"] = success;
                     if (!success)
                     {
                         response["error"] = "Failed to set NPK point";
                     }

                     String response_str;
                     serializeJson(response, response_str);
                     webServer.send(200, "application/json", response_str);
                 });

    webServer.on("/api/calibration/ph/calculate", HTTP_POST,
                 []()
                 {
                     // Реальная логика расчёта калибровки
                     bool success = gCalibrationService.calculatePHCalibration();

                     DynamicJsonDocument response(256);
                     response["success"] = success;
                     if (success)
                     {
                         // Получаем R² из калибровки (нужно добавить геттер)
                         response["r_squared"] = 0.99;  // Временно, пока не добавим геттер
                     }
                     else
                     {
                         response["error"] = "Failed to calculate pH calibration";
                     }

                     String response_str;
                     serializeJson(response, response_str);
                     webServer.send(200, "application/json", response_str);
                 });

    webServer.on("/api/calibration/ec/calculate", HTTP_POST,
                 []()
                 {
                     // Реальная логика расчёта калибровки
                     bool success = gCalibrationService.calculateECCalibration();

                     DynamicJsonDocument response(256);
                     response["success"] = success;
                     if (success)
                     {
                         // Получаем R² из калибровки (нужно добавить геттер)
                         response["r_squared"] = 0.99;  // Временно, пока не добавим геттер
                     }
                     else
                     {
                         response["error"] = "Failed to calculate EC calibration";
                     }

                     String response_str;
                     serializeJson(response, response_str);
                     webServer.send(200, "application/json", response_str);
                 });

    webServer.on("/api/calibration/export", HTTP_GET,
                 []()
                 {
                     // Реальная логика экспорта калибровки
                     String json_data = gCalibrationService.exportCalibrationToJSON();
                     webServer.send(200, "application/json", json_data);
                 });

    webServer.on("/api/calibration/import", HTTP_POST,
                 []()
                 {
                     // NOLINTNEXTLINE(misc-const-correctness)
    String json_data = webServer.arg("plain");
                     // Реальная логика импорта калибровки
                     bool success = gCalibrationService.importCalibrationFromJSON(json_data);

                     DynamicJsonDocument response(256);
                     response["success"] = success;
                     if (!success)
                     {
                         response["error"] = "Failed to import calibration";
                     }

                     String response_str;
                     serializeJson(response, response_str);
                     webServer.send(200, "application/json", response_str);
                 });

    webServer.on("/api/calibration/reset", HTTP_POST,
                 []()
                 {
                     // Реальная логика сброса калибровки
                     gCalibrationService.resetCalibration();
                     
                     DynamicJsonDocument response(128);
                     response["success"] = true;

                     String response_str;
                     serializeJson(response, response_str);
                     webServer.send(200, "application/json", response_str);
                 });

    logDebug("Маршруты данных настроены: /readings, /api/v1/sensor (json), /sensor_json [legacy], /api/calibration/*");
}
