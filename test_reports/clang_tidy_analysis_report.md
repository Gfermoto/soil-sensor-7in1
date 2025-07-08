# CLANG-TIDY ПОЛНЫЙ ОТЧЁТ АНАЛИЗА
**Дата анализа:** 08.07.2025 23:57
**Версия clang-tidy:** 20.1.0

## 📊 СТАТИСТИКА
- **Всего файлов проанализировано:** 24
- **Успешно проанализировано:** 24
- **Ошибки анализа:** 0
- **Всего предупреждений:** 37

## 🔍 КАТЕГОРИИ ПРОБЛЕМ
### Читаемость (33 проблем)
- ``: 23 случаев
- `readability-static-accessed-through-instance`: 7 случаев
- `readability-implicit-bool-conversion`: 2 случаев
- `readability-identifier-length`: 1 случаев

### Прочее (1 проблем)
- `performance-enum-size`: 1 случаев

### Разное (1 проблем)
- `misc-use-internal-linkage`: 1 случаев

### Модернизация (2 проблем)
- `modernize-use-default-member-init`: 1 случаев
- `modernize-return-braced-init-list`: 1 случаев

## 📁 ДЕТАЛЬНЫЙ АНАЛИЗ ПО ФАЙЛАМ
### src/calibration_manager.cpp (2 предупреждений)
**Читаемость:** 2 проблем
- src\calibration_manager.cpp:56:12:: implicit conversion 'int' -> 'bool' [readability-implicit-bool-conversion]
- src\calibration_manager.cpp:83:12:: implicit conversion 'int' -> 'bool' [readability-implicit-bool-conversion]

### src/config.cpp ✅ Проблем не найдено

### src/fake_sensor.cpp ✅ Проблем не найдено

### src/jxct_format_utils.cpp ✅ Проблем не найдено

### src/jxct_ui_system.cpp ✅ Проблем не найдено

### src/logger.cpp ✅ Проблем не найдено

### src/main.cpp ✅ Проблем не найдено

### src/modbus_sensor.cpp ✅ Проблем не найдено

### src/mqtt_client.cpp ✅ Проблем не найдено

### src/ota_manager.cpp ✅ Проблем не найдено

### src/routes_calibration.cpp ✅ Проблем не найдено

### src/sensor_compensation.cpp ✅ Проблем не найдено

### src/thingspeak_client.cpp ✅ Проблем не найдено

### src/validation_utils.cpp ✅ Проблем не найдено

### src/wifi_manager.cpp (8 предупреждений)
**Прочее:** 1 проблем
- src\wifi_manager.cpp:23:12:: enum 'WifiConstants' uses a larger base type ('uint32_t' (aka 'unsigned int'), size: 4 bytes) than necessary for its value set, consider using 'std::uint16_t' (2 bytes) as the base type to reduce its size [performance-enum-size]

**Читаемость:** 7 проблем
- src\wifi_manager.cpp:118:5:: static member accessed through instance [readability-static-accessed-through-instance]
- src\wifi_manager.cpp:177:13:: static member accessed through instance [readability-static-accessed-through-instance]
- src\wifi_manager.cpp:235:5:: static member accessed through instance [readability-static-accessed-through-instance]
- ... и ещё 4 проблем

### src/web/csrf_protection.cpp ✅ Проблем не найдено

### src/web/error_handlers.cpp ✅ Проблем не найдено

### src/web/routes_config.cpp (25 предупреждений)
**Разное:** 1 проблем
- src\web\routes_config.cpp:24:6:: function 'sendConfigExportJson' can be made static or moved into an anonymous namespace to enforce internal linkage [misc-use-internal-linkage]

**Читаемость:** 24 проблем
- src\web\routes_config.cpp:442:5:: confusing array subscript expression, usually the index is inside the [] [readability-misplaced-array-index]
- src\web\routes_config.cpp:443:5:: confusing array subscript expression, usually the index is inside the [] [readability-misplaced-array-index]
- src\web\routes_config.cpp:444:5:: confusing array subscript expression, usually the index is inside the [] [readability-misplaced-array-index]
- ... и ещё 21 проблем

### src/web/routes_data.cpp ✅ Проблем не найдено

### src/web/routes_main.cpp ✅ Проблем не найдено

### src/web/routes_ota.cpp ✅ Проблем не найдено

### src/web/routes_reports.cpp ✅ Проблем не найдено

### src/web/routes_service.cpp ✅ Проблем не найдено

### src/web/web_templates.cpp (2 предупреждений)
**Модернизация:** 2 проблем
- src\web\web_templates.cpp:71:37:: member initializer for 'required' is redundant [modernize-use-default-member-init]
- src\web\web_templates.cpp:91:16:: avoid repeating the return type from the declaration; use a braced initializer list instead [modernize-return-braced-init-list]

## 🎯 РЕКОМЕНДАЦИИ ПО ПРИОРИТЕТАМ

### 🔴 КРИТИЧЕСКИЕ (исправить немедленно)

### 🟡 СРЕДНИЕ (исправить в ближайшее время)
- Проблемы читаемости - затрудняют сопровождение кода
- Модернизация - улучшают качество кода

### 🟢 НИЗКИЕ (можно исправить позже)
- Разные мелкие проблемы
- Прочие стилистические замечания
