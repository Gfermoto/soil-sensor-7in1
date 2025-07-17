# Python Bindings для JXCT

Эта директория содержит Python-обёртки для C++ кода JXCT.

## Статус
- **В разработке** - пока используются заглушки в тестах
- **Цель** - интеграция Python тестов с реальным C++ кодом
- **Альтернатива** - REST API тестирование

## Использование
Тесты автоматически определяют доступность bindings:
```python
try:
    import jxct_core
    REAL_BINDINGS_AVAILABLE = True
except ImportError:
    REAL_BINDINGS_AVAILABLE = False
    # Используем заглушки
```

## Требования для сборки
- pybind11
- Microsoft Visual C++ Build Tools или w64devkit
- Arduino SDK заглушки для desktop-сборки 