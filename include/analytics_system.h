#pragma once

#include <Arduino.h>
#include "modbus_sensor.h"

// v2.4.0: Система базовой аналитики
#define ANALYTICS_BUFFER_SIZE 100           // Circular buffer для 100 измерений в RAM
#define ANALYTICS_STATS_WINDOW_HOUR 3600000 // 1 час в миллисекундах
#define ANALYTICS_STATS_WINDOW_DAY 86400000 // 1 день в миллисекундах  
#define ANALYTICS_STATS_WINDOW_WEEK 604800000 // 1 неделя в миллисекундах

// Структура для одного измерения в историческом буфере
struct AnalyticsDataPoint
{
    float temperature;
    float humidity;
    float ec;
    float ph;
    float nitrogen;
    float phosphorus;
    float potassium;
    unsigned long timestamp;  // Время измерения
    bool valid;              // Валидность данных
};

// Структура для статистики по периодам
struct AnalyticsStats
{
    // Температура
    float temp_min, temp_max, temp_avg;
    // Влажность  
    float humidity_min, humidity_max, humidity_avg;
    // EC
    float ec_min, ec_max, ec_avg;
    // pH
    float ph_min, ph_max, ph_avg;
    // NPK
    float npk_min, npk_max, npk_avg;
    
    // Метаданные
    unsigned long period_start;
    unsigned long period_end;
    uint8_t sample_count;
    bool valid;
};

// Главная структура системы аналитики
struct AnalyticsSystem
{
    AnalyticsDataPoint buffer[ANALYTICS_BUFFER_SIZE];  // Circular buffer 100 точек
    uint8_t head_index;          // Голова буфера (следующая позиция для записи)
    uint8_t count;               // Количество записанных элементов (0-100)
    bool buffer_full;            // Флаг заполненности буфера
    
    // Кэшированная статистика
    AnalyticsStats stats_hour;   // Статистика за час
    AnalyticsStats stats_day;    // Статистика за день  
    AnalyticsStats stats_week;   // Статистика за неделю
    
    unsigned long last_stats_update; // Время последнего пересчета статистики
};

extern AnalyticsSystem analytics;

// Основные функции системы аналитики
void initAnalyticsSystem();
void addDataPointToAnalytics(const SensorData& data);
void calculateAnalyticsStats();
AnalyticsStats getStatsForPeriod(unsigned long period_ms);

// Функции экспорта данных
String exportAnalyticsToJSON(unsigned long period_ms = 0);
String exportAnalyticsToCSV(unsigned long period_ms = 0);

// Утилиты
uint8_t getAnalyticsBufferUsage();
unsigned long getAnalyticsOldestTimestamp();
unsigned long getAnalyticsNewestTimestamp();
void clearAnalyticsBuffer();

// Веб-интерфейс аналитики
void handleAnalyticsPage();
void handleAnalyticsAPI();
void handleAnalyticsExport(); 