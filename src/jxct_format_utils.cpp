#include "jxct_format_utils.h"
#include <cstdio>

std::string format_moisture(float value)
{
    char buf[8];
    snprintf(buf, sizeof(buf), "%.1f", value);
    return std::string(buf);
}

std::string format_temperature(float value)
{
    char buf[8];
    snprintf(buf, sizeof(buf), "%.1f", value);
    return std::string(buf);
}

std::string format_ec(float value)
{
    char buf[8];
    snprintf(buf, sizeof(buf), "%d", (int)(value + 0.5f));
    return std::string(buf);
}

std::string format_ph(float value)
{
    char buf[8];
    snprintf(buf, sizeof(buf), "%.1f", value);
    return std::string(buf);
}

std::string format_npk(float value)
{
    char buf[8];
    snprintf(buf, sizeof(buf), "%d", (int)(value + 0.5f));
    return std::string(buf);
}