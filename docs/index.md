# 🌱 JXCT 7-в-1 Датчик Почвы

[![Version](https://img.shields.io/badge/version-3.5.0-blue.svg)](https://github.com/Gfermoto/soil-sensor-7in1/releases)
[![Platform](https://img.shields.io/badge/platform-ESP32-green.svg)](https://www.espressif.com/en/products/socs/esp32)
[![License](https://img.shields.io/badge/license-MIT-yellow.svg)](LICENSE)
[![Build Status](https://img.shields.io/badge/build-passing-brightgreen.svg)](https://github.com/Gfermoto/soil-sensor-7in1/actions)

**Профессиональная IoT система мониторинга почвы на базе ESP32 с Modbus RTU, MQTT и продвинутыми алгоритмами компенсации**

## 🚀 Быстрый старт

```bash
# Клонирование и сборка
git clone https://github.com/Gfermoto/soil-sensor-7in1.git
cd soil-sensor-7in1
pio run -t upload
```

## 📊 Основные возможности

- **7 параметров:** Температура, влажность, EC, pH, NPK (азот, фосфор, калий)
- **Научная компенсация:** Модель Арчи, уравнение Нернста, FAO 56
- **Веб-интерфейс:** Адаптивный дизайн, OTA обновления
- **IoT интеграция:** MQTT, ThingSpeak, Modbus RTU

## 📚 Документация

### 📖 Руководства пользователя

- **[📋 Руководство пользователя](manuals/USER_GUIDE.md)** - Полное руководство по установке и использованию
- **[🌱 Агрономические рекомендации](manuals/AGRO_RECOMMENDATIONS.md)** - Рекомендации по различным культурам
- **[🔬 Руководство по компенсации](manuals/COMPENSATION_GUIDE.md)** - Научные модели и калибровка
- **[🔌 Схема подключения](manuals/WIRING_DIAGRAM.md)** - Электрические схемы и подключение
- **[⚙️ Управление конфигурацией](manuals/CONFIG_MANAGEMENT.md)** - Настройка и конфигурирование

### 🔧 Техническая документация

- **[📖 Техническая документация](manuals/TECHNICAL_DOCS.md)** - Архитектура и принципы работы
- **[📊 Протокол Modbus](manuals/MODBUS_PROTOCOL.md)** - Спецификация протокола связи
- **[🔄 Управление версиями](manuals/VERSION_MANAGEMENT.md)** - Система версионирования
- **[📊 API документация](api-overview.md)** - REST API и интеграции

### 🔨 Документация для разработчиков

- **[🏗️ Архитектура системы](dev/ARCH_OVERALL.md)** - Общая архитектура проекта
- **[📋 План рефакторинга](dev/QA_REFACTORING_PLAN_2025H2.md)** - Дорожная карта развития
- **[📊 Отчет о техническом долге](dev/TECH_DEBT_REPORT_2025-06.md)** - Анализ качества кода
- **[🤝 Руководство для участников](CONTRIBUTING_DOCS.md)** - Как внести свой вклад

### 💻 Документация кода (Doxygen)

- **[📖 Doxygen документация](html/index.html)** - Полная документация C++ кода
- **[🏗️ Классы и интерфейсы](html/classes.html)** - Архитектура классов
- **[📁 Исходные файлы](html/files.html)** - Структура проекта

## 🏗️ Архитектура проекта

```
JXCT/
├── src/                    # Исходный код
│   ├── main.cpp           # Основной файл приложения
│   ├── config.cpp         # Система конфигурации
│   ├── modbus_sensor.cpp  # Работа с датчиком по Modbus
│   ├── calibration_manager.cpp # Система калибровки
│   ├── mqtt_client.cpp    # MQTT клиент
│   ├── wifi_manager.cpp   # Управление WiFi
│   └── web/               # Веб-интерфейс
│       ├── routes_data.cpp    # API маршруты данных
│       ├── routes_config.cpp  # API конфигурации
│       └── error_handlers.cpp # Обработка ошибок
├── include/               # Заголовочные файлы
│   ├── ISensor.h          # Интерфейс датчика
│   ├── jxct_config_vars.h # Переменные конфигурации
│   ├── sensor_factory.h   # Фабрика датчиков
│   └── validation_utils.h # Утилиты валидации
├── docs/                  # Документация
│   ├── manuals/           # Руководства пользователя
│   ├── dev/               # Документация разработчика
│   └── examples/          # Примеры конфигураций
├── test/                  # Тесты
└── platformio.ini         # Конфигурация PlatformIO
```

## 🔧 Технические требования

### Аппаратные требования
- **Микроконтроллер:** ESP32 (рекомендуется ESP32-WROOM-32)
- **Датчик:** JXCT 7-в-1 датчик почвы
- **Интерфейс:** RS485 (Modbus RTU)
- **Питание:** 5V/12V (в зависимости от датчика)

### Программные требования
- **IDE:** PlatformIO (рекомендуется) или Arduino IDE
- **Фреймворк:** Arduino Framework для ESP32
- **Протоколы:** Modbus RTU, MQTT, HTTP/HTTPS
- **Библиотеки:** См. `platformio.ini`

## 📈 Измеряемые параметры

| Параметр | Диапазон | Точность | Единицы |
|----------|----------|----------|---------|
| Температура | -40...+80°C | ±0.5°C | °C |
| Влажность почвы | 0...100% | ±3% | % |
| Электропроводность (EC) | 0...5000 | ±5% | μS/cm |
| pH | 3...9 | ±0.3 | pH |
| Азот (N) | 0...2000 | ±10% | мг/кг |
| Фосфор (P) | 0...1000 | ±10% | мг/кг |
| Калий (K) | 0...2000 | ±10% | мг/кг |

## 🔬 Алгоритмы компенсации

Система использует научно обоснованные методы компенсации:

- **Температурная компенсация EC:** Модель Арчи (1942)
- **Температурная компенсация pH:** Уравнение Нернста
- **Компенсация NPK:** Модели FAO 56 + European Journal of Soil Science
- **Калибровка:** CSV таблицы на основе лабораторных измерений

## 🌐 Веб-интерфейс

- Адаптивный дизайн для всех устройств
- Реальное время обновления данных
- Цветовая индикация состояния параметров
- OTA обновления прошивки
- Экспорт данных в CSV
- Настройка параметров калибровки

## 🔗 IoT интеграция

- **MQTT:** Публикация данных в брокер
- **ThingSpeak:** Отправка в облачный сервис
- **REST API:** HTTP endpoints для интеграции
- **Modbus TCP:** Сетевой доступ к данным

## 🤝 Поддержка

- **GitHub Issues:** [Сообщить о проблеме](https://github.com/Gfermoto/soil-sensor-7in1/issues)
- **Telegram:** [@Gfermoto](https://t.me/Gfermoto)
- **Документация:** [GitHub Pages](https://gfermoto.github.io/soil-sensor-7in1/)

## 📄 Лицензия

Этот проект распространяется под лицензией MIT. См. файл [LICENSE](https://github.com/Gfermoto/soil-sensor-7in1/blob/main/LICENSE) для подробностей.

---

**EYERA Development Team** | Версия 3.5.0 | Июнь 2025 