# 🚀 Автоматические скрипты релиза JXCT

## 📋 Описание

Автоматические скрипты для создания релизов проекта JXCT без необходимости запоминать все шаги.

## 🎯 Что делают скрипты

### ✅ Полный автоматический цикл релиза:
1. **Повышают версию** в файлах `VERSION` и `manifest.json`
2. **Собирают проект** с `pio run`
3. **Создают Git коммит** с подробным описанием
4. **Создают Git тег** вида `v3.2.8`
5. **Отправляют на GitHub** (`git push origin main --tags`)

## 🛠️ Использование

### PowerShell (рекомендуется)

```powershell
# Patch версия (3.2.7 -> 3.2.8) - по умолчанию
.\scripts\release.ps1

# Minor версия (3.2.7 -> 3.3.0)
.\scripts\release.ps1 minor

# Major версия (3.2.7 -> 4.0.0)  
.\scripts\release.ps1 major

# Автоматически без подтверждения
.\scripts\release.ps1 patch -Auto
```

### Python (альтернатива)

```bash
# Patch версия
python scripts/release_cycle.py

# Minor версия
python scripts/release_cycle.py minor

# Major версия  
python scripts/release_cycle.py major

# Без подтверждения
python scripts/release_cycle.py patch --auto
```

## 📊 Типы версий

| Тип | Описание | Пример |
|-----|----------|--------|
| `patch` | Исправления багов | 3.2.7 → 3.2.8 |
| `minor` | Новые функции | 3.2.7 → 3.3.0 |
| `major` | Кардинальные изменения | 3.2.7 → 4.0.0 |

## 🎉 Результат

После успешного выполнения:

✅ **Версия повышена** во всех файлах  
✅ **Проект собран** и готов к прошивке  
✅ **Релиз создан** на GitHub  
✅ **Тег отправлен** на удаленный репозиторий  

## 💡 Следующие шаги

После создания релиза можете:

```powershell
# Прошить устройство новой версией
pio run --target upload

# Или просто открыть монитор порта
pio device monitor
```

## ⚠️ Требования

- Python 3.6+
- PlatformIO
- Git настроен для push в репозиторий
- Запуск из корневой директории проекта

## 🔧 Файлы скриптов

- `release_cycle.py` - основной Python скрипт
- `release.ps1` - PowerShell обертка для удобства
- `auto_version.py` - автоматическое обновление version.h при сборке 

# 📜 Скрипты проекта

| Скрипт | Назначение |
|--------|------------|
| `release.ps1` | Автоматический релиз (patch/minor/major) + production build |
| `release_cycle.py` | Логика инкремента SemVer, правка `VERSION`, `manifest.json` |
| `auto_version.py` | Генерация build meta для CI (Git hash, дата) |
| `build_fs.ps1` | Сборка `web_spiffs.bin` из `web/` (генерируется Vite) |
| `gen_docs.ps1` | Запуск Doxygen + копирование HTML в `docs/html` |

Запуск примеров:
```powershell
# Патч-релиз (vX.Y.Z -> vX.Y.Z+1)
./scripts/release.ps1 -Auto

# Сборка образа UI
./scripts/build_fs.ps1
``` 