# MODBUS RTU Протокол для JXCT 7-в-1 Датчика Почвы

## 📋 Обзор

Датчик JXCT 7-в-1 использует протокол **Modbus RTU** через интерфейс **RS485** для передачи данных измерений почвы. Этот документ содержит полную техническую спецификацию протокола.

## 🔧 Технические характеристики

### Настройки порта (UART2)
```
Параметр            │ Значение
────────────────────┼─────────────
Скорость передачи   │ 9600 baud
Биты данных         │ 8 bits
Четность            │ None (N)
Стоп биты           │ 1 bit
Управление потоком  │ None
Формат              │ 8N1
Интерфейс           │ RS485 полудуплекс
```

### Параметры MODBUS
```
Параметр                │ Значение
────────────────────────┼─────────────
Протокол                │ Modbus RTU
Адрес устройства        │ 1 (по умолчанию, настраивается)
Функция чтения          │ 0x03 (Read Holding Registers)
Функция записи          │ 0x06 (Write Single Register)
Таймаут ответа          │ 1000 мс
Максимум попыток        │ 3
Интерфейс               │ RS485 полудуплекс
```

## 📊 Карта регистров

### Основные измерения
```
Регистр │ Адрес │ Параметр           │ Формула        │ Единицы │ Диапазон
────────┼───────┼────────────────────┼────────────────┼─────────┼──────────────
0x0006  │ 6     │ pH почвы           │ значение ÷ 100 │ pH      │ 0.00-14.00
0x0012  │ 18    │ Влажность почвы    │ значение ÷ 10  │ %       │ 0.0-100.0
0x0013  │ 19    │ Температура почвы  │ значение ÷ 10  │ °C      │ -10.0-50.0
0x0015  │ 21    │ Электропроводность │ как есть       │ µS/cm   │ 0-20000
0x001E  │ 30    │ Азот (N)           │ как есть       │ мг/кг   │ 0-2000
0x001F  │ 31    │ Фосфор (P)         │ как есть       │ мг/кг   │ 0-2000
0x0020  │ 32    │ Калий (K)          │ как есть       │ мг/кг   │ 0-2000
```

### Системные регистры
```
Регистр │ Адрес │ Параметр           │ Формула        │ Единицы │ Доступ
────────┼───────┼────────────────────┼────────────────┼─────────┼────────────
0x0007  │ 7     │ Версия прошивки    │ как есть       │ версия  │ Только чтение
0x0008  │ 8     │ Калибровка         │ как есть       │ флаги   │ Чтение/запись
0x000B  │ 11    │ Статус ошибок      │ как есть       │ флаги   │ Только чтение
0x000C  │ 12    │ Адрес устройства   │ как есть       │ адрес   │ Чтение/запись
```

## 🔍 Примеры протокола

### 1. Чтение pH почвы (регистр 0x0006)

**Запрос:**
```
Байт │ Hex │ Описание
─────┼─────┼─────────────────────────────
0    │ 01  │ Адрес устройства (1)
1    │ 03  │ Функция (Read Holding Registers)
2    │ 00  │ Адрес регистра (High byte)
3    │ 06  │ Адрес регистра (Low byte) - 0x0006
4    │ 00  │ Количество регистров (High byte)
5    │ 01  │ Количество регистров (Low byte) - 1
6    │ 64  │ CRC16 (High byte)
7    │ 0B  │ CRC16 (Low byte)
```

**Ответ:**
```
Байт │ Hex │ Описание
─────┼─────┼─────────────────────────────
0    │ 01  │ Адрес устройства (1)
1    │ 03  │ Функция (Read Holding Registers)
2    │ 02  │ Количество байт данных (2)
3    │ 02  │ Значение pH (High byte)
4    │ BC  │ Значение pH (Low byte)
5    │ 38  │ CRC16 (High byte)
6    │ 05  │ CRC16 (Low byte)
```

**Расчет значения:**
```
Hex значение: 0x02BC = 700 (decimal)
pH = 700 ÷ 100 = 7.00
```

### 2. Чтение температуры почвы (регистр 0x0013)

**Запрос:**
```
01 03 00 13 00 01 74 0F
```

**Ответ:**
```
01 03 02 00 ED 78 B8
```

**Расчет значения:**
```
Hex значение: 0x00ED = 237 (decimal)
Температура = 237 ÷ 10 = 23.7°C
```

### 3. Чтение нескольких регистров (NPK)

**Запрос (регистры 0x001E-0x0020):**
```
01 03 00 1E 00 03 65 CC
```

**Ответ:**
```
01 03 06 01 5E 01 F3 03 D6 XX XX
```

**Расчет значений:**
```
Азот (N):    0x015E = 350 мг/кг
Фосфор (P):  0x01F3 = 499 мг/кг  
Калий (K):   0x03D6 = 982 мг/кг
```

## 🔧 Схема подключения ESP32

### Подключение RS-485 трансивера

#### Вариант 1: MAX485 (классический)
```
ESP32 GPIO   │ MAX485 Pin │ Функция
─────────────┼────────────┼─────────────────────
GPIO16 (RX)  │ RO         │ Receive Output (прием данных)
GPIO17 (TX)  │ DI         │ Driver Input (передача данных)
GPIO4        │ DE         │ Driver Enable (управление передачей)
GPIO5        │ RE         │ Receiver Enable (управление приемом)
3.3V         │ VCC        │ Питание модуля
GND          │ GND        │ Общий провод
```

#### ⭐ Вариант 2: SP3485E (рекомендуется)
```
ESP32 GPIO   │ SP3485E Pin │ Функция
─────────────┼─────────────┼─────────────────────
GPIO16 (RX)  │ RO (Pin 1)  │ Receive Output (прием данных)
GPIO17 (TX)  │ DI (Pin 4)  │ Driver Input (передача данных)
GPIO4        │ DE (Pin 3)  │ Driver Enable (управление передачей)
GPIO4        │ RE (Pin 2)  │ Receiver Enable (соединить с DE)
3.3V         │ VCC (Pin 8) │ Питание +3.3V
GND          │ GND (Pin 5) │ Общий провод
```

**💡 Преимущества SP3485E:**
- ✅ Нативная 3.3V логика (идеальная совместимость с ESP32)
- ✅ Высокая скорость до 12 Mbps (vs 2.5 Mbps у MAX485)
- ✅ Расширенная защита ±15kV ESD
- ✅ Низкое энергопотребление

### Подключение к датчику JXCT
```
Transceiver  │ JXCT Sensor │ Цвет провода │ Функция
─────────────┼─────────────┼──────────────┼─────────────────
A+ Terminal  │ A+          │ Желтый       │ RS485 Data+
B- Terminal  │ B-          │ Синий        │ RS485 Data-
```

### Питание датчика (отдельное!)
```
Источник     │ JXCT Sensor │ Цвет провода │ Функция
─────────────┼─────────────┼──────────────┼─────────────────
12-24V DC+   │ VCC         │ Красный      │ Питание датчика
GND          │ GND         │ Черный       │ Общий минус
```

## ⚙️ Реализация в коде ESP32

### Инициализация порта
```cpp
// Настройка пинов MAX485
#define RX_PIN 16
#define TX_PIN 17
#define DE_PIN 4
#define RE_PIN 5

void setupModbus() {
    // Настройка пинов управления MAX485
    pinMode(DE_PIN, OUTPUT);
    pinMode(RE_PIN, OUTPUT);
    digitalWrite(DE_PIN, LOW);  // Режим приема
    digitalWrite(RE_PIN, LOW);  // Режим приема
    
    // Инициализация UART2
    Serial2.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);
    
    // Инициализация ModbusMaster
    modbus.begin(1, Serial2);  // Адрес устройства 1
    modbus.preTransmission(preTransmission);
    modbus.postTransmission(postTransmission);
}
```

### Управление направлением передачи
```cpp
void preTransmission() {
    digitalWrite(DE_PIN, HIGH);  // Включить передачу
    digitalWrite(RE_PIN, HIGH);  // Отключить прием
}

void postTransmission() {
    digitalWrite(DE_PIN, LOW);   // Отключить передачу
    digitalWrite(RE_PIN, LOW);   // Включить прием
}
```

### Чтение данных
```cpp
void readSensorData() {
    // Чтение pH
    uint8_t result = modbus.readHoldingRegisters(0x0006, 1);
    if (result == modbus.ku8MBSuccess) {
        uint16_t ph_raw = modbus.getResponseBuffer(0);
        float ph = ph_raw / 100.0;
    }
    
    // Чтение температуры
    result = modbus.readHoldingRegisters(0x0013, 1);
    if (result == modbus.ku8MBSuccess) {
        uint16_t temp_raw = modbus.getResponseBuffer(0);
        float temperature = temp_raw / 10.0;
    }
    
    // Чтение NPK (3 регистра за один запрос)
    result = modbus.readHoldingRegisters(0x001E, 3);
    if (result == modbus.ku8MBSuccess) {
        uint16_t nitrogen = modbus.getResponseBuffer(0);
        uint16_t phosphorus = modbus.getResponseBuffer(1);
        uint16_t potassium = modbus.getResponseBuffer(2);
    }
}
```

## 🛠️ Диагностика и отладка

### Коды ошибок ModbusMaster
```
Код │ Константа              │ Описание
────┼────────────────────────┼─────────────────────────────
0   │ ku8MBSuccess           │ Успешное выполнение
1   │ ku8MBIllegalFunction   │ Недопустимая функция
2   │ ku8MBIllegalDataAddress│ Недопустимый адрес данных
3   │ ku8MBIllegalDataValue  │ Недопустимое значение данных
4   │ ku8MBSlaveDeviceFailure│ Ошибка ведомого устройства
224 │ ku8MBInvalidSlaveID    │ Недопустимый ID устройства
225 │ ku8MBInvalidFunction   │ Недопустимая функция
226 │ ku8MBResponseTimedOut  │ Таймаут ответа
227 │ ku8MBInvalidCRC        │ Ошибка CRC
```

### Проверка связи
```cpp
bool testModbusConnection() {
    logSystem("Тест связи с датчиком JXCT...");
    
    // Попытка чтения версии прошивки
    uint8_t result = modbus.readHoldingRegisters(0x0007, 1);
    
    if (result == modbus.ku8MBSuccess) {
        uint16_t version = modbus.getResponseBuffer(0);
        logSuccess("Датчик найден! Версия прошивки: %d.%d", 
                  (version >> 8) & 0xFF, version & 0xFF);
        return true;
    } else {
        logError("Ошибка связи с датчиком: %d", result);
        return false;
    }
}
```

## 🔍 Расчет CRC16

MODBUS RTU использует CRC16 с полиномом 0xA001:

```cpp
uint16_t calculateCRC16(uint8_t* data, size_t length) {
    uint16_t crc = 0xFFFF;
    
    for (size_t i = 0; i < length; i++) {
        crc ^= (uint16_t)data[i];
        for (int j = 0; j < 8; j++) {
            if (crc & 0x0001) {
                crc = (crc >> 1) ^ 0xA001;
            } else {
                crc = crc >> 1;
            }
        }
    }
    
    return crc;
}
```

## 🚨 Типичные проблемы и решения

### 1. Таймаут ответа (ku8MBResponseTimedOut)
**Причины:**
- Неправильное подключение A+/B-
- Неисправность кабеля RS485
- Неправильный адрес устройства
- Проблемы с питанием датчика

**Решение:**
```cpp
// Проверка подключения
if (!testModbusConnection()) {
    logError("Проверьте подключение RS485 и питание датчика");
}
```

### 2. Ошибка CRC (ku8MBInvalidCRC)
**Причины:**
- Помехи в линии RS485
- Плохое качество кабеля
- Неправильная скорость передачи

**Решение:**
- Использовать экранированный кабель
- Проверить заземление
- Добавить терминальные резисторы (120 Ом)

### 3. Недопустимый адрес данных (ku8MBIllegalDataAddress)
**Причины:**
- Запрос несуществующего регистра
- Различия в версиях прошивки датчика

**Решение:**
```cpp
// Проверка поддерживаемых регистров
const uint16_t test_registers[] = {0x0006, 0x0012, 0x0013, 0x0015};
for (int i = 0; i < 4; i++) {
    uint8_t result = modbus.readHoldingRegisters(test_registers[i], 1);
    if (result != modbus.ku8MBSuccess) {
        logWarn("Регистр 0x%04X недоступен: %d", test_registers[i], result);
    }
}
```

## 📐 Валидация данных

### Допустимые диапазоны
```cpp
bool validateSensorData(SensorData& data) {
    // Температура: -10°C до +50°C
    if (data.temperature < -10.0 || data.temperature > 50.0) return false;
    
    // Влажность: 0% до 100%
    if (data.humidity < 0.0 || data.humidity > 100.0) return false;
    
    // pH: 0 до 14
    if (data.ph < 0.0 || data.ph > 14.0) return false;
    
    // EC: 0 до 20000 µS/cm
    if (data.ec < 0.0 || data.ec > 20000.0) return false;
    
    // NPK: 0 до 2000 мг/кг
    if (data.nitrogen < 0.0 || data.nitrogen > 2000.0) return false;
    if (data.phosphorus < 0.0 || data.phosphorus > 2000.0) return false;
    if (data.potassium < 0.0 || data.potassium > 2000.0) return false;
    
    return true;
}
```

## 📚 Справочная информация

### Документация производителя
- **Производитель:** JXCT (Jingxun Changtong)
- **Модель:** JXBS-3001 7-in-1 Soil Sensor
- **Интерфейс:** RS485 Modbus RTU
- **Питание:** 12-24V DC
- **Протокол:** Modbus RTU

### Связанные файлы проекта
- `src/modbus_sensor.h` - Определения констант и структур
- `src/modbus_sensor.cpp` - Реализация функций
- `include/jxct_config_vars.h` - Настройки пинов
- `docs/API.md` - REST API документация

---

*Документ обновлен для версии JXCT v2.4.3* 