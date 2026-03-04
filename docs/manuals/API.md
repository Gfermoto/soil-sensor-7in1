# API документация JXCT 7-в-1

**Дата:** Июль 2025
**Версия прошивки:** 3.13.2
**Версия API (в коде):** v1 — см. `include/jxct_strings.h`, `#define API_ROOT "/api/v1"`
**Статус:** Актуально

REST API для интеграции с JXCT Soil Sensor v3.13.2

### Связанная документация {#svyazannaya-dokumentatsiya}
- [C++ API](https://gfermoto.github.io/soil-sensor-7in1/api/index.html) — документация исходного кода
- [Архитектура](TECHNICAL_DOCS.md) — техническая документация
- [Тестирование](../TESTING_GUIDE.md) — как тестировать API

---

## Примечание по совместимости

В коде (`include/jxct_strings.h`) используется `API_ROOT = "/api/v1"`. Реальные пути API:

- `GET /api/v1/sensor` — данные датчика
- `GET /api/v1/system/health` — диагностика
- `GET /api/v1/system/status` — статус сервисов
- `POST /api/v1/system/reset` — сброс настроек
- `POST /api/v1/system/reboot` — перезагрузка
- `GET /api/v1/config/export` — экспорт конфигурации

Legacy пути (поддерживаются):
- `GET /health` — диагностика
- `GET /sensor_json` — данные датчика
- `GET /status` — HTML-страница статуса
- `POST /api/config/import` — импорт конфигурации (без версии в пути)

## 📋 Содержание {#Soderzhanie}

- [Связанная документация](#svyazannaya-dokumentatsiya)
- [Содержание](#Soderzhanie)
- [🆕 API v1 Endpoints](#api-v1-endpoints)
- [Доступ к API](#Dostup-k-api)
  - [Таблица актуальных эндпоинтов (API v1)](#tablitsa-aktualnyh-endpointov)
  - [Legacy эндпоинты](#legacy-endpointy)
- [🆕 Калибровка и научные сервисы](#calibration-scientific-services)
- [🆕 Отчеты и мониторинг](#reports-monitoring)
- [Веб-страницы](#Veb-stranitsy)
  - [GET  - Настройки](#get-Nastroyki)
  - [GET readings - Мониторинг](#get-readings-Monitoring)
  - [GET service - Диагностика](#get-service-Diagnostika)
- [Настройки](#Nastroyki)
  - [POST save - Сохранение настроек](#post-save-Sohranenie-nastroek)
- [MQTT интеграция](#mqtt-integratsiya)
  - [Топики публикации](#Topiki-publikatsii)
  - [Команды управления](#Komandy-upravleniya)
- [ThingSpeak интеграция](#thingspeak-integratsiya)
- [Коды ошибок](#Kody-oshibok)
- [CORS поддержка](#cors-podderzhka)
- [Примеры интеграций](#Primery-integratsiy)
  - [Python](#python)
  - [Node.js](#nodejs)
  - [Home Assistant](#home-assistant)
- [Поддержка](#Podderzhka)
  - [Связь с разработчиками](#Svyaz-s-razrabotchikami)
  - [Дополнительные ресурсы](#Dopolnitelnye-resursy)
  - [Полезные ссылки](#Poleznye-ssylki)

---

## API v1 Endpoints (соответствует jxct_strings.h) {#api-v1-endpoints}

### Основные маршруты

| Endpoint | Метод | Описание | Статус |
|----------|-------|----------|--------|
| `/api/v1/sensor` | GET | Данные датчика с timestamp | Активен |
| `/api/v1/system/health` | GET | Состояние системы | Активен |
| `/api/v1/system/status` | GET | Статус сервисов | Активен |
| `/api/v1/system/reset` | POST | Сброс настроек | Активен |
| `/api/v1/system/reboot` | POST | Перезагрузка | Активен |
| `/api/v1/config/export` | GET | Экспорт конфигурации | Активен |
| `/api/config/import` | POST | Импорт конфигурации | Активен |

### Структура ответа

**GET /api/v1/sensor:**
```json
{
  "timestamp": "2025-07-11T14:30:00Z",
  "version": "3.13.2",
  "sensor_data": {
    "temperature": 24.5,
    "humidity": 65.2,
    "ec": 1850,
    "ph": 6.8,
    "nitrogen": 180,
    "phosphorus": 75,
    "potassium": 220
  },
  "compensated": true,
  "calibrated": true,
  "soil_type": "loam",
  "status": "ok"
}
```

**GET /api/v1/system/health:**
```json
{
  "system": "healthy",
  "uptime": 3600000,
  "memory_free": 45632,
  "wifi_signal": -45,
  "services": {
    "modbus": "active",
    "mqtt": "connected", 
    "calibration": "ready",
    "compensation": "active"
  },
  "timestamp": "2025-07-11T14:30:00Z"
}
```

---

## 🌐 Доступ к API {#Dostup-k-api}

**Все endpoints открыты** - авторизация не требуется.

### Таблица актуальных эндпоинтов {#tablitsa-aktualnyh-endpointov}

| Метод | Путь | Описание |
|-------|------|----------|
| GET   | `/api/v1/sensor`         | Основные данные датчика (JSON) |
| GET   | `/api/v1/system/health`  | Полная диагностика устройства  |
| GET   | `/api/v1/system/status`  | Краткий статус сервисов        |
| POST  | `/api/v1/system/reset`   | Сброс настроек (307 на `/reset`) |
| POST  | `/api/v1/system/reboot`  | Перезагрузка (307 на `/reboot`) |
| GET   | `/api/v1/config/export`  | Скачать конфигурацию (JSON, без паролей) |
| POST  | `/api/config/import`     | Импорт конфигурации            |

### Legacy эндпоинты {#legacy-endpointy}

| Метод | Путь | Описание |
|-------|------|----------|
| GET   | `/sensor_json`           | Те же данные (legacy) |
| GET   | `/health`                | Диагностика (legacy) |
| POST  | `/reset`                 | Сброс (legacy) |
| POST  | `/reboot`                | Перезагрузка (legacy) |

---

## 🌐 Веб-страницы {#Veb-stranitsy}

### GET / - Настройки {#get-Nastroyki}
Веб-интерфейс для настройки WiFi, MQTT, ThingSpeak.

### GET /readings - Мониторинг {#get-readings-Monitoring}
Страница с live данными датчика (обновление каждые 2 сек).

### GET /service - Диагностика {#get-service-Diagnostika}
Статус WiFi, MQTT, ThingSpeak, датчика, системные метрики.

## 📝 Настройки {#Nastroyki}

### POST /save - Сохранение настроек {#post-save-Sohranenie-nastroek}
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

## 🆕 Калибровка и научные сервисы {#calibration-scientific-services}

### 📊 **API калибровки**

Новые endpoints для работы с системой калибровки:

| Endpoint | Метод | Описание | Статус |
|----------|-------|----------|--------|
| `/api/calibration/status` | GET | Статус калибровки | ✅ Активен |
| `/api/calibration/temperature` | POST | Добавить точку калибровки температуры | Активен |
| `/api/calibration/humidity` | POST | Добавить точку калибровки влажности | Активен |
| `/api/calibration/ec` | POST | Добавить точку калибровки EC | Активен |
| `/api/calibration/ph` | POST | Добавить точку калибровки pH | Активен |
| `/api/calibration/reset` | POST | Сброс калибровки | ✅ Активен |

**GET /api/calibration/status:**
```json
{
  "calibration_active": true,
  "points": {
    "temperature": 5,
    "humidity": 3,
    "ec": 7,
    "ph": 4
  },
  "last_calibration": "2025-07-11T12:30:00Z",
  "status": "ready"
}
```

**POST /api/calibration/temperature:**
```json
{
  "reference_value": 25.0,
  "sensor_reading": 24.8,
  "timestamp": "2025-07-11T14:30:00Z"
}
```

### 🔬 **Научные сервисы**

Endpoints для работы с научными алгоритмами:

| Сервис | Описание | Алгоритм |
|--------|----------|----------|
| `SensorCompensationService` | Компенсация показаний датчиков | Rhoades et al. (1989), Уравнение Нернста, Delgado et al. |
| `ScientificValidationService` | Валидация научных формул | Проверка источников и коэффициентов |
| `NutrientInteractionService` | Взаимодействие питательных веществ | Антагонизм/синергизм NPK |
| `CropRecommendationEngine` | Рекомендации по культурам | 24 культуры, 13 типов почв |

### Топики публикации MQTT {#Topiki-publikatsii}
```
homeassistant/sensor/jxct_soil/temperature/state
homeassistant/sensor/jxct_soil/humidity/state
homeassistant/sensor/jxct_soil/ec/state
homeassistant/sensor/jxct_soil/ph/state
homeassistant/sensor/jxct_soil/nitrogen/state
homeassistant/sensor/jxct_soil/phosphorus/state
homeassistant/sensor/jxct_soil/potassium/state
```

### Команды управления {#Komandy-upravleniya}
```bash
# Перезагрузка устройства
mosquitto_pub -h mqtt.local -t "jxct/command" -m "reboot"

# Сброс настроек
mosquitto_pub -h mqtt.local -t "jxct/command" -m "reset"

# Тестовая публикация
mosquitto_pub -h mqtt.local -t "jxct/command" -m "publish_test"
```

## 📡 ThingSpeak интеграция {#thingspeak-integratsiya}

Автоматическая отправка данных каждые 15 секунд в поля:
- Field1: Температура (°C)
- Field2: Влажность (%)
- Field3: EC (µS/cm)
- Field4: pH
- Field5: Азот (mg/kg)
- Field6: Фосфор (mg/kg)
- Field7: Калий (mg/kg)

## 🆕 Отчеты и мониторинг {#reports-monitoring}

### 📈 **API отчетов**

Новые endpoints для получения отчетов и метрик:

| Endpoint | Метод | Описание | Статус |
|----------|-------|----------|--------|
| `/api/reports/test-summary` | GET | Сводка результатов тестирования | ✅ Активен |
| `/api/reports/technical-debt` | GET | Отчет по техническому долгу | ✅ Активен |
| `/api/reports/full` | GET | Полный отчёт (test-summary + technical-debt) | Активен |
| `/reports` | GET | HTML-страница отчетов | ✅ Активен |
| `/reports/dashboard.html` | GET | Дашборд метрик | ✅ Активен |

**GET /api/reports/test-summary:**
```json
{
  "timestamp": "2025-07-11T14:30:00Z",
  "total": 53,
  "passed": 53,
  "failed": 0,
  "success_rate": 100.0,
  "coverage": {
    "python_tests": 50,
    "native_tests": 3,
    "total_coverage": "70.8%"
  }
}
```

**GET /api/reports/technical-debt:**
```json
{
  "clang_tidy_warnings": 169,
  "categories": {
    "bugprone": 19,
    "readability": 74,
    "modernize": 38,
    "misc": 38
  },
  "critical_files": [
    "src/business/crop_recommendation_engine.cpp",
    "src/modbus_sensor.cpp"
  ],
  "timestamp": "2025-07-11T14:30:00Z"
}
```

### 🏠 **MQTT интеграция** {#mqtt-integratsiya}

### Топики публикации {#Topiki-publikatsii}
```
homeassistant/sensor/jxct_soil/temperature/state
homeassistant/sensor/jxct_soil/humidity/state
homeassistant/sensor/jxct_soil/ec/state
homeassistant/sensor/jxct_soil/ph/state
homeassistant/sensor/jxct_soil/nitrogen/state
homeassistant/sensor/jxct_soil/phosphorus/state
homeassistant/sensor/jxct_soil/potassium/state
```

### Команды управления {#Komandy-upravleniya}
```bash
# Перезагрузка устройства
mosquitto_pub -h mqtt.local -t "jxct/command" -m "reboot"

# Сброс настроек
mosquitto_pub -h mqtt.local -t "jxct/command" -m "reset"

# Тестовая публикация
mosquitto_pub -h mqtt.local -t "jxct/command" -m "publish_test"
```

## 🔄 Коды ошибок {#Kody-oshibok}

- **200** - Успешно
- **400** - Некорректные параметры
- **403** - Доступ запрещен
- **500** - Внутренняя ошибка сервера

## 📱 CORS поддержка {#cors-podderzhka}

API поддерживает CORS для локальных сетей:
```javascript
fetch('http://192.168.4.1/api/v1/sensor')
  .then(response => response.json())
  .then(data => console.log(data));
```

## 🔧 Примеры интеграций {#Primery-integratsiy}

### Python {#python}
```python
import requests

# Получить данные датчика
response = requests.get('http://192.168.4.1/api/v1/sensor')
data = response.json()
print(f"Температура: {data['temperature']}°C")
```

### Node.js {#nodejs}
```javascript
const axios = require('axios');

async function getSensorData() {
  const response = await axios.get('http://192.168.4.1/api/v1/sensor');
  return response.data;
}
```

### Home Assistant {#home-assistant}
```yaml
# configuration.yaml
sensor:
  - platform: rest
    resource: http://192.168.4.1/api/v1/sensor
    name: "JXCT Soil Sensor"
    json_attributes:
      - temperature
      - humidity
      - ph
      - ec
    value_template: "{{ value_json.temperature }}"
```

## 📞 Поддержка {#Podderzhka}

### 💬 Связь с разработчиками {#Svyaz-s-razrabotchikami}
- **Telegram:** [@Gfermoto](https://t.me/Gfermoto)
- **GitHub Issues:** [Сообщить о проблеме](https://github.com/Gfermoto/soil-sensor-7in1/issues)
- **Документация:** [GitHub Pages](https://gfermoto.github.io/soil-sensor-7in1/)

### 📚 Дополнительные ресурсы {#Dopolnitelnye-resursy}
- [Руководство пользователя](USER_GUIDE.md)
- [Техническая документация](TECHNICAL_DOCS.md)
- [Агрономические рекомендации](AGRO_RECOMMENDATIONS.md)
- [Руководство по компенсации](COMPENSATION_GUIDE.md)
- [Управление конфигурацией](CONFIG_MANAGEMENT.md)
- [Схема подключения](WIRING_DIAGRAM.md)
- [Протокол Modbus](MODBUS_PROTOCOL.md)
- [Управление версиями](VERSION_MANAGEMENT.md)

### 🔗 Полезные ссылки {#Poleznye-ssylki}

- [🌱 GitHub репозиторий](https://github.com/Gfermoto/soil-sensor-7in1) - Исходный код проекта
- [📋 План рефакторинга](../dev/REFACTORING_PLAN.md) - Планы развития
- [📊 Отчет о техническом долге](../dev/TECHNICAL_DEBT_REPORT.md) - Анализ технических проблем
- [🏗️ Архитектура системы](../dev/ARCH_OVERALL.md) - Общая архитектура проекта
