#include "analytics_system.h"
#include "debug.h"
#include "logger.h"
#include "jxct_config_vars.h"  // v2.4.1: Для доступа к Config
#include <math.h>
#include <WebServer.h>  // Для веб-интерфейса аналитики

// Внешние объявления
extern String navHtml();

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
    
    logSuccess("Analytics System инициализирована");
    logDebug("Circular Buffer: %d слотов, размер структуры: %d байт", 
             ANALYTICS_BUFFER_SIZE, sizeof(AnalyticsDataPoint));
}

void addDataPointToAnalytics(const SensorData& data)
{
    if (!data.valid) {
        DEBUG_PRINTLN("[ANALYTICS] Пропускаем невалидные данные");
        return;
    }
    
    // v2.4.1: Создаем временную точку для проверки фильтром выбросов
    AnalyticsDataPoint temp_point;
    temp_point.temperature = data.temperature;
    temp_point.humidity = data.humidity;
    temp_point.ec = data.ec;
    temp_point.ph = data.ph;
    temp_point.nitrogen = data.nitrogen;
    temp_point.phosphorus = data.phosphorus;
    temp_point.potassium = data.potassium;
    temp_point.timestamp = millis();
    temp_point.valid = true;
    
    // v2.4.1: Применяем фильтр выбросов
    if (!applyOutlierFilter(&temp_point)) {
        DEBUG_PRINTLN("[ANALYTICS] Точка отклонена фильтром выбросов");
        return;  // Не добавляем выброс в буфер
    }
    
    // Добавляем проверенную точку в circular buffer
    AnalyticsDataPoint& point = analytics.buffer[analytics.head_index];
    point = temp_point;  // Копируем проверенную точку
    
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
    json += "\"version\":\"2.4.1\",";
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
    html += "<title>📊 Аналитика JXCT</title>";
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
    html += "</head><body><div class='container'>";
    
    // Навигация
    html += navHtml();
    
    html += "<h1>📊 Аналитика</h1>";
    
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
    }
    
    // Простые графики показаний
    html += "<div class='charts-section'>";
    html += "<h3>📈 Графики показаний (последние 10 точек)</h3>";
    
    // Статус данных
    html += "<div class='chart-status'>";
    html += "Данных в буфере: <span id='dataCount'>" + String(analytics.count) + "</span> точек";
    html += "</div>";
    
    // Простые SVG графики
    html += "<div class='chart-row'>";
    html += "<div class='chart-container'>";
    html += "<h4>🌡️ Температура</h4>";
    html += "<div id='tempChart' class='simple-chart'>Загрузка...</div>";
    html += "</div>";
    html += "<div class='chart-container'>";
    html += "<h4>💧 Влажность</h4>";
    html += "<div id='humChart' class='simple-chart'>Загрузка...</div>";
    html += "</div>";
    html += "</div>";
    
    html += "<div class='chart-row'>";
    html += "<div class='chart-container'>";
    html += "<h4>⚗️ pH кислотность</h4>";
    html += "<div id='phChart' class='simple-chart'>Загрузка...</div>";
    html += "</div>";
    html += "<div class='chart-container'>";
    html += "<h4>⚡ EC проводимость</h4>";
    html += "<div id='ecChart' class='simple-chart'>Загрузка...</div>";
    html += "</div>";
    html += "</div>";
    
    html += "</div>"; // charts-section
    
    // Экспорт данных
    html += "<div class='export-section'>";
    html += "<h3>💾 Экспорт данных</h3>";
    html += "<a href='/api/analytics/export?format=json&period=hour' class='btn'>📄 JSON (час)</a>";
    html += "<a href='/api/analytics/export?format=csv&period=hour' class='btn'>📊 CSV (час)</a>";
    html += "<a href='/api/analytics/export?format=json&period=day' class='btn btn-secondary'>📄 JSON (день)</a>";
    html += "<a href='/api/analytics/export?format=csv&period=day' class='btn btn-secondary'>📊 CSV (день)</a>";
    html += "</div>";
    
    // Простой JavaScript для SVG графиков
    html += "<script>";
    html += "let chartsData = [];";
    html += "";
    html += "function createSimpleChart(containerId, data, color, unit, label) {";
    html += "  const container = document.getElementById(containerId);";
    html += "  if (!data || data.length === 0) {";
    html += "    container.innerHTML = '<div class=\"no-data\">📊 Нет данных</div>';";
    html += "    return;";
    html += "  }";
    html += "";
    html += "  const maxVal = Math.max(...data);";
    html += "  const minVal = Math.min(...data);";
    html += "  const range = maxVal - minVal || 1;";
    html += "";
    html += "  let svg = '<svg width=\"100%\" height=\"120\" style=\"background:#f9f9f9;border-radius:6px;\">';";
    html += "  svg += '<defs><linearGradient id=\"grad' + containerId + '\" x1=\"0%\" y1=\"0%\" x2=\"0%\" y2=\"100%\">';";
    html += "  svg += '<stop offset=\"0%\" style=\"stop-color:' + color + ';stop-opacity:0.8\" />';";
    html += "  svg += '<stop offset=\"100%\" style=\"stop-color:' + color + ';stop-opacity:0.1\" />';";
    html += "  svg += '</linearGradient></defs>';";
    html += "";
    html += "  // Создаем точки для линии";
    html += "  const points = data.map((val, i) => {";
    html += "    const x = (i / (data.length - 1)) * 280 + 10;";
    html += "    const y = 100 - ((val - minVal) / range) * 80;";
    html += "    return x + ',' + y;";
    html += "  }).join(' ');";
    html += "";
    html += "  // Создаем область заливки";
    html += "  const areaPoints = '10,100 ' + points + ' ' + (data.length > 1 ? (10 + 280) + ',100' : '10,100');";
    html += "";
    html += "  svg += '<polygon points=\"' + areaPoints + '\" fill=\"url(#grad' + containerId + ')\" />';";
    html += "  svg += '<polyline points=\"' + points + '\" fill=\"none\" stroke=\"' + color + '\" stroke-width=\"2\" />';";
    html += "";
    html += "  // Добавляем точки";
    html += "  data.forEach((val, i) => {";
    html += "    const x = (i / (data.length - 1)) * 280 + 10;";
    html += "    const y = 100 - ((val - minVal) / range) * 80;";
    html += "    svg += '<circle cx=\"' + x + '\" cy=\"' + y + '\" r=\"3\" fill=\"' + color + '\" />';";
    html += "  });";
    html += "";
    html += "  // Подписи";
    html += "  svg += '<text x=\"10\" y=\"15\" font-size=\"12\" fill=\"#666\">Макс: ' + maxVal.toFixed(1) + unit + '</text>';";
    html += "  svg += '<text x=\"10\" y=\"115\" font-size=\"12\" fill=\"#666\">Мин: ' + minVal.toFixed(1) + unit + '</text>';";
    html += "  svg += '<text x=\"200\" y=\"115\" font-size=\"12\" fill=\"#666\">Текущ: ' + data[data.length-1].toFixed(1) + unit + '</text>';";
    html += "";
    html += "  svg += '</svg>';";
    html += "  container.innerHTML = svg;";
    html += "}";
    html += "";
    html += "async function loadSimpleCharts() {";
    html += "  try {";
    html += "    const response = await fetch('/api/analytics?period=hour');";
    html += "    const data = await response.json();";
    html += "    console.log('Данные API:', data);";
    html += "";
    html += "    document.getElementById('dataCount').textContent = data.analytics_export?.info?.total_points || '0';";
    html += "";
    html += "    if (data.analytics_export?.raw_data && data.analytics_export.raw_data.length > 0) {";
    html += "      const rawData = data.analytics_export.raw_data.slice(-10);";
    html += "";
    html += "      createSimpleChart('tempChart', rawData.map(p => p.temperature), '#FF6B6B', '°C', 'Температура');";
    html += "      createSimpleChart('humChart', rawData.map(p => p.humidity), '#4ECDC4', '%', 'Влажность');";
    html += "      createSimpleChart('phChart', rawData.map(p => p.ph), '#FFE66D', '', 'pH');";
    html += "      createSimpleChart('ecChart', rawData.map(p => p.ec), '#A8E6CF', 'µS/cm', 'EC');";
    html += "    } else {";
    html += "      const noDataMsg = '<div class=\"no-data\">📊 Данные накапливаются...<br/>Ожидайте ' + (30 - Math.floor((Date.now() - " + String(millis()) + ") / 1000)) + ' сек</div>';";
    html += "      ['tempChart', 'humChart', 'phChart', 'ecChart'].forEach(id => {";
    html += "        document.getElementById(id).innerHTML = noDataMsg;";
    html += "      });";
    html += "    }";
    html += "  } catch (error) {";
    html += "    console.error('Ошибка загрузки:', error);";
    html += "    const errorMsg = '<div class=\"no-data error\">❌ Ошибка API</div>';";
    html += "    ['tempChart', 'humChart', 'phChart', 'ecChart'].forEach(id => {";
    html += "      document.getElementById(id).innerHTML = errorMsg;";
    html += "    });";
    html += "  }";
    html += "}";
    html += "";
    html += "// Запуск";
    html += "window.onload = function() {";
    html += "  loadSimpleCharts();";
    html += "  setInterval(loadSimpleCharts, 5000);";
    html += "};";
    html += "</script>";
    
    html += "</div></body></html>";
    
    // Добавляем заголовки для кэширования
    webServer.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
    webServer.sendHeader("Pragma", "no-cache");
    webServer.sendHeader("Expires", "0");
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

// v2.4.1: Алгоритмы обработки данных

float calculateMean(float* values, int count)
{
    if (count <= 0) return 0.0f;
    
    float sum = 0.0f;
    for (int i = 0; i < count; i++) {
        sum += values[i];
    }
    return sum / count;
}

float calculateMedian(float* values, int count)
{
    if (count <= 0) return 0.0f;
    
    // Создаем копию массива для сортировки
    float* sorted = new float[count];
    for (int i = 0; i < count; i++) {
        sorted[i] = values[i];
    }
    
    // Простая сортировка пузырьком для небольших массивов
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (sorted[j] > sorted[j + 1]) {
                float temp = sorted[j];
                sorted[j] = sorted[j + 1];
                sorted[j + 1] = temp;
            }
        }
    }
    
    float median;
    if (count % 2 == 0) {
        // Четное количество элементов - среднее из двух средних
        median = (sorted[count/2 - 1] + sorted[count/2]) / 2.0f;
    } else {
        // Нечетное количество - средний элемент
        median = sorted[count/2];
    }
    
    delete[] sorted;
    return median;
}

float calculateStandardDeviation(float* values, int count, float mean)
{
    if (count <= 1) return 0.0f;
    
    float sum_sq_diff = 0.0f;
    for (int i = 0; i < count; i++) {
        float diff = values[i] - mean;
        sum_sq_diff += diff * diff;
    }
    
    return sqrt(sum_sq_diff / (count - 1));  // Выборочное стандартное отклонение
}

bool isOutlier(float value, float mean, float stddev)
{
    if (stddev <= 0.0f) return false;  // Если нет вариации, то нет выбросов
    
    float z_score = fabs(value - mean) / stddev;
    return z_score > OUTLIER_SIGMA_THRESHOLD;
}

bool applyOutlierFilter(AnalyticsDataPoint* point)
{
    extern Config config;
    
    if (!config.outlierFilterEnabled) {
        return true;  // Фильтр отключен - принимаем все данные
    }
    
    // Проверяем выбросы по температуре (используем последние 10 точек)
    const int check_count = 10;
    if (analytics.count >= check_count) {
        float temp_values[check_count];
        int values_collected = 0;
        
        // Собираем последние значения температуры
        for (int i = 0; i < check_count && values_collected < check_count; i++) {
            int idx = (analytics.head_index - 1 - i + ANALYTICS_BUFFER_SIZE) % ANALYTICS_BUFFER_SIZE;
            if (analytics.buffer[idx].valid) {
                temp_values[values_collected] = analytics.buffer[idx].temperature;
                values_collected++;
            }
        }
        
        if (values_collected >= 3) {  // Минимум 3 точки для статистики
            float mean = calculateMean(temp_values, values_collected);
            float stddev = calculateStandardDeviation(temp_values, values_collected, mean);
            
            if (isOutlier(point->temperature, mean, stddev)) {
                logWarn("Outlier filter: температура %.1f°C отклоняется более чем на %.1fσ", 
                        point->temperature, OUTLIER_SIGMA_THRESHOLD);
                return false;  // Отбрасываем выброс
            }
        }
    }
    
    return true;  // Точка прошла фильтр или недостаточно данных для анализа
}

float processValueWithFilter(float* buffer, int count, uint8_t algorithm)
{
    if (count <= 0) return 0.0f;
    
    switch (algorithm) {
        case FILTER_ALGORITHM_MEDIAN:
            return calculateMedian(buffer, count);
        case FILTER_ALGORITHM_MEAN:
        default:
            return calculateMean(buffer, count);
    }
} 