# Протокол Modbus RTU для JXCT 7-в-1

**Дата:** Июль 2025
**Версия:** 3.10.0

**Автор:** JXCT Development Team

---

## 📋 Содержание {#Soderzhanie}

- [Содержание](#Soderzhanie)
- [Обзор](#Obzor)
- [Технические характеристики](#Tehnicheskie-harakteristiki)
  - [Настройки порта (UART2)](#Nastroyki-porta-uart2)
  - [Параметры MODBUS](#Parametry-modbus)
- [Карта регистров](#Karta-registrov)
  - [Основные измерения](#Osnovnye-izmereniya)
  - [Системные регистры](#Sistemnye-registry)
- [Примеры протокола](#Primery-protokola)
  - [1. Чтение pH почвы (регистр 0x0006)](#1-Chtenie-ph-pochvy-registr-0x0006)
  - [2. Чтение температуры почвы (регистр 0x0013)](#2-Chtenie-temperatury-pochvy-registr-0x0013)
  - [3. Чтение нескольких регистров (NPK)](#3-Chtenie-neskolkih-registrov-npk)
- [Схема подключения ESP32](#Shema-podklyucheniya-esp32)
  - [Физическое подключение](#Fizicheskoe-podklyuchenie)
    - [RS-485 интерфейс](#rs-485-interfeys)
    - [Подключение SP3485E](#Podklyuchenie-sp3485e)
    - [Важность раздельного управления DE и RE](#Vazhnost-razdelnogo-upravleniya-de-i-re)
    - [Временные диаграммы переключения](#Vremennye-diagrammy-pereklyucheniya)
  - [Подключение к датчику JXCT](#Podklyuchenie-k-datchiku-jxct)
  - [Питание датчика (отдельное!)](#Pitanie-datchika-otdelnoe)
- [Реализация в коде ESP32](#Realizatsiya-v-kode-esp32)
  - [Инициализация UART и SP3485E](#Initsializatsiya-uart-i-sp3485e)
  - [Чтение данных](#Chtenie-dannyh)
- [Диагностика и отладка](#Diagnostika-i-otladka)
  - [Коды ошибок ModbusMaster](#Kody-oshibok-modbusmaster)
  - [Проверка связи](#Proverka-svyazi)
- [Расчет CRC16](#Raschet-crc16)
- [Типичные проблемы и решения](#Tipichnye-problemy-i-resheniya)
  - [1. Таймаут ответа (ku8MBResponseTimedOut)](#1-Taymaut-otveta-ku8mbresponsetimedout)
  - [2. Ошибка CRC (ku8MBInvalidCRC)](#2-Oshibka-crc-ku8mbinvalidcrc)
  - [3. Недопустимый адрес данных (ku8MBIllegalDataAddress)](#3-Nedopustimyy-adres-dannyh-ku8mbillegaldataaddress)
- [Валидация данных](#Validatsiya-dannyh)
  - [Допустимые диапазоны](#Dopustimye-diapazony)
- [Справочная информация](#Spravochnaya-informatsiya)
  - [Документация производителя](#Dokumentatsiya-proizvoditelya)
  - [Связанные файлы проекта](#Svyazannye-fayly-proekta)
- [Поддержка](#Podderzhka)
  - [Связь с разработчиками](#Svyaz-s-razrabotchikami)
  - [Дополнительные ресурсы](#Dopolnitelnye-resursy)
  - [Полезные ссылки](#Poleznye-ssylki)

---

## 📖 Содержание {#Soderzhanie}

1. **📋 Обзор**
2. **🔧 Технические характеристики**
3. **📊 Карта регистров**
4. **🔍 Примеры протокола**
5. **🔧 Схема подключения ESP32**
6. **⚡ Оптимизация производительности**
7. **🐛 Отладка и диагностика**
8. **🔒 Безопасность**
9. **📋 Связанная документация**

---

## 📋 Обзор {#Obzor}

Датчик JXCT 7-в-1 использует протокол **Modbus RTU** через интерфейс **RS485** для передачи данных измерений почвы. Этот документ содержит полную техническую спецификацию протокола.

## 🔧 Технические характеристики {#Tehnicheskie-harakteristiki}

### Настройки порта (UART2) {#Nastroyki-porta-uart2}
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

### Параметры MODBUS {#Parametry-modbus}
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

## 📊 Карта регистров {#Karta-registrov}

### Основные измерения {#Osnovnye-izmereniya}
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

### Системные регистры {#Sistemnye-registry}
```
Регистр │ Адрес │ Параметр           │ Формула        │ Единицы │ Доступ
────────┼───────┼────────────────────┼────────────────┼─────────┼────────────
0x0007  │ 7     │ Версия прошивки    │ как есть       │ версия  │ Только чтение
0x0008  │ 8     │ Калибровка         │ как есть       │ флаги   │ Чтение/запись
0x000B  │ 11    │ Статус ошибок      │ как есть       │ флаги   │ Только чтение
0x000C  │ 12    │ Адрес устройства   │ как есть       │ адрес   │ Чтение/запись
```

## 🔍 Примеры протокола {#Primery-protokola}

### 1. Чтение pH почвы (регистр 0x0006) {#1-Chtenie-ph-pochvy-registr-0x0006}

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

### 2. Чтение температуры почвы (регистр 0x0013) {#2-Chtenie-temperatury-pochvy-registr-0x0013}

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

### 3. Чтение нескольких регистров (NPK) {#3-Chtenie-neskolkih-registrov-npk}

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

## 🔧 Схема подключения ESP32 {#Shema-podklyucheniya-esp32}

### Физическое подключение {#Fizicheskoe-podklyuchenie}

#### RS-485 интерфейс {#rs-485-interfeys}
- Используется трансивер SP3485E
- Скорость передачи: до 10 Mbps
- Защита от ESD: ±15kV HBM
- Питание: 3.3V (оптимально для ESP32)

#### Подключение SP3485E {#Podklyuchenie-sp3485e}
```
ESP32 GPIO   │ SP3485E Pin │ Функция
─────────────┼────────────┼──────────────────────────────────
GPIO16       │ RO         │ Receive Output (к UART RX)
GPIO17       │ DI         │ Data Input (от UART TX)
GPIO4        │ DE         │ Driver Enable (управление передатчиком)
GPIO5        │ RE         │ Receiver Enable (управление приемником)
GND          │ GND        │ Общий провод
3.3V         │ VCC        │ Питание модуля
```

#### Важность раздельного управления DE и RE {#Vazhnost-razdelnogo-upravleniya-de-i-re}
- **DE (Driver Enable)** - управляет передатчиком:
  - HIGH: передатчик активен (для отправки данных)
  - LOW: передатчик в высокоимпедансном состоянии
- **RE (Receiver Enable)** - управляет приемником:
  - HIGH: приемник отключен (во время передачи)
  - LOW: приемник активен (для приема данных)

Раздельное управление этими пинами обеспечивает:
1. Более точный контроль над временем переключения
2. Возможность тонкой настройки задержек
3. Предотвращение коллизий на шине RS-485
4. Улучшенную помехозащищенность

#### Временные диаграммы переключения {#Vremennye-diagrammy-pereklyucheniya}

```
DE ──┐          ┌────────┐          ┌────────
     │          │        │          │
     └──────────┘        └──────────┘

RE ──┐          ┌────────┐          ┌────────
     │          │        │          │
     └──────────┘        └──────────┘
     ├─ прием ──┤├─ передача ─┤├─ прием ──┤

     │◄─ 50µs ─►│          │◄─ 50µs ─►│
```

Задержки переключения:
- 50 микросекунд перед передачей (preTransmission)
- 50 микросекунд после передачи (postTransmission)

### Подключение к датчику JXCT {#Podklyuchenie-k-datchiku-jxct}
```
Transceiver  │ JXCT Sensor │ Цвет провода │ Функция
─────────────┼─────────────┼──────────────┼─────────────────
A+ Terminal  │ A+          │ Желтый       │ RS485 Data+
B- Terminal  │ B-          │ Синий        │ RS485 Data-
```

### Питание датчика (отдельное!) {#Pitanie-datchika-otdelnoe}
```
Источник     │ JXCT Sensor │ Цвет провода │ Функция
─────────────┼─────────────┼──────────────┼─────────────────
12-24V DC+   │ VCC         │ Красный      │ Питание датчика
GND          │ GND         │ Черный       │ Общий минус
```

## ⚙️ Реализация в коде ESP32 {#Realizatsiya-v-kode-esp32}

### Инициализация UART и SP3485E {#Initsializatsiya-uart-i-sp3485e}

```cpp
void setupModbus() {
    // Настройка UART2 для Modbus
    Serial2.begin(9600, SERIAL_8N1, MODBUS_RX_PIN, MODBUS_TX_PIN);

    // Настройка пинов SP3485E
    pinMode(MODBUS_DE_PIN, OUTPUT);    // GPIO4
    pinMode(MODBUS_RE_PIN, OUTPUT);    // GPIO5
    digitalWrite(MODBUS_DE_PIN, LOW);  // Передатчик выключен
    digitalWrite(MODBUS_RE_PIN, LOW);  // Приемник включен

    // Инициализация Modbus
    node.begin(SENSOR_ID, Serial2);

    // Настройка обработчиков переключения режима
    node.preTransmission(preTransmission);   // Перед передачей
    node.postTransmission(postTransmission); // После передачи
}

// Управление направлением передачи SP3485E
void preTransmission() {
    digitalWrite(MODBUS_DE_PIN, HIGH);  // Режим передачи
    delayMicroseconds(50);
}

void postTransmission() {
    delayMicroseconds(50);
    digitalWrite(MODBUS_RE_PIN, LOW);   // Режим приема
}
```

### Чтение данных {#Chtenie-dannyh}
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

## 🛠️ Диагностика и отладка {#Diagnostika-i-otladka}

### Коды ошибок ModbusMaster {#Kody-oshibok-modbusmaster}
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

### Проверка связи {#Proverka-svyazi}
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

## 🔍 Расчет CRC16 {#Raschet-crc16}

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

## 🚨 Типичные проблемы и решения {#Tipichnye-problemy-i-resheniya}

### 1. Таймаут ответа (ku8MBResponseTimedOut) {#1-Taymaut-otveta-ku8mbresponsetimedout}
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

### 2. Ошибка CRC (ku8MBInvalidCRC) {#2-Oshibka-crc-ku8mbinvalidcrc}
**Причины:**
- Помехи в линии RS485
- Плохое качество кабеля
- Неправильная скорость передачи

**Решение:**
- Использовать экранированный кабель
- Проверить заземление
- Добавить терминальные резисторы (120 Ом)

### 3. Недопустимый адрес данных (ku8MBIllegalDataAddress) {#3-Nedopustimyy-adres-dannyh-ku8mbillegaldataaddress}
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

## 📐 Валидация данных {#Validatsiya-dannyh}

### Допустимые диапазоны {#Dopustimye-diapazony}
```cpp
bool validateSensorData(SensorData& data) {
    // Температура: -45°C до +115°C (диапазон датчика JXCT 7-in-1)
    if (data.temperature < SENSOR_TEMP_MIN || data.temperature > SENSOR_TEMP_MAX) return false;

    // Влажность: 0% до 100%
    if (data.humidity < SENSOR_HUMIDITY_MIN || data.humidity > SENSOR_HUMIDITY_MAX) return false;

    // pH: 3.0 до 9.0 (рабочий диапазон датчика JXCT 7-in-1)
    if (data.ph < SENSOR_PH_MIN || data.ph > SENSOR_PH_MAX) return false;

    // EC: 0 до 10000 µS/cm (рабочий диапазон датчика JXCT 7-in-1)
    if (data.ec < SENSOR_EC_MIN || data.ec > SENSOR_EC_MAX) return false;

    // NPK: 0 до 1999 мг/кг (рабочий диапазон датчика JXCT 7-in-1)
    if (data.nitrogen < SENSOR_NPK_MIN || data.nitrogen > SENSOR_NPK_MAX) return false;
    if (data.phosphorus < SENSOR_NPK_MIN || data.phosphorus > SENSOR_NPK_MAX) return false;
    if (data.potassium < SENSOR_NPK_MIN || data.potassium > SENSOR_NPK_MAX) return false;

    return true;
}
```

## 📋 Справочная информация {#Spravochnaya-informatsiya}

### Документация производителя {#Dokumentatsiya-proizvoditelya}
- **Производитель:** JXCT (Jingxun Changtong)
- **Модель:** JXBS-3001 7-in-1 Soil Sensor
- **Интерфейс:** RS485 Modbus RTU
- **Питание:** 12-24V DC
- **Протокол:** Modbus RTU

### Связанные файлы проекта {#Svyazannye-fayly-proekta}
- `src/modbus_sensor.h` - Определения констант и структур
- `src/modbus_sensor.cpp` - Реализация функций
- `include/jxct_config_vars.h` - Настройки пинов
- `docs/API.md` - REST API документация

---

*Документ обновлен для версии JXCT v3.10.0*

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
- [API документация](API.md)
- [Управление конфигурацией](CONFIG_MANAGEMENT.md)
- [Схема подключения](WIRING_DIAGRAM.md)
- [Управление версиями](VERSION_MANAGEMENT.md)

### 🔗 Полезные ссылки {#Poleznye-ssylki}

- [🌱 GitHub репозиторий](https://github.com/Gfermoto/soil-sensor-7in1) - Исходный код проекта
- [📋 План рефакторинга](../dev/REFACTORING_PLAN.md) - Планы развития
- [📊 Отчет о техническом долге](../dev/TECHNICAL_DEBT_REPORT.md) - Анализ технических проблем
- [🏗️ Архитектура системы](../dev/ARCH_OVERALL.md) - Общая архитектура проекта
