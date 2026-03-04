# JXCT Soil Sensor v3.13.2

IoT система мониторинга почвы на базе ESP32.

**Статья на Habr:** [IoT-датчик почвы 7-в-1 на ESP32: научная компенсация, 24 культуры и open-source прошивка](https://github.com/Gfermoto/soil-sensor-7in1/blob/main/habr_article/article.md) — текст для публикации.

[![Version](https://img.shields.io/github/v/tag/Gfermoto/soil-sensor-7in1?color=blue&label=version)](https://github.com/Gfermoto/soil-sensor-7in1/releases)
[![Platform](https://img.shields.io/badge/platform-ESP32-green.svg)](https://www.espressif.com/en/products/socs/esp32)
[![License](https://img.shields.io/github/license/Gfermoto/soil-sensor-7in1?color=yellow&label=license)](https://github.com/Gfermoto/soil-sensor-7in1/blob/main/LICENSE)

## Быстрый старт

- [Руководство пользователя](manuals/USER_GUIDE.md)
- [Схема подключения](manuals/WIRING_DIAGRAM.md)
- [Агрономические рекомендации](manuals/AGRO_RECOMMENDATIONS.md)
- [Техническая документация](manuals/TECHNICAL_DOCS.md)
- [API](manuals/API.md)
- [Скрипты](SCRIPTS_GUIDE.md)

## Возможности

- Измерения: температура, влажность, EC, pH, NPK
- IoT: MQTT, ThingSpeak, Modbus RTU, REST API
- Безопасность: CSRF, валидация, OTA

## Стек

ESP32, C++17, PlatformIO, MkDocs, Unity/pytest.

## Документация

### Руководства
- [Руководство пользователя](manuals/USER_GUIDE.md)
- [Техническая документация](manuals/TECHNICAL_DOCS.md)
- [Схема подключения](manuals/WIRING_DIAGRAM.md)
- [Агрономические рекомендации](manuals/AGRO_RECOMMENDATIONS.md)
- [Протокол Modbus](manuals/MODBUS_PROTOCOL.md)
- [Управление конфигурацией](manuals/CONFIG_MANAGEMENT.md)
- [Компенсация датчиков](manuals/COMPENSATION_GUIDE.md)
- [Управление версиями](manuals/VERSION_MANAGEMENT.md)

### Разработка
- [Архитектура](dev/ARCH_OVERALL.md)
- [Технический долг](dev/TECHNICAL_DEBT_REPORT.md)
- [План рефакторинга](dev/REFACTORING_PLAN.md)
- [Тестирование](TESTING_GUIDE.md)
- [Скрипты](SCRIPTS_GUIDE.md)

## Поддержка

- [GitHub Issues](https://github.com/Gfermoto/soil-sensor-7in1/issues)
- [Telegram](https://t.me/Gfermoto)
- [Сайт](https://gfermoto.github.io/soil-sensor-7in1/)

## Лицензия

AGPL-3.0
