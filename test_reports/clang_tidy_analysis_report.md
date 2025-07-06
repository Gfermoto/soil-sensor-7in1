# CLANG-TIDY ПОЛНЫЙ ОТЧЁТ АНАЛИЗА
**Дата анализа:** 05.07.2025 20:01
**Версия clang-tidy:** 20.1.0

## 📊 СТАТИСТИКА
- **Всего файлов проанализировано:** 24
- **Успешно проанализировано:** 24
- **Ошибки анализа:** 0
- **Всего предупреждений:** 605

## 🔍 КАТЕГОРИИ ПРОБЛЕМ
### Разное (229 проблем)
- `misc-use-anonymous-namespace`: 81 случаев
- `misc-const-correctness`: 74 случаев
- `misc-use-internal-linkage`: 70 случаев
- `misc-unused-parameters`: 3 случаев
- `64`: 1 случаев

### Модернизация (87 проблем)
- `modernize-avoid-c-arrays`: 59 случаев
- `modernize-macro-to-enum`: 12 случаев
- `modernize-raw-string-literal`: 9 случаев
- `modernize-deprecated-headers`: 4 случаев
- `modernize-use-auto`: 3 случаев

### Читаемость (243 проблем)
- `readability-braces-around-statements`: 100 случаев
- `readability-uppercase-literal-suffix`: 60 случаев
- `readability-identifier-length`: 34 случаев
- `readability-redundant-declaration`: 21 случаев
- `readability-math-missing-parentheses`: 6 случаев

### Потенциальные баги (36 проблем)
- `bugprone-easily-swappable-parameters`: 16 случаев
- `bugprone-narrowing-conversions`: 11 случаев
- `bugprone-branch-clone`: 6 случаев
- `bugprone-too-small-loop-variable`: 1 случаев
- `bugprone-reserved-identifier,cert-dcl37-c,cert-dcl51-cpp`: 1 случаев

### Безопасность (10 проблем)
- `cert-dcl50-cpp`: 10 случаев

## 📁 ДЕТАЛЬНЫЙ АНАЛИЗ ПО ФАЙЛАМ
### src/calibration_manager.cpp (7 предупреждений)
**Разное:** 4 проблем
- src/calibration_manager.cpp:7:13:: variable '_initialized' declared 'static', move to anonymous namespace instead [misc-use-anonymous-namespace]
- src/calibration_manager.cpp:37:6:: function 'saveCsv' can be made static or moved into an anonymous namespace to enforce internal linkage [misc-use-internal-linkage]
- src/calibration_manager.cpp:178:9:: variable 'ratio' of type 'float' can be declared 'const' [misc-const-correctness]
- ... и ещё 1 проблем

**Модернизация:** 1 проблем
- src/calibration_manager.cpp:139:5:: do not declare C-style arrays, use 'std::array' instead [modernize-avoid-c-arrays]

**Читаемость:** 2 проблем
- src/calibration_manager.cpp:179:47:: '*' has higher precedence than '+'; add parentheses to explicitly specify the order of operations [readability-math-missing-parentheses]
- src/calibration_manager.cpp:182:5:: do not use 'else' after 'return' [readability-else-after-return]

### src/config.cpp (7 предупреждений)
**Модернизация:** 4 проблем
- C:\Git\JXCT\src\config.cpp:18:16:: do not declare C-style arrays, use 'std::array' instead [modernize-avoid-c-arrays]
- C:\Git\JXCT\src\config.cpp:20:5:: do not declare C-style arrays, use 'std::array' instead [modernize-avoid-c-arrays]
- C:\Git\JXCT\src\config.cpp:27:16:: do not declare C-style arrays, use 'std::array' instead [modernize-avoid-c-arrays]
- ... и ещё 1 проблем

**Читаемость:** 3 проблем
- C:\Git\JXCT\src\config.cpp:102:51:: floating point literal has suffix 'f', which is not uppercase [readability-uppercase-literal-suffix]
- C:\Git\JXCT\src\config.cpp:103:52:: floating point literal has suffix 'f', which is not uppercase [readability-uppercase-literal-suffix]
- C:\Git\JXCT\src\config.cpp:106:71:: floating point literal has suffix 'f', which is not uppercase [readability-uppercase-literal-suffix]

### src/fake_sensor.cpp (7 предупреждений)
**Разное:** 3 проблем
- C:\Git\JXCT\src\fake_sensor.cpp:14:6:: function 'fakeSensorTask' can be made static or moved into an anonymous namespace to enforce internal linkage [misc-use-internal-linkage]
- C:\Git\JXCT\src\fake_sensor.cpp:14:27:: parameter 'pvParameters' is unused [misc-unused-parameters]
- C:\Git\JXCT\src\fake_sensor.cpp:81:17:: variable 'ec25' of type 'float' can be declared 'const' [misc-const-correctness]

**Читаемость:** 4 проблем
- C:\Git\JXCT\src\fake_sensor.cpp:36:42:: floating point literal has suffix 'f', which is not uppercase [readability-uppercase-literal-suffix]
- C:\Git\JXCT\src\fake_sensor.cpp:81:47:: floating point literal has suffix 'f', which is not uppercase [readability-uppercase-literal-suffix]
- C:\Git\JXCT\src\fake_sensor.cpp:81:54:: floating point literal has suffix 'f', which is not uppercase [readability-uppercase-literal-suffix]
- ... и ещё 1 проблем

### src/jxct_format_utils.cpp (8 предупреждений)
**Разное:** 6 проблем
- src/jxct_format_utils.cpp:6:13:: function 'formatFloat' can be made static or moved into an anonymous namespace to enforce internal linkage [misc-use-internal-linkage]
- src/jxct_format_utils.cpp:20:13:: function 'format_moisture' can be made static or moved into an anonymous namespace to enforce internal linkage [misc-use-internal-linkage]
- src/jxct_format_utils.cpp:25:13:: function 'format_temperature' can be made static or moved into an anonymous namespace to enforce internal linkage [misc-use-internal-linkage]
- ... и ещё 3 проблем

**Модернизация:** 2 проблем
- src/jxct_format_utils.cpp:8:5:: do not declare C-style arrays, use 'std::array' instead [modernize-avoid-c-arrays]
- src/jxct_format_utils.cpp:48:5:: do not declare C-style arrays, use 'std::array' instead [modernize-avoid-c-arrays]

### src/jxct_ui_system.cpp (2 предупреждений)
**Модернизация:** 1 проблем
- C:\Git\JXCT\src\jxct_ui_system.cpp:6:18:: do not declare C-style arrays, use 'std::array' instead [modernize-avoid-c-arrays]

**Потенциальные баги:** 1 проблем
- C:\Git\JXCT\src\jxct_ui_system.cpp:286:58:: 2 adjacent parameters of 'generateButton' of similar type ('const char *') are easily swapped by mistake [bugprone-easily-swappable-parameters]

### src/logger.cpp (27 предупреждений)
**Разное:** 6 проблем
- src/logger.cpp:15:8:: function 'getUptimeString' can be made static or moved into an anonymous namespace to enforce internal linkage [misc-use-internal-linkage]
- src/logger.cpp:17:5:: variable 'totalSeconds' of type 'unsigned long' can be declared 'const' [misc-const-correctness]
- src/logger.cpp:18:5:: variable 'hours' of type 'unsigned long' can be declared 'const' [misc-const-correctness]
- ... и ещё 3 проблем

**Потенциальные баги:** 1 проблем
- src/logger.cpp:38:9:: switch has 2 consecutive identical branches [bugprone-branch-clone]

**Безопасность:** 10 проблем
- src/logger.cpp:79:6:: do not define a C-style variadic function; consider using a function parameter pack or currying instead [cert-dcl50-cpp]
- src/logger.cpp:98:6:: do not define a C-style variadic function; consider using a function parameter pack or currying instead [cert-dcl50-cpp]
- src/logger.cpp:117:6:: do not define a C-style variadic function; consider using a function parameter pack or currying instead [cert-dcl50-cpp]
- ... и ещё 7 проблем

**Модернизация:** 10 проблем
- src/logger.cpp:91:5:: do not declare C-style arrays, use 'std::array' instead [modernize-avoid-c-arrays]
- src/logger.cpp:110:5:: do not declare C-style arrays, use 'std::array' instead [modernize-avoid-c-arrays]
- src/logger.cpp:129:5:: do not declare C-style arrays, use 'std::array' instead [modernize-avoid-c-arrays]
- ... и ещё 7 проблем

### src/main.cpp (24 предупреждений)
**Разное:** 17 проблем
- C:\Git\JXCT\src\main.cpp:30:15:: variable 'lastDataPublish' can be made static or moved into an anonymous namespace to enforce internal linkage [misc-use-internal-linkage]
- C:\Git\JXCT\src\main.cpp:31:15:: variable 'lastNtpUpdate' can be made static or moved into an anonymous namespace to enforce internal linkage [misc-use-internal-linkage]
- C:\Git\JXCT\src\main.cpp:34:6:: function 'initPreferences' can be made static or moved into an anonymous namespace to enforce internal linkage [misc-use-internal-linkage]
- ... и ещё 14 проблем

**Читаемость:** 7 проблем
- C:\Git\JXCT\src\main.cpp:35:6:: redundant 'setupWiFi' declaration [readability-redundant-declaration]
- C:\Git\JXCT\src\main.cpp:36:6:: redundant 'setupModbus' declaration [readability-redundant-declaration]
- C:\Git\JXCT\src\main.cpp:37:6:: redundant 'loadConfig' declaration [readability-redundant-declaration]
- ... и ещё 4 проблем

### src/modbus_sensor.cpp (102 предупреждений)
**Разное:** 33 проблем
- C:\Git\JXCT\src\modbus_sensor.cpp:17:14:: variable 'modbus' can be made static or moved into an anonymous namespace to enforce internal linkage [misc-use-internal-linkage]
- C:\Git\JXCT\src\modbus_sensor.cpp:22:22:: variable 'lastIrrigationTs' declared 'static', move to anonymous namespace instead [misc-use-anonymous-namespace]
- C:\Git\JXCT\src\modbus_sensor.cpp:24:6:: function 'debugPrintBuffer' can be made static or moved into an anonymous namespace to enforce internal linkage [misc-use-internal-linkage]
- ... и ещё 30 проблем

**Читаемость:** 54 проблем
- C:\Git\JXCT\src\modbus_sensor.cpp:24:52:: pointer parameter 'buffer' can be pointer to const [readability-non-const-parameter]
- C:\Git\JXCT\src\modbus_sensor.cpp:113:34:: pointer parameter 'data' can be pointer to const [readability-non-const-parameter]
- C:\Git\JXCT\src\modbus_sensor.cpp:122:17:: implicit conversion 'int' -> 'bool' [readability-implicit-bool-conversion]
- ... и ещё 51 проблем

**Модернизация:** 4 проблем
- C:\Git\JXCT\src\modbus_sensor.cpp:299:13:: use auto when initializing with a cast to avoid duplicating the type name [modernize-use-auto]
- C:\Git\JXCT\src\modbus_sensor.cpp:305:13:: use auto when initializing with a cast to avoid duplicating the type name [modernize-use-auto]
- C:\Git\JXCT\src\modbus_sensor.cpp:380:12:: do not declare C-style arrays, use 'std::array' instead [modernize-avoid-c-arrays]
- ... и ещё 1 проблем

**Потенциальные баги:** 11 проблем
- C:\Git\JXCT\src\modbus_sensor.cpp:513:12:: narrowing conversion from 'int' to 'float' [bugprone-narrowing-conversions]
- C:\Git\JXCT\src\modbus_sensor.cpp:622:43:: 7 adjacent parameters of 'addToMovingAverage' of similar type ('float') are easily swapped by mistake [bugprone-easily-swappable-parameters]
- C:\Git\JXCT\src\modbus_sensor.cpp:668:39:: narrowing conversion from 'int' to 'float' [bugprone-narrowing-conversions]
- ... и ещё 8 проблем

### src/mqtt_client.cpp (70 предупреждений)
**Разное:** 41 проблем
- C:\Git\JXCT\src\mqtt_client.cpp:22:12:: variable 'espClient' can be made static or moved into an anonymous namespace to enforce internal linkage [misc-use-internal-linkage]
- C:\Git\JXCT\src\mqtt_client.cpp:23:14:: variable 'mqttClient' can be made static or moved into an anonymous namespace to enforce internal linkage [misc-use-internal-linkage]
- C:\Git\JXCT\src\mqtt_client.cpp:24:6:: variable 'mqttConnected' can be made static or moved into an anonymous namespace to enforce internal linkage [misc-use-internal-linkage]
- ... и ещё 38 проблем

**Модернизация:** 22 проблем
- C:\Git\JXCT\src\mqtt_client.cpp:27:8:: do not declare C-style arrays, use 'std::array' instead [modernize-avoid-c-arrays]
- C:\Git\JXCT\src\mqtt_client.cpp:34:8:: do not declare C-style arrays, use 'std::array' instead [modernize-avoid-c-arrays]
- C:\Git\JXCT\src\mqtt_client.cpp:35:8:: do not declare C-style arrays, use 'std::array' instead [modernize-avoid-c-arrays]
- ... и ещё 19 проблем

**Читаемость:** 7 проблем
- C:\Git\JXCT\src\mqtt_client.cpp:100:5:: do not use 'else' after 'return' [readability-else-after-return]
- C:\Git\JXCT\src\mqtt_client.cpp:129:41:: statement should be inside braces [readability-braces-around-statements]
- C:\Git\JXCT\src\mqtt_client.cpp:136:42:: statement should be inside braces [readability-braces-around-statements]
- ... и ещё 4 проблем

### src/ota_manager.cpp (31 предупреждений)
**Модернизация:** 8 проблем
- C:\Git\JXCT\src\ota_manager.cpp:16:8:: do not declare C-style arrays, use 'std::array' instead [modernize-avoid-c-arrays]
- C:\Git\JXCT\src\ota_manager.cpp:17:8:: do not declare C-style arrays, use 'std::array' instead [modernize-avoid-c-arrays]
- C:\Git\JXCT\src\ota_manager.cpp:18:8:: do not declare C-style arrays, use 'std::array' instead [modernize-avoid-c-arrays]
- ... и ещё 5 проблем

**Разное:** 17 проблем
- C:\Git\JXCT\src\ota_manager.cpp:16:13:: variable 'statusBuf' declared 'static', move to anonymous namespace instead [misc-use-anonymous-namespace]
- C:\Git\JXCT\src\ota_manager.cpp:17:13:: variable 'guardGap' declared 'static', move to anonymous namespace instead [misc-use-anonymous-namespace]
- C:\Git\JXCT\src\ota_manager.cpp:18:13:: variable 'manifestUrlGlobal' declared 'static', move to anonymous namespace instead [misc-use-anonymous-namespace]
- ... и ещё 14 проблем

**Потенциальные баги:** 3 проблем
- C:\Git\JXCT\src\ota_manager.cpp:30:13:: declaration uses identifier '_printGuard', which is reserved in the global namespace [bugprone-reserved-identifier,cert-dcl37-c,cert-dcl51-cpp]
- C:\Git\JXCT\src\ota_manager.cpp:112:43:: result of multiplication in type 'int' is used as a pointer offset after an implicit widening conversion to type 'ptrdiff_t' [bugprone-implicit-widening-of-multiplication-result]
- C:\Git\JXCT\src\ota_manager.cpp:269:35:: narrowing conversion from 'size_t' (aka 'unsigned long long') to signed type 'int' is implementation-defined [bugprone-narrowing-conversions]

**Читаемость:** 3 проблем
- C:\Git\JXCT\src\ota_manager.cpp:67:56:: implicit conversion 'const char *' -> 'bool' [readability-implicit-bool-conversion]
- C:\Git\JXCT\src\ota_manager.cpp:71:10:: implicit conversion 'const char *' -> 'bool' [readability-implicit-bool-conversion]
- C:\Git\JXCT\src\ota_manager.cpp:112:33:: statement should be inside braces [readability-braces-around-statements]

### src/routes_calibration.cpp (16 предупреждений)
**Читаемость:** 6 проблем
- C:\Git\JXCT\src\routes_calibration.cpp:11:18:: redundant 'webServer' declaration [readability-redundant-declaration]
- C:\Git\JXCT\src\routes_calibration.cpp:96:34:: statement should be inside braces [readability-braces-around-statements]
- C:\Git\JXCT\src\routes_calibration.cpp:98:43:: statement should be inside braces [readability-braces-around-statements]
- ... и ещё 3 проблем

**Разное:** 10 проблем
- C:\Git\JXCT\src\routes_calibration.cpp:18:15:: function 'generateCalibrationPage' declared 'static', move to anonymous namespace instead [misc-use-anonymous-namespace]
- C:\Git\JXCT\src\routes_calibration.cpp:46:5:: variable 'hasSand' of type 'bool' can be declared 'const' [misc-const-correctness]
- C:\Git\JXCT\src\routes_calibration.cpp:47:5:: variable 'hasSandPeat' of type 'bool' can be declared 'const' [misc-const-correctness]
- ... и ещё 7 проблем

### src/sensor_compensation.cpp (69 предупреждений)
**Модернизация:** 6 проблем
- src/sensor_compensation.cpp:2:10:: inclusion of deprecated C++ header 'math.h'; consider using 'cmath' instead [modernize-deprecated-headers]
- src/sensor_compensation.cpp:3:10:: inclusion of deprecated C++ header 'time.h'; consider using 'ctime' instead [modernize-deprecated-headers]
- src/sensor_compensation.cpp:7:18:: do not declare C-style arrays, use 'std::array' instead [modernize-avoid-c-arrays]
- ... и ещё 3 проблем

**Читаемость:** 55 проблем
- src/sensor_compensation.cpp:11:6:: floating point literal has suffix 'f', which is not uppercase [readability-uppercase-literal-suffix]
- src/sensor_compensation.cpp:12:6:: floating point literal has suffix 'f', which is not uppercase [readability-uppercase-literal-suffix]
- src/sensor_compensation.cpp:13:6:: floating point literal has suffix 'f', which is not uppercase [readability-uppercase-literal-suffix]
- ... и ещё 52 проблем

**Разное:** 5 проблем
- src/sensor_compensation.cpp:23:21:: function 'k_h_N' declared 'static', move to anonymous namespace instead [misc-use-anonymous-namespace]
- src/sensor_compensation.cpp:27:21:: function 'k_h_P' declared 'static', move to anonymous namespace instead [misc-use-anonymous-namespace]
- src/sensor_compensation.cpp:31:21:: function 'k_h_K' declared 'static', move to anonymous namespace instead [misc-use-anonymous-namespace]
- ... и ещё 2 проблем

**Потенциальные баги:** 3 проблем
- src/sensor_compensation.cpp:38:30:: 2 adjacent parameters of 'correctEC' of similar type ('float') are easily swapped by mistake [bugprone-easily-swappable-parameters]
- src/sensor_compensation.cpp:57:17:: 2 adjacent parameters of 'correctNPK' of similar type ('float') are easily swapped by mistake [bugprone-easily-swappable-parameters]
- src/sensor_compensation.cpp:57:39:: 3 adjacent parameters of 'correctNPK' of similar type ('float &') are easily swapped by mistake [bugprone-easily-swappable-parameters]

### src/thingspeak_client.cpp (25 предупреждений)
**Модернизация:** 5 проблем
- C:\Git\JXCT\src\thingspeak_client.cpp:5:10:: inclusion of deprecated C++ header 'ctype.h'; consider using 'cctype' instead [modernize-deprecated-headers]
- C:\Git\JXCT\src\thingspeak_client.cpp:35:8:: do not declare C-style arrays, use 'std::array' instead [modernize-avoid-c-arrays]
- C:\Git\JXCT\src\thingspeak_client.cpp:36:8:: do not declare C-style arrays, use 'std::array' instead [modernize-avoid-c-arrays]
- ... и ещё 2 проблем

**Разное:** 8 проблем
- C:\Git\JXCT\src\thingspeak_client.cpp:15:13:: variable 'THINGSPEAK_API_URL' can be made static or moved into an anonymous namespace to enforce internal linkage [misc-use-internal-linkage]
- C:\Git\JXCT\src\thingspeak_client.cpp:17:22:: variable 'lastTsPublish' declared 'static', move to anonymous namespace instead [misc-use-anonymous-namespace]
- C:\Git\JXCT\src\thingspeak_client.cpp:18:12:: variable 'consecutiveFailCount' declared 'static', move to anonymous namespace instead [misc-use-anonymous-namespace]
- ... и ещё 5 проблем

**Читаемость:** 12 проблем
- C:\Git\JXCT\src\thingspeak_client.cpp:21:24:: parameter name 's' is too short, expected at least 3 characters [readability-identifier-length]
- C:\Git\JXCT\src\thingspeak_client.cpp:23:10:: implicit conversion 'char *' -> 'bool' [readability-implicit-bool-conversion]
- C:\Git\JXCT\src\thingspeak_client.cpp:23:12:: statement should be inside braces [readability-braces-around-statements]
- ... и ещё 9 проблем

### src/validation_utils.cpp (4 предупреждений)
**Читаемость:** 4 проблем
- src/validation_utils.cpp:145:35:: parameter name 'ph' is too short, expected at least 3 characters [readability-identifier-length]
- src/validation_utils.cpp:150:35:: parameter name 'ec' is too short, expected at least 3 characters [readability-identifier-length]
- src/validation_utils.cpp:315:37:: parameter name 'ip' is too short, expected at least 3 characters [readability-identifier-length]
- ... и ещё 1 проблем

### src/wifi_manager.cpp (29 предупреждений)
**Модернизация:** 14 проблем
- C:\Git\JXCT\src\wifi_manager.cpp:22:1:: replace macro with enum [modernize-macro-to-enum]
- C:\Git\JXCT\src\wifi_manager.cpp:22:9:: macro 'RESET_BUTTON_PIN' defines an integral constant; prefer an enum instead [modernize-macro-to-enum]
- C:\Git\JXCT\src\wifi_manager.cpp:23:9:: macro 'WIFI_RECONNECT_INTERVAL' defines an integral constant; prefer an enum instead [modernize-macro-to-enum]
- ... и ещё 11 проблем

**Разное:** 12 проблем
- C:\Git\JXCT\src\wifi_manager.cpp:39:11:: variable 'dnsServer' can be made static or moved into an anonymous namespace to enforce internal linkage [misc-use-internal-linkage]
- C:\Git\JXCT\src\wifi_manager.cpp:42:15:: variable 'ledLastToggle' can be made static or moved into an anonymous namespace to enforce internal linkage [misc-use-internal-linkage]
- C:\Git\JXCT\src\wifi_manager.cpp:43:6:: variable 'ledState' can be made static or moved into an anonymous namespace to enforce internal linkage [misc-use-internal-linkage]
- ... и ещё 9 проблем

**Читаемость:** 3 проблем
- C:\Git\JXCT\src\wifi_manager.cpp:50:6:: redundant 'handleRoot' declaration [readability-redundant-declaration]
- C:\Git\JXCT\src\wifi_manager.cpp:223:21:: implicit conversion 'char' -> 'bool' [readability-implicit-bool-conversion]
- C:\Git\JXCT\src\wifi_manager.cpp:223:33:: statement should be inside braces [readability-braces-around-statements]

### src/web/csrf_protection.cpp (10 предупреждений)
**Разное:** 9 проблем
- C:\Git\JXCT\src\web\csrf_protection.cpp:20:15:: variable 'currentCSRFToken' declared 'static', move to anonymous namespace instead [misc-use-anonymous-namespace]
- C:\Git\JXCT\src\web\csrf_protection.cpp:21:22:: variable 'tokenGeneratedTime' declared 'static', move to anonymous namespace instead [misc-use-anonymous-namespace]
- C:\Git\JXCT\src\web\csrf_protection.cpp:39:5:: variable 'macAddr' of type 'String' (aka 'int') can be declared 'const' [misc-const-correctness]
- ... и ещё 6 проблем

**Модернизация:** 1 проблем
- C:\Git\JXCT\src\web\csrf_protection.cpp:96:12:: escaped string literal can be written as a raw string literal [modernize-raw-string-literal]

### src/web/error_handlers.cpp (11 предупреждений)
**Разное:** 4 проблем
- C:\Git\JXCT\src\web\error_handlers.cpp:9:6:: function 'validateInterval' can be made static or moved into an anonymous namespace to enforce internal linkage [misc-use-internal-linkage]
- C:\Git\JXCT\src\web\error_handlers.cpp:29:13:: variable 'uri' of type 'String' (aka 'int') can be declared 'const' [misc-const-correctness]
- C:\Git\JXCT\src\web\error_handlers.cpp:101:6:: function 'isFeatureAvailable' can be made static or moved into an anonymous namespace to enforce internal linkage [misc-use-internal-linkage]
- ... и ещё 1 проблем

**Потенциальные баги:** 1 проблем
- C:\Git\JXCT\src\web\error_handlers.cpp:9:23:: 2 adjacent parameters of 'validateInterval' of similar type are easily swapped by mistake [bugprone-easily-swappable-parameters]

**Читаемость:** 6 проблем
- C:\Git\JXCT\src\web\error_handlers.cpp:83:67:: statement should be inside braces [readability-braces-around-statements]
- C:\Git\JXCT\src\web\error_handlers.cpp:84:84:: statement should be inside braces [readability-braces-around-statements]
- C:\Git\JXCT\src\web\error_handlers.cpp:85:104:: statement should be inside braces [readability-braces-around-statements]
- ... и ещё 3 проблем

### src/web/routes_config.cpp (14 предупреждений)
**Читаемость:** 8 проблем
- C:\Git\JXCT\src\web\routes_config.cpp:19:18:: redundant 'webServer' declaration [readability-redundant-declaration]
- C:\Git\JXCT\src\web\routes_config.cpp:20:17:: redundant 'currentWiFiMode' declaration [readability-redundant-declaration]
- C:\Git\JXCT\src\web\routes_config.cpp:24:13:: redundant 'loadConfig' declaration [readability-redundant-declaration]
- ... и ещё 5 проблем

**Разное:** 5 проблем
- C:\Git\JXCT\src\web\routes_config.cpp:28:13:: function 'sendConfigExportJson' declared 'static', move to anonymous namespace instead [misc-use-anonymous-namespace]
- C:\Git\JXCT\src\web\routes_config.cpp:195:22:: variable 'valSensor' of type 'ValidationResult' can be declared 'const' [misc-const-correctness]
- C:\Git\JXCT\src\web\routes_config.cpp:196:22:: variable 'valMqtt' of type 'ValidationResult' can be declared 'const' [misc-const-correctness]
- ... и ещё 2 проблем

**Модернизация:** 1 проблем
- C:\Git\JXCT\src\web\routes_config.cpp:368:68:: escaped string literal can be written as a raw string literal [modernize-raw-string-literal]

### src/web/routes_data.cpp (70 предупреждений)
**Модернизация:** 3 проблем
- C:\Git\JXCT\src\web\routes_data.cpp:10:10:: inclusion of deprecated C++ header 'time.h'; consider using 'ctime' instead [modernize-deprecated-headers]
- C:\Git\JXCT\src\web\routes_data.cpp:272:64:: escaped string literal can be written as a raw string literal [modernize-raw-string-literal]
- C:\Git\JXCT\src\web\routes_data.cpp:683:21:: escaped string literal can be written as a raw string literal [modernize-raw-string-literal]

**Читаемость:** 54 проблем
- C:\Git\JXCT\src\web\routes_data.cpp:27:15:: redundant 'formatValue' declaration [readability-redundant-declaration]
- C:\Git\JXCT\src\web\routes_data.cpp:44:17:: variable name 'id' is too short, expected at least 3 characters [readability-identifier-length]
- C:\Git\JXCT\src\web\routes_data.cpp:47:39:: statement should be inside braces [readability-braces-around-statements]
- ... и ещё 51 проблем

**Разное:** 8 проблем
- C:\Git\JXCT\src\web\routes_data.cpp:31:13:: variable 'uploadFile' declared 'static', move to anonymous namespace instead [misc-use-anonymous-namespace]
- C:\Git\JXCT\src\web\routes_data.cpp:32:20:: variable 'uploadProfile' declared 'static', move to anonymous namespace instead [misc-use-anonymous-namespace]
- C:\Git\JXCT\src\web\routes_data.cpp:39:18:: function 'computeRecommendations' declared 'static', move to anonymous namespace instead [misc-use-anonymous-namespace]
- ... и ещё 5 проблем

**Потенциальные баги:** 5 проблем
- C:\Git\JXCT\src\web\routes_data.cpp:75:13:: repeated branch body in conditional chain [bugprone-branch-clone]
- C:\Git\JXCT\src\web\routes_data.cpp:103:5:: repeated branch body in conditional chain [bugprone-branch-clone]
- C:\Git\JXCT\src\web\routes_data.cpp:163:13:: repeated branch body in conditional chain [bugprone-branch-clone]
- ... и ещё 2 проблем

### src/web/routes_main.cpp (12 предупреждений)
**Разное:** 5 проблем
- C:\Git\JXCT\src\web\routes_main.cpp:37:17:: variable 'msg' of type 'String' (aka 'int') can be declared 'const' [misc-const-correctness]
- C:\Git\JXCT\src\web\routes_main.cpp:38:17:: variable 'html' of type 'String' (aka 'int') can be declared 'const' [misc-const-correctness]
- C:\Git\JXCT\src\web\routes_main.cpp:51:25:: variable 'msg' of type 'String' (aka 'int') can be declared 'const' [misc-const-correctness]
- ... и ещё 2 проблем

**Читаемость:** 7 проблем
- C:\Git\JXCT\src\web\routes_main.cpp:91:50:: statement should be inside braces [readability-braces-around-statements]
- C:\Git\JXCT\src\web\routes_main.cpp:93:21:: statement should be inside braces [readability-braces-around-statements]
- C:\Git\JXCT\src\web\routes_main.cpp:101:50:: statement should be inside braces [readability-braces-around-statements]
- ... и ещё 4 проблем

### src/web/routes_ota.cpp (19 предупреждений)
**Читаемость:** 2 проблем
- C:\Git\JXCT\src\web\routes_ota.cpp:14:18:: redundant 'webServer' declaration [readability-redundant-declaration]
- C:\Git\JXCT\src\web\routes_ota.cpp:15:17:: redundant 'currentWiFiMode' declaration [readability-redundant-declaration]

**Разное:** 11 проблем
- C:\Git\JXCT\src\web\routes_ota.cpp:19:13:: variable 'isLocalUploadActive' declared 'static', move to anonymous namespace instead [misc-use-anonymous-namespace]
- C:\Git\JXCT\src\web\routes_ota.cpp:20:15:: variable 'localUploadProgress' declared 'static', move to anonymous namespace instead [misc-use-anonymous-namespace]
- C:\Git\JXCT\src\web\routes_ota.cpp:21:15:: variable 'localUploadTotal' declared 'static', move to anonymous namespace instead [misc-use-anonymous-namespace]
- ... и ещё 8 проблем

**Модернизация:** 5 проблем
- C:\Git\JXCT\src\web\routes_ota.cpp:42:81:: escaped string literal can be written as a raw string literal [modernize-raw-string-literal]
- C:\Git\JXCT\src\web\routes_ota.cpp:56:81:: escaped string literal can be written as a raw string literal [modernize-raw-string-literal]
- C:\Git\JXCT\src\web\routes_ota.cpp:438:65:: escaped string literal can be written as a raw string literal [modernize-raw-string-literal]
- ... и ещё 2 проблем

**Потенциальные баги:** 1 проблем
- C:\Git\JXCT\src\web\routes_ota.cpp:349:27:: narrowing conversion from 'size_t' (aka 'unsigned long long') to signed type 'int' is implementation-defined [bugprone-narrowing-conversions]

### src/web/routes_reports.cpp (16 предупреждений)
**Разное:** 16 проблем
- C:\Git\JXCT\src\web\routes_reports.cpp:32:20:: variable 'lastTestSummary' declared 'static', move to anonymous namespace instead [misc-use-anonymous-namespace]
- C:\Git\JXCT\src\web\routes_reports.cpp:33:29:: variable 'lastTechDebt' declared 'static', move to anonymous namespace instead [misc-use-anonymous-namespace]
- C:\Git\JXCT\src\web\routes_reports.cpp:34:22:: variable 'lastReportUpdate' declared 'static', move to anonymous namespace instead [misc-use-anonymous-namespace]
- ... и ещё 13 проблем

### src/web/routes_service.cpp (7 предупреждений)
**Читаемость:** 3 проблем
- C:\Git\JXCT\src\web\routes_service.cpp:21:18:: redundant 'webServer' declaration [readability-redundant-declaration]
- C:\Git\JXCT\src\web\routes_service.cpp:22:17:: redundant 'currentWiFiMode' declaration [readability-redundant-declaration]
- C:\Git\JXCT\src\web\routes_service.cpp:29:15:: redundant 'sensorLastError' declaration [readability-redundant-declaration]

**Разное:** 4 проблем
- C:\Git\JXCT\src\web\routes_service.cpp:36:13:: function 'sendHealthJson' declared 'static', move to anonymous namespace instead [misc-use-anonymous-namespace]
- C:\Git\JXCT\src\web\routes_service.cpp:37:13:: function 'sendServiceStatusJson' declared 'static', move to anonymous namespace instead [misc-use-anonymous-namespace]
- C:\Git\JXCT\src\web\routes_service.cpp:40:8:: function 'formatUptime' can be made static or moved into an anonymous namespace to enforce internal linkage [misc-use-internal-linkage]
- ... и ещё 1 проблем

### src/web/web_templates.cpp (18 предупреждений)
**Разное:** 5 проблем
- C:\Git\JXCT\src\web\web_templates.cpp:9:8:: function 'generatePageHeader' can be made static or moved into an anonymous namespace to enforce internal linkage [misc-use-internal-linkage]
- C:\Git\JXCT\src\web\web_templates.cpp:25:8:: function 'generateBasePage' can be made static or moved into an anonymous namespace to enforce internal linkage [misc-use-internal-linkage]
- C:\Git\JXCT\src\web\web_templates.cpp:43:8:: function 'generateSuccessPage' can be made static or moved into an anonymous namespace to enforce internal linkage [misc-use-internal-linkage]
- ... и ещё 2 проблем

**Потенциальные баги:** 10 проблем
- C:\Git\JXCT\src\web\web_templates.cpp:9:27:: 2 adjacent parameters of 'generatePageHeader' of similar type ('const String &') are easily swapped by mistake [bugprone-easily-swappable-parameters]
- C:\Git\JXCT\src\web\web_templates.cpp:25:25:: 3 adjacent parameters of 'generateBasePage' of similar type ('const String &') are easily swapped by mistake [bugprone-easily-swappable-parameters]
- C:\Git\JXCT\src\web\web_templates.cpp:34:26:: 2 adjacent parameters of 'generateErrorPage' of similar type are easily swapped by mistake [bugprone-easily-swappable-parameters]
- ... и ещё 7 проблем

**Читаемость:** 3 проблем
- C:\Git\JXCT\src\web\web_templates.cpp:108:41:: parameter name 'id' is too short, expected at least 3 characters [readability-identifier-length]
- C:\Git\JXCT\src\web\web_templates.cpp:134:44:: parameter name 'id' is too short, expected at least 3 characters [readability-identifier-length]
- C:\Git\JXCT\src\web\web_templates.cpp:158:42:: parameter name 'id' is too short, expected at least 3 characters [readability-identifier-length]

## 🎯 РЕКОМЕНДАЦИИ ПО ПРИОРИТЕТАМ

### 🔴 КРИТИЧЕСКИЕ (исправить немедленно)
- Потенциальные баги - могут привести к неправильной работе
- Проблемы безопасности - могут создать уязвимости

### 🟡 СРЕДНИЕ (исправить в ближайшее время)
- Проблемы читаемости - затрудняют сопровождение кода
- Модернизация - улучшают качество кода

### 🟢 НИЗКИЕ (можно исправить позже)
- Разные мелкие проблемы
