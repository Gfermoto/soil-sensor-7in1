#ifndef MODBUS_SENSOR_H
#define MODBUS_SENSOR_H

#include <Arduino.h>
#include <ModbusMaster.h>

// 🔥 ВОССТАНОВЛЕНЫ РАБОЧИЕ РЕГИСТРЫ из официальной документации JXCT:
// ✅ ПРАВИЛЬНЫЕ Modbus адреса (подтверждены документацией):
#define REG_PH 0x0006              // pH почвы (÷100)
#define REG_SOIL_MOISTURE 0x0012   // Влажность почвы (÷10)
#define REG_SOIL_TEMP 0x0013       // Температура почвы (÷10)
#define REG_CONDUCTIVITY 0x0015    // Электропроводность (как есть)
#define REG_NITROGEN 0x001E        // Азот (как есть)
#define REG_PHOSPHORUS 0x001F      // Фосфор (как есть)
#define REG_POTASSIUM 0x0020       // Калий (как есть)
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
    bool isValid;               // Альтернативное поле валидности для веб-интерфейса
    unsigned long last_update;  // Время последнего обновления
    unsigned long timestamp;    // Альтернативное поле времени для веб-интерфейса

    // ДЕЛЬТА-ФИЛЬТР v2.2.1: Поля для хранения предыдущих значений
    float prev_temperature;           // Предыдущая температура для дельта-фильтра
    float prev_humidity;              // Предыдущая влажность для дельта-фильтра
    float prev_ec;                    // Предыдущая EC для дельта-фильтра
    float prev_ph;                    // Предыдущий pH для дельта-фильтра
    float prev_nitrogen;              // Предыдущий азот для дельта-фильтра
    float prev_phosphorus;            // Предыдущий фосфор для дельта-фильтра
    float prev_potassium;             // Предыдущий калий для дельта-фильтра
    unsigned long last_mqtt_publish;  // Время последней публикации MQTT

    // СКОЛЬЗЯЩЕЕ СРЕДНЕЕ v2.3.0: Кольцевые буферы для усреднения
    float temp_buffer[15];  // Буфер температуры (макс 15 значений)
    float hum_buffer[15];   // Буфер влажности
    float ec_buffer[15];    // Буфер EC
    float ph_buffer[15];    // Буфер pH
    float n_buffer[15];     // Буфер азота
    float p_buffer[15];     // Буфер фосфора
    float k_buffer[15];     // Буфер калия
    uint8_t buffer_index;   // Текущий индекс в буферах
    uint8_t buffer_filled;  // Количество заполненных элементов (0-15)
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

// Получение текущих данных датчика
SensorData getSensorData();

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

// Функция предварительной передачи для управления SP3485E
void preTransmission();

// Функция после передачи для управления SP3485E
void postTransmission();

// Функция для вывода ошибок Modbus
void printModbusError(uint8_t errNum);

void startRealSensorTask();

// v2.3.0: Функции скользящего среднего
void addToMovingAverage(SensorData& data, float temp, float hum, float ec, float ph, float n, float p, float k);
float calculateMovingAverage(float* buffer, uint8_t window_size, uint8_t filled);
void initMovingAverageBuffers(SensorData& data);

// Тестовые функции
void testSP3485E();              // Тест драйвера SP3485E
bool testModbusConnection();      // Диагностика Modbus связи
void testSerialConfigurations();  // Тест конфигураций Serial2

#endif  // MODBUS_SENSOR_H