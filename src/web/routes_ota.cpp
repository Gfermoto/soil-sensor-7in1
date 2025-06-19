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
#include <Arduino.h>

extern WebServer webServer;
extern WiFiMode currentWiFiMode;
extern String navHtml();

// Глобальные переменные для отслеживания прогресса локальной загрузки
static bool isLocalUploadActive = false;
static size_t localUploadProgress = 0;
static size_t localUploadTotal = 0;
static String localUploadStatus = "idle";

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

    // API: установить найденное обновление
    webServer.on("/api/ota/install", HTTP_POST, []()
                 {
                     logWebRequest("POST", "/api/ota/install", webServer.client().remoteIP().toString());
                     if (currentWiFiMode != WiFiMode::STA)
                     {
                         webServer.send(403, "application/json", "{\"error\":\"unavailable\"}");
                         return;
                     }
                     
                     // Запускаем принудительную установку
                     triggerOtaInstall();
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
                     html += "<div class='section'><div id='progressBar' style='display:none;width:100%;background:#f0f0f0;border-radius:4px;overflow:hidden;'>";
                     html += "<div id='progressFill' style='width:0%;height:20px;background:#4CAF50;transition:width 0.3s;'></div></div></div>";

                     // Форма локальной загрузки
                     html += "<div class='section'><h2>Локальное обновление</h2>";
                     html += "<form id='uploadForm' enctype='multipart/form-data'>";
                     html += "<input type='file' name='firmware' accept='.bin' required style='margin-bottom:10px'>";
                     html += generateButton(ButtonType::PRIMARY, "⬆️", "Загрузить", "") + "</form></div>";

                     html += "<script>\n";
                     html += "let isOtaActive = false;\n";
                     html += "let updateAvailable = false;\n";
                     html += "let updateUrl = '';\n";
                     html += "function updateStatus(){\n";
                     html += "  fetch('/api/ota/status').then(r=>r.json()).then(j=>{\n";
                     html += "    const status = j.status;\n";
                     html += "    document.getElementById('otaStatus').innerText = status;\n";
                     html += "    document.getElementById('auto_ota').checked = j.auto;\n";
                     html += "    \n";
                     html += "    // Показываем прогресс-бар для загрузки\n";
                     html += "    const progressBar = document.getElementById('progressBar');\n";
                     html += "    const progressFill = document.getElementById('progressFill');\n";
                     html += "    \n";
                     html += "    // Локальная загрузка\n";
                     html += "    if (j.localUpload) {\n";
                     html += "      progressBar.style.display = 'block';\n";
                     html += "      if (j.total > 0) {\n";
                     html += "        const percent = Math.round((j.progress * 100) / j.total);\n";
                     html += "        progressFill.style.width = percent + '%';\n";
                     html += "      } else {\n";
                     html += "        progressFill.style.width = '50%';\n";
                     html += "      }\n";
                     html += "      isOtaActive = true;\n";
                     html += "    }\n";
                     html += "    // Удалённая загрузка\n";
                     html += "    else if (status.startsWith('dl ') && status.includes('%')) {\n";
                     html += "      const percent = parseInt(status.match(/\\d+/)[0]);\n";
                     html += "      progressBar.style.display = 'block';\n";
                     html += "      progressFill.style.width = percent + '%';\n";
                     html += "      isOtaActive = true;\n";
                     html += "    } else if (status.startsWith('dl ') && status.includes('KB')) {\n";
                     html += "      progressBar.style.display = 'block';\n";
                     html += "      progressFill.style.width = '50%'; // неопределённый прогресс\n";
                     html += "      isOtaActive = true;\n";
                     html += "    } else if (status === 'connecting' || status === 'downloading' || status === 'verifying' || status === 'finalizing') {\n";
                     html += "      progressBar.style.display = 'block';\n";
                     html += "      progressFill.style.width = '25%';\n";
                     html += "      isOtaActive = true;\n";
                     html += "    } else {\n";
                     html += "      progressBar.style.display = 'none';\n";
                     html += "      if (isOtaActive && (status.startsWith('http ') || status.includes('error') || status.includes('timeout'))) {\n";
                     html += "        showToast('OTA ошибка: ' + status, 'error');\n";
                     html += "      } else if (isOtaActive && (status === 'success, rebooting' || status === 'success')) {\n";
                     html += "        showToast('OTA успешно! Устройство перезагружается...', 'success');\n";
                     html += "      } else if (status.startsWith('update available')) {\n";
                     html += "        updateAvailable = true;\n";
                     html += "        showToast('Найдено обновление! Нажмите \"Установить\"', 'info');\n";
                     html += "        // Показываем кнопку установки\n";
                     html += "        if (!document.getElementById('btnInstall')) {\n";
                     html += "          const installBtn = document.createElement('button');\n";
                     html += "          installBtn.id = 'btnInstall';\n";
                     html += "          installBtn.className = 'btn btn-success';\n";
                     html += "          installBtn.innerHTML = '⬇️ Установить';\n";
                     html += "          installBtn.onclick = installUpdate;\n";
                     html += "          document.getElementById('btnCheck').parentNode.appendChild(installBtn);\n";
                     html += "        }\n";
                     html += "      }\n";
                     html += "      isOtaActive = false;\n";
                     html += "    }\n";
                     html += "  }).catch(e=>console.log('Status fetch error:', e));\n";
                     html += "}\n";
                     html += "\n";
                     html += "function installUpdate() {\n";
                     html += "  if (!updateAvailable) return;\n";
                     html += "  document.getElementById('btnInstall').disabled = true;\n";
                     html += "  document.getElementById('btnInstall').innerText = 'Устанавливаем...';\n";
                     html += "  fetch('/api/ota/install', {method: 'POST'})\n";
                     html += "    .then(() => {\n";
                     html += "      showToast('Установка началась', 'info');\n";
                     html += "      isOtaActive = true;\n";
                     html += "    })\n";
                     html += "    .catch(e => {\n";
                     html += "      showToast('Ошибка установки', 'error');\n";
                     html += "      document.getElementById('btnInstall').disabled = false;\n";
                     html += "      document.getElementById('btnInstall').innerText = '⬇️ Установить';\n";
                     html += "    });\n";
                     html += "}\n";
                     html += "\n";
                     html += "// Обновляем статус каждую секунду во время OTA операций\n";
                     html += "setInterval(updateStatus, 1000);\n";
                     html += "updateStatus();\n";
                     html += "\n";
                     html += "document.getElementById('btnCheck').addEventListener('click',()=>{\n";
                     html += "  document.getElementById('btnCheck').disabled = true;\n";
                     html += "  document.getElementById('btnCheck').innerText = 'Проверяем...';\n";
                     html += "  fetch('/api/ota/check',{method:'POST'})\n";
                     html += "    .then(()=>{\n";
                     html += "      showToast('Проверка OTA запущена','info');\n";
                     html += "      isOtaActive = true;\n";
                     html += "    })\n";
                     html += "    .catch(e=>{\n";
                     html += "      showToast('Ошибка запуска проверки','error');\n";
                     html += "      console.error(e);\n";
                     html += "    })\n";
                     html += "    .finally(()=>{\n";
                     html += "      setTimeout(()=>{\n";
                     html += "        document.getElementById('btnCheck').disabled = false;\n";
                     html += "        document.getElementById('btnCheck').innerText = '🔍 Проверить';\n";
                     html += "      }, 3000);\n";
                     html += "    });\n";
                     html += "});\n";
                     html += "\n";
                     html += "document.getElementById('auto_ota').addEventListener('change',e=>{\n";
                     html += "  fetch('/api/ota/auto?enable='+(e.target.checked?1:0),{method:'POST'})\n";
                     html += "    .then(()=>showToast('Настройка сохранена','success'))\n";
                     html += "    .catch(e=>showToast('Ошибка сохранения','error'));\n";
                     html += "});\n";
                     html += "\n";
                     html += "document.getElementById('uploadForm').addEventListener('submit',e=>{\n";
                     html += "  e.preventDefault();\n";
                     html += "  const f=e.target.firmware.files[0];\n";
                     html += "  if(!f){showToast('Выберите файл','error');return;}\n";
                     html += "  const fd=new FormData();fd.append('firmware',f);\n";
                     html += "  showToast('Загрузка прошивки...','info');\n";
                     html += "  fetch('/ota/upload',{method:'POST',body:fd})\n";
                     html += "    .then(r=>r.json())\n";
                     html += "    .then(j=>{\n";
                     html += "      if(j.ok){\n";
                     html += "        showToast('Прошивка загружена, перезагрузка','success');\n";
                     html += "      }else{\n";
                     html += "        showToast('Ошибка загрузки: '+(j.error||'неизвестная'),'error');\n";
                     html += "      }\n";
                     html += "    })\n";
                     html += "    .catch(e=>{\n";
                     html += "      showToast('Ошибка соединения','error');\n";
                     html += "      console.error(e);\n";
                     html += "    });\n";
                     html += "});\n";
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
    StaticJsonDocument<256> doc;
    
    // Если идёт локальная загрузка, показываем её статус
    if (isLocalUploadActive)
    {
        if (localUploadTotal > 0)
        {
            int percent = (localUploadProgress * 100) / localUploadTotal;
            doc["status"] = "local " + String(percent) + "%";
        }
        else
        {
            doc["status"] = "local " + String(localUploadProgress / 1024) + "KB";
        }
        doc["localUpload"] = true;
        doc["progress"] = localUploadProgress;
        doc["total"] = localUploadTotal;
    }
    else
    {
        doc["status"] = getOtaStatus();
        doc["localUpload"] = false;
    }
    
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
        isLocalUploadActive = true;
        localUploadProgress = 0;
        localUploadTotal = upload.totalSize;
        localUploadStatus = "uploading";
        
        logSystem("[OTA] Приём файла %s (%u байт)", upload.filename.c_str(), upload.totalSize);
        
        if (!Update.begin(upload.totalSize == 0 ? UPDATE_SIZE_UNKNOWN : upload.totalSize))
        {
            logError("[OTA] Update.begin() failed");
            Update.printError(Serial);
            isLocalUploadActive = false;
            localUploadStatus = "error";
            // НЕ отправляем ответ здесь - это приведёт к обрыву загрузки
        }
    }
    else if (upload.status == UPLOAD_FILE_WRITE)
    {
        if (Update.write(upload.buf, upload.currentSize) != upload.currentSize)
        {
            logError("[OTA] Write error: %d байт", upload.currentSize);
            Update.printError(Serial);
            isLocalUploadActive = false;
            localUploadStatus = "error";
        }
        else
        {
            localUploadProgress += upload.currentSize;
            
            // Логируем прогресс каждые 64KB
            static size_t lastLogged = 0;
            if (localUploadProgress - lastLogged > 65536)
            {
                logSystem("[OTA] Загружено: %u байт", localUploadProgress);
                lastLogged = localUploadProgress;
            }
        }
    }
    else if (upload.status == UPLOAD_FILE_END)
    {
        logSystem("[OTA] Загрузка завершена: %u байт", localUploadProgress);
        localUploadStatus = "verifying";
        
        if (Update.end(true)) // true = устанавливать как boot partition
        {
            if (Update.isFinished())
            {
                logSuccess("[OTA] Файл принят успешно, перезагрузка через 2 сек");
                localUploadStatus = "success";
                isLocalUploadActive = false;
                webServer.send(200, "application/json", "{\"ok\":true}");
                delay(2000);
                ESP.restart();
            }
            else
            {
                logError("[OTA] Update не завершён");
                Update.printError(Serial);
                isLocalUploadActive = false;
                localUploadStatus = "error";
                webServer.send(200, "application/json", "{\"ok\":false,\"error\":\"not_finished\"}");
            }
        }
        else
        {
            logError("[OTA] Update.end() failed");
            Update.printError(Serial);
            isLocalUploadActive = false;
            localUploadStatus = "error";
            webServer.send(200, "application/json", "{\"ok\":false,\"error\":\"end_failed\"}");
        }
    }
    else if (upload.status == UPLOAD_FILE_ABORTED)
    {
        logError("[OTA] Загрузка прервана");
        Update.abort();
        isLocalUploadActive = false;
        localUploadStatus = "aborted";
        webServer.send(200, "application/json", "{\"ok\":false,\"error\":\"aborted\"}");
    }
} 