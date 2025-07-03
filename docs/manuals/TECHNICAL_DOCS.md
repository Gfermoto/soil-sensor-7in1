# 🔧 Техническая документация JXCT 7-в-1

**Дата:** Июнь 2025
**Версия:** 3.6.7
**Автор:** JXCT Development Team

---

## 📖 Содержание

1. [🏗️ Архитектура системы](#️-архитектура-системы)
2. [🔌 Аппаратная архитектура](#-аппаратная-архитектура)
3. [💻 Программная архитектура](#-программная-архитектура)
4. [📡 Сетевые протоколы](#-сетевые-протоколы)
5. [🔬 Алгоритмы компенсации](#-алгоритмы-компенсации)
6. [🌐 Веб-интерфейс](#-веб-интерфейс)
7. [📊 API документация](#-api-документация)
8. [🔧 Конфигурация](#-конфигурация)
9. [📁 Структура проекта](#-структура-проекта)
10. [🛠️ Разработка](#️-разработка)

---

## 🏗️ Архитектура системы

### 🎯 Общая концепция

JXCT 7-в-1 представляет собой IoT устройство для мониторинга почвы, построенное на принципах:

- **Модульность:** Разделение на независимые компоненты
- **Масштабируемость:** Возможность добавления новых функций
- **Надежность:** Обработка ошибок и восстановление
- **Производительность:** Оптимизация для ESP32

### 🔄 Жизненный цикл системы

```
Загрузка → Инициализация → Основной цикл → Обработка ошибок → Перезагрузка
    ↓           ↓              ↓              ↓              ↓
  NVS        WiFi/MQTT     Измерения      Логирование    Сохранение
  Загрузка    Подключение   Компенсация    Ошибок        Состояния
```

---

## 🔌 Аппаратная архитектура

### 🧩 Основные компоненты

#### ESP32 микроконтроллер
- **Модель:** ESP32-WROOM-32 (рекомендуется)
- **Процессор:** Dual-core Xtensa LX6 @ 240MHz
- **RAM:** 520KB SRAM
- **Flash:** 4MB (SPIFFS для файловой системы)
- **WiFi:** 802.11 b/g/n
- **Bluetooth:** 4.2 BR/EDR + BLE

#### JXCT 7-в-1 датчик
- **Интерфейс:** Modbus RTU
- **Скорость:** 9600 bps
- **Питание:** 3.3V
- **Потребление:** < 50mA
- **Диапазон температур:** -40°C до +85°C

### 🔌 Схема подключения

```
ESP32                    JXCT Sensor
┌─────────┐              ┌─────────┐
│  3.3V   │──────────────│   VCC   │
│         │              │         │
│  GND    │──────────────│   GND   │
│         │              │         │
│ GPIO2   │──────────────│   TX    │
│         │              │         │
│ GPIO4   │──────────────│   RX    │
└─────────┘              └─────────┘
```

### 📊 Характеристики датчика

| Параметр | Диапазон | Точность | Единицы |
|----------|----------|----------|---------|
| Температура | 0-50°C | ±0.5°C | °C |
| Влажность | 0-100% | ±3% | % |
| EC | 0-5000 | ±5% | µS/cm |
| pH | 3-9 | ±0.3 | pH |
| Азот | 0-2000 | ±10% | мг/кг |
| Фосфор | 0-1000 | ±10% | мг/кг |
| Калий | 0-2000 | ±10% | мг/кг |

---

## 💻 Программная архитектура

### 🏛️ Архитектурные слои

```
┌─────────────────────────────────────┐
│           Веб-интерфейс             │ ← Пользовательский интерфейс
├─────────────────────────────────────┤
│           API слой                  │ ← REST API и JSON
├─────────────────────────────────────┤
│         Бизнес-логика               │ ← Компенсация, калибровка
├─────────────────────────────────────┤
│         Слой данных                 │ ← Датчики, NVS, файлы
├─────────────────────────────────────┤
│         Сетевой слой                │ ← WiFi, MQTT, HTTP
├─────────────────────────────────────┤
│         Аппаратный слой             │ ← ESP32, Modbus
└─────────────────────────────────────┘
```

### 📦 Основные модули

#### 1. **Модуль датчика** (`modbus_sensor.cpp`)
- Чтение данных с JXCT датчика
- Обработка Modbus RTU протокола
- Валидация полученных данных
- Обработка ошибок связи

#### 2. **Модуль компенсации** (`sensor_compensation.cpp`)
- Применение научных моделей
- Температурная компенсация
- Влажностная компенсация
- Коррекция по типу почвы

#### 3. **Модуль калибровки** (`calibration_manager.cpp`)
- Управление CSV калибровочными таблицами
- Линейная интерполяция
- Применение коэффициентов коррекции
- Валидация калибровочных данных

#### 4. **Веб-сервер** (`web/`)
- HTTP сервер на ESP32
- REST API endpoints
- HTML страницы
- Обработка форм и файлов

#### 5. **MQTT клиент** (`mqtt_client.cpp`)
- Подключение к MQTT брокеру
- Публикация данных
- Обработка команд
- Автоматическое переподключение

#### 6. **WiFi менеджер** (`wifi_manager.cpp`)
- Управление WiFi подключением
- Режимы AP/STA
- Автоматическое переподключение
- Диагностика сети

### 🔄 Основной цикл работы

```cpp
void loop() {
    // 1. Чтение данных с датчика
    if (readSensorData()) {
        // 2. Применение калибровки
        applyCalibration();

        // 3. Математическая компенсация
        applyCompensation();

        // 4. Обновление веб-интерфейса
        updateWebInterface();

        // 5. Проверка необходимости публикации
        if (shouldPublish()) {
            publishToMQTT();
            publishToThingSpeak();
        }
    }

    // 6. Обработка веб-запросов
    webServer.handleClient();

    // 7. Проверка OTA обновлений
    checkOTAUpdates();

    // 8. Задержка до следующего цикла
    delay(config.sensorReadInterval);
}
```

---

## 📡 Сетевые протоколы

### 🌐 WiFi

#### Режимы работы
- **STA (Station):** Подключение к существующей сети
- **AP (Access Point):** Создание точки доступа
- **AP+STA:** Одновременная работа в обоих режимах

#### Конфигурация
```cpp
// STA режим
const char* WIFI_SSID = "your_network";
const char* WIFI_PASSWORD = "your_password";

// AP режим
const char* AP_SSID = "JXCT_Setup";
const char* AP_PASSWORD = "12345678";
```

### 📡 MQTT

#### Структура топиков
```
jxct/sensor/{device_id}/temperature
jxct/sensor/{device_id}/humidity
jxct/sensor/{device_id}/ec
jxct/sensor/{device_id}/ph
jxct/sensor/{device_id}/nitrogen
jxct/sensor/{device_id}/phosphorus
jxct/sensor/{device_id}/potassium
jxct/sensor/{device_id}/status
```

#### Формат сообщений
```json
{
  "timestamp": 1640995200,
  "value": 25.5,
  "unit": "°C",
  "quality": "good",
  "compensated": true
}
```

### 🌍 ThingSpeak

#### Структура канала
- **Field 1:** Температура (°C)
- **Field 2:** Влажность (%)
- **Field 3:** EC (µS/cm)
- **Field 4:** pH
- **Field 5:** Азот (мг/кг)
- **Field 6:** Фосфор (мг/кг)
- **Field 7:** Калий (мг/кг)
- **Field 8:** Статус (битовая маска)

### 🔌 Modbus RTU

#### Регистры датчика
| Адрес | Описание | Единицы | Диапазон |
|-------|----------|---------|----------|
| 0x0001 | Температура | 0.1°C | -400-800 |
| 0x0002 | Влажность | 0.1% | 0-1000 |
| 0x0003 | EC | 1 µS/cm | 0-65535 |
| 0x0004 | pH | 0.1 pH | 0-140 |
| 0x0005 | Азот | 1 мг/кг | 0-65535 |
| 0x0006 | Фосфор | 1 мг/кг | 0-65535 |
| 0x0007 | Калий | 1 мг/кг | 0-65535 |

#### Формат запроса
```
01 03 00 01 00 07 25 CA
│  │  │  │  │  │  │  │
│  │  │  │  │  │  │  └─ CRC
│  │  │  │  │  │  └───── Количество регистров (7)
│  │  │  │  │  └──────── Начальный адрес (0x0001)
│  │  │  └─┴──────────── Код функции (03 - чтение)
│  └─┴────────────────── Адрес устройства (01)
```

---

## 🔬 Алгоритмы компенсации

### 🌡️ Температурная компенсация

#### Модель Арчи для EC
```cpp
float compensateEC(float ec, float temperature, SoilType soilType) {
    // Коэффициенты по типам почв
    float k = getSoilCoefficient(soilType);

    // Температурная компенсация
    float tempFactor = 1.0 + 0.02 * (temperature - 25.0);

    // Модель Арчи: EC = σ * φ^m
    return ec * tempFactor * k;
}
```

#### Уравнение Нернста для pH
```cpp
float compensatePH(float ph, float temperature) {
    // Температурная поправка: -0.003 pH/°C
    float tempCorrection = -0.003 * (temperature - 25.0);
    return ph + tempCorrection;
}
```

### 💧 Влажностная компенсация

#### FAO 56 модель для NPK
```cpp
float compensateNPK(float npk, float humidity, SoilType soilType) {
    // Базовый коэффициент влажности
    float humidityFactor = 1.0 + 0.1 * (humidity - 60.0) / 40.0;

    // Коррекция по типу почвы
    float soilFactor = getSoilHumidityFactor(soilType);

    return npk * humidityFactor * soilFactor;
}
```

### 📊 Двухэтапная система

#### Этап 1: CSV калибровка
```cpp
float applyCalibration(float rawValue, SoilProfile profile) {
    if (!hasCalibrationTable(profile)) {
        return rawValue;
    }

    // Линейная интерполяция
    float factor = interpolateCalibration(rawValue, profile);
    return rawValue * factor;
}
```

#### Этап 2: Математическая компенсация
```cpp
float applyCompensation(float calibratedValue, SensorData data) {
    switch (data.type) {
        case SENSOR_EC:
            return compensateEC(calibratedValue, data.temperature, data.soilType);
        case SENSOR_PH:
            return compensatePH(calibratedValue, data.temperature);
        case SENSOR_NPK:
            return compensateNPK(calibratedValue, data.humidity, data.soilType);
        default:
            return calibratedValue;
    }
}
```

---

## 🌐 Веб-интерфейс

### 🏗️ Архитектура

#### Компоненты
- **HTTP сервер:** Встроенный в ESP32
- **HTML генерация:** Динамическая генерация страниц
- **JavaScript:** AJAX для обновления данных
- **CSS:** Адаптивный дизайн

#### Структура страниц
```
/                    → Главная (настройки WiFi/MQTT)
/readings           → Показания датчика
/intervals          → Настройка интервалов
/updates            → OTA обновления
/service            → Сервисные функции
/api/v3.6.9/sensor      → JSON API
```

### 📱 Адаптивный дизайн

#### Breakpoints
- **Mobile:** < 768px
- **Tablet:** 768px - 1024px
- **Desktop:** > 1024px

#### CSS Grid система
```css
.container {
    display: grid;
    grid-template-columns: repeat(auto-fit, minmax(300px, 1fr));
    gap: 20px;
}
```

### 🔄 AJAX обновления

#### JavaScript API
```javascript
// Получение данных датчика
fetch('/api/v3.6.9/sensor')
    .then(response => response.json())
    .then(data => updateDisplay(data));

// Обновление каждые 3 секунды
setInterval(updateSensorData, 3000);
```

---

## 📊 API документация

### 🌐 REST API

#### GET `/api/v3.6.9/sensor`
Получение текущих показаний датчика

**Ответ:**
```json
{
  "timestamp": 1640995200,
  "temperature": {
    "raw": 25.3,
    "compensated": 25.5,
    "recommended": 22.0,
    "unit": "°C"
  },
  "humidity": {
    "raw": 65.2,
    "compensated": 65.0,
    "recommended": 60.0,
    "unit": "%"
  },
  "ec": {
    "raw": 1200,
    "compensated": 1180,
    "recommended": 1500,
    "unit": "µS/cm"
  },
  "ph": {
    "raw": 6.8,
    "compensated": 6.7,
    "recommended": 6.5,
    "unit": "pH"
  },
  "nitrogen": {
    "raw": 35,
    "compensated": 38,
    "recommended": 40,
    "unit": "mg/kg"
  },
  "phosphorus": {
    "raw": 12,
    "compensated": 11,
    "recommended": 10,
    "unit": "mg/kg"
  },
  "potassium": {
    "raw": 28,
    "compensated": 30,
    "recommended": 30,
    "unit": "mg/kg"
  },
  "status": {
    "sensor": "ok",
    "wifi": "connected",
    "mqtt": "connected",
    "calibration": "enabled"
  }
}
```

#### GET `/api/v3.6.9/config`
Получение текущей конфигурации

**Ответ:**
```json
{
  "wifi": {
    "ssid": "your_network",
    "mode": "sta"
  },
  "mqtt": {
    "enabled": true,
    "server": "mqtt.example.com",
    "port": 1883,
    "topic": "jxct/sensor/001"
  },
  "sensor": {
    "read_interval": 30000,
    "calibration_enabled": true,
    "soil_type": "loam",
    "crop": "tomato"
  }
}
```

#### POST `/api/v3.6.9/config`
Обновление конфигурации

**Тело запроса:**
```json
{
  "wifi": {
    "ssid": "new_network",
    "password": "new_password"
  },
  "sensor": {
    "read_interval": 60000
  }
}
```

#### GET `/api/v3.6.9/status`
Получение системного статуса

**Ответ:**
```json
{
  "version": "3.6.7",
  "uptime": 86400,
  "free_memory": 150000,
  "wifi_rssi": -45,
  "mqtt_connected": true,
  "sensor_connected": true,
  "last_reading": 1640995200
}
```

### 📡 MQTT API

#### Топики для публикации
```
jxct/sensor/{device_id}/data
jxct/sensor/{device_id}/status
jxct/sensor/{device_id}/config
```

#### Топики для подписки
```
jxct/sensor/{device_id}/command
jxct/sensor/{device_id}/config/update
```

#### Формат команд
```json
{
  "command": "restart",
  "timestamp": 1640995200,
  "id": "cmd_001"
}
```

---

## 🔧 Конфигурация

### 📝 Структура конфигурации

```cpp
struct Config {
    // WiFi настройки
    char ssid[32];
    char password[64];

    // MQTT настройки
    bool mqttEnabled;
    char mqttServer[64];
    int mqttPort;
    char mqttUser[32];
    char mqttPassword[32];
    char mqttTopic[64];

    // ThingSpeak настройки
    bool thingSpeakEnabled;
    char thingSpeakApiKey[64];
    unsigned long thingSpeakChannelId;

    // Настройки датчика
    int sensorReadInterval;
    int mqttPublishInterval;
    int thingSpeakInterval;
    int webUpdateInterval;

    // Фильтры
    float deltaTemperature;
    float deltaHumidity;
    float deltaPh;
    float deltaEc;
    float deltaNpk;

    // Калибровка
    bool calibrationEnabled;
    SoilProfile soilProfile;

    // Культура и среда
    char cropId[16];
    EnvironmentType environmentType;
    float latitude;
    float longitude;

    // Флаги
    struct {
        uint8_t useRealSensor : 1;
        uint8_t seasonalAdjustEnabled : 1;
        uint8_t outlierFilterEnabled : 1;
        uint8_t isGreenhouse : 1;
    } flags;
};
```

### 💾 Хранение конфигурации

#### NVS (Non-Volatile Storage)
```cpp
// Сохранение
void saveConfig() {
    Preferences prefs;
    prefs.begin("jxct", false);
    prefs.putBytes("config", &config, sizeof(config));
    prefs.end();
}

// Загрузка
void loadConfig() {
    Preferences prefs;
    prefs.begin("jxct", true);
    prefs.getBytes("config", &config, sizeof(config));
    prefs.end();
}
```

### 🔄 Валидация конфигурации

```cpp
bool validateConfig() {
    // Проверка WiFi
    if (strlen(config.ssid) == 0) return false;

    // Проверка MQTT
    if (config.mqttEnabled) {
        if (strlen(config.mqttServer) == 0) return false;
        if (config.mqttPort < 1 || config.mqttPort > 65535) return false;
    }

    // Проверка интервалов
    if (config.sensorReadInterval < 1000) return false;
    if (config.mqttPublishInterval < 60000) return false;

    return true;
}
```

---

## 📁 Структура проекта

```
JXCT/
├── src/                          # Исходный код
│   ├── main.cpp                  # Главный файл
│   ├── config.cpp                # Конфигурация
│   ├── modbus_sensor.cpp         # Работа с датчиком
│   ├── sensor_compensation.cpp   # Компенсация данных
│   ├── calibration_manager.cpp   # Калибровка
│   ├── mqtt_client.cpp           # MQTT клиент
│   ├── wifi_manager.cpp          # WiFi управление
│   ├── ota_manager.cpp           # OTA обновления
│   └── web/                      # Веб-интерфейс
│       ├── routes_main.cpp       # Главные маршруты
│       ├── routes_data.cpp       # Данные датчика
│       ├── routes_config.cpp     # Конфигурация
│       ├── routes_ota.cpp        # OTA обновления
│       └── routes_service.cpp    # Сервисные функции
├── include/                      # Заголовочные файлы
│   ├── ISensor.h                 # Интерфейс датчика
│   ├── basic_sensor_adapter.h    # Базовый адаптер
│   ├── modbus_sensor_adapter.h   # Modbus адаптер
│   ├── calibration_manager.h     # Калибровка
│   ├── sensor_compensation.h     # Компенсация
│   ├── mqtt_client.h             # MQTT клиент
│   ├── wifi_manager.h            # WiFi управление
│   ├── ota_manager.h             # OTA обновления
│   ├── web_routes.h              # Веб маршруты
│   ├── jxct_config_vars.h        # Конфигурация
│   ├── jxct_constants.h          # Константы
│   ├── jxct_ui_system.h          # UI система
│   ├── logger.h                  # Логирование
│   └── version.h                 # Версия
├── docs/                         # Документация
│   ├── manuals/                  # Руководства
│   ├── dev/                      # Разработка
│   ├── examples/                 # Примеры
│   └── html/                     # Doxygen
├── test/                         # Тесты
│   ├── test_validation_utils.cpp # Тесты валидации
│   └── stubs/                    # Заглушки
├── scripts/                      # Скрипты
│   ├── release.ps1               # Релиз
│   └── auto_version.py           # Автоверсионирование
├── platformio.ini                # Конфигурация PlatformIO
├── Doxyfile                      # Конфигурация Doxygen
└── README.md                     # Основная документация
```

---

## 🛠️ Разработка

### 🔧 Требования к окружению

#### PlatformIO
```ini
[env:esp32dev]
platform = espressif32
board = esp32dev
framework = arduino
monitor_speed = 115200
build_flags = -DCORE_DEBUG_LEVEL=3
lib_deps =
    arduino-libraries/ArduinoJson@^6.21.3
    knolleary/PubSubClient@^2.8
    arduino-libraries/NTPClient@^3.2.1
    bblanchon/ArduinoJson@^6.21.3
```

#### Зависимости
- **ArduinoJson:** Работа с JSON
- **PubSubClient:** MQTT клиент
- **NTPClient:** Синхронизация времени
- **ESP32 Arduino:** Основной фреймворк

### 📝 Стандарты кодирования

#### Именование
```cpp
// Классы: PascalCase
class CalibrationManager { };

// Функции: camelCase
void applyCompensation() { }

// Константы: UPPER_SNAKE_CASE
const int MAX_RETRY_COUNT = 3;

// Переменные: camelCase
int sensorReadInterval = 30000;
```

#### Комментарии
```cpp
/**
 * @brief Применяет температурную компенсацию к значению EC
 * @param ec Исходное значение EC
 * @param temperature Температура в градусах Цельсия
 * @param soilType Тип почвы
 * @return Скомпенсированное значение EC
 */
float compensateEC(float ec, float temperature, SoilType soilType);
```

### 🧪 Тестирование

#### Структура тестов
```cpp
#include <unity.h>

void test_compensation() {
    float result = compensateEC(1000, 25.0, SoilType::LOAM);
    TEST_ASSERT_FLOAT_WITHIN(50, 1000, result);
}

void test_calibration() {
    float result = applyCalibration(1000, SoilProfile::SAND);
    TEST_ASSERT_FLOAT_WITHIN(100, 1000, result);
}

int main() {
    UNITY_BEGIN();
    RUN_TEST(test_compensation);
    RUN_TEST(test_calibration);
    return UNITY_END();
}
```

### 📊 Логирование

#### Уровни логирования
```cpp
// Отладка
logDebug("Чтение датчика: %d", sensorId);

// Информация
logInfo("Данные получены: T=%.1f°C", temperature);

// Предупреждение
logWarning("Высокая температура: %.1f°C", temperature);

// Ошибка
logError("Ошибка связи с датчиком: %s", errorMessage);
```

#### Ротация логов
```cpp
// Максимальный размер лога: 10KB
// Автоматическая очистка старых записей
// Сохранение в SPIFFS
```

### 🚀 CI/CD

#### GitHub Actions
```yaml
name: Build and Test
on: [push, pull_request]

jobs:
  build:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v3.6.9
      - uses: actions/setup-python@v3.6.9
      - run: pip install platformio
      - run: pio run
      - run: pio test
```

---

## 📞 Поддержка

### 💬 Связь с разработчиками
- **Telegram:** [@Gfermoto](https://t.me/Gfermoto)
- **GitHub Issues:** [Сообщить о проблеме](https://github.com/Gfermoto/soil-sensor-7in1/issues)
- **Документация:** [GitHub Pages](https://gfermoto.github.io/soil-sensor-7in1/)

### 📚 Дополнительные ресурсы
- [Руководство пользователя](USER_GUIDE.md)
- [API документация](API.md)
- [Агрономические рекомендации](AGRO_RECOMMENDATIONS.md)
- [Руководство по компенсации](COMPENSATION_GUIDE.md)

### 🔗 Полезные ссылки

- [🌱 GitHub репозиторий](https://github.com/Gfermoto/soil-sensor-7in1) - Исходный код проекта
- [📋 План рефакторинга](../dev/QA_REFACTORING_PLAN_2025H2.md) - Планы развития на 2025
- [📊 Отчет о техническом долге](../dev/TECH_DEBT_REPORT_2025-06.md) - Анализ технических проблем
- [🏗️ Архитектура системы](../dev/ARCH_OVERALL.md) - Общая архитектура проекта

---

**© 2025 JXCT Development Team**
*Версия 3.6.7 | Июнь 2025*
