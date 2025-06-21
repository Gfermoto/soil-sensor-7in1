#pragma once
#include "basic_sensor_adapter.h"

class ModbusSensorAdapter : public BasicSensorAdapter
{
   public:
    ModbusSensorAdapter()
        : BasicSensorAdapter("ModbusSensor", []() { setupModbus(); }, []() { readSensorData(); }, &sensorData)
    {
    }
};