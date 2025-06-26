#ifndef I_SENSOR_H
#define I_SENSOR_H

#include <cstdint>

// Вперёд объявляем структуру, определена в modbus_sensor.h
struct SensorData;

class ISensor {
public:
    virtual ~ISensor() = default;

    // Подготовка к работе (инициализация шин, буферов и т.п.)
    virtual bool begin() = 0;

    // Считать актуальные данные; возвращает true, если данные валидны
    virtual bool read(SensorData &out) = 0;

    // Человеческое имя сенсора (для логов)
    virtual const char *name() const = 0;
};

#endif // I_SENSOR_H 