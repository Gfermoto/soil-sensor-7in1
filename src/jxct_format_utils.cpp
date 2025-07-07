#include "jxct_format_utils.h"
#include <cmath>
#include <cstdio>
#include <array>

// Строгая типизация для предотвращения ошибок
enum class FormatType {
    INTEGER,
    FLOAT
};

// Универсальная функция форматирования float
std::string formatFloat(float value, int precision, FormatType formatType)  // NOLINT(misc-use-internal-linkage,misc-use-anonymous-namespace)
{
    std::array<char, 8> buf;
    if (formatType == FormatType::INTEGER)
    {
        snprintf(buf.data(), buf.size(), "%d", static_cast<int>(lround(value)));
    }
    else
    {
        snprintf(buf.data(), buf.size(), "%.*f", precision, value);
    }
    return std::string(buf.data());
}

std::string format_moisture(float value)  // NOLINT(misc-use-internal-linkage,misc-use-anonymous-namespace)
{
    return formatFloat(value, 1, FormatType::FLOAT);
}

std::string format_temperature(float value)  // NOLINT(misc-use-internal-linkage,misc-use-anonymous-namespace)
{
    return formatFloat(value, 1, FormatType::FLOAT);
}

std::string format_ec(float value)  // NOLINT(misc-use-internal-linkage,misc-use-anonymous-namespace)
{
    return formatFloat(value, 0, FormatType::INTEGER);
}

std::string format_ph(float value)  // NOLINT(misc-use-internal-linkage,misc-use-anonymous-namespace)
{
    return formatFloat(value, 1, FormatType::FLOAT);
}

std::string format_npk(float value)  // NOLINT(misc-use-internal-linkage,misc-use-anonymous-namespace)
{
    return formatFloat(value, 0, FormatType::INTEGER);
}

// Универсальная функция форматирования для веб-интерфейса
String formatValue(float value, const char* unit, int precision)  // NOLINT(misc-use-internal-linkage,misc-use-anonymous-namespace)
{
    std::array<char, 32> buf;

    // Форматируем значение с заданной точностью
    switch (precision)
    {
        case 0:
            snprintf(buf.data(), buf.size(), "%.0f%s", value, unit);
            break;
        case 1:
            snprintf(buf.data(), buf.size(), "%.1f%s", value, unit);
            break;
        case 2:
        default:
            snprintf(buf.data(), buf.size(), "%.2f%s", value, unit);
            break;
        case 3:
            snprintf(buf.data(), buf.size(), "%.3f%s", value, unit);
            break;
    }

    return String(buf.data());
}
