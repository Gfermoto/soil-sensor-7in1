#include "../../include/web_routes.h"
#include "../../include/logger.h"
#include "../../include/jxct_ui_system.h"
#include "../../include/jxct_config_vars.h"
#include "../../include/version.h"
#include "../../include/jxct_strings.h"
#include "../wifi_manager.h"
#include "../ota_manager.h"
#include <ArduinoJson.h>
#include <Update.h>
#include <ESP.h>

extern WebServer webServer;
extern WiFiMode currentWiFiMode;
extern String navHtml();

// --- ВСПОМОГАТЕЛЬНЫЕ ---
static void sendOtaStatusJson();
static void handleFirmwareUpload();

void setupOtaRoutes()
{
    logDebug("Настройка OTA маршрутов");

    // API: статус OTA
    webServer.on("/api/ota/status", HTTP_GET, sendOtaStatusJson);

    // API: ручная проверка
    webServer.on("/api/ota/check", HTTP_POST, []()
                 {
                     logWebRequest("POST", "/api/ota/check", webServer.client().remoteIP().toString());
                     if (currentWiFiMode != WiFiMode::STA)
                     {
                         webServer.send(403, "application/json", "{\"error\":\"unavailable\"}");
                         return;
                     }
                     triggerOtaCheck();
                     handleOTA();
                     webServer.send(200, "application/json", "{\"ok\":true}");
                 });

    // API: включить/выключить авто
    webServer.on("/api/ota/auto", HTTP_POST, []()
                 {
                     logWebRequest("POST", "/api/ota/auto", webServer.client().remoteIP().toString());
                     if (currentWiFiMode != WiFiMode::STA)
                     {
                         webServer.send(403, "application/json", "{\"error\":\"unavailable\"}");
                         return;
                     }
                     bool enable = webServer.arg("enable") == "1";
                     config.flags.autoOtaEnabled = enable ? 1 : 0;
                     saveConfig();
                     webServer.send(200, "application/json", "{\"ok\":true}");
                 });

    // HTML страница обновлений
    webServer.on("/updates", HTTP_GET, []()
                 {
                     logWebRequest("GET", "/updates", webServer.client().remoteIP().toString());

                     if (currentWiFiMode == WiFiMode::AP)
                     {
                         webServer.send(200, "text/html; charset=utf-8", generateApModeUnavailablePage("Обновления", "🚀"));
                         return;
                     }

                     String html = generatePageHeader("Обновления", "🚀");
                     html += navHtml();
                     html += "<h1>🚀 Обновления прошивки</h1>";

                     // Информация о версии
                     html += "<div class='info-block'><b>Текущая версия:</b> " + String(JXCT_VERSION_STRING) + "</div>";

                     // Авто-обновление чекбокс
                     String checked = config.flags.autoOtaEnabled ? " checked" : "";
                     html += "<div class='section'><label><input type='checkbox' id='auto_ota'" + checked + "> Авто-обновление</label></div>";

                     // Кнопка ручной проверки
                     {
                         String btn = generateButton(ButtonType::OUTLINE, "🔍", "Проверить", "");
                         btn.replace("<button ", "<button id='btnCheck' ");
                         html += "<div class='section'>" + btn + "</div>";
                     }

                     // Статус
                     html += "<div class='section'><b>Статус:</b> <span id='otaStatus'>" + String(getOtaStatus()) + "</span></div>";

                     // Форма локальной загрузки
                     html += "<div class='section'><h2>Локальное обновление</h2>";
                     html += "<form id='uploadForm' enctype='multipart/form-data'>";
                     html += "<input type='file' name='firmware' accept='.bin' required style='margin-bottom:10px'>";
                     html += generateButton(ButtonType::PRIMARY, "⬆️", "Загрузить", "") + "</form></div>";

                     html += "<script>\n";
                     html += "function fetchStatus(){fetch('/api/ota/status').then(r=>r.json()).then(j=>{document.getElementById('otaStatus').innerText=j.status;document.getElementById('auto_ota').checked=j.auto;});}setInterval(fetchStatus,3000);fetchStatus();\n";
                     html += "document.getElementById('btnCheck').addEventListener('click',()=>{fetch('/api/ota/check',{method:'POST'}).then(()=>showToast('Проверка начата','info'));});\n";
                     html += "document.getElementById('auto_ota').addEventListener('change',e=>{fetch('/api/ota/auto?enable='+(e.target.checked?1:0),{method:'POST'}).then(()=>showToast('Сохранено','success'));});\n";
                     html += "document.getElementById('uploadForm').addEventListener('submit',e=>{e.preventDefault();const f=e.target.firmware.files[0];if(!f){showToast('Выберите файл','error');return;}const fd=new FormData();fd.append('firmware',f);fetch('/ota/upload',{method:'POST',body:fd}).then(r=>r.json()).then(j=>{if(j.ok){showToast('Прошивка загружена, перезагрузка','success');}else{showToast('Ошибка '+(j.error||''),'error');}});});\n";
                     html += "</script>";
                     html += generatePageFooter();
                     webServer.send(200, "text/html; charset=utf-8", html);
                 });

    // Upload маршрут
    webServer.on("/ota/upload", HTTP_POST, []() { /* ответ отправится в обработчике */ }, handleFirmwareUpload);

    logSuccess("Маршруты OTA настроены");
}

static void sendOtaStatusJson()
{
    StaticJsonDocument<128> doc;
    doc["status"] = getOtaStatus();
    doc["auto"] = (bool)config.flags.autoOtaEnabled;
    doc["version"] = JXCT_VERSION_STRING;
    String json;
    serializeJson(doc, json);
    webServer.send(200, "application/json", json);
}

static void handleFirmwareUpload()
{
    HTTPUpload& upload = webServer.upload();
    if (upload.status == UPLOAD_FILE_START)
    {
        logSystem("[OTA] Приём файла %s (%u байт)", upload.filename.c_str(), upload.totalSize);
        triggerOtaCheck(); // для статуса
        if (!Update.begin(upload.totalSize == 0 ? UPDATE_SIZE_UNKNOWN : upload.totalSize))
        {
            logError("[OTA] Update.begin error (no space?)");
            webServer.send(200, "application/json", "{\"ok\":false,\"error\":\"begin\"}");
            return;
        }
    }
    else if (upload.status == UPLOAD_FILE_WRITE)
    {
        if (Update.write(upload.buf, upload.currentSize) != upload.currentSize)
        {
            logError("[OTA] Write error");
        }
    }
    else if (upload.status == UPLOAD_FILE_END)
    {
        if (Update.end() && Update.isFinished())
        {
            logSuccess("[OTA] Файл принят, перезагрузка");
            webServer.send(200, "application/json", "{\"ok\":true}");
            delay(1000);
            ESP.restart();
        }
        else
        {
            logError("[OTA] Update end error");
            webServer.send(200, "application/json", "{\"ok\":false,\"error\":\"update\"}");
        }
    }
} 