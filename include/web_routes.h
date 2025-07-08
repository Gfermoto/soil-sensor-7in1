#pragma once

#include <WebServer.h>
#include "../src/wifi_manager.h"
#include "jxct_strings.h"
#include "logger.h"

// Внешние зависимости
extern WebServer webServer;

// ============================================================================
// CSRF ЗАЩИТА - БЕЗОПАСНАЯ РЕАЛИЗАЦИЯ
// ============================================================================

/**
 * @brief Генерация CSRF токена
 * @return Строка с уникальным CSRF токеном
 */
String generateCSRFToken();

/**
 * @brief Проверка CSRF токена
 * @param token Токен для проверки
 * @return true если токен валиден
 */
bool validateCSRFToken(const String& token);

/**
 * @brief Получение скрытого поля с CSRF токеном для форм
 * @return HTML строка с hidden input для CSRF токена
 */
String getCSRFHiddenField();

/**
 * @brief Middleware для проверки CSRF на POST запросах
 * @return true если запрос безопасен (GET или валидный CSRF токен)
 */
bool checkCSRFSafety();

/**
 * @brief Инициализация CSRF защиты
 */
void initCSRFProtection();

/**
 * @brief Преобразование HTTP метода в строку
 * @param method HTTP метод
 * @return Строковое представление метода
 */
String methodToString(HTTPMethod method);

// ============================================================================
// ОСНОВНЫЕ МАРШРУТЫ (routes_main.cpp)
// ============================================================================

/**
 * @brief Настройка основных маршрутов (/, /save, /status)
 */
void setupMainRoutes();

/**
 * @brief Обработчик главной страницы (уже существует в wifi_manager.cpp)
 */
void handleRoot();

/**
 * @brief Обработчик страницы статуса (уже существует в wifi_manager.cpp)
 */
void handleStatus();

// ============================================================================
// КОНФИГУРАЦИОННЫЕ МАРШРУТЫ (routes_config.cpp)
// ============================================================================

/**
 * @brief Настройка маршрутов для конфигурации
 */
void setupConfigRoutes();

/**
 * @brief Обработчик страницы настроек интервалов
 */
void handleIntervals();

/**
 * @brief Обработчик страницы сложных настроек (лог, алгоритм, временная зона)
 */
void handleAdvanced();

/**
 * @brief Обработчик API импорта конфигурации
 */
void handleApiConfigImport();

/**
 * @brief Обработчик API экспорта конфигурации
 */
void handleApiConfigExport();

// ============================================================================
// СЕРВИСНЫЕ МАРШРУТЫ (routes_service.cpp)
// ============================================================================

/**
 * @brief Настройка сервисных маршрутов (/reset, /status, /info)
 */
void setupServiceRoutes();

/**
 * @brief Обработчик сброса системы
 */
void handleReset();

/**
 * @brief Обработчик информации о системе
 */
void handleInfo();

/**
 * @brief Обработчик обновления статуса
 */
void handleUpdateStatus();

// ============================================================================
// МАРШРУТЫ ДАННЫХ (routes_data.cpp)
// ============================================================================

/**
 * @brief Настройка маршрутов для данных датчиков
 */
void setupDataRoutes();

/**
 * @brief Отправка JSON данных датчиков
 */
void sendSensorJson();

/**
 * @brief Обработчик главной страницы показаний
 */
void handleReadings();

/**
 * @brief Обработчик загрузки калибровочных файлов
 */
void handleReadingsUpload();

// ============================================================================
// КАЛИБРОВОЧНЫЕ МАРШРУТЫ (routes_calibration.cpp)
// ============================================================================

/**
 * @brief Настройка маршрутов калибровки
 */
void setupCalibrationRoutes();

/**
 * @brief Обработчик страницы калибровки
 */
void handleCalibration();

/**
 * @brief Обработчик загрузки калибровочных данных
 */
void handleCalibrationUpload();

// ============================================================================
// ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ ДЛЯ MIDDLEWARE
// ============================================================================

/**
 * @brief Проверка доступности маршрута в текущем режиме
 * @param uri URI запроса
 * @return true если маршрут доступен
 */
bool isRouteAvailable(const String& uri);

/**
 * @brief Проверка доступности функции в текущем режиме WiFi (без параметров)
 * @return true если функция доступна в текущем режиме
 */
bool isFeatureAvailable();

/**
 * @brief Логирование веб-запросов
 * @param method HTTP метод
 * @param uri URI запроса
 * @param clientIP IP клиента
 */
inline void logWebRequest(const String& method, const String& uri, const String& clientIP)
{
    // Логирование только важных запросов, исключаем служебные
    if (uri.startsWith("/sensor_json") || uri.startsWith(API_SENSOR))
    {
        // API запросы логируем на уровне DEBUG
        logDebugSafe("\1", method.c_str(), uri.c_str(), clientIP.c_str());
    }
    else
    {
        // Обычные запросы на уровне INFO
        logInfoSafe("\1", method.c_str(), uri.c_str(), clientIP.c_str());
    }
}

// ============================================================================
// ДОПОЛНИТЕЛЬНЫЕ ФУНКЦИИ ШАБЛОНОВ
// ============================================================================

/**
 * @brief Генерация формы с общими элементами
 */
String generateForm(const String& action, const String& method, const String& formContent, const String& buttonText,
                    const String& buttonIcon);

/**
 * @brief Генерация секции конфигурации
 */
String generateConfigSection(const String& title, const String& content, const String& helpText = "");

/**
 * @brief Генерация поля ввода
 */
String generateInputField(const String& id, const String& name, const String& label, const String& value,
                          const String& type = "text", bool required = false, const String& placeholder = "");

/**
 * @brief Генерация поля чекбокса
 */
String generateCheckboxField(const String& id, const String& name, const String& label, bool checked);

/**
 * @brief Генерация числового поля с валидацией
 */
String generateNumberField(const String& id, const String& name, const String& label, int value, int min, int max,
                           int step = 1);

/**
 * @brief Генерация сообщения об ошибке в форме
 */
String generateFormError(const String& message);

// ============================================================================
// ОБРАБОТКА ОШИБОК (error_handlers.cpp)
// ============================================================================

/**
 * @brief Настройка обработчиков ошибок (404, 500, и т.д.)
 */
void setupErrorHandlers();

/**
 * @brief Валидация входных данных конфигурации
 * @param checkRequired Проверять ли обязательные поля
 * @return true если валидация прошла успешно
 */
bool validateConfigInput(bool checkRequired = true);

/**
 * @brief Обработка ошибок загрузки файлов
 * @param error Описание ошибки
 */
void handleUploadError(const String& error);

// ============================================================================
// HTML ШАБЛОНЫ (web_templates.cpp)
// ============================================================================

/**
 * @brief Генерация заголовка HTML страницы
 * @param title Заголовок страницы
 * @param icon Иконка страницы (опционально)
 * @return HTML заголовок
 */
String generatePageHeader(const String& title, const String& icon = "");

/**
 * @brief Генерация футера HTML страницы
 * @return HTML футер
 */
String generatePageFooter();

/**
 * @brief Генерация страницы ошибки
 * @param errorCode Код ошибки
 * @param errorMessage Сообщение об ошибке
 * @return Полная HTML страница с ошибкой
 */
String generateErrorPage(int errorCode, const String& errorMessage);

/**
 * @brief Генерация страницы успеха
 * @param title Заголовок
 * @param message Сообщение об успехе
 * @param redirectUrl URL для перенаправления (опционально)
 * @param redirectDelay Задержка перенаправления в секундах (по умолчанию 2)
 * @return Полная HTML страница с сообщением об успехе
 */
String generateSuccessPage(const String& title, const String& message, const String& redirectUrl = "",
                           int redirectDelay = 2);

/**
 * @brief Генерация базовой HTML структуры с навигацией
 * @param title Заголовок страницы
 * @param content Содержимое страницы
 * @param icon Иконка страницы (опционально)
 * @return Полная HTML страница
 */
String generateBasePage(const String& title, const String& content, const String& icon = "");

// ============================================================================
// ОБРАБОТКА ОШИБОК (error_handlers.cpp)
// ============================================================================

/**
 * @brief Генерация HTML ответа с ошибкой валидации
 */
String generateValidationErrorResponse(const String& errorMsg);

/**
 * @brief Обработка критических ошибок сервера
 */
void handleCriticalError(const String& error);

// ============================================================================
// ДОПОЛНИТЕЛЬНЫЕ ФУНКЦИИ ШАБЛОНОВ
// ============================================================================

/**
 * @brief Генерация страницы "Недоступно в AP режиме"
 */
String generateApModeUnavailablePage(const String& title, const String& icon);

// ============================================================================
// ДОПОЛНИТЕЛЬНЫЕ МАРШРУТЫ
// ============================================================================

/**
 * @brief Настройка маршрутов OTA обновлений
 */
void setupOtaRoutes();

/**
 * @brief Настройка маршрутов отчетов
 */
void setupReportsRoutes();
