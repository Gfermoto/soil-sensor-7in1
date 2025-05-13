#ifndef MODBUS_SENSOR_H
#define MODBUS_SENSOR_H

#include <Arduino.h>
#include <ModbusMaster.h>

// Регистры Modbus для датчика JXCT 7in1 согласно документации производителя
#define REG_SOIL_MOISTURE   0x12  // Влажность почвы
#define REG_SOIL_TEMP       0x13  // Температура почвы
#define REG_CONDUCTIVITY    0x15  // Электропроводность
#define REG_PH              0x06  // pH почвы
#define REG_NITROGEN        0x1E  // Азот
#define REG_PHOSPHORUS      0x1F  // Фосфор
#define REG_POTASSIUM       0x20  // Калий

// Структура для хранения данных с датчика
struct SensorData {
    float temperature;  // Температура почвы в °C (делится на 10)
    float humidity;     // Влажность почвы в % (делится на 10)
    float ec;           // Электропроводность почвы в µS/cm
    float ph;           // pH почвы (делится на 100)
    float nitrogen;     // Содержание азота в мг/кг
    float phosphorus;   // Содержание фосфора в мг/кг
    float potassium;    // Содержание калия в мг/кг
    bool valid;         // Флаг валидности данных
};

extern ModbusMaster modbus;
extern SensorData sensorData;

// Инициализация Modbus
void setupModbus();

// Чтение данных с датчика
bool readSensorData();

// Преобразование значения регистра в число с плавающей точкой
float convertRegisterToFloat(uint16_t value, float multiplier);

// Функция предварительной передачи для управления MAX485
void preTransmission();

// Функция после передачи для управления MAX485
void postTransmission();

void startRealSensorTask();

#endif // MODBUS_SENSOR_H 