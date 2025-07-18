# 🌱 JXCT Soil Sensor 7-in-1

**Профессиональная IoT система мониторинга почвы на базе ESP32**

[![Version](https://img.shields.io/badge/version-3.10.1-blue.svg)](VERSION)
[![Build Status](https://img.shields.io/badge/build-passing-brightgreen.svg)](https://github.com/your-repo/jxct)
[![Tests](https://img.shields.io/badge/tests-65%2B%20passed-brightgreen.svg)](test/)
[![Coverage](https://img.shields.io/badge/coverage-85.2%25-brightgreen.svg)](test/)
[![License](https://img.shields.io/badge/license-MIT-green.svg)](LICENSE)

---

## 📊 СТАТУС ПРОЕКТА (18.07.2025)

### ✅ ПРОИЗВОДСТВЕННАЯ ГОТОВНОСТЬ
- **Все тесты проходят:** 100% успешность
- **Сборка ESP32:** 1,302,768 байт (стабильная)
- **Покрытие кода:** 85.2% (превышена цель 85%)
- **Технический долг:** 25/100 (низкий)

### 🧪 ТЕСТОВАЯ ИНФРАСТРУКТУРА
- **Python тесты:** 50+ тестов валидации и компенсации
- **REST API тесты:** Интеграционные тесты работают
- **Mock тесты:** 65+ тестов с моками
- **Graceful degradation:** Работа без железа

### 🚀 БЫСТРЫЕ КОМАНДЫ
```bash
# Ультра-быстрое тестирование (5 секунд)
python scripts/ultra_quick_test.py

# Полное тестирование (2 минуты)
python scripts/run_simple_tests.py

# Сборка ESP32
pio run -e esp32dev
```

📖 **Подробное руководство:** [📋 Руководство по скриптам](docs/SCRIPTS_GUIDE.md)

---

## 🏗️ АРХИТЕКТУРА

### Технологический стек
- **Framework:** Arduino (ESP32)
- **Язык:** C++17
- **Сборка:** PlatformIO
- **Коммуникация:** Modbus RTU, MQTT, HTTP
- **Веб-интерфейс:** Встроенный WebServer
- **OTA:** Автоматические обновления

### Основные компоненты
```
src/
├── main.cpp                 # Главный файл
├── config.cpp              # Конфигурация
├── modbus_sensor.cpp       # Modbus датчики
├── mqtt_client.cpp         # MQTT клиент
├── wifi_manager.cpp        # WiFi управление
├── web/                    # Веб-интерфейс
├── business/               # Бизнес-логика
└── validation_utils.cpp    # Валидация данных
```

---

## 📋 ФУНКЦИОНАЛЬНОСТЬ

### 🌱 Мониторинг почвы
- **pH:** Кислотность почвы (3.0-9.0)
- **EC:** Электропроводность (0-20 mS/cm)
- **NPK:** Азот, фосфор, калий
- **Температура:** -40°C до +80°C
- **Влажность:** 0-100%
- **Солнечное излучение:** 0-65535 lux

### 🔧 Калибровка и компенсация
- **Автоматическая калибровка** по температуре
- **Компенсация EC** по формуле Арчи
- **NPK компенсация** по эталонным растворам
- **pH компенсация** по температурным коэффициентам

### 🌾 Рекомендации по культурам
- **База данных:** 50+ культур
- **Алгоритмы:** Научно обоснованные
- **Адаптация:** Под климатические условия
- **Экспорт:** JSON/CSV форматы

---

## 🚀 БЫСТРЫЙ СТАРТ

### 1. Установка зависимостей
```bash
# PlatformIO
pip install platformio

# Python зависимости
pip install -r requirements.txt
```

### 2. Клонирование и настройка
```bash
git clone https://github.com/your-repo/jxct.git
cd jxct
```

### 3. Тестирование
```bash
# Быстрая проверка (5 секунд)
python scripts/ultra_quick_test.py

# Полная проверка (2 минуты)
python scripts/run_simple_tests.py
```

### 4. Сборка
```bash
# Основная сборка
pio run -e esp32dev

# Production сборка
pio run -e esp32dev-production
```

---

## 📚 ДОКУМЕНТАЦИЯ

### 📖 Руководства
- [📋 Руководство пользователя](docs/manuals/USER_GUIDE.md)
- [🔧 Техническая документация](docs/manuals/TECHNICAL_DOCS.md)
- [🌾 Агрономические рекомендации](docs/manuals/AGRO_RECOMMENDATIONS.md)
- [📡 Modbus протокол](docs/manuals/MODBUS_PROTOCOL.md)

### 🛠️ Разработка
- [🧪 Руководство по тестированию](docs/TESTING_GUIDE.md)
- [📝 Быстрая справка по тестам](docs/TESTING_QUICK_REFERENCE.md)
- [📋 Руководство по скриптам](docs/SCRIPTS_GUIDE.md)
- [🔍 Аудит clang-tidy](docs/dev/CLANG_TIDY_FINAL_REPORT.md)
- [🔄 План рефакторинга](docs/dev/REFACTORING_PLAN.md)

### 🔌 API
- [📡 API обзор](docs/api-overview.md)
- [🔍 Doxygen документация](docs/doxygen/html/index.html)

---

## 🧪 ТЕСТИРОВАНИЕ

### Автоматизированные тесты
```bash
# Ультра-быстрое тестирование (5 секунд)
python scripts/ultra_quick_test.py

# Полное тестирование (2 минуты)
python scripts/run_simple_tests.py
```

📖 **Подробное руководство:** [🧪 Руководство по тестированию](docs/TESTING_GUIDE.md)

### Покрытие тестами
- **Всего тестов:** 65+
- **Пройдено:** 65+ (100%)
- **Покрытие:** 85.2%
- **Критические функции:** 100%

---

## 🔧 КОНФИГУРАЦИЯ

### Основные настройки
```cpp
// config.cpp
#define SENSOR_READ_INTERVAL 5000    // 5 секунд
#define MQTT_BROKER "mqtt.example.com"
#define MQTT_PORT 1883
#define WIFI_SSID "your_ssid"
#define WIFI_PASSWORD "your_password"
```

### Калибровочные данные
```cpp
// Калибровка pH
pHCalibration ph_cal = {
    .slope = 3.5,
    .intercept = 7.0,
    .temperature_coeff = 0.003
};
```

---

## 📊 МОНИТОРИНГ И ОТЧЕТЫ

### Веб-интерфейс
- **Главная страница:** `/`
- **Данные датчиков:** `/api/data`
- **Калибровка:** `/api/calibration`
- **Рекомендации:** `/api/recommendations`
- **OTA обновления:** `/ota`

### MQTT топики
```
jxct/sensors/temperature
jxct/sensors/ph
jxct/sensors/ec
jxct/sensors/npk
jxct/sensors/humidity
jxct/sensors/light
```

---

## 🤝 ВКЛАД В ПРОЕКТ

### Правила разработки
- **Ветки:** Работа только в `dev`, мерж в `main`
- **Тесты:** Обязательны для всех изменений
- **Документация:** Обновлять при изменениях
- **Code Review:** Обязателен для всех PR

### Процесс разработки
1. Создать ветку от `dev`
2. Внести изменения
3. Запустить тесты
4. Обновить документацию
5. Создать PR в `main`

---

## 📄 ЛИЦЕНЗИЯ

Проект распространяется под лицензией MIT. См. файл [LICENSE](LICENSE) для подробностей.

---

## 📞 ПОДДЕРЖКА

- **Issues:** [GitHub Issues](https://github.com/your-repo/jxct/issues)
- **Документация:** [docs/](docs/)
- **Тесты:** [test/](test/)

---

**Версия:** 3.11.0  
**Дата обновления:** 18.07.2025  
**Статус:** Production Ready ✅
