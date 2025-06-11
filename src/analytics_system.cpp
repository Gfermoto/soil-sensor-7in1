#include "analytics_system.h"
#include "debug.h"
#include "logger.h"
#include <math.h>
#include <WebServer.h>  // Для веб-интерфейса аналитики

// Глобальная система аналитики
AnalyticsSystem analytics;

void initAnalyticsSystem()
{
    // Инициализация circular buffer
    analytics.head_index = 0;
    analytics.count = 0;
    analytics.buffer_full = false;
    analytics.last_stats_update = 0;
    
    // Очистка буфера
    for (int i = 0; i < ANALYTICS_BUFFER_SIZE; i++) {
        analytics.buffer[i].valid = false;
        analytics.buffer[i].timestamp = 0;
    }
    
    // Инициализация статистики
    analytics.stats_hour.valid = false;
    analytics.stats_day.valid = false;
    analytics.stats_week.valid = false;
    
    logSuccess("Analytics System v2.4.0 инициализирована");
    logDebug("Circular Buffer: %d слотов, размер структуры: %d байт", 
             ANALYTICS_BUFFER_SIZE, sizeof(AnalyticsDataPoint));
}

void addDataPointToAnalytics(const SensorData& data)
{
    if (!data.valid) {
        DEBUG_PRINTLN("[ANALYTICS] Пропускаем невалидные данные");
        return;
    }
    
    // Добавляем точку данных в circular buffer
    AnalyticsDataPoint& point = analytics.buffer[analytics.head_index];
    
    point.temperature = data.temperature;
    point.humidity = data.humidity;
    point.ec = data.ec;
    point.ph = data.ph;
    point.nitrogen = data.nitrogen;
    point.phosphorus = data.phosphorus;
    point.potassium = data.potassium;
    point.timestamp = millis();
    point.valid = true;
    
    // Обновляем индексы
    analytics.head_index = (analytics.head_index + 1) % ANALYTICS_BUFFER_SIZE;
    
    if (analytics.count < ANALYTICS_BUFFER_SIZE) {
        analytics.count++;
    } else {
        analytics.buffer_full = true;
    }
    
    DEBUG_PRINTF("[ANALYTICS] Добавлена точка %d/%d, Temp=%.1f°C\n", 
                 analytics.count, ANALYTICS_BUFFER_SIZE, data.temperature);
    
    // Обновляем статистику каждые 30 секунд
    unsigned long current_time = millis();
    if (current_time - analytics.last_stats_update > 30000) {
        calculateAnalyticsStats();
        analytics.last_stats_update = current_time;
    }
}

void calculateAnalyticsStats()
{
    if (analytics.count == 0) {
        DEBUG_PRINTLN("[ANALYTICS] Нет данных для расчета статистики");
        return;
    }
    
    unsigned long current_time = millis();
    
    // Расчет статистики за час
    analytics.stats_hour = getStatsForPeriod(ANALYTICS_STATS_WINDOW_HOUR);
    
    // Расчет статистики за день
    analytics.stats_day = getStatsForPeriod(ANALYTICS_STATS_WINDOW_DAY);
    
    // Расчет статистики за неделю
    analytics.stats_week = getStatsForPeriod(ANALYTICS_STATS_WINDOW_WEEK);
    
    DEBUG_PRINTF("[ANALYTICS] Статистика обновлена: час=%d, день=%d, неделя=%d точек\n",
                 analytics.stats_hour.sample_count,
                 analytics.stats_day.sample_count,
                 analytics.stats_week.sample_count);
}

AnalyticsStats getStatsForPeriod(unsigned long period_ms)
{
    AnalyticsStats stats;
    stats.valid = false;
    stats.sample_count = 0;
    
    if (analytics.count == 0) {
        return stats;
    }
    
    unsigned long current_time = millis();
    unsigned long cutoff_time = current_time - period_ms;
    
    // Инициализация min/max значений
    bool first_valid = true;
    
    // Проходим по circular buffer
    uint8_t samples_to_check = analytics.buffer_full ? ANALYTICS_BUFFER_SIZE : analytics.count;
    
    float temp_sum = 0, humidity_sum = 0, ec_sum = 0, ph_sum = 0, npk_sum = 0;
    
    for (uint8_t i = 0; i < samples_to_check; i++) {
        uint8_t index = (analytics.head_index - 1 - i + ANALYTICS_BUFFER_SIZE) % ANALYTICS_BUFFER_SIZE;
        const AnalyticsDataPoint& point = analytics.buffer[index];
        
        if (!point.valid || point.timestamp < cutoff_time) {
            continue;
        }
        
        if (first_valid) {
            // Инициализация первыми валидными данными
            stats.temp_min = stats.temp_max = point.temperature;
            stats.humidity_min = stats.humidity_max = point.humidity;
            stats.ec_min = stats.ec_max = point.ec;
            stats.ph_min = stats.ph_max = point.ph;
            stats.npk_min = stats.npk_max = (point.nitrogen + point.phosphorus + point.potassium) / 3.0;
            
            stats.period_start = point.timestamp;
            first_valid = false;
        }
        
        // Обновление min/max/sum
        stats.temp_min = fmin(stats.temp_min, point.temperature);
        stats.temp_max = fmax(stats.temp_max, point.temperature);
        temp_sum += point.temperature;
        
        stats.humidity_min = fmin(stats.humidity_min, point.humidity);
        stats.humidity_max = fmax(stats.humidity_max, point.humidity);
        humidity_sum += point.humidity;
        
        stats.ec_min = fmin(stats.ec_min, point.ec);
        stats.ec_max = fmax(stats.ec_max, point.ec);
        ec_sum += point.ec;
        
        stats.ph_min = fmin(stats.ph_min, point.ph);
        stats.ph_max = fmax(stats.ph_max, point.ph);
        ph_sum += point.ph;
        
        float npk_avg = (point.nitrogen + point.phosphorus + point.potassium) / 3.0;
        stats.npk_min = fmin(stats.npk_min, npk_avg);
        stats.npk_max = fmax(stats.npk_max, npk_avg);
        npk_sum += npk_avg;
        
        stats.sample_count++;
        stats.period_end = point.timestamp;
    }
    
    // Расчет средних значений
    if (stats.sample_count > 0) {
        stats.temp_avg = temp_sum / stats.sample_count;
        stats.humidity_avg = humidity_sum / stats.sample_count;
        stats.ec_avg = ec_sum / stats.sample_count;
        stats.ph_avg = ph_sum / stats.sample_count;
        stats.npk_avg = npk_sum / stats.sample_count;
        stats.valid = true;
    }
    
    return stats;
}

String exportAnalyticsToJSON(unsigned long period_ms)
{
    String json = "{\"analytics_export\":{";
    json += "\"timestamp\":" + String(millis()) + ",";
    json += "\"version\":\"2.4.0\",";
    json += "\"buffer_usage\":" + String(getAnalyticsBufferUsage()) + ",";
    
    // Общая информация
    json += "\"info\":{";
    json += "\"total_points\":" + String(analytics.count) + ",";
    json += "\"buffer_full\":" + String(analytics.buffer_full ? "true" : "false") + ",";
    json += "\"oldest_timestamp\":" + String(getAnalyticsOldestTimestamp()) + ",";
    json += "\"newest_timestamp\":" + String(getAnalyticsNewestTimestamp());
    json += "},";
    
    // Статистика по периодам
    json += "\"statistics\":{";
    
    // Час
    json += "\"hour\":{";
    if (analytics.stats_hour.valid) {
        json += "\"temperature\":{\"min\":" + String(analytics.stats_hour.temp_min, 1) + 
                ",\"max\":" + String(analytics.stats_hour.temp_max, 1) + 
                ",\"avg\":" + String(analytics.stats_hour.temp_avg, 1) + "},";
        json += "\"humidity\":{\"min\":" + String(analytics.stats_hour.humidity_min, 1) + 
                ",\"max\":" + String(analytics.stats_hour.humidity_max, 1) + 
                ",\"avg\":" + String(analytics.stats_hour.humidity_avg, 1) + "},";
        json += "\"ec\":{\"min\":" + String((int)analytics.stats_hour.ec_min) + 
                ",\"max\":" + String((int)analytics.stats_hour.ec_max) + 
                ",\"avg\":" + String((int)analytics.stats_hour.ec_avg) + "},";
        json += "\"ph\":{\"min\":" + String(analytics.stats_hour.ph_min, 2) + 
                ",\"max\":" + String(analytics.stats_hour.ph_max, 2) + 
                ",\"avg\":" + String(analytics.stats_hour.ph_avg, 2) + "},";
        json += "\"samples\":" + String(analytics.stats_hour.sample_count);
    } else {
        json += "\"error\":\"no_data\"";
    }
    json += "},";
    
    // День
    json += "\"day\":{";
    if (analytics.stats_day.valid) {
        json += "\"temperature\":{\"min\":" + String(analytics.stats_day.temp_min, 1) + 
                ",\"max\":" + String(analytics.stats_day.temp_max, 1) + 
                ",\"avg\":" + String(analytics.stats_day.temp_avg, 1) + "},";
        json += "\"samples\":" + String(analytics.stats_day.sample_count);
    } else {
        json += "\"error\":\"no_data\"";
    }
    json += "},";
    
    // Неделя
    json += "\"week\":{";
    if (analytics.stats_week.valid) {
        json += "\"temperature\":{\"min\":" + String(analytics.stats_week.temp_min, 1) + 
                ",\"max\":" + String(analytics.stats_week.temp_max, 1) + 
                ",\"avg\":" + String(analytics.stats_week.temp_avg, 1) + "},";
        json += "\"samples\":" + String(analytics.stats_week.sample_count);
    } else {
        json += "\"error\":\"no_data\"";
    }
    json += "}";
    
    json += "}"; // statistics
    
    // Сырые данные за указанный период (если запрошено)
    if (period_ms > 0) {
        json += ",\"raw_data\":[";
        
        unsigned long current_time = millis();
        unsigned long cutoff_time = current_time - period_ms;
        bool first_point = true;
        
        uint8_t samples_to_check = analytics.buffer_full ? ANALYTICS_BUFFER_SIZE : analytics.count;
        
        for (uint8_t i = 0; i < samples_to_check; i++) {
            uint8_t index = (analytics.head_index - 1 - i + ANALYTICS_BUFFER_SIZE) % ANALYTICS_BUFFER_SIZE;
            const AnalyticsDataPoint& point = analytics.buffer[index];
            
            if (!point.valid || point.timestamp < cutoff_time) {
                continue;
            }
            
            if (!first_point) json += ",";
            
            json += "{";
            json += "\"timestamp\":" + String(point.timestamp) + ",";
            json += "\"temperature\":" + String(point.temperature, 1) + ",";
            json += "\"humidity\":" + String(point.humidity, 1) + ",";
            json += "\"ec\":" + String((int)point.ec) + ",";
            json += "\"ph\":" + String(point.ph, 2) + ",";
            json += "\"nitrogen\":" + String((int)point.nitrogen) + ",";
            json += "\"phosphorus\":" + String((int)point.phosphorus) + ",";
            json += "\"potassium\":" + String((int)point.potassium);
            json += "}";
            
            first_point = false;
        }
        
        json += "]"; // raw_data
    }
    
    json += "}}"; // analytics_export
    
    return json;
}

String exportAnalyticsToCSV(unsigned long period_ms)
{
    String csv = "timestamp,temperature,humidity,ec,ph,nitrogen,phosphorus,potassium\n";
    
    unsigned long current_time = millis();
    unsigned long cutoff_time = period_ms > 0 ? (current_time - period_ms) : 0;
    
    uint8_t samples_to_check = analytics.buffer_full ? ANALYTICS_BUFFER_SIZE : analytics.count;
    
    for (uint8_t i = 0; i < samples_to_check; i++) {
        uint8_t index = (analytics.head_index - 1 - i + ANALYTICS_BUFFER_SIZE) % ANALYTICS_BUFFER_SIZE;
        const AnalyticsDataPoint& point = analytics.buffer[index];
        
        if (!point.valid || (period_ms > 0 && point.timestamp < cutoff_time)) {
            continue;
        }
        
        csv += String(point.timestamp) + ",";
        csv += String(point.temperature, 1) + ",";
        csv += String(point.humidity, 1) + ",";
        csv += String((int)point.ec) + ",";
        csv += String(point.ph, 2) + ",";
        csv += String((int)point.nitrogen) + ",";
        csv += String((int)point.phosphorus) + ",";
        csv += String((int)point.potassium) + "\n";
    }
    
    return csv;
}

uint8_t getAnalyticsBufferUsage()
{
    return (analytics.count * 100) / ANALYTICS_BUFFER_SIZE;
}

unsigned long getAnalyticsOldestTimestamp()
{
    if (analytics.count == 0) return 0;
    
    if (analytics.buffer_full) {
        // Самая старая запись - следующая после головы
        uint8_t oldest_index = analytics.head_index;
        return analytics.buffer[oldest_index].timestamp;
    } else {
        // Самая старая запись - в начале заполненной части
        return analytics.buffer[0].timestamp;
    }
}

unsigned long getAnalyticsNewestTimestamp()
{
    if (analytics.count == 0) return 0;
    
    // Самая новая запись - предыдущая перед головой
    uint8_t newest_index = (analytics.head_index - 1 + ANALYTICS_BUFFER_SIZE) % ANALYTICS_BUFFER_SIZE;
    return analytics.buffer[newest_index].timestamp;
}

void clearAnalyticsBuffer()
{
    analytics.head_index = 0;
    analytics.count = 0;
    analytics.buffer_full = false;
    
    for (int i = 0; i < ANALYTICS_BUFFER_SIZE; i++) {
        analytics.buffer[i].valid = false;
    }
    
    analytics.stats_hour.valid = false;
    analytics.stats_day.valid = false;
    analytics.stats_week.valid = false;
    
    logSuccess("Analytics buffer очищен");
}

// Веб-интерфейс аналитики
void handleAnalyticsPage()
{
    extern WebServer webServer;  // Объявляем внешний webServer
    
    String html = "<!DOCTYPE html><html><head><meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1.0'>";
    html += "<title>📊 Аналитика JXCT v2.4.0</title>";
    html += "<style>";
    html += "body{font-family:Arial,sans-serif;margin:0;padding:20px;background:#f5f5f5}";
    html += ".container{max-width:1200px;margin:0 auto;background:white;padding:20px;border-radius:8px;box-shadow:0 2px 10px rgba(0,0,0,0.1)}";
    html += ".nav{margin-bottom:20px}.nav a{margin-right:15px;text-decoration:none;color:#4CAF50;font-weight:bold}";
    html += "h1{color:#333;border-bottom:2px solid #4CAF50;padding-bottom:10px}";
    html += ".stats-grid{display:grid;grid-template-columns:repeat(auto-fit,minmax(300px,1fr));gap:20px;margin:20px 0}";
    html += ".stat-card{background:#f9f9f9;padding:15px;border-radius:5px;border-left:4px solid #4CAF50}";
    html += ".stat-title{font-weight:bold;color:#333;margin-bottom:10px}";
    html += ".stat-value{font-size:1.2em;color:#4CAF50;margin:5px 0}";
    html += ".info-block{background:#e3f2fd;padding:15px;border-radius:5px;margin:15px 0;border-left:4px solid #2196F3}";
    html += ".warning{background:#fff3e0;border-left-color:#ff9800}";
    html += ".export-section{background:#f1f8e9;padding:15px;border-radius:5px;margin:20px 0}";
    html += ".btn{background:#4CAF50;color:white;padding:10px 15px;border:none;border-radius:3px;cursor:pointer;text-decoration:none;margin:5px}";
    html += ".btn:hover{background:#45a049}.btn-secondary{background:#2196F3}.btn-secondary:hover{background:#1976D2}";
    html += ".chart-container{margin:20px 0;padding:15px;background:#fafafa;border-radius:5px}";
    html += "@media(max-width:768px){.stats-grid{grid-template-columns:1fr}.container{padding:10px;margin:5px}}";
    html += "</style>";
    html += "<script src='https://cdn.jsdelivr.net/npm/chart.js@4.4.0/dist/chart.min.js'></script>";
    html += "</head><body><div class='container'>";
    
    // Навигация
    html += "<div class='nav'>";
    html += "<a href='/'>⚙️ Главная</a>";
    html += "<a href='/readings'>📊 Показания</a>";
    html += "<a href='/intervals'>⏱️ Интервалы</a>";
    html += "<a href='/analytics'>📈 Аналитика</a>";
    html += "<a href='/config_manager'>📁 Конфигурация</a>";
    html += "<a href='/service'>🔧 Сервис</a>";
    html += "</div>";
    
    html += "<h1>📊 Аналитика JXCT v2.4.0</h1>";
    
    // Информация о буфере
    html += "<div class='info-block'>";
    html += "<strong>🗄️ Состояние буфера данных:</strong><br>";
    html += "Заполнено: " + String(analytics.count) + "/" + String(ANALYTICS_BUFFER_SIZE) + " точек (" + String(getAnalyticsBufferUsage()) + "%)<br>";
    html += "Статус: " + String(analytics.buffer_full ? "Полный" : "Заполняется") + "<br>";
    if (analytics.count > 0) {
        html += "Период данных: " + String((getAnalyticsNewestTimestamp() - getAnalyticsOldestTimestamp()) / 60000) + " мин";
    } else {
        html += "Нет данных для анализа";
    }
    html += "</div>";
    
    if (analytics.count == 0) {
        html += "<div class='info-block warning'>";
        html += "⚠️ Данные для аналитики отсутствуют. Подождите несколько минут для накопления измерений.";
        html += "</div>";
    } else {
        // Статистика по периодам
        html += "<div class='stats-grid'>";
        
        // Час
        if (analytics.stats_hour.valid) {
            html += "<div class='stat-card'>";
            html += "<div class='stat-title'>🕐 Последний час (" + String(analytics.stats_hour.sample_count) + " измерений)</div>";
            html += "<div class='stat-value'>Температура: " + String(analytics.stats_hour.temp_min, 1) + "°C ... " + String(analytics.stats_hour.temp_max, 1) + "°C</div>";
            html += "<div class='stat-value'>Среднее: " + String(analytics.stats_hour.temp_avg, 1) + "°C</div>";
            html += "<div class='stat-value'>Влажность: " + String(analytics.stats_hour.humidity_min, 1) + "% ... " + String(analytics.stats_hour.humidity_max, 1) + "%</div>";
            html += "<div class='stat-value'>pH: " + String(analytics.stats_hour.ph_min, 2) + " ... " + String(analytics.stats_hour.ph_max, 2) + "</div>";
            html += "</div>";
        }
        
        // День
        if (analytics.stats_day.valid) {
            html += "<div class='stat-card'>";
            html += "<div class='stat-title'>📅 Последний день (" + String(analytics.stats_day.sample_count) + " измерений)</div>";
            html += "<div class='stat-value'>Температура: " + String(analytics.stats_day.temp_min, 1) + "°C ... " + String(analytics.stats_day.temp_max, 1) + "°C</div>";
            html += "<div class='stat-value'>Среднее: " + String(analytics.stats_day.temp_avg, 1) + "°C</div>";
            html += "<div class='stat-value'>Влажность: " + String(analytics.stats_day.humidity_min, 1) + "% ... " + String(analytics.stats_day.humidity_max, 1) + "%</div>";
            html += "<div class='stat-value'>pH: " + String(analytics.stats_day.ph_min, 2) + " ... " + String(analytics.stats_day.ph_max, 2) + "</div>";
            html += "</div>";
        }
        
        // Неделя
        if (analytics.stats_week.valid) {
            html += "<div class='stat-card'>";
            html += "<div class='stat-title'>📊 Последняя неделя (" + String(analytics.stats_week.sample_count) + " измерений)</div>";
            html += "<div class='stat-value'>Температура: " + String(analytics.stats_week.temp_min, 1) + "°C ... " + String(analytics.stats_week.temp_max, 1) + "°C</div>";
            html += "<div class='stat-value'>Среднее: " + String(analytics.stats_week.temp_avg, 1) + "°C</div>";
            html += "<div class='stat-value'>Влажность: " + String(analytics.stats_week.humidity_min, 1) + "% ... " + String(analytics.stats_week.humidity_max, 1) + "%</div>";
            html += "<div class='stat-value'>pH: " + String(analytics.stats_week.ph_min, 2) + " ... " + String(analytics.stats_week.ph_max, 2) + "</div>";
            html += "</div>";
        }
        
        html += "</div>";
        
        // График температуры
        html += "<div class='chart-container'>";
        html += "<h3>📈 График температуры (последние 20 точек)</h3>";
        html += "<canvas id='tempChart' width='400' height='200'></canvas>";
        html += "</div>";
    }
    
    // Экспорт данных
    html += "<div class='export-section'>";
    html += "<h3>💾 Экспорт данных</h3>";
    html += "<a href='/api/analytics/export?format=json&period=hour' class='btn'>📄 JSON (час)</a>";
    html += "<a href='/api/analytics/export?format=csv&period=hour' class='btn'>📊 CSV (час)</a>";
    html += "<a href='/api/analytics/export?format=json&period=day' class='btn btn-secondary'>📄 JSON (день)</a>";
    html += "<a href='/api/analytics/export?format=csv&period=day' class='btn btn-secondary'>📊 CSV (день)</a>";
    html += "</div>";
    
    // JavaScript для графиков
    if (analytics.count > 0) {
        html += "<script>";
        html += "async function loadChart() {";
        html += "  try {";
        html += "    const response = await fetch('/api/analytics?period=hour');";
        html += "    const data = await response.json();";
        html += "    if (data.raw_data && data.raw_data.length > 0) {";
        html += "      const ctx = document.getElementById('tempChart').getContext('2d');";
        html += "      const chartData = data.raw_data.slice(-20);";  // Последние 20 точек
        html += "      new Chart(ctx, {";
        html += "        type: 'line',";
        html += "        data: {";
        html += "          labels: chartData.map(p => new Date(p.timestamp).toLocaleTimeString()),";
        html += "          datasets: [{";
        html += "            label: 'Температура (°C)',";
        html += "            data: chartData.map(p => p.temperature),";
        html += "            borderColor: '#4CAF50',";
        html += "            backgroundColor: 'rgba(76, 175, 80, 0.1)',";
        html += "            tension: 0.1";
        html += "          }]";
        html += "        },";
        html += "        options: {";
        html += "          responsive: true,";
        html += "          scales: {";
        html += "            y: { beginAtZero: false }";
        html += "          }";
        html += "        }";
        html += "      });";
        html += "    }";
        html += "  } catch (error) {";
        html += "    console.error('Ошибка загрузки данных:', error);";
        html += "  }";
        html += "}";
        html += "window.onload = loadChart;";
        html += "</script>";
    }
    
    html += "</div></body></html>";
    
    webServer.send(200, "text/html; charset=utf-8", html);
}

void handleAnalyticsAPI()
{
    extern WebServer webServer;
    
    String period = webServer.arg("period");
    unsigned long period_ms = 0;
    
    if (period == "hour") {
        period_ms = ANALYTICS_STATS_WINDOW_HOUR;
    } else if (period == "day") {
        period_ms = ANALYTICS_STATS_WINDOW_DAY;
    } else if (period == "week") {
        period_ms = ANALYTICS_STATS_WINDOW_WEEK;
    } else {
        period_ms = ANALYTICS_STATS_WINDOW_HOUR;  // По умолчанию час
    }
    
    String json = exportAnalyticsToJSON(period_ms);
    webServer.send(200, "application/json", json);
}

void handleAnalyticsExport()
{
    extern WebServer webServer;
    
    String format = webServer.arg("format");
    String period = webServer.arg("period");
    
    unsigned long period_ms = 0;
    if (period == "hour") {
        period_ms = ANALYTICS_STATS_WINDOW_HOUR;
    } else if (period == "day") {
        period_ms = ANALYTICS_STATS_WINDOW_DAY;
    } else if (period == "week") {
        period_ms = ANALYTICS_STATS_WINDOW_WEEK;
    }
    
    String filename = "jxct_analytics_" + period + "_" + String(millis());
    
    if (format == "csv") {
        webServer.sendHeader("Content-Disposition", "attachment; filename=" + filename + ".csv");
        webServer.send(200, "text/csv", exportAnalyticsToCSV(period_ms));
    } else {
        // JSON по умолчанию
        webServer.sendHeader("Content-Disposition", "attachment; filename=" + filename + ".json");
        webServer.send(200, "application/json", exportAnalyticsToJSON(period_ms));
    }
} 