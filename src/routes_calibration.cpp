#include <LittleFS.h>
#include "calibration_manager.h"
#include "jxct_ui_system.h"
#include "logger.h"
#include "sensor_compensation.h"
#include "validation_utils.h"
#include "web/csrf_protection.h"  // 🔒 CSRF защита
#include "web_routes.h"

// Внешние зависимости (уже объявлены в заголовочных файлах)
// extern WebServer webServer;  // объявлено в web_routes.h
// extern String navHtml();  // объявлено в wifi_manager.h

// ------------------------------
// HTML генераторы (простой MVP)
// ------------------------------

namespace {
String generateCalibrationPage()
{
    String html;
    html += generatePageHeader("Калибровка", UI_ICON_CALIBRATION);

    html += "<div class='container'>";
    html += navHtml();
    html += "<h1>⚙️ Калибровка датчика</h1>";
    html += "<p>Здесь вы сможете выполнить калибровку датчика и настроить компенсацию показаний.";
    html += " В этой версии доступен выбор профиля почвы и загрузка калибровочных таблиц (CSV).</p>";

    // Форма выбора профиля почвы
    html += "<form action='/calibration/save' method='post' enctype='multipart/form-data'>";
    html += getCSRFHiddenField();  // Добавляем CSRF токен
    html += "<div class='section'><h2>Профиль почвы</h2>";
    html +=
        "<div class='form-group'><label for='soil_profile'>Тип почвы:</label><select id='soil_profile' "
        "name='soil_profile'>";
    html += "<option value='sand'>Песок</option>";
    html += "<option value='sandpeat'>Песчано-торфяной</option>";
    html += "<option value='loam'>Суглинок</option>";
    html += "<option value='peat'>Торф</option>";
    html += "<option value='clay'>Глина</option>";
    html += "</select></div></div>";

    // Загрузка CSV файла
    html += "<div class='section'><h2>Калибровочная таблица";
    // Статусы профилей
    const bool hasSand = CalibrationManager::hasTable(SoilProfile::SAND);
    const bool hasSandPeat = CalibrationManager::hasTable(SoilProfile::SANDPEAT);
    const bool hasLoam = CalibrationManager::hasTable(SoilProfile::LOAM);
    const bool hasClay = CalibrationManager::hasTable(SoilProfile::CLAY);
    const bool hasPeat = CalibrationManager::hasTable(SoilProfile::PEAT);
    html += " <span style='font-size:14px;color:#888'>(";
    html += "Песок:" + String(hasSand ? "✅" : "❌") + ", ";
    html += "Песч.-торф:" + String(hasSandPeat ? "✅" : "❌") + ", ";
    html += "Суглинок:" + String(hasLoam ? "✅" : "❌") + ", ";
    html += "Глина:" + String(hasClay ? "✅" : "❌") + ", ";
    html += "Торф:" + String(hasPeat ? "✅" : "❌") + ")</span></h2>";
    html +=
        "<div class='form-group'><label for='calibration_csv'>Файл CSV:</label><input type='file' id='calibration_csv' "
        "name='calibration_csv' accept='.csv' required></div></div>";

    html += generateButton(ButtonType::PRIMARY, ButtonConfig{UI_ICON_SAVE, "Сохранить", ""});
    html += "</form>";

    html += "</div>";

    html += generatePageFooter();
    return html;
}

// ------------------------------
//    Обработчики маршрутов
// ------------------------------

void handleCalibrationPage()
{
    String html = generateCalibrationPage();
    if (webServer.hasArg("ok"))
    {
        html.replace("<div class='container'>",
                     "<div class='container'><div class='msg msg-success'>✅ Калибровочная таблица сохранена</div>");
    }
    webServer.send(200, "text/html; charset=utf-8", html);
}

// Буфер для загрузки файлов
File uploadFile;
SoilProfile uploadProfile = SoilProfile::SAND;
}

void handleCalibrationUpload()  // ✅ Убираем static - функция extern в header
{
    HTTPUpload& upload = webServer.upload();
    if (upload.status == UPLOAD_FILE_START)
    {
        // Получаем профиль почвы из аргумента формы
        const String profileStr = webServer.arg("soil_profile");
        if (profileStr == "sand") { uploadProfile = SoilProfile::SAND; }
        else if (profileStr == "sandpeat") { uploadProfile = SoilProfile::SANDPEAT; }
        else if (profileStr == "loam") { uploadProfile = SoilProfile::LOAM; }
        else if (profileStr == "peat") { uploadProfile = SoilProfile::PEAT; }
        else if (profileStr == "clay") { uploadProfile = SoilProfile::CLAY; }

        CalibrationManager::init();
        const char* path = CalibrationManager::profileToFilename(uploadProfile);
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
        webServer.sendHeader("Location", "/calibration?ok=1", true);
        webServer.send(302, "text/plain", "Redirect");
    }
}

// ------------------------------
//       Основная функция
// ------------------------------

void setupCalibrationRoutes()
{
    logInfo("Маршруты калибровки инициализированы");
    CalibrationManager::init();

    webServer.on("/calibration", HTTP_GET, handleCalibrationPage);
    webServer.on(
        "/calibration/save", HTTP_POST,
        []()
        {
            // Пустой ack, фактическая обработка в uploadHandler
        },
        handleCalibrationUpload);
}
