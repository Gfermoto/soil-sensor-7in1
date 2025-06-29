#pragma once
#include "Arduino.h"

class WiFiClass {
public:
    String macAddress() { return String("AA:BB:CC:DD:EE:FF"); }
};

static WiFiClass WiFi; 