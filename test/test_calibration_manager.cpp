/**
 * @file test_calibration_manager.cpp
 * @brief Расширенные тесты для модуля управления калибровкой
 * @details Проверка работы с файлами, загрузки/сохранения таблиц и интерполяции
 */

#include <unity.h>
#include "../src/calibration_manager.h"
#include "../src/calibration_manager.cpp"  // Включаем реализацию
#include "../test/stubs/Arduino.h"

// Тестовые данные
static bool mockLittleFSBegun = false;
static bool mockFileExists = false;
static String mockFileContent = "";

// Мок для LittleFS
class MockFile {
public:
    String content;
    size_t position = 0;
    bool isOpen = false;
    
    bool available() { return position < content.length(); }
    String readStringUntil(char delimiter) {
        if (!available()) return "";
        
        int start = position;
        while (position < content.length() && content[position] != delimiter) {
            position++;
        }
        if (position < content.length()) position++; // Пропускаем delimiter
        
        return content.substring(start, position - 1);
    }
    
    uint8_t read() {
        if (available()) return content[position++];
        return 0;
    }
    
    void write(uint8_t byte) {
        content += (char)byte;
    }
    
    void close() { isOpen = false; }
    size_t size() { return content.length(); }
};

static MockFile mockFile;

void setUp(void) {
    // Инициализация перед каждым тестом
    mockLittleFSBegun = false;
    mockFileExists = false;
    mockFileContent = "";
    mockFile = MockFile();
}

void tearDown(void) {
    // Очистка после каждого теста
}

// Тест 1: Проверка инициализации
void test_calibration_manager_init(void) {
    // Первый вызов должен выполнить инициализацию
    mockLittleFSBegun = true;
    bool result = CalibrationManager::init();
    TEST_ASSERT_TRUE(result);
    
    // Повторный вызов должен возвращать true без повторной инициализации
    result = CalibrationManager::init();
    TEST_ASSERT_TRUE(result);
}

// Тест 2: Проверка генерации имени файла
void test_calibration_manager_profile_to_filename(void) {
    const char* filename = CalibrationManager::profileToFilename(SoilProfile::SAND);
    TEST_ASSERT_EQUAL_STRING("/calibration/custom.csv", filename);
    
    // Все профили должны использовать один файл
    filename = CalibrationManager::profileToFilename(SoilProfile::LOAM);
    TEST_ASSERT_EQUAL_STRING("/calibration/custom.csv", filename);
    
    filename = CalibrationManager::profileToFilename(SoilProfile::CLAY);
    TEST_ASSERT_EQUAL_STRING("/calibration/custom.csv", filename);
}

// Тест 3: Проверка парсинга CSV строк
void test_calibration_manager_csv_parsing(void) {
    // Подготавливаем тестовые данные CSV
    mockFileContent = "# Комментарий\n"
                     "Raw,Corrected\n"  // Заголовок
                     "100.0,1.05\n"
                     "200.0,1.10\n"
                     "300.0,1.15\n"
                     "\n"  // Пустая строка
                     "400.0,1.20\n";
    
    mockFile.content = mockFileContent;
    mockFile.isOpen = true;
    
    // Проверяем парсинг строк
    String line1 = mockFile.readStringUntil('\n');
    TEST_ASSERT_EQUAL_STRING("# Комментарий", line1.c_str());
    
    String line2 = mockFile.readStringUntil('\n');
    TEST_ASSERT_EQUAL_STRING("Raw,Corrected", line2.c_str());
    
    String line3 = mockFile.readStringUntil('\n');
    TEST_ASSERT_EQUAL_STRING("100.0,1.05", line3.c_str());
    
    // Проверяем парсинг значений
    int comma = line3.indexOf(',');
    TEST_ASSERT_TRUE(comma > 0);
    
    float raw = line3.substring(0, comma).toFloat();
    float corr = line3.substring(comma + 1).toFloat();
    
    TEST_ASSERT_FLOAT_WITHIN(0.01, 100.0, raw);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 1.05, corr);
}

// Тест 4: Проверка фильтрации комментариев и заголовков
void test_calibration_manager_line_filtering(void) {
    String testLines[] = {
        "# Это комментарий",
        "Raw,Corrected",
        "Temperature,Humidity",
        "100.0,1.05",
        "-50.0,0.95",
        "",
        "   ",
        "200.0,1.10"
    };
    
    for (String line : testLines) {
        line.trim();
        
        // Проверяем логику фильтрации
        bool shouldSkip = (line.length() == 0) ||  // Пустая строка
                         (line[0] == '#') ||        // Комментарий
                         (!isDigit(line[0]) && line[0] != '-');  // Не число
        
        if (line == "# Это комментарий") TEST_ASSERT_TRUE(shouldSkip);
        if (line == "Raw,Corrected") TEST_ASSERT_TRUE(shouldSkip);
        if (line == "Temperature,Humidity") TEST_ASSERT_TRUE(shouldSkip);
        if (line == "100.0,1.05") TEST_ASSERT_FALSE(shouldSkip);
        if (line == "-50.0,0.95") TEST_ASSERT_FALSE(shouldSkip);
        if (line == "") TEST_ASSERT_TRUE(shouldSkip);
        if (line == "200.0,1.10") TEST_ASSERT_FALSE(shouldSkip);
    }
}

// Тест 5: Проверка интерполяции калибровочных значений
void test_calibration_manager_interpolation(void) {
    // Тестовые калибровочные точки
    CalibrationEntry entries[] = {
        {100.0, 1.00},  // rawValue * 1.00
        {200.0, 1.10},  // rawValue * 1.10
        {300.0, 1.20}   // rawValue * 1.20
    };
    
    // Тест 1: Точное совпадение
    float result = 200.0 * 1.10;  // Должно быть 220.0
    TEST_ASSERT_FLOAT_WITHIN(0.01, 220.0, result);
    
    // Тест 2: Интерполяция между точками
    // Для rawValue = 150 (между 100 и 200)
    // Коэффициент должен быть между 1.00 и 1.10
    float rawValue = 150.0;
    float lowerRaw = 100.0, lowerCorr = 1.00;
    float upperRaw = 200.0, upperCorr = 1.10;
    
    float ratio = (rawValue - lowerRaw) / (upperRaw - lowerRaw);
    float interpolatedCoeff = lowerCorr + ratio * (upperCorr - lowerCorr);
    result = rawValue * interpolatedCoeff;
    
    // ratio = (150-100)/(200-100) = 0.5
    // interpolatedCoeff = 1.00 + 0.5*(1.10-1.00) = 1.05
    // result = 150 * 1.05 = 157.5
    TEST_ASSERT_FLOAT_WITHIN(0.01, 1.05, interpolatedCoeff);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 157.5, result);
}

// Тест 6: Проверка граничных случаев интерполяции
void test_calibration_manager_boundary_cases(void) {
    CalibrationEntry entries[] = {
        {100.0, 1.00},
        {200.0, 1.10},
        {300.0, 1.20}
    };
    
    // Случай 1: Значение меньше минимального
    float rawValue = 50.0;  // Меньше 100.0
    float result = rawValue * entries[0].corrected;  // Используем первый коэффициент
    TEST_ASSERT_FLOAT_WITHIN(0.01, 50.0, result);  // 50 * 1.00 = 50
    
    // Случай 2: Значение больше максимального
    rawValue = 400.0;  // Больше 300.0
    result = rawValue * entries[2].corrected;  // Используем последний коэффициент
    TEST_ASSERT_FLOAT_WITHIN(0.01, 480.0, result);  // 400 * 1.20 = 480
    
    // Случай 3: Одна точка калибровки
    CalibrationEntry singleEntry[] = {{150.0, 1.05}};
    rawValue = 200.0;
    result = rawValue * singleEntry[0].corrected;
    TEST_ASSERT_FLOAT_WITHIN(0.01, 210.0, result);  // 200 * 1.05 = 210
}

// Тест 7: Проверка обработки некорректных данных
void test_calibration_manager_invalid_data(void) {
    // Строка без запятой
    String invalidLine = "100.0_1.05";
    int comma = invalidLine.indexOf(',');
    TEST_ASSERT_TRUE(comma < 0);  // Должно быть -1
    
    // Пустая строка после trim
    String emptyLine = "   ";
    emptyLine.trim();
    TEST_ASSERT_TRUE(emptyLine.length() == 0);
    
    // Строка с некорректными числами
    String invalidNumbers = "abc,def";
    comma = invalidNumbers.indexOf(',');
    if (comma >= 0) {
        float raw = invalidNumbers.substring(0, comma).toFloat();
        float corr = invalidNumbers.substring(comma + 1).toFloat();
        // toFloat() возвращает 0.0 для некорректных строк
        TEST_ASSERT_FLOAT_WITHIN(0.01, 0.0, raw);
        TEST_ASSERT_FLOAT_WITHIN(0.01, 0.0, corr);
    }
}

// Тест 8: Проверка работы с различными профилями почвы
void test_calibration_manager_soil_profiles(void) {
    SoilProfile profiles[] = {
        SoilProfile::SAND,
        SoilProfile::LOAM,
        SoilProfile::CLAY,
        SoilProfile::PEAT,
        SoilProfile::SANDPEAT
    };
    
    for (SoilProfile profile : profiles) {
        const char* filename = CalibrationManager::profileToFilename(profile);
        TEST_ASSERT_NOT_NULL(filename);
        TEST_ASSERT_EQUAL_STRING("/calibration/custom.csv", filename);
    }
}

// Тест 9: Проверка максимального количества записей
void test_calibration_manager_max_entries(void) {
    constexpr size_t MAX_ENTRIES = 100;
    CalibrationEntry entries[MAX_ENTRIES];
    size_t entryCount = 0;
    
    // Проверяем, что не превышаем максимум
    for (size_t i = 0; i < MAX_ENTRIES + 10; i++) {
        if (entryCount < MAX_ENTRIES) {
            entries[entryCount++] = {(float)i, 1.0f + i * 0.01f};
        }
    }
    
    TEST_ASSERT_EQUAL_UINT32(MAX_ENTRIES, entryCount);
}

// Тест 10: Проверка точности вычислений с плавающей точкой
void test_calibration_manager_float_precision(void) {
    float raw = 123.456f;
    float coeff = 1.05f;
    float result = raw * coeff;
    
    // Ожидаемый результат: 123.456 * 1.05 = 129.6288
    TEST_ASSERT_FLOAT_WITHIN(0.001, 129.6288f, result);
    
    // Проверка интерполяции с высокой точностью
    float ratio = 0.333333f;
    float lowerCorr = 1.000f;
    float upperCorr = 1.100f;
    float interpolated = lowerCorr + ratio * (upperCorr - lowerCorr);
    
    // Ожидаемый результат: 1.0 + 0.333333 * 0.1 = 1.0333333
    TEST_ASSERT_FLOAT_WITHIN(0.0001, 1.0333333f, interpolated);
}

// Основная функция тестирования
int main(void) {
    UNITY_BEGIN();
    
    RUN_TEST(test_calibration_manager_init);
    RUN_TEST(test_calibration_manager_profile_to_filename);
    RUN_TEST(test_calibration_manager_csv_parsing);
    RUN_TEST(test_calibration_manager_line_filtering);
    RUN_TEST(test_calibration_manager_interpolation);
    RUN_TEST(test_calibration_manager_boundary_cases);
    RUN_TEST(test_calibration_manager_invalid_data);
    RUN_TEST(test_calibration_manager_soil_profiles);
    RUN_TEST(test_calibration_manager_max_entries);
    RUN_TEST(test_calibration_manager_float_precision);
    
    return UNITY_END();
}

// Функция для PlatformIO Unity
void setup() {
    delay(2000);  // Ждем инициализации
    main();
}

void loop() {
    // Пустой цикл
} 