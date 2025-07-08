#include <Arduino.h>
#include <ArduinoJson.h>
#include <Update.h>
#include "../../include/jxct_config_vars.h"
#include "../../include/jxct_constants.h"
#include "../../include/jxct_strings.h"
#include "../../include/jxct_ui_system.h"
#include "../../include/logger.h"
#include "../../include/version.h"
#include "../../include/web_routes.h"
#include "../wifi_manager.h"
#include "ota_manager.h"

// Глобальные переменные для отслеживания прогресса локальной загрузки
namespace
{
bool isLocalUploadActive = false;
size_t localUploadProgress = 0;
size_t localUploadTotal = 0;
String localUploadStatus = "idle";

}  // namespace

// --- ВСПОМОГАТЕЛЬНЫЕ ---
// NOLINTNEXTLINE(misc-use-anonymous-namespace)
static void sendOtaStatusJson();
// NOLINTNEXTLINE(misc-use-anonymous-namespace)
static void handleFirmwareUpload();

void setupOtaRoutes()
{
    logDebug("Настройка OTA маршрутов");

    // API: статус OTA
    webServer.on("/api/ota/status", HTTP_GET, sendOtaStatusJson);

    // API: ручная проверка
    webServer.on("/api/ota/check", HTTP_POST,
                 []()
                 {
                     logWebRequest("POST", "/api/ota/check", webServer.client().remoteIP().toString());
                     if (currentWiFiMode != WiFiMode::STA)
                     {
                         webServer.send(HTTP_FORBIDDEN, HTTP_CONTENT_TYPE_JSON, "{\"error\":\"unavailable\"}");
                         return;
                     }
                     triggerOtaCheck();  // уже включает handleOTA()
                     webServer.send(HTTP_OK, HTTP_CONTENT_TYPE_JSON, "{\"ok\":true}");
                 });

    // API: установить найденное обновление
    webServer.on("/api/ota/install", HTTP_POST,
                 []()
                 {
                     logWebRequest("POST", "/api/ota/install", webServer.client().remoteIP().toString());
                     if (currentWiFiMode != WiFiMode::STA)
                     {
                         webServer.send(HTTP_FORBIDDEN, HTTP_CONTENT_TYPE_JSON, "{\"error\":\"unavailable\"}");
                         return;
                     }

                     // Запускаем принудительную установку
                     triggerOtaInstall();
                     webServer.send(HTTP_OK, HTTP_CONTENT_TYPE_JSON, "{\"ok\":true}");
                 });

    // HTML страница обновлений
    webServer.on(
        "/updates", HTTP_GET,
        []()
        {
            logWebRequest("GET", "/updates", webServer.client().remoteIP().toString());

            if (currentWiFiMode == WiFiMode::AP)
            {
                webServer.send(HTTP_OK, HTTP_CONTENT_TYPE_HTML, generateApModeUnavailablePage("Обновления", "🚀"));
                return;
            }

            String html = generatePageHeader("Обновления", "🚀");
            html += navHtml();
            html += "<h1>🚀 Обновления прошивки</h1>";

            // Информация о версии в красивом блоке
            html +=
                "<div class='info-block' "
                "style='background:#f8f9fa;padding:15px;border-radius:8px;margin-bottom:20px;'>";
            html += "<div style='display:flex;justify-content:space-between;align-items:center;'>";
            html += "<div><b>📱 Текущая версия:</b> " + String(JXCT_VERSION_STRING) + "</div>";
            html += "<div id='otaStatus' style='color:#666;font-style:italic;'>Загрузка статуса...</div>";
            html += "</div></div>";

            // Единый прогресс-бар для всех операций
            html += "<div id='progressContainer' style='display:none;margin-bottom:20px;'>";
            html += "<div style='margin-bottom:8px;'><span id='progressText'>Прогресс</span></div>";
            html += "<div style='width:100%;background:#e9ecef;border-radius:6px;overflow:hidden;height:24px;'>";
            html +=
                "<div id='progressFill' "
                "style='width:0%;height:100%;background:linear-gradient(90deg,#28a745,#20c997);transition:width "
                "0.3s;display:flex;align-items:center;justify-content:center;color:white;font-weight:bold;font-size:"
                "12px;'></div>";
            html += "</div></div>";

            // Блок удаленного обновления
            html +=
                "<div class='section' style='background:white;padding:20px;border-radius:8px;box-shadow:0 2px 4px "
                "rgba(0,0,0,0.1);margin-bottom:20px;'>";
            html += "<h3 style='margin-top:0;color:#495057;'>🌐 Обновление с сервера</h3>";

            // Кнопки проверки и установки
            html += "<div style='display:flex;gap:10px;flex-wrap:wrap;'>";
            {
                String btnCheck = generateButton(ButtonType::OUTLINE, ButtonConfig{"🔍", "Проверить обновления", ""});
                btnCheck.replace("<button ", "<button id='btnCheck' ");
                html += btnCheck;
            }
            html +=
                "<button id='btnInstall' style='display:none;' class='btn btn-success'>⬇️ Скачать и установить</button>";
            html += "</div></div>";

            // Блок локального обновления
            html +=
                "<div class='section' style='background:white;padding:20px;border-radius:8px;box-shadow:0 2px 4px "
                "rgba(0,0,0,0.1);'>";
            html += "<h3 style='margin-top:0;color:#495057;'>📁 Локальное обновление</h3>";
            html +=
                "<p style='color:#6c757d;margin-bottom:15px;'>Загрузите файл прошивки (.bin) с вашего компьютера</p>";
            html += "<div style='margin-bottom:15px;'>";
            html +=
                "<input type='file' name='firmware' accept='.bin' required style='width:100%;padding:10px;border:2px "
                "dashed #dee2e6;border-radius:6px;background:#f8f9fa;'>";
            html += "</div>";
            html += "<form id='uploadForm' enctype='multipart/form-data'>";
            {
                const String uploadBtn = generateButton(ButtonType::SECONDARY, ButtonConfig{"⬆️", "Загрузить прошивку", ""});
                html += uploadBtn;
            }
            html += "</form></div>";

            // JavaScript для управления интерфейсом
            html += "<script>\n";
            html += "let isOtaActive = false;\n";
            html += "let updateAvailable = false;\n";
            html += "\n";
            html += "function showProgress(text, percent) {\n";
            html += "  const container = document.getElementById('progressContainer');\n";
            html += "  const fill = document.getElementById('progressFill');\n";
            html += "  const textEl = document.getElementById('progressText');\n";
            html += "  container.style.display = 'block';\n";
            html += "  textEl.textContent = text;\n";
            html += "  fill.style.width = percent + '%';\n";
            html += "  fill.textContent = percent + '%';\n";
            html += "}\n";
            html += "\n";
            html += "function hideProgress() {\n";
            html += "  document.getElementById('progressContainer').style.display = 'none';\n";
            html += "}\n";
            html += "\n";
            html += "function updateStatus() {\n";
            html += "  fetch('/api/ota/status').then(r=>r.json()).then(j=>{\n";
            html += "    const status = j.status;\n";
            html += "    const statusEl = document.getElementById('otaStatus');\n";
            html += "    statusEl.textContent = status;\n";
            html += "    // Сброс цвета: по-умолчанию серый, перезаписываем только при ошибке\n";
            html += "    statusEl.style.color = '#666';\n";
            html += "    \n";
            html += "    // Обработка прогресса\n";
            html += "    if (j.localUpload) {\n";
            html += "      // Локальная загрузка\n";
            html += "      if (j.total > 0) {\n";
            html += "        const percent = Math.round((j.progress * 100) / j.total);\n";
            html +=
                "        showProgress('Загрузка файла: ' + Math.round(j.progress/1024) + ' КБ из ' + "
                "Math.round(j.total/1024) + ' КБ', percent);\n";
            html += "      } else {\n";
            html += "        showProgress('Загрузка файла: ' + Math.round(j.progress/1024) + ' КБ', 50);\n";
            html += "      }\n";
            html += "      isOtaActive = true;\n";
            html += "    } else if (status.includes('Загружено') && status.includes('%')) {\n";
            html += "      // Удаленная загрузка с процентами\n";
            html += R"(      const percent = parseInt(status.match(/\d+/)[0]);)" "\n";
            html += "      showProgress('Загрузка обновления', percent);\n";
            html += "      isOtaActive = true;\n";
            html += "    } else if (status.includes('Загружено') && status.includes('КБ')) {\n";
            html += "      // Удаленная загрузка без размера\n";
            html += "      showProgress('Загрузка обновления: ' + status, 50);\n";
            html += "      isOtaActive = true;\n";
            html +=
                "    } else if (['Подключение', 'Загрузка', 'Проверка', 'Завершение', 'Завершение установки', "
                "'Проверка обновлений'].includes(status)) {\n";
            html += "      // Этапы OTA\n";
            html += "      const stages = {'Подключение': " + String(OTA_STAGE_CONNECTION) +
                    ", 'Загрузка': " + String(OTA_STAGE_DOWNLOAD) + ", 'Проверка': " + String(OTA_STAGE_VERIFY) +
                    ", 'Завершение': " + String(OTA_STAGE_FINISH) +
                    ", "
                    "'Завершение установки': " +
                    String(OTA_STAGE_INSTALL) + ", 'Проверка обновлений': " + String(OTA_STAGE_CHECK) + "};\n";
            html += "      showProgress(status + '...', stages[status] || " + String(OTA_STAGE_DEFAULT) + ");\n";
            html += "      isOtaActive = true;\n";
            html += "    } else {\n";
            html += "      // Завершенные состояния\n";
            html += "      hideProgress();\n";
            html += "      \n";
            html += "      if (isOtaActive) {\n";
            html += "        if (status.includes('Ошибка') || status.includes('Таймаут')) {\n";
            html += "          showToast('❌ ' + status, 'error');\n";
            html +=
                "        } else if (status.includes('Успешно') || status.includes('завершено') || "
                "status.includes('✅')) {\n";
            html += "          showToast('✅ Обновление успешно завершено!', 'success');\n";
            html += "        } else if (status.includes('Перезагрузка') || status.includes('🔄')) {\n";
            html += "          showToast('🔄 Система перезагружается...', 'info');\n";
            html += "        }\n";
            html += "        isOtaActive = false;\n";
            html += "      }\n";
            html += "      \n";
            html += "      // Показ кнопки установки\n";
            html += "      const installBtn = document.getElementById('btnInstall');\n";
            html += "      if (status.includes('Доступно обновление')) {\n";
            html += "        updateAvailable = true;\n";
            html += "        installBtn.style.display = 'inline-block';\n";
            html += "        if (!installBtn.hasAttribute('data-shown')) {\n";
            html += "          showToast('🎉 Найдено обновление! Нажмите \"Установить\"', 'info');\n";
            html += "          installBtn.setAttribute('data-shown', 'true');\n";
            html += "        }\n";
            html += "      } else {\n";
            html += "        updateAvailable = false;\n";
            html += "        installBtn.style.display = 'none';\n";
            html += "        installBtn.removeAttribute('data-shown');\n";
            html += "      }\n";
            html += "    }\n";
            html += "  }).catch(e => {\n";
            html += "    document.getElementById('otaStatus').textContent = 'Ошибка связи';\n";
            html += "    document.getElementById('otaStatus').style.color = '#dc3545';\n";
            html += "  });\n";
            html += "}\n";
            html += "\n";
            html += "function installUpdate() {\n";
            html += "  if (!updateAvailable) return;\n";
            html += "  const btn = document.getElementById('btnInstall');\n";
            html += "  btn.disabled = true;\n";
            html += "  btn.textContent = '⏳ Устанавливаем...';\n";
            html += "  \n";
            html += "  fetch('/api/ota/install', {method: 'POST'})\n";
            html += "    .then(() => {\n";
            html += "      showToast('🚀 Установка началась', 'info');\n";
            html += "      isOtaActive = true;\n";
            html += "    })\n";
            html += "    .catch(e => {\n";
            html += "      showToast('❌ Ошибка установки', 'error');\n";
            html += "      btn.disabled = false;\n";
            html += "      btn.textContent = '⬇️ Скачать и установить';\n";
            html += "    });\n";
            html += "}\n";
            html += "\n";
            html += "// Обработчики событий\n";
            html += "document.getElementById('btnCheck').addEventListener('click', () => {\n";
            html += "  const btn = document.getElementById('btnCheck');\n";
            html += "  btn.disabled = true;\n";
            html += "  btn.textContent = '⏳ Проверяем...';\n";
            html += "  \n";
            html += "  fetch('/api/ota/check', {method: 'POST'})\n";
            html += "    .then(() => {\n";
            html += "      showToast('🔍 Проверка обновлений запущена', 'info');\n";
            html += "      isOtaActive = true;\n";
            html += "    })\n";
            html += "    .catch(e => {\n";
            html += "      showToast('❌ Ошибка запуска проверки', 'error');\n";
            html += "    })\n";
            html += "    .finally(() => {\n";
            html += "      setTimeout(() => {\n";
            html += "        btn.disabled = false;\n";
            html += "        btn.textContent = '🔍 Проверить обновления';\n";
            html += "      }, " + String(OTA_DELAY_MS) + ");\n";
            html += "    });\n";
            html += "});\n";
            html += "\n";
            html += "document.getElementById('btnInstall').addEventListener('click', installUpdate);\n";
            html += "\n";
            html += "document.getElementById('uploadForm').addEventListener('submit', e => {\n";
            html += "  e.preventDefault();\n";
            html += "  const fileInput = document.querySelector('input[name=\"firmware\"]');\n";
            html += "  const file = fileInput.files[0];\n";
            html += "  \n";
            html += "  if (!file) {\n";
            html += "    showToast('❌ Выберите файл прошивки', 'error');\n";
            html += "    return;\n";
            html += "  }\n";
            html += "  \n";
            html += "  if (!file.name.endsWith('.bin')) {\n";
            html += "    showToast('❌ Файл должен иметь расширение .bin', 'error');\n";
            html += "    return;\n";
            html += "  }\n";
            html += "  \n";
            html += "  const submitBtn = e.target.querySelector('button[type=submit]');\n";
            html += "  submitBtn.disabled = true;\n";
            html += "  submitBtn.textContent = '⏳ Загружаем...';\n";
            html += "  \n";
            html += "  const formData = new FormData();\n";
            html += "  formData.append('firmware', file);\n";
            html += "  \n";
            html += "  showToast('📤 Начинаем загрузку прошивки...', 'info');\n";
            html += "  isOtaActive = true;\n";
            html += "  \n";
            html += "  fetch('/ota/upload', {\n";
            html += "    method: 'POST',\n";
            html += "    body: formData\n";
            html += "  })\n";
            html += "  .then(r => r.json())\n";
            html += "  .then(j => {\n";
            html += "    if (j.ok) {\n";
            html += "      showToast('✅ Прошивка загружена успешно!', 'success');\n";
            html += "    } else {\n";
            html += "      showToast('❌ Ошибка: ' + (j.error || 'неизвестная ошибка'), 'error');\n";
            html += "    }\n";
            html += "  })\n";
            html += "  .catch(e => {\n";
            html += "    showToast('❌ Ошибка соединения', 'error');\n";
            html += "  })\n";
            html += "  .finally(() => {\n";
            html += "    submitBtn.disabled = false;\n";
            html += "    submitBtn.textContent = '⬆️ Загрузить прошивку';\n";
            html += "    fileInput.value = '';\n";
            html += "    isOtaActive = false;\n";
            html += "  });\n";
            html += "});\n";
            html += "\n";
            html += "// Автообновление статуса\n";
            html += "setInterval(updateStatus, " + String(OTA_UPDATE_INTERVAL_MS) + ");\n";
            html += "updateStatus();\n";
            html += "</script>";
            html += generatePageFooter();
            webServer.send(HTTP_OK, HTTP_CONTENT_TYPE_HTML, html);
        });

    // Upload маршрут
    webServer.on("/ota/upload", HTTP_POST, []() { /* ответ отправится в обработчике */ }, handleFirmwareUpload);

    logSuccess("Маршруты OTA настроены");
}

// NOLINTNEXTLINE(misc-use-anonymous-namespace)
static void sendOtaStatusJson()
{
    StaticJsonDocument<OTA_JSON_DOC_SIZE> doc;

    // Если идёт локальная загрузка, показываем её статус
    if (isLocalUploadActive)
    {
        if (localUploadTotal > 0)
        {
                            const size_t percent = (localUploadProgress * 100) / localUploadTotal;
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

    doc["version"] = JXCT_VERSION_STRING;
    String json;
    serializeJson(doc, json);
    webServer.send(HTTP_OK, HTTP_CONTENT_TYPE_JSON, json);
}

// NOLINTNEXTLINE(misc-use-anonymous-namespace)
static void handleFirmwareUpload()
{
    HTTPUpload& upload = webServer.upload();

    if (upload.status == UPLOAD_FILE_START)
    {
        isLocalUploadActive = true;
        localUploadProgress = 0;
        localUploadTotal = upload.totalSize;
        localUploadStatus = "uploading";

        logSystemSafe("\1", upload.filename.c_str(), upload.totalSize);

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
            logErrorSafe("\1", upload.currentSize);
            Update.printError(Serial);
            isLocalUploadActive = false;
            localUploadStatus = "error";
        }
        else
        {
            localUploadProgress += upload.currentSize;

            // Логируем прогресс каждые 64KB
            static size_t lastLogged = 0;
            if (localUploadProgress - lastLogged > OTA_PROGRESS_LOG_THRESHOLD)
            {
                logSystemSafe("\1", localUploadProgress);
                lastLogged = localUploadProgress;
            }
        }
    }
    else if (upload.status == UPLOAD_FILE_END)
    {
        logSystemSafe("\1", localUploadProgress);
        localUploadStatus = "verifying";

        if (Update.end(true))  // true = устанавливать как boot partition
        {
            if (Update.isFinished())
            {
                logSuccess("[OTA] Файл принят успешно, перезагрузка через 2 сек");
                localUploadStatus = "success";
                isLocalUploadActive = false;
                webServer.send(HTTP_OK, HTTP_CONTENT_TYPE_JSON, "{\"ok\":true}");
                delay(OTA_DELAY_MS);
                ESP.restart();
            }
            else
            {
                logError("[OTA] Update не завершён");
                Update.printError(Serial);
                isLocalUploadActive = false;
                localUploadStatus = "error";
                webServer.send(HTTP_OK, HTTP_CONTENT_TYPE_JSON, "{\"ok\":false,\"error\":\"not_finished\"}");
            }
        }
        else
        {
            logError("[OTA] Update.end() failed");
            Update.printError(Serial);
            isLocalUploadActive = false;
            localUploadStatus = "error";
            webServer.send(HTTP_OK, HTTP_CONTENT_TYPE_JSON, "{\"ok\":false,\"error\":\"end_failed\"}");
        }
    }
    else if (upload.status == UPLOAD_FILE_ABORTED)
    {
        logError("[OTA] Загрузка прервана");
        Update.abort();
        isLocalUploadActive = false;
        localUploadStatus = "aborted";
        webServer.send(HTTP_OK, HTTP_CONTENT_TYPE_JSON, "{\"ok\":false,\"error\":\"aborted\"}");
    }
}
