# 🧪 Система тестирования JXCT v3.7.0

## 📋 **Оптимизированная структура тестов**

### 🎯 **Основные тесты (используются ежедневно)**

#### **Python тесты (хост-система)**
- `test_format.py` - Тестирование форматирования кода
- `test_validation.py` - Тестирование валидации данных  
- `test_routes.py` - Тестирование веб-маршрутов

#### **ESP32 тесты (реальное железо)**
- `test_esp32_real.cpp` - Основные тесты для ESP32

### 🔧 **Специализированные тесты (по необходимости)**

#### **Native тесты (C++ на хосте)**
- `native/test_csrf_security.cpp` - Тестирование CSRF защиты
- `native/test_simple_csrf.cpp` - Простые CSRF тесты

#### **Интеграционные тесты**
- `integration/test_web_api_integration.cpp` - API интеграция
- `integration/test_component_interaction.cpp` - Взаимодействие компонентов

#### **Производительность**
- `performance/test_performance.cpp` - Тесты производительности

#### **E2E тесты**
- `e2e/test_web_ui.py` - Тестирование веб-интерфейса

### 🚀 **Быстрый запуск**

```bash
# Основные Python тесты (быстро)
python scripts/run_simple_tests.py

# ESP32 тесты (требует подключенное устройство)
pio test -e esp32dev --upload-port COM6

# Native тесты (C++ на хосте)
pio test -e native

# Все тесты
pio test
```

### ⚙️ **Конфигурации PlatformIO**

- `esp32dev` - Основная разработка ESP32
- `esp32dev-production` - Production сборка
- `native` - Native тесты на хосте
- `native-coverage` - Тесты с покрытием кода

### 🎯 **Рекомендуемый workflow**

1. **Ежедневная разработка**: `python scripts/run_simple_tests.py`
2. **Перед коммитом**: `pio test -e native`
3. **Перед релизом**: `pio test` (все среды)
4. **Отладка ESP32**: `pio test -e esp32dev --upload-port COM6`

### 🧹 **Очищено в v3.7.0**

Удалены экспериментальные файлы:
- `test/esp32/` - дублирующие ESP32 тесты
- `unity_simple.h` - экспериментальный заголовок
- `test_report_generator.*` - экспериментальные генераторы
- `simple_csrf.*` - простые заглушки

### 📊 **Метрики качества**

- **Python тесты**: 8/8 ✅
- **ESP32 готовность**: ✅ (устройство подключено)
- **Покрытие кода**: 70.8% (цель: 85%)
- **Технический долг**: 30 (Low уровень)

---
**Обновлено**: 2025-01-23  
**Версия**: 3.7.0  
**Статус**: Оптимизировано ✅ 