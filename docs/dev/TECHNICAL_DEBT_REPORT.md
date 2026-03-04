# Технический долг

**Инструмент:** clang-tidy  
**Состояние:** ~125–169 предупреждений

## Категории

### 1. Потенциальные баги (приоритет 1)

- `bugprone-easily-swappable-parameters` — соседние параметры одного типа
- `bugprone-narrowing-conversions` — narrowing int→float
- `bugprone-integer-division` — целочисленное деление

**Файлы:** advanced_filters.cpp, modbus_sensor.cpp, validation_utils.cpp

### 2. Читаемость (приоритет 2)

- `readability-convert-member-functions-to-static` — методы без состояния
- `readability-identifier-length` — короткие имена (i, j, x)
- `readability-else-after-return` — лишние else

### 3. Модернизация (приоритет 3)

- `modernize-use-nodiscard` — атрибут [[nodiscard]]
- `modernize-raw-string-literal` — R"(...)"
- `modernize-return-braced-init-list` — braced init

### 4. Разное (приоритет 4)

- `misc-const-correctness` — добавить const
- `misc-use-internal-linkage` — static / anonymous namespace

## Проблемные файлы

1. crop_recommendation_engine.cpp — 19 предупреждений
2. modbus_sensor.cpp — 9
3. web_templates.cpp — неиспользуемые функции
4. validation_utils.cpp — 3
5. advanced_filters.cpp — 1

## План

1. Критические (1–2 дня): 19 потенциальных багов
2. Читаемость (2–3 дня): 74 проблемы
3. Модернизация (1–2 дня): C++17
4. Очистка (1 день): const, linkage, неиспользуемое

## Команда

```bash
python scripts/run_clang_tidy_analysis.py
```
