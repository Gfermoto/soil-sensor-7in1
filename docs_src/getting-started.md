---
title: Быстрый старт
---

## Требования

| Компонент | Версия / минимум |
|-----------|------------------|
| Плата | ESP32 DevKitC / WROOM-32 |
| Датчик | JXCT Soil Sensor 7-in-1 (Modbus RTU) |
| Компилятор | PlatformIO >= 6.1 / Arduino CLI >= 0.34 |
| Python | 3.10+ (для скриптов) |

## Сборка прошивки (PlatformIO)

```bash
# 1. Клонируем репозиторий
$ git clone https://github.com/Gfermoto/soil-sensor-7in1.git && cd soil-sensor-7in1

# 2. Устанавливаем зависимости (один раз)
$ pio pkg install

# 3. Компилируем и прошиваем
$ pio run -e esp32dev -t upload

# 4. Открываем сериал-порт
$ pio device monitor -b 115200
```

> Советы:
> * `-e nodisplay` — сборка без TFT-экрана.
> * `pio run -e test_native` — запускает unit-тесты на хосте.

## Подключение датчика

1. **RS-485 ↔ UART** через MAX3485.
2. A/B линии шины к датчику (A — жёлтый, B — синий).
3. Питание 12 В DC; «GND RS-485» объединён с GND контроллера.
4. В `src/modbus_sensor_adapter.h` настроить `MODBUS_SERIAL_PORT`, RX/TX пины и скорость (по-умолчанию 9600 8N1).

## Веб-настройка Wi-Fi

После первой загрузки создаётся точка доступа `JXCT-XXXX`:  
`http://192.168.4.1` → *Wi-Fi Manager* → выбираем сеть и вводим пароль.  
Контроллер перезагрузится и напечатает в Serial фактический IP.

## OTA-обновления

```text
http://<IP>/ota   (POST .bin)
```

* Поддерживаются дифф-прошивки и SHA-256 проверка.
* Подробности — раздел *Firmware → OTA*.

## Home Assistant

1. Включите опцию **MQTT discovery** в UI.
2. На устройстве активируйте `flags -> hass_enabled = true`.
3. Сенсоры появятся автоматически: влажность, температура, pH, EC, N, P, K.

## Полезные ссылки

* [Конфигурация](/firmware/config/)  
  Экспорт/импорт JSON, массовое развёртывание.
* [REST API](/api/)  
  Полная спецификация эндпоинтов (OpenAPI 3.1).
* [C++ API](_doxygen/html/index.html)  
  Doxygen-документация прошивки. 