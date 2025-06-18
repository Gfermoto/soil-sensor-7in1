#pragma once
#include "ISensor.h"
#include "modbus_sensor.h"

class ModbusSensorAdapter : public ISensor {
public:
    bool begin() override {
        setupModbus();
        return true;
    }

    bool read(SensorData &out) override {
        readSensorData();
        out = sensorData; // копируем глобальную структуру
        return sensorData.valid;
    }

    const char *name() const override { return "ModbusSensor"; }
}; 