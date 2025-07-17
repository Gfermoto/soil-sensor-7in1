# 🚀 Руководство по скриптам JXCT

**Версия:** 3.10.1
**Дата:** Июль 2025

---

## 📋 Обзор {#Obzor}

В проекте JXCT есть два типа скриптов для автоматизации разработки:

- **🔄 Автоматические** — работают без ожидания ввода пользователя
- **💬 Интерактивные** — требуют ручного подтверждения

## 🔄 Автоматические скрипты {#Avtomaticheskie-skripty}

### 1. `auto_deploy.ps1` — Автоматический деплой {#1-auto_deployps1-Avtomaticheskiy-deploy}

Полностью автоматический деплой документации без ожидания ввода.

```powershell
# Базовый запуск
.\scripts\auto_deploy.ps1

# С кастомным сообщением коммита
.\scripts\auto_deploy.ps1 -CommitMessage "Обновление API документации"

# Пропустить тесты
.\scripts\auto_deploy.ps1 -SkipTests

# Принудительный запуск
.\scripts\auto_deploy.ps1 -Force
```

**Что делает:**
- ✅ Автоматически коммитит незафиксированные изменения
- ✅ Генерирует документацию через MkDocs
- ✅ Проверяет корректность сборки
- ✅ Отправляет изменения в Git
- ✅ Не требует ручного ввода

### 2. `auto_test.ps1` — Автоматическое тестирование {#2-auto_testps1-Avtomaticheskoe-testirovanie}

Запуск всех тестов с автоматической генерацией отчетов.

```powershell
# Запуск всех тестов
.\scripts\auto_test.ps1

# Пропустить E2E тесты
.\scripts\auto_test.ps1 -SkipE2E

# Пропустить performance тесты
.\scripts\auto_test.ps1 -SkipPerformance

# Без генерации отчета
.\scripts\auto_test.ps1 -GenerateReport:$false

# Кастомная папка для отчетов
.\scripts\auto_test.ps1 -OutputDir "my_reports"
```

**Что тестирует:**
- 📋 Простые unit-тесты
- 🔄 E2E тесты (если не пропущены)
- ⚡ Performance тесты (если не пропущены)
- 🔍 Comprehensive тесты
- 📊 Автоматическая генерация отчетов

### 3. `deploy.ps1` — Гибридный деплой {#3-deployps1-Gibridnyy-deploy}

Поддерживает как автоматический, так и интерактивный режим.

```powershell
# Автоматический режим
.\scripts\deploy.ps1 -Auto

# Автоматический с кастомным сообщением
.\scripts\deploy.ps1 -Auto -CommitMessage "Мой коммит"

# Интерактивный режим (по умолчанию)
.\scripts\deploy.ps1
```

## 🧪 Тестовые скрипты {#Testovye-skripty}

### Python скрипты для тестирования {#python-skripty-dlya-testirovaniya}

```bash
# Простые тесты
python scripts/run_simple_tests.py

# E2E тесты на реальном устройстве
python scripts/run_e2e_tests.py

# Performance тесты
python scripts/run_performance_tests.py

# Комплексные тесты
python scripts/run_comprehensive_tests.py
```

### Анализ качества кода {#Analiz-kachestva-koda}

```bash
# Анализ технического долга
python scripts/analyze_technical_debt.py

# Проверка консистентности документации
python scripts/doc_consistency_check.py

# Генерация CI отчета
python scripts/generate_ci_summary.py
```

## 🏷️ Управление версиями {#Upravlenie-versiyami}

### Release Manager {#release-manager}

```powershell
# Поднять patch версию (3.10.0 -> 3.10.1)
.\scripts\release.ps1 version -Type patch

# Поднять minor версию (3.10.0 -> 3.11.0)
.\scripts\release.ps1 version -Type minor

# Создать релиз с конкретной версией
.\scripts\release.ps1 release -Version "3.10.1" -Message "Новые функции"

# Быстрый bump + релиз
.\scripts\release.ps1 bump -Type patch
```

### Python напрямую {#python-napryamuyu}

```bash
# Поднять версию
python scripts/release_manager.py version --type patch

# Создать релиз
python scripts/release_manager.py release --version "3.10.1"

# Синхронизировать версии
python scripts/release_manager.py sync
```

## 🎯 Рекомендации по использованию {#Rekomendatsii-po-ispolzovaniyu}

### Для разработки: {#Dlya-razrabotki}
- Используйте `auto_deploy.ps1` для быстрых обновлений
- Используйте `auto_test.ps1` для проверки качества кода
- Запускайте тесты перед каждым коммитом

### Для CI/CD: {#Dlya-cicd}
- Используйте автоматические скрипты с флагами
- Настройте автоматический запуск тестов перед деплоем
- Используйте `-SkipE2E` для быстрых проверок

### Для релизов: {#Dlya-relizov}
- Используйте `release.ps1` для управления версиями
- Комбинируйте с автоматическими скриптами
- Всегда тестируйте перед релизом

## ⚡ Быстрые команды {#Bystrye-komandy}

```powershell
# Быстрый деплой
.\scripts\auto_deploy.ps1

# Быстрое тестирование
.\scripts\auto_test.ps1 -SkipE2E

# Полный цикл: тесты + деплой
.\scripts\auto_test.ps1 && .\scripts\auto_deploy.ps1

# Проверка качества кода
python scripts/analyze_technical_debt.py

# Форматирование кода
python scripts/auto_format.py
```

## 🔧 Настройка {#Nastroyka}

### Требования {#Trebovaniya}

- **Python 3.8+** — для Python скриптов
- **PowerShell 5.1+** — для PowerShell скриптов
- **Git** — для управления версиями
- **PlatformIO** — для сборки ESP32 (опционально)

### Установка зависимостей {#Ustanovka-zavisimostey}

```bash
# Установка Python пакетов
pip install -r requirements.txt

# Установка PlatformIO (если нужно)
pip install platformio
```

### Переменные окружения {#Peremennye-okruzheniya}

```bash
# Для автоматических коммитов
export GIT_AUTHOR_NAME="Your Name"
export GIT_AUTHOR_EMAIL="your.email@example.com"

# Для тестирования
export JXCT_TEST_DEVICE_IP="192.168.1.100"
export JXCT_TEST_TIMEOUT="30"
```

## 📊 Мониторинг и отчеты {#Monitoring-i-otchety}

### Где сохраняются отчеты {#Gde-sohranyayutsya-otchety}

- **`test_reports/`** — результаты тестирования
- **Git history** — история деплоев и изменений
- **Console output** — детальные логи выполнения

### Типы отчетов {#Tipy-otchetov}

- **JSON отчеты** — для машинной обработки
- **HTML отчеты** — для просмотра в браузере
- **Markdown отчеты** — для документации
- **CI отчеты** — для интеграции с CI/CD

## 🚨 Устранение проблем {#Ustranenie-problem}

### Ошибка "Python не найден" {#Oshibka-python-ne-nayden}
```bash
# Проверьте установку Python
python --version

# Добавьте Python в PATH
# Windows: https://docs.python.org/3/using/windows.html
```

### Ошибка "Git не найден" {#Oshibka-git-ne-nayden}
```bash
# Установите Git
# https://git-scm.com/downloads
```

### Ошибка коммита {#Oshibka-kommita}
```bash
# Проверьте git статус
git status

# Настройте git конфигурацию
git config --global user.name "Your Name"
git config --global user.email "your.email@example.com"
```

### Ошибка тестирования {#Oshibka-testirovaniya}
```bash
# Проверьте подключение к устройству
ping 192.168.1.100

# Проверьте логи тестов
cat test_reports/latest_test_report.json
```

---

## 📚 Дополнительные ресурсы {#Dopolnitelnye-resursy}

- [Руководство по тестированию](TESTING_GUIDE.md)
- [Быстрая справка по тестированию](TESTING_QUICK_REFERENCE.md)
- [Техническая документация](manuals/TECHNICAL_DOCS.md)
- [GitHub Issues](https://github.com/Gfermoto/soil-sensor-7in1/issues)

---

**Примечание:** Автоматические скрипты предназначены для ускорения разработки и CI/CD. Для критических операций рекомендуется использовать интерактивные версии с ручным подтверждением.
