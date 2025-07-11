/**
 * @file validation_utils.cpp
 * @brief Утилиты валидации для устранения дублирования кода
 * @details Централизованные функции валидации конфигурации и данных
 */

#include "validation_utils.h"
#include "jxct_constants.h"
#include "logger.h"

namespace {
// Внутренние функции — только для этой единицы трансляции
// (функции, объявленные в заголовочном файле, реализованы вне namespace)

// Структура для устранения проблемы с легко перепутываемыми параметрами
struct IntervalValidation {
    unsigned long interval;
    unsigned long min_val;
    unsigned long max_val;
    const char* field_name;
    
private:
    IntervalValidation(unsigned long interval, unsigned long min, unsigned long max, const char* field_name)
        : interval(interval), min_val(min), max_val(max), field_name(field_name) {}
public:
    static IntervalValidation fromValues(unsigned long interval, unsigned long min, unsigned long max, const char* field_name) {
        return {interval, min, max, field_name};
    }
    // Builder для предотвращения ошибок с параметрами
    struct Builder {
        unsigned long val = 0;
        unsigned long min = 0;
        unsigned long max = 0;
        const char* name = "";
        Builder& interval(unsigned long value) { val = value; return *this; }
        Builder& minValue(unsigned long minVal) { min = minVal; return *this; }
        Builder& maxValue(unsigned long maxVal) { max = maxVal; return *this; }
        Builder& fieldName(const char* fieldName) { name = fieldName; return *this; }
        IntervalValidation build() const {
            return IntervalValidation::fromValues(val, min, max, name);
        }
    };
    static Builder builder() { return {}; }
};

static ValidationResult validateInterval(const IntervalValidation& params)
{
    if (params.interval < params.min_val || params.interval > params.max_val)
    {
        const String message =
            String(params.field_name) + " должен быть в диапазоне " + String(params.min_val) + "-" + String(params.max_val) + " мс";
        return {false, message};
    }
    return {true, ""};
}
}  // namespace

// Обратная совместимость
ValidationResult validateInterval(unsigned long interval, unsigned long min_val, unsigned long max_val,
                                  const char* field_name)
{
    return validateInterval(IntervalValidation::builder()
        .interval(interval)
        .minValue(min_val)
        .maxValue(max_val)
        .fieldName(field_name)
        .build());
}

ValidationResult validateSensorReadInterval(unsigned long interval)
{
    return validateInterval(interval, CONFIG_INTERVAL_MIN, CONFIG_INTERVAL_MAX, "Интервал чтения датчика");
}

ValidationResult validateMQTTPublishInterval(unsigned long interval)
{
    return validateInterval(interval, CONFIG_INTERVAL_MIN, CONFIG_INTERVAL_MAX, "Интервал публикации MQTT");
}

ValidationResult validateThingSpeakInterval(unsigned long interval)
{
    return validateInterval(interval, CONFIG_THINGSPEAK_MIN, CONFIG_THINGSPEAK_MAX, "Интервал ThingSpeak");
}

ValidationResult validateNTPInterval(unsigned long interval)
{
    return validateInterval(interval, 10000, 86400000, "Интервал обновления NTP");
}

// ============================================================================
// ВАЛИДАЦИЯ ДАННЫХ ДАТЧИКА
// ============================================================================

// Универсальная функция валидации диапазона
ValidationResult validateRange(float value, float min_val, float max_val, const char* field_name)
{
    if (value < min_val || value > max_val)
    {
        const String message = String(field_name) + " вне допустимого диапазона";
        return {false, message};
    }
    return {true, ""};
}

ValidationResult validateTemperature(float temperature)
{
    return validateRange(temperature, SENSOR_TEMP_MIN, SENSOR_TEMP_MAX, "Температура");
}

ValidationResult validateHumidity(float humidity)
{
    return validateRange(humidity, SENSOR_HUMIDITY_MIN, SENSOR_HUMIDITY_MAX, "Влажность");
}

ValidationResult validatePH(float phValue)
{
    return validateRange(phValue, SENSOR_PH_MIN, SENSOR_PH_MAX, "pH");
}

ValidationResult validateEC(float ecValue)
{
    return validateRange(ecValue, SENSOR_EC_MIN, SENSOR_EC_MAX, "EC");
}

ValidationResult validateNPK(float value, const char* nutrient)
{
    if (value < SENSOR_NPK_MIN || value > SENSOR_NPK_MAX)
    {
        const String message = String(nutrient) + " вне допустимого диапазона";
        return {false, message};
    }
    return {true, ""};
}

// ============================================================================
// КОМПЛЕКСНАЯ ВАЛИДАЦИЯ
// ============================================================================

ConfigValidationResult validateFullConfig(const ConfigData& config, bool checkRequired)
{
    ConfigValidationResult result;
    result.isValid = true;

    // Проверка SSID (всегда обязательно)
    auto ssidResult = validateSSID(config.ssid);
    if (!ssidResult.isValid)
    {
        result.isValid = false;
        result.errors.push_back({"ssid", ssidResult.message});
    }

    // Проверка пароля WiFi
    auto passwordResult = validatePassword(config.password);
    if (!passwordResult.isValid)
    {
        result.isValid = false;
        result.errors.push_back({"password", passwordResult.message});
    }

    if (checkRequired)
    {
        // Проверка MQTT настроек (если включен)
        if (config.mqttEnabled)
        {
            auto mqttServerResult = validateMQTTServer(config.mqttServer);
            if (!mqttServerResult.isValid)
            {
                result.isValid = false;
                result.errors.push_back({"mqtt_server", mqttServerResult.message});
            }

            auto mqttPortResult = validateMQTTPort(config.mqttPort);
            if (!mqttPortResult.isValid)
            {
                result.isValid = false;
                result.errors.push_back({"mqtt_port", mqttPortResult.message});
            }
        }

        // Проверка ThingSpeak настроек (если включен)
        if (config.thingSpeakEnabled)
        {
            auto tsResult = validateThingSpeakAPIKey(config.thingSpeakAPIKey);
            if (!tsResult.isValid)
            {
                result.isValid = false;
                result.errors.push_back({"thingspeak_api_key", tsResult.message});
            }
        }
    }

    // Проверка интервалов
    auto sensorIntervalResult = validateSensorReadInterval(config.sensorReadInterval);
    if (!sensorIntervalResult.isValid)
    {
        result.isValid = false;
        result.errors.push_back({"sensor_read_interval", sensorIntervalResult.message});
    }

    auto mqttIntervalResult = validateMQTTPublishInterval(config.mqttPublishInterval);
    if (!mqttIntervalResult.isValid)
    {
        result.isValid = false;
        result.errors.push_back({"mqtt_publish_interval", mqttIntervalResult.message});
    }

    auto tsIntervalResult = validateThingSpeakInterval(config.thingspeakInterval);
    if (!tsIntervalResult.isValid)
    {
        result.isValid = false;
        result.errors.push_back({"thingspeak_interval", tsIntervalResult.message});
    }

    auto ntpIntervalResult = validateNTPInterval(config.ntpUpdateInterval);
    if (!ntpIntervalResult.isValid)
    {
        result.isValid = false;
        result.errors.push_back({"ntp_update_interval", ntpIntervalResult.message});
    }

    return result;
}

SensorValidationResult validateFullSensorData(const SensorData& data)
{
    SensorValidationResult result;
    result.isValid = true;

    auto tempResult = validateTemperature(data.temperature);
    if (!tempResult.isValid)
    {
        result.isValid = false;
        result.errors.push_back({"temperature", tempResult.message});
    }

    auto humResult = validateHumidity(data.humidity);
    if (!humResult.isValid)
    {
        result.isValid = false;
        result.errors.push_back({"humidity", humResult.message});
    }

    auto phResult = validatePH(data.ph);
    if (!phResult.isValid)
    {
        result.isValid = false;
        result.errors.push_back({"ph", phResult.message});
    }

    auto ecResult = validateEC(data.ec);
    if (!ecResult.isValid)
    {
        result.isValid = false;
        result.errors.push_back({"ec", ecResult.message});
    }

    auto nitrogenResult = validateNPK(data.nitrogen, "Азот");
    if (!nitrogenResult.isValid)
    {
        result.isValid = false;
        result.errors.push_back({"nitrogen", nitrogenResult.message});
    }

    auto phosphorusResult = validateNPK(data.phosphorus, "Фосфор");
    if (!phosphorusResult.isValid)
    {
        result.isValid = false;
        result.errors.push_back({"phosphorus", phosphorusResult.message});
    }

    auto potassiumResult = validateNPK(data.potassium, "Калий");
    if (!potassiumResult.isValid)
    {
        result.isValid = false;
        result.errors.push_back({"potassium", potassiumResult.message});
    }

    return result;
}

// ============================================================================
// УТИЛИТЫ ВАЛИДАЦИИ
// ============================================================================

bool isValidIPAddress(const String& ipAddress)
{
    int parts = 0;
    int start = 0;

    for (int i = 0; i <= ipAddress.length(); ++i)
    {
        if (i == ipAddress.length() || ipAddress.charAt(i) == '.')
        {
            if (i == start)
            {
                return false;  // Пустая часть
            }

            const String part = ipAddress.substring(start, i);
            const int value = part.toInt();

            if (value < 0 || value > 255)
            {
                return false;
            }
            if (part != String(value))
            {
                return false;  // Проверка на ведущие нули
            }

            parts++;
            start = i + 1;
        }
    }

    return parts == 4;
}

bool isValidHostname(const String& hostname)
{
    if (hostname.length() == 0 || hostname.length() > 253)
    {
        return false;
    }

    for (const char character : hostname)
    {
        if (!isAlphaNumeric(character) && character != '.' && character != '-')
        {
            return false;
        }
    }

    // Не должен начинаться или заканчиваться точкой или дефисом
    return hostname.charAt(0) != '.' && hostname.charAt(0) != '-' &&
           hostname.charAt(hostname.length() - 1) != '.' &&
           hostname.charAt(hostname.length() - 1) != '-';
}

String formatValidationErrors(const ConfigValidationResult& result)
{
    if (result.isValid)
    {
        return "";
    }

    String formatted = "Ошибки валидации:\n";  // NOLINT(misc-const-correctness) - изменяется в цикле
    for (const auto& error : result.errors)
    {
        formatted += "• " + error.field + ": " + error.message + "\n";
    }
    return formatted;
}

String formatSensorValidationErrors(const SensorValidationResult& result)
{
    if (result.isValid)
    {
        return "";
    }

    String formatted = "Ошибки валидации датчика:\n";  // NOLINT(misc-const-correctness) - изменяется в цикле
    for (const auto& error : result.errors)
    {
        formatted += "• " + error.field + ": " + error.message + "\n";
    }
    return formatted;
}

// ============================================================================
// ЛОГИРОВАНИЕ ВАЛИДАЦИИ
// ============================================================================

void logValidationResult(const ConfigValidationResult& result, const char* context)
{
    if (result.isValid)
    {
        logSuccessSafe("\1", context);
    }
    else
    {
        logErrorSafe("\1", context);
        for (const auto& error : result.errors)
        {
            logErrorSafe("\1", error.field.c_str(), error.message.c_str());
        }
    }
}

void logSensorValidationResult(const SensorValidationResult& result, const char* context)
{
    if (result.isValid)
    {
        logSuccessSafe("\1", context);
    }
    else
    {
        logWarnSafe("\1", context);
        for (const auto& error : result.errors)
        {
            logWarnSafe("\1", error.field.c_str(), error.message.c_str());
        }
    }
}

ValidationResult validateSSID(const String& ssid)
{
    if (ssid.length() == 0)
    {
        return ValidationResult{false, "SSID не может быть пустым"};
    }
    if (ssid.length() > 32)
    {
        return ValidationResult{false, "SSID слишком длинный"};
    }
    return ValidationResult{true, ""};
}

ValidationResult validatePassword(const String& password)
{
    if (password.length() > 0 && password.length() < 8)
    {
        return ValidationResult{false, "Пароль должен содержать минимум 8 символов"};
    }
    if (password.length() > 63)
    {
        return ValidationResult{false, "Пароль слишком длинный"};
    }
    return ValidationResult{true, ""};
}

ValidationResult validateMQTTServer(const String& server)
{
    if (server.length() == 0)
    {
        return ValidationResult{false, "MQTT сервер не может быть пустым"};
    }
    if (server.length() > 253)
    {
        return ValidationResult{false, "MQTT сервер слишком длинный"};
    }
    if (!isValidHostname(server) && !isValidIPAddress(server))
    {
        return ValidationResult{false, "Недопустимый формат MQTT сервера"};
    }
    return ValidationResult{true, ""};
}

ValidationResult validateMQTTPort(int port)
{
    if (port < CONFIG_MQTT_PORT_MIN || port > CONFIG_MQTT_PORT_MAX)
    {
        return ValidationResult{false, "MQTT порт должен быть от " + String(CONFIG_MQTT_PORT_MIN) + " до " + String(CONFIG_MQTT_PORT_MAX)};
    }
    return ValidationResult{true, ""};
}

ValidationResult validateThingSpeakAPIKey(const String& apiKey)
{
    if (apiKey.length() == 0)
    {
        return ValidationResult{false, "API ключ ThingSpeak не может быть пустым"};
    }
    if (apiKey.length() > 16)
    {
        return ValidationResult{false, "API ключ ThingSpeak слишком длинный"};
    }
    return ValidationResult{true, ""};
}
