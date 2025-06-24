#pragma once
#include "basic_sensor_adapter.h"
#include "fake_sensor.h"
#include "modbus_sensor.h"

class FakeSensorAdapter : public BasicSensorAdapter {
public:
    FakeSensorAdapter()
        : BasicSensorAdapter("FakeSensor",
                             []() { startFakeSensorTask(); },
                             []() { /* данные генерируются в задаче */ },
                             &sensorData) {}
}; 