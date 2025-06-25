#pragma once

/**
 * @file validation_utils.h
 * @brief Заголовочный файл утилит валидации
 * @details Объявления функций валидации конфигурации и данных датчика
 */

#include <Arduino.h>
#include <vector>

// ============================================================================
// СТРУКТУРЫ РЕЗУЛЬТАТОВ ВАЛИДАЦИИ
// ============================================================================

/**
 * @brief Результат валидации одного поля
 */
struct ValidationResult
{
    bool isValid;
    String message;
};

/**
 * @brief Ошибка валидации с указанием поля
 */
struct ValidationError
{
    String field;
    String message;
};

/**
 * @brief Результат валидации конфигурации
 */
struct ConfigValidationResult
{
    bool isValid;
    std::vector<ValidationError> errors;
};

/**
 * @brief Результат валидации данных датчика
 */
struct SensorValidationResult
{
    bool isValid;
    std::vector<ValidationError> errors;
};

// ============================================================================
// СТРУКТУРЫ ДАННЫХ (forward declarations)
// ============================================================================

// Упрощенная структура конфигурации для валидации
struct ConfigData
{
    String ssid;
    String password;
    bool mqttEnabled;
    String mqttServer;
    int mqttPort;
    bool thingSpeakEnabled;
    String thingSpeakAPIKey;
    unsigned long sensorReadInterval;
    unsigned long mqttPublishInterval;
    unsigned long thingspeakInterval;
    unsigned long ntpUpdateInterval;
};

// Упрощенная структура данных датчика для валидации
struct SensorData
{
    float temperature;
    float humidity;
    float ph;
    float ec;          // Исправлено: должно быть float
    float nitrogen;    // Исправлено: должно быть float
    float phosphorus;  // Исправлено: должно быть float
    float potassium;   // Исправлено: должно быть float
};

// ============================================================================
// ФУНКЦИИ ВАЛИДАЦИИ КОНФИГУРАЦИИ
// ============================================================================

/**
 * @brief Валидация SSID
 * @param ssid SSID для проверки
 * @return Результат валидации
 */
ValidationResult validateSSID(const String& ssid);

/**
 * @brief Валидация пароля WiFi
 * @param password Пароль для проверки
 * @return Результат валидации
 */
ValidationResult validatePassword(const String& password);

/**
 * @brief Валидация MQTT сервера
 * @param server Адрес сервера для проверки
 * @return Результат валидации
 */
ValidationResult validateMQTTServer(const String& server);

/**
 * @brief Валидация MQTT порта
 * @param port Порт для проверки
 * @return Результат валидации
 */
ValidationResult validateMQTTPort(int port);

/**
 * @brief Валидация ThingSpeak API ключа
 * @param apiKey API ключ для проверки
 * @return Результат валидации
 */
ValidationResult validateThingSpeakAPIKey(const String& apiKey);

/**
 * @brief Валидация интервала (общая функция)
 * @param interval Интервал для проверки
 * @param min_val Минимальное значение
 * @param max_val Максимальное значение
 * @param name Название интервала для сообщения об ошибке
 * @return Результат валидации
 */
ValidationResult validateInterval(unsigned long interval, unsigned long min_val, unsigned long max_val,
                                  const char* name);

/**
 * @brief Валидация интервала чтения датчика
 * @param interval Интервал для проверки
 * @return Результат валидации
 */
ValidationResult validateSensorReadInterval(unsigned long interval);

/**
 * @brief Валидация интервала публикации MQTT
 * @param interval Интервал для проверки
 * @return Результат валидации
 */
ValidationResult validateMQTTPublishInterval(unsigned long interval);

/**
 * @brief Валидация интервала ThingSpeak
 * @param interval Интервал для проверки
 * @return Результат валидации
 */
ValidationResult validateThingSpeakInterval(unsigned long interval);

/**
 * @brief Валидация интервала обновления NTP
 * @param interval Интервал для проверки
 * @return Результат валидации
 */
ValidationResult validateNTPInterval(unsigned long interval);

// ============================================================================
// ФУНКЦИИ ВАЛИДАЦИИ ДАННЫХ ДАТЧИКА
// ============================================================================

/**
 * @brief Универсальная функция валидации диапазона
 * @param value Значение для проверки
 * @param min_val Минимальное значение
 * @param max_val Максимальное значение
 * @param field_name Название поля для сообщения об ошибке
 * @return Результат валидации
 */
ValidationResult validateRange(float value, float min_val, float max_val, const char* field_name);

/**
 * @brief Валидация температуры
 * @param temperature Температура для проверки
 * @return Результат валидации
 */
ValidationResult validateTemperature(float temperature);

/**
 * @brief Валидация влажности
 * @param humidity Влажность для проверки
 * @return Результат валидации
 */
ValidationResult validateHumidity(float humidity);

/**
 * @brief Валидация pH
 * @param ph pH для проверки
 * @return Результат валидации
 */
ValidationResult validatePH(float ph);

/**
 * @brief Валидация электропроводности
 * @param ec EC для проверки
 * @return Результат валидации
 */
ValidationResult validateEC(float ec);

/**
 * @brief Валидация NPK значений
 * @param value Значение для проверки
 * @param nutrient Название питательного вещества
 * @return Результат валидации
 */
ValidationResult validateNPK(float value, const char* nutrient);

// ============================================================================
// КОМПЛЕКСНАЯ ВАЛИДАЦИЯ
// ============================================================================

/**
 * @brief Полная валидация конфигурации
 * @param config Конфигурация для проверки
 * @param checkRequired Проверять ли обязательные поля
 * @return Результат валидации с списком ошибок
 */
ConfigValidationResult validateFullConfig(const ConfigData& config, bool checkRequired = true);

/**
 * @brief Полная валидация данных датчика
 * @param data Данные датчика для проверки
 * @return Результат валидации с списком ошибок
 */
SensorValidationResult validateFullSensorData(const SensorData& data);

// ============================================================================
// УТИЛИТЫ ВАЛИДАЦИИ
// ============================================================================

/**
 * @brief Проверка валидности IP адреса
 * @param ip IP адрес для проверки
 * @return true если IP адрес валиден
 */
bool isValidIPAddress(const String& ip);

/**
 * @brief Проверка валидности hostname
 * @param hostname Hostname для проверки
 * @return true если hostname валиден
 */
bool isValidHostname(const String& hostname);

/**
 * @brief Форматирование ошибок валидации конфигурации
 * @param result Результат валидации
 * @return Отформатированная строка с ошибками
 */
String formatValidationErrors(const ConfigValidationResult& result);

/**
 * @brief Форматирование ошибок валидации датчика
 * @param result Результат валидации
 * @return Отформатированная строка с ошибками
 */
String formatSensorValidationErrors(const SensorValidationResult& result);

// ============================================================================
// ЛОГИРОВАНИЕ ВАЛИДАЦИИ
// ============================================================================

/**
 * @brief Логирование результата валидации конфигурации
 * @param result Результат валидации
 * @param context Контекст валидации
 */
void logValidationResult(const ConfigValidationResult& result, const char* context);

/**
 * @brief Логирование результата валидации датчика
 * @param result Результат валидации
 * @param context Контекст валидации
 */
void logSensorValidationResult(const SensorValidationResult& result, const char* context);