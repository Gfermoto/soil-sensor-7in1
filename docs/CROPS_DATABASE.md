# База данных сельскохозяйственных культур

## Структура данных

Каждая культура описывается следующими параметрами:
```json
{
  "name": "Название культуры",
  "type": "vegetable|fruit|berry|grain|herb",
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
    "max_ppv": 80
  },
  "ph": {
    "min": 6.0,
    "optimal": 6.5,
    "max": 7.5
  },
  "ec": {
    "min": 0.8,
    "optimal": 1.3,
    "max": 1.8,
    "stages": {
      "vegetative": {"min": 0.8, "optimal": 1.2, "max": 1.6},
      "flowering": {"min": 1.0, "optimal": 1.4, "max": 1.8},
      "fruiting": {"min": 1.2, "optimal": 1.6, "max": 2.0}
    }
  },
  "npk": {
    "stages": {
      "vegetative": {"N": 4, "P": 1, "K": 3},
      "flowering": {"N": 3, "P": 1, "K": 4},
      "fruiting": {"N": 2, "P": 1, "K": 4}
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
    "max_ppv": 85
  },
  "ph": {
    "min": 5.5,
    "optimal": 6.3,
    "max": 6.8
  },
  "ec": {
    "min": 1.0,
    "optimal": 1.5,
    "max": 2.0,
    "stages": {
      "vegetative": {"min": 1.0, "optimal": 1.5, "max": 1.8},
      "flowering": {"min": 1.2, "optimal": 1.8, "max": 2.2},
      "fruiting": {"min": 1.5, "optimal": 2.0, "max": 2.5}
    }
  },
  "npk": {
    "stages": {
      "vegetative": {"N": 4, "P": 1, "K": 3},
      "flowering": {"N": 3, "P": 1, "K": 4},
      "fruiting": {"N": 2, "P": 1, "K": 4}
    }
  }
}
```

### Огурцы
```json
{
  "name": "Огурцы",
  "type": "vegetable",
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
    "max_ppv": 85
  },
  "ph": {
    "min": 6.0,
    "optimal": 6.5,
    "max": 7.0
  },
  "ec": {
    "min": 1.2,
    "optimal": 1.7,
    "max": 2.2,
    "stages": {
      "vegetative": {"min": 1.2, "optimal": 1.7, "max": 2.0},
      "flowering": {"min": 1.4, "optimal": 1.9, "max": 2.3},
      "fruiting": {"min": 1.6, "optimal": 2.1, "max": 2.5}
    }
  },
  "npk": {
    "stages": {
      "vegetative": {"N": 4, "P": 1, "K": 3},
      "flowering": {"N": 3, "P": 1, "K": 4},
      "fruiting": {"N": 2, "P": 1, "K": 4}
    }
  }
}
```

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