/**
 * @file modbus_sensor.cpp
 * @brief Работа с Modbus-датчиком JXCT
 * @details Реализация функций и задач для опроса датчика по Modbus RTU, преобразование данных, управление линией передачи.
 */
#include <Arduino.h>
#include "modbus_sensor.h"
#include "jxct_device_info.h"
#include "jxct_config_vars.h"
#include "logger.h"

ModbusMaster modbus;
SensorData sensorData;
SensorCache sensorCache;
String sensorLastError = "";

// Константы для работы с датчиком
const unsigned long CACHE_TIMEOUT = 5000;  // 5 секунд
const uint8_t MAX_RETRIES = 3;            // Максимальное количество попыток чтения
const unsigned long RETRY_DELAY = 1000;    // Задержка между попытками

// Определяем пины для DE и RE
#define DE_PIN 4
#define RE_PIN 5

void debugPrintBuffer(const char* prefix, uint8_t* buffer, size_t length) {
    if (currentLogLevel < LOG_DEBUG) return;
    
    String hex_str = "";
    for(size_t i = 0; i < length; i++) {
        if(buffer[i] < 0x10) hex_str += "0";
        hex_str += String(buffer[i], HEX);
        hex_str += " ";
    }
    logDebug("%s%s", prefix, hex_str.c_str());
}

void testMAX485() {
    logDebug("Тест MAX485...");
    digitalWrite(DE_PIN, HIGH);
    digitalWrite(RE_PIN, HIGH);
    delay(2);
    Serial2.write(0x55);
    Serial2.flush();
    digitalWrite(DE_PIN, LOW);
    digitalWrite(RE_PIN, LOW);
    delay(1000);
    if (Serial2.available()) {
        String response = "Получен ответ: ";
        while (Serial2.available()) {
            response += String(Serial2.read(), HEX) + " ";
        }
        logDebug("%s", response.c_str());
    } else {
        logDebug("Нет ответа от MAX485");
    }
}

void setupModbus() {
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
    
    // Инициализируем Serial2 на скорости 9600 (из примера)
    logSystem("Инициализация Serial2 на 9600 бод...");
    Serial2.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);
    delay(100);
    logSuccess("Serial2 инициализирован");
    
    // Инициализируем ModbusMaster с адресом устройства 1
    logSystem("Инициализация ModbusMaster...");
    modbus.begin(1, Serial2);
    
    // Устанавливаем колбэки для управления DE/RE
    modbus.preTransmission(preTransmission);
    modbus.postTransmission(postTransmission);
    
    logSuccess("ModbusMaster инициализирован успешно");
    logPrintSeparator("─", 60);
}

// Функция для расчета CRC16 Modbus
uint16_t calculateCRC16(uint8_t* data, size_t length) {
    uint16_t crc = 0xFFFF;
    
    for (size_t i = 0; i < length; i++) {
        crc ^= (uint16_t)data[i];
        for (int j = 0; j < 8; j++) {
            if (crc & 0x0001) {
                crc = (crc >> 1) ^ 0xA001;
            } else {
                crc = crc >> 1;
            }
        }
    }
    
    return crc;
}

bool validateSensorData(SensorData& data) {
    if (data.temperature < MIN_TEMPERATURE || data.temperature > MAX_TEMPERATURE) return false;
    if (data.humidity < MIN_HUMIDITY || data.humidity > MAX_HUMIDITY) return false;
    if (data.ec < MIN_EC || data.ec > MAX_EC) return false;
    if (data.ph < MIN_PH || data.ph > MAX_PH) return false;
    if (data.nitrogen < MIN_NPK || data.nitrogen > MAX_NPK) return false;
    if (data.phosphorus < MIN_NPK || data.phosphorus > MAX_NPK) return false;
    if (data.potassium < MIN_NPK || data.potassium > MAX_NPK) return false;
    return true;
}

bool getCachedData(SensorData& data) {
    if (!sensorCache.is_valid) return false;
    if (millis() - sensorCache.timestamp > CACHE_TIMEOUT) return false;
    
    data = sensorCache.data;
    return true;
}

bool readFirmwareVersion() {
    logSensor("Запрос версии прошивки датчика...");
    uint8_t result = modbus.readHoldingRegisters(0x07, 1);
    
    if (result == modbus.ku8MBSuccess) {
        uint16_t version = modbus.getResponseBuffer(0);
        logSuccess("Версия прошивки датчика: %d.%d", (version >> 8) & 0xFF, version & 0xFF);
        return true;
    } else {
        logError("Ошибка чтения версии прошивки: %d", result);
        printModbusError(result);
        return false;
    }
}

bool readErrorStatus() {
    uint8_t result = modbus.readHoldingRegisters(REG_ERROR_STATUS, 1);
    if (result == modbus.ku8MBSuccess) {
        sensorData.error_status = modbus.getResponseBuffer(0);
        return true;
    }
    return false;
}

bool changeDeviceAddress(uint8_t new_address) {
    if (new_address < 1 || new_address > 247) return false;
    
    uint8_t result = modbus.writeSingleRegister(REG_DEVICE_ADDRESS, new_address);
    if (result == modbus.ku8MBSuccess) {
        delay(100); // Даем время на применение нового адреса
        modbus.begin(new_address, Serial2);
        return true;
    }
    return false;
}

void readSensorData() {
    logSensor("Чтение всех параметров JXCT 7-в-1 датчика...");
    
    uint8_t result;
    bool success = true;
    
    // 1. PH (адрес 0x0006, ÷ 100)
    logDebug("Чтение PH (0x0006)...");
    result = modbus.readHoldingRegisters(0x0006, 1);
    if (result == modbus.ku8MBSuccess) {
        uint16_t ph_raw = modbus.getResponseBuffer(0);
        sensorData.ph = (float)ph_raw / 100.0;  // Делим на 100
        logData("PH: %.2f", sensorData.ph);
    } else {
        logError("Ошибка чтения PH: %d", result);
        success = false;
    }
    
    // 2. Влажность почвы (адрес 0x0012, ÷ 10)
    logDebug("Чтение влажности (0x0012)...");
    result = modbus.readHoldingRegisters(0x0012, 1);
    if (result == modbus.ku8MBSuccess) {
        uint16_t moisture_raw = modbus.getResponseBuffer(0);
        sensorData.humidity = (float)moisture_raw / 10.0;  // Записываем в humidity для веб-интерфейса
        sensorData.moisture = sensorData.humidity;          // Дублируем в moisture
        logData("Влажность: %.1f%%", sensorData.humidity);
    } else {
        logError("Ошибка чтения влажности: %d", result);
        success = false;
    }
    
    // 3. Температура (адрес 0x0013, ÷ 10)
    logDebug("Чтение температуры (0x0013)...");
    result = modbus.readHoldingRegisters(0x0013, 1);
    if (result == modbus.ku8MBSuccess) {
        uint16_t temp_raw = modbus.getResponseBuffer(0);
        sensorData.temperature = (float)temp_raw / 10.0;  // Делим на 10
        logData("Температура: %.1f°C", sensorData.temperature);
    } else {
        logError("Ошибка чтения температуры: %d", result);
        success = false;
    }
    
    // 4. Электропроводность (адрес 0x0015, как есть)
    logDebug("Чтение проводимости (0x0015)...");
    result = modbus.readHoldingRegisters(0x0015, 1);
    if (result == modbus.ku8MBSuccess) {
        uint16_t conductivity_raw = modbus.getResponseBuffer(0);
        sensorData.ec = (float)conductivity_raw;  // Как есть, µS/cm
        logData("Проводимость: %.0f µS/cm", sensorData.ec);
    } else {
        logError("Ошибка чтения проводимости: %d", result);
        success = false;
    }
    
    // 5-7. NPK одним запросом (адреса 0x001E-0x0020, как есть)
    logDebug("Чтение NPK (0x001E-0x0020)...");
    result = modbus.readHoldingRegisters(0x001E, 3);
    if (result == modbus.ku8MBSuccess) {
        uint16_t nitrogen_raw = modbus.getResponseBuffer(0);
        uint16_t phosphorus_raw = modbus.getResponseBuffer(1);
        uint16_t potassium_raw = modbus.getResponseBuffer(2);
        
        sensorData.nitrogen = (float)nitrogen_raw;
        sensorData.phosphorus = (float)phosphorus_raw;
        sensorData.potassium = (float)potassium_raw;
        
        logData("NPK: N=%.0f, P=%.0f, K=%.0f мг/кг", 
               sensorData.nitrogen, sensorData.phosphorus, sensorData.potassium);
    } else {
        logError("Ошибка чтения NPK: %d", result);
        success = false;
    }
    
    if (success) {
        sensorData.valid = true;
                sensorData.last_update = millis();
        
        logSuccess("Все 7 параметров успешно прочитаны!");
        logData("📊 PH=%.2f, Влаж=%.1f%%, Темп=%.1f°C, EC=%.0fµS/cm, N=%.0f, P=%.0f, K=%.0f", 
               sensorData.ph, sensorData.humidity, sensorData.temperature, sensorData.ec,
               sensorData.nitrogen, sensorData.phosphorus, sensorData.potassium);
    } else {
        logWarn("Не все параметры удалось прочитать");
        sensorData.valid = false;
    }
}

float convertRegisterToFloat(uint16_t value, float multiplier) {
    return value * multiplier;
}

void preTransmission() {
    logDebug("Modbus TX режим");
    digitalWrite(DE_PIN, HIGH);
    digitalWrite(RE_PIN, HIGH);
    delayMicroseconds(50);
}

void postTransmission() {
    delayMicroseconds(50);
    digitalWrite(DE_PIN, LOW);
    digitalWrite(RE_PIN, LOW);
    logDebug("Modbus RX режим");
}

void realSensorTask(void *pvParameters) {
    for (;;) {
        readSensorData();
        vTaskDelay(SENSOR_READ_INTERVAL / portTICK_PERIOD_MS);
    }
}

void startRealSensorTask() {
    xTaskCreate(realSensorTask, "RealSensor", 4096, NULL, 1, NULL);
}

// Функция для вывода ошибок Modbus
void printModbusError(uint8_t errNum) {
    switch (errNum) {
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