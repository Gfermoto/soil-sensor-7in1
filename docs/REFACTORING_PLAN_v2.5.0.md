# ПЛАН РЕФАКТОРИНГА v2.5.0 - РАЗБИВКА setupWebServer()

**Дата:** 11 июня 2025  
**Цель:** Подготовка к OTA внедрению путем упрощения архитектуры  

---

## 🎯 ТЕКУЩЕЕ СОСТОЯНИЕ

### Проблемы с setupWebServer()
- **Размер:** 1000+ строк в одной функции
- **Сложность:** >15 маршрутов в одном блоке 
- **Дублирование:** HTML-генерация повторяется
- **Поддержка:** Сложно найти и изменить конкретный маршрут

### Критические места для OTA:
1. **Авторизация** - checkWebAuth() используется везде
2. **HTML генерация** - встроенные стили дублируются 
3. **JSON ответы** - схожие структуры без переиспользования

---

## 🚀 ПЛАН РАЗБИВКИ НА ЭТАПЫ

### **ЭТАП 1: ВЫДЕЛЕНИЕ ОБЩИХ ФУНКЦИЙ** ⏱️ 2 часа

#### 1.1 HTML Template функции
```cpp
// include/web_templates.h
String buildHTMLPage(const String& title, const String& content, bool needAuth = true);
String buildErrorPage(const String& error);
String buildSuccessPage(const String& message, const String& redirect = "/");
String buildRedirectPage(const String& url, int delay = 2);
```

#### 1.2 Общие HTTP утилиты  
```cpp
// include/web_utils.h
void sendJSONResponse(int code, const JsonDocument& doc);
void sendHTMLResponse(const String& html, int code = 200);
void sendErrorResponse(const String& error, int code = 400);
bool validateAuth();  // Обертка checkWebAuth()
```

#### 1.3 CSS централизация
```cpp
// Убрать встроенные стили везде, использовать только:
html += "<style>" + String(getUnifiedCSS()) + "</style>";
```

### **ЭТАП 2: ГРУППИРОВКА МАРШРУТОВ** ⏱️ 4 часа

#### 2.1 Аутентификация & Главная
```cpp
// src/web_auth_routes.cpp
void setupAuthRoutes() {
    webServer.on("/", HTTP_GET, handleRoot);
    webServer.on("/", HTTP_POST, handleAuthPost);  
}
```

#### 2.2 Конфигурация & Сохранение
```cpp  
// src/web_config_routes.cpp
void setupConfigRoutes() {
    webServer.on("/save", HTTP_POST, handleSave);
    webServer.on("/config_manager", HTTP_GET, handleConfigManager);
    webServer.on("/export_config", HTTP_GET, handleExportConfig);
    webServer.on("/import_config", HTTP_POST, handleImportConfig);
}
```

#### 2.3 Интервалы & Фильтры  
```cpp
// src/web_intervals_routes.cpp
void setupIntervalsRoutes() {
    webServer.on("/intervals", HTTP_GET, handleIntervals);
    webServer.on("/save_intervals", HTTP_POST, handleSaveIntervals);
    webServer.on("/reset_intervals", HTTP_GET, handleResetIntervals);
}
```

#### 2.4 Сервисные функции
```cpp
// src/web_service_routes.cpp  
void setupServiceRoutes() {
    webServer.on("/service", HTTP_GET, handleService);
    webServer.on("/reset", HTTP_POST, handleReset);
    webServer.on("/reboot", HTTP_POST, handleReboot);
    webServer.on("/ota", HTTP_POST, handleOTA);  // Подготовка к v2.5.0
}
```

#### 2.5 API маршруты
```cpp
// src/web_api_routes.cpp
void setupAPIRoutes() {
    webServer.on("/api/sensor", HTTP_GET, handleAPISensor);
    webServer.on("/api/status", HTTP_GET, handleAPIStatus);
    webServer.on("/sensor_json", HTTP_GET, handleSensorJSON);
    webServer.on("/status", HTTP_GET, handleStatus);
}
```

#### 2.6 Показания датчика
```cpp
// src/web_readings_routes.cpp
void setupReadingsRoutes() {
    webServer.on("/readings", HTTP_GET, handleReadings);
}
```

### **ЭТАП 3: НОВАЯ АРХИТЕКТУРА setupWebServer()** ⏱️ 1 час

```cpp
// src/wifi_manager.cpp - Упрощенная версия
void setupWebServer() {
    // Настройка общих middleware
    setupAuthRoutes();
    setupConfigRoutes(); 
    setupIntervalsRoutes();
    setupServiceRoutes();
    setupAPIRoutes();
    setupReadingsRoutes();
    
    // Запуск сервера
    webServer.begin();
    logSuccess("Веб-сервер запущен на порту %d", HTTP_PORT);
}
```

---

## 📁 НОВАЯ СТРУКТУРА ФАЙЛОВ

```
src/
├── wifi_manager.cpp          # Только WiFi логика  
├── web_server/               # Новая папка для веб-модулей
│   ├── web_templates.cpp     # HTML шаблоны
│   ├── web_utils.cpp         # HTTP утилиты  
│   ├── web_auth_routes.cpp   # Авторизация & главная
│   ├── web_config_routes.cpp # Конфигурация
│   ├── web_intervals_routes.cpp # Интервалы & фильтры
│   ├── web_service_routes.cpp   # Сервисные функции
│   ├── web_api_routes.cpp       # JSON API
│   └── web_readings_routes.cpp  # Показания датчика
include/
├── web_server/               # Заголовки
│   ├── web_templates.h
│   ├── web_utils.h  
│   └── web_routes.h          # Объявления всех setup*Routes()
```

---

## 🔧 ДЕТАЛИ РЕАЛИЗАЦИИ

### Общие HTML шаблоны
```cpp
String buildHTMLPage(const String& title, const String& content, bool needAuth) {
    if (needAuth && !validateAuth()) {
        sendAuthForm();
        return "";
    }
    
    String html = "<!DOCTYPE html><html><head><meta charset='UTF-8'>";
    html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
    html += "<title>" + title + " JXCT</title>";
    html += "<style>" + String(getUnifiedCSS()) + "</style>";
    html += "</head><body><div class='container'>";
    html += navHtml();
    html += content;
    html += "</div>" + String(getToastHTML()) + "</body></html>";
    return html;
}
```

### JSON API стандартизация
```cpp
void sendJSONResponse(int code, const JsonDocument& doc) {
    String json;
    serializeJson(doc, json);
    webServer.send(code, "application/json", json);
}

// Стандартные ответы
void sendErrorJSON(const String& error) {
    StaticJsonDocument<128> doc;
    doc["error"] = error;
    sendJSONResponse(400, doc);
}

void sendSuccessJSON(const String& message) {
    StaticJsonDocument<128> doc;  
    doc["success"] = message;
    sendJSONResponse(200, doc);
}
```

### Авторизация middleware
```cpp
bool validateAuth() {
    if (strlen(config.webPassword) == 0) return true;
    return checkWebAuth();
}

#define REQUIRE_AUTH() if (!validateAuth()) { sendAuthForm(); return; }
```

---

## ⚡ ПРЕИМУЩЕСТВА ПОСЛЕ РЕФАКТОРИНГА

### Разработка & Поддержка
- ✅ **Быстрый поиск** кода для конкретной страницы
- ✅ **Изолированное тестирование** каждого маршрута  
- ✅ **Модульность** - легко добавлять новые страницы
- ✅ **Переиспользование** - общие шаблоны и утилиты

### Производительность
- ✅ **Меньше Flash** - убрано дублирование CSS (~6KB экономии)
- ✅ **Читаемость** - каждый файл <200 строк
- ✅ **Быстрая компиляция** - изменения в одном маршруте не затрагивают другие

### Готовность к OTA
- ✅ **Простота добавления** OTA интерфейса в web_service_routes.cpp
- ✅ **Стабильность** - изолированный код маршрутов
- ✅ **Отладка** - легко найти проблемный компонент

---

## 📋 ЧЕКЛИСТ ВЫПОЛНЕНИЯ

### Этап 1: Общие функции ✅
- [ ] Создать web_server/ папки в src/ и include/
- [ ] Реализовать web_templates.cpp
- [ ] Реализовать web_utils.cpp  
- [ ] Заменить встроенные CSS на getUnifiedCSS()
- [ ] Протестировать HTML генерацию

### Этап 2: Разбивка маршрутов ✅
- [ ] Выделить web_auth_routes.cpp
- [ ] Выделить web_config_routes.cpp
- [ ] Выделить web_intervals_routes.cpp
- [ ] Выделить web_service_routes.cpp  
- [ ] Выделить web_api_routes.cpp
- [ ] Выделить web_readings_routes.cpp
- [ ] Протестировать каждый модуль отдельно

### Этап 3: Интеграция ✅
- [ ] Обновить setupWebServer()
- [ ] Добавить все #include
- [ ] Протестировать полную функциональность
- [ ] Проверить размер Flash/RAM
- [ ] Обновить документацию

---

## 🎯 ИТОГОВЫЙ РЕЗУЛЬТАТ

После рефакторинга:
- **setupWebServer()**: 50 строк вместо 1000+
- **Модули**: 6 файлов по 100-200 строк каждый
- **Flash экономия**: ~6KB от устранения CSS дублирования  
- **Читаемость**: ++++ (критично для OTA разработки)
- **Готовность к OTA**: 100% (легко добавить OTA routes)

**Время выполнения:** 7 часов (1 рабочий день)  
**Риск поломки:** Низкий (поэтапная замена)  
**Выгода:** Высокая (идеальная база для OTA)

---

**Составитель:** Автоматический анализ архитектуры  
**Статус:** К выполнению 