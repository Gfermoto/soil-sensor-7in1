# Contributing to JXCT Soil Sensor

Спасибо за интерес к проекту JXCT! Мы приветствуем вклад от сообщества.

## Содержание
- [Быстрый старт](#быстрый-старт)
- [Сообщение об ошибках](#сообщение-об-ошибках)
- [Предложение новых функций](#предложение-новых-функций)
- [Разработка](#разработка)
- [Документация](#документация)
- [Тестирование](#тестирование)
- [Pull Request](#pull-request)
- [Чек-лист](#чек-лист)

---

## Быстрый старт {#быстрый-старт}

### Требования
- PlatformIO IDE или CLI
- ESP32 DevKit
- Python 3.8+
- Git

### Настройка окружения
```bash
git clone https://github.com/Gfermoto/soil-sensor-7in1.git
cd soil-sensor-7in1
pip install -r requirements.txt
```

### Первая сборка
```bash
pio run -t upload
pio run -t uploadfs
```

---

## Сообщение об ошибках {#сообщение-об-ошибках}

### Перед созданием issue
1. Проверьте [FAQ](https://github.com/Gfermoto/soil-sensor-7in1#faq)
2. Изучите [Troubleshooting](https://github.com/Gfermoto/soil-sensor-7in1#troubleshooting)
3. Поищите в существующих issues

### Создание bug report
Используйте шаблон:
```markdown
**Описание ошибки**
Краткое описание проблемы

**Шаги для воспроизведения**
1. Подключите датчик
2. Откройте web-интерфейс
3. ...

**Ожидаемое поведение**
Что должно происходить

**Фактическое поведение**
Что происходит на самом деле

**Окружение**
- Версия прошивки: 3.10.0
- ESP32: DevKit v1
- Браузер: Chrome 120

**Дополнительная информация**
Логи, скриншоты, конфигурация
```

---

## Предложение новых функций {#предложение-новых-функций}

### Критерии принятия
- Соответствует целям проекта (IoT, точное земледелие)
- Технически реализуемо на ESP32
- Не нарушает существующую архитектуру
- Включает план тестирования

### Создание feature request
```markdown
**Описание функции**
Что должна делать новая функция

**Обоснование**
Почему это нужно пользователям

**Предлагаемая реализация**
Как это можно реализовать

**Альтернативы**
Другие способы решения проблемы

**Дополнительная информация**
Примеры, исследования, ссылки
```

---

## Разработка {#разработка}

### Архитектура проекта
```
JXCT/
├── src/                    # Основной код
│   ├── main.cpp           # Точка входа
│   ├── web/               # Веб-интерфейс
│   ├── calibration_manager.cpp
│   └── sensor_compensation.cpp
├── include/               # Заголовочные файлы
├── test/                  # Тесты
│   ├── native/           # Unit тесты
│   ├── e2e/              # E2E тесты
│   └── performance/      # Тесты производительности
├── docs/                  # Документация
└── scripts/               # Скрипты автоматизации
```

### Стандарты кодирования
- **Язык:** C++17
- **Форматирование:** clang-format
- **Именование:** camelCase для функций, PascalCase для классов
- **Комментарии:** Doxygen формат
- **Безопасность:** CSRF защита для всех web endpoints

### Запуск тестов
```bash
# Unit тесты
pio test -e native

# E2E тесты
python scripts/run_e2e_tests.py

# Анализ технического долга
python scripts/audit/analyze_technical_debt.py

# Форматирование кода
python scripts/auto_format.py
```

---

## Документация {#документация}

### Стандарты документации
- **Markdown (CommonMark)** с поддержкой GitHub Flavored Markdown
- **UTF-8 без BOM**
- **Заголовки:** тема + версия
- **Код:** тройные бэктики с указанием языка
- **Диаграммы:** Mermaid

### Структура документации
```
docs/
├── manuals/        # Руководства пользователя
├── dev/            # Технические документы
├── examples/       # Примеры конфигов, CSV
└── api/            # Автогенерированная API документация
```

### Линтеры документации
```bash
npm i -g markdownlint-cli markdown-link-check
markdownlint "**/*.md"
markdown-link-check README.md
```

---

## Тестирование {#тестирование}

### Требования к тестам
- Покрытие кода не менее 70%
- Все unit тесты проходят
- Все E2E тесты проходят
- Тесты производительности в норме

### Добавление новых тестов
```cpp
// test/native/test_new_feature.cpp
#include <unity.h>
#include "new_feature.h"

void test_new_feature_basic() {
    // Arrange
    NewFeature feature;

    // Act
    bool result = feature.process();

    // Assert
    TEST_ASSERT_TRUE(result);
}

int main() {
    UNITY_BEGIN();
    RUN_TEST(test_new_feature_basic);
    return UNITY_END();
}
```

---

## Pull Request {#pull-request}

### Процесс создания PR
1. **Создайте ветку** от `main`
   ```bash
   git checkout -b feature/your-feature-name
   ```

2. **Внесите изменения** с частыми коммитами
   ```bash
   git add .
   git commit -m "feat: add new sensor calibration method"
   ```

3. **Запустите тесты** локально
   ```bash
   pio test -e native
   python scripts/run_e2e_tests.py
   ```

4. **Создайте Pull Request** с описанием изменений

### Шаблон PR
```markdown
## Описание
Краткое описание изменений

## Тип изменений
- [ ] Bug fix
- [ ] New feature
- [ ] Breaking change
- [ ] Documentation update

## Тестирование
- [ ] Unit тесты проходят
- [ ] E2E тесты проходят
- [ ] Локальное тестирование выполнено

## Документация
- [ ] README обновлен (если нужно)
- [ ] Документация API обновлена (если нужно)
- [ ] Комментарии в коде добавлены

## Чек-лист
- [ ] Код отформатирован
- [ ] Все тесты проходят
- [ ] Покрытие кода не уменьшилось
- [ ] Документация обновлена
```

---

## Чек-лист {#чек-лист}

### Перед отправкой PR
- [ ] Код отформатирован (`python scripts/auto_format.py`)
- [ ] Все тесты проходят (`pio test -e native`)
- [ ] E2E тесты проходят (`python scripts/run_e2e_tests.py`)
- [ ] Покрытие кода не уменьшилось
- [ ] Документация обновлена
- [ ] Коммиты имеют понятные сообщения
- [ ] PR описание заполнено по шаблону

### Для документации
- [ ] Локально прошёл `markdownlint`
- [ ] Все ссылки работают (`markdown-link-check`)
- [ ] Добавлен/обновлён TOC при необходимости
- [ ] Обновлены ссылки при переименовании файлов

### Для новых функций
- [ ] Добавлены unit тесты
- [ ] Добавлены E2E тесты (если применимо)
- [ ] Обновлена документация
- [ ] Проверена совместимость с существующим кодом

---

## Контакты

- **Issues:** [GitHub Issues](https://github.com/Gfermoto/soil-sensor-7in1/issues)
- **Discussions:** [GitHub Discussions](https://github.com/Gfermoto/soil-sensor-7in1/discussions)
- **Документация:** [Сайт документации](https://gfermoto.github.io/soil-sensor-7in1/)

Спасибо за вклад в развитие проекта!
