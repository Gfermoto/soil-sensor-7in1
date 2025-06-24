/**
 * @file modbus_sensor.cpp
 * @brief Работа с Modbus-датчиком JXCT
 * @details Реализация функций и задач для опроса датчика по Modbus RTU, преобразование данных, управление линией
 * передачи.
 */
#include <Arduino.h>
#include "modbus_sensor.h"
#include "jxct_device_info.h"
#include "jxct_config_vars.h"
#include "debug.h"  // ✅ Добавляем систему условной компиляции
#include "logger.h"
#include "jxct_constants.h"  // ✅ Централизованные константы
#include "sensor_compensation.h"
#include "calibration_manager.h"

ModbusMaster modbus;
SensorData sensorData;
SensorCache sensorCache;
String sensorLastError = "";

static unsigned long lastIrrigationTs = 0;

void debugPrintBuffer(const char* prefix, uint8_t* buffer, size_t length)
{
    if (currentLogLevel < LOG_DEBUG) return;

    String hex_str = "";
    for (size_t i = 0; i < length; i++)
    {
        if (buffer[i] < 0x10) hex_str += "0";
        hex_str += String(buffer[i], HEX);
        hex_str += " ";
    }
    logDebug("%s%s", prefix, hex_str.c_str());
}

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
    pinMode(MODBUS_DE_PIN, OUTPUT);      // Driver Enable - управление передатчиком
    pinMode(MODBUS_RE_PIN, OUTPUT);      // Receiver Enable - управление приемником
    
    // Тест 1: Включаем передачу, выключаем прием
    digitalWrite(MODBUS_DE_PIN, HIGH);   // Активируем передатчик
    digitalWrite(MODBUS_RE_PIN, HIGH);   // Отключаем приемник
    delay(10);
    
    // Тест 2: Включаем прием, выключаем передачу
    digitalWrite(MODBUS_DE_PIN, LOW);    // Деактивируем передатчик
    digitalWrite(MODBUS_RE_PIN, LOW);    // Активируем приемник
    
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
    logPrintHeader("ИНИЦИАЛИЗАЦИЯ MODBUS", COLOR_CYAN);

    // Устанавливаем пины управления SP3485E
    logSystem("Настройка пинов SP3485E...");
    pinMode(MODBUS_DE_PIN, OUTPUT);      // Driver Enable - GPIO4
    pinMode(MODBUS_RE_PIN, OUTPUT);      // Receiver Enable - GPIO5
    
    // Начальное состояние: прием включен, передача выключена
    digitalWrite(MODBUS_DE_PIN, LOW);    // Передатчик в высокоимпедансном состоянии
    digitalWrite(MODBUS_RE_PIN, LOW);    // Приемник активен
    
    logSystem("DE пин: %d, RE пин: %d", MODBUS_DE_PIN, MODBUS_RE_PIN);
    logSuccess("Пины SP3485E настроены");

    // Инициализация UART для Modbus
    Serial2.begin(9600, SERIAL_8N1, MODBUS_RX_PIN, MODBUS_TX_PIN);
    
    // Настройка Modbus с обработчиками переключения режима
    modbus.begin(JXCT_MODBUS_ID, Serial2);
    modbus.preTransmission(preTransmission);   // Вызывается перед передачей
    modbus.postTransmission(postTransmission); // Вызывается после передачи
    
    logSuccess("Modbus инициализирован");
    logPrintHeader("MODBUS ГОТОВ ДЛЯ ПОЛНОГО ТЕСТИРОВАНИЯ", COLOR_GREEN);
}

// Функция для расчета CRC16 Modbus
uint16_t calculateCRC16(uint8_t* data, size_t length)
{
    uint16_t crc = 0xFFFF;

    for (size_t i = 0; i < length; i++)
    {
        crc ^= (uint16_t)data[i];
        for (int j = 0; j < 8; j++)
        {
            if (crc & 0x0001)
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

bool validateSensorData(SensorData& data)
{
    if (data.temperature < MIN_TEMPERATURE || data.temperature > MAX_TEMPERATURE) return false;
    if (data.humidity < MIN_HUMIDITY || data.humidity > MAX_HUMIDITY) return false;
    if (data.ec < MIN_EC || data.ec > MAX_EC) return false;
    if (data.ph < MIN_PH || data.ph > MAX_PH) return false;
    if (data.nitrogen < MIN_NPK || data.nitrogen > MAX_NPK) return false;
    if (data.phosphorus < MIN_NPK || data.phosphorus > MAX_NPK) return false;
    if (data.potassium < MIN_NPK || data.potassium > MAX_NPK) return false;
    return true;
}

bool getCachedData(SensorData& data)
{
    if (!sensorCache.is_valid) return false;
    if (millis() - sensorCache.timestamp > MODBUS_CACHE_TIMEOUT) return false;

    data = sensorCache.data;
    return true;
}

bool readFirmwareVersion()
{
    logSensor("Запрос версии прошивки датчика...");
    uint8_t result = modbus.readHoldingRegisters(0x07, 1);

    if (result == modbus.ku8MBSuccess)
    {
        uint16_t version = modbus.getResponseBuffer(0);
        logSuccess("Версия прошивки датчика: %d.%d", (version >> 8) & 0xFF, version & 0xFF);
        return true;
    }
    else
    {
        logError("Ошибка чтения версии прошивки: %d", result);
        printModbusError(result);
        return false;
    }
}

bool readErrorStatus()
{
    uint8_t result = modbus.readHoldingRegisters(REG_ERROR_STATUS, 1);
    if (result == modbus.ku8MBSuccess)
    {
        sensorData.error_status = modbus.getResponseBuffer(0);
        return true;
    }
    return false;
}

bool changeDeviceAddress(uint8_t new_address)
{
    if (new_address < 1 || new_address > 247) return false;

    uint8_t result = modbus.writeSingleRegister(REG_DEVICE_ADDRESS, new_address);
    if (result == modbus.ku8MBSuccess)
    {
        // ✅ Неблокирующая задержка через vTaskDelay
        delay(100);  // ОТКАТ: Критичный timing для Modbus
        modbus.begin(new_address, Serial2);
        return true;
    }
    return false;
}

// Добавляем функцию диагностики Modbus связи
bool testModbusConnection()
{
    logSystem("=== ТЕСТ MODBUS СОЕДИНЕНИЯ ===");
    
    // Проверяем пины
    logSystem("DE пин: %d, RE пин: %d", MODBUS_DE_PIN, MODBUS_RE_PIN);
    
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
    unsigned long pre_delay = micros() - start_time;
    
    start_time = micros();
    postTransmission();
    unsigned long post_delay = micros() - start_time;
    
    logSystem("Задержка preTransmission: %lu мкс", pre_delay);
    logSystem("Задержка postTransmission: %lu мкс", post_delay);
    
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
    if (Serial2.baudRate() == 9600)
    {
        logSuccess("Скорость UART настроена правильно: 9600");
    }
    else
    {
        logError("Неверная скорость UART: %d", Serial2.baudRate());
        return false;
    }

    // Тест 4: Попытка чтения регистра версии прошивки
    logSystem("Тест 4: Чтение версии прошивки...");
    uint8_t result = modbus.readHoldingRegisters(0x00, 1);
    if (result == modbus.ku8MBSuccess)
    {
        logSuccess("Успешно прочитан регистр версии");
    }
    else
    {
        logError("Ошибка чтения регистра версии: %02X", result);
        return false;
    }
    
    logSuccess("=== ТЕСТ MODBUS ЗАВЕРШЕН УСПЕШНО ===");
    return true;
    }

// ============================================================================
// ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ ДЛЯ СНИЖЕНИЯ ЦИКЛОМАТИЧЕСКОЙ СЛОЖНОСТИ
// ============================================================================

/**
 * @brief Чтение одного регистра с обработкой ошибок
 * @param reg_addr Адрес регистра
 * @param reg_name Название регистра для логирования
 * @param multiplier Множитель для конвертации
 * @param target Указатель на переменную для сохранения результата
 * @param is_float Флаг - сохранять как float или int
 * @return true если чтение успешно
 */
bool readSingleRegister(uint16_t reg_addr, const char* reg_name, float multiplier, void* target, bool is_float)
{
    logDebug("Чтение %s (0x%04X)...", reg_name, reg_addr);
    uint8_t result = modbus.readHoldingRegisters(reg_addr, 1);

    if (result == modbus.ku8MBSuccess)
    {
        uint16_t raw_value = modbus.getResponseBuffer(0);

        if (is_float)
        {
            float* float_target = static_cast<float*>(target);
            *float_target = convertRegisterToFloat(raw_value, multiplier);
            logDebug("%s: %.2f", reg_name, *float_target);
    }
    else
    {
            uint16_t* int_target = static_cast<uint16_t*>(target);
            *int_target = raw_value;
            logDebug("%s: %d", reg_name, *int_target);
        }
        return true;
    }
    else
    {
        logError("Ошибка чтения %s: %d", reg_name, result);
        printModbusError(result);
        return false;
    }
}

/**
 * @brief Чтение основных параметров (температура, влажность, pH, EC)
 * @return Количество успешно прочитанных параметров
 */
int readBasicParameters()
{
    int success_count = 0;

    // pH (÷ 100)
    if (readSingleRegister(REG_PH, "pH", 0.01f, &sensorData.ph, true)) success_count++;

    // Влажность (÷ 10)
    if (readSingleRegister(REG_SOIL_MOISTURE, "Влажность", 0.1f, &sensorData.humidity, true)) success_count++;

    // Температура (÷ 10)
    if (readSingleRegister(REG_SOIL_TEMP, "Температура", 0.1f, &sensorData.temperature, true)) success_count++;

    // EC (без деления)
    if (readSingleRegister(REG_CONDUCTIVITY, "EC", 1.0f, &sensorData.ec, true)) success_count++;

    return success_count;
}

/**
 * @brief Чтение NPK параметров (азот, фосфор, калий)
 * @return Количество успешно прочитанных параметров
 */
int readNPKParameters()
{
    int success_count = 0;

    // Азот
    if (readSingleRegister(REG_NITROGEN, "Азот", 1.0f, &sensorData.nitrogen, true)) success_count++;

    // Фосфор
    if (readSingleRegister(REG_PHOSPHORUS, "Фосфор", 1.0f, &sensorData.phosphorus, true)) success_count++;

    // Калий
    if (readSingleRegister(REG_POTASSIUM, "Калий", 1.0f, &sensorData.potassium, true)) success_count++;

    return success_count;
    }

// ------------------------------------------------------------
// 🔽 Helper functions to reduce cyclomatic complexity
// ------------------------------------------------------------

static void saveRawSnapshot(SensorData& d)
{
    d.raw_temperature = d.temperature;
    d.raw_humidity    = d.humidity;
    d.raw_ec          = d.ec;
    d.raw_ph          = d.ph;
    d.raw_nitrogen    = d.nitrogen;
    d.raw_phosphorus  = d.phosphorus;
    d.raw_potassium   = d.potassium;
}

static void updateIrrigationFlag(SensorData& d)
{
    constexpr uint8_t WIN = 6;
    static float buf[WIN] = {NAN};
    static uint8_t idx = 0, filled = 0, persist = 0;

    float baseline = d.humidity;
    for (uint8_t i = 0; i < filled; ++i)
        baseline = (buf[i] < baseline) ? buf[i] : baseline;

    bool spike = (filled == WIN) &&
                 (d.humidity - baseline >= config.irrigationSpikeThreshold) &&
                 (d.humidity > 25.0f);
    persist = spike ? persist + 1 : 0;
    if (persist >= 2) {
        lastIrrigationTs = millis();
        persist = 0;
    }

    buf[idx] = d.humidity;
    idx = (idx + 1) % WIN;
    if (filled < WIN) ++filled;

    d.recentIrrigation = (millis() - lastIrrigationTs) <= (unsigned long)config.irrigationHoldMinutes * 60000UL;
}

static void applyCompensationIfEnabled(SensorData& d)
{
    if (!config.flags.calibrationEnabled) return;

    SoilType soil = SoilType::LOAM;
    SoilProfile profile = SoilProfile::SAND;
    switch (config.soilProfile) {
        case 0: soil = SoilType::SAND; profile = SoilProfile::SAND; break;
        case 1: soil = SoilType::LOAM; profile = SoilProfile::LOAM; break;
        case 2: soil = SoilType::PEAT; profile = SoilProfile::PEAT; break;
        case 3: soil = SoilType::CLAY; profile = SoilProfile::CLAY; break;
        case 4: soil = SoilType::SANDPEAT; profile = SoilProfile::SANDPEAT; break;
    }

    // Шаг 1: Применяем калибровочную таблицу CSV (лабораторная поверка)
    float tempCalibrated = CalibrationManager::applyCalibration(d.temperature, profile);
    float humCalibrated = CalibrationManager::applyCalibration(d.humidity, profile);
    float ecCalibrated = CalibrationManager::applyCalibration(d.ec, profile);
    float phCalibrated = CalibrationManager::applyCalibration(d.ph, profile);
    float nCalibrated = CalibrationManager::applyCalibration(d.nitrogen, profile);
    float pCalibrated = CalibrationManager::applyCalibration(d.phosphorus, profile);
    float kCalibrated = CalibrationManager::applyCalibration(d.potassium, profile);

    // Шаг 2: Применяем математическую компенсацию (температурная, влажностная)
    float ec25 = ecCalibrated / (1.0f + 0.021f * (tempCalibrated - 25.0f));
    d.ec = correctEC(ec25, tempCalibrated, humCalibrated, soil);

    d.ph = correctPH(phCalibrated, tempCalibrated);

    d.nitrogen = nCalibrated;
    d.phosphorus = pCalibrated;
    d.potassium = kCalibrated;
    correctNPK(tempCalibrated, humCalibrated, d.nitrogen, d.phosphorus, d.potassium, soil);

    // Обновляем остальные значения
    d.temperature = tempCalibrated;
    d.humidity = humCalibrated;
}

/**
 * @brief Финализация данных датчика (валидация, кэширование, скользящее среднее)
 * @param success Флаг успешности чтения всех параметров
 */
void finalizeSensorData(bool success)
{
    sensorData.valid       = success;
    sensorData.last_update = millis();

    if (!success) {
        logError("❌ Не удалось прочитать один или несколько параметров");
        return;
    }

    saveRawSnapshot(sensorData);
    updateIrrigationFlag(sensorData);
    applyCompensationIfEnabled(sensorData);

    addToMovingAverage(sensorData,
                       sensorData.temperature,
                       sensorData.humidity,
                       sensorData.ec,
                       sensorData.ph,
                       sensorData.nitrogen,
                       sensorData.phosphorus,
                       sensorData.potassium);

    if (validateSensorData(sensorData)) {
        logSuccess("✅ Все параметры прочитаны и валидны");
        sensorCache = {sensorData, true, millis()};
    } else {
        logWarn("⚠️ Данные прочитаны, но не прошли валидацию");
        sensorData.valid = false;
    }
}

// ============================================================================
// ОСНОВНАЯ ФУНКЦИЯ ЧТЕНИЯ ДАТЧИКА (РЕФАКТОРИНГ)
// ============================================================================

void readSensorData()
{
    logSensor("Чтение всех параметров JXCT 7-в-1 датчика...");

    // Читаем основные параметры (4 параметра)
    int basic_success = readBasicParameters();

    // Читаем NPK параметры (3 параметра)
    int npk_success = readNPKParameters();

    // Общий успех - все 7 параметров прочитаны
    bool total_success = (basic_success == 4) && (npk_success == 3);

    // Финализируем данные
    finalizeSensorData(total_success);
}

float convertRegisterToFloat(uint16_t value, float multiplier)
{
    return value * multiplier;
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
    delayMicroseconds(50);             // Ждем стабилизации сигналов
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
void realSensorTask(void* pvParameters)
{
    logPrintHeader("ПРОСТОЕ ЧТЕНИЕ ДАТЧИКА JXCT", COLOR_CYAN);
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
    xTaskCreate(realSensorTask, "RealSensor", 4096, NULL, 1, NULL);
}

// Функция для вывода ошибок Modbus
void printModbusError(uint8_t errNum)
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
            logError("Modbus: Неизвестная ошибка %d", errNum);
            break;
    }
}

// ========================================
// v2.3.0: РЕАЛИЗАЦИЯ СКОЛЬЗЯЩЕГО СРЕДНЕГО
// ========================================

void initMovingAverageBuffers(SensorData& data)
{
    // Инициализируем буферы нулями
    for (int i = 0; i < 15; i++)
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

void addToMovingAverage(SensorData& data, float temp, float hum, float ec, float ph, float n, float p, float k)
{
    uint8_t window_size = config.movingAverageWindow;
    if (window_size < 5) window_size = 5;
    if (window_size > 15) window_size = 15;

    // ---------- O(1) running sum для каждого параметра ----------
    static float sum_temp = 0, sum_hum = 0, sum_ec = 0, sum_ph = 0, sum_n = 0, sum_p = 0, sum_k = 0;

    // Если буфер заполнен – вычитаем значение, которое покинет окно
    if (data.buffer_filled >= window_size) {
        sum_temp -= data.temp_buffer[data.buffer_index];
        sum_hum  -= data.hum_buffer[data.buffer_index];
        sum_ec   -= data.ec_buffer[data.buffer_index];
        sum_ph   -= data.ph_buffer[data.buffer_index];
        sum_n    -= data.n_buffer[data.buffer_index];
        sum_p    -= data.p_buffer[data.buffer_index];
        sum_k    -= data.k_buffer[data.buffer_index];
    }

    // Записываем новые значения в буфер и добавляем их к сумме
    data.temp_buffer[data.buffer_index] = temp; sum_temp += temp;
    data.hum_buffer[data.buffer_index]  = hum;  sum_hum  += hum;
    data.ec_buffer[data.buffer_index]   = ec;   sum_ec   += ec;
    data.ph_buffer[data.buffer_index]   = ph;   sum_ph   += ph;
    data.n_buffer[data.buffer_index]    = n;    sum_n    += n;
    data.p_buffer[data.buffer_index]    = p;    sum_p    += p;
    data.k_buffer[data.buffer_index]    = k;    sum_k    += k;

    // Обновляем индексы кольцевого буфера
    data.buffer_index = (data.buffer_index + 1) % window_size;
    if (data.buffer_filled < window_size) data.buffer_filled++;

    uint8_t effective_window = (data.buffer_filled < window_size) ? data.buffer_filled : window_size;

    if (effective_window >= 3 && config.filterAlgorithm == 0) {
        // Среднее (O(1))
        data.temperature = sum_temp / effective_window;
        data.humidity    = sum_hum  / effective_window;
        data.ec          = sum_ec   / effective_window;
        data.ph          = sum_ph   / effective_window;
        data.nitrogen    = sum_n    / effective_window;
        data.phosphorus  = sum_p    / effective_window;
        data.potassium   = sum_k    / effective_window;

        DEBUG_PRINTF("[AVG O1] win=%d temp=%.1f\n", effective_window, data.temperature);
    }
    else if (effective_window >= 3) {
        // Оставляем старый алгоритм для медианы и др.
        data.temperature = calculateMovingAverage(data.temp_buffer, window_size, data.buffer_filled);
        data.humidity    = calculateMovingAverage(data.hum_buffer, window_size, data.buffer_filled);
        data.ec          = calculateMovingAverage(data.ec_buffer, window_size, data.buffer_filled);
        data.ph          = calculateMovingAverage(data.ph_buffer, window_size, data.buffer_filled);
        data.nitrogen    = calculateMovingAverage(data.n_buffer, window_size, data.buffer_filled);
        data.phosphorus  = calculateMovingAverage(data.p_buffer, window_size, data.buffer_filled);
        data.potassium   = calculateMovingAverage(data.k_buffer, window_size, data.buffer_filled);
    } else {
        // Пока мало данных – возвращаем последние значения
        data.temperature = temp;
        data.humidity    = hum;
        data.ec          = ec;
        data.ph          = ph;
        data.nitrogen    = n;
        data.phosphorus  = p;
        data.potassium   = k;
    }
}

float calculateMovingAverage(float* buffer, uint8_t window_size, uint8_t filled)
{
    if (filled == 0) return 0.0;
    
    // Берем последние filled элементов (или window_size, если filled >= window_size)
    uint8_t elements_to_use = (filled < window_size) ? filled : window_size;
    
    // v2.4.1: Используем настраиваемый алгоритм (среднее или медиана)
    extern Config config;
    
    if (config.filterAlgorithm == 1)
    {  // FILTER_ALGORITHM_MEDIAN
        // Создаем временный массив для медианы
        float temp_values[15];  // Максимальный размер окна
        for (uint8_t i = 0; i < elements_to_use; i++)
        {
            temp_values[i] = buffer[i];
        }
        
        // Простая сортировка для медианы
        for (uint8_t i = 0; i < elements_to_use - 1; i++)
        {
            for (uint8_t j = 0; j < elements_to_use - i - 1; j++)
            {
                if (temp_values[j] > temp_values[j + 1])
                {
                    float temp = temp_values[j];
                    temp_values[j] = temp_values[j + 1];
                    temp_values[j + 1] = temp;
                }
            }
        }
        
        // Возвращаем медиану
        if (elements_to_use % 2 == 0)
        {
            return (temp_values[elements_to_use / 2 - 1] + temp_values[elements_to_use / 2]) / 2.0f;
        }
        else
        {
            return temp_values[elements_to_use / 2];
        }
    }
    else
    {
        // FILTER_ALGORITHM_MEAN (по умолчанию)
        float sum = 0.0;
        for (uint8_t i = 0; i < elements_to_use; i++)
        {
            sum += buffer[i];
        }
        return sum / elements_to_use;
    }
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