# API Документация

Добро пожаловать в API документацию для датчика почвы JXCT 7-в-1!

## Обзор

Этот проект предоставляет полную API документацию, сгенерированную с помощью Doxygen. Документация включает в себя:

- **Заголовочные файлы** - интерфейсы классов и функций
- **Исходный код** - реализация всех компонентов
- **Структуры данных** - определения структур и классов
- **Диаграммы** - визуализация связей между компонентами

## Основные разделы

### 📚 [API Reference](../site/api-overview/index.html)
Главная страница API документации с обзором всех компонентов.

### 🔧 [Исходный код](../site/api-overview/files.html)
Список всех файлов исходного кода с подробным описанием.

### 📋 [Список функций](../site/api-overview/functions.html)
Алфавитный список всех функций с описанием параметров и возвращаемых значений.

### 🏗️ [Структуры данных](../site/api-overview/annotated.html)
Определения всех структур данных, классов и интерфейсов.

## Основные компоненты

### Датчики
- `ISensor` - базовый интерфейс для всех датчиков
- `BasicSensorAdapter` - адаптер для базовых датчиков
- `ModbusSensorAdapter` - адаптер для Modbus датчиков
- `FakeSensorAdapter` - тестовый адаптер

### Менеджеры
- `WiFiManager` - управление WiFi подключением
- `MQTTClient` - MQTT клиент для отправки данных
- `ThingSpeakClient` - интеграция с ThingSpeak
- `OTAManager` - обновление по воздуху
- `CalibrationManager` - управление калибровкой

### Утилиты
- `ValidationUtils` - валидация данных
- `FormatUtils` - форматирование данных
- `Logger` - система логирования

## Примеры использования

### Подключение датчика
```cpp
#include "ISensor.h"
#include "ModbusSensorAdapter.h"

// Создание адаптера для Modbus датчика
ModbusSensorAdapter sensor(Serial2, 1);

// Инициализация
if (sensor.begin()) {
    Serial.println("Датчик подключен успешно");
}
```

### Получение данных
```cpp
SensorData data = sensor.readData();
if (data.isValid()) {
    Serial.printf("Температура: %.2f°C\n", data.temperature);
    Serial.printf("Влажность: %.2f%%\n", data.humidity);
    Serial.printf("pH: %.2f\n", data.ph);
}
```

## Сборка документации

Документация автоматически генерируется при каждом коммите в ветку `main`. Для локальной сборки:

```bash
# Установка Doxygen
sudo apt-get install doxygen graphviz

# Генерация документации
doxygen Doxyfile
```

## Поддержка

Если у вас есть вопросы по API или вы нашли ошибки в документации, пожалуйста:

1. Создайте Issue в GitHub репозитории
2. Обратитесь в Telegram: [@jxct_support](https://t.me/jxct_support)
3. Присоединитесь к Discord серверу: [JXCT Community](https://discord.gg/jxct) 