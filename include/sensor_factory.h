#pragma once
#include "ISensor.h"
#include "fake_sensor_adapter.h"
#include "jxct_config_vars.h"
#include "modbus_sensor_adapter.h"
#include <memory>

static std::unique_ptr<ISensor> createSensorInstance()
{
    if (config.flags.useRealSensor) return std::make_unique<ModbusSensorAdapter>();
    return std::make_unique<FakeSensorAdapter>();
}