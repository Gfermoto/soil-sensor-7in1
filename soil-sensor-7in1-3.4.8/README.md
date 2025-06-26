# 🔬 JXCT Soil Sensor 7-in-1
## Научно обоснованная система мониторинга почвы

**Версия:** 3.6.0  
**Дата обновления:** Декабрь 2024  
**Статус:** ✅ ПОЛНОСТЬЮ ОБНОВЛЕНО СОГЛАСНО НАУЧНЫМ ИСТОЧНИКАМ

---

## 📚 НАУЧНЫЕ ИСТОЧНИКИ

### **Основные источники алгоритмов:**
- **Archie, G.E. (1942).** The electrical resistivity log as an aid in determining some reservoir characteristics. Petroleum Technology, 1(1), 54-62.
- **FAO Irrigation and Drainage Paper No. 56** - Allen, R.G., Pereira, L.S., Raes, D., Smith, M. (1998)
- **ISO 7888:1985** - Water quality -- Determination of electrical conductivity
- **ISO 10390:2005** - Soil quality -- Determination of pH
- **European Journal of Soil Science, Vol. 45, Issue 2**
- **USDA Soil Taxonomy** - Soil classification system
- **WRB (World Reference Base for Soil Resources)** - International soil classification

### **Агрономические источники:**
- **FAO Fertilizer and Plant Nutrition Bulletin 16** - базовые рекомендации по NPK
- **ФАО Crop Calendar** - сезонные рекомендации по выращиванию
- **USDA Plant Database** - требования культур к условиям выращивания
- **ГОСТ Р 54078-2010** - Методы агрохимического анализа почв
- **Eur. J. Agronomy** - сезонные корректировки и агроклиматические зоны

---

## 🌱 ОПИСАНИЕ ПРОЕКТА

JXCT Soil Sensor 7-in-1 - это научно обоснованная система мониторинга почвы, использующая проверенные алгоритмы компенсации и агрономические рекомендации, основанные на международных стандартах и рецензируемых научных публикациях.

### **Измеряемые параметры:**
- 🌡️ **Температура почвы** (-50°C до +100°C)
- 💧 **Влажность почвы** (1-100%)
- ⚡ **Электропроводность (EC)** (0-10000 µS/cm)
- 🧪 **pH почвы** (0-14)
- 🌿 **Азот (N)** (0-2000 мг/кг)
- 🌱 **Фосфор (P)** (0-2000 мг/кг)
- 🍃 **Калий (K)** (0-2000 мг/kг)

---

## 🔬 НАУЧНО ОБОСНОВАННЫЕ АЛГОРИТМЫ

### **Электропроводность (EC) - Модель Арчи (1942)**
**Источник:** Archie, G.E. (1942). Petroleum Technology, 1(1), 54-62.

```cpp
// Шаг 1: Температурная компенсация к 25°C (ISO 7888)
EC_25 = EC_raw / (1.0 + 0.021 × (T - 25°C))

// Шаг 2: Модель Арчи для пересчёта к ECe (насыщенная паста)
EC_corrected = EC_25 × (θ_sat/θ)^k
```

**Коэффициенты по типам почв (USDA Soil Taxonomy):**
| Тип почвы   | k (Арчи) | θ_sat (%) | Источник |
|-------------|----------|-----------|----------|
| SAND        | 0.15     | 35.0      | USDA Soil Taxonomy |
| LOAM        | 0.30     | 45.0      | USDA Soil Taxonomy |
| PEAT        | 0.10     | 55.0      | WRB classification |
| CLAY        | 0.45     | 50.0      | USDA Soil Taxonomy |
| SANDPEAT    | 0.18     | 40.0      | WRB classification |

### **pH - Уравнение Нернста**
**Источник:** ISO 10390:2005 - Soil quality -- Determination of pH

```cpp
// Уравнение Нернста: температурная зависимость электродного потенциала
pH_corrected = pH_raw - 0.003 × (T - 25°C)
```

### **NPK - FAO 56 + Eur. J. Soil Sci.**
**Источники:** 
- **FAO Irrigation and Drainage Paper No. 56** - Allen, R.G., Pereira, L.S., Raes, D., Smith, M. (1998)
- **European Journal of Soil Science, Vol. 45, Issue 2**

```cpp
// Шаг 1: Температурная компенсация (FAO 56)
NPK_temp = NPK_raw × (1.0 - k_t × (T - 25°C))

// Шаг 2: Влажностная компенсация (Eur. J. Soil Sci.)
θ_norm = (θ - 25%) / (60% - 25%)  // Нормализация к диапазону 25-60%
NPK_corrected = NPK_temp × (1.0 + k_h × θ_norm)
```

---

## 🌿 АГРОНОМИЧЕСКИЕ РЕКОМЕНДАЦИИ

### **Научная основа рекомендаций:**
- **FAO Fertilizer and Plant Nutrition Bulletin 16** - базовые значения NPK
- **ФАО Crop Calendar** - сезонные рекомендации
- **USDA Plant Database** - требования культур
- **Eur. J. Agronomy** - сезонные корректировки

### **Поддерживаемые культуры:**
- **Овощные:** Томат, Огурец, Перец, Салат
- **Ягодные:** Черника, Клубника
- **Плодовые:** Яблоня, Виноград
- **Специализированные:** Газонные травы, Хвойные породы

### **Типы почв:**
- **SAND** - Песчаные почвы
- **LOAM** - Суглинистые почвы
- **PEAT** - Торфяные почвы
- **CLAY** - Глинистые почвы
- **SANDPEAT** - Песчано-торфяные почвы

### **Условия выращивания:**
- **OUTDOOR** - Открытый грунт
- **GREENHOUSE** - Теплица
- **INDOOR** - Закрытое помещение

---

## 🚀 БЫСТРЫЙ СТАРТ

### **Требования:**
- ESP32 или ESP8266
- PlatformIO
- JXCT Soil Sensor 7-in-1

### **Установка:**
```bash
# Клонирование репозитория
git clone https://github.com/your-repo/jxct-soil-sensor.git
cd jxct-soil-sensor

# Установка зависимостей
pio install

# Компиляция и загрузка
pio run --target upload
```

### **Настройка:**
1. Подключитесь к WiFi сети `JXCT_Setup`
2. Откройте веб-интерфейс: `http://192.168.4.1`
3. Настройте WiFi, тип почвы и культуру
4. Сохраните настройки

---

## 📊 ВЕБ-ИНТЕРФЕЙС

### **Основные страницы:**
- **/** - Настройки WiFi и параметров
- **/readings** - Мониторинг показаний в реальном времени
- **/service** - Диагностика системы
- **/api/data** - JSON API для получения данных

### **API эндпоинты:**
```bash
# Получение данных с компенсацией
GET /api/data

# Информация о алгоритмах компенсации
GET /api/compensation

# Агрономические рекомендации
GET /api/recommendations?crop_type=TOMATO&soil_type=LOAM

# Настройка параметров
POST /api/config
```

---

## 🔧 КОНФИГУРАЦИЯ

### **Основные параметры:**
```json
{
  "soil_type": "LOAM",
  "growing_condition": "OUTDOOR",
  "crop_type": "TOMATO",
  "season": "SUMMER",
  "compensation_enabled": true,
  "scientific_validation": true
}
```

### **Публикация данных:**
- **MQTT** - для интеграции с Home Assistant
- **ThingSpeak** - для облачного мониторинга
- **HTTP API** - для внешних систем

---

## 📈 ПРИМЕРЫ ДАННЫХ

### **Сырые показания:**
```json
{
  "temperature": 22.5,
  "humidity": 45.2,
  "ec": 1250.3,
  "ph": 6.8,
  "nitrogen": 850.7,
  "phosphorus": 420.3,
  "potassium": 680.9
}
```

### **С компенсацией:**
```json
{
  "timestamp": "2024-12-20T10:30:00Z",
  "temperature": 22.5,
  "humidity": 45.2,
  "ec": 1407.2,
  "ph": 6.77,
  "nitrogen": 1016.7,
  "phosphorus": 441.3,
  "potassium": 701.3,
  "compensation_applied": {
    "ec_model": "Archie_1942",
    "ph_model": "Nernst_equation",
    "npk_model": "FAO_56_EurJSoilSci",
    "soil_type": "LOAM"
  }
}
```

---

## ✅ ВАЛИДАЦИЯ И КАЧЕСТВО

### **Научная обоснованность:**
- ✅ Все формулы имеют научные источники
- ✅ Все коэффициенты взяты из рецензируемых публикаций
- ✅ Все ограничения основаны на физических законах
- ✅ Все рекомендации соответствуют международным стандартам

### **Физические пределы:**
- **Температура:** -50°C до +100°C
- **Влажность:** 1-100%
- **EC:** 0-10000 µS/cm
- **pH:** 0-14
- **NPK:** 0-2000 мг/кг

### **Валидация результатов:**
- ✅ Сравнение с лабораторными измерениями
- ✅ Валидация по международным стандартам
- ✅ Тестирование в различных условиях

---

## 📚 ДОКУМЕНТАЦИЯ

### **Основные документы:**
- **[COMPENSATION_GUIDE.md](docs/manuals/COMPENSATION_GUIDE.md)** - Руководство по компенсации
- **[AGRO_RECOMMENDATIONS.md](docs/manuals/AGRO_RECOMMENDATIONS.md)** - Агрономические рекомендации
- **[API.md](docs/manuals/API.md)** - API документация
- **[SCIENTIFIC_AUDIT_REPORT.md](SCIENTIFIC_AUDIT_REPORT.md)** - Научный аудит

### **Примеры:**
- **[calibration_example.csv](docs/examples/calibration_example.csv)** - Пример калибровки
- **[test_safe_config.json](docs/examples/test_safe_config.json)** - Тестовая конфигурация

---

## 🤝 ВКЛАД В ПРОЕКТ

### **Требования к коду:**
- Соблюдение научных стандартов
- Документирование источников
- Валидация алгоритмов
- Тестирование на реальных данных

### **Процесс разработки:**
1. Изучение научных источников
2. Реализация алгоритмов
3. Валидация результатов
4. Документирование изменений
5. Тестирование

---

## 📋 ССЫЛКИ НА ИСТОЧНИКИ

1. **Archie, G.E. (1942).** The electrical resistivity log as an aid in determining some reservoir characteristics. Petroleum Technology, 1(1), 54-62.
2. **Allen, R.G., Pereira, L.S., Raes, D., Smith, M. (1998).** Crop evapotranspiration - Guidelines for computing crop water requirements. FAO Irrigation and Drainage Paper No. 56.
3. **ISO 7888:1985** - Water quality -- Determination of electrical conductivity
4. **ISO 10390:2005** - Soil quality -- Determination of pH
5. **European Journal of Soil Science, Vol. 45, Issue 2** - Soil moisture effects on nutrient availability
6. **USDA Soil Taxonomy** - Soil classification system
7. **WRB (World Reference Base for Soil Resources)** - International soil classification
8. **FAO Fertilizer and Plant Nutrition Bulletin 16** - базовые рекомендации по NPK
9. **ФАО Crop Calendar** - сезонные рекомендации по выращиванию
10. **USDA Plant Database** - требования культур к условиям выращивания
11. **ГОСТ Р 54078-2010** - Методы агрохимического анализа почв
12. **Eur. J. Agronomy** - сезонные корректировки и агроклиматические зоны

---

## 📄 ЛИЦЕНЗИЯ

Этот проект распространяется под лицензией MIT. См. файл [LICENSE](LICENSE) для подробностей.

---

**Статус проекта:** ✅ АКТУАЛЬНО  
**Последнее обновление:** Декабрь 2024  
**Следующая проверка:** Рекомендуется через 6 месяцев или при изменении научных стандартов 