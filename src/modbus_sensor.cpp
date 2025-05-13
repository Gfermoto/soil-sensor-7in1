/**
 * @file modbus_sensor.cpp
 * @brief Работа с Modbus-датчиком JXCT
 * @details Реализация функций и задач для опроса датчика по Modbus RTU, преобразование данных, управление линией передачи.
 */
#include "modbus_sensor.h"
#include "jxct_device_info.h"
#include "jxct_config_vars.h"

ModbusMaster modbus;
SensorData sensorData;

void setupModbus() {
    // Настройка UART для Modbus (4800 бод согласно документации)
    Serial2.begin(4800, SERIAL_8N1, RX_PIN, TX_PIN);
    
    // Настройка модуля MAX485
    pinMode(MAX485_DE_RE, OUTPUT);
    digitalWrite(MAX485_DE_RE, LOW); // Режим приёма по умолчанию
    
    // Инициализация ModbusMaster с ID из конфигурации
    modbus.begin(config.modbusId, Serial2);
    
    // Установка колбэков для управления направлением передачи данных
    modbus.preTransmission(preTransmission);
    modbus.postTransmission(postTransmission);
    
    // Инициализация структуры данных
    sensorData.valid = false;
}

bool readSensorData() {
    uint8_t result;
    
    // Сброс флага валидности данных
    sensorData.valid = false;
    
    // Чтение температуры почвы
    result = modbus.readHoldingRegisters(REG_SOIL_TEMP, 1);
    if (result == modbus.ku8MBSuccess) {
        sensorData.temperature = convertRegisterToFloat(modbus.getResponseBuffer(0), 0.1);
    } else {
        Serial.println("Ошибка чтения температуры почвы");
        return false;
    }
    
    // Чтение влажности почвы
    result = modbus.readHoldingRegisters(REG_SOIL_MOISTURE, 1);
    if (result == modbus.ku8MBSuccess) {
        sensorData.humidity = convertRegisterToFloat(modbus.getResponseBuffer(0), 0.1);
    } else {
        Serial.println("Ошибка чтения влажности почвы");
        return false;
    }
    
    // Чтение электропроводности
    result = modbus.readHoldingRegisters(REG_CONDUCTIVITY, 1);
    if (result == modbus.ku8MBSuccess) {
        sensorData.ec = convertRegisterToFloat(modbus.getResponseBuffer(0), 1.0);
    } else {
        Serial.println("Ошибка чтения электропроводности");
        return false;
    }
    
    // Чтение pH
    result = modbus.readHoldingRegisters(REG_PH, 1);
    if (result == modbus.ku8MBSuccess) {
        sensorData.ph = convertRegisterToFloat(modbus.getResponseBuffer(0), 0.01);
    } else {
        Serial.println("Ошибка чтения pH");
        return false;
    }
    
    // Чтение NPK (азот, фосфор, калий)
    result = modbus.readHoldingRegisters(REG_NITROGEN, 1);
    if (result == modbus.ku8MBSuccess) {
        sensorData.nitrogen = convertRegisterToFloat(modbus.getResponseBuffer(0), 1.0);
    } else {
        Serial.println("Ошибка чтения азота");
        return false;
    }
    
    result = modbus.readHoldingRegisters(REG_PHOSPHORUS, 1);
    if (result == modbus.ku8MBSuccess) {
        sensorData.phosphorus = convertRegisterToFloat(modbus.getResponseBuffer(0), 1.0);
    } else {
        Serial.println("Ошибка чтения фосфора");
        return false;
    }
    
    result = modbus.readHoldingRegisters(REG_POTASSIUM, 1);
    if (result == modbus.ku8MBSuccess) {
        sensorData.potassium = convertRegisterToFloat(modbus.getResponseBuffer(0), 1.0);
    } else {
        Serial.println("Ошибка чтения калия");
        return false;
    }
    
    // Если дошли до этой точки, данные успешно прочитаны
    sensorData.valid = true;
    return true;
}

float convertRegisterToFloat(uint16_t value, float multiplier) {
    return value * multiplier;
}

void preTransmission() {
    digitalWrite(MAX485_DE_RE, HIGH); // Режим передачи
    delayMicroseconds(50);
}

void postTransmission() {
    delayMicroseconds(50);
    digitalWrite(MAX485_DE_RE, LOW); // Режим приёма
}

void realSensorTask(void *pvParameters) {
    for (;;) {
        if (readSensorData()) {
            Serial.println("[realSensorTask] Данные с реального датчика успешно прочитаны");
        } else {
            Serial.println("[realSensorTask] Ошибка чтения данных с реального датчика");
            sensorData.valid = false;
        }
        vTaskDelay(SENSOR_READ_INTERVAL / portTICK_PERIOD_MS);
    }
}

void startRealSensorTask() {
    xTaskCreate(realSensorTask, "RealSensor", 2048, NULL, 1, NULL);
} 