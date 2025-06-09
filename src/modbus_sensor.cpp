/**
 * @file modbus_sensor.cpp
 * @brief Работа с Modbus-датчиком JXCT
 * @details Реализация функций и задач для опроса датчика по Modbus RTU, преобразование данных, управление линией передачи.
 */
#include <Arduino.h>
#include "modbus_sensor.h"
#include "jxct_device_info.h"
#include "jxct_config_vars.h"

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
    Serial.print(prefix);
    for(size_t i = 0; i < length; i++) {
        Serial.print("0x");
        if(buffer[i] < 0x10) Serial.print("0");
        Serial.print(buffer[i], HEX);
        Serial.print(" ");
    }
    Serial.println();
}

void testMAX485() {
    Serial.println("Тест MAX485...");
    digitalWrite(DE_PIN, HIGH);
    digitalWrite(RE_PIN, HIGH);
    delay(2);
    Serial2.write(0x55);
    Serial2.flush();
    digitalWrite(DE_PIN, LOW);
    digitalWrite(RE_PIN, LOW);
    delay(1000);
    if (Serial2.available()) {
        Serial.print("Получен ответ: ");
        while (Serial2.available()) {
            Serial.print(Serial2.read(), HEX);
            Serial.print(" ");
        }
        Serial.println();
    } else {
        Serial.println("Нет ответа от MAX485");
    }
}

void setupModbus() {
    Serial.println("\n[Modbus] Начало инициализации Modbus...");
    
    // Проверяем пины
    Serial.printf("[Modbus] RX_PIN: %d, TX_PIN: %d\n", RX_PIN, TX_PIN);
    Serial.printf("[Modbus] DE_PIN: %d, RE_PIN: %d\n", DE_PIN, RE_PIN);
    
    // Устанавливаем пины управления MAX485
    Serial.println("[Modbus] Настройка пинов MAX485...");
    pinMode(DE_PIN, OUTPUT);
    pinMode(RE_PIN, OUTPUT);
    digitalWrite(DE_PIN, LOW);  // Режим приема
    digitalWrite(RE_PIN, LOW);  // Режим приема
    Serial.println("[Modbus] Пины MAX485 настроены");
    
    // Инициализируем Serial2 на скорости 9600 (из примера)
    Serial.println("[Modbus] Инициализация Serial2 на 9600 бод...");
    Serial2.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);
    delay(100);
    Serial.println("[Modbus] Serial2 инициализирован");
    
    // Инициализируем ModbusMaster с адресом устройства 1
    Serial.println("[Modbus] Инициализация ModbusMaster...");
    modbus.begin(1, Serial2);
    
    // Устанавливаем колбэки для управления DE/RE
    modbus.preTransmission(preTransmission);
    modbus.postTransmission(postTransmission);
    
    Serial.println("[Modbus] ModbusMaster инициализирован успешно");
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
    Serial.println("\n[Modbus] Запрос версии прошивки");
    uint8_t result = modbus.readHoldingRegisters(0x07, 1);
    Serial.printf("[Modbus] Результат запроса версии: %d\n", result);
    
    if (result == modbus.ku8MBSuccess) {
        uint16_t version = modbus.getResponseBuffer(0);
        Serial.printf("[Modbus] Версия прошивки: %d.%d\n", (version >> 8) & 0xFF, version & 0xFF);
        return true;
    } else {
        Serial.printf("[Modbus] Ошибка чтения версии: %d\n", result);
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
    Serial.println("\n[readSensorData] Попытка чтения NPK значений...");
    
    uint8_t result;
    
    // Пробуем прочитать Nitrogen (адрес 0x1E как в примере)
    Serial.println("[readSensorData] Чтение Nitrogen (0x1E)...");
    result = modbus.readHoldingRegisters(0x1E, 1);
    
    if (result == modbus.ku8MBSuccess) {
        uint16_t nitrogen_raw = modbus.getResponseBuffer(0);
        sensorData.nitrogen = (float)nitrogen_raw;
        Serial.printf("[readSensorData] Nitrogen успешно прочитан: %d (%.1f мг/кг)\n", nitrogen_raw, sensorData.nitrogen);
        
        // Пробуем прочитать Phosphorus (адрес 0x1F)
        Serial.println("[readSensorData] Чтение Phosphorus (0x1F)...");
        result = modbus.readHoldingRegisters(0x1F, 1);
        if (result == modbus.ku8MBSuccess) {
            uint16_t phosphorus_raw = modbus.getResponseBuffer(0);
            sensorData.phosphorus = (float)phosphorus_raw;
            Serial.printf("[readSensorData] Phosphorus успешно прочитан: %d (%.1f мг/кг)\n", phosphorus_raw, sensorData.phosphorus);
            
            // Пробуем прочитать Potassium (адрес 0x20)
            Serial.println("[readSensorData] Чтение Potassium (0x20)...");
            result = modbus.readHoldingRegisters(0x20, 1);
            if (result == modbus.ku8MBSuccess) {
                uint16_t potassium_raw = modbus.getResponseBuffer(0);
                sensorData.potassium = (float)potassium_raw;
                Serial.printf("[readSensorData] Potassium успешно прочитан: %d (%.1f мг/кг)\n", potassium_raw, sensorData.potassium);
                
                sensorData.valid = true;
                sensorData.last_update = millis();
                Serial.println("[readSensorData] *** ВСЕ NPK ЗНАЧЕНИЯ УСПЕШНО ПРОЧИТАНЫ! ***");
            } else {
                Serial.printf("[readSensorData] Ошибка чтения Potassium: %d\n", result);
                printModbusError(result);
            }
        } else {
            Serial.printf("[readSensorData] Ошибка чтения Phosphorus: %d\n", result);
            printModbusError(result);
        }
    } else {
        Serial.printf("[readSensorData] Ошибка чтения Nitrogen: %d\n", result);
        printModbusError(result);
    }
}

float convertRegisterToFloat(uint16_t value, float multiplier) {
    return value * multiplier;
}

void preTransmission() {
    Serial.println("\n[Modbus] preTransmission - переключение в режим передачи");
    Serial.printf("[Modbus] DE: %d, RE: %d\n", digitalRead(DE_PIN), digitalRead(RE_PIN));
    digitalWrite(DE_PIN, HIGH);
    digitalWrite(RE_PIN, HIGH);
    delayMicroseconds(50);
}

void postTransmission() {
    Serial.println("[Modbus] postTransmission - переключение в режим приема");
    delayMicroseconds(50);
    digitalWrite(DE_PIN, LOW);
    digitalWrite(RE_PIN, LOW);
    Serial.printf("[Modbus] DE: %d, RE: %d\n", digitalRead(DE_PIN), digitalRead(RE_PIN));
}

void realSensorTask(void *pvParameters) {
    for (;;) {
        readSensorData();
        vTaskDelay(SENSOR_READ_INTERVAL / portTICK_PERIOD_MS);
    }
}

void startRealSensorTask() {
    xTaskCreate(realSensorTask, "RealSensor", 2048, NULL, 1, NULL);
}

// Функция для вывода ошибок Modbus (как в примере)
void printModbusError(uint8_t errNum) {
    switch (errNum) {
        case ModbusMaster::ku8MBSuccess:
            Serial.println("[Modbus] Успех");
            break;
        case ModbusMaster::ku8MBIllegalFunction:
            Serial.println("[Modbus] Ошибка: Illegal Function Exception");
            break;
        case ModbusMaster::ku8MBIllegalDataAddress:
            Serial.println("[Modbus] Ошибка: Illegal Data Address Exception");
            break;
        case ModbusMaster::ku8MBIllegalDataValue:
            Serial.println("[Modbus] Ошибка: Illegal Data Value Exception");
            break;
        case ModbusMaster::ku8MBSlaveDeviceFailure:
            Serial.println("[Modbus] Ошибка: Slave Device Failure");
            break;
        case ModbusMaster::ku8MBInvalidSlaveID:
            Serial.println("[Modbus] Ошибка: Invalid Slave ID");
            break;
        case ModbusMaster::ku8MBInvalidFunction:
            Serial.println("[Modbus] Ошибка: Invalid Function");
            break;
        case ModbusMaster::ku8MBResponseTimedOut:
            Serial.println("[Modbus] Ошибка: Response Timed Out");
            break;
        case ModbusMaster::ku8MBInvalidCRC:
            Serial.println("[Modbus] Ошибка: Invalid CRC");
            break;
        default:
            Serial.printf("[Modbus] Неизвестная ошибка: %d\n", errNum);
            break;
    }
} 