# JXCT Soil Sensor v3.13.2

IoT система мониторинга почвы на базе ESP32.

[![Version](https://img.shields.io/github/v/tag/Gfermoto/soil-sensor-7in1?color=blue&label=version&style=for-the-badge)](https://github.com/Gfermoto/soil-sensor-7in1/releases)
[![Platform](https://img.shields.io/badge/platform-ESP32-green.svg?style=for-the-badge)](https://www.espressif.com/en/products/socs/esp32)
[![License](https://img.shields.io/github/license/Gfermoto/soil-sensor-7in1?color=yellow&label=license&style=for-the-badge)](LICENSE)
[![CI](https://img.shields.io/github/actions/workflow/status/Gfermoto/soil-sensor-7in1/ci.yml?branch=main&style=for-the-badge)](https://github.com/Gfermoto/soil-sensor-7in1/actions/workflows/ci.yml)
[![Docs](https://img.shields.io/badge/docs-site-blue.svg?style=for-the-badge)](https://gfermoto.github.io/soil-sensor-7in1/)

7 параметров почвы (влажность, температура, pH, EC, NPK), веб-интерфейс, OTA, MQTT. Модель Арчи для EC, уравнение Нернста для pH.

## Быстрый старт

```bash
git clone https://github.com/Gfermoto/soil-sensor-7in1.git
cd soil-sensor-7in1
pip install -r requirements.txt
pio run -t upload
pio run -t uploadfs
```

## Тесты

```bash
pio test -e native
python scripts/run_e2e_tests.py
```

## Документация

- [Руководство пользователя](docs/manuals/USER_GUIDE.md)
- [Техническая документация](docs/manuals/TECHNICAL_DOCS.md)
- [Схема подключения](docs/manuals/WIRING_DIAGRAM.md)
- [API](docs/manuals/API.md)
- [Сайт](https://gfermoto.github.io/soil-sensor-7in1/)

## Структура

```
src/         # Код прошивки
include/     # Заголовки
test/        # Тесты
scripts/     # Автоматизация
docs/        # Документация
```

## Troubleshooting

| Проблема | Решение |
|----------|---------|
| Web не виден | Проверить WiFi, IP |
| OTA не работает | .bin из релиза |
| Тесты падают | requirements.txt, PlatformIO |
| CSRF ошибка | Перезагрузить страницу |
| MQTT не подключается | Настройки MQTT |
| Неточные показания | Калибровка через web UI |

## FAQ

- **Обновление прошивки:** OTA через web или `pio run -t upload`
- **Home Assistant:** MQTT в web UI, добавить устройство
- **Компенсация:** [COMPENSATION_GUIDE](docs/manuals/COMPENSATION_GUIDE.md)

## Вклад

[CONTRIBUTING](CONTRIBUTING.md). PR: тесты проходят, покрытие не падает, код отформатирован.

## Лицензия

[LICENSE](LICENSE)
