# Руководство по тестированию JXCT

**Версия:** 3.6.0
**Дата обновления:** 3 июля 2025

## 📋 Обзор

JXCT использует многоуровневую систему тестирования для обеспечения качества кода и надёжности системы мониторинга почвы.

## 🧪 Типы тестов

### 1. Unit тесты
- **Назначение:** Тестирование отдельных функций и классов
- **Покрытие:** 70.8% строк, 75.0% функций
- **Запуск:** `pio test -e native -v`

### 2. Интеграционные тесты
- **Назначение:** Тестирование взаимодействия компонентов
- **Файлы:** `test/integration/test_component_interaction.cpp`
- **Запуск:** `python scripts/run_comprehensive_tests.py`

### 3. E2E тесты
- **Назначение:** Тестирование веб-интерфейса
- **Файлы:** `test/e2e/test_web_ui.py`
- **Запуск:** `python test/e2e/test_web_ui.py`

### 4. Тесты производительности
- **Назначение:** Измерение производительности критических функций
- **Метрики:** Валидация < 100ms, Компенсация < 50ms
- **Запуск:** `python scripts/run_performance_tests.py`

## 🔧 Автоматизация

### Pre-commit система
```bash
# Автоматический запуск при коммите
git commit -m "Your message"

# Ручной запуск
python scripts/pre-commit.py --verbose
```

**Что проверяется:**
- ✅ Форматирование кода (trailing whitespace)
- ✅ Clang-format для C++ файлов
- ✅ Python синтаксис
- ✅ Статический анализ технического долга

### CI/CD Pipeline
```yaml
# .github/workflows/ci-cd.yml
name: CI/CD Pipeline
on: [push, pull_request]
jobs:
  - test
  - build
  - deploy-docs
```

## 📊 Результаты тестирования

### Текущие метрики (3 июля 2025)
```json
{
  "coverage": {
    "lines": "70.8%",
    "functions": "75.0%",
    "branches": "66.7%"
  },
  "performance": {
    "validation": "< 100ms",
    "compensation": "< 50ms"
  },
  "technical_debt": {
    "score": 90,
    "status": "Medium"
  }
}
```

## 🚀 Запуск тестов

### Комплексное тестирование
```bash
# Все тесты + анализ
python scripts/run_comprehensive_tests.py

# Только анализ технического долга
python scripts/analyze_technical_debt.py

# Только тесты производительности
python scripts/run_performance_tests.py
```

### Отдельные тесты
```bash
# Unit тесты (требует PlatformIO)
pio test -e native -v

# E2E тесты
python test/e2e/test_web_ui.py

# CSRF тесты
python test/native/test_csrf_security.py
```

## 📈 Отчёты

### Генерируемые отчёты
- `test_reports/comprehensive-report.json` - Полный отчёт
- `test_reports/technical-debt-ci.json` - Анализ технического долга
- `test_reports/e2e-test-report.json` - E2E тесты
- `test_reports/performance-test-report.json` - Производительность

### Просмотр отчётов
```bash
# HTML отчёт
open test_reports/comprehensive-report.html

# JSON отчёт
cat test_reports/comprehensive-report.json | jq
```

## 🔍 Отладка

### Проблемы с Unicode (Windows)
```bash
# Установка переменной окружения
set PYTHONIOENCODING=utf-8

# Или в PowerShell
$env:PYTHONIOENCODING="utf-8"
```

### Проблемы с PlatformIO
```bash
# Очистка кэша
pio run -t clean

# Обновление зависимостей
pio pkg update
```

## 📋 Чек-лист тестирования

### Перед коммитом
- [ ] Pre-commit проверки прошли
- [ ] Unit тесты проходят
- [ ] Нет критических предупреждений

### Перед релизом
- [ ] Все тесты проходят
- [ ] Покрытие кода > 70%
- [ ] Технический долг < 100
- [ ] E2E тесты проходят
- [ ] Тесты производительности в норме

## 🛠️ Инструменты

### Основные скрипты
- `scripts/pre-commit.py` - Pre-commit проверки
- `scripts/format_code.py` - Форматирование кода
- `scripts/auto_format.py` - Удаление trailing whitespace
- `scripts/run_comprehensive_tests.py` - Комплексное тестирование
- `scripts/analyze_technical_debt.py` - Анализ технического долга

### Конфигурация
- `.clang-format` - Стиль C++ кода
- `platformio.ini` - Конфигурация PlatformIO
- `.github/workflows/ci-cd.yml` - CI/CD pipeline

## 📚 Дополнительные ресурсы

- [Документация PlatformIO](https://docs.platformio.org/)
- [Руководство по CSRF защите](docs/CSRF_SECURITY_AUDIT.md)
- [Отчёт о техническом долге](docs/TECH_DEBT_REPORT_2025-06.md)
- [Руководство по автоматизации](docs/QUICK_TECH_DEBT_FIXES.md)

---

**Поддержка:** Создайте issue в GitHub для вопросов по тестированию
