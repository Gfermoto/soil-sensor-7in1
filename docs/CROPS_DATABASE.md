# База данных сельскохозяйственных культур

## Структура данных

Каждая культура описывается следующими параметрами:
```json
{
  "name": "Название культуры",
  "type": "vegetable|fruit|berry|grain|herb",
  "environment_type": "outdoor|greenhouse|both",
  "soil_types": ["sand", "loam", "clay", "peat"],
  "stages": ["germination", "vegetative", "flowering", "fruiting"],
  "temperature": {
    "min": 10,
    "optimal": 18,
    "max": 30,
    "stages": {
      "germination": {"min": 12, "optimal": 20, "max": 25},
      "vegetative": {"min": 15, "optimal": 22, "max": 28},
      "flowering": {"min": 18, "optimal": 24, "max": 30},
      "fruiting": {"min": 16, "optimal": 23, "max": 28}
    }
  },
  "moisture": {
    "min_ppv": 60,
    "optimal_ppv": 70,
    "max_ppv": 80,
    "soil_type_adjustments": {
      "sand": {"min": -5, "optimal": -5, "max": -5},
      "clay": {"min": +5, "optimal": +5, "max": +5},
      "peat": {"min": +10, "optimal": +10, "max": +10}
    }
  },
  "ph": {
    "min": 5.5,
    "optimal": 6.3,
    "max": 7.0,
    "sensor_limits": {
      "absolute_min": 3.0,
      "absolute_max": 9.0,
      "accuracy": 0.3
    }
  },
  "ec": {
    "stages": {
      "germination": {"min": 500, "optimal": 800, "max": 1200},
      "vegetative": {"min": 1000, "optimal": 1500, "max": 2000},
      "flowering": {"min": 1200, "optimal": 1800, "max": 2200},
      "fruiting": {"min": 1500, "optimal": 2000, "max": 2500}
    },
    "sensor_limits": {
      "absolute_min": 0,
      "absolute_max": 10000,
      "resolution": 10
    }
  },
  "npk": {
    "stages": {
      "germination": {
        "N": {"min": 100, "optimal": 150, "max": 200},
        "P": {"min": 50, "optimal": 75, "max": 100},
        "K": {"min": 50, "optimal": 75, "max": 100}
      },
      "vegetative": {
        "N": {"min": 150, "optimal": 200, "max": 250},
        "P": {"min": 75, "optimal": 100, "max": 125},
        "K": {"min": 100, "optimal": 150, "max": 200}
      },
      "flowering": {
        "N": {"min": 125, "optimal": 175, "max": 225},
        "P": {"min": 100, "optimal": 150, "max": 200},
        "K": {"min": 150, "optimal": 200, "max": 250}
      },
      "fruiting": {
        "N": {"min": 100, "optimal": 150, "max": 200},
        "P": {"min": 125, "optimal": 175, "max": 225},
        "K": {"min": 175, "optimal": 225, "max": 275}
      }
    },
    "sensor_limits": {
      "absolute_min": 0,
      "absolute_max": 1999,
      "accuracy": "2%"
    }
  }
}
```

## Овощные культуры

### Томаты
```json
{
  "name": "Томаты",
  "type": "vegetable",
  "environment_type": "both",
  "soil_types": ["loam", "clay"],
  "stages": ["germination", "vegetative", "flowering", "fruiting"],
  "temperature": {
    "min": 15,
    "optimal": 22,
    "max": 30,
    "stages": {
      "germination": {"min": 20, "optimal": 25, "max": 30},
      "vegetative": {"min": 18, "optimal": 24, "max": 28},
      "flowering": {"min": 18, "optimal": 23, "max": 27},
      "fruiting": {"min": 17, "optimal": 22, "max": 26}
    }
  },
  "moisture": {
    "min_ppv": 65,
    "optimal_ppv": 75,
    "max_ppv": 85,
    "soil_type_adjustments": {
      "sand": {"min": -5, "optimal": -5, "max": -5},
      "clay": {"min": +5, "optimal": +5, "max": +5},
      "peat": {"min": +10, "optimal": +10, "max": +10}
    }
  },
  "ph": {
    "min": 5.5,
    "optimal": 6.3,
    "max": 6.8
  },
  "ec": {
    "stages": {
      "germination": {"min": 0.5, "optimal": 0.8, "max": 1.2},
      "vegetative": {"min": 1.0, "optimal": 1.5, "max": 1.8},
      "flowering": {"min": 1.2, "optimal": 1.8, "max": 2.2},
      "fruiting": {"min": 1.5, "optimal": 2.0, "max": 2.5}
    }
  }
}
```

### Огурцы
```json
{
  "name": "Огурцы",
  "type": "vegetable",
  "environment_type": "both",
  "soil_types": ["loam", "sand"],
  "stages": ["germination", "vegetative", "flowering", "fruiting"],
  "temperature": {
    "min": 18,
    "optimal": 24,
    "max": 32,
    "stages": {
      "germination": {"min": 22, "optimal": 28, "max": 32},
      "vegetative": {"min": 20, "optimal": 25, "max": 30},
      "flowering": {"min": 20, "optimal": 24, "max": 28},
      "fruiting": {"min": 19, "optimal": 23, "max": 27}
    }
  },
  "moisture": {
    "min_ppv": 70,
    "optimal_ppv": 80,
    "max_ppv": 85,
    "soil_type_adjustments": {
      "sand": {"min": -5, "optimal": -5, "max": -5},
      "clay": {"min": +5, "optimal": +5, "max": +5},
      "peat": {"min": +10, "optimal": +10, "max": +10}
    }
  },
  "ph": {
    "min": 5.5,
    "optimal": 6.5,
    "max": 7.0
  },
  "ec": {
    "stages": {
      "germination": {"min": 0.5, "optimal": 0.8, "max": 1.2},
      "vegetative": {"min": 1.2, "optimal": 1.7, "max": 2.0},
      "flowering": {"min": 1.4, "optimal": 1.9, "max": 2.3},
      "fruiting": {"min": 1.6, "optimal": 2.1, "max": 2.5}
    }
  }
}
```

## Типы почв и их характеристики

### Песчаные почвы (sand)
- Быстро прогреваются
- Хорошая аэрация
- Низкая влагоёмкость
- Быстрое вымывание питательных веществ
- Требуют частого полива и подкормок

### Суглинистые почвы (loam)
- Оптимальный водно-воздушный режим
- Хорошая удержимость питательных веществ
- Средняя скорость прогрева
- Универсальны для большинства культур

### Глинистые почвы (clay)
- Медленно прогреваются
- Высокая влагоёмкость
- Склонность к уплотнению
- Требуют улучшения структуры
- Риск закисания

### Торфяные почвы (peat)
- Высокая влагоёмкость
- Низкая теплопроводность
- Богаты органикой
- Требуют известкования
- Риск пересыхания верхнего слоя

[Продолжение следует... Будут добавлены другие культуры]

## Использование базы данных

1. База данных хранится в формате JSON
2. Для каждой культуры определены оптимальные диапазоны всех измеряемых параметров
3. Учитываются стадии развития растения
4. Все значения основаны на научных исследованиях и практическом опыте

## Обновление базы

1. Регулярное обновление значений на основе новых исследований
2. Добавление новых культур
3. Уточнение параметров на основе обратной связи от пользователей
4. Версионирование базы данных для поддержки обратной совместимости 