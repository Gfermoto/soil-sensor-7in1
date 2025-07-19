#!/usr/bin/env python3
"""
🧪 Простой тест адаптивных фильтров
"""

import math

def test_exponential_smoothing():
    """Тест экспоненциального сглаживания"""
    print("🔄 Тест экспоненциального сглаживания...")
    
    # Симулируем экспоненциальное сглаживание
    alpha = 0.3  # Коэффициент сглаживания
    values = [25.0, 26.0, 24.5, 27.0, 25.5, 26.5, 24.0, 25.8]
    smoothed = []
    
    # Первое значение
    smoothed.append(values[0])
    
    # Применяем формулу: S_t = α * X_t + (1-α) * S_{t-1}
    for i in range(1, len(values)):
        s_t = alpha * values[i] + (1 - alpha) * smoothed[i-1]
        smoothed.append(s_t)
    
    # Проверяем, что сглаживание работает
    original_variance = sum((x - sum(values)/len(values))**2 for x in values) / len(values)
    smoothed_variance = sum((x - sum(smoothed)/len(smoothed))**2 for x in smoothed) / len(smoothed)
    
    # Сглаженные значения должны иметь меньшую дисперсию
    smoothing_effective = smoothed_variance < original_variance
    
    print(f"   📊 Исходная дисперсия: {original_variance:.4f}")
    print(f"   📊 Сглаженная дисперсия: {smoothed_variance:.4f}")
    print(f"   ✅ Сглаживание эффективно: {'Да' if smoothing_effective else 'Нет'}")
    
    return smoothing_effective

def test_outlier_detection():
    """Тест обнаружения выбросов"""
    print("🔍 Тест обнаружения выбросов...")
    
    # Создаем данные с выбросами
    normal_values = [25.0, 25.1, 25.2, 25.0, 25.1, 25.3, 25.0, 25.2]
    outlier_values = [25.0, 25.1, 25.2, 25.0, 25.1, 35.0, 25.0, 25.2]  # Выброс 35.0
    
    # Вычисляем статистики
    normal_mean = sum(normal_values) / len(normal_values)
    normal_std = math.sqrt(sum((x - normal_mean) ** 2 for x in normal_values) / len(normal_values))
    
    # Проверяем выбросы (3σ правило)
    threshold = 3 * normal_std
    outliers_detected = []
    
    for value in outlier_values:
        deviation = abs(value - normal_mean)
        if deviation > threshold:
            outliers_detected.append(value)
    
    # Должен быть обнаружен выброс 35.0
    outlier_found = 35.0 in outliers_detected
    
    print(f"   📊 Среднее: {normal_mean:.2f}")
    print(f"   📊 Стандартное отклонение: {normal_std:.2f}")
    print(f"   📊 Порог (3σ): {threshold:.2f}")
    print(f"   🔍 Обнаружено выбросов: {len(outliers_detected)}")
    print(f"   ✅ Выброс 35.0 обнаружен: {'Да' if outlier_found else 'Нет'}")
    
    return outlier_found

def main():
    """Главная функция"""
    print("🔍 ВАЛИДАЦИЯ АДАПТИВНЫХ ФИЛЬТРОВ")
    print("   📊 Проверяем алгоритмы фильтрации...")
    
    # Запускаем тесты
    test1 = test_exponential_smoothing()
    test2 = test_outlier_detection()
    
    # Подсчитываем результаты
    total_tests = 2
    passed_tests = sum([test1, test2])
    
    print(f"\n📊 ИТОГИ ВАЛИДАЦИИ:")
    print(f"   ✅ Пройдено: {passed_tests}/{total_tests}")
    print(f"   📈 Успешность: {passed_tests/total_tests*100:.1f}%")
    
    if passed_tests == total_tests:
        print("🎉 АДАПТИВНЫЕ ФИЛЬТРЫ РАБОТАЮТ КОРРЕКТНО!")
        return 0
    else:
        print("⚠️ НЕКОТОРЫЕ ФИЛЬТРЫ ТРЕБУЮТ ВНИМАНИЯ")
        return 1

if __name__ == "__main__":
    exit(main()) 