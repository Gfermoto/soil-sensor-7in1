/**
 * @file validation_utils.cpp
 * @brief Утилиты валидации для устранения дублирования кода
 * @details Централизованные функции валидации конфигурации и данных
 */

#include "validation_utils.h"
#include "jxct_constants.h"
#include "logger.h"

// ============================================================================
// ВАЛИДАЦИЯ КОНФИГУРАЦИИ
// ============================================================================

ValidationResult validateSSID(const String& ssid)
{
    if (ssid.length() == 0)
    {
        return {false, "SSID не может быть пустым"};
    }
    if (ssid.length() > 32)
    {
        return {false, "SSID слишком длинный (максимум 32 символа)"};
    }
    return {true, ""};
}

ValidationResult validatePassword(const String& password)
{
    if (password.length() > 0 && password.length() < 8)
    {
        return {false, "Пароль должен содержать минимум 8 символов"};
    }
    if (password.length() > 63)
    {
        return {false, "Пароль слишком длинный (максимум 63 символа)"};
    }
    return {true, ""};
}

ValidationResult validateMQTTServer(const String& server)
{
    if (server.length() == 0)
    {
        return {false, "MQTT сервер не может быть пустым"};
    }
    if (server.length() > 253)
    {
        return {false, "MQTT сервер слишком длинный"};
    }
    // Простая проверка на валидность hostname/IP
    if (server.indexOf(' ') >= 0)
    {
        return {false, "MQTT сервер содержит недопустимые символы"};
    }
    return {true, ""};
}

ValidationResult validateMQTTPort(int port)
{
    if (port < CONFIG_MQTT_PORT_MIN || port > CONFIG_MQTT_PORT_MAX)
    {
        return {false, "MQTT порт должен быть в диапазоне 1-65535"};
    }
    return {true, ""};
}

ValidationResult validateThingSpeakAPIKey(const String& apiKey)
{
    if (apiKey.length() == 0)
    {
        return {false, "ThingSpeak API ключ не может быть пустым"};
    }
    if (apiKey.length() != 16)
    {
        return {false, "ThingSpeak API ключ должен содержать 16 символов"};
    }
    // Проверяем, что содержит только допустимые символы
    for (char c : apiKey)
    {
        if (!isAlphaNumeric(c))
        {
            return {false, "ThingSpeak API ключ содержит недопустимые символы"};
        }
    }
    return {true, ""};
}

ValidationResult validateInterval(unsigned long interval, unsigned long min_val, unsigned long max_val,
                                  const char* name)
{
    if (interval < min_val || interval > max_val)
    {
        String message = String(name) + " должен быть в диапазоне " + String(min_val) + "-" + String(max_val) + " мс";
        return {false, message};
    }
    return {true, ""};
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
        String message = String(field_name) + " вне допустимого диапазона";
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

ValidationResult validatePH(float ph)
{
    return validateRange(ph, SENSOR_PH_MIN, SENSOR_PH_MAX, "pH");
}

ValidationResult validateEC(float ec)
{
    return validateRange(ec, SENSOR_EC_MIN, SENSOR_EC_MAX, "EC");
}

ValidationResult validateNPK(float value, const char* nutrient)
{
    if (value < SENSOR_NPK_MIN || value > SENSOR_NPK_MAX)
    {
        String message = String(nutrient) + " вне допустимого диапазона";
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

bool isValidIPAddress(const String& ip)
{
    int parts = 0;
    int start = 0;

    for (int i = 0; i <= ip.length(); i++)
    {
        if (i == ip.length() || ip.charAt(i) == '.')
        {
            if (i == start)
            {
                return false;  // Пустая часть
            }

            String part = ip.substring(start, i);
            int value = part.toInt();

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

    for (char c : hostname)
    {
        if (!isAlphaNumeric(c) && c != '.' && c != '-')
        {
            return false;
        }
    }

    // Не должен начинаться или заканчиваться точкой или дефисом
    if (hostname.charAt(0) == '.' || hostname.charAt(0) == '-' || hostname.charAt(hostname.length() - 1) == '.' ||
        hostname.charAt(hostname.length() - 1) == '-')
    {
        return false;
    }

    return true;
}

String formatValidationErrors(const ConfigValidationResult& result)
{
    if (result.isValid)
    {
        return "";
    }

    String formatted = "Ошибки валидации:\n";
    for (const auto& error : result.errors)
    {
        formatted += "• " + error.field + ": " + error.message + "\n";
    }
    return formatted;
}

String formatSensorValidationErrors(const SensorValidationResult& result)
{
    if (result.isValid) return "";

    String formatted = "Ошибки валидации датчика:\n";
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
        logSuccess("Валидация %s прошла успешно", context);
    }
    else
    {
        logError("Валидация %s не пройдена:", context);
        for (const auto& error : result.errors)
        {
            logError("  %s: %s", error.field.c_str(), error.message.c_str());
        }
    }
}

void logSensorValidationResult(const SensorValidationResult& result, const char* context)
{
    if (result.isValid)
    {
        logSuccess("Валидация датчика %s прошла успешно", context);
    }
    else
    {
        logWarn("Валидация датчика %s не пройдена:", context);
        for (const auto& error : result.errors)
        {
            logWarn("  %s: %s", error.field.c_str(), error.message.c_str());
        }
    }
}
