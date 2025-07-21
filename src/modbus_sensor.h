#ifndef MODBUS_SENSOR_H
#define MODBUS_SENSOR_H

#ifdef TEST_BUILD
#include "esp32_stubs.h"
#elif defined(ESP32) || defined(ARDUINO)
#include <ModbusMaster.h>
#include "Arduino.h"
#else
#include "esp32_stubs.h"
#endif

// ===================== КОНСТАНТЫ MODBUS И ДАННЫХ =====================
constexpr uint16_t REG_PH = 0x0006;
constexpr uint16_t REG_SOIL_MOISTURE = 0x0012;
constexpr uint16_t REG_SOIL_TEMP = 0x0013;
constexpr uint16_t REG_CONDUCTIVITY = 0x0015;
constexpr uint16_t REG_NITROGEN = 0x001E;
constexpr uint16_t REG_PHOSPHORUS = 0x001F;
constexpr uint16_t REG_POTASSIUM = 0x0020;
constexpr uint16_t REG_FIRMWARE_VERSION = 0x07;
constexpr uint16_t REG_CALIBRATION = 0x08;
constexpr uint16_t REG_ERROR_STATUS = 0x0B;
constexpr uint16_t REG_DEVICE_ADDRESS = 0x0C;
constexpr size_t SENSOR_BUFFER_SIZE = 15;
constexpr float SENSOR_DEFAULT_FLOAT = 0.0F;
constexpr float SENSOR_DEFAULT_SCALE = 1.0F;
constexpr int SENSOR_MAX_VALUE = 1000;
constexpr int SENSOR_MIN_VALUE = 100;

// Допустимые пределы измерений (используем единые константы из jxct_constants.h)
#include "jxct_constants.h"
#define MIN_TEMPERATURE SENSOR_TEMP_MIN
#define MAX_TEMPERATURE SENSOR_TEMP_MAX
#define MIN_HUMIDITY SENSOR_HUMIDITY_MIN
#define MAX_HUMIDITY SENSOR_HUMIDITY_MAX
#define MIN_EC SENSOR_EC_MIN
#define MAX_EC SENSOR_EC_MAX
#define MIN_PH SENSOR_PH_MIN
#define MAX_PH SENSOR_PH_MAX
#define MIN_NPK SENSOR_NPK_MIN
#define MAX_NPK SENSOR_NPK_MAX

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
    float temp_buffer[SENSOR_BUFFER_SIZE];  // Буфер температуры (макс 15 значений)
    float hum_buffer[SENSOR_BUFFER_SIZE];   // Буфер влажности
    float ec_buffer[SENSOR_BUFFER_SIZE];    // Буфер EC
    float ph_buffer[SENSOR_BUFFER_SIZE];    // Буфер pH
    float n_buffer[SENSOR_BUFFER_SIZE];     // Буфер азота
    float p_buffer[SENSOR_BUFFER_SIZE];     // Буфер фосфора
    float k_buffer[SENSOR_BUFFER_SIZE];     // Буфер калия
    uint8_t buffer_index;   // Текущий индекс в буферах
    uint8_t buffer_filled;  // Количество заполненных элементов (0-15)

    // RAW значения до компенсации (v2.5.1)
    float raw_temperature;
    float raw_humidity;
    float raw_ec;
    float raw_ph;
    float raw_nitrogen;
    float raw_phosphorus;
    float raw_potassium;
    bool recentIrrigation;
};

// Структура для кэширования данных
struct SensorCache
{
    SensorData data;
    bool is_valid;
    unsigned long timestamp;
};

extern SensorData sensorData;
extern SensorCache sensorCache;
String& getSensorLastError();

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
void addToMovingAverage(SensorData& data, const SensorData& newReading);
float calculateMovingAverage(const float* buffer, uint8_t window_size, uint8_t filled);
void initMovingAverageBuffers(SensorData& data);

// Тестовые функции
void testSP3485E();               // Тест драйвера SP3485E
bool testModbusConnection();      // Диагностика Modbus связи
void testSerialConfigurations();  // Тест конфигураций Serial2

#endif  // MODBUS_SENSOR_H
