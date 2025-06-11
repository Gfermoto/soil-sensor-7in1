#ifndef MODBUS_SENSOR_H
#define MODBUS_SENSOR_H

#include <Arduino.h>
#include <ModbusMaster.h>

// Регистры Modbus для датчика JXCT 7in1 согласно документации производителя
#define REG_SOIL_MOISTURE 0x00     // Влажность почвы
#define REG_SOIL_TEMP 0x01         // Температура почвы
#define REG_CONDUCTIVITY 0x02      // Электропроводность
#define REG_PH 0x03                // pH почвы
#define REG_NITROGEN 0x04          // Азот
#define REG_PHOSPHORUS 0x05        // Фосфор
#define REG_POTASSIUM 0x06         // Калий
#define REG_FIRMWARE_VERSION 0x07  // Версия прошивки
#define REG_CALIBRATION 0x08       // Калибровка
#define REG_ERROR_STATUS 0x0B      // Статус ошибок
#define REG_DEVICE_ADDRESS 0x0C    // Адрес устройства

// Допустимые пределы измерений
#define MIN_TEMPERATURE -10.0
#define MAX_TEMPERATURE 50.0
#define MIN_HUMIDITY 0.0
#define MAX_HUMIDITY 100.0
#define MIN_EC 0.0
#define MAX_EC 20000.0
#define MIN_PH 0.0
#define MAX_PH 14.0
#define MIN_NPK 0.0
#define MAX_NPK 2000.0

// Структура для хранения данных с датчика
struct SensorData
{
    float temperature;          // Температура почвы в °C (делится на 10)
    float humidity;             // Влажность почвы в % (делится на 10)
    float ec;                   // Электропроводность почвы в µS/cm
    float ph;                   // pH почвы (делится на 100)
    float nitrogen;             // Содержание азота в мг/кг
    float phosphorus;           // Содержание фосфора в мг/кг
    float potassium;            // Содержание калия в мг/кг
    float moisture;             // Добавляем поле для влажности
    float conductivity;         // Добавляем поле для электропроводности
    uint16_t firmware_version;  // Версия прошивки
    uint8_t error_status;       // Статус ошибок
    bool valid;                 // Флаг валидности данных
    unsigned long last_update;  // Время последнего обновления
    
    // ДЕЛЬТА-ФИЛЬТР v2.2.1: Поля для хранения предыдущих значений
    float prev_temperature;     // Предыдущая температура для дельта-фильтра
    float prev_humidity;        // Предыдущая влажность для дельта-фильтра  
    float prev_ec;              // Предыдущая EC для дельта-фильтра
    float prev_ph;              // Предыдущий pH для дельта-фильтра
    float prev_nitrogen;        // Предыдущий азот для дельта-фильтра
    float prev_phosphorus;      // Предыдущий фосфор для дельта-фильтра
    float prev_potassium;       // Предыдущий калий для дельта-фильтра
    unsigned long last_mqtt_publish; // Время последней публикации MQTT
    
    // СКОЛЬЗЯЩЕЕ СРЕДНЕЕ v2.3.0: Кольцевые буферы для усреднения
    float temp_buffer[15];      // Буфер температуры (макс 15 значений)
    float hum_buffer[15];       // Буфер влажности
    float ec_buffer[15];        // Буфер EC
    float ph_buffer[15];        // Буфер pH
    float n_buffer[15];         // Буфер азота
    float p_buffer[15];         // Буфер фосфора
    float k_buffer[15];         // Буфер калия
    uint8_t buffer_index;       // Текущий индекс в буферах
    uint8_t buffer_filled;      // Количество заполненных элементов (0-15)
};

// Структура для кэширования данных
struct SensorCache
{
    SensorData data;
    bool is_valid;
    unsigned long timestamp;
};

extern ModbusMaster modbus;
extern SensorData sensorData;
extern SensorCache sensorCache;
extern String sensorLastError;

// Инициализация Modbus
void setupModbus();

// Чтение данных с датчика
void readSensorData();

// Чтение версии прошивки
bool readFirmwareVersion();

// Чтение статуса ошибок
bool readErrorStatus();

// Изменение адреса устройства
bool changeDeviceAddress(uint8_t new_address);

// Проверка валидности значений
bool validateSensorData(SensorData& data);

// Получение кэшированных данных
bool getCachedData(SensorData& data);

// Преобразование значения регистра в число с плавающей точкой
float convertRegisterToFloat(uint16_t value, float multiplier);

// Функция предварительной передачи для управления MAX485
void preTransmission();

// Функция после передачи для управления MAX485
void postTransmission();

// Функция для вывода ошибок Modbus
void printModbusError(uint8_t errNum);

void startRealSensorTask();

// v2.3.0: Функции скользящего среднего
void addToMovingAverage(SensorData& data, float temp, float hum, float ec, float ph, float n, float p, float k);
float calculateMovingAverage(float* buffer, uint8_t window_size, uint8_t filled);
void initMovingAverageBuffers(SensorData& data);

#endif  // MODBUS_SENSOR_H