#!/usr/bin/env python3
import json

# Загружаем данные анализа
with open('test_reports/clang_tidy_analysis_data.json', 'r', encoding='utf-8') as f:
    data = json.load(f)

print("Файлы с предупреждениями misc-use-internal-linkage:")
print("=" * 50)

internal_linkage_files = []

# data - это список файлов с предупреждениями
for file_data in data:
    filename = file_data.get('file', 'unknown')
    warnings = file_data.get('warnings', [])
    
    internal_linkage_count = 0
    for warning in warnings:
        if 'misc-use-internal-linkage' in warning:
            internal_linkage_count += 1
    
    if internal_linkage_count > 0:
        internal_linkage_files.append((filename, internal_linkage_count))
        print(f"{filename}: {internal_linkage_count} предупреждений")

print(f"\nВсего файлов с internal-linkage: {len(internal_linkage_files)}")
print(f"Общее количество предупреждений: {sum(count for _, count in internal_linkage_files)}")

# Сортируем по количеству предупреждений
internal_linkage_files.sort(key=lambda x: x[1], reverse=True)
print("\nТоп файлов для рефакторинга:")
for filename, count in internal_linkage_files[:10]:
    print(f"  {filename}: {count}") 