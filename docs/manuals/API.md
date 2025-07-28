# 📊 API документация JXCT 7-в-1

**Дата:** Июль 2025
**Версия API:** v3.11.0

REST API для интеграции с JXCT Soil Sensor v3.11.0

### 🔗 Связанная документация {#Svyazannaya-dokumentatsiya}
- **[🖥️ C++ API](https://gfermoto.github.io/soil-sensor-7in1/api/index.html)** - Документация исходного кода
- **[🏗️ Архитектура](TECHNICAL_DOCS.md)** - Техническая документация
- **[🧪 Тестирование](../TESTING_GUIDE.md)** - Как тестировать API

---

## 📋 Содержание {#Soderzhanie}

  - [Связанная документация](#Svyazannaya-dokumentatsiya)
- [Содержание](#Soderzhanie)
- [Доступ к API](#Dostup-k-api)
  - [Таблица актуальных эндпоинтов (API v3.11.0)](#Tablitsa-aktualnyh-endpointov-api-v3.11.0)
  - [УстаревшиеDEPRECATED эндпоинты](#Ustarevshiedeprecated-endpointy)
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

## 📖 Содержание {#Soderzhanie}

1. **🌐 Доступ к API**
2. **📊 Основные endpoints**
3. **🌐 Веб-страницы**
4. **📝 Настройки**
5. **🏠 MQTT интеграция**
6. **📡 ThingSpeak интеграция**
7. **🔄 Коды ошибок**
8. **📱 CORS поддержка**

---

## 🌐 Доступ к API {#Dostup-k-api}

**Все endpoints открыты** - авторизация не требуется.

### 📋 Таблица актуальных эндпоинтов (API v3.11.0) {#Tablitsa-aktualnyh-endpointov-api-v3.11.0}

| Метод | Путь | Описание |
|-------|------|----------|
| GET   | `/api/v3.11.0/sensor`         | Основные данные датчика (JSON) |
| GET   | `/api/v3.11.0/system/health`  | Полная диагностика устройства  |
| GET   | `/api/v3.11.0/system/status`  | Краткий статус сервисов        |
| POST  | `/api/v3.11.0/system/reset`   | Сброс настроек (307 на `/reset`) |
| POST  | `/api/v3.11.0/system/reboot`  | Перезагрузка (307 на `/reboot`) |
| GET   | `/api/v3.11.0/config/export`  | Скачать конфигурацию (JSON, без паролей) |
| POST  | `/api/v3.11.0/config/import`  | Импорт конфигурации            |

### 🕑 Устаревшие/DEPRECATED эндпоинты {#Ustarevshiedeprecated-endpointy}

| Метод | Путь | Описание |
|-------|------|----------|
| GET   | `/sensor_json`           | Те же данные (legacy, будет удалён в будущих версиях) |
| GET   | `/api/sensor`            | DEPRECATED alias → `/api/v3.11.0/sensor` |
| GET   | `/api/config/export`     | DEPRECATED alias → `/api/v3.11.0/config/export` |
| POST  | `/api/config/import`     | DEPRECATED alias → `/api/v3.11.0/config/import` |
| POST  | `/reset`                 | Legacy сброс (будет удалён) |
| POST  | `/reboot`                | Legacy перезагрузка (будет удалён) |
| GET   | `/health`                | Старый путь диагностики |

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

## 🏠 MQTT интеграция {#mqtt-integratsiya}

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

## 📡 ThingSpeak интеграция {#thingspeak-integratsiya}

Автоматическая отправка данных каждые 15 секунд в поля:
- Field1: Температура (°C)
- Field2: Влажность (%)
- Field3: EC (µS/cm)
- Field4: pH
- Field5: Азот (mg/kg)
- Field6: Фосфор (mg/kg)
- Field7: Калий (mg/kg)

## 🔄 Коды ошибок {#Kody-oshibok}

- **200** - Успешно
- **400** - Некорректные параметры
- **403** - Доступ запрещен
- **500** - Внутренняя ошибка сервера

## 📱 CORS поддержка {#cors-podderzhka}

API поддерживает CORS для локальных сетей:
```javascript
fetch('http://192.168.4.1/api/sensor')
  .then(response => response.json())
  .then(data => console.log(data));
```

## 🔧 Примеры интеграций {#Primery-integratsiy}

### Python {#python}
```python
import requests

# Получить данные датчика
response = requests.get('http://192.168.4.1/api/sensor')
data = response.json()
print(f"Температура: {data['temperature']}°C")
```

### Node.js {#nodejs}
```javascript
const axios = require('axios');

async function getSensorData() {
  const response = await axios.get('http://192.168.4.1/api/sensor');
  return response.data;
}
```

### Home Assistant {#home-assistant}
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
