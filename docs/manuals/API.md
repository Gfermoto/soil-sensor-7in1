# 📊 API документация JXCT 7-в-1

**Дата:** Июнь 2025
**Версия API:** v1

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

### 📋 Таблица актуальных эндпоинтов (API v1)

| Метод | Путь | Описание |
|-------|------|----------|
| GET   | `/api/v1/sensor`         | Основные данные датчика (JSON) |
| GET   | `/api/v1/system/health`  | Полная диагностика устройства  |
| GET   | `/api/v1/system/status`  | Краткий статус сервисов        |
| POST  | `/api/v1/system/reset`   | Сброс настроек (307 на `/reset`) |
| POST  | `/api/v1/system/reboot`  | Перезагрузка (307 на `/reboot`) |
| GET   | `/api/v1/config/export`  | Скачать конфигурацию (JSON, без паролей) |
| POST  | `/api/v1/config/import`  | Импорт конфигурации            |

### 🕑 Устаревшие/DEPRECATED эндпоинты

| Метод | Путь | Описание |
|-------|------|----------|
| GET   | `/sensor_json`           | Те же данные (legacy, будет удалён в будущих версиях) |
| GET   | `/api/sensor`            | DEPRECATED alias → `/api/v1/sensor` |
| GET   | `/api/config/export`     | DEPRECATED alias → `/api/v1/config/export` |
| POST  | `/api/config/import`     | DEPRECATED alias → `/api/v1/config/import` |
| POST  | `/reset`                 | Legacy сброс (будет удалён) |
| POST  | `/reboot`                | Legacy перезагрузка (будет удалён) |
| GET   | `/health`                | Старый путь диагностики |

---

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
- [📋 План рефакторинга](../dev/REFACTORING_PLAN.md) - Планы развития
- [📊 Отчет о техническом долге](../dev/TECHNICAL_DEBT_REPORT.md) - Анализ технических проблем
- [🏗️ Архитектура системы](../dev/ARCH_OVERALL.md) - Общая архитектура проекта
