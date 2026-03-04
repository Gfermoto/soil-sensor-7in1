# Управление конфигурацией JXCT 7-в-1

**Версия:** 3.13.2
**Дата:** Июль 2025
**Автор:** JXCT Development Team

---

## 📋 Содержание {#Soderzhanie}

- [Содержание](#Soderzhanie)
- [Обзор](#obzor)
- [Веб-интерфейс](#web-interfeys)
  - [Доступ к управлению конфигурацией](#Dostup-k-upravleniyu-konfiguratsiey)
  - [Основные функции](#Osnovnye-funktsii)
- [Экспорт конфигурации](#eksport)
  - [Что экспортируется](#Chto-eksportiruetsya)
  - [Что заменяется заглушками (по безопасности)](#Chto-zamenyaetsya-zaglushkami-po-bezopasnosti)
  - [Что НЕ экспортируется](#Chto-NE-eksportiruetsya)
  - [Пример экспортированного файла](#Primer-eksportirovannogo-fayla)
- [Импорт конфигурации](#import)
  - [Поддерживаемые форматы](#Podderzhivaemye-formaty)
  - [Процесс импорта](#Protsess-importa)
  - [Обработка ошибок](#Obrabotka-oshibok)
- [Массовое развертывание](#massovoe-razvertyvanie)
  - [Шаблон конфигурации](#Shablon-konfiguratsii)
  - [Заглушки в шаблоне](#Zaglushki-v-shablone)
  - [Процесс массового развертывания](#Protsess-massovogo-razvertyvaniya)
  - [Пример готового файла для продакшена](#Primer-gotovogo-fayla-dlya-prodakshena)
- [Технические детали](#tehnicheskie-detali)
  - [Парсер JSON](#Parser-json)
  - [Безопасность](#Bezopasnost)
  - [Ограничения](#Ogranicheniya)
- [Отладка](#otladka)
  - [Логи в Serial Monitor](#Logi-v-serial-monitor)
  - [Типичные ошибки](#Tipichnye-oshibki)
- [Связанная документация](#svyazannaya-dokumentaciya)
- [История изменений](#istoriya-izmeneniy)
  - [v3.10.0](#v3100)
- [Поддержка](#podderzhka)
  - [Связь с разработчиками](#Svyaz-s-razrabotchikami)
  - [Дополнительные ресурсы](#Dopolnitelnye-resursy)
  - [Полезные ссылки](#Poleznye-ssylki)

---

## 📖 Содержание {#Soderzhanie}

1. **🎯 Обзор**
2. **🌐 Веб-интерфейс**
3. **📤 Экспорт конфигурации**
4. **📥 Импорт конфигурации**
5. **🏭 Массовое развертывание**
6. **🔧 Технические детали**
7. **🐛 Отладка**
8. **📋 Связанная документация**
9. **🔄 История изменений**

---

## 🎯 Обзор {#obzor}

Система JXCT поддерживает полноценное управление конфигурацией через веб-интерфейс с возможностью экспорта и импорта настроек в формате JSON.

## 🌐 Веб-интерфейс {#web-interfeys}

### Доступ к управлению конфигурацией {#Dostup-k-upravleniyu-konfiguratsiey}
```
http://IP_УСТРОЙСТВА/config_manager
```

### Основные функции {#Osnovnye-funktsii}
- 📤 **Экспорт настроек** - сохранение текущей конфигурации
- 📥 **Импорт настроек** - загрузка конфигурации из файла
- 🔄 **Автоматическая перезагрузка** после импорта
- ⚠️ **Безопасность** - исключение критических данных из экспорта

## 📤 Экспорт конфигурации {#eksport}

### Что экспортируется {#Chto-eksportiruetsya}
- ✅ **MQTT настройки**: server, port, user, enabled
- ✅ **ThingSpeak настройки**: channel_id, enabled
- ✅ **Интервалы**: sensor_read, mqtt_publish, thingspeak, web_update
- ✅ **Дельта-фильтры**: temperature, humidity, ph, ec, npk
- ✅ **Скользящее среднее**: window, force_cycles, algorithm, outlier_filter
- ✅ **Флаги**: hass_enabled, real_sensor

### Что заменяется заглушками (по безопасности) {#Chto-zamenyaetsya-zaglushkami-po-bezopasnosti}
- 🔒 **MQTT сервер** → `YOUR_MQTT_SERVER_HERE`
- 🔒 **MQTT пользователь** → `YOUR_MQTT_USER_HERE`
- 🔒 **MQTT пароль** → `YOUR_MQTT_PASSWORD_HERE`
- 🔒 **ThingSpeak канал** → `YOUR_CHANNEL_ID_HERE`
- 🔒 **ThingSpeak API ключ** → `YOUR_API_KEY_HERE`

### Что НЕ экспортируется {#Chto-NE-eksportiruetsya}
- ❌ **WiFi настройки** (ssid, password)
- ❌ **MAC-зависимые поля** (topic_prefix, device_name)
- ❌ **Системная информация** (version, exported timestamp)

### Пример экспортированного файла {#Primer-eksportirovannogo-fayla}
```json
{
  "mqtt": {
    "enabled": true,
    "server": "192.168.2.11",
    "port": 1883,
    "user": "mqtt"
  },
  "thingspeak": {
    "enabled": true,
    "channel_id": "2952280"
  },
  "intervals": {
    "sensor_read": 5000,
    "mqtt_publish": 60000,
    "thingspeak": 900000,
    "web_update": 5000
  },
  "delta_filter": {
    "temperature": 0.10,
    "humidity": 0.50,
    "ph": 0.01,
    "ec": 10.00,
    "npk": 1.00
  },
  "moving_average": {
    "window": 5,
    "force_cycles": 5,
    "algorithm": 0,
    "outlier_filter": 0
  },
  "flags": {
    "hass_enabled": true,
    "real_sensor": true
  }
}
```

## 📥 Импорт конфигурации {#import}

### Поддерживаемые форматы {#Podderzhivaemye-formaty}
- **JSON** - единственный поддерживаемый формат
- **Одна строка** - JSON должен быть в одну строку без форматирования
- **UTF-8** - кодировка файла

### Процесс импорта {#Protsess-importa}
1. **Выбор файла** - через веб-интерфейс
2. **Загрузка** - файл передается на устройство
3. **Парсинг** - JSON разбирается и проверяется
4. **Применение** - настройки записываются в NVS
5. **Перезагрузка** - устройство автоматически перезагружается

### Обработка ошибок {#Obrabotka-oshibok}
- **Неверный JSON** - сообщение об ошибке парсинга
- **Пустой файл** - предупреждение о пустом содержимом
- **Недоступность в AP режиме** - импорт отключен в режиме точки доступа

## 🏭 Массовое развертывание {#massovoe-razvertyvanie}

### Шаблон конфигурации {#Shablon-konfiguratsii}
Используйте файл `test_safe_config.json` как шаблон для массового развертывания.

### Заглушки в шаблоне {#Zaglushki-v-shablone}
- `YOUR_MQTT_SERVER_HERE` - адрес MQTT сервера
- `YOUR_MQTT_USER_HERE` - имя пользователя MQTT
- `YOUR_MQTT_PASSWORD_HERE` - пароль MQTT
- `YOUR_CHANNEL_ID_HERE` - ID канала ThingSpeak
- `YOUR_API_KEY_HERE` - API ключ ThingSpeak

### Процесс массового развертывания {#Protsess-massovogo-razvertyvaniya}
1. **Копирование шаблона**
   ```bash
   cp test_safe_config.json production_config.json
   ```

2. **Замена заглушек** (в текстовом редакторе)
   - Найти и заменить все заглушки на реальные значения
   - Использовать функцию "Найти и заменить" для быстрой замены

3. **Применение на устройствах**
   - Загрузить готовый файл на каждое устройство
   - Устройства автоматически перезагрузятся с новыми настройками

### Пример готового файла для продакшена {#Primer-gotovogo-fayla-dlya-prodakshena}
```json
{"mqtt":{"enabled":true,"server":"192.168.4.1","port":1883,"user":"sensor_user","password":"***"},"thingspeak":{"enabled":true,"channel_id":"1234567","api_key":"YOUR_API_KEY"},"intervals":{"sensor_read":5000,"mqtt_publish":60000,"thingspeak":900000,"web_update":5000},"delta_filter":{"temperature":0.10,"humidity":0.50,"ph":0.01,"ec":10.00,"npk":1.00},"moving_average":{"window":5,"force_cycles":5,"algorithm":0,"outlier_filter":0},"flags":{"hass_enabled":true,"real_sensor":true}}
```

## 🔧 Технические детали {#tehnicheskie-detali}

### Парсер JSON {#Parser-json}
- **Простой парсер** - без использования ArduinoJson для экономии памяти
- **Секционный поиск** - поиск значений в соответствующих секциях JSON
- **Игнорирование заглушек** - заглушки не применяются к конфигурации
- **Отладочные сообщения** - детальные логи в Serial Monitor

### Безопасность {#Bezopasnost}
- **Фильтрация полей** - критические данные не экспортируются
- **Проверка режима** - импорт недоступен в AP режиме
- **Валидация данных** - проверка корректности JSON
- **Уникальные идентификаторы** - автоматическая генерация по MAC адресу

### Ограничения {#Ogranicheniya}
- **Размер файла** - ограничен доступной памятью ESP32
- **Формат JSON** - только одна строка без форматирования
- **Кодировка** - только UTF-8
- **Режим работы** - импорт недоступен в AP режиме

## 🐛 Отладка {#otladka}

### Логи в Serial Monitor {#Logi-v-serial-monitor}
```
⚙️ Начало загрузки файла конфигурации: config.json
⚙️ Загрузка завершена, размер: 425 байт
[IMPORT] MQTT enabled: 1, server: 192.168.2.11, port: 1883, user: mqtt
[IMPORT] ThingSpeak enabled: 1, channel: 2952280, interval: 900000
[IMPORT] Intervals - sensor: 5000, mqtt: 60000, ts: 900000, web: 5000
[IMPORT] Flags - hass: 1, real_sensor: 1
✅ JSON конфигурация успешно распарсена
✅ Конфигурация сохранена
✅ Конфигурация импортирована успешно
```

### Типичные ошибки {#Tipichnye-oshibki}
- **"Пустой файл"** - файл не содержит данных
- **"Ошибка парсинга JSON"** - неверный формат JSON
- **"Import недоступен в режиме AP"** - устройство в режиме точки доступа
- **"Not found: /api/config/import"** - устройство не подключено к сети

## 📋 Связанная документация {#svyazannaya-dokumentaciya}

- [Пример конфигурации](../examples/test_safe_config.json) - шаблон для массового развёртывания
- [API.md](API.md) - REST API для управления конфигурацией
- [План рефакторинга QA](../dev/REFACTORING_PLAN.md) - планы развития

## 🔄 История изменений {#istoriya-izmeneniy}

### v3.10.0 {#v3100}
- ✅ Реализован полноценный импорт/экспорт конфигурации
- ✅ Добавлен шаблон для массового развертывания
- ✅ Исправлен парсер JSON для работы с вложенными секциями
- ✅ Добавлена поддержка заглушек в шаблоне
- ✅ Улучшена отладка и логирование
- ✅ Исправлен редирект после импорта

## 📞 Поддержка {#podderzhka}

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
- [Схема подключения](WIRING_DIAGRAM.md)
- [Протокол Modbus](MODBUS_PROTOCOL.md)
- [Управление версиями](VERSION_MANAGEMENT.md)

### 🔗 Полезные ссылки {#Poleznye-ssylki}

- [🌱 GitHub репозиторий](https://github.com/Gfermoto/soil-sensor-7in1) - Исходный код проекта
- [📋 План рефакторинга](../dev/REFACTORING_PLAN.md) - Планы развития
- [📊 Отчет о техническом долге](../dev/TECHNICAL_DEBT_REPORT.md) - Анализ технических проблем
- [🏗️ Архитектура системы](../dev/ARCH_OVERALL.md) - Общая архитектура проекта
