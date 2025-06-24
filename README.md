# 🌱 JXCT 7-в-1 Датчик Почвы

[![Version](https://img.shields.io/badge/version-3.4.9-blue.svg)](https://github.com/Gfermoto/soil-sensor-7in1/releases)
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

- **7 параметров:** Температура, влажность, EC, pH, NPK
- **Научная компенсация:** Модель Арчи, уравнение Нернста, FAO 56
- **Веб-интерфейс:** Адаптивный дизайн, OTA обновления
- **IoT интеграция:** MQTT, ThingSpeak, Modbus RTU

## 📚 Документация

**[📖 Полная документация](https://gfermoto.github.io/soil-sensor-7in1/)**

- [📋 Руководство пользователя](docs/manuals/USER_GUIDE.md)
- [🔧 Техническая документация](docs/manuals/TECHNICAL_DOCS.md)
- [🌱 Агрономические рекомендации](docs/manuals/AGRO_RECOMMENDATIONS.md)
- [📊 API документация](docs/manuals/API.md)
- [🔬 Руководство по компенсации](docs/manuals/COMPENSATION_GUIDE.md)

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

Этот проект распространяется под лицензией MIT. См. файл [LICENSE](LICENSE) для подробностей.

---

**JXCT Development Team** | Версия 3.4.9 | Июнь 2025 