# Руководство по скриптам JXCT

**Версия:** 3.10.0
**Дата:** Июль 2025

---

## Обзор

В проекте JXCT есть два типа скриптов для автоматизации разработки:

- **Автоматические** — работают без ожидания ввода пользователя
- **Интерактивные** — требуют ручного подтверждения

## Автоматические скрипты

### 1. `auto_deploy.ps1` — Автоматический деплой

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
- Автоматически коммитит незафиксированные изменения
- Генерирует документацию через MkDocs
- Проверяет корректность сборки
- Отправляет изменения в Git
- Не требует ручного ввода

### 2. `auto_test.ps1` — Автоматическое тестирование

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
- Простые unit-тесты
- E2E тесты (если не пропущены)
- Performance тесты (если не пропущены)
- Comprehensive тесты
- Автоматическая генерация отчетов

### 3. `deploy.ps1` — Гибридный деплой

Поддерживает как автоматический, так и интерактивный режим.

```powershell
# Автоматический режим
.\scripts\deploy.ps1 -Auto

# Автоматический с кастомным сообщением
.\scripts\deploy.ps1 -Auto -CommitMessage "Мой коммит"

# Интерактивный режим (по умолчанию)
.\scripts\deploy.ps1
```

## Тестовые скрипты

### Python скрипты для тестирования

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

### Анализ качества кода

```bash
# Анализ технического долга
python scripts/audit/analyze_technical_debt.py

# Проверка консистентности документации
python scripts/doc_consistency_check.py

# Генерация CI отчета
python scripts/generate_ci_summary.py
```

## Управление версиями

### Release Manager

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

### Python напрямую

```bash
# Поднять версию
python scripts/release_manager.py version --type patch

# Создать релиз
python scripts/release_manager.py release --version "3.10.1"

# Синхронизировать версии
python scripts/release_manager.py sync
```

## Рекомендации по использованию

### Для разработки
- Используйте `auto_deploy.ps1` для быстрых обновлений
- Используйте `auto_test.ps1` для проверки качества кода
- Запускайте тесты перед каждым коммитом

### Для CI/CD
- Используйте автоматические скрипты с флагами
- Настройте автоматический запуск тестов перед деплоем
- Используйте `-SkipE2E` для быстрых проверок

### Для релизов
- Используйте `release.ps1` для управления версиями
- Комбинируйте с автоматическими скриптами
- Всегда тестируйте перед релизом

## Быстрые команды

```powershell
# Быстрый деплой
.\scripts\auto_deploy.ps1

# Быстрое тестирование
.\scripts\auto_test.ps1 -SkipE2E

# Полный цикл: тесты + деплой
.\scripts\auto_test.ps1 && .\scripts\auto_deploy.ps1

# Проверка качества кода
python scripts/audit/analyze_technical_debt.py

# Форматирование кода
python scripts/auto_format.py
```

## Настройка

### Требования

- **Python 3.8+** — для Python скриптов
- **PowerShell 5.1+** — для PowerShell скриптов
- **Git** — для управления версиями
- **PlatformIO** — для сборки ESP32 (опционально)

### Установка зависимостей

```bash
# Установка Python пакетов
pip install -r requirements.txt

# Установка PlatformIO (если нужно)
pip install platformio
```

### Переменные окружения

```bash
# Для автоматических коммитов
export GIT_AUTHOR_NAME="Your Name"
export GIT_AUTHOR_EMAIL="your.email@example.com"

# Для тестирования
export JXCT_TEST_DEVICE_IP="192.168.1.100"
export JXCT_TEST_TIMEOUT="30"
```

## Мониторинг и отчеты

### Где сохраняются отчеты

- **`test_reports/`** — результаты тестирования
- **Git history** — история деплоев и изменений
- **Console output** — детальные логи выполнения

### Типы отчетов

- **JSON отчеты** — для машинной обработки
- **HTML отчеты** — для просмотра в браузере
- **Markdown отчеты** — для документации
- **CI отчеты** — для интеграции с CI/CD

## Устранение проблем

### Ошибка "Python не найден"
```bash
# Проверьте установку Python
python --version

# Добавьте Python в PATH
# Windows: https://docs.python.org/3/using/windows.html
```

### Ошибка "Git не найден"
```bash
# Установите Git
# https://git-scm.com/downloads
```

### Ошибка коммита
```bash
# Проверьте git статус
git status

# Настройте git конфигурацию
git config --global user.name "Your Name"
git config --global user.email "your.email@example.com"
```

### Ошибка тестирования
```bash
# Проверьте подключение к устройству
ping 192.168.1.100

# Проверьте логи тестов
cat test_reports/latest_test_report.json
```

---

## Быстрые команды разработки

### Основные команды

```bash
# Быстрая сборка и тестирование
pio run -e esp32dev && python scripts/run_simple_tests.py

# Полный цикл проверки
python scripts/run_clang_tidy_analysis.py && pio check -e static-analysis && python -m pytest test/

# Обновление версии и деплой
python scripts/auto_version.py && python scripts/sync_versions.py

# Генерация документации
python scripts/gen_docs.py && mkdocs build
```

### Настройка среды разработки

#### Требования
- **Python 3.8+** с pip
- **PlatformIO Core** 6.0+
- **w64devkit** (для Windows, clang-tidy)
- **Git** для контроля версий

#### Быстрая настройка
```bash
# 1. Клонирование репозитория
git clone https://github.com/your-org/JXCT.git
cd JXCT

# 2. Установка зависимостей
pip install -r requirements.txt
pio lib install

# 3. Настройка pre-commit хуков
python scripts/setup_hooks.py

# 4. Первая сборка
pio run -e esp32dev
```

#### Настройка clang-tools (Windows)
```powershell
# Автоматическая настройка PATH
.\scripts\setup_clang_tools_simple.ps1

# Проверка установки
clang-tidy --version
```

### Команды мониторинга

```bash
# Статус проекта
python scripts/generate_ci_summary.py

# Анализ технического долга
python scripts/audit/analyze_technical_debt.py

# Проверка производительности
python scripts/performance_monitor.py

# Валидация формул
python scripts/validate_formula_consistency.py
```

### Workflow команды

```bash
# Ежедневная проверка
make daily-check  # или python scripts/run_simple_tests.py

# Перед коммитом
make pre-commit   # или python scripts/pre-commit.py

# Релиз
make release      # или python scripts/release_manager.py
```

### Отладка

```bash
# Подробный лог сборки
pio run -e esp32dev -v

# Отладка тестов
python -m pytest test/ -v -s

# Анализ памяти
pio run -e esp32dev -t size

# Мониторинг Serial
pio device monitor
```

### Среда разработки

#### VS Code настройка
- Установите PlatformIO IDE extension
- Используйте workspace settings из `.vscode/`
- Включите clang-tidy в настройках

#### Полезные алиасы
```bash
# Добавьте в .bashrc или PowerShell Profile
alias piorun='pio run -e esp32dev'
alias piotest='pio test -e native'  
alias jxcttest='python scripts/run_simple_tests.py'
alias jxctbuild='pio run -e esp32dev && python scripts/run_simple_tests.py'
```

---

## Дополнительные ресурсы

- [Руководство по тестированию](TESTING_GUIDE.md)
- [Быстрая справка по тестированию](TESTING_QUICK_REFERENCE.md)
- [Техническая документация](manuals/TECHNICAL_DOCS.md)
- [GitHub Issues](https://github.com/Gfermoto/soil-sensor-7in1/issues)

---

**Примечание:** Автоматические скрипты предназначены для ускорения разработки и CI/CD. Для критических операций рекомендуется использовать интерактивные версии с ручным подтверждением.
