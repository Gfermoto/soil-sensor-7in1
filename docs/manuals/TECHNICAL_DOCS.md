# 🔧 Техническая документация JXCT 7-в-1

**Дата:** Июль 2025
**Версия:** 3.10.0
**Автор:** JXCT Development Team

---

## 📋 Содержание {#Soderzhanie}

- [Содержание](#Soderzhanie)
- [Архитектура системы](#Arhitektura-sistemy)
  - [Дополнительная документация](#Dopolnitelnaya-dokumentatsiya)
  - [Общая концепция](#Obshchaya-kontseptsiya)
  - [Жизненный цикл системы](#Zhiznennyy-tsikl-sistemy)
- [Аппаратная архитектура](#Apparatnaya-arhitektura)
  - [Основные компоненты](#Osnovnye-komponenty)
    - [ESP32 микроконтроллер](#esp32-mikrokontroller)
    - [JXCT 7-в-1 датчик](#jxct-7-v-1-datchik)
  - [Схема подключения](#Shema-podklyucheniya)
  - [Характеристики датчика](#Harakteristiki-datchika)
- [Программная архитектура](#Programmnaya-arhitektura)
  - [Архитектурные слои](#Arhitekturnye-sloi)
  - [Основные модули](#Osnovnye-moduli)
    - [1. Модуль датчика (modbus_sensor.cpp)](#1-Modul-datchika-modbus_sensorcpp)
    - [2. Модуль компенсации (sensor_compensation.cpp)](#2-Modul-kompensatsii-sensor_compensationcpp)
    - [3. Модуль калибровки (calibration_manager.cpp)](#3-Modul-kalibrovki-calibration_managercpp)
    - [4. Веб-сервер (web)](#4-Veb-server-web)
    - [5. MQTT клиент (mqtt_client.cpp)](#5-mqtt-klient-mqtt_clientcpp)
    - [6. WiFi менеджер (wifi_manager.cpp)](#6-wifi-menedzher-wifi_managercpp)
  - [Основной цикл работы](#Osnovnoy-tsikl-raboty)
- [Сетевые протоколы](#Setevye-protokoly)
  - [WiFi](#wifi)
    - [Режимы работы](#Rezhimy-raboty)
    - [Конфигурация](#Konfiguratsiya)
  - [MQTT](#mqtt)
    - [Структура топиков](#Struktura-topikov)
    - [Формат сообщений](#Format-soobshcheniy)
  - [ThingSpeak](#thingspeak)
    - [Структура канала](#Struktura-kanala)
  - [Modbus RTU](#modbus-rtu)
    - [Регистры датчика](#Registry-datchika)
    - [Формат запроса](#Format-zaprosa)
- [Алгоритмы компенсации](#Algoritmy-kompensatsii)
  - [Температурная компенсация](#Temperaturnaya-kompensatsiya)
    - [Модель Арчи для EC](#Model-Archi-dlya-ec)
    - [Уравнение Нернста для pH](#Uravnenie-Nernsta-dlya-ph)
  - [Влажностная компенсация](#Vlazhnostnaya-kompensatsiya)
    - [FAO 56 модель для NPK](#fao-56-model-dlya-npk)
  - [Двухэтапная система](#Dvuhetapnaya-sistema)
    - [Этап 1 CSV калибровка](#Etap-1-csv-kalibrovka)
    - [Этап 2 Математическая компенсация](#Etap-2-Matematicheskaya-kompensatsiya)
- [Веб-интерфейс](#Veb-interfeys)
  - [Архитектура](#Arhitektura)
    - [Компоненты](#Komponenty)
    - [Структура страниц](#Struktura-stranits)
  - [Адаптивный дизайн](#Adaptivnyy-dizayn)
    - [Breakpoints](#breakpoints)
    - [CSS Grid система](#css-grid-sistema)
  - [AJAX обновления](#ajax-obnovleniya)
    - [JavaScript API](#javascript-api)
- [API документация](#api-dokumentatsiya)
  - [REST API](#rest-api)
    - [GET apiv3.11.0sensor](#get-apiv3.11.0sensor)
- [GET apiv3.11.0config](#get-apiv3.11.0config)
- [POST apiv3.11.0config](#post-apiv3.11.0config)
- [GET apiv3.11.0status](#get-apiv3.11.0status)
  - [MQTT API](#mqtt-api)
    - [Топики для публикации](#Topiki-dlya-publikatsii)
    - [Топики для подписки](#Topiki-dlya-podpiski)
    - [Формат команд](#Format-komand)
- [Конфигурация](#Konfiguratsiya)
  - [Структура конфигурации](#Struktura-konfiguratsii)
  - [Хранение конфигурации](#Hranenie-konfiguratsii)
    - [NVS (Non-Volatile Storage)](#nvs-non-volatile-storage)
  - [Валидация конфигурации](#Validatsiya-konfiguratsii)
- [Структура проекта](#Struktura-proekta)
- [Разработка](#Razrabotka)
  - [Требования к окружению](#Trebovaniya-k-okruzheniyu)
    - [PlatformIO](#platformio)
    - [Зависимости](#Zavisimosti)
  - [Стандарты кодирования](#Standarty-kodirovaniya)
    - [Именование](#Imenovanie)
    - [Комментарии](#Kommentarii)
  - [Тестирование](#Testirovanie)
    - [Структура тестов](#Struktura-testov)
  - [Логирование](#Logirovanie)
    - [Уровни логирования](#Urovni-logirovaniya)
    - [Ротация логов](#Rotatsiya-logov)
  - [CICD](#cicd)
    - [GitHub Actions](#github-actions)
- [Поддержка](#Podderzhka)
  - [Связь с разработчиками](#Svyaz-s-razrabotchikami)
  - [Дополнительные ресурсы](#Dopolnitelnye-resursy)
  - [Полезные ссылки](#Poleznye-ssylki)

---

## 📖 Содержание {#Soderzhanie}

1. **🏗️ Архитектура системы**
2. **🔌 Аппаратная архитектура**
3. **💻 Программная архитектура**
4. **📡 Сетевые протоколы**
5. **🔬 Алгоритмы компенсации**
6. **🌐 Веб-интерфейс**
7. **📊 API документация**
8. **🔧 Конфигурация**
9. **📁 Структура проекта**
10. **🛠️ Разработка**

---

## 🏗️ Архитектура системы {#Arhitektura-sistemy}

### 📖 Дополнительная документация {#Dopolnitelnaya-dokumentatsiya}
- **[🖥️ C++ API](../DOXYGEN_API.md)** - Подробная документация кода
- **[📋 REST API](API.md)** - Веб API для интеграции
- **[🧪 Тестирование](../TESTING_GUIDE.md)** - Тестирование системы

### 🎯 Общая концепция {#Obshchaya-kontseptsiya}

JXCT 7-в-1 представляет собой IoT устройство для мониторинга почвы, построенное на принципах:

- **Модульность:** Разделение на независимые компоненты
- **Масштабируемость:** Возможность добавления новых функций
- **Надежность:** Обработка ошибок и восстановление
- **Производительность:** Оптимизация для ESP32

### 🔄 Жизненный цикл системы {#Zhiznennyy-tsikl-sistemy}

```
Загрузка → Инициализация → Основной цикл → Обработка ошибок → Перезагрузка
    ↓           ↓              ↓              ↓              ↓
  NVS        WiFi/MQTT     Измерения      Логирование    Сохранение
  Загрузка    Подключение   Компенсация    Ошибок        Состояния
```

---

## 🔌 Аппаратная архитектура {#Apparatnaya-arhitektura}

### 🧩 Основные компоненты {#Osnovnye-komponenty}

#### ESP32 микроконтроллер {#esp32-mikrokontroller}
- **Модель:** ESP32-WROOM-32 (рекомендуется)
- **Процессор:** Dual-core Xtensa LX6 @ 240MHz
- **RAM:** 520KB SRAM
- **Flash:** 4MB (SPIFFS для файловой системы)
- **WiFi:** 802.11 b/g/n
- **Bluetooth:** 4.2 BR/EDR + BLE

#### JXCT 7-в-1 датчик {#jxct-7-v-1-datchik}
- **Интерфейс:** Modbus RTU
- **Скорость:** 9600 bps
- **Питание:** 3.3V
- **Потребление:** < 50mA
- **Диапазон температур:** -40°C до +85°C

### 🔌 Схема подключения {#Shema-podklyucheniya}

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

### 📊 Характеристики датчика {#Harakteristiki-datchika}

| Параметр | Диапазон | Точность | Единицы |
|----------|----------|----------|---------|
| Температура | -45°C до 115°C | ±0.5°C | °C |
| Влажность | 0-100%RH | ±3% (0-53%), ±5% (53-100%) | %RH |
| EC | 0-10000 | ±5% | µS/cm |
| pH | 3-9 | ±0.3 | pH |
| NPK | 0-1999 | 2% F.S. | мг/кг |
| Разрешение EC | 10 µS/cm | - | µS/cm |

---

## 💻 Программная архитектура {#Programmnaya-arhitektura}

### 🏛️ Архитектурные слои {#Arhitekturnye-sloi}

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

### 📦 Основные модули {#Osnovnye-moduli}

#### 1. **Модуль датчика** (`modbus_sensor.cpp`) {#1-Modul-datchika-modbus_sensorcpp}
- Чтение данных с JXCT датчика
- Обработка Modbus RTU протокола
- Валидация полученных данных
- Обработка ошибок связи

#### 2. **Модуль компенсации** (`sensor_compensation.cpp`) {#2-Modul-kompensatsii-sensor_compensationcpp}
- Применение научных моделей
- Температурная компенсация
- Влажностная компенсация
- Коррекция по типу почвы

#### 3. **Модуль калибровки** (`calibration_manager.cpp`) {#3-Modul-kalibrovki-calibration_managercpp}
- Управление CSV калибровочными таблицами
- Линейная интерполяция
- Применение коэффициентов коррекции
- Валидация калибровочных данных

#### 4. **Веб-сервер** (`web/`) {#4-Veb-server-web}
- HTTP сервер на ESP32
- REST API endpoints
- HTML страницы
- Обработка форм и файлов

#### 5. **MQTT клиент** (`mqtt_client.cpp`) {#5-mqtt-klient-mqtt_clientcpp}
- Подключение к MQTT брокеру
- Публикация данных
- Обработка команд
- Автоматическое переподключение

#### 6. **WiFi менеджер** (`wifi_manager.cpp`) {#6-wifi-menedzher-wifi_managercpp}
- Управление WiFi подключением
- Режимы AP/STA
- Автоматическое переподключение
- Диагностика сети

### 🔄 Основной цикл работы {#Osnovnoy-tsikl-raboty}

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

## 📡 Сетевые протоколы {#Setevye-protokoly}

### 🌐 WiFi {#wifi}

#### Режимы работы {#Rezhimy-raboty}
- **STA (Station):** Подключение к существующей сети
- **AP (Access Point):** Создание точки доступа
- **AP+STA:** Одновременная работа в обоих режимах

#### Конфигурация {#Konfiguratsiya}
```cpp
// STA режим
const char* WIFI_SSID = "your_network";
const char* WIFI_PASSWORD = "your_password";

// AP режим
const char* AP_SSID = "JXCT_Setup";
const char* AP_PASSWORD = "12345678";
```

### 📡 MQTT {#mqtt}

#### Структура топиков {#Struktura-topikov}
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

#### Формат сообщений {#Format-soobshcheniy}
```json
{
  "timestamp": 1640995200,
  "value": 25.5,
  "unit": "°C",
  "quality": "good",
  "compensated": true
}
```

### 🌍 ThingSpeak {#thingspeak}

#### Структура канала {#Struktura-kanala}
- **Field 1:** Температура (°C)
- **Field 2:** Влажность (%)
- **Field 3:** EC (µS/cm)
- **Field 4:** pH
- **Field 5:** Азот (мг/кг)
- **Field 6:** Фосфор (мг/кг)
- **Field 7:** Калий (мг/кг)
- **Field 8:** Статус (битовая маска)

### 🔌 Modbus RTU {#modbus-rtu}

#### Регистры датчика {#Registry-datchika}
| Адрес | Описание | Единицы | Диапазон |
|-------|----------|---------|----------|
| 0x0006 | pH | 0.01 pH | 300-900 |
| 0x0012 | Влажность | 0.1% | 0-1000 |
| 0x0013 | Температура | 0.1°C | -450-1150 |
| 0x0015 | EC | 1 µS/cm | 0-10000 |
| 0x001E | Азот | 1 мг/кг | 0-1999 |
| 0x001F | Фосфор | 1 мг/кг | 0-1999 |
| 0x0020 | Калий | 1 мг/кг | 0-1999 |

#### Формат запроса {#Format-zaprosa}
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

## 🔬 Алгоритмы компенсации {#Algoritmy-kompensatsii}

### 🌡️ Температурная компенсация {#Temperaturnaya-kompensatsiya}

#### Модель Арчи для EC {#Model-Archi-dlya-ec}
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

#### Уравнение Нернста для pH {#Uravnenie-Nernsta-dlya-ph}
```cpp
float compensatePH(float ph, float temperature) {
    // Температурная поправка: -0.003 pH/°C
    float tempCorrection = -0.003 * (temperature - 25.0);
    return ph + tempCorrection;
}
```

### 💧 Влажностная компенсация {#Vlazhnostnaya-kompensatsiya}

#### FAO 56 модель для NPK {#fao-56-model-dlya-npk}
```cpp
float compensateNPK(float npk, float humidity, SoilType soilType) {
    // Базовый коэффициент влажности
    float humidityFactor = 1.0 + 0.1 * (humidity - 60.0) / 40.0;

    // Коррекция по типу почвы
    float soilFactor = getSoilHumidityFactor(soilType);

    return npk * humidityFactor * soilFactor;
}
```

### 📊 Двухэтапная система {#Dvuhetapnaya-sistema}

#### Этап 1: CSV калибровка {#Etap-1-csv-kalibrovka}
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

#### Этап 2: Математическая компенсация {#Etap-2-Matematicheskaya-kompensatsiya}
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

## 🌐 Веб-интерфейс {#Veb-interfeys}

### 🏗️ Архитектура {#Arhitektura}

#### Компоненты {#Komponenty}
- **HTTP сервер:** Встроенный в ESP32
- **HTML генерация:** Динамическая генерация страниц
- **JavaScript:** AJAX для обновления данных
- **CSS:** Адаптивный дизайн

#### Структура страниц {#Struktura-stranits}
```
/                    → Главная (настройки WiFi/MQTT)
/readings           → Показания датчика
/intervals          → Настройка интервалов
/updates            → OTA обновления
/service            → Сервисные функции
/api/v3.11.0/sensor      → JSON API
```

### 📱 Адаптивный дизайн {#Adaptivnyy-dizayn}

#### Breakpoints {#breakpoints}
- **Mobile:** < 768px
- **Tablet:** 768px - 1024px
- **Desktop:** > 1024px

#### CSS Grid система {#css-grid-sistema}
```css
.container {
    display: grid;
    grid-template-columns: repeat(auto-fit, minmax(300px, 1fr));
    gap: 20px;
}
```

### 🔄 AJAX обновления {#ajax-obnovleniya}

#### JavaScript API {#javascript-api}
```javascript
// Получение данных датчика
fetch('/api/v3.11.0/sensor')
    .then(response => response.json())
    .then(data => updateDisplay(data));

// Обновление каждые 3 секунды
setInterval(updateSensorData, 3000);
```

---

## 📊 API документация {#api-dokumentatsiya}

### 🌐 REST API {#rest-api}

#### GET `/api/v3.11.0/sensor` {#get-apiv3.11.0sensor}
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

#### GET `/api/v3.11.0/config` {#get-apiv3.11.0config}
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

#### POST `/api/v3.11.0/config` {#post-apiv3.11.0config}
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

#### GET `/api/v3.11.0/status` {#get-apiv3.11.0status}
Получение системного статуса

**Ответ:**
```json
{
  "version": "3.10.0",
  "uptime": 86400,
  "free_memory": 150000,
  "wifi_rssi": -45,
  "mqtt_connected": true,
  "sensor_connected": true,
  "last_reading": 1640995200
}
```

### 📡 MQTT API {#mqtt-api}

#### Топики для публикации {#Topiki-dlya-publikatsii}
```
jxct/sensor/{device_id}/data
jxct/sensor/{device_id}/status
jxct/sensor/{device_id}/config
```

#### Топики для подписки {#Topiki-dlya-podpiski}
```
jxct/sensor/{device_id}/command
jxct/sensor/{device_id}/config/update
```

#### Формат команд {#Format-komand}
```json
{
  "command": "restart",
  "timestamp": 1640995200,
  "id": "cmd_001"
}
```

---

## 🔧 Конфигурация {#Konfiguratsiya}

### 📝 Структура конфигурации {#Struktura-konfiguratsii}

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

### 💾 Хранение конфигурации {#Hranenie-konfiguratsii}

#### NVS (Non-Volatile Storage) {#nvs-non-volatile-storage}
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

### 🔄 Валидация конфигурации {#Validatsiya-konfiguratsii}

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

## 📁 Структура проекта {#Struktura-proekta}

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

## 🛠️ Разработка {#Razrabotka}

### 🔧 Требования к окружению {#Trebovaniya-k-okruzheniyu}

#### PlatformIO {#platformio}
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

#### Зависимости {#Zavisimosti}
- **ArduinoJson:** Работа с JSON
- **PubSubClient:** MQTT клиент
- **NTPClient:** Синхронизация времени
- **ESP32 Arduino:** Основной фреймворк

### 📝 Стандарты кодирования {#Standarty-kodirovaniya}

#### Именование {#Imenovanie}
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

#### Комментарии {#Kommentarii}
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

### 🧪 Тестирование {#Testirovanie}

#### Структура тестов {#Struktura-testov}
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

### 📊 Логирование {#Logirovanie}

#### Уровни логирования {#Urovni-logirovaniya}
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

#### Ротация логов {#Rotatsiya-logov}
```cpp
// Максимальный размер лога: 10KB
// Автоматическая очистка старых записей
// Сохранение в SPIFFS
```

### 🚀 CI/CD {#cicd}

#### GitHub Actions {#github-actions}
```yaml
name: Build and Test
on: [push, pull_request]

jobs:
  build:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v3.11.0
- uses: actions/setup-python@v3.11.0
      - run: pip install platformio
      - run: pio run
      - run: pio test
```

---

## 📞 Поддержка {#Podderzhka}

### 💬 Связь с разработчиками {#Svyaz-s-razrabotchikami}
- **Telegram:** [@Gfermoto](https://t.me/Gfermoto)
- **GitHub Issues:** [Сообщить о проблеме](https://github.com/Gfermoto/soil-sensor-7in1/issues)
- **Документация:** [GitHub Pages](https://gfermoto.github.io/soil-sensor-7in1/)

### 📚 Дополнительные ресурсы {#Dopolnitelnye-resursy}
- [Руководство пользователя](USER_GUIDE.md)
- [API документация](API.md)
- [Агрономические рекомендации](AGRO_RECOMMENDATIONS.md)
- [Руководство по компенсации](COMPENSATION_GUIDE.md)

### 🔗 Полезные ссылки {#Poleznye-ssylki}

- [🌱 GitHub репозиторий](https://github.com/Gfermoto/soil-sensor-7in1) - Исходный код проекта
- [📋 План рефакторинга](../dev/REFACTORING_PLAN.md) - Планы развития
- [📊 Отчет о техническом долге](../dev/TECHNICAL_DEBT_REPORT.md) - Анализ технических проблем
- [🏗️ Архитектура системы](../dev/ARCH_OVERALL.md) - Общая архитектура проекта
- **[📖 API документация](API.md)** - REST API и интеграции
- **[🖥️ C++ API (Doxygen)](../DOXYGEN_API.md)** - Документация исходного кода
- **[🔌 Схема подключения](WIRING_DIAGRAM.md)** - Электрические соединения
- **[🌱 Агрономические рекомендации](AGRO_RECOMMENDATIONS.md)** - Применение в сельском хозяйстве
- **[🧪 Тестирование](../TESTING_GUIDE.md)** - Как тестировать систему

---

**© 2025 JXCT Development Team**
*Версия 3.10.0 | Июль 2025*
