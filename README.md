# JXCT Soil Sensor v2.4.2

IoT система мониторинга почвы на ESP32 с датчиком JXCT 7-в-1 (Modbus RTU).

## 🚀 **Новое в v2.4.2 - Подготовка к OTA:**
- **Фильтр выбросов >2σ:** Статистическое исключение аномальных измерений  
- **Алгоритм медианы:** Выбор между средним и медианой для устойчивости к выбросам
- **Веб-настройки:** Управление алгоритмами фильтрации через интерфейс
- **Подготовка к OTA:** Устранены блокирующие вызовы, оптимизирована архитектура

## 🔧 **Готовность к OTA v2.5.0:**
- ✅ **Память:** 1.1MB свободно (достаточно для OTA partition)
- ✅ **Стабильность:** Убраны блокирующие delay(), watchdog готов
- ✅ **Архитектура:** Константы вынесены, модульность улучшена

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

## 📈 Производительность v2.4.2
- **Flash:** 916KB (46.6%) - стабильное использование после оптимизации
- **RAM:** 53KB (16.2%) - оптимальное потребление памяти
- **Свободно для OTA:** 1.1MB (достаточно для partition + обновление)
- **Стабильность:** Убраны все блокирующие delay(), готовность к OTA
- **Архитектура:** Константы централизованы, код подготовлен к модуляризации

## 🗓️ План развития
См. [`docs/DEVELOPMENT_ROADMAP_2025.md`](docs/DEVELOPMENT_ROADMAP_2025.md)

## 📄 Лицензия
MIT License 