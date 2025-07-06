# CLANG-TIDY ПОЛНЫЙ ОТЧЁТ АНАЛИЗА
**Дата анализа:** 06.07.2025 23:59
**Версия clang-tidy:** 20.1.0

## 📊 СТАТИСТИКА
- **Всего файлов проанализировано:** 24
- **Успешно проанализировано:** 24
- **Ошибки анализа:** 0
- **Всего предупреждений:** 397

## 🔍 КАТЕГОРИИ ПРОБЛЕМ
### Читаемость (167 проблем)
- `readability-braces-around-statements`: 54 случаев
- `readability-identifier-length`: 32 случаев
- ``: 23 случаев
- `readability-implicit-bool-conversion`: 18 случаев
- `readability-static-accessed-through-instance`: 16 случаев

### Разное (121 проблем)
- `misc-const-correctness`: 119 случаев
- `misc-unused-parameters`: 1 случаев
- `misc-use-anonymous-namespace`: 1 случаев

### Модернизация (67 проблем)
- `modernize-avoid-c-arrays`: 24 случаев
- `modernize-macro-to-enum`: 12 случаев
- `modernize-raw-string-literal`: 9 случаев
- `modernize-use-nullptr`: 7 случаев
- `modernize-return-braced-init-list`: 6 случаев

### Потенциальные баги (30 проблем)
- `bugprone-easily-swappable-parameters`: 17 случаев
- `bugprone-narrowing-conversions`: 5 случаев
- `bugprone-branch-clone`: 5 случаев
- `bugprone-switch-missing-default-case`: 1 случаев
- `bugprone-too-small-loop-variable`: 1 случаев

### Безопасность (12 проблем)
- `cert-dcl50-cpp`: 12 случаев

## 📁 ДЕТАЛЬНЫЙ АНАЛИЗ ПО ФАЙЛАМ
### src/calibration_manager.cpp (7 предупреждений)
**Читаемость:** 4 проблем
- src\calibration_manager.cpp:55:12:: implicit conversion 'int' -> 'bool' [readability-implicit-bool-conversion]
- src\calibration_manager.cpp:82:12:: implicit conversion 'int' -> 'bool' [readability-implicit-bool-conversion]
- src\calibration_manager.cpp:190:53:: '*' has higher precedence than '+'; add parentheses to explicitly specify the order of operations [readability-math-missing-parentheses]
- ... и ещё 1 проблем

**Разное:** 2 проблем
- src\calibration_manager.cpp:107:9:: variable 'raw' of type 'float' can be declared 'const' [misc-const-correctness]
- src\calibration_manager.cpp:108:9:: variable 'corr' of type 'float' can be declared 'const' [misc-const-correctness]

**Модернизация:** 1 проблем
- src\calibration_manager.cpp:150:5:: do not declare C-style arrays, use 'std::array' instead [modernize-avoid-c-arrays]

### src/config.cpp (6 предупреждений)
**Модернизация:** 6 проблем
- src\config.cpp:18:5:: do not declare C-style arrays, use 'std::array' instead [modernize-avoid-c-arrays]
- src\config.cpp:20:5:: do not declare C-style arrays, use 'std::array' instead [modernize-avoid-c-arrays]
- src\config.cpp:22:12:: avoid repeating the return type from the declaration; use a braced initializer list instead [modernize-return-braced-init-list]
- ... и ещё 3 проблем

### src/fake_sensor.cpp (4 предупреждений)
**Разное:** 1 проблем
- src\fake_sensor.cpp:14:27:: parameter 'pvParameters' is unused [misc-unused-parameters]

**Потенциальные баги:** 1 проблем
- src\fake_sensor.cpp:27:29:: narrowing conversion from 'int' to 'float' [bugprone-narrowing-conversions]

**Модернизация:** 2 проблем
- src\fake_sensor.cpp:109:53:: use nullptr [modernize-use-nullptr]
- src\fake_sensor.cpp:109:62:: use nullptr [modernize-use-nullptr]

### src/jxct_format_utils.cpp (3 предупреждений)
**Модернизация:** 3 проблем
- src\jxct_format_utils.cpp:8:5:: do not declare C-style arrays, use 'std::array' instead [modernize-avoid-c-arrays]
- src\jxct_format_utils.cpp:48:5:: do not declare C-style arrays, use 'std::array' instead [modernize-avoid-c-arrays]
- src\jxct_format_utils.cpp:68:12:: avoid repeating the return type from the declaration; use a braced initializer list instead [modernize-return-braced-init-list]

### src/jxct_ui_system.cpp (2 предупреждений)
**Модернизация:** 1 проблем
- src\jxct_ui_system.cpp:6:18:: do not declare C-style arrays, use 'std::array' instead [modernize-avoid-c-arrays]

**Потенциальные баги:** 1 проблем
- src\jxct_ui_system.cpp:286:58:: 2 adjacent parameters of 'generateButton' of similar type ('const char *') are easily swapped by mistake [bugprone-easily-swappable-parameters]

### src/logger.cpp (15 предупреждений)
**Безопасность:** 12 проблем
- src\logger.cpp:34:8:: do not define a C-style variadic function; consider using a function parameter pack or currying instead [cert-dcl50-cpp]
- src\logger.cpp:207:6:: do not define a C-style variadic function; consider using a function parameter pack or currying instead [cert-dcl50-cpp]
- src\logger.cpp:223:6:: do not define a C-style variadic function; consider using a function parameter pack or currying instead [cert-dcl50-cpp]
- ... и ещё 9 проблем

**Модернизация:** 1 проблем
- src\logger.cpp:45:16:: avoid repeating the return type from the declaration; use a braced initializer list instead [modernize-return-braced-init-list]

**Читаемость:** 2 проблем
- src\logger.cpp:428:9:: static member accessed through instance [readability-static-accessed-through-instance]
- src\logger.cpp:435:68:: static member accessed through instance [readability-static-accessed-through-instance]

### src/main.cpp (7 предупреждений)
**Разное:** 3 проблем
- src\main.cpp:75:9:: variable 'currentState' of type 'bool' can be declared 'const' [misc-const-correctness]
- src\main.cpp:228:5:: variable 'currentTime' of type 'unsigned long' can be declared 'const' [misc-const-correctness]
- src\main.cpp:284:9:: variable 'tsOk' of type 'bool' can be declared 'const' [misc-const-correctness]

**Модернизация:** 3 проблем
- src\main.cpp:140:22:: use nullptr [modernize-use-nullptr]
- src\main.cpp:203:55:: use nullptr [modernize-use-nullptr]
- src\main.cpp:203:64:: use nullptr [modernize-use-nullptr]

**Читаемость:** 1 проблем
- src\main.cpp:232:9:: implicit conversion 'NTPClient *' -> 'bool' [readability-implicit-bool-conversion]

### src/modbus_sensor.cpp (79 предупреждений)
**Читаемость:** 44 проблем
- src\modbus_sensor.cpp:47:34:: pointer parameter 'data' can be pointer to const [readability-non-const-parameter]
- src\modbus_sensor.cpp:56:17:: implicit conversion 'int' -> 'bool' [readability-implicit-bool-conversion]
- src\modbus_sensor.cpp:70:34:: parameter name 'd' is too short, expected at least 3 characters [readability-identifier-length]
- ... и ещё 41 проблем

**Модернизация:** 7 проблем
- src\modbus_sensor.cpp:84:12:: do not declare C-style arrays, use 'std::array' instead [modernize-avoid-c-arrays]
- src\modbus_sensor.cpp:394:13:: use auto when initializing with a cast to avoid duplicating the type name [modernize-use-auto]
- src\modbus_sensor.cpp:400:13:: use auto when initializing with a cast to avoid duplicating the type name [modernize-use-auto]
- ... и ещё 4 проблем

**Разное:** 24 проблем
- src\modbus_sensor.cpp:90:5:: variable 'spike' of type 'bool' can be declared 'const' [misc-const-correctness]
- src\modbus_sensor.cpp:136:5:: variable 'tempCalibrated' of type 'float' can be declared 'const' [misc-const-correctness]
- src\modbus_sensor.cpp:137:5:: variable 'humCalibrated' of type 'float' can be declared 'const' [misc-const-correctness]
- ... и ещё 21 проблем

**Потенциальные баги:** 4 проблем
- src\modbus_sensor.cpp:111:5:: switching on non-enum value without default case may not cover all cases [bugprone-switch-missing-default-case]
- src\modbus_sensor.cpp:513:12:: narrowing conversion from 'int' to 'float' [bugprone-narrowing-conversions]
- src\modbus_sensor.cpp:725:33:: loop variable has narrower type 'uint8_t' than iteration's upper bound 'int' [bugprone-too-small-loop-variable]
- ... и ещё 1 проблем

### src/mqtt_client.cpp (17 предупреждений)
**Читаемость:** 7 проблем
- src\mqtt_client.cpp:120:5:: do not use 'else' after 'return' [readability-else-after-return]
- src\mqtt_client.cpp:201:9:: static member accessed through instance [readability-static-accessed-through-instance]
- src\mqtt_client.cpp:506:28:: implicit conversion 'NTPClient *' -> 'bool' [readability-implicit-bool-conversion]
- ... и ещё 4 проблем

**Разное:** 9 проблем
- src\mqtt_client.cpp:179:5:: variable 'mqttServerIP' of type 'IPAddress' can be declared 'const' [misc-const-correctness]
- src\mqtt_client.cpp:225:5:: variable 'result' of type 'bool' can be declared 'const' [misc-const-correctness]
- src\mqtt_client.cpp:237:5:: variable 'state' of type 'int' can be declared 'const' [misc-const-correctness]
- ... и ещё 6 проблем

**Модернизация:** 1 проблем
- src\mqtt_client.cpp:807:12:: avoid repeating the return type from the declaration; use a braced initializer list instead [modernize-return-braced-init-list]

### src/ota_manager.cpp (24 предупреждений)
**Читаемость:** 7 проблем
- src\ota_manager.cpp:70:56:: implicit conversion 'const char *' -> 'bool' [readability-implicit-bool-conversion]
- src\ota_manager.cpp:71:51:: implicit conversion 'WiFiClient *' -> 'bool' [readability-implicit-bool-conversion]
- src\ota_manager.cpp:74:10:: implicit conversion 'const char *' -> 'bool' [readability-implicit-bool-conversion]
- ... и ещё 4 проблем

**Модернизация:** 6 проблем
- src\ota_manager.cpp:114:5:: do not declare C-style arrays, use 'std::array' instead [modernize-avoid-c-arrays]
- src\ota_manager.cpp:140:12:: do not declare C-style arrays, use 'std::array' instead [modernize-avoid-c-arrays]
- src\ota_manager.cpp:227:5:: do not declare C-style arrays, use 'std::array' instead [modernize-avoid-c-arrays]
- ... и ещё 3 проблем

**Потенциальные баги:** 2 проблем
- src\ota_manager.cpp:116:18:: result of multiplication in type 'int' is used as a pointer offset after an implicit widening conversion to type 'ptrdiff_t' [bugprone-implicit-widening-of-multiplication-result]
- src\ota_manager.cpp:554:25:: narrowing conversion from 'unsigned int' to signed type 'int' is implementation-defined [bugprone-narrowing-conversions]

**Разное:** 9 проблем
- src\ota_manager.cpp:128:5:: variable 'freeHeap' of type 'size_t' (aka 'unsigned long long') can be declared 'const' [misc-const-correctness]
- src\ota_manager.cpp:207:5:: variable 'heapBeforeDownload' of type 'size_t' (aka 'unsigned long long') can be declared 'const' [misc-const-correctness]
- src\ota_manager.cpp:232:5:: variable 'isChunked' of type 'bool' can be declared 'const' [misc-const-correctness]
- ... и ещё 6 проблем

### src/routes_calibration.cpp (1 предупреждений)
**Разное:** 1 проблем
- src\routes_calibration.cpp:97:9:: variable 'profileStr' of type 'String' can be declared 'const' [misc-const-correctness]

### src/sensor_compensation.cpp (22 предупреждений)
**Модернизация:** 6 проблем
- src\sensor_compensation.cpp:2:10:: inclusion of deprecated C++ header 'math.h'; consider using 'cmath' instead [modernize-deprecated-headers]
- src\sensor_compensation.cpp:3:10:: inclusion of deprecated C++ header 'time.h'; consider using 'ctime' instead [modernize-deprecated-headers]
- src\sensor_compensation.cpp:7:18:: do not declare C-style arrays, use 'std::array' instead [modernize-avoid-c-arrays]
- ... и ещё 3 проблем

**Читаемость:** 11 проблем
- src\sensor_compensation.cpp:26:19:: '*' has higher precedence than '-'; add parentheses to explicitly specify the order of operations [readability-math-missing-parentheses]
- src\sensor_compensation.cpp:30:19:: '*' has higher precedence than '-'; add parentheses to explicitly specify the order of operations [readability-math-missing-parentheses]
- src\sensor_compensation.cpp:34:19:: '*' has higher precedence than '-'; add parentheses to explicitly specify the order of operations [readability-math-missing-parentheses]
- ... и ещё 8 проблем

**Потенциальные баги:** 3 проблем
- src\sensor_compensation.cpp:40:30:: 2 adjacent parameters of 'correctEC' of similar type ('float') are easily swapped by mistake [bugprone-easily-swappable-parameters]
- src\sensor_compensation.cpp:59:17:: 2 adjacent parameters of 'correctNPK' of similar type ('float') are easily swapped by mistake [bugprone-easily-swappable-parameters]
- src\sensor_compensation.cpp:59:39:: 3 adjacent parameters of 'correctNPK' of similar type ('float &') are easily swapped by mistake [bugprone-easily-swappable-parameters]

**Разное:** 2 проблем
- src\sensor_compensation.cpp:47:5:: variable 'k' of type 'float' can be declared 'const' [misc-const-correctness]
- src\sensor_compensation.cpp:66:5:: variable 'idx' of type 'int' can be declared 'const' [misc-const-correctness]

### src/thingspeak_client.cpp (12 предупреждений)
**Модернизация:** 5 проблем
- src\thingspeak_client.cpp:5:10:: inclusion of deprecated C++ header 'ctype.h'; consider using 'cctype' instead [modernize-deprecated-headers]
- src\thingspeak_client.cpp:38:1:: do not declare C-style arrays, use 'std::array' instead [modernize-avoid-c-arrays]
- src\thingspeak_client.cpp:39:1:: do not declare C-style arrays, use 'std::array' instead [modernize-avoid-c-arrays]
- ... и ещё 2 проблем

**Читаемость:** 5 проблем
- src\thingspeak_client.cpp:24:10:: implicit conversion 'char *' -> 'bool' [readability-implicit-bool-conversion]
- src\thingspeak_client.cpp:29:49:: statement should be inside braces [readability-braces-around-statements]
- src\thingspeak_client.cpp:30:20:: statement should be inside braces [readability-braces-around-statements]
- ... и ещё 2 проблем

**Разное:** 2 проблем
- src\thingspeak_client.cpp:70:5:: variable 'now' of type 'unsigned long' can be declared 'const' [misc-const-correctness]
- src\thingspeak_client.cpp:109:5:: variable 'res' of type 'int' can be declared 'const' [misc-const-correctness]

### src/validation_utils.cpp (9 предупреждений)
**Читаемость:** 4 проблем
- src\validation_utils.cpp:79:21:: variable name 'ch' is too short, expected at least 3 characters [readability-identifier-length]
- src\validation_utils.cpp:315:37:: parameter name 'ip' is too short, expected at least 3 characters [readability-identifier-length]
- src\validation_utils.cpp:356:15:: variable name 'c' is too short, expected at least 3 characters [readability-identifier-length]
- ... и ещё 1 проблем

**Разное:** 5 проблем
- src\validation_utils.cpp:329:13:: variable 'part' of type 'String' can be declared 'const' [misc-const-correctness]
- src\validation_utils.cpp:330:13:: variable 'value' of type 'int' can be declared 'const' [misc-const-correctness]
- src\validation_utils.cpp:356:10:: variable 'c' of type 'char' can be declared 'const' [misc-const-correctness]
- ... и ещё 2 проблем

### src/wifi_manager.cpp (30 предупреждений)
**Модернизация:** 15 проблем
- src\wifi_manager.cpp:22:1:: replace macro with enum [modernize-macro-to-enum]
- src\wifi_manager.cpp:22:9:: macro 'RESET_BUTTON_PIN' defines an integral constant; prefer an enum instead [modernize-macro-to-enum]
- src\wifi_manager.cpp:23:9:: macro 'WIFI_RECONNECT_INTERVAL' defines an integral constant; prefer an enum instead [modernize-macro-to-enum]
- ... и ещё 12 проблем

**Разное:** 6 проблем
- src\wifi_manager.cpp:79:9:: variable 'now' of type 'unsigned long' can be declared 'const' [misc-const-correctness]
- src\wifi_manager.cpp:231:5:: variable 'apSsid' of type 'String' can be declared 'const' [misc-const-correctness]
- src\wifi_manager.cpp:248:5:: variable 'hostname' of type 'String' can be declared 'const' [misc-const-correctness]
- ... и ещё 3 проблем

**Читаемость:** 9 проблем
- src\wifi_manager.cpp:116:5:: static member accessed through instance [readability-static-accessed-through-instance]
- src\wifi_manager.cpp:172:13:: static member accessed through instance [readability-static-accessed-through-instance]
- src\wifi_manager.cpp:220:21:: implicit conversion 'char' -> 'bool' [readability-implicit-bool-conversion]
- ... и ещё 6 проблем

### src/web/csrf_protection.cpp (6 предупреждений)
**Разное:** 5 проблем
- src\web\csrf_protection.cpp:76:5:: variable 'isValid' of type 'bool' can be declared 'const' [misc-const-correctness]
- src\web\csrf_protection.cpp:104:5:: variable 'method' of type 'HTTPMethod' (aka 'http_method') can be declared 'const' [misc-const-correctness]
- src\web\csrf_protection.cpp:126:5:: variable 'clientIP' of type 'String' can be declared 'const' [misc-const-correctness]
- ... и ещё 2 проблем

**Модернизация:** 1 проблем
- src\web\csrf_protection.cpp:98:12:: escaped string literal can be written as a raw string literal [modernize-raw-string-literal]

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
- src\web\routes_config.cpp:406:25:: variable name 'up' is too short, expected at least 3 characters [readability-identifier-length]
- src\web\routes_config.cpp:442:5:: confusing array subscript expression, usually the index is inside the [] [readability-misplaced-array-index]
- ... и ещё 23 проблем

**Модернизация:** 1 проблем
- src\web\routes_config.cpp:362:68:: escaped string literal can be written as a raw string literal [modernize-raw-string-literal]

### src/web/routes_data.cpp (49 предупреждений)
**Модернизация:** 3 проблем
- src\web\routes_data.cpp:10:10:: inclusion of deprecated C++ header 'time.h'; consider using 'ctime' instead [modernize-deprecated-headers]
- src\web\routes_data.cpp:281:64:: escaped string literal can be written as a raw string literal [modernize-raw-string-literal]
- src\web\routes_data.cpp:702:21:: escaped string literal can be written as a raw string literal [modernize-raw-string-literal]

**Потенциальные баги:** 5 проблем
- src\web\routes_data.cpp:76:50:: repeated branch body in conditional chain [bugprone-branch-clone]
- src\web\routes_data.cpp:106:5:: repeated branch body in conditional chain [bugprone-branch-clone]
- src\web\routes_data.cpp:169:13:: repeated branch body in conditional chain [bugprone-branch-clone]
- ... и ещё 2 проблем

**Разное:** 8 проблем
- src\web\routes_data.cpp:95:5:: variable 'soil' of type 'int' can be declared 'const' [misc-const-correctness]
- src\web\routes_data.cpp:145:9:: variable 'rainy' of type 'bool' can be declared 'const' [misc-const-correctness]
- src\web\routes_data.cpp:257:9:: variable 'profileStr' of type 'String' can be declared 'const' [misc-const-correctness]
- ... и ещё 5 проблем

**Читаемость:** 33 проблем
- src\web\routes_data.cpp:144:13:: variable name 'm' is too short, expected at least 3 characters [readability-identifier-length]
- src\web\routes_data.cpp:144:17:: implicit conversion 'struct tm *' -> 'bool' [readability-implicit-bool-conversion]
- src\web\routes_data.cpp:258:34:: statement should be inside braces [readability-braces-around-statements]
- ... и ещё 30 проблем

### src/web/routes_main.cpp (18 предупреждений)
**Разное:** 18 проблем
- src\web\routes_main.cpp:37:17:: variable 'msg' of type 'String' can be declared 'const' [misc-const-correctness]
- src\web\routes_main.cpp:38:17:: variable 'html' of type 'String' can be declared 'const' [misc-const-correctness]
- src\web\routes_main.cpp:47:21:: variable 'hostRes' of type 'ValidationResult' can be declared 'const' [misc-const-correctness]
- ... и ещё 15 проблем

### src/web/routes_ota.cpp (8 предупреждений)
**Модернизация:** 5 проблем
- src\web\routes_ota.cpp:43:81:: escaped string literal can be written as a raw string literal [modernize-raw-string-literal]
- src\web\routes_ota.cpp:57:81:: escaped string literal can be written as a raw string literal [modernize-raw-string-literal]
- src\web\routes_ota.cpp:441:65:: escaped string literal can be written as a raw string literal [modernize-raw-string-literal]
- ... и ещё 2 проблем

**Разное:** 2 проблем
- src\web\routes_ota.cpp:134:17:: variable 'uploadBtn' of type 'String' can be declared 'const' [misc-const-correctness]
- src\web\routes_ota.cpp:351:13:: variable 'percent' of type 'int' can be declared 'const' [misc-const-correctness]

**Потенциальные баги:** 1 проблем
- src\web\routes_ota.cpp:351:27:: narrowing conversion from 'size_t' (aka 'unsigned long long') to signed type 'int' is implementation-defined [bugprone-narrowing-conversions]

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
- Проблемы безопасности - могут создать уязвимости

### 🟡 СРЕДНИЕ (исправить в ближайшее время)
- Проблемы читаемости - затрудняют сопровождение кода
- Модернизация - улучшают качество кода

### 🟢 НИЗКИЕ (можно исправить позже)
- Разные мелкие проблемы
