# Документация для разработчиков

**Версия:** 3.13.x

## Содержимое

- **ARCH_OVERALL.md** — архитектура (кратко, детали в manuals/TECHNICAL_DOCS.md)
- **TECHNICAL_DEBT_REPORT.md** — анализ clang-tidy
- **TECHNICAL_DEBT_QUICK_REFERENCE.md** — быстрая справка по техническому долгу
- **REFACTORING_PLAN.md** — план рефакторинга
- **CLANG_TOOLS_GUIDE.md** — настройка clang-tidy, clang-format, cppcheck

## Команды

```bash
python scripts/run_simple_tests.py
python scripts/run_clang_tidy_analysis.py
pio run -e esp32dev
pio test -e native
```

## Требования

PlatformIO 6.0+, Python 3.8+, ESP32 с датчиком JXCT 7-in-1.
