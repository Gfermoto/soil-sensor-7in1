# 📊 API документация JXCT 7-в-1

**Дата:** Июнь 2025
**Версия:** 3.6.7

REST API для интеграции с JXCT Soil Sensor v2.2.0.

---

## 📖 Содержание

1. [🌐 Доступ к API](#-доступ-к-api)
2. [📊 Основные endpoints](#-основные-endpoints)
3. [🌐 Веб-страницы](#-веб-страницы)
4. [📝 Настройки](#-настройки)
5. [🏠 MQTT интеграция](#-mqtt-интеграция)
6. [📡 ThingSpeak интеграция](#-thingspeak-интеграция)
7. [🔄 Коды ошибок](#-коды-ошибок)
8. [📱 CORS поддержка](#-cors-поддержка)

---

## 🌐 Доступ к API

**Все endpoints открыты** - авторизация не требуется.
**Доступные endpoints:**

| Метод | Путь | Описание |
|-------|------|----------|
| GET | `/api/v1/sensor` | Основные данные датчика (JSON). |
| GET | `/sensor_json` | Те же данные (legacy, будет удалён в v2.7.0). |
| GET | `/api/sensor` | DEPRECATED alias → `/api/v1/sensor`. |
| GET | `/api/v1/system/health` | Полная диагностика устройства. |
| GET | `/api/v1/system/status` | Краткий статус сервисов. |
| POST | `/api/v1/system/reset` | Сброс настроек (307 на `/reset`). |
| POST | `/api/v1/system/reboot` | Перезагрузка (307 на `/reboot`). |
| GET | `/api/v1/config/export` | Скачать конфигурацию (JSON, без паролей). |
| GET | `/api/config/export` | DEPRECATED alias → `/api/v1/config/export`. |
| POST | `/api/config/import` | Импорт конфигурации. |
| GET | `/readings` | Веб-страница с показаниями датчика. |
| GET | `/service` | Веб-страница диагностики сервисов. |

Другие страницы UI: `/`, `/intervals`, `/config_manager`.

## 📊 Основные endpoints

### GET /api/sensor - Данные датчика
```bash
curl http://192.168.4.1/api/sensor
```

**Ответ:**
```json
{
  "temperature": 23.7,
  "humidity": 54.4,
  "ec": 1200,
  "ph": 6.8,
  "nitrogen": 15,
  "phosphorus": 8,
  "potassium": 20,
  "timestamp": 1717920000,
  "valid": true,
  "sensor_enabled": true
}
```

### GET /sensor_json – Данные датчика (frontend)

Возвращает идентичный JSON, используется JavaScript на странице `/readings`. Для внешней интеграции рекомендуется `/api/sensor`.

### GET /service_status – Состояние сервисов

Пример ответа:
```json
{
  "wifi_connected": true,
  "mqtt_enabled": true,
  "mqtt_connected": true,
  "mqtt_last_error": "",
  "thingspeak_enabled": true,
  "thingspeak_last_pub": "2025-06-11T15:30:00Z",
  "thingspeak_last_error": "",
  "hass_enabled": false,
  "sensor_ok": true
}
```

### GET /api/config/export – Экспорт настроек

Скачивает файл `jxct_config_TIMESTAMP.json` со всеми настройками (чувствительные данные подменены «YOUR_…»).

### POST /api/config/import – Импорт настроек
Загрузите JSON, полученный ранее экспортом, чтобы восстановить конфигурацию.

### POST /reset – Legacy сброс (будет удалён в v2.7.0).

### POST /reboot – Legacy перезагрузка.

### GET /health - Системная информация
```bash
curl http://192.168.4.1/health
```

**Ответ:**
```json
{
  "device": {
    "model": "JXCT-7in1",
    "version": "3.6.7"
  },
  "memory": {
    "free_heap": 228732,
    "flash_used": 876701,
    "flash_total": 4194304
  },
  "wifi": {
    "connected": true,
    "mode": "STA",
    "ssid": "MyWiFi",
    "ip": "192.168.4.1",
    "rssi": -63
  },
  "services": {
    "mqtt": {
      "enabled": true,
      "connected": true,
      "server": "mqtt.local"
    },
    "thingspeak": {
      "enabled": true,
      "last_publish": "2025-06-11T15:30:00Z"
    }
  },
  "uptime": 86400,
  "timestamp": "2025-06-11T15:30:15Z"
}
```

## 🌐 Веб-страницы

### GET / - Настройки
Веб-интерфейс для настройки WiFi, MQTT, ThingSpeak.

### GET /readings - Мониторинг
Страница с live данными датчика (обновление каждые 2 сек).

### GET /service - Диагностика
Статус WiFi, MQTT, ThingSpeak, датчика, системные метрики.

## 📝 Настройки

### POST /save - Сохранение настроек
```bash
curl -X POST http://192.168.4.1/save \
  -d "wifi_ssid=MyWiFi" \
  -d "wifi_password=mypass" \
  -d "mqtt_server=mqtt.local" \
  -d "mqtt_port=1883" \
  -d "thingspeak_api_key=YOUR_KEY"
```

**Параметры:**
- `wifi_ssid`, `wifi_password` - WiFi настройки
- `mqtt_server`, `mqtt_port`, `mqtt_user`, `mqtt_password` - MQTT
- `thingspeak_api_key` - ThingSpeak API ключ
- `homeassistant_discovery` - включить HA Discovery (1/0)
- `web_password` - пароль для веб-интерфейса

## 🏠 MQTT интеграция

### Топики публикации
```
homeassistant/sensor/jxct_soil/temperature/state
homeassistant/sensor/jxct_soil/humidity/state
homeassistant/sensor/jxct_soil/ec/state
homeassistant/sensor/jxct_soil/ph/state
homeassistant/sensor/jxct_soil/nitrogen/state
homeassistant/sensor/jxct_soil/phosphorus/state
homeassistant/sensor/jxct_soil/potassium/state
```

### Команды управления
```bash
# Перезагрузка устройства
mosquitto_pub -h mqtt.local -t "jxct/command" -m "reboot"

# Сброс настроек
mosquitto_pub -h mqtt.local -t "jxct/command" -m "reset"

# Тестовая публикация
mosquitto_pub -h mqtt.local -t "jxct/command" -m "publish_test"
```

## 📡 ThingSpeak интеграция

Автоматическая отправка данных каждые 15 секунд в поля:
- Field1: Температура (°C)
- Field2: Влажность (%)
- Field3: EC (µS/cm)
- Field4: pH
- Field5: Азот (mg/kg)
- Field6: Фосфор (mg/kg)
- Field7: Калий (mg/kg)

## 🔄 Коды ошибок

- **200** - Успешно
- **400** - Некорректные параметры
- **403** - Доступ запрещен
- **500** - Внутренняя ошибка сервера

## 📱 CORS поддержка

API поддерживает CORS для локальных сетей:
```javascript
fetch('http://192.168.4.1/api/sensor')
  .then(response => response.json())
  .then(data => console.log(data));
```

## 🔧 Примеры интеграций

### Python
```python
import requests

# Получить данные датчика
response = requests.get('http://192.168.4.1/api/sensor')
data = response.json()
print(f"Температура: {data['temperature']}°C")
```

### Node.js
```javascript
const axios = require('axios');

async function getSensorData() {
  const response = await axios.get('http://192.168.4.1/api/sensor');
  return response.data;
}
```

### Home Assistant
```yaml
# configuration.yaml
sensor:
  - platform: rest
    resource: http://192.168.4.1/api/sensor
    name: "JXCT Soil Sensor"
    json_attributes:
      - temperature
      - humidity
      - ph
      - ec
    value_template: "{{ value_json.temperature }}"
```

## 📞 Поддержка

### 💬 Связь с разработчиками
- **Telegram:** [@Gfermoto](https://t.me/Gfermoto)
- **GitHub Issues:** [Сообщить о проблеме](https://github.com/Gfermoto/soil-sensor-7in1/issues)
- **Документация:** [GitHub Pages](https://gfermoto.github.io/soil-sensor-7in1/)

### 📚 Дополнительные ресурсы
- [Руководство пользователя](USER_GUIDE.md)
- [Техническая документация](TECHNICAL_DOCS.md)
- [Агрономические рекомендации](AGRO_RECOMMENDATIONS.md)
- [Руководство по компенсации](COMPENSATION_GUIDE.md)
- [Управление конфигурацией](CONFIG_MANAGEMENT.md)
- [Схема подключения](WIRING_DIAGRAM.md)
- [Протокол Modbus](MODBUS_PROTOCOL.md)
- [Управление версиями](VERSION_MANAGEMENT.md)

### 🔗 Полезные ссылки

- [🌱 GitHub репозиторий](https://github.com/Gfermoto/soil-sensor-7in1) - Исходный код проекта
- [📋 План рефакторинга](../dev/QA_REFACTORING_PLAN_2025H2.md) - Планы развития на 2025
- [📊 Отчет о техническом долге](../dev/TECH_DEBT_REPORT_2025-06.md) - Анализ технических проблем
- [🏗️ Архитектура системы](../dev/ARCH_OVERALL.md) - Общая архитектура проекта
