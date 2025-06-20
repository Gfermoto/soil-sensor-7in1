#include "ota_manager.h"
#include <Update.h>
#include "logger.h"
#include "jxct_config_vars.h"
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <mbedtls/sha256.h>
#include <esp_ota_ops.h>
#include <strings.h>
#include "version.h"
#include <Arduino.h>
#include <esp_task_wdt.h>

// Глобальные переменные для OTA 2.0
// КРИТИЧЕСКОЕ ИСПРАВЛЕНИЕ: Делаем буфер константным после инициализации
static char manifestUrlGlobal[512] = "";  // Увеличиваем размер буфера
static WiFiClient* clientPtr = nullptr;
static char statusBuf[64] = "Ожидание";
static bool urlInitialized = false;  // Флаг инициализации для защиты от перезаписи

// Переменные для двухэтапного OTA (проверка -> установка)
static bool updateAvailable = false;
static String pendingUpdateUrl = "";
static String pendingUpdateSha256 = "";
static String pendingUpdateVersion = "";

const char* getOtaStatus() { return statusBuf; }

void setupOTA(const char* manifestUrl, WiFiClient& client)
{
    // КРИТИЧЕСКАЯ ЗАЩИТА: Проверяем повторную инициализацию
    if (urlInitialized) {
        logWarn("[OTA] [SETUP DEBUG] ⚠️ OTA уже инициализирован, пропускаем повторную инициализацию");
        return;
    }
    
    // ДОБАВЛЕНО: Детальная диагностика инициализации
    logSystem("[OTA] [SETUP DEBUG] Инициализация OTA 2.0...");
    logSystem("[OTA] [SETUP DEBUG] Входные параметры:");
    logSystem("[OTA] [SETUP DEBUG]   manifestUrl: %s", manifestUrl ? manifestUrl : "NULL");
    logSystem("[OTA] [SETUP DEBUG]   client: %s", &client ? "OK" : "NULL");
    
    // КРИТИЧЕСКАЯ ПРОВЕРКА: Валидация входного URL
    if (!manifestUrl || strlen(manifestUrl) < 20 || strstr(manifestUrl, "github.com") == nullptr) {
        logError("[OTA] [SETUP DEBUG] ❌ Неверный URL манифеста!");
        return;
    }
    
    // КРИТИЧЕСКОЕ ИСПРАВЛЕНИЕ: Защищенное копирование с проверкой целостности
    memset(manifestUrlGlobal, 0, sizeof(manifestUrlGlobal));  // Очищаем буфер
    strlcpy(manifestUrlGlobal, manifestUrl, sizeof(manifestUrlGlobal));
    
    // ПРОВЕРКА ЦЕЛОСТНОСТИ после копирования
    if (strlen(manifestUrlGlobal) != strlen(manifestUrl) || 
        strstr(manifestUrlGlobal, "github.com") == nullptr) {
        logError("[OTA] [SETUP DEBUG] ❌ URL поврежден при копировании!");
        memset(manifestUrlGlobal, 0, sizeof(manifestUrlGlobal));
        return;
    }
    
    clientPtr = &client;
    strlcpy(statusBuf, "Готов", sizeof(statusBuf));
    urlInitialized = true;  // Защищаем от повторной инициализации
    
    // Сброс состояния обновлений
    updateAvailable = false;
    pendingUpdateUrl = "";
    pendingUpdateSha256 = "";
    pendingUpdateVersion = "";
    
    logSystem("[OTA] [SETUP DEBUG] Глобальные переменные установлены:");
    logSystem("[OTA] [SETUP DEBUG]   manifestUrlGlobal: %s", manifestUrlGlobal);
    logSystem("[OTA] [SETUP DEBUG]   clientPtr: %p", clientPtr);
    logSystem("[OTA] [SETUP DEBUG]   statusBuf: '%s'", statusBuf);
    logSystem("[OTA] [SETUP DEBUG]   urlInitialized: %s", urlInitialized ? "ДА" : "НЕТ");
    
    logSuccess("[OTA] [SETUP DEBUG] ✅ OTA инициализирован успешно с защитой памяти");
}

static bool verifySha256(const uint8_t* calcDigest, const char* expectedHex)
{
    char calcHex[65];
    for (int i = 0; i < 32; ++i)
        sprintf(&calcHex[i * 2], "%02x", calcDigest[i]);
    return strcasecmp(calcHex, expectedHex) == 0;
}

// Вспомогательная функция для инициализации загрузки
static bool initializeDownload(HTTPClient& http, const String& binUrl, int& contentLen)
{
    esp_task_wdt_reset();
    strcpy(statusBuf, "Подключение");
    
    // КРИТИЧЕСКОЕ ИСПРАВЛЕНИЕ: Проверяем память перед началом
    size_t freeHeap = ESP.getFreeHeap();
    logSystem("[OTA] Свободная память перед HTTP: %d байт", freeHeap);
    
    // УВЕЛИЧИВАЕМ ТРЕБОВАНИЯ К ПАМЯТИ для безопасности
    if (freeHeap < 70000) {
        strcpy(statusBuf, "Мало памяти");
        logError("[OTA] Недостаточно памяти для HTTP: %d байт", freeHeap);
        return false;
    }
    
    // ИСПРАВЛЕНО: Защита от повреждения памяти - копируем URL в статический буфер
    static char urlBuffer[256];
    strlcpy(urlBuffer, binUrl.c_str(), sizeof(urlBuffer));
    logSystem("[OTA] Загрузка: %s", urlBuffer);
    
    // ДОПОЛНИТЕЛЬНАЯ ЗАЩИТА: Проверяем целостность URL
    if (strlen(urlBuffer) < 10 || strstr(urlBuffer, "github.com") == nullptr) {
        strcpy(statusBuf, "Поврежденный URL");
        logError("[OTA] URL поврежден или некорректен: %s", urlBuffer);
        return false;
    }

    // ИСПРАВЛЕНО: Добавляем проверку инициализации HTTP
    logSystem("[OTA] Инициализация HTTP клиента...");
    if (!http.begin(*clientPtr, binUrl)) {
        strcpy(statusBuf, "Ошибка HTTP init");
        logError("[OTA] Не удалось инициализировать HTTP клиент");
        return false;
    }
    
    http.setTimeout(60000);  // Увеличиваем таймаут до 60 секунд
    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
    
    logSystem("[OTA] Выполняем HTTP GET запрос...");
    esp_task_wdt_reset();
    
    int code = http.GET();
    esp_task_wdt_reset();
    
    logSystem("[OTA] HTTP ответ: %d", code);
    
    if (code != HTTP_CODE_OK)
    {
        snprintf(statusBuf, sizeof(statusBuf), "Ошибка HTTP %d", code);
        logError("[OTA] HTTP ошибка %d", code);
        return false;
    }

    contentLen = http.getSize();
    if (contentLen == -1)
    {
        logSystem("[OTA] Chunked transfer, размер неизвестен");
        contentLen = UPDATE_SIZE_UNKNOWN;
    }
    else
    {
        logSystem("[OTA] Размер файла: %d байт", contentLen);
    }

    if (!Update.begin(contentLen))
    {
        strcpy(statusBuf, "Нет места");
        logError("[OTA] Update.begin() failed");
        Update.printError(Serial);
        return false;
    }

    return true;
}

// Вспомогательная функция для загрузки данных
static bool downloadData(HTTPClient& http, int contentLen, mbedtls_sha256_context& shaCtx)
{
    strcpy(statusBuf, "Загрузка");
    
    // КРИТИЧЕСКОЕ ИСПРАВЛЕНИЕ: Проверяем память перед загрузкой
    size_t heapBeforeDownload = ESP.getFreeHeap();
    logSystem("[OTA] Память перед загрузкой: %d байт", heapBeforeDownload);
    
    // УВЕЛИЧИВАЕМ ТРЕБОВАНИЯ К ПАМЯТИ для безопасности
    if (heapBeforeDownload < 60000) {
        strcpy(statusBuf, "Мало памяти для загрузки");
        logError("[OTA] Недостаточно памяти для загрузки: %d байт", heapBeforeDownload);
        return false;
    }
    
    WiFiClient* stream = http.getStreamPtr();
    if (!stream) {
        strcpy(statusBuf, "Ошибка потока");
        logError("[OTA] Не удалось получить поток данных");
        return false;
    }
    
    // ИСПРАВЛЕНО: Оптимальный размер буфера для стабильной загрузки
    uint8_t buf[512];  // Увеличиваем буфер для более быстрой загрузки
    size_t totalDownloaded = 0;
    unsigned long lastProgress = millis();
    unsigned long lastActivity = millis();
    const unsigned long TIMEOUT_MS = 60000;  // Увеличиваем таймаут загрузки до 60 секунд
    bool isChunked = (contentLen == UPDATE_SIZE_UNKNOWN);

    while (http.connected())
    {
        esp_task_wdt_reset();
        
        size_t avail = stream->available();
        if (avail > 0)
        {
            lastActivity = millis();
            size_t toRead = (avail > sizeof(buf)) ? sizeof(buf) : avail;
            int readBytes = stream->readBytes(buf, toRead);
            
            if (readBytes <= 0)
            {
                strcpy(statusBuf, "Ошибка чтения");
                logError("[OTA] Ошибка чтения данных");
                Update.abort();
                return false;
            }

            if (Update.write(buf, readBytes) != (size_t)readBytes)
            {
                strcpy(statusBuf, "Ошибка записи");
                logError("[OTA] Ошибка записи во flash");
                Update.printError(Serial);
                Update.abort();
                return false;
            }

            mbedtls_sha256_update_ret(&shaCtx, buf, readBytes);
            totalDownloaded += readBytes;

            // Прогресс каждые 3 секунды
            if (millis() - lastProgress > 3000)
            {
                if (isChunked)
                {
                    snprintf(statusBuf, sizeof(statusBuf), "Загружено %dКБ", (int)(totalDownloaded / 1024));
                }
                else
                {
                    int percent = (totalDownloaded * 100) / contentLen;
                    snprintf(statusBuf, sizeof(statusBuf), "Загружено %d%%", percent);
                }
                logSystem("[OTA] Загружено: %d байт", totalDownloaded);
                lastProgress = millis();
            }
        }
        else
        {
            if (millis() - lastActivity > TIMEOUT_MS)
            {
                strcpy(statusBuf, "Таймаут");
                logError("[OTA] Таймаут загрузки (нет данных %lu мс)", TIMEOUT_MS);
                Update.abort();
                return false;
            }
            
            if (isChunked && !http.connected())
            {
                logSystem("[OTA] Chunked transfer завершён, загружено %d байт", totalDownloaded);
                break;
            }
            
            esp_task_wdt_reset();
            delay(10);
        }
    }

    if (!isChunked && totalDownloaded != (size_t)contentLen)
    {
        snprintf(statusBuf, sizeof(statusBuf), "Неполная загрузка %d/%d", totalDownloaded, contentLen);
        logError("[OTA] Неполная загрузка: %d из %d байт", totalDownloaded, contentLen);
        Update.abort();
        return false;
    }

    return true;
}

// Основная функция загрузки и обновления (упрощенная)
static bool downloadAndUpdate(const String& binUrl, const char* expectedSha256)
{
    logSystem("[OTA] Начинаем загрузку и обновление");
    
    // КРИТИЧЕСКОЕ ИСПРАВЛЕНИЕ: Проверяем общее состояние системы
    size_t initialHeap = ESP.getFreeHeap();
    logSystem("[OTA] Начальная память: %d байт", initialHeap);
    
    // УВЕЛИЧИВАЕМ ТРЕБОВАНИЯ К ПАМЯТИ для безопасности
    if (initialHeap < 80000) {
        strcpy(statusBuf, "Критически мало памяти");
        logError("[OTA] Критически мало памяти: %d байт", initialHeap);
        return false;
    }
    
    // ИСПРАВЛЕНО: Создаем HTTP клиент в куче для экономии стека
    HTTPClient* http = new HTTPClient();
    if (!http) {
        strcpy(statusBuf, "Ошибка создания HTTP клиента");
        logError("[OTA] Не удалось создать HTTP клиент");
        return false;
    }
    
    int contentLen;
    
    // Инициализация загрузки с дополнительными проверками
    logSystem("[OTA] Инициализация загрузки...");
    if (!initializeDownload(*http, binUrl, contentLen))
    {
        logError("[OTA] Ошибка инициализации загрузки");
        http->end();
        delete http;
        return false;
    }
    
    logSystem("[OTA] Инициализация успешна, размер контента: %d", contentLen);
    
    // ИСПРАВЛЕНО: Создаем SHA256 контекст в куче
    mbedtls_sha256_context* shaCtx = new mbedtls_sha256_context();
    if (!shaCtx) {
        strcpy(statusBuf, "Ошибка создания SHA256 контекста");
        logError("[OTA] Не удалось создать SHA256 контекст");
        http->end();
        delete http;
        return false;
    }
    
    mbedtls_sha256_init(shaCtx);
    mbedtls_sha256_starts_ret(shaCtx, 0);
    
    // Загрузка данных
    bool downloadSuccess = downloadData(*http, contentLen, *shaCtx);
    http->end();
    delete http;
    
    if (!downloadSuccess)
    {
        mbedtls_sha256_free(shaCtx);
        delete shaCtx;
        return false;
    }

    // Проверка SHA256
    strcpy(statusBuf, "Проверка");
    uint8_t digest[32];
    mbedtls_sha256_finish_ret(shaCtx, digest);
    mbedtls_sha256_free(shaCtx);
    delete shaCtx;

    // Проверяем SHA256
    if (!verifySha256(digest, expectedSha256))
    {
        strcpy(statusBuf, "Неверная контрольная сумма");
        logError("[OTA] SHA256 не совпадает");
        Update.abort();
        return false;
    }

    // Завершение обновления
    strcpy(statusBuf, "Завершение");
    if (!Update.end(true))
    {
        strcpy(statusBuf, "Ошибка завершения");
        logError("[OTA] Update.end() failed");
        Update.printError(Serial);
        return false;
    }

    strcpy(statusBuf, "Перезагрузка");
    logSystem("[OTA] Обновление успешно завершено. Перезагрузка через 3 секунды...");
    delay(3000);
    ESP.restart();
    return true;
}

// Принудительная проверка OTA (игнорирует таймер)
void triggerOtaCheck()
{
    static bool isChecking = false;
    
    if (isChecking) {
        logWarn("[OTA] Проверка уже выполняется, пропускаем");
        return;
    }
    
    isChecking = true;
    logSystem("[OTA] Принудительная проверка OTA запущена");
    handleOTA();
    isChecking = false;
}

// Принудительная установка найденного обновления
void triggerOtaInstall()
{
    if (!updateAvailable || pendingUpdateUrl.isEmpty())
    {
        logError("[OTA] Нет доступных обновлений для установки");
        strcpy(statusBuf, "Нет обновлений");
        return;
    }
    
    logSystem("[OTA] Принудительная установка обновления %s", pendingUpdateVersion.c_str());
    logSystem("[OTA] URL: %s", pendingUpdateUrl.c_str());
    logSystem("[OTA] SHA256: %.16s...", pendingUpdateSha256.c_str());
    
    bool result = downloadAndUpdate(pendingUpdateUrl, pendingUpdateSha256.c_str());
    if (!result)
    {
        logError("[OTA] Установка обновления не удалась");
        strcpy(statusBuf, "Ошибка установки");
        // Сбрасываем информацию об обновлении при ошибке
        updateAvailable = false;
        pendingUpdateUrl = "";
        pendingUpdateSha256 = "";
        pendingUpdateVersion = "";
    }
}

void handleOTA()
{
    // ДОБАВЛЕНО: Детальная диагностика для отладки
    static unsigned long debugCallCount = 0;
    debugCallCount++;
    
    // Сброс watchdog перед началом проверки
    esp_task_wdt_reset();
    
    // КРИТИЧЕСКАЯ ПРОВЕРКА: Проверяем инициализацию и целостность URL
    if (!urlInitialized || strlen(manifestUrlGlobal) == 0) {
        logError("[OTA] [DEBUG] OTA не инициализирован или URL пуст - выходим");
        return;
    }
    
    // КРИТИЧЕСКАЯ ПРОВЕРКА: Проверяем целостность URL перед использованием
    if (strstr(manifestUrlGlobal, "github.com") == nullptr) {
        logError("[OTA] [DEBUG] ❌ URL поврежден в памяти: %s", manifestUrlGlobal);
        logError("[OTA] [DEBUG] Переинициализируем OTA...");
        urlInitialized = false;  // Сбрасываем флаг для переинициализации
        return;
    }
    
    logSystem("[OTA] [DEBUG] handleOTA() вызов #%lu, URL проверен: %.50s...", 
              debugCallCount, manifestUrlGlobal);

    if (!clientPtr)
    {
        logError("[OTA] [DEBUG] clientPtr не задан - выходим");
        return;
    }

    logSystem("[OTA] [DEBUG] Начинаем проверку обновлений...");
    logSystem("[OTA] [DEBUG] URL манифеста: %s", manifestUrlGlobal);
    strcpy(statusBuf, "Проверка обновлений");

    HTTPClient http;
    logSystem("[OTA] [DEBUG] Инициализируем HTTP клиент...");
    http.begin(*clientPtr, manifestUrlGlobal);
    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
    http.setTimeout(15000); // 15 секунд таймаут
    
    logSystem("[OTA] [DEBUG] Отправляем GET запрос...");
    int code = http.GET();
    esp_task_wdt_reset();
    
    logSystem("[OTA] [DEBUG] HTTP ответ: %d", code);
    
    if (code != HTTP_CODE_OK)
    {
        snprintf(statusBuf, sizeof(statusBuf), "Ошибка манифеста %d", code);
        logError("[OTA] [DEBUG] Ошибка загрузки манифеста: HTTP %d", code);
        
        // Дополнительная диагностика для популярных ошибок
        if (code == HTTP_CODE_NOT_FOUND) {
            logError("[OTA] [DEBUG] 404 - файл манифеста не найден на сервере");
        } else if (code == HTTP_CODE_MOVED_PERMANENTLY || code == HTTP_CODE_FOUND) {
            logError("[OTA] [DEBUG] %d - редирект, но followRedirects включен", code);
        } else if (code == -1) {
            logError("[OTA] [DEBUG] -1 - ошибка подключения/DNS");
        } else if (code == -11) {
            logError("[OTA] [DEBUG] -11 - таймаут подключения");
        }
        
        http.end();
        return;
    }

    String manifestContent = http.getString();
    int contentLength = manifestContent.length();
    http.end();
    
    logSystem("[OTA] [DEBUG] Манифест получен: %d байт", contentLength);
    
    // Показываем первые 200 символов для диагностики
    String preview = manifestContent.substring(0, min(200, contentLength));
    logSystem("[OTA] [DEBUG] Начало манифеста: '%s'%s", 
              preview.c_str(), contentLength > 200 ? "..." : "");

    // Проверяем что это JSON
    if (!manifestContent.startsWith("{")) {
        logError("[OTA] [DEBUG] Манифест не начинается с '{' - возможно HTML ошибка");
        strcpy(statusBuf, "Неверный формат");
        return;
    }

    const size_t capacity = JSON_OBJECT_SIZE(3) + 300; // Увеличиваем буфер
    StaticJsonDocument<capacity> doc;
    DeserializationError err = deserializeJson(doc, manifestContent);
    if (err)
    {
        strcpy(statusBuf, "Ошибка JSON");
        logError("[OTA] [DEBUG] Ошибка парсинга JSON: %s", err.c_str());
        logError("[OTA] [DEBUG] JSON содержимое: '%s'", manifestContent.c_str());
        return;
    }

    const char* newVersion = doc["version"] | "";
    const char* binUrl = doc["url"] | "";
    const char* sha256 = doc["sha256"] | "";

    logSystem("[OTA] [DEBUG] Парсинг JSON успешен:");
    logSystem("[OTA] [DEBUG]   version: '%s'", newVersion);
    logSystem("[OTA] [DEBUG]   url: '%s'", binUrl);
    logSystem("[OTA] [DEBUG]   sha256: '%s'", sha256);
    logSystem("[OTA] [DEBUG]   текущая версия: '%s'", JXCT_VERSION_STRING);

    // Детальная валидация полей
    if (strlen(newVersion) == 0) {
        logError("[OTA] [DEBUG] Поле 'version' пустое или отсутствует");
        strcpy(statusBuf, "Нет версии в манифесте");
        return;
    }
    if (strlen(binUrl) == 0) {
        logError("[OTA] [DEBUG] Поле 'url' пустое или отсутствует");
        strcpy(statusBuf, "Нет URL в манифесте");
        return;
    }
    if (strlen(sha256) != 64) {
        logError("[OTA] [DEBUG] Поле 'sha256' неверной длины: %d (ожидается 64)", strlen(sha256));
        strcpy(statusBuf, "Неверная подпись");
        return;
    }

    // Проверка версий
    logSystem("[OTA] [DEBUG] Сравниваем версии: '%s' vs '%s'", newVersion, JXCT_VERSION_STRING);
    
    if (strcmp(newVersion, JXCT_VERSION_STRING) == 0)
    {
        strcpy(statusBuf, "Актуальная версия");
        updateAvailable = false;
        pendingUpdateUrl = "";
        pendingUpdateSha256 = "";
        pendingUpdateVersion = "";
        logSystem("[OTA] [DEBUG] Версии совпадают - обновление не требуется");
        return;
    }

    // Сохраняем информацию об обновлении
    updateAvailable = true;
    pendingUpdateUrl = String(binUrl);
    pendingUpdateSha256 = String(sha256);
    pendingUpdateVersion = String(newVersion);
    
    snprintf(statusBuf, sizeof(statusBuf), "Доступно обновление: %s", newVersion);
    logSystem("[OTA] [DEBUG] ✅ ОБНОВЛЕНИЕ НАЙДЕНО!");
    logSystem("[OTA] [DEBUG]   Текущая: %s", JXCT_VERSION_STRING);
    logSystem("[OTA] [DEBUG]   Доступна: %s", newVersion);
    logSystem("[OTA] [DEBUG]   URL: %s", binUrl);
    logSystem("[OTA] [DEBUG]   SHA256: %.16s...", sha256);
    logSystem("[OTA] [DEBUG] Ожидаем подтверждения установки через веб-интерфейс");
} 