# Управление версиями JXCT

**Дата:** Июль 2025
**Версия:** 3.10.0

**Автор:** JXCT Development Team

---

## 📋 Содержание {#Soderzhanie}

- [Содержание](#Soderzhanie)
- [Обзор](#Obzor)
- [Файл версии](#Fayl-versii)
- [Как изменить версию](#Kak-izmenit-versiyu)
  - [Простой способ](#Prostoy-sposob)
  - [Автоматическое обновление](#Avtomaticheskoe-obnovlenie)
- [Доступные макросы](#Dostupnye-makrosy)
  - [Основные макросы версии](#Osnovnye-makrosy-versii)
  - [Информация о сборке](#Informatsiya-o-sborke)
  - [Константы устройства](#Konstanty-ustroystva)
- [Сравнение версий](#Sravnenie-versiy)
- [Преимущества](#Preimushchestva)
  - [До (проблемы)](#Do-problemy)
  - [После (решение)](#Posle-reshenie)
- [Примеры использования](#Primery-ispolzovaniya)
  - [В коде C](#V-kode-c)
  - [В MQTT сообщениях](#V-mqtt-soobshcheniyah)
  - [В веб-интерфейсе](#V-veb-interfeyse)
- [Процесс релиза](#Protsess-reliza)
- [Важные замечания](#Vazhnye-zamechaniya)
- [Результат](#Rezultat)
- [Поддержка](#Podderzhka)
  - [Связь с разработчиками](#Svyaz-s-razrabotchikami)
  - [Дополнительные ресурсы](#Dopolnitelnye-resursy)
  - [Полезные ссылки](#Poleznye-ssylki)

---

## 📖 Содержание {#Soderzhanie}

1. **🎯 Обзор**
2. **📁 Файл версии**
3. **🔧 Как изменить версию**
4. **📋 Доступные макросы**
5. **🔍 Сравнение версий**
6. **🚀 Преимущества**
7. **📝 Примеры использования**
8. **🔄 Процесс релиза**
9. **⚠️ Важные замечания**
10. **🎯 Результат**

---

## 🎯 Обзор {#Obzor}

Начиная с версии 2.4.5, проект JXCT использует **централизованное управление версией**. Это означает, что версия определяется в одном месте и автоматически обновляется во всех частях проекта.

## 📁 Файл версии {#Fayl-versii}

**Файл:** `include/version.h`

Это единственное место, где нужно изменять версию проекта.

## 🔧 Как изменить версию {#Kak-izmenit-versiyu}

### Простой способ {#Prostoy-sposob}
Отредактируйте файл `include/version.h` и измените только эти три строки:

```cpp
#define JXCT_VERSION_MAJOR 2    // Основная версия
#define JXCT_VERSION_MINOR 4    // Минорная версия
#define JXCT_VERSION_PATCH 5    // Патч версия
```

**Пример:** Для версии 2.5.0:
```cpp
#define JXCT_VERSION_MAJOR 2
#define JXCT_VERSION_MINOR 5
#define JXCT_VERSION_PATCH 0
```

### Автоматическое обновление {#Avtomaticheskoe-obnovlenie}

После изменения версии в `version.h`, она автоматически обновится в:

- ✅ **MQTT сообщениях** (`sw_version` в Home Assistant)
- ✅ **Веб-интерфейсе** (все страницы)
- ✅ **Баннере запуска** в Serial Monitor
- **API ответах** (`/api/v1/sensor`, `/health`)
- ✅ **Логах системы**
- ✅ **OTA обновлениях**

## 📋 Доступные макросы {#Dostupnye-makrosy}

### Основные макросы версии {#Osnovnye-makrosy-versii}
```cpp
JXCT_VERSION_MAJOR          // 2
JXCT_VERSION_MINOR          // 4
JXCT_VERSION_PATCH          // 5
JXCT_VERSION_STRING         // "2.4.5"
JXCT_VERSION_CODE           // 20405 (для сравнения)
```

### Информация о сборке {#Informatsiya-o-sborke}
```cpp
JXCT_BUILD_DATE             // "Dec 25 2024"
JXCT_BUILD_TIME             // "15:30:45"
JXCT_FULL_VERSION_STRING    // "2.4.5 (built Dec 25 2024 15:30:45)"
```

### Константы устройства {#Konstanty-ustroystva}
```cpp
DEVICE_MANUFACTURER[]       // "Eyera"
DEVICE_MODEL[]             // "JXCT-7in1"
DEVICE_SW_VERSION[]        // "2.4.5" (автоматически)
FIRMWARE_VERSION           // "2.4.5" (для совместимости)
```

## 🔍 Сравнение версий {#Sravnenie-versiy}

Для проверки версии в коде:

```cpp
// Проверка минимальной версии
#if JXCT_VERSION_AT_LEAST(2, 4, 5)
    // Код для версии 2.4.5 и выше
#endif

// Проверка точной версии
#if JXCT_VERSION_CODE == 20405  // 2.4.5
    // Код только для версии 2.4.5
#endif
```

## 🚀 Преимущества {#Preimushchestva}

### ✅ До (проблемы): {#Do-problemy}
- Версия была разбросана по 4+ файлам
- При обновлении легко забыть какой-то файл
- Версии в MQTT и веб-интерфейсе могли не совпадать
- Ручное обновление каждого места

### ✅ После (решение): {#Posle-reshenie}
- **Одно место** для изменения версии
- **Автоматическое обновление** везде
- **Гарантированная согласованность**
- **Простота сопровождения**

## 📝 Примеры использования {#Primery-ispolzovaniya}

### В коде C++ {#V-kode-c}
```cpp
#include "version.h"

void printVersion() {
    Serial.println("Версия: " JXCT_VERSION_STRING);
    Serial.println("Полная версия: " JXCT_FULL_VERSION_STRING);
}
```

### В MQTT сообщениях {#V-mqtt-soobshcheniyah}
```cpp
doc["device"]["sw_version"] = DEVICE_SW_VERSION;  // Автоматически "2.4.5"
```

### В веб-интерфейсе {#V-veb-interfeyse}
```cpp
html += "Версия: " + String(FIRMWARE_VERSION);  // Автоматически "2.4.5"
```

## 🔄 Процесс релиза {#Protsess-reliza}

1. **Измените версию** в `include/version.h`
2. **Скомпилируйте** проект (`pio run`)
3. **Проверьте** что версия обновилась везде
4. **Создайте коммит** с новой версией
5. **Создайте тег** в Git: `git tag v3.10.0`

## ⚠️ Важные замечания {#Vazhnye-zamechaniya}

- **НЕ изменяйте** версию в других файлах - она перезапишется
- **Всегда компилируйте** после изменения версии
- **Используйте семантическое версионирование**: MAJOR.MINOR.PATCH
- **Обновляйте CHANGELOG.md** при изменении версии

## 🎯 Результат {#Rezultat}

Теперь для изменения версии во всем проекте достаточно изменить **3 строки** в **1 файле**!

```cpp
// Было: изменения в 4+ файлах
// Стало: изменения в 1 файле
#define JXCT_VERSION_PATCH 6  // Изменили только эту строку
// Версия автоматически обновилась везде! 🎉
```

## 📞 Поддержка {#Podderzhka}

### 💬 Связь с разработчиками {#Svyaz-s-razrabotchikami}
- **Telegram:** [@Gfermoto](https://t.me/Gfermoto)
- **GitHub Issues:** [Сообщить о проблеме](https://github.com/Gfermoto/soil-sensor-7in1/issues)
- **Документация:** [GitHub Pages](https://gfermoto.github.io/soil-sensor-7in1/)

### 📚 Дополнительные ресурсы {#Dopolnitelnye-resursy}
- [Руководство пользователя](USER_GUIDE.md)
- [Техническая документация](TECHNICAL_DOCS.md)
- [Агрономические рекомендации](AGRO_RECOMMENDATIONS.md)
- [Руководство по компенсации](COMPENSATION_GUIDE.md)
- [API документация](API.md)
- [Управление конфигурацией](CONFIG_MANAGEMENT.md)
- [Схема подключения](WIRING_DIAGRAM.md)
- [Протокол Modbus](MODBUS_PROTOCOL.md)

### 🔗 Полезные ссылки {#Poleznye-ssylki}

- [🌱 GitHub репозиторий](https://github.com/Gfermoto/soil-sensor-7in1) - Исходный код проекта
- [📋 План рефакторинга](../dev/REFACTORING_PLAN.md) - Планы развития
- [📊 Отчет о техническом долге](../dev/TECHNICAL_DEBT_REPORT.md) - Анализ технических проблем
- [🏗️ Архитектура системы](../dev/ARCH_OVERALL.md) - Общая архитектура проекта
