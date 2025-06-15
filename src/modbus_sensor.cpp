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

ModbusMaster modbus;
SensorData sensorData;
SensorCache sensorCache;
String sensorLastError = "";

// Используем все пины из централизованных констант jxct_constants.h
#define RX_PIN MODBUS_RX_PIN
#define TX_PIN MODBUS_TX_PIN
#define DE_PIN MODBUS_DE_PIN
#define RE_PIN MODBUS_RE_PIN

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

void testMAX485()
{
    logSystem("=== ТЕСТИРОВАНИЕ MAX485 ===");
    
    // Проверяем состояние пинов
    logSystem("Состояние пинов до теста:");
    logSystem("  DE_PIN (%d): %d", DE_PIN, digitalRead(DE_PIN));
    logSystem("  RE_PIN (%d): %d", RE_PIN, digitalRead(RE_PIN));
    
    // Режим передачи
    logSystem("Переключение в режим передачи...");
    digitalWrite(DE_PIN, HIGH);   // DE=HIGH для передачи
    digitalWrite(RE_PIN, HIGH);   // RE=HIGH отключает прием (правильно для TX)
    delay(10);  // Увеличил задержку
    logSystem("  DE_PIN (%d): %d", DE_PIN, digitalRead(DE_PIN));
    logSystem("  RE_PIN (%d): %d", RE_PIN, digitalRead(RE_PIN));
    
    // Тестовая передача
    logSystem("Отправка тестового байта 0x55...");
    Serial2.write(0x55);
    Serial2.flush();
    logSystem("Тестовый байт отправлен");
    
    // Режим приема
    logSystem("Переключение в режим приема...");
    digitalWrite(DE_PIN, LOW);    // DE=LOW для приема
    digitalWrite(RE_PIN, LOW);    // RE=LOW включает прием
    delay(100);  // Ждем ответ
    logSystem("  DE_PIN (%d): %d", DE_PIN, digitalRead(DE_PIN));
    logSystem("  RE_PIN (%d): %d", RE_PIN, digitalRead(RE_PIN));
    
    // Проверяем ответ
    if (Serial2.available())
    {
        String response = "Получен ответ: ";
        while (Serial2.available())
        {
            response += String(Serial2.read(), HEX) + " ";
        }
        logSuccess("%s", response.c_str());
    }
    else
    {
        logWarn("Нет ответа от MAX485 (это нормально без датчика)");
    }
    
    logSystem("=== ТЕСТ MAX485 ЗАВЕРШЕН ===");
}

void setupModbus()
{
    logPrintHeader("ИНИЦИАЛИЗАЦИЯ MODBUS", COLOR_CYAN);

    // Проверяем пины
    logSystem("RX_PIN: %d, TX_PIN: %d", RX_PIN, TX_PIN);
    logSystem("DE_PIN: %d, RE_PIN: %d", DE_PIN, RE_PIN);

    // Устанавливаем пины управления MAX485
    logSystem("Настройка пинов MAX485...");
    pinMode(DE_PIN, OUTPUT);
    pinMode(RE_PIN, OUTPUT);
    digitalWrite(DE_PIN, LOW);  // Режим приема
    digitalWrite(RE_PIN, LOW);  // Режим приема
    logSuccess("Пины MAX485 настроены");

    // 🔥 ВОССТАНОВЛЕНЫ РАБОЧИЕ ПАРАМЕТРЫ из документации:
    // Скорость: 9600 bps, Четность: 8N1, Адрес: 1
    logSystem("🔥 ВОССТАНОВЛЕНИЕ РАБОЧИХ ПАРАМЕТРОВ JXCT:");
    logSystem("   Документация: 9600 bps, 8N1, адрес 1");
    Serial2.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);  // РАБОЧИЕ ПАРАМЕТРЫ!
    delay(100);  // ОТКАТ: Критичный timing для Serial2
    logSystem("Serial2.available() после инициализации: %d", Serial2.available());

    // Инициализируем ModbusMaster с настройкой таймаутов
    logSystem("Инициализация ModbusMaster (адрес 1)...");
    modbus.begin(1, Serial2);
    
    // НАСТРОЙКА ТАЙМАУТОВ (критично для ESP32!)
    logSystem("Настройка таймаутов ModbusMaster...");
    
    // Если библиотека не поддерживает установку таймаутов, используем стандартные
    logSystem("Используем стандартные таймауты библиотеки ModbusMaster");
    
    // Устанавливаем функции управления направлением передачи
    modbus.preTransmission(preTransmission);
    modbus.postTransmission(postTransmission);
    logSuccess("ModbusMaster инициализирован на 9600 бод, 8N1");

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
    logSensor("=== ДИАГНОСТИКА MODBUS СВЯЗИ ===");
    
    // Тест 1: Попытка чтения регистра версии прошивки
    logSystem("Тест 1: Чтение версии прошивки (адрес 0x07)...");
    uint8_t result = modbus.readHoldingRegisters(0x07, 1);
    if (result == modbus.ku8MBSuccess) {
        uint16_t version = modbus.getResponseBuffer(0);
        logSuccess("✅ Modbus связь работает! Версия: %d.%d", (version >> 8) & 0xFF, version & 0xFF);
        return true;
    } else {
        logError("❌ Ошибка чтения версии: %d", result);
        printModbusError(result);
    }
    
    // Тест 2: Простое чтение первого доступного регистра
    logSystem("Тест 2: Чтение регистра 0x%04X (pH)...", REG_PH);
    result = modbus.readHoldingRegisters(REG_PH, 1);
    if (result == modbus.ku8MBSuccess) {
        uint16_t value = modbus.getResponseBuffer(0);
        logSuccess("✅ pH регистр читается! Значение: %d", value);
        return true;
    } else {
        logError("❌ Ошибка чтения pH: %d", result);
        printModbusError(result);
    }
    
    // Тест 3: Проверка статуса пинов DE/RE
    logSystem("Тест 3: Проверка конфигурации пинов...");
    logSystem("DE_PIN: %d, RE_PIN: %d", DE_PIN, RE_PIN);
    logSystem("DE состояние: %d, RE состояние: %d", digitalRead(DE_PIN), digitalRead(RE_PIN));
    
    // Тест 4: Проверка Serial2
    logSystem("Тест 4: Проверка Serial2...");
    logSystem("Serial2 доступен: %s", Serial2.available() ? "ДА" : "НЕТ");
    
    return false;
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
    if (readSingleRegister(REG_PH, "pH", 0.01f, &sensorData.ph, true))
        success_count++;
    
    // Влажность (÷ 10)
    if (readSingleRegister(REG_SOIL_MOISTURE, "Влажность", 0.1f, &sensorData.humidity, true))
        success_count++;
    
    // Температура (÷ 10)
    if (readSingleRegister(REG_SOIL_TEMP, "Температура", 0.1f, &sensorData.temperature, true))
        success_count++;
    
    // EC (без деления)
    if (readSingleRegister(REG_CONDUCTIVITY, "EC", 1.0f, &sensorData.ec, true))
        success_count++;
    
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
    if (readSingleRegister(REG_NITROGEN, "Азот", 1.0f, &sensorData.nitrogen, true))
        success_count++;
    
    // Фосфор
    if (readSingleRegister(REG_PHOSPHORUS, "Фосфор", 1.0f, &sensorData.phosphorus, true))
        success_count++;
    
    // Калий
    if (readSingleRegister(REG_POTASSIUM, "Калий", 1.0f, &sensorData.potassium, true))
        success_count++;
    
    return success_count;
}

/**
 * @brief Финализация данных датчика (валидация, кэширование, скользящее среднее)
 * @param success Флаг успешности чтения всех параметров
 */
void finalizeSensorData(bool success)
{
    sensorData.valid = success;
    sensorData.last_update = millis();

    if (success)
    {
        // Добавляем данные в буферы скользящего среднего
        addToMovingAverage(sensorData, sensorData.temperature, sensorData.humidity, 
                          sensorData.ec, sensorData.ph, sensorData.nitrogen, 
                          sensorData.phosphorus, sensorData.potassium);

        // Валидация данных
        if (validateSensorData(sensorData))
        {
            logSuccess("✅ Все параметры прочитаны и валидны");
            
            // Обновляем кэш
            sensorCache.data = sensorData;
            sensorCache.timestamp = millis();
            sensorCache.is_valid = true;
        }
        else
        {
            logWarn("⚠️ Данные прочитаны, но не прошли валидацию");
            sensorData.valid = false;
        }
    }
    else
    {
        logError("❌ Не удалось прочитать один или несколько параметров");
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

void preTransmission()
{
    DEBUG_PRINTLN("Modbus TX режим");
    digitalWrite(DE_PIN, HIGH);  // DE=HIGH включает передачу
    digitalWrite(RE_PIN, HIGH);  // RE=HIGH отключает прием (для передачи)
    delayMicroseconds(50);  // ✅ Микросекундные задержки критичны для Modbus
}

void postTransmission()
{
    delayMicroseconds(50);  // ✅ Микросекундные задержки критичны для Modbus
    digitalWrite(DE_PIN, LOW);   // DE=LOW отключает передачу
    digitalWrite(RE_PIN, LOW);   // RE=LOW включает прием (для ответов датчика!)
    DEBUG_PRINTLN("Modbus RX режим");
}

// ✅ Неблокирующая задача реального датчика с ДИАГНОСТИКОЙ
void realSensorTask(void* pvParameters)
{
    logPrintHeader("ПРОСТОЕ ЧТЕНИЕ ДАТЧИКА JXCT", COLOR_CYAN);
    logSystem("🔥 Использую РАБОЧИЕ параметры: 9600 bps, 8N1, адрес 1");
    logSystem("📊 Функция: периодическое чтение всех регистров датчика");
    
    for (;;) {
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
    for (int i = 0; i < 15; i++) {
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
    if (window_size < 5) window_size = 5;     // Минимум 5
    if (window_size > 15) window_size = 15;   // Максимум 15
    
    // Добавляем новые значения в кольцевые буферы
    data.temp_buffer[data.buffer_index] = temp;
    data.hum_buffer[data.buffer_index] = hum;
    data.ec_buffer[data.buffer_index] = ec;
    data.ph_buffer[data.buffer_index] = ph;
    data.n_buffer[data.buffer_index] = n;
    data.p_buffer[data.buffer_index] = p;
    data.k_buffer[data.buffer_index] = k;
    
    // Обновляем индекс (кольцевой буфер)
    data.buffer_index = (data.buffer_index + 1) % window_size;
    
    // Обновляем количество заполненных элементов
    if (data.buffer_filled < window_size) {
        data.buffer_filled++;
    }
    
    // Вычисляем скользящее среднее только если буфер достаточно заполнен (минимум 3 значения)
    if (data.buffer_filled >= 3) {
        // Используем реальный размер заполненных данных для расчета
        uint8_t effective_window = (data.buffer_filled < window_size) ? data.buffer_filled : window_size;
        
        data.temperature = calculateMovingAverage(data.temp_buffer, effective_window, data.buffer_filled);
        data.humidity = calculateMovingAverage(data.hum_buffer, effective_window, data.buffer_filled);
        data.ec = calculateMovingAverage(data.ec_buffer, effective_window, data.buffer_filled);
        data.ph = calculateMovingAverage(data.ph_buffer, effective_window, data.buffer_filled);
        data.nitrogen = calculateMovingAverage(data.n_buffer, effective_window, data.buffer_filled);
        data.phosphorus = calculateMovingAverage(data.p_buffer, effective_window, data.buffer_filled);
        data.potassium = calculateMovingAverage(data.k_buffer, effective_window, data.buffer_filled);
        
        DEBUG_PRINTF("[MOVING_AVG] Окно=%d, заполнено=%d, Темп=%.1f°C\n", effective_window, data.buffer_filled, data.temperature);
    } else {
        // Если данных мало, используем последние значения без усреднения
        data.temperature = temp;
        data.humidity = hum;
        data.ec = ec;
        data.ph = ph;
        data.nitrogen = n;
        data.phosphorus = p;
        data.potassium = k;
        
        DEBUG_PRINTF("[MOVING_AVG] Накопление данных: %d/%d\n", data.buffer_filled, window_size);
    }
}

float calculateMovingAverage(float* buffer, uint8_t window_size, uint8_t filled)
{
    if (filled == 0) return 0.0;
    
    // Берем последние filled элементов (или window_size, если filled >= window_size)
    uint8_t elements_to_use = (filled < window_size) ? filled : window_size;
    
    // v2.4.1: Используем настраиваемый алгоритм (среднее или медиана)
    extern Config config;
    
    if (config.filterAlgorithm == 1) {  // FILTER_ALGORITHM_MEDIAN
        // Создаем временный массив для медианы
        float temp_values[15];  // Максимальный размер окна
        for (uint8_t i = 0; i < elements_to_use; i++) {
            temp_values[i] = buffer[i];
        }
        
        // Простая сортировка для медианы
        for (uint8_t i = 0; i < elements_to_use - 1; i++) {
            for (uint8_t j = 0; j < elements_to_use - i - 1; j++) {
                if (temp_values[j] > temp_values[j + 1]) {
                    float temp = temp_values[j];
                    temp_values[j] = temp_values[j + 1];
                    temp_values[j + 1] = temp;
                }
            }
        }
        
        // Возвращаем медиану
        if (elements_to_use % 2 == 0) {
            return (temp_values[elements_to_use/2 - 1] + temp_values[elements_to_use/2]) / 2.0f;
        } else {
            return temp_values[elements_to_use/2];
        }
    } else {
        // FILTER_ALGORITHM_MEAN (по умолчанию)
        float sum = 0.0;
        for (uint8_t i = 0; i < elements_to_use; i++) {
            sum += buffer[i];
        }
        return sum / elements_to_use;
    }
}

// Функция для получения текущих данных датчика
SensorData getSensorData() {
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