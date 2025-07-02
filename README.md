# 🌱 JXCT 7-в-1 Датчик Почвы

[![Version](https://img.shields.io/github/v/tag/Gfermoto/soil-sensor-7in1?color=blue&label=version)](https://github.com/Gfermoto/soil-sensor-7in1/releases)
[![Platform](https://img.shields.io/badge/platform-ESP32-green.svg)](https://www.espressif.com/en/products/socs/esp32)
[![License](https://img.shields.io/github/license/Gfermoto/soil-sensor-7in1?color=yellow&label=license)](LICENSE)
[![Build & Release](https://github.com/Gfermoto/soil-sensor-7in1/actions/workflows/firmware_release.yml/badge.svg)](https://github.com/Gfermoto/soil-sensor-7in1/actions/workflows/firmware_release.yml)
[![Docs](https://github.com/Gfermoto/soil-sensor-7in1/actions/workflows/pages.yml/badge.svg?branch=main)](https://github.com/Gfermoto/soil-sensor-7in1/actions/workflows/pages.yml)
[![Docker](https://img.shields.io/github/v/release/Gfermoto/soil-sensor-7in1?label=docker&color=blue)](https://github.com/Gfermoto/soil-sensor-7in1/packages)

**Профессиональная IoT система мониторинга почвы на базе ESP32 с Modbus RTU, MQTT и продвинутыми алгоритмами компенсации**

## 🚀 Быстрый старт

### Использование Docker-образа

Для быстрой сборки и разработки используйте готовый Docker-образ:

```bash
# Скачать образ
docker pull ghcr.io/gfermoto/soil-sensor-7in1:latest

# Запустить контейнер с вашим кодом
docker run -v $(pwd):/workspace ghcr.io/gfermoto/soil-sensor-7in1:latest pio run

# Или войти в контейнер для разработки
docker run -it -v $(pwd):/workspace ghcr.io/gfermoto/soil-sensor-7in1:latest bash
```

Образ содержит все необходимые инструменты: PlatformIO, Doxygen, MkDocs.

### Локальная разработка

```bash
# Клонирование и сборка
git clone https://github.com/Gfermoto/soil-sensor-7in1.git
cd soil-sensor-7in1
pio run -t upload
```

## 📊 Основные возможности

- **7 параметров:** Температура, влажность, EC, pH, NPK
- **Научная компенсация:** Модель Арчи, уравнение Нернста, FAO 56
- **Веб-интерфейс:** Адаптивный дизайн, OTA обновления
- **IoT интеграция:** MQTT, ThingSpeak, Modbus RTU

## 📚 Документация

**[📖 Полная документация](https://gfermoto.github.io/soil-sensor-7in1/)**

**[🧭 Быстрая навигация](docs/NAVIGATION.md)** - Прямые ссылки на все документы

### 📋 Для пользователей
- [📋 Руководство пользователя](docs/manuals/USER_GUIDE.md)
- [🔧 Техническая документация](docs/manuals/TECHNICAL_DOCS.md)
- [🌱 Агрономические рекомендации](docs/manuals/AGRO_RECOMMENDATIONS.md)
- [📊 API документация](docs/manuals/API.md)
- [🔬 Руководство по компенсации](docs/manuals/COMPENSATION_GUIDE.md)

### 👨‍💻 Для разработчиков
- [🧪 Руководство по тестированию](docs/TESTING_GUIDE.md) - Полная система тестирования
- [📊 Текущие результаты тестирования](docs/CURRENT_TEST_RESULTS.md) - Актуальные метрики качества
- [🔄 Интеграция отчётов](docs/REPORTS_INTEGRATION.md) - Веб-интеграция и API
- [📋 Отчёт о завершении Этапа 2](docs/STAGE_2_COMPLETION_REPORT.md) - Итоговый отчёт
- [📖 Документация разработчика](docs/README.md) - Навигация по docs/
- [🏗️ Архитектура проекта](docs/dev/ARCH_OVERALL.md) - Общая архитектура
- [🔧 План рефакторинга](docs/dev/QA_REFACTORING_PLAN_2025H2.md) - Планы развития
- [⚠️ Отчёт о техническом долге](docs/dev/TECH_DEBT_REPORT_2025-06.md) - Анализ качества кода

## 🏗️ Архитектура

```
JXCT/
├── src/                    # Исходный код
│   ├── main.cpp           # Основной файл
│   ├── config.cpp         # Конфигурация
│   ├── modbus_sensor.cpp  # Работа с датчиком
│   ├── calibration_manager.cpp # Калибровка
│   └── web/               # Веб-интерфейс
├── include/               # Заголовочные файлы
├── docs/                  # Документация
└── platformio.ini         # Конфигурация PlatformIO
```

## 🔧 Технические требования

- **Платформа:** ESP32 (рекомендуется ESP32-WROOM-32)
- **Датчик:** JXCT 7-в-1 датчик почвы
- **IDE:** PlatformIO
- **Протоколы:** Modbus RTU, MQTT, HTTP

## 🤝 Поддержка

- **Telegram:** [@Gfermoto](https://t.me/Gfermoto)
- **Issues:** [GitHub Issues](https://github.com/Gfermoto/soil-sensor-7in1/issues)
- **Документация:** [GitHub Pages](https://gfermoto.github.io/soil-sensor-7in1/)

## 📄 Лицензия

Этот проект распространяется под лицензией **GNU Affero General Public License v3.0 (AGPL-3.0)**. Это означает, что любой, кто распространяет или предоставляет сервис на базе изменённой версии, обязан опубликовать исходный код своих изменений.

### Коммерческая лицензия

Если вы хотите использовать проект в проприетарном продукте без раскрытия исходников, мы предлагаем платную коммерческую лицензию. Напишите нам:

- Email: eyera.team@gmail.com
- Telegram: [@Gfermoto](https://t.me/Gfermoto)

Мы обсудим условия и предоставим индивидуальное лицензионное соглашение.

---

**EYERA Development Team** | Версия 3.6.7 | Июль 2025
