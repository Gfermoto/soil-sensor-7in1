#pragma once
#include <cstdint>
class ESPClass
{
   public:
    uint32_t getFreeHeap()
    {
        return 100000;
    }
    uint32_t getEfuseMac()
    {
        return 0xDEADBEEF;
    }
};

static ESPClass ESP;
