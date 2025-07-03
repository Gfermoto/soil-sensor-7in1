# 🔄 Интеграция отчётов с сайтом

## Обзор

Система автоматической интеграции отчётов тестирования с сайтом обеспечивает:

- **Автоматическое обновление** отчётов при каждом CI/CD запуске
- **Веб-интерфейс** для просмотра метрик качества кода
- **API endpoints** для программного доступа к отчётам
- **Виджеты** для встраивания метрик в другие страницы

## Архитектура интеграции

```
┌─────────────────┐    ┌──────────────────┐    ┌─────────────────┐
│   CI/CD Tests   │───▶│  Report Sync     │───▶│   GitHub Pages  │
│                 │    │                  │    │                 │
│ • Unit Tests    │    │ • JSON Reports   │    │ • HTML Reports  │
│ • Tech Debt     │    │ • HTML Pages     │    │ • API Endpoints │
│ • Coverage      │    │ • Metrics API    │    │ • Widgets       │
└─────────────────┘    └──────────────────┘    └─────────────────┘
```

## Компоненты системы

### 1. ESP32 API маршруты (`src/web/routes_reports.cpp`)

Предоставляет API для доступа к отчётам непосредственно с устройства:

```cpp
// API endpoints
/api/reports/test-summary     - Сводка тестирования
/api/reports/technical-debt   - Технический долг
/api/reports/full            - Полные отчёты

// Web interface
/reports                     - Главная страница отчётов
/reports/dashboard          - Краткий дашборд
```

### 2. Скрипт синхронизации (`scripts/sync_reports_to_site.py`)

Автоматически синхронизирует отчёты с сайтом:

```python
# Основные функции
- load_test_summary()       # Загрузка отчётов тестирования
- load_technical_debt()     # Загрузка отчётов технического долга
- generate_reports_html()   # Генерация HTML страниц
- sync_reports()           # Основная функция синхронизации
```

### 3. GitHub Action (`.github/workflows/update-site-reports.yml`)

Автоматизирует процесс обновления:

```yaml
# Триггеры
- workflow_run: после завершения тестирования
- workflow_dispatch: ручной запуск
- schedule: ежедневно в 3:00 UTC

# Основные шаги
- Проверка наличия отчётов
- Синхронизация с сайтом
- Генерация метрик
- Деплой на GitHub Pages
```

## Структура файлов отчётов

### JSON отчёты

```
site/reports/
├── test-summary.json      # Сводка тестирования
├── technical-debt.json    # Технический долг
└── metrics.json          # Сводные метрики для виджетов
```

### HTML страницы

```
site/reports/
├── index.html            # Главная страница отчётов
├── dashboard.html        # Краткий дашборд
├── widget.js            # JavaScript виджет
└── README.md            # Документация
```

## API спецификация

### GET /api/reports/test-summary

Возвращает сводку тестирования:

```json
{
  "timestamp": "2025-01-22T12:00:00Z",
  "total": 13,
  "passed": 13,
  "failed": 0,
  "success_rate": 100.0
}
```

### GET /api/reports/technical-debt

Возвращает метрики технического долга:

```json
{
  "code_smells": 66,
  "duplicated_lines": 933,
  "complexity_issues": 6,
  "security_hotspots": 134,
  "maintainability_rating": "D",
  "debt_ratio": 1.93,
  "coverage": 70.8
}
```

### GET /api/reports/full

Возвращает полные отчёты в объединённом формате.

## Встраивание виджетов

### На любую HTML страницу

```html
<!-- Контейнер для виджета -->
<div id="jxct-reports-widget"></div>

<!-- Подключение скрипта -->
<script src="reports/widget.js"></script>

<!-- Или ручная загрузка -->
<script>
window.loadJXCTReportsWidget('jxct-reports-widget');
</script>
```

### В ESP32 веб-интерфейс

Виджет автоматически интегрируется в главную страницу сайта в секции "Текущий статус".

## Конфигурация

### Настройка синхронизации

Скрипт `sync_reports_to_site.py` принимает параметры:

```bash
# Автоматическая синхронизация
python scripts/sync_reports_to_site.py

# С указанием корневой директории
python scripts/sync_reports_to_site.py /path/to/project
```

### Настройка GitHub Action

```yaml
# Принудительное обновление
workflow_dispatch:
  inputs:
    force_update:
      description: 'Принудительное обновление отчётов'
      type: boolean
      default: false
```

## Мониторинг и отладка

### Проверка статуса интеграции

1. **GitHub Actions**: Проверьте логи workflow `🔄 Update Site Reports`
2. **Файлы отчётов**: Убедитесь, что файлы в `site/reports/` обновляются
3. **API endpoints**: Проверьте доступность API на сайте

### Типичные проблемы

#### Отчёты не обновляются

```bash
# Проверьте наличие исходных отчётов
ls -la test_reports/

# Запустите синхронизацию вручную
python scripts/sync_reports_to_site.py

# Проверьте права доступа GitHub Action
# Убедитесь, что GITHUB_TOKEN имеет права на pages:write
```

#### Виджет не загружается

```javascript
// Проверьте консоль браузера на ошибки
// Убедитесь, что файл metrics.json доступен
fetch('reports/metrics.json')
  .then(response => response.json())
  .then(data => console.log(data))
  .catch(error => console.error(error));
```

#### Кодировка символов

```python
# Убедитесь, что файлы читаются с UTF-8
with open('file.json', 'r', encoding='utf-8') as f:
    data = json.load(f)
```

## Расширение функциональности

### Добавление новых метрик

1. **Обновите структуру отчётов** в `scripts/analyze_technical_debt.py`
2. **Добавьте обработку** в `scripts/sync_reports_to_site.py`
3. **Обновите виджеты** для отображения новых метрик
4. **Обновите API** в `src/web/routes_reports.cpp`

### Создание кастомных виджетов

```javascript
// Создайте собственный виджет
function createCustomWidget(containerId, metrics) {
    const container = document.getElementById(containerId);
    container.innerHTML = `
        <div class="custom-widget">
            <h3>Кастомные метрики</h3>
            <div>Успешность: ${metrics.test_success_rate}%</div>
            <!-- Добавьте свою логику -->
        </div>
    `;
}

// Используйте API для получения данных
fetch('/reports/metrics.json')
    .then(response => response.json())
    .then(data => createCustomWidget('my-widget', data));
```

## Безопасность

### Ограничения доступа

- **GitHub Pages**: Публичный доступ к отчётам
- **ESP32 API**: Доступ через локальную сеть
- **Чувствительные данные**: Не включайте секреты в отчёты

### Рекомендации

1. **Фильтруйте данные** перед публикацией
2. **Используйте HTTPS** для передачи отчётов
3. **Ограничьте частоту** обновлений для экономии ресурсов

## Производительность

### Оптимизация

- **Кэширование**: Отчёты кэшируются на 5 минут
- **Сжатие**: GitHub Pages автоматически сжимает файлы
- **CDN**: GitHub Pages использует глобальный CDN

### Метрики производительности

```javascript
// Измерение времени загрузки виджета
const start = performance.now();
loadReportsWidget('widget-container');
const end = performance.now();
console.log(`Загрузка виджета: ${end - start}ms`);
```

## Заключение

Система интеграции отчётов обеспечивает:

- ✅ **Автоматизацию** процесса обновления отчётов
- ✅ **Прозрачность** метрик качества кода
- ✅ **Удобство** доступа к информации
- ✅ **Расширяемость** для новых типов отчётов

Для получения дополнительной помощи обращайтесь к [документации проекта](index.md) или создавайте [issue](https://github.com/Gfermoto/soil-sensor-7in1/issues). 