#include "web_routes.h"
#include "sensor_compensation.h"
#include "logger.h"
#include "jxct_ui_system.h"
#include "validation_utils.h"
#include "calibration_manager.h"
#include <vector>
#include <LittleFS.h>

// Экземпляр веб-сервера объявлен во внешнем модуле
extern WebServer webServer;

// ------------------------------
// HTML генераторы (простой MVP)
// ------------------------------

static String generateCalibrationPage()
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
    html += "<div class='section'><h2>Профиль почвы</h2>";
    html += "<div class='form-group'><label for='soil_profile'>Тип почвы:</label><select id='soil_profile' name='soil_profile'>";
    html += "<option value='sand'>Песок</option>";
    html += "<option value='loam'>Суглинок</option>";
    html += "<option value='peat'>Торф</option>";
    html += "</select></div></div>";

    // Загрузка CSV файла
    html += "<div class='section'><h2>Калибровочная таблица</h2>";
    html += "<div class='form-group'><label for='calibration_csv'>Файл CSV:</label><input type='file' id='calibration_csv' name='calibration_csv' accept='.csv' required></div></div>";

    html += generateButton(ButtonType::PRIMARY, UI_ICON_SAVE, "Сохранить", "");
    html += "</form>";

    html += "</div>";

    html += generatePageFooter();
    return html;
}

// ------------------------------
//    Обработчики маршрутов
// ------------------------------

static void handleCalibrationPage()
{
    webServer.send(200, "text/html; charset=utf-8", generateCalibrationPage());
}

// Буфер для загрузки файлов
static File uploadFile;
static SoilProfile uploadProfile = SoilProfile::SAND;

static void handleCalibrationUpload()
{
    HTTPUpload& upload = webServer.upload();
    if (upload.status == UPLOAD_FILE_START)
    {
        // Получаем профиль почвы из аргумента формы
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
        webServer.sendHeader("Location", "/calibration", true);
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
    webServer.on("/calibration/save", HTTP_POST, []() {
        // Пустой ack, фактическая обработка в uploadHandler
    }, handleCalibrationUpload);
} 