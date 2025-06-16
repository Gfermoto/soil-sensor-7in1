# Система рекомендаций JXCT

## Общие принципы

1. **Цель системы:** Предоставление точных и практических рекомендаций по уходу за растениями на основе показаний датчиков
2. **Источники данных:**
   - Текущие показания датчиков
   - База данных культур
   - История измерений
   - Стадия развития растения
3. **Типы рекомендаций:**
   - Срочные (требуют немедленного внимания)
   - Плановые (для оптимизации условий)
   - Профилактические (предупреждение проблем)

## Алгоритм анализа

### 1. Сбор данных
```python
def collect_data():
    return {
        "current": get_current_readings(),
        "history": get_readings_history(hours=24),
        "crop": get_crop_parameters(),
        "stage": get_growth_stage()
    }
```

### 2. Анализ отклонений
```python
def analyze_deviations(data):
    deviations = []
    
    # Проверка каждого параметра
    for param in ["temperature", "moisture", "ph", "ec", "npk"]:
        current = data["current"][param]
        optimal = data["crop"][param]["stages"][data["stage"]]
        
        if current < optimal["min"]:
            deviations.append({
                "param": param,
                "type": "low",
                "value": current,
                "target": optimal["optimal"],
                "severity": calculate_severity(current, optimal)
            })
        elif current > optimal["max"]:
            deviations.append({
                "param": param,
                "type": "high",
                "value": current,
                "target": optimal["optimal"],
                "severity": calculate_severity(current, optimal)
            })
    
    return deviations
```

### 3. Генерация рекомендаций
```python
def generate_recommendations(deviations):
    recommendations = []
    
    for dev in deviations:
        if dev["param"] == "moisture" and dev["type"] == "low":
            recommendations.append({
                "type": "urgent",
                "action": "water",
                "details": f"Полейте растения. Текущая влажность {dev['value']}%, целевая {dev['target']}%",
                "severity": dev["severity"]
            })
        # Аналогичные правила для других параметров
    
    return sorted(recommendations, key=lambda x: x["severity"], reverse=True)
```

## Визуализация

### 1. Цветовая индикация
- 🟢 В пределах нормы
- 🟡 Легкое отклонение
- 🟠 Среднее отклонение
- 🔴 Критическое отклонение

### 2. Графики
```javascript
function renderChart(data, optimal) {
    return {
        type: 'line',
        data: {
            datasets: [{
                label: 'Текущие показания',
                data: data,
                borderColor: 'blue'
            }, {
                label: 'Оптимальный диапазон',
                data: optimal,
                backgroundColor: 'rgba(0, 255, 0, 0.2)'
            }]
        },
        options: {
            plugins: {
                annotation: {
                    annotations: {
                        box1: {
                            type: 'box',
                            yMin: optimal.min,
                            yMax: optimal.max,
                            backgroundColor: 'rgba(0, 255, 0, 0.1)'
                        }
                    }
                }
            }
        }
    }
}
```

## Форматы рекомендаций

### 1. Срочные рекомендации
```json
{
    "type": "urgent",
    "title": "Низкая влажность почвы",
    "description": "Требуется полив",
    "current_value": 45,
    "optimal_range": {
        "min": 60,
        "optimal": 70,
        "max": 80
    },
    "action": {
        "type": "watering",
        "amount": "2л/м²",
        "urgency": "immediate"
    }
}
```

### 2. Плановые рекомендации
```json
{
    "type": "planned",
    "title": "Оптимизация pH",
    "description": "Рекомендуется постепенное снижение pH",
    "current_value": 7.8,
    "optimal_range": {
        "min": 6.0,
        "optimal": 6.5,
        "max": 7.0
    },
    "action": {
        "type": "ph_adjustment",
        "method": "add_sulfur",
        "amount": "30г/м²",
        "period": "7 дней"
    }
}
```

## API Endpoints

### 1. Получение рекомендаций
```http
GET /api/v1/recommendations
Query parameters:
  - crop_id: string
  - growth_stage: string
  - readings: object
```

### 2. Подтверждение выполнения
```http
POST /api/v1/recommendations/{id}/complete
Body:
  - action_taken: string
  - notes: string (optional)
```

## Кэширование и производительность

1. **Локальное кэширование:**
   - База данных культур
   - История рекомендаций
   - Шаблоны рекомендаций

2. **Периодичность обновления:**
   - Показания датчиков: каждые 5 минут
   - Анализ трендов: каждый час
   - Генерация рекомендаций: при изменении показаний
   
3. **Приоритизация:**
   - Критические отклонения: немедленное уведомление
   - Плановые рекомендации: раз в сутки
   - Профилактические советы: еженедельно

## Расширение системы

1. **Добавление новых параметров:**
   - Освещенность
   - Содержание CO₂
   - Микроэлементы

2. **Интеграция с внешними системами:**
   - Метеоданные
   - Системы полива
   - Календарь агротехнических мероприятий

3. **Машинное обучение:**
   - Прогнозирование трендов
   - Оптимизация рекомендаций
   - Анализ эффективности мероприятий 