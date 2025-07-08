#include "jxct_format_utils.h"
#include <array>
#include <cmath>
#include <cstdio>

namespace
{
// Строгая типизация для предотвращения ошибок
enum class FormatType : std::uint8_t // NOLINT(performance-enum-size)
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

    switch (options.formatType)  // NOLINT(bugprone-branch-clone)
    {
        case FormatType::INTEGER:
        case FormatType::FLOAT:
            if (options.formatType == FormatType::INTEGER)
            {
                snprintf(buf.data(), buf.size(), "%d", static_cast<int>(lround(value)));
            }
            else
            {
                snprintf(buf.data(), buf.size(), "%.*f", options.precision, value);
            }
            break;
    }
    return std::string(buf.data());
}
}  // namespace

// NOLINTNEXTLINE(misc-use-internal-linkage)
std::string format_moisture(float value)
{
    return formatFloat(value, FormatOptions(1, FormatType::FLOAT));
}

// NOLINTNEXTLINE(misc-use-internal-linkage)
std::string format_temperature(float value)
{
    return formatFloat(value, FormatOptions(1, FormatType::FLOAT));
}

// NOLINTNEXTLINE(misc-use-internal-linkage)
std::string format_ec(float value)
{
    return formatFloat(value, FormatOptions(0, FormatType::INTEGER));
}

// NOLINTNEXTLINE(misc-use-internal-linkage)
std::string format_ph(float value)
{
    return formatFloat(value, FormatOptions(1, FormatType::FLOAT));
}

// NOLINTNEXTLINE(misc-use-internal-linkage)
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
