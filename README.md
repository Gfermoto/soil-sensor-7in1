# JXCT Soil Sensor (ESP32, PlatformIO)

Проект для работы с многофункциональным почвенным датчиком JXCT 7-in-1 на базе ESP32 с поддержкой Modbus RTU, WiFi, MQTT, ThingSpeak и веб-интерфейса.

## Возможности
- Опрос реального или эмулированного датчика по Modbus RTU
- Веб-интерфейс для настройки WiFi, MQTT, ThingSpeak
- Публикация данных в MQTT и ThingSpeak
- Интеграция с Home Assistant (MQTT Discovery)
- Поддержка сброса настроек кнопкой
- Подробная самодокументация (Doxygen)

## Структура проекта
- `src/` — исходный код (основная логика)
- `include/` — заголовочные файлы
- `scripts/` — вспомогательные скрипты (генерация документации)
- `docs/` — автогенерируемая документация (после запуска Doxygen)
- `platformio.ini` — конфигурация PlatformIO

## Быстрый старт
1. Клонируйте репозиторий:
   ```sh
   git clone https://github.com/Gfermoto/JXCT.git
   cd JXCT
   ```
2. Откройте проект в PlatformIO (VSCode) или выполните:
   ```sh
   pio run -e esp32dev
   ```
3. Залейте прошивку на ESP32:
   ```sh
   pio run -e esp32dev -t upload
   ```
4. Откройте монитор порта:
   ```sh
   pio device monitor
   ```
5. После загрузки настройте устройство через WiFi (точка доступа `JXCT-Sensor-AP`).

## Генерация документации
1. Установите [Doxygen](https://www.doxygen.nl/download.html) и добавьте его в PATH.
2. Запустите скрипт:
   ```powershell
   scripts\gen_docs.ps1
   ```
3. Откройте `docs/html/index.html` в браузере.

## Требования
- ESP32 (любая DevKit)
- PlatformIO (VSCode или CLI)
- Doxygen (для документации)

## Лицензия
MIT

## API

REST API доступен в режиме клиента (STA) по адресу:

- **GET /api/sensor** — получить текущие показания датчика в формате JSON.

Пример запроса:
```sh
curl http://<IP-устройства>/api/sensor
```

Пример успешного ответа:
```json
{
  "temperature": 23.5,
  "humidity": 41.2,
  "ec": 1200,
  "ph": 6.8,
  "nitrogen": 15,
  "phosphorus": 8,
  "potassium": 20,
  "timestamp": 1712345678
}
```

Возможные ошибки:
- 403, {"error":"AP mode"} — если устройство в режиме точки доступа (AP).

## OTA

Функция OTA-обновления прошивки планируется, но пока не реализована. Следите за обновлениями! 