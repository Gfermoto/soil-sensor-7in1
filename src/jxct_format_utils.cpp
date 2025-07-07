#include "jxct_format_utils.h"
#include <cmath>
#include <cstdio>
#include <array>

// Строгая типизация для предотвращения ошибок
enum class FormatType {
    INTEGER,
    FLOAT
};

// Строгая типизация для предотвращения ошибок
struct FormatOptions {
    int precision;
    FormatType formatType;
    
    FormatOptions(int p, FormatType ft) : precision(p), formatType(ft) {}
};

// Универсальная функция форматирования float
std::string formatFloat(float value, const FormatOptions& options)  // NOLINT(misc-use-internal-linkage,misc-use-anonymous-namespace)
{
    std::array<char, 8> buf;
    
    if (options.formatType == FormatType::INTEGER)
    {
        snprintf(buf.data(), buf.size(), "%d", static_cast<int>(lround(value)));
    }
    else
    {
        snprintf(buf.data(), buf.size(), "%.*f", options.precision, value);
    }
    return std::string(buf.data());
}

std::string format_moisture(float value)  // NOLINT(misc-use-internal-linkage,misc-use-anonymous-namespace)
{
    return formatFloat(value, FormatOptions(1, FormatType::FLOAT));
}

std::string format_temperature(float value)  // NOLINT(misc-use-internal-linkage,misc-use-anonymous-namespace)
{
    return formatFloat(value, FormatOptions(1, FormatType::FLOAT));
}

std::string format_ec(float value)  // NOLINT(misc-use-internal-linkage,misc-use-anonymous-namespace)
{
    return formatFloat(value, FormatOptions(0, FormatType::INTEGER));
}

std::string format_ph(float value)  // NOLINT(misc-use-internal-linkage,misc-use-anonymous-namespace)
{
    return formatFloat(value, FormatOptions(1, FormatType::FLOAT));
}

std::string format_npk(float value)  // NOLINT(misc-use-internal-linkage,misc-use-anonymous-namespace)
{
    return formatFloat(value, FormatOptions(0, FormatType::INTEGER));
}

// Универсальная функция форматирования для веб-интерфейса
String formatValue(float value, const char* unit, int precision)  // NOLINT(misc-use-internal-linkage,misc-use-anonymous-namespace)
{
    std::array<char, 32> buf;

    // Форматируем значение с заданной точностью
    // Используем clamp для ограничения precision в допустимых пределах
    int clampedPrecision;
    if (precision < 0 || precision > 3) {
        clampedPrecision = 2;
    } else {
        clampedPrecision = precision;
    }
    snprintf(buf.data(), buf.size(), "%.*f%s", clampedPrecision, value, unit);

    return String(buf.data());
}
