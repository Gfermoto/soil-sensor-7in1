/**
 * @file advanced_filters.h
 * @brief Заголовочный файл для улучшенных алгоритмов фильтрации
 * @version 3.10.0
 * @author JXCT Development Team
 */

#ifndef ADVANCED_FILTERS_H
#define ADVANCED_FILTERS_H

#include <array>
#include <cstdint>

// Включаем полное определение SensorData
#include "modbus_sensor.h"

namespace AdvancedFilters
{

// ============================================================================
// ТИПЫ ФИЛЬТРОВ
// ============================================================================

enum class FilterType
{
    TEMPERATURE,
    HUMIDITY,
    EC,
    PH,
    NITROGEN,
    PHOSPHORUS,
    POTASSIUM
};

// ============================================================================
// ПУБЛИЧНЫЕ ФУНКЦИИ
// ============================================================================

/**
 * @brief Применяет улучшенную фильтрацию к данным датчика
 * @param data Данные датчика для фильтрации
 * @details Комбинирует экспоненциальное сглаживание, адаптивные пороги и фильтр Калмана
 */
void applyAdvancedFiltering(SensorData& data);

/**
 * @brief Сбрасывает все фильтры в начальное состояние
 * @details Используется при смене конфигурации или перезагрузке
 */
void resetAllFilters();

/**
 * @brief Выводит статистику работы фильтров
 * @details Показывает средние значения и стандартные отклонения для каждого параметра
 */
void logFilterStatistics();

}  // namespace AdvancedFilters

#endif  // ADVANCED_FILTERS_H
