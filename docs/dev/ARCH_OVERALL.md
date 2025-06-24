# 🏛️ Архитектура JXCT (v2.7.1)

## 🏗️ **ОБЩАЯ АРХИТЕКТУРА**

### 📊 **Системная архитектура**
```
┌─────────────────┐    ┌─────────────────┐    ┌─────────────────┐
│   JXCT Sensor   │    │      ESP32      │    │   Web Browser   │
│   (Hardware)    │◄──►│   (Controller)  │◄──►│   (Interface)   │
└─────────────────┘    └─────────────────┘    └─────────────────┘
        │                       │                       │
        │ Modbus RTU            │ WiFi                  │ HTTP/HTTPS
        │ 9600 baud             │ 802.11 b/g/n          │ REST API
        │                       │                       │
┌─────────────────┐    ┌─────────────────┐    ┌─────────────────┐
│   Cloud MQTT    │    │   ThingSpeak    │    │   Local Storage │
│   (IoT Platform)│    │   (Data Log)    │    │   (NVS Flash)   │
└─────────────────┘    └─────────────────┘    └─────────────────┘
```

### 🔧 **Модульная структура**
```
JXCT/
├── src/
│   ├── main.cpp                 # Точка входа, инициализация
│   ├── config.cpp               # Конфигурация системы
│   ├── modbus_sensor.cpp        # Работа с датчиком JXCT
│   ├── calibration_manager.cpp  # ✅ Калибровка и компенсация
│   ├── mqtt_client.cpp          # MQTT клиент
│   ├── thingspeak_client.cpp    # ThingSpeak интеграция
│   ├── ota_manager.cpp          # OTA обновления
│   ├── wifi_manager.cpp         # WiFi управление
│   ├── logger.cpp               # Система логирования
│   ├── validation_utils.cpp     # Валидация данных
│   ├── jxct_format_utils.cpp    # Форматирование данных
│   ├── jxct_ui_system.cpp       # UI компоненты
│   └── web/                     # Веб-интерфейс
│       ├── routes_data.cpp      # API данных
│       ├── routes_config.cpp    # API конфигурации
│       ├── routes_calibration.cpp # API калибровки
│       ├── routes_service.cpp   # Статические файлы
│       └── error_handlers.cpp   # Обработка ошибок
├── include/                     # Заголовочные файлы
├── docs/                        # Документация
├── examples/                    # Примеры использования
└── scripts/                     # Скрипты сборки и релиза
```

## 🔬 **СИСТЕМА КОМПЕНСАЦИИ И КАЛИБРОВКИ**

### ✅ **ИСПРАВЛЕННАЯ ДВУХЭТАПНАЯ КОМПЕНСАЦИЯ**

#### 📊 **Алгоритм применения компенсации**
```cpp
// Функция: applyCompensationIfEnabled()
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

#### 🔧 **Калибровочный менеджер**
```cpp
// Файл: src/calibration_manager.cpp

class CalibrationManager {
public:
    // ✅ НОВАЯ ФУНКЦИЯ: Применение CSV калибровочной таблицы
    static float applyCalibration(float rawValue, const SoilProfile& profile) {
        if (!profile.calibrationEnabled) {
            return rawValue; // Возвращаем сырое значение если калибровка отключена
        }
        
        // Линейная интерполяция между точками калибровки
        float coefficient = interpolateCalibration(rawValue, profile.calibrationTable);
        return rawValue * coefficient; // ✅ КОРРЕКТНАЯ ФОРМУЛА
    }
    
private:
    static float interpolateCalibration(float value, const CalibrationTable& table) {
        // Поиск ближайших точек калибровки
        // Линейная интерполяция коэффициента
        // Возврат интерполированного коэффициента
    }
};
```

### 📊 **Научные модели компенсации**

#### 1️⃣ **Модель Арчи (1942) - EC компенсация**
```cpp
// Файл: src/sensor_compensation.cpp

float correctEC(float ec25, float temperature, float humidity, SoilType soilType) {
    // Температурная компенсация к 25°C
    float ecTemp = ec25 / (1.0f + 0.021f * (temperature - 25.0f));
    
    // Влажностная компенсация по модели Арчи
    float k = getArchieCoefficient(soilType);
    float theta = humidity / 100.0f; // Нормализация влажности
    float thetaSat = getSaturationMoisture(soilType);
    
    return ecTemp * pow(thetaSat / theta, k);
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

#### 2️⃣ **Уравнение Нернста - pH компенсация**
```cpp
float correctPH(float phRaw, float temperature) {
    // Температурная поправка pH по уравнению Нернста
    // dE/dT = -0.003 V/°C для pH электрода
    return phRaw - 0.003f * (temperature - 25.0f);
}
```

#### 3️⃣ **FAO 56 + Eur. J. Soil Sci. - NPK компенсация**
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

## 🌐 **ВЕБ-ИНТЕРФЕЙС**

### 📱 **Архитектура веб-интерфейса**
```
┌─────────────────┐    ┌─────────────────┐    ┌─────────────────┐
│   Web Browser   │    │   ESP32 Web     │    │   Data Storage  │
│   (Frontend)    │◄──►│   Server        │◄──►│   (NVS/Flash)   │
└─────────────────┘    └─────────────────┘    └─────────────────┘
        │                       │                       │
        │ HTTP/HTTPS            │ REST API              │ JSON/CSV
        │ HTML/CSS/JS           │ JSON Responses        │ Binary Data
        │                       │                       │
┌─────────────────┐    ┌─────────────────┐    ┌─────────────────┐
│   Static Files  │    │   API Endpoints │    │   Error Handler │
│   (CSS/JS/IMG)  │    │   (/api/*)      │    │   (4xx/5xx)     │
└─────────────────┘    └─────────────────┘    └─────────────────┘
```

### 🔧 **API Endpoints**
```cpp
// Файл: src/web/routes_data.cpp

// GET /api/readings - Текущие показания
void handleReadings() {
    // Возвращает JSON с текущими данными датчика
    // Включает объяснения компенсации и калибровки
}

// GET /api/data - Исторические данные
void handleData() {
    // Возвращает CSV с историческими данными
    // Поддерживает фильтрацию по датам
}

// POST /api/calibration - Загрузка калибровочной таблицы
void handleCalibrationUpload() {
    // Загружает CSV файл калибровки
    // Валидирует формат и применяет калибровку
}

// GET /api/calibration/example - Пример CSV файла
void handleCalibrationExample() {
    // Возвращает пример калибровочной таблицы
    // Для скачивания пользователем
}
```

### 🎨 **UI компоненты**
```cpp
// Файл: src/jxct_ui_system.cpp

class UISystem {
public:
    // Генерация HTML страниц
    static String generateReadingsPage(const SensorData& data);
    static String generateConfigPage(const SystemConfig& config);
    static String generateOTAPage();
    
    // UI компоненты
    static String createValueCard(const String& title, float value, 
                                 const String& unit, const String& status);
    static String createCalibrationStatus(bool enabled, const String& profile);
    static String createCompensationInfo(const String& model, const String& source);
};
```

## 🔄 **СИСТЕМА ОБНОВЛЕНИЙ (OTA)**

### 📦 **OTA архитектура**
```
┌─────────────────┐    ┌─────────────────┐    ┌─────────────────┐
│   GitHub        │    ┌─────────────────┐    │   ESP32         │
│   Releases      │◄──►│   Manifest      │◄──►│   OTA Client    │
└─────────────────┘    │   Server        │    └─────────────────┘
                       └─────────────────┘              │
                                                        │ HTTP
                                                        ▼
                                               ┌─────────────────┐
                                               │   Firmware      │
                                               │   Storage       │
                                               └─────────────────┘
```

### 🔧 **OTA процесс**
```cpp
// Файл: src/ota_manager.cpp

class OTAManager {
public:
    // Проверка обновлений
    bool checkForUpdates() {
        // 1. Загрузка manifest.json
        // 2. Сравнение версий
        // 3. Возврат информации об обновлении
    }
    
    // Установка обновления
    bool installUpdate(const String& url, const String& sha256) {
        // 1. Загрузка прошивки
        // 2. Проверка SHA256
        // 3. Установка обновления
        // 4. Перезагрузка системы
    }
};
```

## 📊 **ХРАНЕНИЕ ДАННЫХ**

### 💾 **NVS (Non-Volatile Storage)**
```cpp
// Структура хранения в NVS
struct NVSData {
    // Конфигурация
    char wifi_ssid[32];
    char wifi_password[64];
    char mqtt_server[64];
    int mqtt_port;
    
    // Калибровка
    bool calibration_enabled;
    CalibrationTable calibration_table;
    SoilType soil_type;
    
    // OTA
    char manifest_url[256];
    unsigned long last_check_ts;
    bool auto_update_enabled;
};
```

### 📈 **Исторические данные**
```cpp
// Структура записи данных
struct DataRecord {
    unsigned long timestamp;  // Unix timestamp
    float temperature;        // °C
    float humidity;          // %
    float ec;               // µS/cm
    float ph;               // pH
    float nitrogen;         // мг/кг
    float phosphorus;       // мг/кг
    float potassium;        // мг/кг
    bool compensation_applied; // Флаг применения компенсации
};
```

## 🔌 **ИНТЕГРАЦИИ**

### 🌐 **MQTT клиент**
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

### ☁️ **ThingSpeak интеграция**
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

## 🔍 **ЛОГИРОВАНИЕ И ДИАГНОСТИКА**

### 📝 **Система логирования**
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

### 🔧 **Валидация данных**
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

## 🚀 **ПРОИЗВОДИТЕЛЬНОСТЬ И ОПТИМИЗАЦИЯ**

### 📊 **Использование памяти**
- **RAM:** ~57KB (17.4% от доступной)
- **Flash:** ~1184KB (60.2% от доступной)
- **NVS:** ~4KB для конфигурации
- **Heap:** Динамическое управление

### ⚡ **Оптимизации**
- **Статические буферы:** Для критических операций
- **Пул соединений:** Для HTTP запросов
- **Кэширование:** Часто используемых данных
- **Сжатие:** JSON ответов

### 🔄 **Планировщик задач**
```cpp
// Основной цикл задач
void loop() {
    // 1. Чтение датчика (каждые 30 секунд)
    if (millis() - lastSensorRead > SENSOR_READ_INTERVAL) {
        readSensorData();
        lastSensorRead = millis();
    }
    
    // 2. Отправка данных (каждые 5 минут)
    if (millis() - lastDataSend > DATA_SEND_INTERVAL) {
        sendDataToCloud();
        lastDataSend = millis();
    }
    
    // 3. Проверка OTA (каждый час)
    if (millis() - lastOtaCheck > OTA_CHECK_INTERVAL) {
        checkForUpdates();
        lastOtaCheck = millis();
    }
    
    // 4. Обработка веб-запросов
    webServer.handleClient();
    
    // 5. Обработка MQTT
    mqttClient.loop();
}
```

---

**Версия документации:** 3.4.6  
**Дата обновления:** 2025-06-24  
**Статус:** ✅ Актуально 