# CLANG-TIDY ПОЛНЫЙ ОТЧЁТ АНАЛИЗА
**Дата анализа:** 08.07.2025 23:01
**Версия clang-tidy:** 20.1.0

## 📊 СТАТИСТИКА
- **Всего файлов проанализировано:** 24
- **Успешно проанализировано:** 24
- **Ошибки анализа:** 0
- **Всего предупреждений:** 47

## 🔍 КАТЕГОРИИ ПРОБЛЕМ
### Читаемость (29 проблем)
- ``: 23 случаев
- `readability-implicit-bool-conversion`: 2 случаев
- `readability-qualified-auto`: 2 случаев
- `readability-static-accessed-through-instance`: 1 случаев
- `readability-identifier-length`: 1 случаев

### Разное (7 проблем)
- `misc-use-anonymous-namespace`: 4 случаев
- `misc-use-internal-linkage`: 3 случаев

### Прочее (1 проблем)
- `performance-enum-size`: 1 случаев

### Модернизация (10 проблем)
- `modernize-raw-string-literal`: 8 случаев
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

### src/ota_manager.cpp (5 предупреждений)
**Разное:** 3 проблем
- src\ota_manager.cpp:34:13:: function 'printGuard' declared 'static', move to anonymous namespace instead [misc-use-anonymous-namespace]
- src\ota_manager.cpp:127:13:: function 'initializeDownload' declared 'static', move to anonymous namespace instead [misc-use-anonymous-namespace]
- src\ota_manager.cpp:208:13:: function 'downloadData' declared 'static', move to anonymous namespace instead [misc-use-anonymous-namespace]

**Читаемость:** 2 проблем
- src\ota_manager.cpp:347:5:: 'auto http' can be declared as 'auto *http' [readability-qualified-auto]
- src\ota_manager.cpp:370:5:: 'auto shaCtx' can be declared as 'auto *shaCtx' [readability-qualified-auto]

### src/routes_calibration.cpp ✅ Проблем не найдено

### src/sensor_compensation.cpp ✅ Проблем не найдено

### src/thingspeak_client.cpp (1 предупреждений)
**Разное:** 1 проблем
- src\thingspeak_client.cpp:16:13:: variable 'THINGSPEAK_API_URL' can be made static or moved into an anonymous namespace to enforce internal linkage [misc-use-internal-linkage]

### src/validation_utils.cpp ✅ Проблем не найдено

### src/wifi_manager.cpp (2 предупреждений)
**Прочее:** 1 проблем
- src\wifi_manager.cpp:23:12:: enum 'WifiConstants' uses a larger base type ('uint32_t' (aka 'unsigned int'), size: 4 bytes) than necessary for its value set, consider using 'std::uint16_t' (2 bytes) as the base type to reduce its size [performance-enum-size]

**Читаемость:** 1 проблем
- src\wifi_manager.cpp:118:5:: static member accessed through instance [readability-static-accessed-through-instance]

### src/web/csrf_protection.cpp ✅ Проблем не найдено

### src/web/error_handlers.cpp (2 предупреждений)
**Разное:** 2 проблем
- src\web\error_handlers.cpp:207:6:: function 'logWebRequest' can be made static or moved into an anonymous namespace to enforce internal linkage [misc-use-internal-linkage]
- src\web\error_handlers.cpp:277:6:: function 'checkRouteAccess' can be made static or moved into an anonymous namespace to enforce internal linkage [misc-use-internal-linkage]

### src/web/routes_config.cpp (26 предупреждений)
**Разное:** 1 проблем
- src\web\routes_config.cpp:23:13:: function 'sendConfigExportJson' declared 'static', move to anonymous namespace instead [misc-use-anonymous-namespace]

**Модернизация:** 1 проблем
- src\web\routes_config.cpp:364:68:: escaped string literal can be written as a raw string literal [modernize-raw-string-literal]

**Читаемость:** 24 проблем
- src\web\routes_config.cpp:444:5:: confusing array subscript expression, usually the index is inside the [] [readability-misplaced-array-index]
- src\web\routes_config.cpp:445:5:: confusing array subscript expression, usually the index is inside the [] [readability-misplaced-array-index]
- src\web\routes_config.cpp:446:5:: confusing array subscript expression, usually the index is inside the [] [readability-misplaced-array-index]
- ... и ещё 21 проблем

### src/web/routes_data.cpp (2 предупреждений)
**Модернизация:** 2 проблем
- src\web\routes_data.cpp:321:64:: escaped string literal can be written as a raw string literal [modernize-raw-string-literal]
- src\web\routes_data.cpp:793:21:: escaped string literal can be written as a raw string literal [modernize-raw-string-literal]

### src/web/routes_main.cpp ✅ Проблем не найдено

### src/web/routes_ota.cpp (5 предупреждений)
**Модернизация:** 5 проблем
- src\web\routes_ota.cpp:44:81:: escaped string literal can be written as a raw string literal [modernize-raw-string-literal]
- src\web\routes_ota.cpp:58:81:: escaped string literal can be written as a raw string literal [modernize-raw-string-literal]
- src\web\routes_ota.cpp:442:65:: escaped string literal can be written as a raw string literal [modernize-raw-string-literal]
- ... и ещё 2 проблем

### src/web/routes_reports.cpp ✅ Проблем не найдено

### src/web/routes_service.cpp ✅ Проблем не найдено

### src/web/web_templates.cpp (2 предупреждений)
**Модернизация:** 2 проблем
- src\web\web_templates.cpp:57:20:: use default member initializer for 'required' [modernize-use-default-member-init]
- src\web\web_templates.cpp:91:16:: avoid repeating the return type from the declaration; use a braced initializer list instead [modernize-return-braced-init-list]

## 🎯 РЕКОМЕНДАЦИИ ПО ПРИОРИТЕТАМ

### 🔴 КРИТИЧЕСКИЕ (исправить немедленно)

### 🟡 СРЕДНИЕ (исправить в ближайшее время)
- Проблемы читаемости - затрудняют сопровождение кода
- Модернизация - улучшают качество кода

### 🟢 НИЗКИЕ (можно исправить позже)
- Разные мелкие проблемы
- Прочие стилистические замечания
