# JXCT Soil Sensor v2.2.1

IoT система мониторинга почвы на ESP32 с датчиком JXCT 7-в-1 (Modbus RTU).

## 🚀 **Новое в v2.2.1 - Оптимизация интервалов:**
- **Экономия трафика в 100 раз:** MQTT публикация каждые 5 мин вместо 3 сек
- **Дельта-фильтр:** публикация только при значимых изменениях (±0.5°C, ±2%, ±0.1pH)
- **Принудительная публикация:** каждый час для мониторинга состояния
- **Оптимизация датчика:** опрос каждые 30 сек вместо 5 сек (экономия CPU)

## 🌱 Что измеряет
- Температура почвы
- Влажность почвы  
- Электропроводность (EC)
- pH уровень
- NPK (Азот, Фосфор, Калий)

## ⚡ Возможности
- **WiFi подключение** - автоматическое переключение AP/STA режимов
- **Веб-интерфейс** - настройка и мониторинг через браузер
- **MQTT публикация** - отправка данных в брокер
- **Home Assistant** - автоматическое добавление датчиков
- **ThingSpeak** - загрузка в облако
- **REST API** - JSON endpoints для интеграций

## 🔧 Оборудование
- ESP32-DevKit (любой с 4MB Flash)
- Датчик JXCT 7-в-1 Soil Sensor  
- MAX485 модуль для RS485
- Провода для подключения

## 📦 Подключение
```
ESP32        MAX485       JXCT Sensor
GPIO16  -->  RO           
GPIO17  -->  DI           
GPIO4   -->  DE/RE        
3.3V    -->  VCC          
GND     -->  GND          
         A+  ---------->  A+
         B-  ---------->  B-
```

## 🚀 Быстрый старт

### 1. Прошивка
```bash
git clone https://github.com/your-repo/jxct-soil-sensor
cd jxct-soil-sensor
pio run -t upload
```

### 2. Первоначальная настройка
1. ESP32 создаст WiFi сеть `jxct-XXXXXX` с паролем `12345678`
2. Подключитесь и откройте `http://192.168.4.1`
3. Настройте WiFi, MQTT, ThingSpeak
4. Нажмите "Сохранить настройки"

### 3. Использование
После подключения к домашней WiFi:
- Настройки: `http://[IP-адрес]/`  
- Мониторинг: `http://[IP-адрес]/readings`
- API данных: `http://[IP-адрес]/api/sensor`

## 🔌 Интеграция с Home Assistant

Добавьте в `configuration.yaml`:
```yaml
mqtt:
  sensor:
    - name: "Soil Temperature"  
      state_topic: "homeassistant/sensor/jxct_soil/temperature/state"
      unit_of_measurement: "°C"
    - name: "Soil Humidity"
      state_topic: "homeassistant/sensor/jxct_soil/humidity/state"  
      unit_of_measurement: "%"
    # ... остальные датчики
```

Или включите MQTT Discovery в настройках - датчики добавятся автоматически.

## 📊 API

### Получить данные датчика
```bash
curl http://192.168.1.100/api/sensor
```

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
  "valid": true
}
```

### Системная информация
```bash
curl http://192.168.1.100/health
```

Полная документация API: [`docs/API.md`](docs/API.md)

## 🛠️ Разработка

**Требования:**
- PlatformIO
- ESP32 Arduino Framework

**Конфигурации сборки:**
- `esp32dev` - разработка с полными логами
- `esp32dev-production` - продакшн с оптимизацией

```bash
# Разработка
pio run -e esp32dev -t upload -t monitor

# Продакшн  
pio run -e esp32dev-production -t upload
```

## 📈 Производительность v2.2.1
- **Flash:** 878KB (44.7%) - +1.4KB за дельта-фильтр
- **RAM:** 52.8KB (16.1%) - +32 байта за новые поля
- **Свободная память:** 230KB+ (стабильно)
- **Uptime:** 37+ минут протестировано, стабильная работа
- **Экономия трафика:** MQTT в 100 раз, ThingSpeak в 60 раз

## 🗓️ План развития
См. [`docs/DEVELOPMENT_ROADMAP_2025.md`](docs/DEVELOPMENT_ROADMAP_2025.md)

## 📄 Лицензия
MIT License 