# 🚀 Настройка среды разработки JXCT

> 📍 **Краткая справка для разработчиков**

## 🎯 Быстрый старт

### 1. Cursor + AI
- **[📖 Полное руководство Cursor](docs/dev/CURSOR_GUIDE.md)** - Настройка AI ассистента
- **[🤖 Настройка ботов](docs/dev/BOTS_SETUP_GUIDE.md)** - GitHub Actions и автоматизация

### 2. Анализ кода
- **clang-tidy:** `python scripts/run_clang_tidy_analysis.py`
- **Тесты:** `python scripts/run_simple_tests.py`
- **Технический долг:** `python scripts/analyze_technical_debt.py`

### 3. Сборка
```bash
pio run -e esp32dev
pio test -e native
```

## 📚 Документация

- **[📖 Полная документация](docs/README.md)**
- **[🔧 Техническая документация](docs/dev/)**
- **[👥 Руководства пользователя](docs/manuals/)**

## ⚠️ Важно

- **Используйте только одно расширение C++** (clangd или cpptools)
- **Все боты настроены** и не требуют токенов
- **AI ассистент готов** к работе с проектом

---

**Готово к разработке!** 🎉 