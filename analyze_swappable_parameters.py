#!/usr/bin/env python3
import json

# Загружаем данные анализа
with open('test_reports/clang_tidy_analysis_data.json', 'r', encoding='utf-8') as f:
    data = json.load(f)

print("Файлы с предупреждениями bugprone-easily-swappable-parameters:")
print("=" * 60)

swappable_files = []

for file_data in data:
    filename = file_data.get('file', 'unknown')
    warnings = file_data.get('warnings', [])
    
    swappable_count = 0
    swappable_warnings = []
    for warning in warnings:
        if 'bugprone-easily-swappable-parameters' in warning:
            swappable_count += 1
            swappable_warnings.append(warning)
    
    if swappable_count > 0:
        swappable_files.append((filename, swappable_count, swappable_warnings))
        print(f"{filename}: {swappable_count} предупреждений")

print(f"\nВсего файлов с swappable-parameters: {len(swappable_files)}")
print(f"Общее количество предупреждений: {sum(count for _, count, _ in swappable_files)}")

# Сортируем по количеству предупреждений
swappable_files.sort(key=lambda x: x[1], reverse=True)
print("\nТоп файлов для рефакторинга:")
for filename, count, warnings in swappable_files[:10]:
    print(f"  {filename}: {count}")
    # Показываем первые 2 предупреждения для анализа
    for i, warning in enumerate(warnings[:2]):
        print(f"    {i+1}. {warning}")
    if len(warnings) > 2:
        print(f"    ... и ещё {len(warnings)-2} предупреждений")
    print() 