# CLANG-TIDY ПОЛНЫЙ ОТЧЁТ АНАЛИЗА
**Дата анализа:** 07.07.2025 18:19
**Версия clang-tidy:** 20.1.0

## 📊 СТАТИСТИКА
- **Всего файлов проанализировано:** 24
- **Успешно проанализировано:** 24
- **Ошибки анализа:** 0
- **Всего предупреждений:** 325

## 🔍 КАТЕГОРИИ ПРОБЛЕМ
### Читаемость (158 проблем)
- `readability-braces-around-statements`: 47 случаев
- `readability-identifier-length`: 32 случаев
- ``: 23 случаев
- `readability-implicit-bool-conversion`: 17 случаев
- `readability-static-accessed-through-instance`: 16 случаев

### Модернизация (48 проблем)
- `modernize-avoid-c-arrays`: 12 случаев
- `modernize-macro-to-enum`: 12 случаев
- `modernize-raw-string-literal`: 9 случаев
- `modernize-use-nullptr`: 5 случаев
- `modernize-use-auto`: 5 случаев

### Разное (94 проблем)
- `misc-const-correctness`: 90 случаев
- `misc-use-internal-linkage`: 2 случаев
- `misc-unused-parameters`: 1 случаев
- `misc-use-anonymous-namespace`: 1 случаев

### Прочее (1 проблем)
- `performance-enum-size`: 1 случаев

### Потенциальные баги (24 проблем)
- `bugprone-easily-swappable-parameters`: 15 случаев
- `bugprone-branch-clone`: 5 случаев
- `bugprone-narrowing-conversions`: 2 случаев
- `bugprone-switch-missing-default-case`: 1 случаев
- `bugprone-too-small-loop-variable`: 1 случаев

## 📁 ДЕТАЛЬНЫЙ АНАЛИЗ ПО ФАЙЛАМ
### src/calibration_manager.cpp (5 предупреждений)
**Читаемость:** 4 проблем
- src\calibration_manager.cpp:55:12:: implicit conversion 'int' -> 'bool' [readability-implicit-bool-conversion]
- src\calibration_manager.cpp:82:12:: implicit conversion 'int' -> 'bool' [readability-implicit-bool-conversion]
- src\calibration_manager.cpp:190:53:: '*' has higher precedence than '+'; add parentheses to explicitly specify the order of operations [readability-math-missing-parentheses]
- ... и ещё 1 проблем

**Модернизация:** 1 проблем
- src\calibration_manager.cpp:150:5:: do not declare C-style arrays, use 'std::array' instead [modernize-avoid-c-arrays]

### src/config.cpp (2 предупреждений)
**Модернизация:** 2 проблем
- src\config.cpp:19:5:: do not declare C-style arrays, use 'std::array' instead [modernize-avoid-c-arrays]
- src\config.cpp:28:5:: do not declare C-style arrays, use 'std::array' instead [modernize-avoid-c-arrays]

### src/fake_sensor.cpp (1 предупреждений)
**Разное:** 1 проблем
- src\fake_sensor.cpp:14:27:: parameter 'pvParameters' is unused [misc-unused-parameters]

### src/jxct_format_utils.cpp (5 предупреждений)
**Прочее:** 1 проблем
- src\jxct_format_utils.cpp:7:12:: enum 'FormatType' uses a larger base type ('int', size: 4 bytes) than necessary for its value set, consider using 'std::uint8_t' (1 byte) as the base type to reduce its size [performance-enum-size]

**Читаемость:** 3 проблем
- src\jxct_format_utils.cpp:17:23:: parameter name 'p' is too short, expected at least 3 characters [readability-identifier-length]
- src\jxct_format_utils.cpp:17:37:: parameter name 'ft' is too short, expected at least 3 characters [readability-identifier-length]
- src\jxct_format_utils.cpp:71:56:: conditional operator is used as sub-expression of parent conditional operator, refrain from using nested conditional operators [readability-avoid-nested-conditional-operator]

**Потенциальные баги:** 1 проблем
- src\jxct_format_utils.cpp:28:5:: if with identical then and else branches [bugprone-branch-clone]

### src/jxct_ui_system.cpp (1 предупреждений)
**Модернизация:** 1 проблем
- src\jxct_ui_system.cpp:6:18:: do not declare C-style arrays, use 'std::array' instead [modernize-avoid-c-arrays]

### src/logger.cpp (2 предупреждений)
**Читаемость:** 2 проблем
- src\logger.cpp:194:9:: static member accessed through instance [readability-static-accessed-through-instance]
- src\logger.cpp:200:73:: static member accessed through instance [readability-static-accessed-through-instance]

### src/main.cpp (4 предупреждений)
**Модернизация:** 3 проблем
- src\main.cpp:140:22:: use nullptr [modernize-use-nullptr]
- src\main.cpp:203:55:: use nullptr [modernize-use-nullptr]
- src\main.cpp:203:64:: use nullptr [modernize-use-nullptr]

**Читаемость:** 1 проблем
- src\main.cpp:232:9:: implicit conversion 'NTPClient *' -> 'bool' [readability-implicit-bool-conversion]

### src/modbus_sensor.cpp (64 предупреждений)
**Читаемость:** 42 проблем
- src\modbus_sensor.cpp:47:34:: pointer parameter 'data' can be pointer to const [readability-non-const-parameter]
- src\modbus_sensor.cpp:56:17:: implicit conversion 'int' -> 'bool' [readability-implicit-bool-conversion]
- src\modbus_sensor.cpp:70:34:: parameter name 'd' is too short, expected at least 3 characters [readability-identifier-length]
- ... и ещё 39 проблем

**Модернизация:** 6 проблем
- src\modbus_sensor.cpp:84:12:: do not declare C-style arrays, use 'std::array' instead [modernize-avoid-c-arrays]
- src\modbus_sensor.cpp:394:13:: use auto when initializing with a cast to avoid duplicating the type name [modernize-use-auto]
- src\modbus_sensor.cpp:400:13:: use auto when initializing with a cast to avoid duplicating the type name [modernize-use-auto]
- ... и ещё 3 проблем

**Потенциальные баги:** 4 проблем
- src\modbus_sensor.cpp:111:5:: switching on non-enum value without default case may not cover all cases [bugprone-switch-missing-default-case]
- src\modbus_sensor.cpp:725:33:: loop variable has narrower type 'uint8_t' than iteration's upper bound 'int' [bugprone-too-small-loop-variable]
- src\modbus_sensor.cpp:737:9:: if with identical then and else branches [bugprone-branch-clone]
- ... и ещё 1 проблем

**Разное:** 12 проблем
- src\modbus_sensor.cpp:255:5:: variable 'result' of type 'uint8_t' (aka 'unsigned char') can be declared 'const' [misc-const-correctness]
- src\modbus_sensor.cpp:259:9:: variable 'version' of type 'uint16_t' (aka 'unsigned short') can be declared 'const' [misc-const-correctness]
- src\modbus_sensor.cpp:273:5:: variable 'result' of type 'uint8_t' (aka 'unsigned char') can be declared 'const' [misc-const-correctness]
- ... и ещё 9 проблем

### src/mqtt_client.cpp (14 предупреждений)
**Читаемость:** 7 проблем
- src\mqtt_client.cpp:120:5:: do not use 'else' after 'return' [readability-else-after-return]
- src\mqtt_client.cpp:201:9:: static member accessed through instance [readability-static-accessed-through-instance]
- src\mqtt_client.cpp:506:28:: implicit conversion 'NTPClient *' -> 'bool' [readability-implicit-bool-conversion]
- ... и ещё 4 проблем

**Разное:** 6 проблем
- src\mqtt_client.cpp:324:5:: variable 'isConnected' of type 'bool' can be declared 'const' [misc-const-correctness]
- src\mqtt_client.cpp:483:5:: variable 'currentTime' of type 'unsigned long' can be declared 'const' [misc-const-correctness]
- src\mqtt_client.cpp:559:5:: variable 'deviceIdStr' of type 'String' can be declared 'const' [misc-const-correctness]
- ... и ещё 3 проблем

**Модернизация:** 1 проблем
- src\mqtt_client.cpp:807:12:: avoid repeating the return type from the declaration; use a braced initializer list instead [modernize-return-braced-init-list]

### src/ota_manager.cpp (18 предупреждений)
**Читаемость:** 7 проблем
- src\ota_manager.cpp:70:25:: implicit conversion 'const char *' -> 'bool' [readability-implicit-bool-conversion]
- src\ota_manager.cpp:71:25:: implicit conversion 'WiFiClient *' -> 'bool' [readability-implicit-bool-conversion]
- src\ota_manager.cpp:74:10:: implicit conversion 'const char *' -> 'bool' [readability-implicit-bool-conversion]
- ... и ещё 4 проблем

**Модернизация:** 4 проблем
- src\ota_manager.cpp:227:5:: do not declare C-style arrays, use 'std::array' instead [modernize-avoid-c-arrays]
- src\ota_manager.cpp:338:5:: use auto when initializing with new to avoid duplicating the type name [modernize-use-auto]
- src\ota_manager.cpp:361:5:: use auto when initializing with new to avoid duplicating the type name [modernize-use-auto]
- ... и ещё 1 проблем

**Разное:** 6 проблем
- src\ota_manager.cpp:238:9:: variable 'avail' of type 'size_t' (aka 'unsigned long long') can be declared 'const' [misc-const-correctness]
- src\ota_manager.cpp:242:13:: variable 'toRead' of type 'size_t' (aka 'unsigned long long') can be declared 'const' [misc-const-correctness]
- src\ota_manager.cpp:326:5:: variable 'initialHeap' of type 'size_t' (aka 'unsigned long long') can be declared 'const' [misc-const-correctness]
- ... и ещё 3 проблем

**Потенциальные баги:** 1 проблем
- src\ota_manager.cpp:554:25:: narrowing conversion from 'unsigned int' to signed type 'int' is implementation-defined [bugprone-narrowing-conversions]

### src/routes_calibration.cpp ✅ Проблем не найдено

### src/sensor_compensation.cpp (19 предупреждений)
**Модернизация:** 6 проблем
- src\sensor_compensation.cpp:2:10:: inclusion of deprecated C++ header 'math.h'; consider using 'cmath' instead [modernize-deprecated-headers]
- src\sensor_compensation.cpp:3:10:: inclusion of deprecated C++ header 'time.h'; consider using 'ctime' instead [modernize-deprecated-headers]
- src\sensor_compensation.cpp:7:18:: do not declare C-style arrays, use 'std::array' instead [modernize-avoid-c-arrays]
- ... и ещё 3 проблем

**Читаемость:** 9 проблем
- src\sensor_compensation.cpp:26:19:: '*' has higher precedence than '-'; add parentheses to explicitly specify the order of operations [readability-math-missing-parentheses]
- src\sensor_compensation.cpp:30:19:: '*' has higher precedence than '-'; add parentheses to explicitly specify the order of operations [readability-math-missing-parentheses]
- src\sensor_compensation.cpp:34:19:: '*' has higher precedence than '-'; add parentheses to explicitly specify the order of operations [readability-math-missing-parentheses]
- ... и ещё 6 проблем

**Потенциальные баги:** 2 проблем
- src\sensor_compensation.cpp:44:26:: 2 adjacent parameters of 'ECCompensationParams' of similar type ('float') are easily swapped by mistake [bugprone-easily-swappable-parameters]
- src\sensor_compensation.cpp:69:53:: 3 adjacent parameters of 'correctNPK' of similar type ('float &') are easily swapped by mistake [bugprone-easily-swappable-parameters]

**Разное:** 2 проблем
- src\sensor_compensation.cpp:50:7:: function 'correctEC' can be made static or moved into an anonymous namespace to enforce internal linkage [misc-use-internal-linkage]
- src\sensor_compensation.cpp:69:6:: function 'correctNPK' can be made static or moved into an anonymous namespace to enforce internal linkage [misc-use-internal-linkage]

### src/thingspeak_client.cpp (3 предупреждений)
**Модернизация:** 1 проблем
- src\thingspeak_client.cpp:6:10:: inclusion of deprecated C++ header 'ctype.h'; consider using 'cctype' instead [modernize-deprecated-headers]

**Читаемость:** 2 проблем
- src\thingspeak_client.cpp:25:10:: implicit conversion 'char *' -> 'bool' [readability-implicit-bool-conversion]
- src\thingspeak_client.cpp:121:5:: do not use 'else' after 'return' [readability-else-after-return]

### src/validation_utils.cpp (9 предупреждений)
**Читаемость:** 4 проблем
- src\validation_utils.cpp:49:21:: variable name 'ch' is too short, expected at least 3 characters [readability-identifier-length]
- src\validation_utils.cpp:285:37:: parameter name 'ip' is too short, expected at least 3 characters [readability-identifier-length]
- src\validation_utils.cpp:326:15:: variable name 'c' is too short, expected at least 3 characters [readability-identifier-length]
- ... и ещё 1 проблем

**Разное:** 5 проблем
- src\validation_utils.cpp:299:13:: variable 'part' of type 'String' can be declared 'const' [misc-const-correctness]
- src\validation_utils.cpp:300:13:: variable 'value' of type 'int' can be declared 'const' [misc-const-correctness]
- src\validation_utils.cpp:326:10:: variable 'c' of type 'char' can be declared 'const' [misc-const-correctness]
- ... и ещё 2 проблем

### src/wifi_manager.cpp (26 предупреждений)
**Модернизация:** 13 проблем
- src\wifi_manager.cpp:23:1:: replace macro with enum [modernize-macro-to-enum]
- src\wifi_manager.cpp:23:9:: macro 'RESET_BUTTON_PIN' defines an integral constant; prefer an enum instead [modernize-macro-to-enum]
- src\wifi_manager.cpp:24:9:: macro 'WIFI_RECONNECT_INTERVAL' defines an integral constant; prefer an enum instead [modernize-macro-to-enum]
- ... и ещё 10 проблем

**Читаемость:** 8 проблем
- src\wifi_manager.cpp:117:5:: static member accessed through instance [readability-static-accessed-through-instance]
- src\wifi_manager.cpp:173:13:: static member accessed through instance [readability-static-accessed-through-instance]
- src\wifi_manager.cpp:231:5:: static member accessed through instance [readability-static-accessed-through-instance]
- ... и ещё 5 проблем

**Разное:** 5 проблем
- src\wifi_manager.cpp:232:5:: variable 'apSsid' of type 'String' can be declared 'const' [misc-const-correctness]
- src\wifi_manager.cpp:249:5:: variable 'hostname' of type 'String' can be declared 'const' [misc-const-correctness]
- src\wifi_manager.cpp:259:9:: variable 'startTime' of type 'unsigned long' can be declared 'const' [misc-const-correctness]
- ... и ещё 2 проблем

### src/web/csrf_protection.cpp (6 предупреждений)
**Разное:** 5 проблем
- src\web\csrf_protection.cpp:67:5:: variable 'isValid' of type 'bool' can be declared 'const' [misc-const-correctness]
- src\web\csrf_protection.cpp:86:5:: variable 'method' of type 'HTTPMethod' (aka 'http_method') can be declared 'const' [misc-const-correctness]
- src\web\csrf_protection.cpp:108:5:: variable 'clientIP' of type 'String' can be declared 'const' [misc-const-correctness]
- ... и ещё 2 проблем

**Модернизация:** 1 проблем
- src\web\csrf_protection.cpp:80:12:: escaped string literal can be written as a raw string literal [modernize-raw-string-literal]

### src/web/error_handlers.cpp (9 предупреждений)
**Потенциальные баги:** 2 проблем
- src\web\error_handlers.cpp:14:21:: 2 adjacent parameters of 'ValidationRange' of similar type ('int') are easily swapped by mistake [bugprone-easily-swappable-parameters]
- src\web\error_handlers.cpp:24:17:: 3 adjacent parameters of 'HttpRequest' of similar type ('const String &') are easily swapped by mistake [bugprone-easily-swappable-parameters]

**Разное:** 6 проблем
- src\web\error_handlers.cpp:60:13:: variable 'uri' of type 'String' can be declared 'const' [misc-const-correctness]
- src\web\error_handlers.cpp:61:13:: variable 'method' of type 'String' can be declared 'const' [misc-const-correctness]
- src\web\error_handlers.cpp:67:13:: variable 'html' of type 'String' can be declared 'const' [misc-const-correctness]
- ... и ещё 3 проблем

**Читаемость:** 1 проблем
- src\web\error_handlers.cpp:151:20:: redundant boolean literal in conditional return statement [readability-simplify-boolean-expr]

### src/web/routes_config.cpp (33 предупреждений)
**Разное:** 6 проблем
- src\web\routes_config.cpp:23:13:: function 'sendConfigExportJson' declared 'static', move to anonymous namespace instead [misc-use-anonymous-namespace]
- src\web\routes_config.cpp:190:22:: variable 'valSensor' of type 'ValidationResult' can be declared 'const' [misc-const-correctness]
- src\web\routes_config.cpp:191:22:: variable 'valMqtt' of type 'ValidationResult' can be declared 'const' [misc-const-correctness]
- ... и ещё 3 проблем

**Читаемость:** 26 проблем
- src\web\routes_config.cpp:199:79:: conditional operator is used as sub-expression of parent conditional operator, refrain from using nested conditional operators [readability-avoid-nested-conditional-operator]
- src\web\routes_config.cpp:405:25:: variable name 'up' is too short, expected at least 3 characters [readability-identifier-length]
- src\web\routes_config.cpp:441:5:: confusing array subscript expression, usually the index is inside the [] [readability-misplaced-array-index]
- ... и ещё 23 проблем

**Модернизация:** 1 проблем
- src\web\routes_config.cpp:361:68:: escaped string literal can be written as a raw string literal [modernize-raw-string-literal]

### src/web/routes_data.cpp (43 предупреждений)
**Модернизация:** 3 проблем
- src\web\routes_data.cpp:10:10:: inclusion of deprecated C++ header 'time.h'; consider using 'ctime' instead [modernize-deprecated-headers]
- src\web\routes_data.cpp:270:64:: escaped string literal can be written as a raw string literal [modernize-raw-string-literal]
- src\web\routes_data.cpp:691:21:: escaped string literal can be written as a raw string literal [modernize-raw-string-literal]

**Разное:** 8 проблем
- src\web\routes_data.cpp:92:5:: variable 'soil' of type 'int' can be declared 'const' [misc-const-correctness]
- src\web\routes_data.cpp:138:9:: variable 'rainy' of type 'bool' can be declared 'const' [misc-const-correctness]
- src\web\routes_data.cpp:250:9:: variable 'profileStr' of type 'String' can be declared 'const' [misc-const-correctness]
- ... и ещё 5 проблем

**Читаемость:** 29 проблем
- src\web\routes_data.cpp:137:13:: variable name 'm' is too short, expected at least 3 characters [readability-identifier-length]
- src\web\routes_data.cpp:137:17:: implicit conversion 'struct tm *' -> 'bool' [readability-implicit-bool-conversion]
- src\web\routes_data.cpp:330:20:: variable name 'ti' is too short, expected at least 3 characters [readability-identifier-length]
- ... и ещё 26 проблем

**Потенциальные баги:** 3 проблем
- src\web\routes_data.cpp:162:13:: repeated branch body in conditional chain [bugprone-branch-clone]
- src\web\routes_data.cpp:183:13:: repeated branch body in conditional chain [bugprone-branch-clone]
- src\web\routes_data.cpp:189:13:: repeated branch body in conditional chain [bugprone-branch-clone]

### src/web/routes_main.cpp (18 предупреждений)
**Разное:** 18 проблем
- src\web\routes_main.cpp:37:17:: variable 'msg' of type 'String' can be declared 'const' [misc-const-correctness]
- src\web\routes_main.cpp:38:17:: variable 'html' of type 'String' can be declared 'const' [misc-const-correctness]
- src\web\routes_main.cpp:47:21:: variable 'hostRes' of type 'ValidationResult' can be declared 'const' [misc-const-correctness]
- ... и ещё 15 проблем

### src/web/routes_ota.cpp (7 предупреждений)
**Модернизация:** 5 проблем
- src\web\routes_ota.cpp:43:81:: escaped string literal can be written as a raw string literal [modernize-raw-string-literal]
- src\web\routes_ota.cpp:57:81:: escaped string literal can be written as a raw string literal [modernize-raw-string-literal]
- src\web\routes_ota.cpp:441:65:: escaped string literal can be written as a raw string literal [modernize-raw-string-literal]
- ... и ещё 2 проблем

**Разное:** 2 проблем
- src\web\routes_ota.cpp:134:17:: variable 'uploadBtn' of type 'String' can be declared 'const' [misc-const-correctness]
- src\web\routes_ota.cpp:351:13:: variable 'percent' of type 'size_t' (aka 'unsigned long long') can be declared 'const' [misc-const-correctness]

### src/web/routes_reports.cpp (7 предупреждений)
**Разное:** 7 проблем
- src\web\routes_reports.cpp:121:5:: variable 'now' of type 'unsigned long' can be declared 'const' [misc-const-correctness]
- src\web\routes_reports.cpp:136:5:: variable 'html' of type 'String' can be declared 'const' [misc-const-correctness]
- src\web\routes_reports.cpp:266:5:: variable 'statusIcon' of type 'String' can be declared 'const' [misc-const-correctness]
- ... и ещё 4 проблем

### src/web/routes_service.cpp (5 предупреждений)
**Разное:** 5 проблем
- src\web\routes_service.cpp:132:26:: variable 'html' of type 'String' can be declared 'const' [misc-const-correctness]
- src\web\routes_service.cpp:144:22:: variable 'html' of type 'String' can be declared 'const' [misc-const-correctness]
- src\web\routes_service.cpp:172:26:: variable 'html' of type 'String' can be declared 'const' [misc-const-correctness]
- ... и ещё 2 проблем

### src/web/web_templates.cpp (24 предупреждений)
**Потенциальные баги:** 11 проблем
- src\web\web_templates.cpp:12:14:: 2 adjacent parameters of 'PageInfo' of similar type ('const String &') are easily swapped by mistake [bugprone-easily-swappable-parameters]
- src\web\web_templates.cpp:23:14:: 4 adjacent parameters of 'FormInfo' of similar type ('const String &') are easily swapped by mistake [bugprone-easily-swappable-parameters]
- src\web\web_templates.cpp:37:20:: 5 adjacent parameters of 'InputFieldInfo' of similar type ('const String &') are easily swapped by mistake [bugprone-easily-swappable-parameters]
- ... и ещё 8 проблем

**Читаемость:** 13 проблем
- src\web\web_templates.cpp:23:79:: parameter name 'fc' is too short, expected at least 3 characters [readability-identifier-length]
- src\web\web_templates.cpp:23:97:: parameter name 'bt' is too short, expected at least 3 characters [readability-identifier-length]
- src\web\web_templates.cpp:23:115:: parameter name 'bi' is too short, expected at least 3 characters [readability-identifier-length]
- ... и ещё 10 проблем

## 🎯 РЕКОМЕНДАЦИИ ПО ПРИОРИТЕТАМ

### 🔴 КРИТИЧЕСКИЕ (исправить немедленно)
- Потенциальные баги - могут привести к неправильной работе

### 🟡 СРЕДНИЕ (исправить в ближайшее время)
- Проблемы читаемости - затрудняют сопровождение кода
- Модернизация - улучшают качество кода

### 🟢 НИЗКИЕ (можно исправить позже)
- Разные мелкие проблемы
- Прочие стилистические замечания
