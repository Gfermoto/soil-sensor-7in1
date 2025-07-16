# 🚀 Быстрые команды JXCT

## 📋 Ежедневные операции

### 🔍 Анализ кода (ОБЯЗАТЕЛЬНО перед изменениями)
```bash
# Полный анализ clang-tidy
python scripts/run_clang_tidy_analysis.py

# Быстрая проверка cppcheck
pio check -e static-analysis
```

### 🧪 Тестирование
```bash
# Ключевые тесты (быстро)
python -m pytest test/test_validation.py test/test_compensation_formulas.py

# Полные тесты
python scripts/run_simple_tests.py

# Native тесты C++
pio test -e native
```

### 🔨 Сборка
```bash
# Основная сборка
pio run -e esp32dev

# Production сборка
pio run -e esp32dev-production
```

### 📝 Версионирование
```bash
# Обновить версию
python scripts/auto_version.py

# Синхронизировать версии
python scripts/sync_versions.py
```

## 🎯 Рабочий процесс

### 1. Перед изменениями:
```bash
python scripts/run_clang_tidy_analysis.py
python -m pytest test/test_validation.py
pio run -e esp32dev
```

### 2. После изменений:
```bash
python scripts/auto_version.py
python scripts/run_simple_tests.py
pio run -e esp32dev
```

### 3. Перед коммитом:
```bash
git add .
git commit -m "feat: описание изменений"
git push
```

## 🔧 Устранение проблем

### Если сборка не работает:
```bash
pio run -t clean
pio run -e esp32dev
```

### Если тесты падают:
```bash
python -m pytest test/test_validation.py -v
python scripts/run_simple_tests.py
```

### Если clang-tidy не работает:
```bash
python scripts/run_clang_tidy_analysis.py -v
```

## 📊 Мониторинг

### Проверка статуса:
```bash
# Версия проекта
cat VERSION

# Статус git
git status

# Последние коммиты
git log --oneline -5
```

### Анализ отчётов:
```bash
# Clang-tidy отчёт
cat test_reports/clang_tidy_analysis_report.md

# Тестовые отчёты
ls test_reports/
```

## 🎯 CodeRabbit анализ

### При анализе кода всегда проверять:
1. **Безопасность** - уязвимости, переполнения
2. **Производительность** - неэффективные алгоритмы  
3. **Читаемость** - сложный код, плохие имена
4. **Совместимость** - проблемы с ESP32
5. **Стиль** - соответствие C++17
6. **Логика** - потенциальные баги

### Пример анализа:
```cpp
// Код для анализа
void processSensorData(float value, float min, float max) {
    if (value < min || value > max) {
        return false;
    }
    // обработка...
}

// Мой анализ как CodeRabbit:
🔍 АНАЛИЗ КОДА:
⚠️ ПРОБЛЕМЫ:
1. Возвращаемый тип void, но return false
2. Параметры min/max легко перепутать
3. Нет проверки на NaN/inf

✅ РЕКОМЕНДАЦИИ:
1. Изменить тип на bool
2. Использовать именованные параметры
3. Добавить валидацию входных данных
```

## 🚨 Критические правила

### НЕ ДЕЛАТЬ:
- ❌ Изменять код без анализа
- ❌ Игнорировать предупреждения clang-tidy
- ❌ Коммитить без тестирования
- ❌ Забывать обновлять версию

### ОБЯЗАТЕЛЬНО:
- ✅ Анализ перед изменениями
- ✅ Тестирование после изменений
- ✅ Проверка сборки
- ✅ Обновление версии
- ✅ CodeRabbit анализ PR 