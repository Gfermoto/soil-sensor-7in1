#pragma once

#include <WebServer.h>
#include <WiFi.h>
#include "../src/wifi_manager.h"

// Внешние зависимости
extern WebServer webServer;

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
 * @brief Обработчик статуса (уже существует в wifi_manager.cpp)
 */
void handleStatus();

// ============================================================================
// ДАННЫЕ ДАТЧИКА (routes_data.cpp)
// ============================================================================

/**
 * @brief Настройка маршрутов данных датчика (/readings, /sensor_json, /api/sensor)
 */
void setupDataRoutes();

// Все обработчики данных реализованы как lambda-функции внутри setupDataRoutes()

// ============================================================================
// УПРАВЛЕНИЕ КОНФИГУРАЦИЕЙ (routes_config.cpp)
// ============================================================================

/**
 * @brief Настройка маршрутов конфигурации (/intervals, /config_manager, /api/config/*)
 */
void setupConfigRoutes();

// Все обработчики конфигурации реализованы как lambda-функции внутри setupConfigRoutes()

// ============================================================================
// СЕРВИСНЫЕ ФУНКЦИИ (routes_service.cpp)
// ============================================================================

/**
 * @brief Настройка сервисных маршрутов (/health, /service_status, /reset, /reboot, /ota)
 */
void setupServiceRoutes();

// Все сервисные обработчики реализованы как lambda-функции внутри setupServiceRoutes()

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
// УТИЛИТЫ
// ============================================================================

/**
 * @brief Проверка доступности функции в текущем режиме WiFi
 * @param feature Название функции для проверки
 * @return true если функция доступна в текущем режиме
 */
bool isFeatureAvailable(const String& feature);

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
void logWebRequest(const String& method, const String& uri, const String& clientIP);

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

/**
 * @brief Генерация страницы "Недоступно в AP режиме"
 */
String generateApModeUnavailablePage(const String& title, const String& icon);

// ============================================================================
// ДОПОЛНИТЕЛЬНЫЕ ФУНКЦИИ ОБРАБОТКИ ОШИБОК
// ============================================================================

/**
 * @brief Генерация HTML ответа с ошибкой валидации
 */
String generateValidationErrorResponse(const String& errorMsg);

/**
 * @brief Обработка критических ошибок сервера
 */
void handleCriticalError(const String& error);

/**
 * @brief Проверка доступности маршрута в текущем режиме
 */
bool isRouteAvailable(const String& uri);

/**
 * @brief Middleware для проверки доступности маршрута
 */
bool checkRouteAccess(const String& routeName, const String& icon);