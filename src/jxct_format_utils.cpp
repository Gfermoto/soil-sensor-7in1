#include "jxct_format_utils.h"
#include <cstdio>

// Универсальная функция форматирования float
std::string formatFloat(float value, int precision, bool asInt)
{
    char buf[8];
    if (asInt)
    {
        snprintf(buf, sizeof(buf), "%d", (int)(value + 0.5F));
    }
    else
    {
        snprintf(buf, sizeof(buf), "%.*f", precision, value);
    }
    return std::string(buf);
}

std::string format_moisture(float value)
{
    return formatFloat(value, 1, false);
}

std::string format_temperature(float value)
{
    return formatFloat(value, 1, false);
}

std::string format_ec(float value)
{
    return formatFloat(value, 0, true);
}

std::string format_ph(float value)
{
    return formatFloat(value, 1, false);
}

std::string format_npk(float value)
{
    return formatFloat(value, 0, true);
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
