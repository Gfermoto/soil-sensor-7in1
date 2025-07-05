# 🔌 Обзор API JXCT 7-в-1

**Версия API:** v1
**Базовый URL:** `http://[IP-адрес-устройства]`

## 📋 Краткий обзор {#Kratkiy-obzor}

JXCT Soil Sensor предоставляет REST API для получения данных датчика и управления устройством. API не требует авторизации и предназначен для интеграции с внешними системами.

## 🚀 Основные возможности {#Osnovnye-vozmozhnosti}

### 📊 Данные датчика {#Dannye-datchika}
- **GET** `/api/v1/sensor` — получение текущих показаний
- **GET** `/api/v1/system/health` — полная диагностика системы
- **GET** `/api/v1/system/status` — краткий статус сервисов

### ⚙️ Управление устройством {#Upravlenie-ustroystvom}
- **POST** `/api/v1/system/reset` — сброс настроек
- **POST** `/api/v1/system/reboot` — перезагрузка устройства

### 📁 Конфигурация {#Konfiguratsiya}
- **GET** `/api/v1/config/export` — экспорт настроек
- **POST** `/api/v1/config/import` — импорт настроек

## 🔗 Интеграции {#Integratsii}

### 📡 MQTT {#mqtt}
Автоматическая публикация данных в топики:
```
homeassistant/sensor/jxct_soil/temperature/state
homeassistant/sensor/jxct_soil/humidity/state
homeassistant/sensor/jxct_soil/ec/state
homeassistant/sensor/jxct_soil/ph/state
homeassistant/sensor/jxct_soil/nitrogen/state
homeassistant/sensor/jxct_soil/phosphorus/state
homeassistant/sensor/jxct_soil/potassium/state
```

### 🌐 ThingSpeak {#thingspeak}
Автоматическая отправка данных каждые 15 секунд в поля:
- Field1: Температура (°C)
- Field2: Влажность (%)
- Field3: EC (µS/cm)
- Field4: pH
- Field5: Азот (mg/kg)
- Field6: Фосфор (mg/kg)
- Field7: Калий (mg/kg)

### 🏠 Home Assistant {#home-assistant}
Поддержка автоматического обнаружения (Auto Discovery) для интеграции с Home Assistant.

## 📝 Примеры использования {#Primery-ispolzovaniya}

### Получение данных датчика {#Poluchenie-dannyh-datchika}
```bash
curl http://192.168.4.1/api/v1/sensor
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

### Диагностика системы {#Diagnostika-sistemy}
```bash
curl http://192.168.4.1/api/v1/system/health
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
    "ssid": "MyWiFi",
    "ip": "192.168.4.1"
  },
  "services": {
    "mqtt": {"enabled": true, "connected": true},
    "thingspeak": {"enabled": true}
  }
}
```

## 🔧 Коды ответов {#Kody-otvetov}

- **200** — Успешный запрос
- **400** — Некорректные параметры
- **403** — Доступ запрещен
- **500** — Внутренняя ошибка сервера

## 📱 CORS поддержка {#cors-podderzhka}

API поддерживает CORS для веб-приложений:
```javascript
fetch('http://192.168.4.1/api/v1/sensor')
  .then(response => response.json())
  .then(data => console.log(data));
```

## 📚 Подробная документация {#Podrobnaya-dokumentatsiya}

Для получения подробной информации об эндпоинтах, параметрах и примерах интеграции см. **[📖 API документация](manuals/API.md)**.

## 🔗 Полезные ссылки {#Poleznye-ssylki}

- **[👤 Руководство пользователя](manuals/USER_GUIDE.md)** — настройка и использование
- **[🔧 Техническая документация](manuals/TECHNICAL_DOCS.md)** — технические детали
- **[📡 Протокол Modbus](manuals/MODBUS_PROTOCOL.md)** — промышленная интеграция
- **[🖥️ C++ API (Doxygen)](DOXYGEN_API.md)** — документация исходного кода
