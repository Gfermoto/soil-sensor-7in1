# Руководство JXCT по агро-профилю, компенсации показаний и системе рекомендаций

> Версия: v2.6.0 (draft, 2025-06-16)
> 
> Документ объединяет материалы из:
> 1. `CROPS_DATABASE.md`
> 2. `SENSOR_COMPENSATION.md`
> 3. `RECOMMENDATIONS_SYSTEM.md`
>
> **Ключевые дополнения**:
> • Добавлена среда выращивания «indoor» (помещение) наряду с `outdoor` и `greenhouse`  
> • Учёт сезонных коэффициентов вынесен в отдельный модуль; пользователь может отключить их через чекбокс «Seasonal Adjust» в UI настроек  
> • Все формулы приведены к единому стилю; ссылки между разделами скорректированы

---

## 1. Структура агро-профиля

```json
{
  "latitude": 55.7558,
  "longitude": 37.6173,
  "environment_type": "outdoor | greenhouse | indoor",
  "cropId": "tomato",
  "soil_type": "loam",
  "stage": "vegetative",
  "isGreenhouse": false,       // <- устарело, заменено на environment_type
  "seasonalAdjustEnabled": true,
  "irrigationSpikeThreshold": 15,
  "irrigationHoldMinutes": 5,
  "postIrrigationHoldMinutes": 180
}
```

*Переход с флага `isGreenhouse`*: при миграции в прошивке автоматически выполняется:

```cpp
if (oldConfig.flags.isGreenhouse) {
    config.environment_type = "greenhouse";
}
```

---

## 2. База данных культур (сокращённо)

Раздел подробно описывает JSON-схему каждой культуры. Ниже приведён фрагмент с обновлённым перечислением `environment_type`:

```jsonc
{
  "name": "Томаты",
  "type": "vegetable",
  "environment_type": "both | indoor",  // новое значение
  "soil_types": ["loam", "clay"],
  // ... остальное без изменений ...
}
```

Полная база вынесена в `data/crops_v1.json`; структура и требования см. в Приложении А.

---

## 3. Компенсация показаний сенсора

### 3.1 Выбор профиля по среде

| environment_type | Профиль компенсации |
|------------------|----------------------|
| outdoor          | `field_correction()` |
| greenhouse       | `greenhouse_correction()` |
| indoor           | `indoor_correction()` |

Функция `indoor_correction()` наследует коэффициенты «теплица», но ослабляет влажностную коррекцию (θ) и убирает засоление >3 000 µS/cm, т.к. в помещении субстрат контролируемый.

### 3.2 Сезонные коэффициенты

Сезонность влияет на:
• температурный коэффициент EC (`k_ec_temp`)  
• влажностную коррекцию pH / NPK (`k_humidity_factor`)

Набор коэффициентов хранится в `SeasonalTable[12]`. При отключении чекбокса «Seasonal Adjust» пользователь устанавливает флаг `config.flags.seasonalAdjustEnabled = false`, что эквивалентно `k_humidity_factor = 1.0` и выбору «сухого периода» для `k_ec_temp`.

```cpp
float k_ec_temp = config.flags.seasonalAdjustEnabled ? SeasonalTable[month].k_ec_temp
                                                    : DEFAULT_K_EC_TEMP;
```

Подробное математическое описание см. в Приложении B.

---

## 4. Система рекомендаций

Алгоритм анализа и генерации рекомендаций сохраняется, но принимает во внимание новое поле `environment_type` и состояние флага `seasonalAdjustEnabled`.

```python
optimal = get_optimal_range(crop, param, stage, environment_type)
if not config.seasonalAdjustEnabled:
    optimal = remove_seasonal_bias(optimal)
```

Результат выводится в таблице `/readings` в новом столбце «Реком.».

---

## 5. Изменения в UI

1. Страница настроек (`/`):
   • Переключатель «Среда выращивания»: `Outdoor`, `Greenhouse`, `Indoor`  
   • Чекбокс «Seasonal Adjust» (по умолчанию включён)
2. Таблица показаний (`/readings`):
   • Бейдж 💦 («Недавний полив») перемещён над таблицей  
   • 4-й столбец «Реком.» отображает расчётные оптимумы

---

## 6. API

`GET /api/v1/recommendations` принимает новые query-параметры:

```
?env=outdoor|greenhouse|indoor&seasonal=0|1
```

---

## 7. Приложения

• **Приложение А** – Полная JSON-схема `crops_v1.json`  
• **Приложение B** – Таблицы сезонных коэффициентов и derivation  
• **Приложение C** – Псевдокод indoor-профиля компенсации

---

© JXCT, 2025 