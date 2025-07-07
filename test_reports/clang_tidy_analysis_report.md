# CLANG-TIDY ПОЛНЫЙ ОТЧЁТ АНАЛИЗА
**Дата анализа:** 07.07.2025 23:21
**Версия clang-tidy:** 20.1.0

## 📊 СТАТИСТИКА
- **Всего файлов проанализировано:** 24
- **Успешно проанализировано:** 24
- **Ошибки анализа:** 0
- **Всего предупреждений:** 190

## 🔍 КАТЕГОРИИ ПРОБЛЕМ
### Читаемость (93 проблем)
- ``: 23 случаев
- `readability-identifier-length`: 19 случаев
- `readability-static-accessed-through-instance`: 16 случаев
- `readability-implicit-bool-conversion`: 12 случаев
- `readability-math-missing-parentheses`: 5 случаев

### Прочее (2 проблем)
- `performance-enum-size`: 2 случаев

### Потенциальные баги (22 проблем)
- `bugprone-easily-swappable-parameters`: 14 случаев
- `bugprone-branch-clone`: 5 случаев
- `bugprone-narrowing-conversions`: 2 случаев
- `bugprone-reserved-identifier,cert-dcl37-c,cert-dcl51-cpp`: 1 случаев

### Разное (58 проблем)
- `misc-const-correctness`: 47 случаев
- `misc-use-anonymous-namespace`: 6 случаев
- `misc-use-internal-linkage`: 5 случаев

### Модернизация (15 проблем)
- `modernize-raw-string-literal`: 8 случаев
- `modernize-use-auto`: 4 случаев
- `modernize-return-braced-init-list`: 1 случаев
- `modernize-avoid-c-arrays`: 1 случаев
- `modernize-deprecated-headers`: 1 случаев

## 📁 ДЕТАЛЬНЫЙ АНАЛИЗ ПО ФАЙЛАМ
### src/calibration_manager.cpp (5 предупреждений)
**Читаемость:** 5 проблем
- src\calibration_manager.cpp:24:9:: implicit conversion 'bool' -> 'int' [readability-implicit-bool-conversion]
- src\calibration_manager.cpp:56:12:: implicit conversion 'int' -> 'bool' [readability-implicit-bool-conversion]
- src\calibration_manager.cpp:83:12:: implicit conversion 'int' -> 'bool' [readability-implicit-bool-conversion]
- ... и ещё 2 проблем

### src/config.cpp ✅ Проблем не найдено

### src/fake_sensor.cpp ✅ Проблем не найдено

### src/jxct_format_utils.cpp (7 предупреждений)
**Прочее:** 1 проблем
- src\jxct_format_utils.cpp:9:12:: enum 'FormatType' uses a larger base type ('int', size: 4 bytes) than necessary for its value set, consider using 'std::uint8_t' (1 byte) as the base type to reduce its size [performance-enum-size]

**Потенциальные баги:** 1 проблем
- src\jxct_format_utils.cpp:31:9:: switch has 2 consecutive identical branches [bugprone-branch-clone]

**Разное:** 5 проблем
- src\jxct_format_utils.cpp:42:13:: function 'format_moisture' can be made static or moved into an anonymous namespace to enforce internal linkage [misc-use-internal-linkage]
- src\jxct_format_utils.cpp:47:13:: function 'format_temperature' can be made static or moved into an anonymous namespace to enforce internal linkage [misc-use-internal-linkage]
- src\jxct_format_utils.cpp:52:13:: function 'format_ec' can be made static or moved into an anonymous namespace to enforce internal linkage [misc-use-internal-linkage]
- ... и ещё 2 проблем

### src/jxct_ui_system.cpp ✅ Проблем не найдено

### src/logger.cpp (2 предупреждений)
**Читаемость:** 2 проблем
- src\logger.cpp:228:9:: static member accessed through instance [readability-static-accessed-through-instance]
- src\logger.cpp:235:73:: static member accessed through instance [readability-static-accessed-through-instance]

### src/main.cpp ✅ Проблем не найдено

### src/modbus_sensor.cpp (24 предупреждений)
**Читаемость:** 14 проблем
- src\modbus_sensor.cpp:48:34:: pointer parameter 'data' can be pointer to const [readability-non-const-parameter]
- src\modbus_sensor.cpp:57:17:: implicit conversion 'int' -> 'bool' [readability-implicit-bool-conversion]
- src\modbus_sensor.cpp:86:5:: multiple declarations in a single statement reduces readability [readability-isolate-declaration]
- ... и ещё 11 проблем

**Разное:** 6 проблем
- src\modbus_sensor.cpp:369:5:: variable 'pre_delay' of type 'unsigned long' can be declared 'const' [misc-const-correctness]
- src\modbus_sensor.cpp:373:5:: variable 'post_delay' of type 'unsigned long' can be declared 'const' [misc-const-correctness]
- src\modbus_sensor.cpp:429:13:: function 'readSingleRegister' declared 'static', move to anonymous namespace instead [misc-use-anonymous-namespace]
- ... и ещё 3 проблем

**Модернизация:** 2 проблем
- src\modbus_sensor.cpp:441:13:: use auto when initializing with a cast to avoid duplicating the type name [modernize-use-auto]
- src\modbus_sensor.cpp:447:13:: use auto when initializing with a cast to avoid duplicating the type name [modernize-use-auto]

**Потенциальные баги:** 2 проблем
- src\modbus_sensor.cpp:765:9:: if with identical then and else branches [bugprone-branch-clone]
- src\modbus_sensor.cpp:782:22:: narrowing conversion from 'int' to 'float' [bugprone-narrowing-conversions]

### src/mqtt_client.cpp (7 предупреждений)
**Разное:** 2 проблем
- src\mqtt_client.cpp:105:18:: function 'getCachedIP' declared 'static', move to anonymous namespace instead [misc-use-anonymous-namespace]
- src\mqtt_client.cpp:794:5:: variable 'currentTime' of type 'unsigned long' can be declared 'const' [misc-const-correctness]

**Читаемость:** 4 проблем
- src\mqtt_client.cpp:206:9:: static member accessed through instance [readability-static-accessed-through-instance]
- src\mqtt_client.cpp:761:18:: variable name 't' is too short, expected at least 3 characters [readability-identifier-length]
- src\mqtt_client.cpp:807:9:: implicit conversion 'int' -> 'bool' [readability-implicit-bool-conversion]
- ... и ещё 1 проблем

**Модернизация:** 1 проблем
- src\mqtt_client.cpp:818:12:: avoid repeating the return type from the declaration; use a braced initializer list instead [modernize-return-braced-init-list]

### src/ota_manager.cpp (17 предупреждений)
**Потенциальные баги:** 2 проблем
- src\ota_manager.cpp:34:13:: declaration uses identifier '_printGuard', which is reserved in the global namespace [bugprone-reserved-identifier,cert-dcl37-c,cert-dcl51-cpp]
- src\ota_manager.cpp:564:25:: narrowing conversion from 'unsigned int' to signed type 'int' is implementation-defined [bugprone-narrowing-conversions]

**Разное:** 8 проблем
- src\ota_manager.cpp:34:13:: function '_printGuard' declared 'static', move to anonymous namespace instead [misc-use-anonymous-namespace]
- src\ota_manager.cpp:128:13:: function 'initializeDownload' declared 'static', move to anonymous namespace instead [misc-use-anonymous-namespace]
- src\ota_manager.cpp:209:13:: function 'downloadData' declared 'static', move to anonymous namespace instead [misc-use-anonymous-namespace]
- ... и ещё 5 проблем

**Читаемость:** 4 проблем
- src\ota_manager.cpp:227:10:: implicit conversion 'WiFiClient *' -> 'bool' [readability-implicit-bool-conversion]
- src\ota_manager.cpp:349:10:: implicit conversion 'HTTPClient *' -> 'bool' [readability-implicit-bool-conversion]
- src\ota_manager.cpp:372:10:: implicit conversion 'mbedtls_sha256_context *' -> 'bool' [readability-implicit-bool-conversion]
- ... и ещё 1 проблем

**Модернизация:** 3 проблем
- src\ota_manager.cpp:348:5:: use auto when initializing with new to avoid duplicating the type name [modernize-use-auto]
- src\ota_manager.cpp:371:5:: use auto when initializing with new to avoid duplicating the type name [modernize-use-auto]
- src\ota_manager.cpp:398:5:: do not declare C-style arrays, use 'std::array' instead [modernize-avoid-c-arrays]

### src/routes_calibration.cpp ✅ Проблем не найдено

### src/sensor_compensation.cpp (16 предупреждений)
**Читаемость:** 14 проблем
- src\sensor_compensation.cpp:29:19:: '*' has higher precedence than '-'; add parentheses to explicitly specify the order of operations [readability-math-missing-parentheses]
- src\sensor_compensation.cpp:33:19:: '*' has higher precedence than '-'; add parentheses to explicitly specify the order of operations [readability-math-missing-parentheses]
- src\sensor_compensation.cpp:37:19:: '*' has higher precedence than '-'; add parentheses to explicitly specify the order of operations [readability-math-missing-parentheses]
- ... и ещё 11 проблем

**Потенциальные баги:** 2 проблем
- src\sensor_compensation.cpp:48:26:: 2 adjacent parameters of 'ECCompensationParams' of similar type ('float') are easily swapped by mistake [bugprone-easily-swappable-parameters]
- src\sensor_compensation.cpp:72:53:: 3 adjacent parameters of 'correctNPK' of similar type ('float &') are easily swapped by mistake [bugprone-easily-swappable-parameters]

### src/thingspeak_client.cpp (1 предупреждений)
**Читаемость:** 1 проблем
- src\thingspeak_client.cpp:134:5:: do not use 'else' after 'return' [readability-else-after-return]

### src/validation_utils.cpp (3 предупреждений)
**Читаемость:** 1 проблем
- src\validation_utils.cpp:369:16:: redundant boolean literal in conditional return statement [readability-simplify-boolean-expr]

**Разное:** 2 проблем
- src\validation_utils.cpp:382:5:: variable 'formatted' of type 'String' can be declared 'const' [misc-const-correctness]
- src\validation_utils.cpp:397:5:: variable 'formatted' of type 'String' can be declared 'const' [misc-const-correctness]

### src/wifi_manager.cpp (12 предупреждений)
**Прочее:** 1 проблем
- src\wifi_manager.cpp:23:12:: enum 'WifiConstants' uses a larger base type ('uint32_t' (aka 'unsigned int'), size: 4 bytes) than necessary for its value set, consider using 'std::uint16_t' (2 bytes) as the base type to reduce its size [performance-enum-size]

**Читаемость:** 9 проблем
- src\wifi_manager.cpp:118:5:: static member accessed through instance [readability-static-accessed-through-instance]
- src\wifi_manager.cpp:177:13:: static member accessed through instance [readability-static-accessed-through-instance]
- src\wifi_manager.cpp:235:5:: static member accessed through instance [readability-static-accessed-through-instance]
- ... и ещё 6 проблем

**Разное:** 2 проблем
- src\wifi_manager.cpp:300:17:: variable 'ntpStart' of type 'unsigned long' can be declared 'const' [misc-const-correctness]
- src\wifi_manager.cpp:328:5:: variable 'isPressed' of type 'bool' can be declared 'const' [misc-const-correctness]

### src/web/csrf_protection.cpp (2 предупреждений)
**Разное:** 2 проблем
- src\web\csrf_protection.cpp:105:5:: variable 'method' of type 'HTTPMethod' (aka 'http_method') can be declared 'const' [misc-const-correctness]
- src\web\csrf_protection.cpp:136:5:: variable 'isValid' of type 'bool' can be declared 'const' [misc-const-correctness]

### src/web/error_handlers.cpp (4 предупреждений)
**Потенциальные баги:** 2 проблем
- src\web\error_handlers.cpp:14:21:: 2 adjacent parameters of 'ValidationRange' of similar type ('int') are easily swapped by mistake [bugprone-easily-swappable-parameters]
- src\web\error_handlers.cpp:24:17:: 3 adjacent parameters of 'HttpRequest' of similar type ('const String &') are easily swapped by mistake [bugprone-easily-swappable-parameters]

**Читаемость:** 1 проблем
- src\web\error_handlers.cpp:159:20:: redundant boolean literal in conditional return statement [readability-simplify-boolean-expr]

**Разное:** 1 проблем
- src\web\error_handlers.cpp:245:9:: variable 'html' of type 'String' can be declared 'const' [misc-const-correctness]

### src/web/routes_config.cpp (32 предупреждений)
**Разное:** 4 проблем
- src\web\routes_config.cpp:23:13:: function 'sendConfigExportJson' declared 'static', move to anonymous namespace instead [misc-use-anonymous-namespace]
- src\web\routes_config.cpp:192:13:: variable 'valTs' of type 'ValidationResult' can be declared 'const' [misc-const-correctness]
- src\web\routes_config.cpp:196:17:: variable 'html' of type 'String' can be declared 'const' [misc-const-correctness]
- ... и ещё 1 проблем

**Читаемость:** 27 проблем
- src\web\routes_config.cpp:198:82:: conditional operator is used as sub-expression of parent conditional operator, refrain from using nested conditional operators [readability-avoid-nested-conditional-operator]
- src\web\routes_config.cpp:408:13:: different indentation for 'if' and corresponding 'else' [readability-misleading-indentation]
- src\web\routes_config.cpp:412:13:: different indentation for 'if' and corresponding 'else' [readability-misleading-indentation]
- ... и ещё 24 проблем

**Модернизация:** 1 проблем
- src\web\routes_config.cpp:359:68:: escaped string literal can be written as a raw string literal [modernize-raw-string-literal]

### src/web/routes_data.cpp (19 предупреждений)
**Модернизация:** 3 проблем
- src\web\routes_data.cpp:10:10:: inclusion of deprecated C++ header 'time.h'; consider using 'ctime' instead [modernize-deprecated-headers]
- src\web\routes_data.cpp:308:64:: escaped string literal can be written as a raw string literal [modernize-raw-string-literal]
- src\web\routes_data.cpp:786:21:: escaped string literal can be written as a raw string literal [modernize-raw-string-literal]

**Разное:** 7 проблем
- src\web\routes_data.cpp:117:5:: variable 'soil' of type 'int' can be declared 'const' [misc-const-correctness]
- src\web\routes_data.cpp:276:9:: variable 'profileStr' of type 'String' can be declared 'const' [misc-const-correctness]
- src\web\routes_data.cpp:329:5:: variable 'rec' of type 'RecValues' can be declared 'const' [misc-const-correctness]
- ... и ещё 4 проблем

**Читаемость:** 6 проблем
- src\web\routes_data.cpp:162:27:: implicit conversion 'struct tm *' -> 'bool' [readability-implicit-bool-conversion]
- src\web\routes_data.cpp:369:20:: variable name 'ti' is too short, expected at least 3 characters [readability-identifier-length]
- src\web\routes_data.cpp:374:17:: variable name 'm' is too short, expected at least 3 characters [readability-identifier-length]
- ... и ещё 3 проблем

**Потенциальные баги:** 3 проблем
- src\web\routes_data.cpp:187:13:: repeated branch body in conditional chain [bugprone-branch-clone]
- src\web\routes_data.cpp:208:13:: repeated branch body in conditional chain [bugprone-branch-clone]
- src\web\routes_data.cpp:214:13:: repeated branch body in conditional chain [bugprone-branch-clone]

### src/web/routes_main.cpp (11 предупреждений)
**Разное:** 11 проблем
- src\web\routes_main.cpp:48:21:: variable 'portRes' of type 'ValidationResult' can be declared 'const' [misc-const-correctness]
- src\web\routes_main.cpp:59:21:: variable 'tsRes' of type 'ValidationResult' can be declared 'const' [misc-const-correctness]
- src\web\routes_main.cpp:183:9:: variable 'mqttChecked' of type 'String' can be declared 'const' [misc-const-correctness]
- ... и ещё 8 проблем

### src/web/routes_ota.cpp (6 предупреждений)
**Модернизация:** 5 проблем
- src\web\routes_ota.cpp:44:81:: escaped string literal can be written as a raw string literal [modernize-raw-string-literal]
- src\web\routes_ota.cpp:58:81:: escaped string literal can be written as a raw string literal [modernize-raw-string-literal]
- src\web\routes_ota.cpp:442:65:: escaped string literal can be written as a raw string literal [modernize-raw-string-literal]
- ... и ещё 2 проблем

**Разное:** 1 проблем
- src\web\routes_ota.cpp:352:13:: variable 'percent' of type 'size_t' (aka 'unsigned long long') can be declared 'const' [misc-const-correctness]

### src/web/routes_reports.cpp (4 предупреждений)
**Разное:** 4 проблем
- src\web\routes_reports.cpp:136:5:: variable 'html' of type 'String' can be declared 'const' [misc-const-correctness]
- src\web\routes_reports.cpp:269:5:: variable 'html' of type 'String' can be declared 'const' [misc-const-correctness]
- src\web\routes_reports.cpp:482:22:: variable 'html' of type 'String' can be declared 'const' [misc-const-correctness]
- ... и ещё 1 проблем

### src/web/routes_service.cpp (3 предупреждений)
**Разное:** 3 проблем
- src\web\routes_service.cpp:132:26:: variable 'html' of type 'String' can be declared 'const' [misc-const-correctness]
- src\web\routes_service.cpp:171:26:: variable 'html' of type 'String' can be declared 'const' [misc-const-correctness]
- src\web\routes_service.cpp:217:5:: variable 'days' of type 'unsigned long' can be declared 'const' [misc-const-correctness]

### src/web/web_templates.cpp (15 предупреждений)
**Потенциальные баги:** 10 проблем
- src\web\web_templates.cpp:12:14:: 2 adjacent parameters of 'PageInfo' of similar type ('const String &') are easily swapped by mistake [bugprone-easily-swappable-parameters]
- src\web\web_templates.cpp:23:14:: 5 adjacent parameters of 'FormInfo' of similar type ('const String &') are easily swapped by mistake [bugprone-easily-swappable-parameters]
- src\web\web_templates.cpp:39:20:: 5 adjacent parameters of 'InputFieldInfo' of similar type ('const String &') are easily swapped by mistake [bugprone-easily-swappable-parameters]
- ... и ещё 7 проблем

**Читаемость:** 5 проблем
- src\web\web_templates.cpp:62:35:: parameter name 'id' is too short, expected at least 3 characters [readability-identifier-length]
- src\web\web_templates.cpp:69:15:: redundant 'navHtml' declaration [readability-redundant-declaration]
- src\web\web_templates.cpp:191:41:: parameter name 'id' is too short, expected at least 3 characters [readability-identifier-length]
- ... и ещё 2 проблем

## 🎯 РЕКОМЕНДАЦИИ ПО ПРИОРИТЕТАМ

### 🔴 КРИТИЧЕСКИЕ (исправить немедленно)
- Потенциальные баги - могут привести к неправильной работе

### 🟡 СРЕДНИЕ (исправить в ближайшее время)
- Проблемы читаемости - затрудняют сопровождение кода
- Модернизация - улучшают качество кода

### 🟢 НИЗКИЕ (можно исправить позже)
- Разные мелкие проблемы
- Прочие стилистические замечания
