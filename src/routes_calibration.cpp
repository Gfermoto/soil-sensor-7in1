#include "web_routes.h"
#include "sensor_compensation.h"
#include "logger.h"
#include "jxct_ui_system.h"
#include "validation_utils.h"

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

static void handleCalibrationSave()
{
    // TODO: обработка формы и загрузки CSV (будет реализовано на следующем этапе)
    logInfo("[CAL] Получены данные формы калибровки");
    webServer.sendHeader("Location", "/calibration", true);
    webServer.send(302, "text/plain", "Redirect");
}

// ------------------------------
//       Основная функция
// ------------------------------

void setupCalibrationRoutes()
{
    logInfo("Маршруты калибровки инициализированы");
    webServer.on("/calibration", HTTP_GET, handleCalibrationPage);
    webServer.on("/calibration/save", HTTP_POST, handleCalibrationSave);
} 