/**
 * @file validation_utils.cpp
 * @brief Утилиты валидации для устранения дублирования кода
 * @details Централизованные функции валидации конфигурации и данных
 */

#include "validation_utils.h"
#include "jxct_constants.h"
#include "logger.h"

namespace
{
// Внутренние функции — только для этой единицы трансляции

// Структура для устранения проблемы с легко перепутываемыми параметрами
struct IntervalValidation
{
    unsigned long interval;
    unsigned long min_val;
    unsigned long max_val;
    const char* field_name;

   private:
    IntervalValidation(unsigned long interval, unsigned long min, unsigned long max,
                       const char* field_name)  // NOLINT(bugprone-easily-swappable-parameters)
        : interval(interval), min_val(min), max_val(max), field_name(field_name)
    {
    }

   public:
    // Builder для предотвращения ошибок с параметрами
    struct Builder
    {
        unsigned long val = 0;
        unsigned long min = 0;
        unsigned long max = 0;
        const char* name = "";
        Builder& interval(unsigned long value)
        {
            val = value;
            return *this;
        }
        Builder& minValue(unsigned long minVal)
        {
            min = minVal;
            return *this;
        }
        Builder& maxValue(unsigned long maxVal)
        {
            max = maxVal;
            return *this;
        }
        Builder& fieldName(const char* fieldName)
        {
            name = fieldName;
            return *this;
        }
        [[nodiscard]] IntervalValidation build() const
        {
            return {val, min, max, name};
        }
    };
    static Builder builder()
    {
        return {};
    }
};

struct RangeParams
{
    float value = 0.0F;
    float min_val = 0.0F;
    float max_val = 0.0F;
    const char* field_name = "";

   public:
    struct Builder
    {
        float value = 0.0F;
        float min_val = 0.0F;
        float max_val = 0.0F;
        const char* field_name = "";
        Builder& setValue(float val)
        {
            value = val;
            return *this;
        }
        Builder& setMinVal(float min)
        {
            min_val = min;
            return *this;
        }
        Builder& setMaxVal(float max)
        {
            max_val = max;
            return *this;
        }
        Builder& setFieldName(const char* name)
        {
            field_name = name;
            return *this;
        }
        [[nodiscard]] RangeParams build() const
        {
            return {value, min_val, max_val, field_name};
        }
    };
    static Builder builder()
    {
        return {};
    }
};

// Внутренние функции валидации
ValidationResult validateInterval(const IntervalValidation& params)
{
    if (params.interval < params.min_val || params.interval > params.max_val)
    {
        const String message = String(params.field_name) + " должен быть в диапазоне " + String(params.min_val) + "-" +
                               String(params.max_val) + " мс";
        return {false, message};
    }
    return {true, ""};
}

ValidationResult validateIntervalInternal(const IntervalValidation& params)
{
    return validateInterval(params);
}

ValidationResult validateSensorReadIntervalInternal(unsigned long interval)
{
    return validateIntervalInternal(IntervalValidation::builder()
                                        .interval(interval)
                                        .minValue(CONFIG_INTERVAL_MIN)
                                        .maxValue(CONFIG_INTERVAL_MAX)
                                        .fieldName("Интервал чтения датчика")
                                        .build());
}

ValidationResult validateMQTTPublishIntervalInternal(unsigned long interval)
{
    return validateIntervalInternal(IntervalValidation::builder()
                                        .interval(interval)
                                        .minValue(CONFIG_INTERVAL_MIN)
                                        .maxValue(CONFIG_INTERVAL_MAX)
                                        .fieldName("Интервал публикации MQTT")
                                        .build());
}

ValidationResult validateThingSpeakIntervalInternal(unsigned long interval)
{
    return validateIntervalInternal(IntervalValidation::builder()
                                        .interval(interval)
                                        .minValue(CONFIG_THINGSPEAK_MIN)
                                        .maxValue(CONFIG_THINGSPEAK_MAX)
                                        .fieldName("Интервал ThingSpeak")
                                        .build());
}

ValidationResult validateNTPIntervalInternal(unsigned long interval)
{
    return validateIntervalInternal(IntervalValidation::builder()
                                        .interval(interval)
                                        .minValue(10000)
                                        .maxValue(86400000)
                                        .fieldName("Интервал обновления NTP")
                                        .build());
}

// Универсальная функция валидации диапазона
ValidationResult validateRangeInternal(const RangeParams& params)
{
    if (params.value < params.min_val || params.value > params.max_val)
    {
        const String message = String(params.field_name) + " вне допустимого диапазона";
        return {false, message};
    }
    return {true, ""};
}

ValidationResult validateTemperatureInternal(float temperature)
{
    return validateRangeInternal(RangeParams::builder()
                                     .setValue(temperature)
                                     .setMinVal(SENSOR_TEMP_MIN)
                                     .setMaxVal(SENSOR_TEMP_MAX)
                                     .setFieldName("Температура")
                                     .build());
}

ValidationResult validateHumidityInternal(float humidity)
{
    return validateRangeInternal(RangeParams::builder()
                                     .setValue(humidity)
                                     .setMinVal(SENSOR_HUMIDITY_MIN)
                                     .setMaxVal(SENSOR_HUMIDITY_MAX)
                                     .setFieldName("Влажность")
                                     .build());
}

ValidationResult validatePHInternal(float phValue)
{
    return validateRangeInternal(RangeParams::builder()
                                     .setValue(phValue)
                                     .setMinVal(SENSOR_PH_MIN)
                                     .setMaxVal(SENSOR_PH_MAX)
                                     .setFieldName("pH")
                                     .build());
}

ValidationResult validateECInternal(float ecValue)
{
    // EC должен быть больше 0 согласно документации
    if (ecValue <= 0.0F || ecValue > SENSOR_EC_MAX)
    {
        const String message = "EC вне допустимого диапазона (0, " + String(SENSOR_EC_MAX) + "]";
        return {false, message};
    }
    return {true, ""};
}

ValidationResult validateNPKInternal(float value, const char* nutrient)
{
    if (value < SENSOR_NPK_MIN || value > SENSOR_NPK_MAX)
    {
        const String message = String(nutrient) + " вне допустимого диапазона";
        return {false, message};
    }
    return {true, ""};
}
}  // namespace

// Публичные функции (обёртки для обратной совместимости)
ValidationResult validateInterval(
    unsigned long interval, unsigned long min_val, unsigned long max_val,
    const char* field_name)  // NOLINT(bugprone-easily-swappable-parameters,misc-use-internal-linkage)
{
    return validateIntervalInternal(IntervalValidation::builder()
                                        .interval(interval)
                                        .minValue(min_val)
                                        .maxValue(max_val)
                                        .fieldName(field_name)
                                        .build());
}

ValidationResult validateSensorReadInterval(
    unsigned long
        interval)  // NOLINT(misc-use-internal-linkage,readability-convert-member-functions-to-static,misc-use-anonymous-namespace)
{
    return validateSensorReadIntervalInternal(interval);
}

ValidationResult validateMQTTPublishInterval(
    unsigned long
        interval)  // NOLINT(misc-use-internal-linkage,readability-convert-member-functions-to-static,misc-use-anonymous-namespace)
{
    return validateMQTTPublishIntervalInternal(interval);
}

ValidationResult validateThingSpeakInterval(
    unsigned long
        interval)  // NOLINT(misc-use-internal-linkage,readability-convert-member-functions-to-static,misc-use-anonymous-namespace)
{
    return validateThingSpeakIntervalInternal(interval);
}

ValidationResult validateNTPInterval(
    unsigned long
        interval)  // NOLINT(misc-use-internal-linkage,readability-convert-member-functions-to-static,misc-use-anonymous-namespace)
{
    return validateNTPIntervalInternal(interval);
}

// ============================================================================
// ВАЛИДАЦИЯ ДАННЫХ ДАТЧИКА
// ============================================================================

// Публичные функции валидации данных датчика
ValidationResult validateRange(
    float value, float min_val, float max_val,
    const char* field_name)  // NOLINT(bugprone-easily-swappable-parameters,misc-use-internal-linkage)
{
    return validateRangeInternal(
        RangeParams::builder().setValue(value).setMinVal(min_val).setMaxVal(max_val).setFieldName(field_name).build());
}

ValidationResult validateTemperature(
    float
        temperature)  // NOLINT(misc-use-internal-linkage,readability-convert-member-functions-to-static,misc-use-anonymous-namespace)
{
    return validateTemperatureInternal(temperature);
}

ValidationResult validateHumidity(
    float
        humidity)  // NOLINT(misc-use-internal-linkage,readability-convert-member-functions-to-static,misc-use-anonymous-namespace)
{
    return validateHumidityInternal(humidity);
}

ValidationResult validatePH(
    float
        phValue)  // NOLINT(misc-use-internal-linkage,readability-convert-member-functions-to-static,misc-use-anonymous-namespace)
{
    return validatePHInternal(phValue);
}

ValidationResult validateEC(
    float
        ecValue)  // NOLINT(misc-use-internal-linkage,readability-convert-member-functions-to-static,misc-use-anonymous-namespace)
{
    return validateECInternal(ecValue);
}

ValidationResult validateNPK(
    float value,
    const char*
        nutrient)  // NOLINT(misc-use-internal-linkage,readability-convert-member-functions-to-static,misc-use-anonymous-namespace)
{
    return validateNPKInternal(value, nutrient);
}

// ============================================================================
// КОМПЛЕКСНАЯ ВАЛИДАЦИЯ
// ============================================================================

ConfigValidationResult validateFullConfig(const ConfigData& config,
                                          bool checkRequired)  // NOLINT(misc-use-internal-linkage)
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
    auto sensorIntervalResult = validateSensorReadIntervalInternal(config.sensorReadInterval);
    if (!sensorIntervalResult.isValid)
    {
        result.isValid = false;
        result.errors.push_back({"sensor_read_interval", sensorIntervalResult.message});
    }

    auto mqttIntervalResult = validateMQTTPublishIntervalInternal(config.mqttPublishInterval);
    if (!mqttIntervalResult.isValid)
    {
        result.isValid = false;
        result.errors.push_back({"mqtt_publish_interval", mqttIntervalResult.message});
    }

    auto tsIntervalResult = validateThingSpeakIntervalInternal(config.thingspeakInterval);
    if (!tsIntervalResult.isValid)
    {
        result.isValid = false;
        result.errors.push_back({"thingspeak_interval", tsIntervalResult.message});
    }

    auto ntpIntervalResult = validateNTPIntervalInternal(config.ntpUpdateInterval);
    if (!ntpIntervalResult.isValid)
    {
        result.isValid = false;
        result.errors.push_back({"ntp_update_interval", ntpIntervalResult.message});
    }

    return result;
}

SensorValidationResult validateFullSensorData(const SensorData& data)  // NOLINT(misc-use-internal-linkage)
{
    SensorValidationResult result;
    result.isValid = true;

    auto tempResult = validateTemperatureInternal(data.temperature);
    if (!tempResult.isValid)
    {
        result.isValid = false;
        result.errors.push_back({"temperature", tempResult.message});
    }

    auto humResult = validateHumidityInternal(data.humidity);
    if (!humResult.isValid)
    {
        result.isValid = false;
        result.errors.push_back({"humidity", humResult.message});
    }

    auto phResult = validatePHInternal(data.ph);
    if (!phResult.isValid)
    {
        result.isValid = false;
        result.errors.push_back({"ph", phResult.message});
    }

    auto ecResult = validateECInternal(data.ec);
    if (!ecResult.isValid)
    {
        result.isValid = false;
        result.errors.push_back({"ec", ecResult.message});
    }

    auto nitrogenResult = validateNPKInternal(data.nitrogen, "Азот");
    if (!nitrogenResult.isValid)
    {
        result.isValid = false;
        result.errors.push_back({"nitrogen", nitrogenResult.message});
    }

    auto phosphorusResult = validateNPKInternal(data.phosphorus, "Фосфор");
    if (!phosphorusResult.isValid)
    {
        result.isValid = false;
        result.errors.push_back({"phosphorus", phosphorusResult.message});
    }

    auto potassiumResult = validateNPKInternal(data.potassium, "Калий");
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

bool isValidIPAddress(const String& ipAddress)  // NOLINT(misc-use-internal-linkage)
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

bool isValidHostname(const String& hostname)  // NOLINT(misc-use-internal-linkage)
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
    return hostname.charAt(0) != '.' && hostname.charAt(0) != '-' && hostname.charAt(hostname.length() - 1) != '.' &&
           hostname.charAt(hostname.length() - 1) != '-';
}

String formatValidationErrors(const ConfigValidationResult& result)  // NOLINT(misc-use-internal-linkage)
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

String formatSensorValidationErrors(const SensorValidationResult& result)  // NOLINT(misc-use-internal-linkage)
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

void logValidationResult(const ConfigValidationResult& result,
                         const char* context)  // NOLINT(misc-use-internal-linkage)
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

void logSensorValidationResult(const SensorValidationResult& result,
                               const char* context)  // NOLINT(misc-use-internal-linkage)
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

ValidationResult validateSSID(const String& ssid)  // NOLINT(misc-use-internal-linkage)
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

ValidationResult validatePassword(const String& password)  // NOLINT(misc-use-internal-linkage)
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

ValidationResult validateMQTTServer(const String& server)  // NOLINT(misc-use-internal-linkage)
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

ValidationResult validateMQTTPort(int port)  // NOLINT(misc-use-internal-linkage)
{
    if (port < CONFIG_MQTT_PORT_MIN || port > CONFIG_MQTT_PORT_MAX)
    {
        return ValidationResult{
            false, "MQTT порт должен быть от " + String(CONFIG_MQTT_PORT_MIN) + " до " + String(CONFIG_MQTT_PORT_MAX)};
    }
    return ValidationResult{true, ""};
}

ValidationResult validateThingSpeakAPIKey(const String& apiKey)  // NOLINT(misc-use-internal-linkage)
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
