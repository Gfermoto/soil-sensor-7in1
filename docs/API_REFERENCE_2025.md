# 🌐 API СПРАВОЧНИК: JXCT Soil Sensor v2.2.0

**Дата создания:** 11 июня 2025  
**Версия API:** 1.0  
**Статус:** Production Ready ✅  
**Протокол:** HTTP/1.1

---

## 📊 ОБЗОР API

JXCT Soil Sensor предоставляет REST API для интеграции с внешними системами, получения данных датчика, мониторинга состояния системы и управления настройками.

### **Базовые характеристики:**
- **Протокол:** HTTP (порт 80)
- **Авторизация:** Простая парольная защита (опционально)
- **Формат данных:** JSON + HTML
- **Кодировка:** UTF-8
- **CORS:** Поддерживается для локальных сетей

### **Доступность API:**
- ✅ **STA Mode** (клиент WiFi) - полный доступ ко всем endpoints
- ⚠️ **AP Mode** (точка доступа) - только настройки и базовая диагностика

---

## 🔐 АВТОРИЗАЦИЯ

### **Механизм авторизации:**
- **Тип:** Form-based authentication
- **Область действия:** Только страницы настроек и их API
- **Bypass:** Страницы мониторинга доступны без авторизации

### **Защищенные endpoints:**
- `GET /` - главная страница настроек
- `POST /save` - сохранение настроек

### **Открытые endpoints:**
- `GET /readings` - показания датчика
- `GET /service` - диагностика
- `GET /health` - системная информация
- `GET /api/sensor` - JSON API датчика

### **Пример авторизации:**
```bash
# Неавторизованный запрос вернет форму авторизации
curl http://192.168.1.100/

# Авторизация через POST
curl -X POST http://192.168.1.100/ \
  -d "auth_password=your_password"
```

---

## 📋 ОСНОВНЫЕ ENDPOINTS

### **GET / - Главная страница настроек**
Возвращает веб-интерфейс для настройки системы.

**URL:** `http://device_ip/`  
**Метод:** GET, POST  
**Авторизация:** Требуется (если установлен пароль)  
**Формат ответа:** HTML

**Параметры GET:**
- `auth_password` (опционально) - пароль для авторизации

**Ответ:**
- **200 OK** - HTML страница настроек
- **200 OK** - HTML форма авторизации (если пароль неверный)

---

### **GET /readings - Страница показаний датчика**
Возвращает страницу с live данными датчика.

**URL:** `http://device_ip/readings`  
**Метод:** GET  
**Авторизация:** Не требуется  
**Формат ответа:** HTML с JavaScript для real-time обновлений

**Особенности:**
- Auto-refresh каждые 2 секунды
- Адаптивный дизайн для мобильных устройств
- Toast уведомления об ошибках

**Ответ:**
```html
<!-- HTML страница с данными датчика -->
<!-- JavaScript для AJAX обновлений -->
```

---

### **GET /service - Диагностическая страница**
Возвращает подробную информацию о состоянии системы.

**URL:** `http://device_ip/service`  
**Метод:** GET  
**Авторизация:** Не требуется  
**Формат ответа:** HTML

**Информация включает:**
- Статус WiFi подключения
- Состояние MQTT клиента
- Статус ThingSpeak интеграции
- Информация о датчике
- Системные метрики

---

## 🔌 JSON API ENDPOINTS

### **GET /health - Системная информация**
Возвращает подробную системную информацию в JSON формате.

**URL:** `http://device_ip/health`  
**Метод:** GET  
**Авторизация:** Не требуется  
**Формат ответа:** JSON

**Пример ответа:**
```json
{
  "device": {
    "manufacturer": "Eyera",
    "model": "JXCT-7in1",  
    "version": "2.2.0",
    "build_date": "2025-06-10"
  },
  "memory": {
    "free_heap": 228732,
    "largest_block": 118772,
    "heap_size": 327680,
    "flash_used": 876701,
    "flash_total": 4194304
  },
  "wifi": {
    "connected": true,
    "mode": "STA",
    "ssid": "MyWiFi",
    "ip": "192.168.1.100", 
    "rssi": -63,
    "channel": 6
  },
  "services": {
    "mqtt": {
      "enabled": true,
      "connected": true,
      "server": "mqtt.local",
      "port": 1883,
      "last_error": ""
    },
    "thingspeak": {
      "enabled": true,
      "last_publish": "2025-06-11T15:30:00Z",
      "last_error": ""
    },
    "homeassistant": {
      "enabled": true
    }
  },
  "sensor": {
    "enabled": true,
    "valid": true,
    "last_read": "2025-06-11T15:30:15Z",
    "last_error": ""
  },
  "uptime": 86400,
  "timestamp": "2025-06-11T15:30:15Z",
  "boot_time": "2025-06-10T15:30:15Z"
}
```

---

### **GET /api/sensor - Данные датчика**
Возвращает текущие показания датчика.

**URL:** `http://device_ip/api/sensor`  
**Метод:** GET  
**Авторизация:** Не требуется  
**Формат ответа:** JSON

**Пример успешного ответа:**
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
  "last_update": 1717920000,
  "sensor_enabled": true
}
```

**Пример ошибки:**
```json
{
  "error": "Sensor read timeout",
  "valid": false,
  "last_update": 1717919900,
  "sensor_enabled": true
}
```

**Коды ответов:**
- **200 OK** - данные получены успешно
- **500 Internal Server Error** - ошибка чтения датчика
- **503 Service Unavailable** - датчик отключен

---

### **GET /sensor_json - AJAX endpoint**
Lightweight endpoint для AJAX обновлений веб-интерфейса.

**URL:** `http://device_ip/sensor_json`  
**Метод:** GET  
**Авторизация:** Не требуется  
**Формат ответа:** JSON (компактный)

**Особенности:**
- Оптимизирован для частых запросов
- Минимальный размер ответа
- Кэширование данных

**Пример ответа:**
```json
{
  "t": 23.7,
  "h": 54.4, 
  "e": 1200,
  "p": 6.8,
  "n": 15,
  "ph": 8,
  "k": 20,
  "v": true,
  "ts": 1717920000
}
```

---

### **GET /service_status - Статус сервисов**
Возвращает состояние всех сервисов для AJAX обновлений.

**URL:** `http://device_ip/service_status`  
**Метод:** GET  
**Авторизация:** Не требуется  
**Формат ответа:** JSON

**Пример ответа:**
```json
{
  "wifi_connected": true,
  "wifi_ip": "192.168.1.100",
  "wifi_ssid": "MyWiFi",
  "wifi_rssi": -63,
  "mqtt_enabled": true,
  "mqtt_connected": true,
  "mqtt_last_error": "",
  "thingspeak_enabled": true,
  "thingspeak_last_pub": "2025-06-11T15:30:00Z",
  "thingspeak_last_error": "",
  "hass_enabled": true,
  "sensor_ok": true
}
```

---

## ⚙️ КОНФИГУРАЦИОННЫЕ ENDPOINTS

### **POST /save - Сохранение настроек**
Сохраняет конфигурацию системы.

**URL:** `http://device_ip/save`  
**Метод:** POST  
**Авторизация:** Требуется  
**Content-Type:** application/x-www-form-urlencoded

**Параметры (все опционально):**
```bash
# WiFi настройки
ssid=MyWiFi
password=mypassword

# MQTT настройки  
mqtt_enabled=on
mqtt_server=mqtt.local
mqtt_port=1883
mqtt_user=user
mqtt_password=pass
mqtt_qos=0

# ThingSpeak настройки
ts_enabled=on
ts_api_key=YOUR_API_KEY
ts_channel_id=123456
ts_interval=60

# Home Assistant
hass_enabled=on

# Датчик
real_sensor=on

# Безопасность
web_password=newpassword
```

**Ответы:**
- **302 Found** - перенаправление на главную страницу после сохранения
- **403 Forbidden** - неверный пароль авторизации
- **400 Bad Request** - некорректные параметры

---

## 🔧 СЛУЖЕБНЫЕ ENDPOINTS

### **GET /status - Краткий статус**
Возвращает краткую информацию о состоянии системы.

**URL:** `http://device_ip/status`  
**Метод:** GET  
**Авторизация:** Не требуется  
**Формат ответа:** JSON

**Пример ответа:**
```json
{
  "wifi_mode": "STA",
  "wifi_connected": true,
  "ip": "192.168.1.100"
}
```

---

## 📱 МОБИЛЬНАЯ ОПТИМИЗАЦИЯ

### **Viewport и адаптивность:**
Все HTML endpoints включают мобильную адаптацию:

```html
<meta name="viewport" content="width=device-width, initial-scale=1.0">
```

### **CSS Media queries:**
```css
@media (max-width: 600px) {
  /* Мобильные стили */
  .container { padding: 10px; }
  input, select { width: 100%; }
}
```

---

## 🔄 REAL-TIME ОБНОВЛЕНИЯ

### **AJAX Pattern:**
Веб-интерфейс использует JavaScript для автоматических обновлений:

```javascript
// Обновление данных каждые 2 секунды
function updateData() {
  fetch('/sensor_json')
    .then(response => response.json())
    .then(data => {
      // Обновление DOM элементов
      document.getElementById('temp').textContent = data.t + '°C';
      // ... остальные поля
    })
    .catch(error => {
      showToast('Ошибка загрузки данных', 'error');
    });
}

setInterval(updateData, 2000);
```

---

## 🚨 ОБРАБОТКА ОШИБОК

### **HTTP коды ошибок:**
- **200 OK** - успешный запрос
- **302 Found** - перенаправление после сохранения
- **400 Bad Request** - некорректные параметры
- **403 Forbidden** - требуется авторизация
- **404 Not Found** - endpoint не найден
- **500 Internal Server Error** - внутренняя ошибка сервера  
- **503 Service Unavailable** - сервис недоступен

### **JSON формат ошибок:**
```json
{
  "error": "Описание ошибки",
  "code": "ERROR_CODE",
  "timestamp": 1717920000
}
```

---

## 🔗 ИНТЕГРАЦИЯ С ВНЕШНИМИ СИСТЕМАМИ

### **Home Assistant Integration:**
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
      - nitrogen
      - phosphorus
      - potassium
    value_template: "{{ value_json.valid }}"
```

### **Node-RED Integration:**
```javascript
// HTTP Request node
var msg = {};
msg.url = "http://192.168.1.100/api/sensor";
msg.method = "GET";
return msg;
```

### **Python Integration:**
```python
import requests
import json

def get_sensor_data(device_ip):
    try:
        response = requests.get(f"http://{device_ip}/api/sensor", timeout=5)
        response.raise_for_status()
        return response.json()
    except requests.exceptions.RequestException as e:
        print(f"Error: {e}")
        return None

# Использование
data = get_sensor_data("192.168.1.100")
if data and data.get("valid"):
    print(f"Temperature: {data['temperature']}°C")
    print(f"Humidity: {data['humidity']}%")
    print(f"pH: {data['ph']}")
```

---

## 🔧 CURL ПРИМЕРЫ

### **Получение данных датчика:**
```bash
curl -s http://192.168.1.100/api/sensor | jq .
```

### **Системная информация:**
```bash
curl -s http://192.168.1.100/health | jq .device
```

### **Мониторинг в реальном времени:**
```bash
while true; do
  curl -s http://192.168.1.100/sensor_json | jq '.t, .h, .p'
  sleep 5
done
```

### **Настройка MQTT:**
```bash
curl -X POST http://192.168.1.100/save \
  -d "auth_password=mypass" \
  -d "mqtt_enabled=on" \
  -d "mqtt_server=mqtt.local" \
  -d "mqtt_port=1883"
```

---

## 📊 ПРОИЗВОДИТЕЛЬНОСТЬ API

### **Время отклика (типичное):**
- `/api/sensor` - 50-100ms
- `/sensor_json` - 20-50ms
- `/health` - 100-200ms
- `/service_status` - 80-150ms

### **Throughput:**
- Поддерживает до 10 одновременных подключений
- Максимальная частота запросов: 1 запрос/секунда на endpoint
- Рекомендуемая частота: 1 запрос/2-5 секунд

### **Кэширование:**
- Данные датчика кэшируются на 1 секунду
- Системная информация кэшируется на 5 секунд
- DNS записи кэшируются на 5 минут

---

## 🔮 БУДУЩИЕ ВЕРСИИ API

### **v1.1 (планируется в v2.3.0):**
- OTA endpoints для обновления firmware
- Расширенная диагностика

### **v1.2 (планируется в v2.4.0):**
- История данных endpoints
- Статистика и аналитика
- CSV экспорт

### **v2.0 (планируется в v2.5.0):**
- WebSocket поддержка
- Webhook система
- Расширенная авторизация

---

**Заключение:** API v1.0 предоставляет полный набор функций для интеграции JXCT Soil Sensor с внешними системами, отличается простотой использования и высокой производительностью.

*API документация создана 11 июня 2025 на основе анализа реального кода проекта* 