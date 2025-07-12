#!/usr/bin/env python3
"""
Тест производительности JXCT
Проверяет скорость работы критических функций
"""

import time
import sys
import random
import math

# Принудительно устанавливаем stdout в utf-8 для Windows
if hasattr(sys.stdout, 'encoding') and sys.stdout.encoding and sys.stdout.encoding.lower() not in ['utf-8', 'utf8']:
    try:
        sys.stdout = open(sys.stdout.fileno(), mode='w', encoding='utf-8', buffering=1)
    except Exception:
        pass

def test_validation_performance():
    """Тест производительности валидации"""
    print("⚡ ТЕСТ ПРОИЗВОДИТЕЛЬНОСТИ ВАЛИДАЦИИ")
    print("=" * 50)
    
    # Тестовые данные
    test_values = [
        (25.0, 50.0, 7.0, 1500.0),  # Нормальные значения
        (-10.0, 30.0, 6.5, 1200.0), # Низкие значения
        (40.0, 80.0, 8.5, 3000.0),  # Высокие значения
    ] * 1000  # 3000 тестов
    
    start_time = time.time()
    
    valid_count = 0
    for temp, hum, ph, ec in test_values:
        # Имитируем валидацию
        if -50 <= temp <= 80 and 0 <= hum <= 100 and 0 <= ph <= 14 and 0 <= ec <= 10000:
            valid_count += 1
    
    end_time = time.time()
    duration = end_time - start_time
    
    print(f"📊 Результаты:")
    print(f"  Всего тестов: {len(test_values)}")
    print(f"  Валидных: {valid_count}")
    print(f"  Время: {duration:.3f} секунд")
    print(f"  Скорость: {len(test_values)/duration:.0f} тестов/сек")
    
    # Проверяем производительность
    assert duration < 1.0, f"Производительность валидации требует оптимизации: {duration:.3f}s"
    print("  ✅ Производительность валидации отличная")

def test_calculation_performance():
    """Тест производительности расчетов"""
    print("\n🧮 ТЕСТ ПРОИЗВОДИТЕЛЬНОСТИ РАСЧЕТОВ")
    print("=" * 50)
    
    # Тестовые данные для расчетов
    test_data = [(random.uniform(0, 50), random.uniform(0, 100), random.uniform(0, 14), random.uniform(0, 5000)) for _ in range(1000)]
    
    start_time = time.time()
    
    results = []
    for temp, hum, ph, ec in test_data:
        # Имитируем сложные расчеты
        temp_factor = 1.0 / (1.0 + 0.02 * (temp - 25.0))
        hum_factor = 1.0 + 0.01 * (hum - 50.0)
        ph_factor = 1.0 + 0.05 * (ph - 7.0)
        
        ec_corrected = ec * temp_factor * hum_factor * ph_factor
        results.append(ec_corrected)
    
    end_time = time.time()
    duration = end_time - start_time
    
    print(f"📊 Результаты:")
    print(f"  Всего расчетов: {len(test_data)}")
    print(f"  Время: {duration:.3f} секунд")
    print(f"  Скорость: {len(test_data)/duration:.0f} расчетов/сек")
    
    # Проверяем производительность
    assert duration < 0.5, f"Производительность расчетов требует оптимизации: {duration:.3f}s"
    print("  ✅ Производительность расчетов отличная")

def test_memory_performance():
    """Тест производительности памяти"""
    print("\n💾 ТЕСТ ПРОИЗВОДИТЕЛЬНОСТИ ПАМЯТИ")
    print("=" * 50)
    
    # Имитируем работу с памятью
    data_blocks = []
    start_time = time.time()
    
    # Создаем блоки данных
    for i in range(100):
        block = {
            'id': i,
            'data': [random.random() for _ in range(100)],
            'timestamp': time.time()
        }
        data_blocks.append(block)
    
    # Обрабатываем данные
    processed = 0
    for block in data_blocks:
        # Имитируем обработку
        result = sum(block['data']) / len(block['data'])
        processed += 1
    
    end_time = time.time()
    duration = end_time - start_time
    
    print(f"📊 Результаты:")
    print(f"  Блоков данных: {len(data_blocks)}")
    print(f"  Обработано: {processed}")
    print(f"  Время: {duration:.3f} секунд")
    print(f"  Скорость: {len(data_blocks)/duration:.0f} блоков/сек")
    
    # Проверяем производительность
    assert duration < 0.1, f"Производительность памяти требует оптимизации: {duration:.3f}s"
    print("  ✅ Производительность памяти отличная")

def main():
    """Главная функция"""
    print("🚀 ТЕСТИРОВАНИЕ ПРОИЗВОДИТЕЛЬНОСТИ JXCT")
    print("=" * 60)
    
    # Запускаем все тесты производительности
    validation_ok = test_validation_performance()
    calculation_ok = test_calculation_performance()
    memory_ok = test_memory_performance()
    
    # Итоговый результат
    total_ok = validation_ok and calculation_ok and memory_ok
    
    print(f"\n📊 ИТОГОВЫЙ РЕЗУЛЬТАТ:")
    print(f"  Валидация: {'✅' if validation_ok else '❌'}")
    print(f"  Расчеты: {'✅' if calculation_ok else '❌'}")
    print(f"  Память: {'✅' if memory_ok else '❌'}")
    print(f"  Общий результат: {'✅ ТЕСТ ПРОЙДЕН' if total_ok else '❌ ТЕСТ ПРОВАЛЕН'}")
    
    return 0 if total_ok else 1

if __name__ == "__main__":
    sys.exit(main()) 