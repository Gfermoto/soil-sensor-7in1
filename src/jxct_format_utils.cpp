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

// Универсальная функция форматирования для веб-интерфейса
String formatValue(float value, const char* unit, int precision)
{
    char buf[32];

    // Форматируем значение с заданной точностью
    switch (precision)
    {
        case 0:
            snprintf(buf, sizeof(buf), "%.0f%s", value, unit);
            break;
        case 1:
            snprintf(buf, sizeof(buf), "%.1f%s", value, unit);
            break;
        case 2:
        default:
            snprintf(buf, sizeof(buf), "%.2f%s", value, unit);
            break;
        case 3:
            snprintf(buf, sizeof(buf), "%.3f%s", value, unit);
            break;
    }

    return String(buf);
}