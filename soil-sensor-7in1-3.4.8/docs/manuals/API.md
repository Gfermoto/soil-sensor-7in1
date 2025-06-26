# 🔬 API JXCT Soil Sensor 7-in-1
## Научно обоснованные алгоритмы и интерфейсы

**Версия:** 3.6.0  
**Дата обновления:** Декабрь 2024  
**Статус:** ✅ ПОЛНОСТЬЮ ОБНОВЛЕНО СОГЛАСНО НАУЧНЫМ ИСТОЧНИКАМ

---

## 📚 НАУЧНЫЕ ИСТОЧНИКИ АЛГОРИТМОВ

### **Основные источники:**
- **Archie, G.E. (1942).** The electrical resistivity log as an aid in determining some reservoir characteristics. Petroleum Technology, 1(1), 54-62.
- **FAO Irrigation and Drainage Paper No. 56** - Allen, R.G., Pereira, L.S., Raes, D., Smith, M. (1998)
- **ISO 7888:1985** - Water quality -- Determination of electrical conductivity
- **ISO 10390:2005** - Soil quality -- Determination of pH
- **European Journal of Soil Science, Vol. 45, Issue 2**
- **USDA Soil Taxonomy** - Soil classification system
- **WRB (World Reference Base for Soil Resources)** - International soil classification

---

## 🌐 ВЕБ-ИНТЕРФЕЙС

### **Основные эндпоинты**

#### **GET /api/data**
Получение текущих показаний датчика с научно обоснованной компенсацией.

**Ответ:**
```json
{
  "timestamp": "2024-12-20T10:30:00Z",
  "temperature": 22.5,
  "humidity": 45.2,
  "ec": 1250.3,
  "ph": 6.8,
  "nitrogen": 850.7,
  "phosphorus": 420.3,
  "potassium": 680.9,
  "compensation_applied": {
    "ec_model": "Archie_1942",
    "ph_model": "Nernst_equation",
    "npk_model": "FAO_56_EurJSoilSci",
    "soil_type": "LOAM",
    "temperature_compensation": true,
    "humidity_compensation": true
  },
  "scientific_sources": [
    "Archie_1942",
    "FAO_56_1998",
    "ISO_7888_1985",
    "ISO_10390_2005",
    "EurJSoilSci_45_2"
  ]
}
```

#### **GET /api/compensation**
Получение информации о применённых алгоритмах компенсации.

**Ответ:**
```json
{
  "ec_compensation": {
    "model": "Archie_1942",
    "temperature_coefficient": 0.021,
    "soil_coefficients": {
      "SAND": {"k": 0.15, "theta_sat": 35.0},
      "LOAM": {"k": 0.30, "theta_sat": 45.0},
      "PEAT": {"k": 0.10, "theta_sat": 55.0},
      "CLAY": {"k": 0.45, "theta_sat": 50.0},
      "SANDPEAT": {"k": 0.18, "theta_sat": 40.0}
    },
    "source": "Archie, G.E. (1942). Petroleum Technology, 1(1), 54-62"
  },
  "ph_compensation": {
    "model": "Nernst_equation",
    "temperature_coefficient": -0.003,
    "source": "ISO 10390:2005"
  },
  "npk_compensation": {
    "model": "FAO_56_EurJSoilSci",
    "temperature_coefficients": {
      "SAND": {"N": 0.0041, "P": 0.0053, "K": 0.0032},
      "LOAM": {"N": 0.0038, "P": 0.0049, "K": 0.0029},
      "PEAT": {"N": 0.0028, "P": 0.0035, "K": 0.0021},
      "CLAY": {"N": 0.0032, "P": 0.0042, "K": 0.0024},
      "SANDPEAT": {"N": 0.0040, "P": 0.0051, "K": 0.0031}
    },
    "humidity_coefficients": {
      "SAND": {"N": 0.05, "P": 0.06, "K": 0.04},
      "LOAM": {"N": 0.04, "P": 0.05, "K": 0.03},
      "PEAT": {"N": 0.06, "P": 0.07, "K": 0.05},
      "CLAY": {"N": 0.03, "P": 0.04, "K": 0.02},
      "SANDPEAT": {"N": 0.05, "P": 0.06, "K": 0.04}
    },
    "sources": [
      "FAO Irrigation and Drainage Paper No. 56",
      "European Journal of Soil Science, Vol. 45, Issue 2"
    ]
  }
}
```

#### **POST /api/config**
Настройка параметров датчика и алгоритмов компенсации.

**Запрос:**
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

**Ответ:**
```json
{
  "status": "success",
  "config_applied": {
    "soil_type": "LOAM",
    "growing_condition": "OUTDOOR",
    "crop_type": "TOMATO",
    "season": "SUMMER",
    "compensation_coefficients": {
      "ec_k": 0.30,
      "ec_theta_sat": 45.0,
      "npk_temp_N": 0.0038,
      "npk_temp_P": 0.0049,
      "npk_temp_K": 0.0029,
      "npk_hum_N": 0.04,
      "npk_hum_P": 0.05,
      "npk_hum_K": 0.03
    },
    "seasonal_adjustments": {
      "N": -0.10,
      "P": 0.05,
      "K": 0.25
    }
  }
}
```

---

## 🔬 АЛГОРИТМЫ КОМПЕНСАЦИИ

### **Электропроводность (EC) - Модель Арчи (1942)**

#### **Источник:**
Archie, G.E. (1942). The electrical resistivity log as an aid in determining some reservoir characteristics. Petroleum Technology, 1(1), 54-62.

#### **Формула:**
```cpp
// Шаг 1: Температурная компенсация к 25°C (ISO 7888)
EC_25 = EC_raw / (1.0 + 0.021 × (T - 25°C))

// Шаг 2: Модель Арчи для пересчёта к ECe (насыщенная паста)
EC_corrected = EC_25 × (θ_sat/θ)^k
```

#### **Коэффициенты по типам почв:**
| Тип почвы   | k (Арчи) | θ_sat (%) | Источник |
|-------------|----------|-----------|----------|
| SAND        | 0.15     | 35.0      | USDA Soil Taxonomy |
| LOAM        | 0.30     | 45.0      | USDA Soil Taxonomy |
| PEAT        | 0.10     | 55.0      | WRB classification |
| CLAY        | 0.45     | 50.0      | USDA Soil Taxonomy |
| SANDPEAT    | 0.18     | 40.0      | WRB classification |

### **pH - Уравнение Нернста**

#### **Источник:**
ISO 10390:2005 - Soil quality -- Determination of pH

#### **Формула:**
```cpp
// Уравнение Нернста: температурная зависимость электродного потенциала
pH_corrected = pH_raw - 0.003 × (T - 25°C)
```

### **NPK - FAO 56 + Eur. J. Soil Sci.**

#### **Источники:**
- **FAO Irrigation and Drainage Paper No. 56** - Allen, R.G., Pereira, L.S., Raes, D., Smith, M. (1998)
- **European Journal of Soil Science, Vol. 45, Issue 2**

#### **Формула:**
```cpp
// Шаг 1: Температурная компенсация (FAO 56)
NPK_temp = NPK_raw × (1.0 - k_t × (T - 25°C))

// Шаг 2: Влажностная компенсация (Eur. J. Soil Sci.)
θ_norm = (θ - 25%) / (60% - 25%)  // Нормализация к диапазону 25-60%
NPK_corrected = NPK_temp × (1.0 + k_h × θ_norm)
```

---

## 🌱 АГРОНОМИЧЕСКИЕ РЕКОМЕНДАЦИИ

### **GET /api/recommendations**
Получение научно обоснованных агрономических рекомендаций.

**Параметры запроса:**
- `crop_type` - тип культуры
- `soil_type` - тип почвы
- `growing_condition` - условия выращивания
- `season` - сезон

**Ответ:**
```json
{
  "crop_requirements": {
    "nitrogen": {
      "optimal": 1400,
      "unit": "mg/kg",
      "source": "FAO Fertilizer and Plant Nutrition Bulletin 16"
    },
    "phosphorus": {
      "optimal": 500,
      "unit": "mg/kg",
      "source": "FAO Fertilizer and Plant Nutrition Bulletin 16"
    },
    "potassium": {
      "optimal": 1200,
      "unit": "mg/kg",
      "source": "FAO Fertilizer and Plant Nutrition Bulletin 16"
    },
    "ph": {
      "optimal": 6.5,
      "range": [6.0, 7.0],
      "source": "USDA Plant Database"
    },
    "ec": {
      "optimal": 1500,
      "unit": "µS/cm",
      "range": [1200, 1800],
      "source": "FAO Irrigation and Drainage Paper No. 56"
    }
  },
  "soil_corrections": {
    "humidity": 5,
    "ec": 0,
    "nitrogen": 0,
    "phosphorus": 0,
    "source": "USDA Soil Taxonomy"
  },
  "seasonal_adjustments": {
    "nitrogen": -0.10,
    "phosphorus": 0.05,
    "potassium": 0.25,
    "source": "Eur. J. Agronomy"
  },
  "growing_condition_adjustments": {
    "humidity": 0,
    "ec": 0,
    "nitrogen": 0,
    "phosphorus": 0,
    "potassium": 0,
    "source": "FAO Crop Calendar"
  },
  "recommendations": [
    {
      "type": "fertilization",
      "priority": "high",
      "description": "Внести азотные удобрения для поддержания вегетативного роста",
      "amount": "150 mg/kg N",
      "timing": "within 7 days",
      "scientific_basis": "FAO Fertilizer and Plant Nutrition Bulletin 16"
    },
    {
      "type": "irrigation",
      "priority": "medium",
      "description": "Поддерживать влажность почвы в диапазоне 40-50%",
      "frequency": "daily",
      "scientific_basis": "FAO Irrigation and Drainage Paper No. 56"
    }
  ]
}
```

---

## 📊 ВАЛИДАЦИЯ И ОГРАНИЧЕНИЯ

### **Физические пределы:**
- **Температура:** -50°C до +100°C (валидация входных данных)
- **Влажность:** 1-100% (валидация входных данных)
- **EC:** 0-10000 µS/cm (максимум по спецификации JXCT)
- **pH:** 0-14 (физические пределы)
- **NPK:** 0-2000 мг/кг (максимум по спецификации JXCT)

### **GET /api/validation**
Проверка валидности данных и применяемых алгоритмов.

**Ответ:**
```json
{
  "input_validation": {
    "temperature": {
      "value": 22.5,
      "valid": true,
      "range": [-50, 100]
    },
    "humidity": {
      "value": 45.2,
      "valid": true,
      "range": [1, 100]
    },
    "ec": {
      "value": 1250.3,
      "valid": true,
      "range": [0, 10000]
    },
    "ph": {
      "value": 6.8,
      "valid": true,
      "range": [0, 14]
    },
    "nitrogen": {
      "value": 850.7,
      "valid": true,
      "range": [0, 2000]
    },
    "phosphorus": {
      "value": 420.3,
      "valid": true,
      "range": [0, 2000]
    },
    "potassium": {
      "value": 680.9,
      "valid": true,
      "range": [0, 2000]
    }
  },
  "algorithm_validation": {
    "ec_compensation": {
      "model": "Archie_1942",
      "valid": true,
      "coefficients_valid": true
    },
    "ph_compensation": {
      "model": "Nernst_equation",
      "valid": true,
      "coefficients_valid": true
    },
    "npk_compensation": {
      "model": "FAO_56_EurJSoilSci",
      "valid": true,
      "coefficients_valid": true
    }
  }
}
```

---

## 🔧 КОНФИГУРАЦИЯ

### **GET /api/config/current**
Получение текущей конфигурации.

**Ответ:**
```json
{
  "device_info": {
    "model": "JXCT Soil Sensor 7-in-1",
    "version": "3.6.0",
    "firmware": "v3.6.0-scientific",
    "serial_number": "JXCT-2024-001"
  },
  "sensor_config": {
    "soil_type": "LOAM",
    "growing_condition": "OUTDOOR",
    "crop_type": "TOMATO",
    "season": "SUMMER",
    "compensation_enabled": true,
    "scientific_validation": true
  },
  "algorithm_config": {
    "ec_model": "Archie_1942",
    "ph_model": "Nernst_equation",
    "npk_model": "FAO_56_EurJSoilSci",
    "validation_enabled": true,
    "debug_mode": false
  },
  "publishing_config": {
    "mqtt_enabled": true,
    "thingspeak_enabled": true,
    "publish_interval": 300,
    "publish_raw_data": false,
    "publish_compensated_data": true
  }
}
```

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

---

**Статус документа:** ✅ АКТУАЛЬНО  
**Последнее обновление:** Декабрь 2024  
**Следующая проверка:** Рекомендуется через 6 месяцев или при изменении научных стандартов 