# CLANG-TIDY ПОЛНЫЙ ОТЧЁТ АНАЛИЗА
**Дата анализа:** 08.07.2025 20:57
**Версия clang-tidy:** 20.1.0

## 📊 СТАТИСТИКА
- **Всего файлов проанализировано:** 24
- **Успешно проанализировано:** 24
- **Ошибки анализа:** 0
- **Всего предупреждений:** 70

## 🔍 КАТЕГОРИИ ПРОБЛЕМ
### Читаемость (32 проблем)
- ``: 23 случаев
- `readability-implicit-bool-conversion`: 6 случаев
- `readability-static-accessed-through-instance`: 2 случаев
- `readability-identifier-length`: 1 случаев

### Потенциальные баги (19 проблем)
- `bugprone-easily-swappable-parameters`: 12 случаев
- `bugprone-branch-clone`: 4 случаев
- `bugprone-too-small-loop-variable`: 1 случаев
- `bugprone-narrowing-conversions`: 1 случаев
- `bugprone-reserved-identifier,cert-dcl37-c,cert-dcl51-cpp`: 1 случаев

### Разное (8 проблем)
- `misc-use-anonymous-namespace`: 6 случаев
- `misc-use-internal-linkage`: 2 случаев

### Модернизация (10 проблем)
- `modernize-raw-string-literal`: 8 случаев
- `modernize-use-auto`: 2 случаев

### Прочее (1 проблем)
- `performance-enum-size`: 1 случаев

## 📁 ДЕТАЛЬНЫЙ АНАЛИЗ ПО ФАЙЛАМ
### src/calibration_manager.cpp (6 предупреждений)
**Читаемость:** 6 проблем
- src\calibration_manager.cpp:31:9:: implicit conversion 'bool' -> 'int' [readability-implicit-bool-conversion]
- src\calibration_manager.cpp:56:12:: implicit conversion 'int' -> 'bool' [readability-implicit-bool-conversion]
- src\calibration_manager.cpp:83:12:: implicit conversion 'int' -> 'bool' [readability-implicit-bool-conversion]
- ... и ещё 3 проблем

### src/config.cpp ✅ Проблем не найдено

### src/fake_sensor.cpp ✅ Проблем не найдено

### src/jxct_format_utils.cpp (1 предупреждений)
**Потенциальные баги:** 1 проблем
- src\jxct_format_utils.cpp:33:13:: if with identical then and else branches [bugprone-branch-clone]

### src/jxct_ui_system.cpp ✅ Проблем не найдено

### src/logger.cpp (1 предупреждений)
**Читаемость:** 1 проблем
- src\logger.cpp:228:9:: static member accessed through instance [readability-static-accessed-through-instance]

### src/main.cpp (2 предупреждений)
**Разное:** 2 проблем
- src\main.cpp:56:6:: function 'initPreferences' can be made static or moved into an anonymous namespace to enforce internal linkage [misc-use-internal-linkage]
- src\main.cpp:68:6:: function 'resetButtonTask' can be made static or moved into an anonymous namespace to enforce internal linkage [misc-use-internal-linkage]

### src/modbus_sensor.cpp (3 предупреждений)
**Разное:** 1 проблем
- src\modbus_sensor.cpp:430:13:: function 'readSingleRegister' declared 'static', move to anonymous namespace instead [misc-use-anonymous-namespace]

**Потенциальные баги:** 2 проблем
- src\modbus_sensor.cpp:741:33:: loop variable has narrower type 'uint8_t' than iteration's upper bound 'int' [bugprone-too-small-loop-variable]
- src\modbus_sensor.cpp:770:22:: narrowing conversion from 'int' to 'float' [bugprone-narrowing-conversions]

### src/mqtt_client.cpp (1 предупреждений)
**Разное:** 1 проблем
- src\mqtt_client.cpp:105:18:: function 'getCachedIP' declared 'static', move to anonymous namespace instead [misc-use-anonymous-namespace]

### src/ota_manager.cpp (6 предупреждений)
**Потенциальные баги:** 1 проблем
- src\ota_manager.cpp:34:13:: declaration uses identifier '_printGuard', which is reserved in the global namespace [bugprone-reserved-identifier,cert-dcl37-c,cert-dcl51-cpp]

**Разное:** 3 проблем
- src\ota_manager.cpp:34:13:: function '_printGuard' declared 'static', move to anonymous namespace instead [misc-use-anonymous-namespace]
- src\ota_manager.cpp:128:13:: function 'initializeDownload' declared 'static', move to anonymous namespace instead [misc-use-anonymous-namespace]
- src\ota_manager.cpp:209:13:: function 'downloadData' declared 'static', move to anonymous namespace instead [misc-use-anonymous-namespace]

**Модернизация:** 2 проблем
- src\ota_manager.cpp:348:5:: use auto when initializing with new to avoid duplicating the type name [modernize-use-auto]
- src\ota_manager.cpp:371:5:: use auto when initializing with new to avoid duplicating the type name [modernize-use-auto]

### src/routes_calibration.cpp ✅ Проблем не найдено

### src/sensor_compensation.cpp ✅ Проблем не найдено

### src/thingspeak_client.cpp ✅ Проблем не найдено

### src/validation_utils.cpp ✅ Проблем не найдено

### src/wifi_manager.cpp (2 предупреждений)
**Прочее:** 1 проблем
- src\wifi_manager.cpp:23:12:: enum 'WifiConstants' uses a larger base type ('uint32_t' (aka 'unsigned int'), size: 4 bytes) than necessary for its value set, consider using 'std::uint16_t' (2 bytes) as the base type to reduce its size [performance-enum-size]

**Читаемость:** 1 проблем
- src\wifi_manager.cpp:118:5:: static member accessed through instance [readability-static-accessed-through-instance]

### src/web/csrf_protection.cpp ✅ Проблем не найдено

### src/web/error_handlers.cpp (2 предупреждений)
**Потенциальные баги:** 2 проблем
- src\web\error_handlers.cpp:14:21:: 2 adjacent parameters of 'ValidationRange' of similar type ('int') are easily swapped by mistake [bugprone-easily-swappable-parameters]
- src\web\error_handlers.cpp:24:17:: 3 adjacent parameters of 'HttpRequest' of similar type ('const String &') are easily swapped by mistake [bugprone-easily-swappable-parameters]

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

### src/web/routes_data.cpp (5 предупреждений)
**Потенциальные баги:** 3 проблем
- src\web\routes_data.cpp:187:13:: repeated branch body in conditional chain [bugprone-branch-clone]
- src\web\routes_data.cpp:208:13:: repeated branch body in conditional chain [bugprone-branch-clone]
- src\web\routes_data.cpp:214:13:: repeated branch body in conditional chain [bugprone-branch-clone]

**Модернизация:** 2 проблем
- src\web\routes_data.cpp:308:64:: escaped string literal can be written as a raw string literal [modernize-raw-string-literal]
- src\web\routes_data.cpp:786:21:: escaped string literal can be written as a raw string literal [modernize-raw-string-literal]

### src/web/routes_main.cpp ✅ Проблем не найдено

### src/web/routes_ota.cpp (5 предупреждений)
**Модернизация:** 5 проблем
- src\web\routes_ota.cpp:44:81:: escaped string literal can be written as a raw string literal [modernize-raw-string-literal]
- src\web\routes_ota.cpp:58:81:: escaped string literal can be written as a raw string literal [modernize-raw-string-literal]
- src\web\routes_ota.cpp:442:65:: escaped string literal can be written as a raw string literal [modernize-raw-string-literal]
- ... и ещё 2 проблем

### src/web/routes_reports.cpp ✅ Проблем не найдено

### src/web/routes_service.cpp ✅ Проблем не найдено

### src/web/web_templates.cpp (10 предупреждений)
**Потенциальные баги:** 10 проблем
- src\web\web_templates.cpp:12:14:: 2 adjacent parameters of 'PageInfo' of similar type ('const String &') are easily swapped by mistake [bugprone-easily-swappable-parameters]
- src\web\web_templates.cpp:23:14:: 5 adjacent parameters of 'FormInfo' of similar type ('const String &') are easily swapped by mistake [bugprone-easily-swappable-parameters]
- src\web\web_templates.cpp:39:20:: 5 adjacent parameters of 'InputFieldInfo' of similar type ('const String &') are easily swapped by mistake [bugprone-easily-swappable-parameters]
- ... и ещё 7 проблем

## 🎯 РЕКОМЕНДАЦИИ ПО ПРИОРИТЕТАМ

### 🔴 КРИТИЧЕСКИЕ (исправить немедленно)
- Потенциальные баги - могут привести к неправильной работе

### 🟡 СРЕДНИЕ (исправить в ближайшее время)
- Проблемы читаемости - затрудняют сопровождение кода
- Модернизация - улучшают качество кода

### 🟢 НИЗКИЕ (можно исправить позже)
- Разные мелкие проблемы
- Прочие стилистические замечания
