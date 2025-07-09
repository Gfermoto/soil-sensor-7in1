/**
 * @file advanced_filters.cpp
 * @brief Улучшенные алгоритмы фильтрации для снижения зашумленности данных
 * @version 3.10.0
 * @author JXCT Development Team
 */

#include "advanced_filters.h"
#include "jxct_config_vars.h"
#include "jxct_constants.h"
#include "logger.h"
#include "modbus_sensor.h"
#include <algorithm>
#include <cmath>
#include <numeric>

namespace AdvancedFilters {

// ============================================================================
// ЭКСПОНЕНЦИАЛЬНОЕ СГЛАЖИВАНИЕ
// ============================================================================

struct ExponentialSmoothingState {
    float smoothed_value;
    bool initialized;
    
    ExponentialSmoothingState() : smoothed_value(0.0F), initialized(false) {}
};

static ExponentialSmoothingState exp_smooth_temp;
static ExponentialSmoothingState exp_smooth_hum;
static ExponentialSmoothingState exp_smooth_ec;
static ExponentialSmoothingState exp_smooth_ph;
static ExponentialSmoothingState exp_smooth_n;
static ExponentialSmoothingState exp_smooth_p;
static ExponentialSmoothingState exp_smooth_k;

float applyExponentialSmoothing(float new_value, ExponentialSmoothingState& state, float alpha) {
    if (!state.initialized) {
        state.smoothed_value = new_value;
        state.initialized = true;
        return new_value;
    }
    
    // Экспоненциальное сглаживание: S_t = α * X_t + (1-α) * S_{t-1}
    state.smoothed_value = alpha * new_value + (1.0F - alpha) * state.smoothed_value;
    return state.smoothed_value;
}

// ============================================================================
// АДАПТИВНЫЕ ПОРОГИ ВЫБРОСОВ
// ============================================================================

struct StatisticsBuffer {
    std::array<float, STATISTICS_WINDOW_SIZE> values;
    uint8_t index;
    uint8_t filled;
    float mean;
    float std_dev;
    bool valid;
    
    StatisticsBuffer() : index(0), filled(0), mean(0.0F), std_dev(0.0F), valid(false) {
        values.fill(0.0F);
    }
};

static StatisticsBuffer stats_temp;
static StatisticsBuffer stats_hum;
static StatisticsBuffer stats_ec;
static StatisticsBuffer stats_ph;
static StatisticsBuffer stats_n;
static StatisticsBuffer stats_p;
static StatisticsBuffer stats_k;

void updateStatistics(float new_value, StatisticsBuffer& buffer) {
    // Добавляем новое значение
    buffer.values[buffer.index] = new_value;
    buffer.index = (buffer.index + 1) % STATISTICS_WINDOW_SIZE;
    
    if (buffer.filled < STATISTICS_WINDOW_SIZE) {
        buffer.filled++;
    }
    
    // Вычисляем среднее
    float sum = 0.0F;
    for (uint8_t i = 0; i < buffer.filled; ++i) {
        sum += buffer.values[i];
    }
    buffer.mean = sum / static_cast<float>(buffer.filled);
    
    // Вычисляем стандартное отклонение
    float variance_sum = 0.0F;
    for (uint8_t i = 0; i < buffer.filled; ++i) {
        float diff = buffer.values[i] - buffer.mean;
        variance_sum += diff * diff;
    }
    buffer.std_dev = sqrt(variance_sum / static_cast<float>(buffer.filled));
    
    // Минимальное стандартное отклонение для стабильности
    if (buffer.std_dev < MIN_STANDARD_DEVIATION) {
        buffer.std_dev = MIN_STANDARD_DEVIATION;
    }
    
    buffer.valid = (buffer.filled >= 5); // Минимум 5 значений для статистики
}

bool isOutlier(float value, const StatisticsBuffer& buffer, float threshold_multiplier) {
    if (!buffer.valid) {
        return false; // Недостаточно данных для определения выброса
    }
    
    float deviation = abs(value - buffer.mean);
    float threshold = threshold_multiplier * buffer.std_dev;
    
    return deviation > threshold;
}

// ============================================================================
// ФИЛЬТР КАЛМАНА
// ============================================================================

struct KalmanFilter {
    float x;      // Состояние (оценка)
    float P;      // Ковариация ошибки оценки
    float Q;      // Шум процесса
    float R;      // Шум измерений
    bool initialized;
    
    KalmanFilter(float process_noise, float measurement_noise) 
        : x(0.0F), P(KALMAN_INITIAL_UNCERTAINTY), Q(process_noise), R(measurement_noise), initialized(false) {}
    
    float update(float measurement) {
        if (!initialized) {
            x = measurement;
            initialized = true;
            return measurement;
        }
        
        // Предсказание
        float P_pred = P + Q;
        
        // Обновление
        float K = P_pred / (P_pred + R); // Коэффициент Калмана
        x = x + K * (measurement - x);
        P = (1.0F - K) * P_pred;
        
        return x;
    }
    
    void reset() {
        x = 0.0F;
        P = KALMAN_INITIAL_UNCERTAINTY;
        initialized = false;
    }
};

static KalmanFilter kalman_temp(KALMAN_PROCESS_NOISE, KALMAN_MEASUREMENT_NOISE);
static KalmanFilter kalman_hum(KALMAN_PROCESS_NOISE, KALMAN_MEASUREMENT_NOISE);
static KalmanFilter kalman_ec(KALMAN_PROCESS_NOISE, KALMAN_MEASUREMENT_NOISE);
static KalmanFilter kalman_ph(KALMAN_PROCESS_NOISE, KALMAN_MEASUREMENT_NOISE);
static KalmanFilter kalman_n(KALMAN_PROCESS_NOISE, KALMAN_MEASUREMENT_NOISE);
static KalmanFilter kalman_p(KALMAN_PROCESS_NOISE, KALMAN_MEASUREMENT_NOISE);
static KalmanFilter kalman_k(KALMAN_PROCESS_NOISE, KALMAN_MEASUREMENT_NOISE);

// ============================================================================
// КОМБИНИРОВАННЫЙ ФИЛЬТР
// ============================================================================

float applyCombinedFilter(float raw_value, FilterType type, bool enable_kalman, bool enable_adaptive) {
    float filtered_value = raw_value;
    
    // 1. Обновляем статистику для адаптивных порогов
    if (enable_adaptive) {
        StatisticsBuffer* buffer = nullptr;
        switch (type) {
            case FilterType::TEMPERATURE: buffer = &stats_temp; break;
            case FilterType::HUMIDITY: buffer = &stats_hum; break;
            case FilterType::EC: buffer = &stats_ec; break;
            case FilterType::PH: buffer = &stats_ph; break;
            case FilterType::NITROGEN: buffer = &stats_n; break;
            case FilterType::PHOSPHORUS: buffer = &stats_p; break;
            case FilterType::POTASSIUM: buffer = &stats_k; break;
        }
        
        if (buffer) {
            updateStatistics(raw_value, *buffer);
            
            // Проверяем на выбросы
            if (isOutlier(raw_value, *buffer, config.outlierThreshold)) {
                // Возвращаем предыдущее значение вместо выброса
                return buffer->mean;
            }
        }
    }
    
    // 2. Применяем экспоненциальное сглаживание
    ExponentialSmoothingState* exp_state = nullptr;
    float alpha = config.exponentialAlpha; // Базовый коэффициент
    
    // Дифференцированные настройки для шумных параметров
    switch (type) {
        case FilterType::EC:
            alpha = config.exponentialAlpha * 0.7F; // Более агрессивное сглаживание для EC
            break;
        case FilterType::NITROGEN:
        case FilterType::PHOSPHORUS:
        case FilterType::POTASSIUM:
            alpha = config.exponentialAlpha * 0.8F; // Умеренное сглаживание для NPK
            break;
        default:
            alpha = config.exponentialAlpha; // Обычное сглаживание для остальных
            break;
    }
    
    switch (type) {
        case FilterType::TEMPERATURE: exp_state = &exp_smooth_temp; break;
        case FilterType::HUMIDITY: exp_state = &exp_smooth_hum; break;
        case FilterType::EC: exp_state = &exp_smooth_ec; break;
        case FilterType::PH: exp_state = &exp_smooth_ph; break;
        case FilterType::NITROGEN: exp_state = &exp_smooth_n; break;
        case FilterType::PHOSPHORUS: exp_state = &exp_smooth_p; break;
        case FilterType::POTASSIUM: exp_state = &exp_smooth_k; break;
    }
    
    if (exp_state) {
        filtered_value = applyExponentialSmoothing(filtered_value, *exp_state, alpha);
    }
    
    // 3. Применяем фильтр Калмана (если включен)
    if (enable_kalman) {
        KalmanFilter* kalman = nullptr;
        switch (type) {
            case FilterType::TEMPERATURE: kalman = &kalman_temp; break;
            case FilterType::HUMIDITY: kalman = &kalman_hum; break;
            case FilterType::EC: kalman = &kalman_ec; break;
            case FilterType::PH: kalman = &kalman_ph; break;
            case FilterType::NITROGEN: kalman = &kalman_n; break;
            case FilterType::PHOSPHORUS: kalman = &kalman_p; break;
            case FilterType::POTASSIUM: kalman = &kalman_k; break;
        }
        
        if (kalman) {
            filtered_value = kalman->update(filtered_value);
        }
    }
    
    return filtered_value;
}

// ============================================================================
// ПУБЛИЧНЫЕ ФУНКЦИИ
// ============================================================================

void applyAdvancedFiltering(SensorData& data) {
    if (!config.adaptiveFiltering && !config.kalmanEnabled) {
        return; // Фильтрация отключена
    }
    
    // Применяем комбинированный фильтр ко всем параметрам
    data.temperature = applyCombinedFilter(data.temperature, FilterType::TEMPERATURE, 
                                         config.kalmanEnabled, config.adaptiveFiltering);
    data.humidity = applyCombinedFilter(data.humidity, FilterType::HUMIDITY, 
                                      config.kalmanEnabled, config.adaptiveFiltering);
    data.ec = applyCombinedFilter(data.ec, FilterType::EC, 
                                config.kalmanEnabled, config.adaptiveFiltering);
    data.ph = applyCombinedFilter(data.ph, FilterType::PH, 
                                config.kalmanEnabled, config.adaptiveFiltering);
    data.nitrogen = applyCombinedFilter(data.nitrogen, FilterType::NITROGEN, 
                                      config.kalmanEnabled, config.adaptiveFiltering);
    data.phosphorus = applyCombinedFilter(data.phosphorus, FilterType::PHOSPHORUS, 
                                        config.kalmanEnabled, config.adaptiveFiltering);
    data.potassium = applyCombinedFilter(data.potassium, FilterType::POTASSIUM, 
                                       config.kalmanEnabled, config.adaptiveFiltering);
}

void resetAllFilters() {
    // Сбрасываем экспоненциальное сглаживание
    exp_smooth_temp.initialized = false;
    exp_smooth_hum.initialized = false;
    exp_smooth_ec.initialized = false;
    exp_smooth_ph.initialized = false;
    exp_smooth_n.initialized = false;
    exp_smooth_p.initialized = false;
    exp_smooth_k.initialized = false;
    
    // Сбрасываем статистические буферы
    stats_temp = StatisticsBuffer();
    stats_hum = StatisticsBuffer();
    stats_ec = StatisticsBuffer();
    stats_ph = StatisticsBuffer();
    stats_n = StatisticsBuffer();
    stats_p = StatisticsBuffer();
    stats_k = StatisticsBuffer();
    
    // Сбрасываем фильтры Калмана
    kalman_temp.reset();
    kalman_hum.reset();
    kalman_ec.reset();
    kalman_ph.reset();
    kalman_n.reset();
    kalman_p.reset();
    kalman_k.reset();
    
    logSystem("[ADVANCED_FILTERS] Все фильтры сброшены");
}

void logFilterStatistics() {
    if (!config.adaptiveFiltering) {
        return;
    }
    
    logSystem("=== СТАТИСТИКА ФИЛЬТРОВ ===");
    logSystemSafe("Температура: μ=%.2f, σ=%.2f", stats_temp.mean, stats_temp.std_dev);
    logSystemSafe("Влажность: μ=%.2f, σ=%.2f", stats_hum.mean, stats_hum.std_dev);
    logSystemSafe("EC: μ=%.2f, σ=%.2f", stats_ec.mean, stats_ec.std_dev);
    logSystemSafe("pH: μ=%.2f, σ=%.2f", stats_ph.mean, stats_ph.std_dev);
    logSystemSafe("N: μ=%.2f, σ=%.2f", stats_n.mean, stats_n.std_dev);
    logSystemSafe("P: μ=%.2f, σ=%.2f", stats_p.mean, stats_p.std_dev);
    logSystemSafe("K: μ=%.2f, σ=%.2f", stats_k.mean, stats_k.std_dev);
}

} // namespace AdvancedFilters 