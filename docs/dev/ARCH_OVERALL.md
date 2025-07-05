# 🏗️ Общая архитектура JXCT v3.6.9

> **Версия документа:** 3.6.9
> **Дата обновления:** 2025-07-05
> **Статус:** Актуальная архитектура с фокусом на стабильность

---

## 📋 Содержание {#Soderzhanie}

- [ПРИНЦИПЫ АРХИТЕКТУРЫ](#PRINTsIPY-ARHITEKTURY)
  - [1. Стабильность превыше всего](#1-Stabilnost-prevyshe-vsego)
  - [2. Модульность и переиспользование](#2-Modulnost-i-pereispolzovanie)
  - [3. Производительность и эффективность](#3-Proizvoditelnost-i-effektivnost)
- [ОСНОВНЫЕ КОМПОНЕНТЫ](#OSNOVNYE-KOMPONENTY)
  - [СИСТЕМА ДАТЧИКОВ](#SISTEMA-DATChIKOV)
    - [Абстракция датчиков](#Abstraktsiya-datchikov)
    - [Адаптеры датчиков](#Adaptery-datchikov)
  - [СИСТЕМА КОМПЕНСАЦИИ](#SISTEMA-KOMPENSATsII)
    - [ИСПРАВЛЕННАЯ ДВУХЭТАПНАЯ КОМПЕНСАЦИЯ](#ISPRAVLENNAYa-DVUHETAPNAYa-KOMPENSATsIYa)
    - [Калибровочный менеджер](#Kalibrovochnyy-menedzher)
    - [1 Модель Арчи - EC компенсация](#1-Model-Archi-ec-kompensatsiya)
    - [2 Уравнение Нернста - pH компенсация](#2-Uravnenie-Nernsta-ph-kompensatsiya)
    - [3 FAO 56  Eur. J. Soil Sci. - NPK компенсация](#3-fao-56-eur-j-soil-sci-npk-kompensatsiya)
- [ВЕБ-ИНТЕРФЕЙС](#VEB-INTERFEYS)
  - [Архитектура веб-интерфейса](#Arhitektura-veb-interfeysa)
  - [Маршрутизация](#Marshrutizatsiya)
  - [UI компоненты](#ui-komponenty)
- [ИНТЕГРАЦИИ](#INTEGRATsII)
  - [MQTT клиент](#mqtt-klient)
  - [ThingSpeak интеграция](#thingspeak-integratsiya)
- [ЛОГИРОВАНИЕ И ДИАГНОСТИКА](#LOGIROVANIE-I-DIAGNOSTIKA)
  - [Система логирования](#Sistema-logirovaniya)
  - [Валидация данных](#Validatsiya-dannyh)
- [ПРОИЗВОДИТЕЛЬНОСТЬ И ОПТИМИЗАЦИЯ](#PROIZVODITELNOST-I-OPTIMIZATsIYa)
  - [Использование памяти](#Ispolzovanie-pamyati)
  - [Оптимизации](#Optimizatsii)
- [БЕЗОПАСНОСТЬ](#BEZOPASNOST)
  - [Текущие меры](#Tekushchie-mery)
  - [Планируемые улучшения](#Planiruemye-uluchsheniya)
- [МОНИТОРИНГ И МЕТРИКИ](#MONITORING-I-METRIKI)
  - [Ключевые метрики](#Klyuchevye-metriki)
  - [Диагностика](#Diagnostika)
- [ПЛАНЫ РАЗВИТИЯ](#PLANY-RAZVITIYa)
  - [Краткосрочные цели (3-6 месяцев)](#Kratkosrochnye-tseli-3-6-mesyatsev)
  - [Долгосрочные цели (6-12 месяцев)](#Dolgosrochnye-tseli-6-12-mesyatsev)

---

## 🎯 **ПРИНЦИПЫ АРХИТЕКТУРЫ** {#PRINTsIPY-ARHITEKTURY}

### 1. **Стабильность превыше всего** {#1-Stabilnost-prevyshe-vsego}
- Эволюционное развитие без революционных изменений
- Приоритет качества кода над архитектурными экспериментами
- Тщательное тестирование перед внедрением изменений

### 2. **Модульность и переиспользование** {#2-Modulnost-i-pereispolzovanie}
- Четкое разделение ответственности между модулями
- Минимизация циклических зависимостей
- Использование интерфейсов для абстракции

### 3. **Производительность и эффективность** {#3-Proizvoditelnost-i-effektivnost}
- Оптимизация использования памяти и Flash
- Эффективные алгоритмы обработки данных
- Минимизация накладных расходов

---

## 🔧 **ОСНОВНЫЕ КОМПОНЕНТЫ** {#OSNOVNYE-KOMPONENTY}

### 📡 **СИСТЕМА ДАТЧИКОВ** {#SISTEMA-DATChIKOV}

#### 🔌 **Абстракция датчиков** {#Abstraktsiya-datchikov}
```cpp
// Файл: include/ISensor.h

class ISensor {
public:
    virtual ~ISensor() = default;
    virtual bool initialize() = 0;
    virtual SensorData read() = 0;
    virtual bool isConnected() const = 0;
    virtual String getSensorInfo() const = 0;
};
```

#### 🎭 **Адаптеры датчиков** {#Adaptery-datchikov}
```cpp
// Файл: src/modbus_sensor.h

class ModbusSensorAdapter : public ISensor {
private:
    ModbusSensor* sensor;

public:
    ModbusSensorAdapter(ModbusSensor* s) : sensor(s) {}

    bool initialize() override {
        return sensor->begin();
    }

    SensorData read() override {
        return sensor->getData();
    }

    bool isConnected() const override {
        return sensor->isConnected();
    }

    String getSensorInfo() const override {
        return "Modbus Sensor v" + String(sensor->getVersion());
    }
};
```

### 🧮 **СИСТЕМА КОМПЕНСАЦИИ** {#SISTEMA-KOMPENSATsII}

#### ✅ **ИСПРАВЛЕННАЯ ДВУХЭТАПНАЯ КОМПЕНСАЦИЯ** {#ISPRAVLENNAYa-DVUHETAPNAYa-KOMPENSATsIYa}

```cpp
// Файл: src/modbus_sensor.cpp

void applyCompensationIfEnabled(SensorData& d, const SoilProfile& profile) {
    // ШАГ 1: Применяем калибровочную таблицу CSV (лабораторная поверка)
    float tempCalibrated = CalibrationManager::applyCalibration(d.temperature, profile);
    float humCalibrated = CalibrationManager::applyCalibration(d.humidity, profile);
    float ecCalibrated = CalibrationManager::applyCalibration(d.ec, profile);
    float phCalibrated = CalibrationManager::applyCalibration(d.ph, profile);
    float nCalibrated = CalibrationManager::applyCalibration(d.nitrogen, profile);
    float pCalibrated = CalibrationManager::applyCalibration(d.phosphorus, profile);
    float kCalibrated = CalibrationManager::applyCalibration(d.potassium, profile);

    // ШАГ 2: Применяем математическую компенсацию (температурная, влажностная)
    float ec25 = ecCalibrated / (1.0f + 0.021f * (tempCalibrated - 25.0f));
    d.ec = correctEC(ec25, tempCalibrated, humCalibrated, profile.soilType);
    d.ph = correctPH(phCalibrated, tempCalibrated);
    correctNPK(tempCalibrated, humCalibrated, d.nitrogen, d.phosphorus, d.potassium, profile.soilType);
}
```

#### 🔧 **Калибровочный менеджер** {#Kalibrovochnyy-menedzher}
```cpp
// Файл: src/calibration_manager.cpp

class CalibrationManager {
public:
    // Применение калибровочной таблицы с линейной интерполяцией
    static float applyCalibration(float rawValue, const CalibrationProfile& profile) {
        if (!profile.isLoaded()) {
            return rawValue; // Возвращаем исходное значение если таблица не загружена
        }

        // Поиск ближайших точек калибровки
        CalibrationPoint lower, upper;
        if (profile.findInterpolationPoints(rawValue, lower, upper)) {
            // Линейная интерполяция
            float ratio = (rawValue - lower.raw) / (upper.raw - lower.raw);
            return lower.calibrated + ratio * (upper.calibrated - lower.calibrated);
        }

        return rawValue; // Вне диапазона калибровки
    }
};
```

#### 1️⃣ **Модель Арчи - EC компенсация** {#1-Model-Archi-ec-kompensatsiya}
```cpp
float correctEC(float ec25, float temperature, float humidity, SoilType soilType) {
    // Температурная компенсация (Archie, 1942)
    float tempFactor = 1.0f + 0.021f * (temperature - 25.0f);

    // Влажностная компенсация по модели Арчи
    float porosity = getPorosity(soilType);
    float archieCoeff = getArchieCoefficient(soilType);
    float humFactor = pow(humidity / 100.0f, archieCoeff);

    return ec25 * tempFactor * humFactor;
}

float getArchieCoefficient(SoilType soilType) {
    switch (soilType) {
        case SAND: return 0.15f;      // Песок
        case LOAM: return 0.30f;      // Суглинок
        case CLAY: return 0.45f;      // Глина
        case PEAT: return 0.10f;      // Торф
        case SANDY_PEAT: return 0.18f; // Песчано-торфяной
        default: return 0.30f;
    }
}
```

#### 2️⃣ **Уравнение Нернста - pH компенсация** {#2-Uravnenie-Nernsta-ph-kompensatsiya}
```cpp
float correctPH(float phRaw, float temperature) {
    // Температурная поправка pH по уравнению Нернста
    // dE/dT = -0.003 V/°C для pH электрода
    return phRaw - 0.003f * (temperature - 25.0f);
}
```

#### 3️⃣ **FAO 56 + Eur. J. Soil Sci. - NPK компенсация** {#3-fao-56-eur-j-soil-sci-npk-kompensatsiya}
```cpp
void correctNPK(float temperature, float humidity,
                float& nitrogen, float& phosphorus, float& potassium,
                SoilType soilType) {
    // Температурная компенсация NPK
    float tempFactor = 1.0f - 0.02f * (temperature - 25.0f);

    // Влажностная компенсация по FAO 56
    float humFactor = 1.0f + 0.05f * (humidity - 50.0f) / 50.0f;

    // Применение компенсации
    nitrogen *= tempFactor * humFactor;
    phosphorus *= tempFactor * humFactor;
    potassium *= tempFactor * humFactor;
}
```

## 🌐 **ВЕБ-ИНТЕРФЕЙС** {#VEB-INTERFEYS}

### 📱 **Архитектура веб-интерфейса** {#Arhitektura-veb-interfeysa}
```
┌─────────────────┐    ┌─────────────────┐    ┌─────────────────┐
│   Web Browser   │    │   ESP32 Web     │    │   Data Storage  │
│   (Frontend)    │◄──►│   Server        │◄──►│   (NVS/Flash)   │
└─────────────────┘    └─────────────────┘    └─────────────────┘
                              │
                              ▼
                       ┌─────────────────┐
                       │   Sensor Data   │
                       │   Processing    │
                       └─────────────────┘
```

### 🛣️ **Маршрутизация** {#Marshrutizatsiya}
```cpp
// Файл: src/web/routes_data.cpp

void setupDataRoutes() {
    // Основные маршруты данных
    server.on("/readings", HTTP_GET, handleReadings);
    server.on("/api/sensor", HTTP_GET, handleApiData);
    server.on("/api/calibration", HTTP_POST, handleCalibrationUpload);

    // Статические файлы
    server.on("/calibration_example.csv", HTTP_GET, handleStaticFile);
    server.on("/favicon.ico", HTTP_GET, handleStaticFile);
}
```

### 🎨 **UI компоненты** {#ui-komponenty}
- **Информационные блоки** с объяснением процессов компенсации
- **Визуальная индикация** статуса калибровки
- **Цветовая кодировка** значений (зеленый, желтый, оранжевый, красный)
- **Стрелки изменений** ↑↓ для показа динамики
- **Ссылки на примеры** CSV файлов

## 🔌 **ИНТЕГРАЦИИ** {#INTEGRATsII}

### 🌐 **MQTT клиент** {#mqtt-klient}
```cpp
// Файл: src/mqtt_client.cpp

class MQTTClient {
public:
    // Подключение к MQTT серверу
    bool connect(const String& server, int port,
                 const String& username, const String& password);

    // Публикация данных
    bool publishData(const SensorData& data);

    // Подписка на команды
    void subscribeToCommands();

private:
    // Обработка входящих сообщений
    void handleMessage(const String& topic, const String& payload);
};
```

### ☁️ **ThingSpeak интеграция** {#thingspeak-integratsiya}
```cpp
// Файл: src/thingspeak_client.cpp

class ThingSpeakClient {
public:
    // Отправка данных в ThingSpeak
    bool sendData(const SensorData& data);

    // Получение данных из ThingSpeak
    SensorData getData();

private:
    // HTTP запросы к ThingSpeak API
    bool makeRequest(const String& endpoint, const String& data);
};
```

## 🔍 **ЛОГИРОВАНИЕ И ДИАГНОСТИКА** {#LOGIROVANIE-I-DIAGNOSTIKA}

### 📝 **Система логирования** {#Sistema-logirovaniya}
```cpp
// Файл: src/logger.cpp

class Logger {
public:
    // Уровни логирования
    enum Level { DEBUG, INFO, WARNING, ERROR, CRITICAL };

    // Логирование с уровнем
    static void log(Level level, const String& message);

    // Логирование данных датчика
    static void logSensorData(const SensorData& data);

    // Логирование ошибок
    static void logError(const String& error, const String& context);

private:
    // Форматирование времени
    static String formatTimestamp();

    // Цветной вывод (для отладки)
    static String getColorCode(Level level);
};
```

### 🎯 **Валидация данных** {#Validatsiya-dannyh}
```cpp
// Файл: src/validation_utils.cpp

class ValidationUtils {
public:
    // Валидация данных датчика
    static bool validateSensorData(const SensorData& data);

    // Валидация конфигурации
    static bool validateConfig(const SystemConfig& config);

    // Валидация калибровочной таблицы
    static bool validateCalibrationTable(const CalibrationTable& table);

private:
    // Проверка диапазонов значений
    static bool isInRange(float value, float min, float max);

    // Проверка корректности CSV
    static bool isValidCSV(const String& csvData);
};
```

## 🚀 **ПРОИЗВОДИТЕЛЬНОСТЬ И ОПТИМИЗАЦИЯ** {#PROIZVODITELNOST-I-OPTIMIZATsIYa}

### 📊 **Использование памяти** {#Ispolzovanie-pamyati}
- **RAM:** ~57KB (17.4% от доступной)
- **Flash:** ~1184KB (60.2% от доступной)
- **NVS:** ~4KB для конфигурации
- **Heap:** Динамическое управление

### ⚡ **Оптимизации** {#Optimizatsii}
- **Статические буферы:** Для критических операций
- **Пул памяти:** Для частых операций
- **Кэширование:** Результатов вычислений
- **Lazy loading:** Загрузка данных по требованию

## 🔒 **БЕЗОПАСНОСТЬ** {#BEZOPASNOST}

### 🛡️ **Текущие меры** {#Tekushchie-mery}
- **Rate limiting:** 20 запросов в минуту на IP
- **Input validation:** Проверка всех входных данных
- **Error handling:** Безопасная обработка ошибок

### 🚧 **Планируемые улучшения** {#Planiruemye-uluchsheniya}
- **CSRF токены:** Для всех POST запросов
- **OTA подпись:** SHA-256 для прошивок
- **HTTPS:** Для веб-интерфейса
- **Dependency scanning:** Автоматическая проверка уязвимостей

## 📈 **МОНИТОРИНГ И МЕТРИКИ** {#MONITORING-I-METRIKI}

### 📊 **Ключевые метрики** {#Klyuchevye-metriki}
- **Время отклика:** < 500ms для веб-запросов
- **Использование памяти:** < 80% от доступной
- **Стабильность:** 99.9% uptime
- **Точность:** RMS погрешность < 5%

### 🔍 **Диагностика** {#Diagnostika}
- **Логирование:** Все критические операции
- **Метрики производительности:** Время выполнения функций
- **Алерты:** При превышении пороговых значений

---

## 📋 **ПЛАНЫ РАЗВИТИЯ** {#PLANY-RAZVITIYa}

### 🎯 **Краткосрочные цели (3-6 месяцев)** {#Kratkosrochnye-tseli-3-6-mesyatsev}
1. **Повышение покрытия тестами** до 85%
2. **Улучшение безопасности** (CSRF, OTA подпись)
3. **Оптимизация производительности** (кэширование, пулы памяти)
4. **Расширение документации** (API, примеры использования)

### 🚀 **Долгосрочные цели (6-12 месяцев)** {#Dolgosrochnye-tseli-6-12-mesyatsev}
1. **Модульная архитектура** с четким разделением слоев
2. **Расширенные интеграции** (дополнительные облачные сервисы)
3. **Машинное обучение** для улучшения точности
4. **Мобильное приложение** для управления устройством

---

**Версия:** 3.6.7
**Дата:** 2025-07-05
**Автор:** JXCT Development Team
