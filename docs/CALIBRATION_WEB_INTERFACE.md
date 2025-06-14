# Внедрение веб-интерфейса калибровки датчика JXCT 7-в-1

## 🎯 Концепция внедрения

Данный документ содержит технические рекомендации по внедрению страницы калибровки в существующий веб-интерфейс ESP32-проекта для обеспечения автоматической коррекции показаний датчика в зависимости от условий выращивания.

> ⚡ **Важно**: Документ фокусируется исключительно на **математической компенсации показаний**. Фильтрация, валидация и обработка аномальных значений не рассматриваются, предполагая использование существующих механизмов фильтрации в проекте.

## 🎛️ Настройки пользовательского интерфейса

### **1. Основные параметры среды (задает пользователь)**

```html
<div class='section'>
  <h2>🌿 Профиль выращивания</h2>
  
  <div class='form-group'>
    <label>Тип среды выращивания:</label>
    <input type='radio' name='environment' value='0' id='greenhouse'> 
    <label for='greenhouse'>🏠 Теплица</label>
    <input type='radio' name='environment' value='1' id='field'> 
    <label for='field'>🌾 Открытый грунт</label>
  </div>
  
  <div class='form-group'>
    <label for='soil_type'>Тип почвы:</label>
    <select name='soil_type' id='soil_type'>
      <option value='0'>🏖️ Песчаная (КОЕ < 5 мэкв/100г)</option>
      <option value='1'>🧱 Глинистая (КОЕ > 25 мэкв/100г)</option>
      <option value='2'>🌿 Торфяная (органика > 80%)</option>
    </select>
  </div>
  
  <div class='form-group'>
    <label for='latitude'>Географические координаты:</label>
    <input type='number' name='latitude' step='0.000001' min='-90' max='90' 
           value='55.7558' placeholder='Широта'>
    <input type='number' name='longitude' step='0.000001' min='-180' max='180' 
           value='37.6176' placeholder='Долгота'>
    <small>Для автоматического определения сезонов</small>
  </div>
  
  <div class='form-group'>
    <label for='timezone'>Часовой пояс:</label>
    <select name='timezone' id='timezone'>
      <option value='-12'>UTC-12 (Бейкер)</option>
      <option value='-11'>UTC-11 (Самоа)</option>
      <!-- ... другие пояса -->
      <option value='3' selected>UTC+3 (Москва)</option>
      <option value='7'>UTC+7 (Новосибирск)</option>
      <option value='12'>UTC+12 (Камчатка)</option>
    </select>
  </div>
  
  <div class='form-group'>
    <label for='weather_api'>Ключ OpenWeatherMap API:</label>
    <input type='text' name='weather_api' maxlength='32' 
           placeholder='Ваш API ключ для определения осадков'>
    <small>
      <a href='https://openweathermap.org/api' target='_blank'>Получить бесплатный ключ</a>
      (1000 запросов/день)
    </small>
  </div>
</div>

<div class='section'>
  <h2>⚙️ Режим калибровки</h2>
  
  <div class='form-group'>
    <label>Тип коррекции:</label>
    <input type='radio' name='calibration_mode' value='0' id='auto_cal' checked> 
    <label for='auto_cal'>🤖 Автоматическая (рекомендуется)</label>
    <input type='radio' name='calibration_mode' value='1' id='manual_cal'> 
    <label for='manual_cal'>✋ Ручная настройка</label>
  </div>
  
  <div id='manual_coefficients' style='display:none'>
    <h3>Пользовательские коэффициенты</h3>
    <div class='coeff-grid'>
      <label>EC температурная коррекция:</label>
      <input type='number' name='k_temp_ec' step='0.001' value='0.019'>
      
      <label>N температурная коррекция:</label>
      <input type='number' name='k_temp_n' step='0.0001' value='0.0025'>
      
      <label>P температурная коррекция:</label>
      <input type='number' name='k_temp_p' step='0.0001' value='0.003'>
      
      <label>K температурная коррекция:</label>
      <input type='number' name='k_temp_k' step='0.0001' value='0.0018'>
    </div>
  </div>
</div>
```

## 🤖 Автоматические данные системы

### **1. Получение времени и сезона (NTP)**

```cpp
// Функция определения сезона по координатам и дате
String getCurrentSeason(float latitude, int month) {
    bool northernHemisphere = (latitude >= 0);
    
    if (northernHemisphere) {
        if (month >= 3 && month <= 5) return "Весна";
        if (month >= 6 && month <= 8) return "Лето"; 
        if (month >= 9 && month <= 11) return "Осень";
        return "Зима";
    } else {
        // Южное полушарие - сезоны смещены на 6 месяцев
        if (month >= 3 && month <= 5) return "Осень";
        if (month >= 6 && month <= 8) return "Зима"; 
        if (month >= 9 && month <= 11) return "Весна";
        return "Лето";
    }
}

// API endpoint для получения текущих условий
void handleCalibrationStatus() {
    StaticJsonDocument<512> doc;
    
    time_t now = time(nullptr);
    struct tm *timeinfo = localtime(&now);
    
    int currentHour = timeinfo->tm_hour;
    int currentMonth = timeinfo->tm_mon + 1;
    
    doc["current_time"] = String(currentHour) + ":00";
    doc["current_month"] = currentMonth;
    doc["current_season"] = getCurrentSeason(config.calibration.latitude, currentMonth);
    
    // Рекомендации по времени замеров
    bool isOptimalTime = false;
    String recommendation = "";
    
    if (config.calibration.growingEnvironment == 0) { // Теплица
        isOptimalTime = (currentHour == 7); // 7:00 утра
        recommendation = isOptimalTime ? 
            "✅ Оптимальное время для замеров в теплице" :
            "⏰ Рекомендуется замер в 7:00 (стабильные T/θ)";
    } else { // Открытый грунт
        isOptimalTime = (currentHour >= 9 && currentHour <= 16);
        recommendation = isOptimalTime ? 
            "✅ Подходящее время для замеров" :
            "⏰ Рекомендуется замер с 9:00 до 16:00";
    }
    
    doc["optimal_time"] = isOptimalTime;
    doc["recommendation"] = recommendation;
    
    // Проверка погодных условий
    if (strlen(config.calibration.weatherApiKey) > 0) {
        bool recentRain = checkRecentRainfall();
        doc["recent_rainfall"] = recentRain;
        
        if (recentRain && config.calibration.growingEnvironment == 1) {
            doc["measurement_blocked"] = true;
            doc["rain_warning"] = "⚠️ Замеры заблокированы: дождь менее 12 часов назад";
        }
    }
    
    String response;
    serializeJson(doc, response);
    webServer.send(200, "application/json", response);
}
```

### **2. Интеграция с OpenWeatherMap API**

```cpp
#include <HTTPClient.h>
#include <ArduinoJson.h>

// Структура для хранения погодных данных
struct WeatherData {
    bool hasRain;
    float precipitation;  // мм осадков за последние 3 часа
    unsigned long lastUpdate;
    bool dataValid;
};

WeatherData weatherData = {false, 0.0, 0, false};

// Получение данных о погоде
bool updateWeatherData() {
    if (strlen(config.calibration.weatherApiKey) == 0) {
        return false;
    }
    
    HTTPClient http;
    String url = "http://api.openweathermap.org/data/2.5/weather?lat=" + 
                String(config.calibration.latitude, 6) + 
                "&lon=" + String(config.calibration.longitude, 6) + 
                "&appid=" + String(config.calibration.weatherApiKey) + 
                "&units=metric";
    
    http.begin(url);
    int httpCode = http.GET();
    
    if (httpCode == HTTP_CODE_OK) {
        String payload = http.getString();
        StaticJsonDocument<1024> doc;
        
        if (deserializeJson(doc, payload) == DeserializationError::Ok) {
            // Проверяем наличие дождя
            if (doc["weather"][0]["main"] == "Rain") {
                weatherData.hasRain = true;
                if (doc["rain"]["3h"].is<float>()) {
                    weatherData.precipitation = doc["rain"]["3h"];
                }
            } else {
                weatherData.hasRain = false;
                weatherData.precipitation = 0.0;
            }
            
            weatherData.lastUpdate = millis();
            weatherData.dataValid = true;
            
            logSuccess("Погодные данные обновлены: дождь=%s, осадки=%.1fмм", 
                      weatherData.hasRain ? "да" : "нет", 
                      weatherData.precipitation);
            return true;
        }
    }
    
    http.end();
    logError("Ошибка получения погодных данных: код %d", httpCode);
    return false;
}

// Проверка недавних осадков (для открытого грунта)
bool checkRecentRainfall() {
    // Обновляем данные каждые 30 минут
    if (millis() - weatherData.lastUpdate > 1800000) { // 30 мин
        updateWeatherData();
    }
    
    return weatherData.dataValid && weatherData.hasRain;
}
```

## 📊 Расширение структуры конфигурации

### **Добавление в `jxct_config_vars.h`:**

```cpp
struct __attribute__((packed)) CalibrationSettings {
    // Основные настройки (12 байт)
    uint8_t growingEnvironment;    // 0=теплица, 1=открытый грунт
    uint8_t soilType;             // 0=песок, 1=глина, 2=торф  
    uint8_t calibrationMode;      // 0=авто, 1=ручная
    uint8_t weatherEnabled;       // 0=отключено, 1=включено
    int8_t timezone;              // Часовой пояс (-12 до +12)
    uint8_t reserved[3];          // Резерв для выравнивания
    
    // Координаты (8 байт)
    float latitude;               // Широта (-90 до +90)
    float longitude;              // Долгота (-180 до +180)
    
    // API ключи (32 байта)
    char weatherApiKey[32];       // Ключ OpenWeatherMap API
    
    // Пользовательские коэффициенты (16 байт) - только для ручного режима
    float kTempEC;                // Температурная коррекция EC
    float kTempN;                 // Температурная коррекция N
    float kTempP;                 // Температурная коррекция P  
    float kTempK;                 // Температурная коррекция K
    
} calibration;

// Добавляем в основную структуру Config:
struct __attribute__((packed)) Config {
    // ... существующие поля ...
    
    // v2.4.0: Настройки калибровки (68 байт)
    CalibrationSettings calibration;
    
    // ... остальные поля ...
};
```

## 🔧 Интеграция в веб-сервер

### **1. Добавление в навигацию (`wifi_manager.cpp`):**

```cpp
String navHtml()
{
    String html = "<div class='nav'>";
    html += "<a href='/'>" UI_ICON_CONFIG " Настройки</a>";
    if (currentWiFiMode == WiFiMode::STA)
    {
        html += "<a href='/readings'>" UI_ICON_DATA " Показания</a>";
        html += "<a href='/calibration'>" UI_ICON_CALIBRATION " Калибровка</a>";  // НОВОЕ
        html += "<a href='/intervals'>" UI_ICON_INTERVALS " Интервалы</a>";
        html += "<a href='/config_manager'>" UI_ICON_FOLDER " Конфигурация</a>";
        html += "<a href='/service'>" UI_ICON_SERVICE " Сервис</a>";
    }
    html += "</div>";
    return html;
}
```

### **2. Обработчики страниц:**

```cpp
void setupWebServer() {
    // ... существующие обработчики ...
    
    // Страница калибровки
    webServer.on("/calibration", HTTP_GET, handleCalibration);
    webServer.on("/calibration", HTTP_POST, handleCalibrationSave);
    
    // API для статуса калибровки
    webServer.on("/api/calibration/status", HTTP_GET, handleCalibrationStatus);
    
    // API для тестирования коррекции
    webServer.on("/api/calibration/test", HTTP_POST, handleCalibrationTest);
    
    // ... остальные обработчики ...
}

// Главная страница калибровки
void handleCalibration() {
    if (!checkWebAuth()) return;
    
    String html = buildCalibrationPage();
    webServer.send(200, "text/html; charset=utf-8", html);
}

// Построение HTML страницы калибровки
String buildCalibrationPage() {
    String html = "<!DOCTYPE html><html><head>";
    html += "<meta charset='UTF-8'>";
    html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
    html += "<title>🔧 Калибровка датчика JXCT</title>";
    html += "<style>" + String(getUnifiedCSS()) + "</style>";
    html += "</head><body><div class='container'>";
    
    html += navHtml();
    html += "<h1>🔧 Калибровка датчика</h1>";
    
    // Текущий статус
    html += "<div class='section' id='current_status'>";
    html += "<h2>📊 Текущие условия</h2>";
    html += "<div class='status-loading'>Загрузка данных...</div>";
    html += "</div>";
    
    // Форма настроек
    html += "<form action='/calibration' method='post'>";
    html += buildCalibrationForm();
    html += "<button type='submit' class='btn-primary'>💾 Сохранить настройки</button>";
    html += "</form>";
    
    // JavaScript для динамического обновления
    html += "<script>";
    html += "function updateStatus() {";
    html += "  fetch('/api/calibration/status')";
    html += "    .then(r => r.json())";
    html += "    .then(data => {";
    html += "      const status = document.getElementById('current_status');";
    html += "      status.innerHTML = `";
    html += "        <h2>📊 Текущие условия</h2>";
    html += "        <div class='status-grid'>";
    html += "          <div>Сезон: <span class='badge'>${data.current_season}</span></div>";
    html += "          <div>Время: <span class='badge'>${data.current_time}</span></div>";
    html += "          <div>Статус: <span class='${data.optimal_time ? 'badge-success' : 'badge-warning'}'>";
    html += "            ${data.optimal_time ? 'Оптимально' : 'Не оптимально'}</span></div>";
    html += "        </div>";
    html += "        <div class='recommendation'>${data.recommendation}</div>";
    html += "        ${data.rain_warning ? `<div class='alert-warning'>${data.rain_warning}</div>` : ''}`;";
    html += "    });";
    html += "}";
    html += "updateStatus(); setInterval(updateStatus, 30000);"; // Обновление каждые 30 сек
    html += "</script>";
    
    html += "</div></body></html>";
    return html;
}
```

## 🔄 Логика применения коррекции

### **Основная функция коррекции:**

```cpp
#include "calibration_algorithms.h"

// Применение математической компенсации к показаниям датчика
SensorData applyMathematicalCompensation(SensorData rawData) {
    if (config.calibration.calibrationMode == 0) {
        // Автоматическая компенсация по профилю среды
        return applyAutomaticCompensation(rawData);
    } else {
        // Ручная компенсация с пользовательскими коэффициентами
        return applyManualCompensation(rawData);
    }
}

// Автоматическая компенсация в зависимости от профиля
SensorData applyAutomaticCompensation(SensorData raw) {
    SensorData compensated = raw;
    
    // Получаем текущие условия
    time_t now = time(nullptr);
    struct tm *timeinfo = localtime(&now);
    int month = timeinfo->tm_mon + 1;
    
    // Выбираем алгоритм компенсации
    if (config.calibration.growingEnvironment == 0) {
        // Теплица - используем профиль А
        compensated = applyGreenhouseCompensation(raw, month);
    } else {
        // Открытый грунт - используем профиль Б
        compensated = applyFieldCompensation(raw, month, config.calibration.soilType);
    }
    
    return compensated;
}

// Коррекция для теплиц (из документации SENSOR_COMPENSATION.md)
SensorData applyGreenhouseCorrection(SensorData raw, int month) {
    SensorData corrected = raw;
    
    // Коэффициенты для теплиц
    const float K_TEMP_EC = 0.021;
    const float K_TEMP_N = 0.0028;
    const float K_TEMP_P = 0.0032;
    const float K_TEMP_K = 0.002;
    const float K_HUMIDITY_PH = 0.025;
    const float K_EC_N = 0.00025;
    const float K_EC_P = 0.00025;
    const float K_EC_K = 0.00012;
    
    // 1. Температурное ограничение
    float T_limited = min(raw.temperature, 45.0f);
    
    // 2. Используем текущую влажность
    float theta_current = raw.humidity;
    
    // 3. Коррекция EC
    float EC_temp = raw.ec / (1 + K_TEMP_EC * (T_limited - 25));
    corrected.ec = (theta_current > 20) ? EC_temp * (45.0f / theta_current) : EC_temp;
    
    // 4. Коррекция pH
    float pH_temp = raw.ph - 0.003f * (T_limited - 25);
    corrected.ph = pH_temp + K_HUMIDITY_PH * (50 - theta_current);
    
    // 5. Коррекция NPK
    float N_temp = raw.nitrogen * (1 - K_TEMP_N * (T_limited - 25));
    float N_theta = N_temp / (1 + 0.018f * (theta_current - 45));
    float N_ec = N_theta * (1 - K_EC_N * corrected.ec);
    corrected.nitrogen = N_ec * (corrected.ph < 6.0f ? 1.1f : 1.0f);
    
    // Аналогично для P и K...
    
    // 6. Дополнительная коррекция засоления
    if (corrected.ec > 3000) {
        corrected.potassium *= 0.85f;
    }
    
    return corrected;
}
```

## 📱 CSS стили для страницы

```css
/* Дополнительные стили для страницы калибровки */
.status-grid {
    display: grid;
    grid-template-columns: repeat(auto-fit, minmax(200px, 1fr));
    gap: 15px;
    margin: 15px 0;
}

.status-grid > div {
    background: #f8f9fa;
    padding: 10px;
    border-radius: 5px;
    border-left: 4px solid #007bff;
}

.badge {
    background: #007bff;
    color: white;
    padding: 2px 8px;
    border-radius: 12px;
    font-size: 12px;
    font-weight: bold;
}

.badge-success {
    background: #28a745;
    color: white;
    padding: 2px 8px;
    border-radius: 12px;
    font-size: 12px;
}

.badge-warning {
    background: #ffc107;
    color: #212529;
    padding: 2px 8px;
    border-radius: 12px;
    font-size: 12px;
}

.recommendation {
    background: #e3f2fd;
    border: 1px solid #2196f3;
    border-radius: 5px;
    padding: 10px;
    margin: 10px 0;
    font-weight: 500;
}

.alert-warning {
    background: #fff3cd;
    border: 1px solid #ffeaa7;
    color: #856404;
    padding: 10px;
    border-radius: 5px;
    margin: 10px 0;
}

.coeff-grid {
    display: grid;
    grid-template-columns: 1fr auto;
    gap: 10px;
    align-items: center;
}

.btn-primary {
    background: #007bff;
    color: white;
    border: none;
    padding: 12px 24px;
    border-radius: 5px;
    cursor: pointer;
    font-size: 16px;
    font-weight: 500;
}

.btn-primary:hover {
    background: #0056b3;
}
```

## 🚀 План внедрения

### **Этап 1: Подготовка (1-2 дня)**
1. Добавить поля калибровки в структуру `Config`
2. Обновить функции `loadConfig()` / `saveConfig()`
3. Добавить иконку калибровки в UI константы

### **Этап 2: Базовый интерфейс (2-3 дня)**
1. Создать страницу `/calibration` с формой настроек
2. Добавить API endpoint `/api/calibration/status`
3. Интегрировать в навигационное меню

### **Этап 3: Логика коррекции (3-4 дня)**
1. Реализовать функции автоматической коррекции
2. Добавить поддержку ручных коэффициентов
3. Интегрировать с основным циклом чтения датчика

### **Этап 4: Погодная интеграция (2-3 дня)**
1. Добавить HTTP-клиент для OpenWeatherMap
2. Реализовать кэширование погодных данных
3. Добавить логику блокировки замеров после дождя

### **Этап 5: Тестирование (1-2 дня)**
1. Проверить алгоритмы математической коррекции
2. Протестировать сохранение/загрузку настроек
3. Валидация точности компенсации с эталонными данными

**Общее время внедрения: 8-12 дней разработки**

## 📚 Дополнительные ресурсы

- [OpenWeatherMap API Documentation](https://openweathermap.org/api)
- [ESP32 HTTP Client Library](https://github.com/espressif/arduino-esp32/tree/master/libraries/HTTPClient)
- [ArduinoJson Library](https://arduinojson.org/)
- [NTP Time Synchronization](https://github.com/arduino-libraries/NTPClient)

## ⚠️ Важные примечания

1. **Память ESP32**: Добавление калибровочных настроек увеличит размер конфигурации на ~68 байт
2. **HTTP запросы**: Погодные данные увеличат использование WiFi, рекомендуется кэширование на 30 минут
3. **Точность времени**: Критически важна синхронизация NTP для корректного определения сезонов
4. **Чистая компенсация**: Документ фокусируется на математической коррекции показаний без дублирования существующей фильтрации

---

*Документ создан для технической реализации веб-интерфейса калибровки датчика JXCT 7-в-1* 