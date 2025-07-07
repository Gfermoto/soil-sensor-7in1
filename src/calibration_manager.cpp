#include "calibration_manager.h"
#include "logger.h"
#include "sensor_compensation.h"

namespace CalibrationManager
{
namespace {
bool _initialized = false;
}

const char* profileToFilename(SoilProfile /*profile*/)
{
    return "/calibration/custom.csv";  // единый файл
}

bool init()
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

bool saveCsv(SoilProfile profile, Stream& fileStream)
{
    if (!init())
    {
        return false;
    }
    const char* path = profileToFilename(profile);

    File calibrationFile = LittleFS.open(path, "w");
    if (!calibrationFile)
    {
        logError("Не удалось открыть файл %s для записи", path);
        return false;
    }

    while (fileStream.available())
    {
        const uint8_t dataByte = fileStream.read();
        calibrationFile.write(dataByte);
    }

    calibrationFile.close();
    logSuccess("Калибровочная таблица %s сохранена (%d байт)", path, calibrationFile.size());
    return true;
}

bool loadTable(SoilProfile profile, CalibrationEntry* outBuffer, size_t maxEntries, size_t& outCount)
{
    outCount = 0;
    if (!init())
    {
        return false;
    }
    const char* path = profileToFilename(profile);
    File calibrationFile = LittleFS.open(path, "r");
    if (!calibrationFile)
    {
        logWarn("Нет калибровочной таблицы %s", path);
        return false;
    }

    String line;
    while (calibrationFile.available() && outCount < maxEntries)
    {
        line = calibrationFile.readStringUntil('\n');
        line.trim();
        if (line.length() == 0)
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
        if (comma < 0)
        {
            continue;
        }

        const float raw = line.substring(0, comma).toFloat();
        const float corr = line.substring(comma + 1).toFloat();
        outBuffer[outCount++] = {raw, corr};
    }

    calibrationFile.close();
    logInfo("Загружено %d записей из %s", outCount, path);
    return outCount > 0;
}

bool hasTable(SoilProfile profile)
{
    if (!init())
    {
        return false;
    }
    return LittleFS.exists(profileToFilename(profile));
}

bool deleteTable(SoilProfile profile)
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

float applyCalibration(float rawValue, SoilProfile profile)
{
    // Если калибровочная таблица не загружена, возвращаем исходное значение
    if (!hasTable(profile))
    {
        return rawValue;
    }

    // Загружаем калибровочную таблицу
    constexpr size_t MAX_ENTRIES = 100;
    CalibrationEntry entries[MAX_ENTRIES];
    size_t entryCount;

    if (!loadTable(profile, entries, MAX_ENTRIES, entryCount) || entryCount == 0)
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
        const float interpolatedCoeff = lowerCorr + ratio * (upperCorr - lowerCorr);
        return rawValue * interpolatedCoeff;
    }
    else
    {
        // Если нет интервала, используем ближайший коэффициент
        return rawValue * lowerCorr;
    }
}
}  // namespace CalibrationManager
