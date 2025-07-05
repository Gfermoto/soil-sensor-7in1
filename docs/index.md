# 🌱 JXCT Soil Sensor - Документация

**Профессиональная IoT система мониторинга почвы на базе ESP32**

[![Version](https://img.shields.io/github/v/tag/Gfermoto/soil-sensor-7in1?color=blue&label=version)](https://github.com/Gfermoto/soil-sensor-7in1/releases)
[![Platform](https://img.shields.io/badge/platform-ESP32-green.svg)](https://www.espressif.com/en/products/socs/esp32)
[![License](https://img.shields.io/github/license/Gfermoto/soil-sensor-7in1?color=yellow&label=license)](https://github.com/Gfermoto/soil-sensor-7in1/blob/main/LICENSE)

---

## 🚀 Быстрый старт {#Bystryy-start}

### Для пользователей {#Dlya-polzovateley}
- **[👤 Руководство пользователя](manuals/USER_GUIDE.md)** - Установка и настройка
- **[🔌 Схема подключения](manuals/WIRING_DIAGRAM.md)** - Электрические соединения
- **[🌱 Агрономические рекомендации](manuals/AGRO_RECOMMENDATIONS.md)** - Применение в сельском хозяйстве

### Для разработчиков {#Dlya-razrabotchikov}
- **[🔧 Техническая документация](manuals/TECHNICAL_DOCS.md)** - Архитектура и компоненты
- **[📖 API документация](manuals/API.md)** - REST API и интеграции
- **[🚀 Руководство по скриптам](SCRIPTS_GUIDE.md)** - Автоматизация разработки

---

## 📊 Основные возможности {#Osnovnye-vozmozhnosti}

### 🌡️ Измерения почвы {#Izmereniya-pochvy}
- **Температура**: -40°C до +80°C (±0.5°C)
- **Влажность**: 0-100% VWC (±3%)
- **Электропроводность**: 0-23 mS/cm (±2%)
- **pH почвы**: 3-9 pH (±0.3)
- **NPK удобрения**: 0-1999 mg/kg (±2%)

### 🔗 IoT интеграции {#iot-integratsii}
- **MQTT** - Публикация данных в реальном времени
- **ThingSpeak** - Облачное хранение и аналитика
- **Modbus RTU** - Промышленный протокол
- **REST API** - Веб-интерфейс и интеграции
- **WiFi** - Беспроводное подключение

### 🛡️ Безопасность {#Bezopasnost}
- **CSRF защита** - Защита от межсайтовых запросов
- **Валидация данных** - Проверка входных параметров
- **OTA обновления** - Безопасные обновления прошивки

---

## 🏗️ Архитектура системы {#Arhitektura-sistemy}

```
📦 JXCT Soil Sensor
├── 🌐 ESP32 - Основной контроллер
├── 🔌 RS485 - Подключение к датчику
├── 📡 WiFi - Сетевое подключение
├── 💾 SPIFFS - Локальное хранение
└── 🔄 OTA - Обновления по воздуху
```

---

## 🛠️ Разработка {#Razrabotka}

### Технологический стек {#Tehnologicheskiy-stek}
- **Платформа**: ESP32 (Arduino Framework)
- **Язык**: C++17
- **Сборка**: PlatformIO
- **Документация**: MkDocs Material
- **Тестирование**: Unity Framework

### Структура проекта {#Struktura-proekta}
```
📦 JXCT
├── 📁 src/           # Исходный код
├── 📁 include/       # Заголовочные файлы
├── 📁 test/          # Тесты
├── 📁 scripts/       # Автоматизация
├── 📁 docs/          # Документация
└── 📁 site/          # Собранная документация
```

---

## 📚 Документация {#Dokumentatsiya}

### 📖 Руководства {#Rukovodstva}
- **[👤 Руководство пользователя](manuals/USER_GUIDE.md)** - Установка и настройка
- **[🔧 Техническая документация](manuals/TECHNICAL_DOCS.md)** - Архитектура и компоненты
- **[🔌 Схема подключения](manuals/WIRING_DIAGRAM.md)** - Электрические соединения
- **[🌱 Агрономические рекомендации](manuals/AGRO_RECOMMENDATIONS.md)** - Применение в сельском хозяйстве
- **[📡 Протокол Modbus](manuals/MODBUS_PROTOCOL.md)** - Спецификация протокола
- **[⚙️ Управление конфигурацией](manuals/CONFIG_MANAGEMENT.md)** - Настройка системы
- **[⚖️ Компенсация датчиков](manuals/COMPENSATION_GUIDE.md)** - Калибровка и компенсация
- **[🏷️ Управление версиями](manuals/VERSION_MANAGEMENT.md)** - Система версионирования

### 🔌 API {#api}
- **[📋 Обзор API](api-overview.md)** - Краткий обзор эндпоинтов
- **[📖 API документация](manuals/API.md)** - Полное описание API
- **[🖥️ C++ API (Doxygen)](DOXYGEN_API.md)** - Документация исходного кода

### 🧪 Тестирование {#Testirovanie}
- **[📖 Руководство по тестированию](TESTING_GUIDE.md)** - Полное руководство
- **[⚡ Быстрая справка](TESTING_QUICK_REFERENCE.md)** - Команды и примеры

### 🛠️ Разработка {#Razrabotka}
- **[🏗️ Архитектура](dev/ARCH_OVERALL.md)** - Общая архитектура
- **[🧪 QA аудит](dev/QA_AUDIT_REPORT.md)** - Анализ качества
- **[🔄 План рефакторинга](dev/REFACTORING_PLAN.md)** - Улучшения кода
- **[📉 Технический долг](dev/TECHNICAL_DEBT_REPORT.md)** - Анализ долга
- **[🚀 Руководство по скриптам](SCRIPTS_GUIDE.md)** - Автоматизация разработки

---

## 🆘 Поддержка {#Podderzhka}

- **💬 Telegram**: [@Gfermoto](https://t.me/Gfermoto)
- **🐛 Issues**: [GitHub Issues](https://github.com/Gfermoto/soil-sensor-7in1/issues)
- **📖 Документация**: [GitHub Pages](https://gfermoto.github.io/soil-sensor-7in1/)

---

## 📄 Лицензия {#Litsenziya}

Этот проект распространяется под лицензией **GNU Affero General Public License v3.0 (AGPL-3.0)**.

---

**EYERA Development Team** | Версия 3.6.9 | 2025
