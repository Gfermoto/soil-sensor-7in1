# ⚡ БЫСТРЫЕ КОМАНДЫ JXCT

## 🚀 ОСНОВНЫЕ КОМАНДЫ (БЕЗ ЗАВИСАНИЙ)

### ⚡ Ультра-быстрое тестирование (рекомендуется)
```bash
python scripts/ultra_quick_test.py
```
**Время:** ~5 секунд | **Зависания:** НЕТ

### 🔍 Быстрое тестирование
```bash
python scripts/quick_test.py
```
**Время:** ~30 секунд | **Зависания:** НЕТ

### 📋 Полное тестирование
```bash
python scripts/run_simple_tests.py
```
**Время:** ~2 минуты | **Зависания:** НЕТ

## 🧪 ОТДЕЛЬНЫЕ ТЕСТЫ

### Python тесты
```bash
python -m pytest test/test_validation.py test/test_compensation_formulas.py -v
```

### Тесты с моками
```bash
python test/test_mock_coverage.py
```

### REST API тесты
```bash
python test/test_rest_api_integration.py
```

## 🔨 СБОРКА

### ESP32 сборка
```bash
pio run -e esp32dev
```

### Production сборка
```bash
pio run -e esp32dev-production
```

## 🔍 АНАЛИЗ КОДА

### Быстрый clang-tidy
```bash
python scripts/run_clang_tidy_analysis.py --quick
```

### Полный clang-tidy
```bash
python scripts/run_clang_tidy_analysis.py
```

## 📊 СТАТИСТИКА

### Покрытие тестами
```bash
python -m pytest --cov=src test/
```

### Размер прошивки
```bash
ls -la .pio/build/esp32dev/firmware.bin
```

## 🚨 РЕШЕНИЕ ПРОБЛЕМ

### Если тесты зависают
1. Используйте `ultra_quick_test.py` - самый надежный
2. Проверьте подключение к интернету
3. Перезапустите терминал

### Если сборка падает
1. `pio run -t clean`
2. `pio run -e esp32dev`

### Если тесты падают
1. Проверьте Python версию (3.8+)
2. Установите зависимости: `pip install pytest`
3. Запустите `ultra_quick_test.py`

## 📈 РЕКОМЕНДАЦИИ

- **Для ежедневной работы:** `ultra_quick_test.py`
- **Для проверки перед коммитом:** `quick_test.py`
- **Для полной проверки:** `run_simple_tests.py`
- **Для отладки:** отдельные тесты

## 🎯 ЦЕЛИ КАЧЕСТВА

- ✅ Все тесты проходят
- ✅ Покрытие >85%
- ✅ Сборка успешна
- ✅ Нет критических предупреждений 