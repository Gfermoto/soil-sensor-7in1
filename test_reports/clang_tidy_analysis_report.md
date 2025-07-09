# CLANG-TIDY ПОЛНЫЙ ОТЧЁТ АНАЛИЗА
**Дата анализа:** 09.07.2025 00:32
**Версия clang-tidy:** 20.1.0

## 📊 СТАТИСТИКА
- **Всего файлов проанализировано:** 24
- **Успешно проанализировано:** 24
- **Ошибки анализа:** 0
- **Всего предупреждений:** 12

## 🔍 КАТЕГОРИИ ПРОБЛЕМ
### Читаемость (10 проблем)
- `readability-static-accessed-through-instance`: 7 случаев
- `readability-implicit-bool-conversion`: 2 случаев
- `readability-redundant-declaration`: 1 случаев

### Прочее (1 проблем)
- `performance-enum-size`: 1 случаев

### Модернизация (1 проблем)
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

### src/wifi_manager.cpp (9 предупреждений)
**Прочее:** 1 проблем
- src\wifi_manager.cpp:23:12:: enum 'WifiConstants' uses a larger base type ('int', size: 4 bytes) than necessary for its value set, consider using 'std::uint16_t' (2 bytes) as the base type to reduce its size [performance-enum-size]

**Читаемость:** 8 проблем
- src\wifi_manager.cpp:118:5:: static member accessed through instance [readability-static-accessed-through-instance]
- src\wifi_manager.cpp:177:13:: static member accessed through instance [readability-static-accessed-through-instance]
- src\wifi_manager.cpp:258:5:: static member accessed through instance [readability-static-accessed-through-instance]
- ... и ещё 5 проблем

### src/web/csrf_protection.cpp ✅ Проблем не найдено

### src/web/error_handlers.cpp ✅ Проблем не найдено

### src/web/routes_config.cpp ✅ Проблем не найдено

### src/web/routes_data.cpp ✅ Проблем не найдено

### src/web/routes_main.cpp ✅ Проблем не найдено

### src/web/routes_ota.cpp ✅ Проблем не найдено

### src/web/routes_reports.cpp ✅ Проблем не найдено

### src/web/routes_service.cpp ✅ Проблем не найдено

### src/web/web_templates.cpp (1 предупреждений)
**Модернизация:** 1 проблем
- src\web\web_templates.cpp:91:16:: avoid repeating the return type from the declaration; use a braced initializer list instead [modernize-return-braced-init-list]

## 🎯 РЕКОМЕНДАЦИИ ПО ПРИОРИТЕТАМ

### 🔴 КРИТИЧЕСКИЕ (исправить немедленно)

### 🟡 СРЕДНИЕ (исправить в ближайшее время)
- Проблемы читаемости - затрудняют сопровождение кода
- Модернизация - улучшают качество кода

### 🟢 НИЗКИЕ (можно исправить позже)
- Прочие стилистические замечания
