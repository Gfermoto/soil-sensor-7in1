#pragma once

#ifdef TEST_BUILD
#include "esp32_stubs.h"
#elif defined(ESP32) || defined(ARDUINO)
#include <FS.h>
#include <LittleFS.h>
#include "Arduino.h"
#else
#include "esp32_stubs.h"
#endif
#include "sensor_compensation.h"

// Структура одной записи калибровочной таблицы (сырое значение -> скорректированное)
struct CalibrationEntry
{
    float raw;
    float corrected;
};

namespace CalibrationManager
{
// Инициализация файловой системы (LittleFS) и каталога /calibration
bool init();

// Сохранение CSV-файла, полученного через upload, в каталог /calibration/<profile>.csv
bool saveCsv(SoilProfile profile, Stream& fileStream);

// Загрузка таблицы калибровки в оперативную память
bool loadTable(SoilProfile profile, CalibrationEntry* outBuffer, size_t maxEntries, size_t& outCount);

// Проверка существования таблицы
bool hasTable(SoilProfile profile);

// Удаление таблицы
bool deleteTable(SoilProfile profile);

// Преобразование профиля в имя файла
const char* profileToFilename(SoilProfile profile);

// Применение калибровочной таблицы к значению датчика
float applyCalibration(float rawValue, SoilProfile profile);
}  // namespace CalibrationManager
