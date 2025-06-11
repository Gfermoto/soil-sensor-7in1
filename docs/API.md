# API Справочник

REST API для интеграции с JXCT Soil Sensor v2.2.0.

## 🔐 Авторизация

**Защищенные endpoints:** `/` (настройки), `POST /save`  
**Открытые endpoints:** `/readings`, `/service`, `/health`, `/api/*`

```bash
# Авторизация (если установлен пароль)
curl -X POST http://192.168.1.100/ -d "auth_password=your_password"
```

## 📊 Основные endpoints

### GET /api/sensor - Данные датчика
```bash
curl http://192.168.1.100/api/sensor
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

### GET /health - Системная информация
```bash
curl http://192.168.1.100/health
```

**Ответ:**
```json
{
  "device": {
    "model": "JXCT-7in1",
    "version": "2.2.0"
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
    "ip": "192.168.1.100",
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
**Авторизация:** требуется (если установлен пароль)

### GET /readings - Мониторинг 
Страница с live данными датчика (обновление каждые 2 сек).  
**Авторизация:** не требуется

### GET /service - Диагностика
Статус WiFi, MQTT, ThingSpeak, датчика, системные метрики.  
**Авторизация:** не требуется

## 📝 Настройки

### POST /save - Сохранение настроек
```bash
curl -X POST http://192.168.1.100/save \
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
- **401** - Требуется авторизация  
- **500** - Внутренняя ошибка сервера

## 📱 CORS поддержка

API поддерживает CORS для локальных сетей:
```javascript
fetch('http://192.168.1.100/api/sensor')
  .then(response => response.json())
  .then(data => console.log(data));
```

## 🔧 Примеры интеграций

### Python
```python
import requests

# Получить данные датчика
response = requests.get('http://192.168.1.100/api/sensor')
data = response.json()
print(f"Температура: {data['temperature']}°C")
```

### Node.js
```javascript
const axios = require('axios');

async function getSensorData() {
  const response = await axios.get('http://192.168.1.100/api/sensor');
  return response.data;
}
```

### Home Assistant
```yaml
# configuration.yaml
sensor:
  - platform: rest
    resource: http://192.168.1.100/api/sensor
    name: "JXCT Soil Sensor"
    json_attributes:
      - temperature
      - humidity  
      - ph
      - ec
    value_template: "{{ value_json.temperature }}"
``` 