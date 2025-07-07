#include "jxct_format_utils.h"
#include <array>
#include <cmath>
#include <cstdio>

namespace
{
// Строгая типизация для предотвращения ошибок
enum class FormatType : std::uint8_t
{
    INTEGER,
    FLOAT
};

// Строгая типизация для предотвращения ошибок
struct FormatOptions
{
    int precision;
    FormatType formatType;

    FormatOptions(int precision, FormatType formatType) : precision(precision), formatType(formatType) {}
};

// Универсальная функция форматирования float
std::string formatFloat(float value, const FormatOptions& options)
{
    std::array<char, 8> buf;

    switch (options.formatType)
    {
        case FormatType::INTEGER:
            snprintf(buf.data(), buf.size(), "%d", static_cast<int>(lround(value)));
            break;
        case FormatType::FLOAT:
            snprintf(buf.data(), buf.size(), "%.*f", options.precision, value);
            break;
    }
    return std::string(buf.data());
}
}  // namespace

std::string format_moisture(float value)
{
    return formatFloat(value, FormatOptions(1, FormatType::FLOAT));
}

std::string format_temperature(float value)
{
    return formatFloat(value, FormatOptions(1, FormatType::FLOAT));
}

std::string format_ec(float value)
{
    return formatFloat(value, FormatOptions(0, FormatType::INTEGER));
}

std::string format_ph(float value)
{
    return formatFloat(value, FormatOptions(1, FormatType::FLOAT));
}

std::string format_npk(float value)
{
    return formatFloat(value, FormatOptions(0, FormatType::INTEGER));
}

// Универсальная функция форматирования для веб-интерфейса
String formatValue(float value, const char* unit, int precision)
{
    std::array<char, 32> buf;

    // Форматируем значение с заданной точностью
    // Используем clamp для ограничения precision в допустимых пределах
    const int clampedPrecision = (precision < 0 || precision > 3) ? 2 : precision;
    snprintf(buf.data(), buf.size(), "%.*f%s", clampedPrecision, value, unit);

    return String(buf.data());
}
