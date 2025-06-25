# 🧪 Руководство по тестированию JXCT

## Обзор

Проект JXCT оснащён комплексной системой автоматического тестирования с генерацией отчётов и анализом технического долга.

## Структура тестирования

### 📁 Файлы и директории

```
test/
├── test_simple.cpp                    # Упрощённые тесты (активные)
├── test_comprehensive.cpp.bak         # Комплексные тесты (в разработке)
├── test_validation_utils.cpp.bak      # Старые тесты (backup)
├── test_framework_config.hpp          # Конфигурация фреймворка
├── test_report_generator.hpp          # Генератор отчётов (заголовки)
├── test_report_generator.cpp          # Генератор отчётов (реализация)
└── stubs/                             # Заглушки для тестирования
    ├── Arduino.h
    ├── logger.cpp
    └── Preferences.h

test_reports/                          # Директория отчётов
├── simple-test-report.json           # Простой отчёт тестов
├── technical-debt.json               # Анализ технического долга
├── comprehensive-report.json         # Комплексный отчёт
└── comprehensive-report.html         # HTML отчёт

scripts/
├── analyze_technical_debt.py         # Анализатор технического долга
└── run_comprehensive_tests.py        # Комплексное тестирование
```

## 🚀 Быстрый старт

### 1. Запуск базовых тестов

```bash
# Unit тесты
pio test -e native -v

# Тесты с покрытием кода
pio test -e native-coverage -v

# Комплексные тесты
pio test -e native-comprehensive -v
```

### 2. Анализ технического долга

```bash
python scripts/analyze_technical_debt.py
```

### 3. Комплексное тестирование

```bash
python scripts/run_comprehensive_tests.py
```

## 📊 Типы тестов

### Unit тесты
- **Валидация данных**: SSID, пароли, температура
- **Компенсация датчиков**: NPK, EC, pH
- **Утилиты форматирования**: обработка данных

### Integration тесты
- **Цепочки обработки**: измерение → компенсация → валидация
- **Взаимодействие компонентов**: датчики + обработка

### Performance тесты
- **Скорость валидации**: < 100мс на 1000 операций
- **Скорость компенсации**: < 50мс на 500 операций

### Security тесты
- **Небезопасные функции**: strcpy, sprintf, system
- **Хардкод секретов**: пароли, ключи

## 📈 Метрики качества

### Покрытие кода
- **Текущее**: ~70.8%
- **Цель**: 90%+
- **Инструменты**: gcov, lcov

### Технический долг
- **Code smells**: 66 (цель: <20)
- **Дублированные строки**: 933 (цель: <100)
- **Сложность**: 6 функций (цель: <3)
- **Уязвимости**: 134 (цель: 0)

### Рейтинги
- **Поддерживаемость**: D → A
- **Надёжность**: C → A
- **Безопасность**: D → A

## 🔧 Конфигурация

### PlatformIO окружения

```ini
[env:native]                    # Базовые тесты
[env:native-comprehensive]      # Комплексные тесты  
[env:native-coverage]          # Тесты с покрытием
[env:esp32dev]                 # Тесты на железе
```

### Настройки отчётов

```cpp
// test_framework_config.hpp
struct ReportConfig {
    bool generateXML = true;
    bool generateHTML = true; 
    bool generateJSON = true;
    bool includeTechnicalDebt = true;
    std::string outputDir = "test_reports";
};
```

## 📋 Отчёты

### JSON отчёт
```json
{
  "timestamp": "2025-01-22T12:00:00Z",
  "project": "JXCT Soil Sensor", 
  "version": "3.6.0",
  "summary": {
    "total_tests": 13,
    "passed_tests": 13,
    "success_rate": 100.0
  }
}
```

### HTML отчёт
- Интерактивные графики
- Детали по каждому тесту
- Рекомендации по улучшению

## 🎯 CI/CD интеграция

### GitHub Actions

```yaml
# .github/workflows/comprehensive-testing.yml
name: 🧪 Comprehensive Testing & Reports

on:
  push:
    branches: [ main, develop ]
  schedule:
    - cron: '0 2 * * *'  # Ежедневно в 2:00 UTC
```

### Артефакты
- `technical-debt-report`
- `test-reports-unit`
- `test-reports-integration` 
- `coverage-report`
- `comprehensive-test-report`

## 📚 Примеры использования

### Добавление нового теста

```cpp
void test_new_feature() {
    auto result = validateNewFeature("test_data");
    TEST_ASSERT_TRUE(result.isValid);
    g_stats.addResult(result.isValid);
}

// Регистрация в main()
RUN_TEST(test_new_feature);
```

### Создание теста производительности

```cpp
void test_performance_new_feature() {
#ifndef ARDUINO
    auto start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < 1000; ++i) {
        processNewFeature(test_data[i]);
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    TEST_ASSERT_TRUE(duration.count() < 100);  // < 100мс
#endif
}
```

## 🔍 Анализ проблем

### Высокий технический долг
1. **Code smells** → рефакторинг длинных функций
2. **Дублирование** → выделение общих функций
3. **Сложность** → разбиение на части
4. **Уязвимости** → замена небезопасных функций

### Низкое покрытие
1. Добавить тесты для непокрытых функций
2. Тестировать граничные случаи
3. Проверить обработку ошибок

### Медленные тесты
1. Оптимизировать алгоритмы
2. Использовать моки для внешних зависимостей
3. Параллелизация тестов

## 🚀 Планы развития

### Этап 2: Интеграция с сайтом
- [ ] Автоматическое обновление страницы тестов
- [ ] API для получения метрик
- [ ] Дашборд качества кода

### Этап 3: Уменьшение технического долга
- [ ] План рефакторинга на основе отчётов
- [ ] Автоматические исправления
- [ ] Контроль качества в CI/CD

### Этап 4: Расширенная аналитика
- [ ] Тренды качества кода
- [ ] Прогнозирование проблем
- [ ] Интеграция с SonarQube

## 📞 Поддержка

При возникновении проблем:

1. Проверьте логи в `test_reports/`
2. Запустите `pio test -vvv` для детальной диагностики
3. Убедитесь в наличии всех зависимостей
4. Создайте issue с описанием проблемы

---

**Автор**: EYERA Development Team  
**Версия**: 1.0.0  
**Дата**: 2025-01-22 