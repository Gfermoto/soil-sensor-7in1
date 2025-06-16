#include "calibration_manager.h"
#include "logger.h"

namespace CalibrationManager
{
    static bool _initialized = false;

    const char* profileToFilename(SoilProfile profile)
    {
        switch (profile)
        {
        case SoilProfile::SAND:
            return "/calibration/sand.csv";
        case SoilProfile::LOAM:
            return "/calibration/loam.csv";
        case SoilProfile::PEAT:
            return "/calibration/peat.csv";
        default:
            return "/calibration/unknown.csv";
        }
    }

    bool init()
    {
        if (_initialized) return true;

        if (!LittleFS.begin(true))
        {
            logError("LittleFS не инициализирован");
            return false;
        }

        // Создаем каталог /calibration при необходимости
        if (!LittleFS.exists("/calibration"))
        {
            LittleFS.mkdir("/calibration");
        }

        _initialized = true;
        logSuccess("LittleFS инициализирован, доступен каталог /calibration");
        return true;
    }

    bool saveCsv(SoilProfile profile, Stream& fileStream)
    {
        if (!init()) return false;
        const char* path = profileToFilename(profile);

        File f = LittleFS.open(path, "w");
        if (!f)
        {
            logError("Не удалось открыть файл %s для записи", path);
            return false;
        }

        while (fileStream.available())
        {
            uint8_t b = fileStream.read();
            f.write(b);
        }

        f.close();
        logSuccess("Калибровочная таблица %s сохранена (%d байт)", path, f.size());
        return true;
    }

    bool loadTable(SoilProfile profile, CalibrationEntry* outBuffer, size_t maxEntries, size_t& outCount)
    {
        outCount = 0;
        if (!init()) return false;
        const char* path = profileToFilename(profile);
        File f = LittleFS.open(path, "r");
        if (!f)
        {
            logWarn("Нет калибровочной таблицы %s", path);
            return false;
        }

        String line;
        while (f.available() && outCount < maxEntries)
        {
            line = f.readStringUntil('\n');
            line.trim();
            if (line.length() == 0) continue;

            int comma = line.indexOf(',');
            if (comma < 0) continue;

            float raw = line.substring(0, comma).toFloat();
            float corr = line.substring(comma + 1).toFloat();
            outBuffer[outCount++] = {raw, corr};
        }

        f.close();
        logInfo("Загружено %d записей из %s", outCount, path);
        return outCount > 0;
    }

    bool hasTable(SoilProfile profile)
    {
        if (!init()) return false;
        return LittleFS.exists(profileToFilename(profile));
    }

    bool deleteTable(SoilProfile profile)
    {
        if (!init()) return false;
        const char* path = profileToFilename(profile);
        if (LittleFS.exists(path))
        {
            return LittleFS.remove(path);
        }
        return false;
    }
} // namespace CalibrationManager 