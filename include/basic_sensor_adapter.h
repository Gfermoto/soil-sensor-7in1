#pragma once

#include "ISensor.h"
#include "modbus_sensor.h"  // forward for SensorData

// BasicSensorAdapter — лёгкий каркас абстракции без использования std::function (экономия flash)
class BasicSensorAdapter : public ISensor
{
   public:
    using InitFn = void (*)();     // функция инициализации
    using AcquireFn = void (*)();  // функция получения данных

    constexpr BasicSensorAdapter(const char* n, InitFn initFn, AcquireFn acquireFn, SensorData* shared)
        : _name(n), _init(initFn), _acquire(acquireFn), _data(shared)
    {
    }

    bool begin() override
    {
        if (_init) _init();
        return true;
    }

    bool read(SensorData& out) override
    {
        if (_acquire) _acquire();
        if (_data)
        {
            out = *_data;
            return _data->valid;
        }
        return false;
    }

    const char* name() const override
    {
        return _name;
    }

   private:
    const char* _name;
    InitFn _init;
    AcquireFn _acquire;
    SensorData* _data;
};