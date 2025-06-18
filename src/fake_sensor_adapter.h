#pragma once
#include "ISensor.h"
#include "fake_sensor.h"
#include "modbus_sensor.h"

class FakeSensorAdapter : public ISensor {
public:
    bool begin() override {
        startFakeSensorTask();
        return true;
    }

    bool read(SensorData &out) override {
        // данные обновляются задачей fake sensor
        out = sensorData;
        return sensorData.valid;
    }

    const char *name() const override { return "FakeSensor"; }
}; 