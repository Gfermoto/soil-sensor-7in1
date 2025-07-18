# 🚀 Руководство по скриптам JXCT

**Версия:** 3.11.0
**Дата:** Июль 2025

---

## 📋 Обзор {#Obzor}

В проекте JXCT используются автоматические и интерактивные скрипты для автоматизации разработки и CI/CD. Все скрипты приведены в папке `scripts/`.

- **🔄 Автоматические** — работают без ожидания ввода пользователя
- **💬 Интерактивные** — требуют ручного подтверждения

## 🔄 Автоматические скрипты {#Avtomaticheskie-skripty}

### 1. `deploy.ps1` — Гибридный деплой (автоматический и интерактивный режим)

```powershell
# Автоматический режим
./scripts/deploy.ps1 -Auto

# С кастомным сообщением
./scripts/deploy.ps1 -Auto -CommitMessage "Обновление API документации"

# Интерактивный режим (по умолчанию)
./scripts/deploy.ps1
```

**Что делает:**
- ✅ Автоматически коммитит незакоммиченные изменения
- ✅ Генерирует документацию через MkDocs
- ✅ Проверяет корректность сборки
- ✅ Отправляет изменения в Git
- ✅ Не требует ручного ввода (в режиме -Auto)

### 2. `run_simple_tests.py` — Полный прогон тестов
```bash
python scripts/run_simple_tests.py
```

### 3. `ultra_quick_test.py` — Ультра-быстрый smoke-тест
```bash
python scripts/ultra_quick_test.py
```

### 4. `run_clang_tidy_analysis.py` — Статический анализ
```bash
python scripts/run_clang_tidy_analysis.py
```

### 5. `format_code.py` — Универсальное форматирование кода
```bash
python scripts/format_code.py
```

### 6. `auto_format.py` — Удаление trailing whitespace и поиск длинных строк
```bash
python scripts/auto_format.py
```

### 7. `pre-commit.py` — Pre-commit хуки
```bash
python scripts/pre-commit.py
```

### 8. `release.ps1` и `release_manager.py` — Управление версиями и релизами
```powershell
./scripts/release.ps1 version -Type patch
./scripts/release.ps1 release -Version "3.10.1" -Message "Новые функции"
```
или
```bash
python scripts/release_manager.py version --type patch
python scripts/release_manager.py release --version "3.10.1"
```

---

## 🧪 Тестовые скрипты {#Testovye-skripty}

### Python скрипты для тестирования
```bash
# Полные тесты
python scripts/run_simple_tests.py

# Ультра-быстрый smoke-тест
python scripts/ultra_quick_test.py

# E2E тесты
python scripts/run_e2e_tests.py

# Performance тесты
python scripts/run_performance_tests.py

# Комплексные тесты (расширенное покрытие)
python scripts/run_comprehensive_tests.py
```

---

## 🛠️ Форматирование кода

### Универсальный форматтер
```bash
python scripts/format_code.py
```

### Удаление trailing whitespace
```bash
python scripts/auto_format.py
```

---

## 🏷️ Быстрые команды

```bash
# Ультра-быстрое тестирование (5 секунд)
python scripts/ultra_quick_test.py

# Полное тестирование (2 минуты)
python scripts/run_simple_tests.py

# Статический анализ
python scripts/run_clang_tidy_analysis.py

# Форматирование кода
python scripts/format_code.py
```

---

## 🧑‍💻 Рекомендации по использованию

### Для разработки:
- Используйте `deploy.ps1` для деплоя документации (с параметром -Auto для автоматизации)
- Запускайте тесты перед каждым коммитом
- Используйте `format_code.py` для форматирования кода

### Для CI/CD:
- Используйте только актуальные скрипты, перечисленные выше
- Все тесты и анализы должны запускаться автоматически через GitHub Actions

---

## ⚠️ Удалённые/устаревшие скрипты
- auto_deploy.ps1 (заменён на deploy.ps1)
- format_all.ps1 (заменён на format_code.py)
- run_all_tests.py (дублирует run_simple_tests.py)
- fix_anchors*.py, remove_duplicate_toc.py (устарели)
- run_clang_tidy_analysis_original.py (устарел)
- quick-test.yml, docs.yml.disabled (workflow удалены)

---

## 📝 История изменений
- 2025-07-17: Глобальный рефакторинг инфраструктуры, удалены дублирующие и устаревшие скрипты, обновлено руководство.
