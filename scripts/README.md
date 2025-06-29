# JXCT Scripts

Скрипты для автоматизации разработки и релизов JXCT Soil Sensor.

## 🚀 Release Manager

**Единственный скрипт** для управления версиями и релизами: `release_manager.py`

### Установка

```bash
# Убедитесь, что Python и Git установлены
python --version
git --version
```

### Команды

#### 1. `version` - Поднять версию без релиза
```powershell
# Увеличить patch версию (3.6.2 -> 3.6.3)
.\scripts\release.ps1 version -Type patch

# Увеличить minor версию (3.6.2 -> 3.7.0)
.\scripts\release.ps1 version -Type minor

# Увеличить major версию (3.6.2 -> 4.0.0)
.\scripts\release.ps1 version -Type major

# Только обновить файлы, без коммита
.\scripts\release.ps1 version -Type patch -NoCommit

# Обновить файлы и закоммитить, но не пушить
.\scripts\release.ps1 version -Type patch -NoPush
```

#### 2. `release` - Создать релиз с конкретной версией
```powershell
# Создать релиз 3.6.3
.\scripts\release.ps1 release -Version "3.6.3" -Message "Новые функции"

# Создать релиз без автоматического пуша
.\scripts\release.ps1 release -Version "3.6.3" -NoPush
```

#### 3. `bump` - Увеличить версию и создать релиз
```powershell
# Увеличить patch и создать релиз (3.6.2 -> 3.6.3)
.\scripts\release.ps1 bump -Type patch

# Увеличить minor и создать релиз (3.6.2 -> 3.7.0)
.\scripts\release.ps1 bump -Type minor
```

#### 4. `sync` - Синхронизировать версии между файлами
```powershell
# Синхронизировать версии
.\scripts\release.ps1 sync
```

### Python напрямую

```bash
# Поднять версию
python scripts/release_manager.py version --type patch

# Создать релиз
python scripts/release_manager.py release --version "3.6.3"

# Увеличить версию и создать релиз
python scripts/release_manager.py bump --type minor

# Синхронизировать версии
python scripts/release_manager.py sync
```

### Что делает каждая команда

| Команда | Действие | Git тег | Пуш |
|---------|----------|---------|-----|
| `version` | Обновляет файлы версии | ❌ | Опционально |
| `release` | Создает полный релиз | ✅ | Да |
| `bump` | Увеличивает версию + релиз | ✅ | Да |
| `sync` | Синхронизирует файлы | ❌ | ❌ |

### Файлы версий

- **`VERSION`** - основной файл версии (например, "3.6.2")
- **`include/version.h`** - C++ заголовочный файл с макросами
- **`platformio.ini`** - комментарии с версией и датой обновления

### GitHub Actions

При пуше тега автоматически запускается workflow, который:

1. ✅ Синхронизирует версии между файлами
2. ✅ Проверяет соответствие версий
3. ✅ Собирает прошивку для production
4. ✅ Создает GitHub Release с файлами:
   - `firmware.bin` - прошивка для ESP32
   - `manifest.json` - метаданные прошивки

### Типичные сценарии использования

#### Разработка (поднять версию без релиза)
```powershell
# После добавления новых функций
.\scripts\release.ps1 version -Type minor -Message "Добавлены новые культуры"
```

#### Релиз (создать официальный релиз)
```powershell
# Когда готовы к релизу
.\scripts\release.ps1 release -Version "3.6.3" -Message "Релиз с новыми функциями"
```

#### Быстрый patch релиз
```powershell
# Исправление бага
.\scripts\release.ps1 bump -Type patch
```

### Устранение проблем

#### Ошибка "Python не найден"
```bash
# Установите Python и добавьте в PATH
# https://www.python.org/downloads/
```

#### Ошибка "Git не найден"
```bash
# Установите Git
# https://git-scm.com/downloads
```

#### Конфликты версий
```bash
# Синхронизируйте версии
.\scripts\release.ps1 sync
```

#### Ошибка коммита
```bash
# Проверьте git статус
git status

# Добавьте изменения вручную
git add VERSION include/version.h platformio.ini
git commit -m "version: обновлена до X.X.X"
```

## 📋 Другие скрипты

### `auto_version.py`
Автоматически генерирует `include/version.h` из файла `VERSION` при сборке PlatformIO.

### `format_all.ps1`
Форматирует весь код проекта.

### `gen_docs.ps1`
Генерирует документацию с помощью Doxygen.

## 🎯 Принципы

1. **Один источник истины** - вся логика версий в `release_manager.py`
2. **Разделение ответственности** - поднятие версии и создание релиза отдельно
3. **Автоматизация** - минимум ручных действий
4. **Надёжность** - проверки и валидация на каждом шаге 