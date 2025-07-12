# Руководство по тестированию JXCT v3.10.1

## Быстрый старт

### Предварительные требования
- Python 3.8+
- Компилятор C++ (w64devkit для Windows)
- Git

### Настройка среды

#### Windows
1. Установите w64devkit: https://github.com/skeeto/w64devkit
2. Добавьте путь к компилятору в PATH:
   ```
   C:\Program Files\w64devkit\bin
   ```
3. Перезапустите терминал

#### Linux/macOS
```bash
# Ubuntu/Debian
sudo apt install g++ python3-pip

# macOS
brew install gcc python3
```

## Запуск тестов

### Python тесты (основные)
```bash
# Все тесты
python -m pytest test/ -v

# Конкретная категория
python -m pytest test/performance/ -v
python -m pytest test/e2e/ -v

# С подробным выводом
python -m pytest test/ -v --tb=long
```

### Интеграционные тесты C++
```bash
# Автоматический запуск с настройкой среды
python test/integration/run_integration_tests.py

# Ручная компиляция
g++ -std=c++17 -I./include -I./src -I./test/stubs -I./test/stubs/Unity -I./test/stubs/Unity/src \
    test/integration/test_component_interaction.cpp test/stubs/Unity/src/unity.c \
    -o test_component_interaction
```

### Полный тестовый цикл
```bash
# 1. Python тесты
python -m pytest test/ -v

# 2. Интеграционные тесты C++
python test/integration/run_integration_tests.py

# 3. Проверка качества кода
python scripts/analyze_technical_debt.py
```

## Структура тестов

```
test/
├── e2e/                    # End-to-end тесты веб-интерфейса
├── integration/            # Интеграционные тесты C++
├── performance/            # Тесты производительности
├── stubs/                  # Заглушки для тестирования
│   ├── Arduino.h          # Заглушка Arduino
│   ├── esp32_stubs.h      # Заглушки ESP32
│   └── Unity/             # Unity Test Framework
└── *.py                   # Unit тесты Python
```

## Добавление новых тестов

### Python тест
```python
def test_new_feature():
    """Описание теста"""
    # Arrange
    input_data = "test"
    
    # Act
    result = process_data(input_data)
    
    # Assert
    assert result == "expected"
```

### C++ интеграционный тест
```cpp
void test_new_component() {
    TEST_CASE("Описание теста");
    
    // Arrange
    Component component;
    
    // Act
    bool result = component.process();
    
    // Assert
    TEST_ASSERT_TRUE(result);
}
```

## Устранение проблем

### Ошибки компиляции C++
1. Проверьте, что w64devkit установлен и добавлен в PATH
2. Убедитесь, что все заглушки на месте:
   - `test/stubs/Arduino.h`
   - `test/stubs/esp32_stubs.h`
   - `test/stubs/Unity/src/unity.c`

### Ошибки кодировки
- Все файлы должны быть в UTF-8
- В Windows используйте `encoding="utf-8"` в Python скриптах

### Проблемы с зависимостями
```bash
# Обновление зависимостей
pip install -r requirements.txt

# Проверка версий
python -c "import pytest; print(pytest.__version__)"
```

## Метрики качества

### Целевые показатели
- **Покрытие тестами:** >90%
- **Успешность тестов:** >95%
- **Время выполнения:** <30 сек
- **Предупреждения pytest:** 0

### Мониторинг
```bash
# Анализ технического долга
python scripts/analyze_technical_debt.py

# Проверка дублирования кода
python scripts/analyze_internal_linkage.py
```

## CI/CD интеграция

### GitHub Actions
```yaml
- name: Run Python tests
  run: python -m pytest test/ -v

- name: Run C++ integration tests
  run: python test/integration/run_integration_tests.py
```

### Локальная проверка перед коммитом
```bash
# Pre-commit hook
python -m pytest test/ -v --tb=short
python test/integration/run_integration_tests.py
```

## Контакты

При возникновении проблем:
1. Проверьте логи тестов
2. Убедитесь в корректности среды
3. Обратитесь к документации API
4. Создайте issue с подробным описанием

---
*Последнее обновление: $(Get-Date -Format "yyyy-MM-dd")* 