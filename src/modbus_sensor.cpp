/**
 * @file modbus_sensor.cpp
 * @brief Работа с Modbus-датчиком JXCT
 * @details Реализация функций и задач для опроса датчика по Modbus RTU, преобразование данных, управление линией
 * передачи.
 */
#include "modbus_sensor.h"
#include <Arduino.h>
#include <algorithm>  // для std::min
#include "advanced_filters.h"  // ✅ Улучшенная система фильтрации
#include "calibration_manager.h"
#include "debug.h"  // ✅ Добавляем систему условной компиляции
#include "jxct_config_vars.h"
#include "jxct_constants.h"  // ✅ Централизованные константы
#include "jxct_device_info.h"
#include "logger.h"
#include "sensor_compensation.h"
#include "business_services.h"

// Глобальные переменные (должны быть доступны через extern)
ModbusMaster modbus;
SensorData sensorData;
SensorCache sensorCache;
String sensorLastError;

// Внутренние переменные и функции — только для этой единицы трансляции
namespace {

// Структура для устранения проблемы с легко перепутываемыми параметрами
struct RegisterConversion {
    uint16_t value;
    float multiplier;
    
    // Приватный конструктор
private:
    RegisterConversion(uint16_t reg_value, float mult) : value(reg_value), multiplier(mult) {}
public:
    static RegisterConversion fromRaw(uint16_t registerValue, float scaleMultiplier) {
        return RegisterConversion(registerValue, scaleMultiplier);
    }
    float toFloat() const {
        return static_cast<float>(value) * multiplier;
    }
};

float convertRegisterToFloat(const RegisterConversion& conversion)
{
    return conversion.toFloat();
}

unsigned long lastIrrigationTs = 0;  // время последнего полива (для фильтрации всплесков)

void debugPrintBuffer(const char* prefix, const uint8_t* buffer, size_t length)
{
    if (currentLogLevel < LOG_DEBUG)
    {
        return;
    }

    String hex_str = "";
    for (size_t i = 0; i < length; ++i)
    {
        if (buffer[i] < 0x10)
        {
            hex_str += "0";
        }
        hex_str += String(buffer[i], HEX);
        hex_str += " ";
    }
    logDebugSafe("\1", prefix, hex_str.c_str());
}

uint16_t calculateCRC16(const uint8_t* data, size_t length)
{
    uint16_t crc = 0xFFFF;

    for (size_t i = 0; i < length; ++i)
    {
        crc ^= (uint16_t)data[i];
        for (int j = 0; j < 8; ++j)
        {
            if ((crc & 0x0001) != 0)
            {
                crc = (crc >> 1) ^ 0xA001;
            }
            else
            {
                crc = crc >> 1;
            }
        }
    }

    return crc;
}

void saveRawSnapshot(SensorData& data)
{
    data.raw_temperature = data.temperature;
    data.raw_humidity = data.humidity;
    data.raw_ec = data.ec;
    data.raw_ph = data.ph;
    data.raw_nitrogen = data.nitrogen;
    data.raw_phosphorus = data.phosphorus;
    data.raw_potassium = data.potassium;
}

void updateIrrigationFlag(SensorData& data)
{
    constexpr uint8_t WIN = 6;
    static std::array<float, WIN> buf = {NAN};
    static uint8_t idx = 0;
    static uint8_t filled = 0;
    static uint8_t persist = 0;

    float baseline = data.humidity;
    for (uint8_t i = 0; i < filled; ++i)
    {
        baseline = (buf[i] < baseline) ? buf[i] : baseline;
    }

    const bool spike =
        (filled == WIN) && (data.humidity - baseline >= config.irrigationSpikeThreshold) && (data.humidity > 25.0F);
    persist = spike ? persist + 1 : 0;
    if (persist >= 2)
    {
        lastIrrigationTs = millis();
        persist = 0;
    }

    buf[idx] = data.humidity;
    idx = (idx + 1) % WIN;
    if (filled < WIN)
    {
        ++filled;
    }

    data.recentIrrigation = (millis() - lastIrrigationTs) <= (unsigned long)config.irrigationHoldMinutes * 60000UL;
}

void applyCompensationIfEnabled(SensorData& data)
{
    if (!config.flags.calibrationEnabled)
    {
        return;
    }

    logDebugSafe("✅ Применяем исправленную компенсацию датчика");

    // Преобразуем конфигурацию в типы бизнес-логики
    // Используем массивы для устранения дублирования кода
    static const std::array<SoilType, 5> soilTypes = {{
        SoilType::SAND,      // 0
        SoilType::LOAM,      // 1
        SoilType::PEAT,      // 2
        SoilType::CLAY,      // 3
        SoilType::SANDPEAT   // 4
    }};
    
    static const std::array<SoilProfile, 5> soilProfiles = {{
        SoilProfile::SAND,      // 0
        SoilProfile::LOAM,      // 1
        SoilProfile::PEAT,      // 2
        SoilProfile::CLAY,      // 3
        SoilProfile::SANDPEAT   // 4
    }};
    
    const int profileIndex = (config.soilProfile >= 0 && config.soilProfile < 5) ? config.soilProfile : 1;
    const SoilType soil = soilTypes[profileIndex];
    const SoilProfile profile = soilProfiles[profileIndex];

    // Шаг 1: Применяем калибровку через бизнес-сервис
    getCalibrationService().applyCalibration(data, profile);

    // Шаг 2: Применяем компенсацию через бизнес-сервис
    getCompensationService().applyCompensation(data, soil);
}

bool readSingleRegister(uint16_t reg_addr, const char* reg_name, float multiplier, void* target, bool is_float)
{
    logDebugSafe("\1", reg_name, reg_addr);
    const uint8_t result = modbus.readHoldingRegisters(reg_addr, 1);

    if (result == ModbusMaster::ku8MBSuccess)
    {
        const uint16_t raw_value = modbus.getResponseBuffer(0);
        if (is_float)
        {
            auto* float_target = static_cast<float*>(target);
            *float_target = convertRegisterToFloat(RegisterConversion::fromRaw(raw_value, multiplier));
            logDebugSafe("\1", reg_name, *float_target);
        }
        else
        {
            auto* int_target = static_cast<uint16_t*>(target);
            *int_target = raw_value;
            logDebugSafe("\1", reg_name, *int_target);
        }
        return true;
    }
    logErrorSafe("\1", reg_name, result);
    printModbusError(result);
    return false;
}

int readBasicParameters()
{
    int success_count = 0;
    if (readSingleRegister(REG_PH, "pH", 0.01F, &sensorData.ph, true)) { success_count++; }
    if (readSingleRegister(REG_SOIL_MOISTURE, "Влажность", 0.1F, &sensorData.humidity, true)) { success_count++; }
    if (readSingleRegister(REG_SOIL_TEMP, "Температура", 0.1F, &sensorData.temperature, true)) { success_count++; }
    if (readSingleRegister(REG_CONDUCTIVITY, "EC", 1.0F, &sensorData.ec, true)) { success_count++; }
    return success_count;
}

int readNPKParameters()
{
    int success_count = 0;
    if (readSingleRegister(REG_NITROGEN, "Азот", 1.0F, &sensorData.nitrogen, true)) { success_count++; }
    if (readSingleRegister(REG_PHOSPHORUS, "Фосфор", 1.0F, &sensorData.phosphorus, true)) { success_count++; }
    if (readSingleRegister(REG_POTASSIUM, "Калий", 1.0F, &sensorData.potassium, true)) { success_count++; }
    return success_count;
}

struct MovingAverageParams {
    uint8_t window_size;
    uint8_t filled;
};

float calculateMovingAverage(const float* buffer, MovingAverageParams params)
{
    if (params.filled == 0) { return 0.0F; }
    const uint8_t elements_to_use = std::min(params.filled, params.window_size);
    if (config.filterAlgorithm == 1) {
        std::array<float, 15> temp_values{};
        for (int i = 0; i < elements_to_use; ++i) {
            temp_values.at(i) = buffer[i];
        }
        for (int i = 0; i < elements_to_use - 1; ++i) {
            for (int j = 0; j < elements_to_use - i - 1; ++j) {
                if (temp_values[j] > temp_values[j + 1]) {
                    std::swap(temp_values[j], temp_values[j + 1]);
                }
            }
        }
        return temp_values[elements_to_use / 2];
    } else {
        float sum = 0.0F;
        for (int i = 0; i < elements_to_use; ++i) {
            sum += buffer[i];
        }
        return sum / elements_to_use;
    }
}

} // namespace

/**
 * @brief Тестирование работы SP3485E
 * @details Проверяет корректность работы пинов DE и RE:
 * 1. Устанавливает оба пина в режим OUTPUT
 * 2. Проверяет переключение HIGH/LOW
 * 3. Проверяет возможность независимого управления передатчиком и приемником
 */
void testSP3485E()
{
    logSystem("=== ТЕСТИРОВАНИЕ SP3485E ===");

    // Проверяем пины
    pinMode(MODBUS_DE_PIN, OUTPUT);  // Driver Enable - управление передатчиком
    pinMode(MODBUS_RE_PIN, OUTPUT);  // Receiver Enable - управление приемником

    // Тест 1: Включаем передачу, выключаем прием
    digitalWrite(MODBUS_DE_PIN, HIGH);  // Активируем передатчик
    digitalWrite(MODBUS_RE_PIN, HIGH);  // Отключаем приемник
    delay(10);

    // Тест 2: Включаем прием, выключаем передачу
    digitalWrite(MODBUS_DE_PIN, LOW);  // Деактивируем передатчик
    digitalWrite(MODBUS_RE_PIN, LOW);  // Активируем приемник

    // Проверяем состояние
    if (digitalRead(MODBUS_DE_PIN) == LOW && digitalRead(MODBUS_RE_PIN) == LOW)
    {
        logSuccess("SP3485E DE/RE пины работают корректно");
    }
    else
    {
        logWarn("Нет ответа от SP3485E (это нормально без датчика)");
    }

    logSystem("=== ТЕСТ SP3485E ЗАВЕРШЕН ===");
}

/**
 * @brief Инициализация Modbus и SP3485E
 * @details Настраивает пины управления SP3485E и инициализирует UART для Modbus.
 * Важно: DE и RE управляются раздельно для лучшего контроля над временем переключения
 */
void setupModbus()
{
    logPrintHeader("ИНИЦИАЛИЗАЦИЯ MODBUS", LogColor::CYAN);

    // Устанавливаем пины управления SP3485E
    logSystem("Настройка пинов SP3485E...");
    pinMode(MODBUS_DE_PIN, OUTPUT);  // Driver Enable - GPIO4
    pinMode(MODBUS_RE_PIN, OUTPUT);  // Receiver Enable - GPIO5

    // Начальное состояние: прием включен, передача выключена
    digitalWrite(MODBUS_DE_PIN, LOW);  // Передатчик в высокоимпедансном состоянии
    digitalWrite(MODBUS_RE_PIN, LOW);  // Приемник активен

    logSystemSafe("\1", MODBUS_DE_PIN, MODBUS_RE_PIN);
    logSuccess("Пины SP3485E настроены");

    // Инициализация UART для Modbus
    Serial2.begin(9600, SERIAL_8N1, MODBUS_RX_PIN, MODBUS_TX_PIN);  // NOLINT(readability-static-accessed-through-instance)

    // Настройка Modbus с обработчиками переключения режима
    modbus.begin(JXCT_MODBUS_ID, Serial2);  // NOLINT(readability-static-accessed-through-instance)
    modbus.preTransmission(preTransmission);    // Вызывается перед передачей
    modbus.postTransmission(postTransmission);  // Вызывается после передачи

    logSuccess("Modbus инициализирован");
    logPrintHeader("MODBUS ГОТОВ ДЛЯ ПОЛНОГО ТЕСТИРОВАНИЯ", LogColor::GREEN);
}

bool validateSensorData(SensorData& data)
{
    if (data.temperature < MIN_TEMPERATURE || data.temperature > MAX_TEMPERATURE)
    {
        return false;
    }
    if (data.humidity < MIN_HUMIDITY || data.humidity > MAX_HUMIDITY)
    {
        return false;
    }
    if (data.ec < MIN_EC || data.ec > MAX_EC)
    {
        return false;
    }
    if (data.ph < MIN_PH || data.ph > MAX_PH)
    {
        return false;
    }
    if (data.nitrogen < MIN_NPK || data.nitrogen > MAX_NPK)
    {
        return false;
    }
    if (data.phosphorus < MIN_NPK || data.phosphorus > MAX_NPK)
    {
        return false;
    }
    if (data.potassium < MIN_NPK || data.potassium > MAX_NPK)
    {
        return false;
    }
    return true;
}

bool getCachedData(SensorData& data)
{
    if (!sensorCache.is_valid)
    {
        return false;
    }
    if (millis() - sensorCache.timestamp > MODBUS_CACHE_TIMEOUT)
    {
        return false;
    }

    data = sensorCache.data;
    return true;
}

bool readFirmwareVersion()
{
    logSensor("Запрос версии прошивки датчика...");
    const uint8_t result = modbus.readHoldingRegisters(0x07, 1);

    if (result == modbus.ku8MBSuccess)  // NOLINT(readability-static-accessed-through-instance)
    {
        const uint16_t version = modbus.getResponseBuffer(0);
        logSuccessSafe("\1", (version >> 8) & 0xFF, version & 0xFF);
        return true;
    }
    logErrorSafe("\1", result);
    printModbusError(result);
    return false;
}

bool readErrorStatus()
{
    const uint8_t result = modbus.readHoldingRegisters(REG_ERROR_STATUS, 1);
    if (result == modbus.ku8MBSuccess)  // NOLINT(readability-static-accessed-through-instance)
    {
        sensorData.error_status = modbus.getResponseBuffer(0);
        return true;
    }
    return false;
}

bool changeDeviceAddress(uint8_t new_address) // NOLINT(misc-use-internal-linkage)
{
    if (new_address < 1 || new_address > 247)
    {
        return false;
    }

    const uint8_t result = modbus.writeSingleRegister(REG_DEVICE_ADDRESS, new_address);
    if (result == modbus.ku8MBSuccess)  // NOLINT(readability-static-accessed-through-instance)
    {
        // ✅ Неблокирующая задержка через vTaskDelay
        delay(100);  // ОТКАТ: Критичный timing для Modbus
        modbus.begin(new_address, Serial2);  // NOLINT(readability-static-accessed-through-instance)
        return true;
    }
    return false;
}

// Добавляем функцию диагностики Modbus связи
bool testModbusConnection()
{
    logSystem("=== ТЕСТ MODBUS СОЕДИНЕНИЯ ===");

    // Проверяем пины
    logSystemSafe("\1", MODBUS_DE_PIN, MODBUS_RE_PIN);

    // Тест 1: Проверка конфигурации пинов
    logSystem("Тест 1: Проверка конфигурации пинов...");
    pinMode(MODBUS_DE_PIN, OUTPUT);
    pinMode(MODBUS_RE_PIN, OUTPUT);
    if (digitalRead(MODBUS_DE_PIN) == LOW && digitalRead(MODBUS_RE_PIN) == LOW)
    {
        logSuccess("Пины в правильном начальном состоянии (прием)");
    }
    else
    {
        logError("Неверное начальное состояние пинов");
        return false;
    }

    // Тест 2: Проверка временных задержек
    logSystem("Тест 2: Проверка временных задержек...");
    unsigned long start_time = micros();
    preTransmission();
    const unsigned long pre_delay = micros() - start_time;

    start_time = micros();
    postTransmission();
    const unsigned long post_delay = micros() - start_time;

    logSystemSafe("\1", pre_delay);
    logSystemSafe("\1", post_delay);

    if (pre_delay >= 50 && post_delay >= 50)
    {
        logSuccess("Временные задержки в норме");
    }
    else
    {
        logWarn("Временные задержки меньше рекомендованных (50 мкс)");
    }

    // Тест 3: Проверка конфигурации UART
    logSystem("Тест 3: Проверка конфигурации UART...");
    if (Serial2.baudRate() == 9600)  // NOLINT(readability-static-accessed-through-instance)
    {
        logSuccess("Скорость UART настроена правильно: 9600");
    }
    else
    {
        logErrorSafe("\1", Serial2.baudRate());  // NOLINT(readability-static-accessed-through-instance)
        return false;
    }

    // Тест 4: Попытка чтения регистра версии прошивки
    logSystem("Тест 4: Чтение версии прошивки...");
    const uint8_t result = modbus.readHoldingRegisters(0x00, 1);
    if (result == modbus.ku8MBSuccess)  // NOLINT(readability-static-accessed-through-instance)
    {
        logSuccess("Успешно прочитан регистр версии");
    }
    else
    {
        logErrorSafe("\1", result);
        return false;
    }

    logSuccess("=== ТЕСТ MODBUS ЗАВЕРШЕН УСПЕШНО ===");
    return true;
}

// ============================================================================
// ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ ДЛЯ СНИЖЕНИЯ ЦИКЛОМАТИЧЕСКОЙ СЛОЖНОСТИ
// ============================================================================

namespace {
/**
 * @brief Финализация данных датчика (валидация, кэширование, скользящее среднее)
 * @param success Флаг успешности чтения всех параметров
 */
void finalizeSensorData(bool success)
{
    sensorData.valid = success;
    sensorData.last_update = millis();

    if (!success)
    {
        logError("❌ Не удалось прочитать один или несколько параметров");
        return;
    }

    saveRawSnapshot(sensorData);
    updateIrrigationFlag(sensorData);
    applyCompensationIfEnabled(sensorData);

    // ✅ v3.10.0: Применяем улучшенную фильтрацию
    AdvancedFilters::applyAdvancedFiltering(sensorData);

    addToMovingAverage(sensorData, sensorData);

    if (validateSensorData(sensorData))
    {
        logSuccess("✅ Все параметры прочитаны и валидны с улучшенной фильтрацией");
        sensorCache = {sensorData, true, millis()};
    }
    else
    {
        logWarn("⚠️ Данные прочитаны, но не прошли валидацию");
        sensorData.valid = false;
    }
}
}

// ============================================================================
// ОСНОВНАЯ ФУНКЦИЯ ЧТЕНИЯ ДАТЧИКА (РЕФАКТОРИНГ)
// ============================================================================

void readSensorData()
{
    logSensor("Чтение всех параметров JXCT 7-в-1 датчика...");

    // Читаем основные параметры (4 параметра)
    const int basic_success = readBasicParameters();

    // Читаем NPK параметры (3 параметра)
    const int npk_success = readNPKParameters();

    // Общий успех - все 7 параметров прочитаны
    const bool total_success = (basic_success == 4) && (npk_success == 3);

    // Финализируем данные
    finalizeSensorData(total_success);
}

/**
 * @brief Подготовка к передаче данных
 * @details Включает передатчик и отключает приемник с необходимой задержкой.
 * Важно: Задержка 50 мкс необходима для стабилизации сигналов на шине RS-485
 */
void preTransmission()
{
    digitalWrite(MODBUS_DE_PIN, HIGH);  // Активируем передатчик
    digitalWrite(MODBUS_RE_PIN, HIGH);  // Отключаем приемник для предотвращения эха
    delayMicroseconds(50);              // Ждем стабилизации сигналов
}

/**
 * @brief Завершение передачи данных
 * @details Выключает передатчик и включает приемник с необходимой задержкой.
 * Важно: Задержка 50 мкс необходима для корректного переключения режимов
 */
void postTransmission()
{
    delayMicroseconds(50);             // Ждем завершения передачи последнего байта
    digitalWrite(MODBUS_DE_PIN, LOW);  // Отключаем передатчик
    digitalWrite(MODBUS_RE_PIN, LOW);  // Включаем приемник
}

// ✅ Неблокирующая задача реального датчика с ДИАГНОСТИКОЙ
static void realSensorTask(void* /*pvParameters*/)  // NOLINT(misc-use-internal-linkage,misc-use-anonymous-namespace)
{
    logPrintHeader("ПРОСТОЕ ЧТЕНИЕ ДАТЧИКА JXCT", LogColor::CYAN);
    logSystem("🔥 Использую РАБОЧИЕ параметры: 9600 bps, 8N1, адрес 1");
    logSystem("📊 Функция: периодическое чтение всех регистров датчика");

    for (;;)
    {
        // Простое чтение всех параметров датчика с рабочими настройками
        readSensorData();

        // Пауза между чтениями (настраиваемая в config в миллисекундах)
        vTaskDelay(pdMS_TO_TICKS(config.sensorReadInterval));
    }
}

void startRealSensorTask()
{
    // ✅ v3.10.0: Увеличиваем стек для задачи датчика из-за фильтрации
    xTaskCreate(realSensorTask, "RealSensor", 8192, nullptr, 1, nullptr);
}

// Функция для вывода ошибок Modbus
void printModbusError(uint8_t errNum) // NOLINT(misc-use-internal-linkage)
{
    switch (errNum)
    {
        case ModbusMaster::ku8MBSuccess:
            logSuccess("Modbus операция успешна");
            break;
        case ModbusMaster::ku8MBIllegalFunction:
            logError("Modbus: Illegal Function Exception");
            break;
        case ModbusMaster::ku8MBIllegalDataAddress:
            logError("Modbus: Illegal Data Address Exception");
            break;
        case ModbusMaster::ku8MBIllegalDataValue:
            logError("Modbus: Illegal Data Value Exception");
            break;
        case ModbusMaster::ku8MBSlaveDeviceFailure:
            logError("Modbus: Slave Device Failure");
            break;
        case ModbusMaster::ku8MBInvalidSlaveID:
            logError("Modbus: Invalid Slave ID");
            break;
        case ModbusMaster::ku8MBInvalidFunction:
            logError("Modbus: Invalid Function");
            break;
        case ModbusMaster::ku8MBResponseTimedOut:
            logError("Modbus: Response Timed Out");
            break;
        case ModbusMaster::ku8MBInvalidCRC:
            logError("Modbus: Invalid CRC");
            break;
        default:
            logErrorSafe("\1", errNum);
            break;
    }
}

// ========================================
// v2.3.0: РЕАЛИЗАЦИЯ СКОЛЬЗЯЩЕГО СРЕДНЕГО
// ========================================

void initMovingAverageBuffers(SensorData& data)
{
    // Инициализируем буферы нулями
    for (int i = 0; i < 15; ++i)
    {
        data.temp_buffer[i] = 0.0;
        data.hum_buffer[i] = 0.0;
        data.ec_buffer[i] = 0.0;
        data.ph_buffer[i] = 0.0;
        data.n_buffer[i] = 0.0;
        data.p_buffer[i] = 0.0;
        data.k_buffer[i] = 0.0;
    }
    data.buffer_index = 0;
    data.buffer_filled = 0;
    DEBUG_PRINTLN("[MOVING_AVG] Буферы скользящего среднего инициализированы");
}

void addToMovingAverage(SensorData& data, const SensorData& newReading)
{
    uint8_t window_size = std::max(static_cast<uint8_t>(5), std::min(static_cast<uint8_t>(15), config.movingAverageWindow));

    // Обновляем буферы
    data.temp_buffer[data.buffer_index] = newReading.temperature;
    data.hum_buffer[data.buffer_index] = newReading.humidity;
    data.ec_buffer[data.buffer_index] = newReading.ec;
    data.ph_buffer[data.buffer_index] = newReading.ph;
    data.n_buffer[data.buffer_index] = newReading.nitrogen;
    data.p_buffer[data.buffer_index] = newReading.phosphorus;
    data.k_buffer[data.buffer_index] = newReading.potassium;

    // Обновляем индекс
    data.buffer_index = (data.buffer_index + 1) % window_size;
    if (data.buffer_filled < window_size)
    {
        data.buffer_filled++;
    }

    // Вычисляем скользящее среднее
    data.temperature = calculateMovingAverage(data.temp_buffer, {window_size, data.buffer_filled});
    data.humidity = calculateMovingAverage(data.hum_buffer, {window_size, data.buffer_filled});
    data.ec = calculateMovingAverage(data.ec_buffer, {window_size, data.buffer_filled});
    data.ph = calculateMovingAverage(data.ph_buffer, {window_size, data.buffer_filled});
    data.nitrogen = calculateMovingAverage(data.n_buffer, {window_size, data.buffer_filled});
    data.phosphorus = calculateMovingAverage(data.p_buffer, {window_size, data.buffer_filled});
    data.potassium = calculateMovingAverage(data.k_buffer, {window_size, data.buffer_filled});
}

// Функция для получения текущих данных датчика
SensorData getSensorData()
{
    // Возвращаем копию текущих данных датчика
    SensorData result = sensorData;

    // Обновляем поле isValid для совместимости с веб-интерфейсом
    result.isValid = result.valid;
    result.timestamp = result.last_update;

    // Копируем значения в поля с правильными именами для веб-интерфейса
    result.conductivity = result.ec;
    result.moisture = result.humidity;

    return result;
}

