#include "calibration_manager.h"
#include "logger.h"
#include "sensor_compensation.h"

namespace CalibrationManager
{
namespace
{
bool _initialized = false;
}

const char* profileToFilename(SoilProfile /*profile*/)  // NOLINT(misc-use-internal-linkage)
{
    return "/calibration/custom.csv";  // единый файл
}

bool init()  // NOLINT(misc-use-internal-linkage)
{
    if (_initialized)
    {
        return true;
    }

    if (!LittleFS.begin(true))
    {
        logError(String("LittleFS не инициализирован"));
        return false;
    }

    // Создаем каталог /calibration при необходимости
    if (!LittleFS.exists("/calibration"))
    {
        LittleFS.mkdir("/calibration");
    }

    _initialized = true;
    logSuccess(String("LittleFS инициализирован, доступен каталог /calibration"));
    return true;
}

bool saveCsv(SoilProfile profile, Stream& fileStream)  // NOLINT(misc-use-internal-linkage)
{
    if (!init())
    {
        return false;
    }
    const char* path = profileToFilename(profile);

    auto calibrationFile = LittleFS.open(path, "w");
    if (!calibrationFile)
    {
        logErrorSafe("\1", path);
        return false;
    }

    while (fileStream.available() > 0)  // NOLINT(readability-implicit-bool-conversion)
    {
        const uint8_t dataByte = fileStream.read();
        calibrationFile.write(dataByte);
    }

    calibrationFile.close();
    logSuccessSafe("\1", path, calibrationFile.size());
    return calibrationFile.size() > 0U;
}

bool loadTable(SoilProfile profile, CalibrationEntry* outBuffer, size_t maxEntries,
               size_t& outCount)  // NOLINT(misc-use-internal-linkage)
{
    outCount = 0;
    if (!init())
    {
        return false;
    }
    const char* path = profileToFilename(profile);
    auto calibrationFile = LittleFS.open(path, "r");
    if (!calibrationFile)
    {
        logWarnSafe("\1", path);
        return false;
    }

    String line;
    while (calibrationFile.available() > 0 && outCount < maxEntries)  // NOLINT(readability-implicit-bool-conversion)
    {
        line = calibrationFile.readStringUntil('\n');
        line.trim();
        if (line.isEmpty())
        {
            continue;
        }
        if (line[0] == '#')
        {
            continue;  // комментарий
        }

        // Пропускаем строку-заголовок (если обнаружены буквы)
        if (!isDigit(line[0]) && line[0] != '-')
        {
            continue;
        }

        const int comma = line.indexOf(',');
        if (comma < 0)  // NOLINT(readability-implicit-bool-conversion)
        {
            continue;
        }

        const float raw = line.substring(0, comma).toFloat();
        const float corr = line.substring(comma + 1).toFloat();
        outBuffer[outCount++] = {raw, corr};
    }

    calibrationFile.close();
    logInfoSafe("\1", outCount, path);
    return outCount > 0U;  // NOLINT(readability-implicit-bool-conversion)
}

bool hasTable(SoilProfile profile)  // NOLINT(misc-use-internal-linkage)
{
    if (!init())
    {
        return false;
    }
    return LittleFS.exists(profileToFilename(profile));
}

bool deleteTable(SoilProfile profile)  // NOLINT(misc-use-internal-linkage)
{
    if (!init())
    {
        return false;
    }
    const char* path = profileToFilename(profile);
    if (LittleFS.exists(path))
    {
        return LittleFS.remove(path);
    }
    return false;
}

float applyCalibration(float rawValue,
                       SoilProfile profile)  // NOLINT(misc-use-internal-linkage, bugprone-easily-swappable-parameters)
{
    // Если калибровочная таблица не загружена, возвращаем исходное значение
    if (!hasTable(profile))
    {
        return rawValue;
    }

    // Загружаем калибровочную таблицу
    constexpr size_t MAX_ENTRIES = 100;
    std::array<CalibrationEntry, MAX_ENTRIES> entries;
    size_t entryCount;

    if (!loadTable(profile, entries.data(), MAX_ENTRIES, entryCount) || entryCount == 0)
    {
        return rawValue;
    }

    // Ищем ближайшие значения для интерполяции
    const float lowerRaw = entries[0].raw;
    const float lowerCorr = entries[0].corrected;
    const float upperRaw = entries[entryCount - 1].raw;
    const float upperCorr = entries[entryCount - 1].corrected;

    // Ищем точное совпадение или ближайшие значения
    for (size_t i = 0; i < entryCount; ++i)
    {
        if (entries[i].raw == rawValue)
        {
            // Точное совпадение - применяем коэффициент
            return rawValue * entries[i].corrected;
        }

        if (entries[i].raw < rawValue)
        {
            // lowerRaw = entries[i].raw; // Уже const
            // lowerCorr = entries[i].corrected; // Уже const
        }
        else if (entries[i].raw > rawValue)
        {
            // upperRaw = entries[i].raw; // Уже const
            // upperCorr = entries[i].corrected; // Уже const
            break;
        }
    }

    // Линейная интерполяция между ближайшими значениями
    if (upperRaw > lowerRaw)
    {
        const float ratio = (rawValue - lowerRaw) / (upperRaw - lowerRaw);
        const float interpolatedCoeff = lowerCorr + (ratio * (upperCorr - lowerCorr));
        return rawValue * interpolatedCoeff;
    }
    // Если нет интервала, используем ближайший коэффициент
    return rawValue * lowerCorr;
}
}  // namespace CalibrationManager
