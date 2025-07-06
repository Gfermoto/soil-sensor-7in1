#!/usr/bin/env python3
"""
JXCT Clang-Tidy Analysis Script
Полный анализ кода с помощью clang-tidy и генерация отчёта
"""

import os
import subprocess
import json
import datetime
from pathlib import Path
from collections import defaultdict

def run_clang_tidy(file_path):
    """Запускает clang-tidy для одного файла"""
    try:
        cmd = [
            "C:\\Program Files\\LLVM\\bin\\clang-tidy.exe",
            file_path,
            "--quiet",
            "--format-style=file"
        ]
        
        result = subprocess.run(cmd, capture_output=True, text=True, timeout=30, encoding='utf-8', errors='ignore')
        return {
            'file': file_path,
            'returncode': result.returncode,
            'stdout': result.stdout or '',
            'stderr': result.stderr or ''
        }
    except subprocess.TimeoutExpired:
        return {
            'file': file_path,
            'returncode': -1,
            'stdout': '',
            'stderr': 'Timeout expired'
        }
    except Exception as e:
        return {
            'file': file_path,
            'returncode': -1,
            'stdout': '',
            'stderr': str(e)
        }

def parse_warnings(output):
    """Парсит предупреждения clang-tidy"""
    if not output:
        return []
    
    warnings = []
    lines = output.split('\n')
    
    for line in lines:
        if 'warning:' in line and '.cpp:' in line:
            parts = line.split('warning:')
            if len(parts) >= 2:
                location = parts[0].strip()
                warning_text = parts[1].strip()
                warnings.append({
                    'location': location,
                    'warning': warning_text
                })
    
    return warnings

def categorize_warnings(warnings):
    """Категоризирует предупреждения по типам"""
    categories = defaultdict(list)
    
    for warning in warnings:
        warning_text = warning['warning']
        
        if 'readability-' in warning_text:
            categories['Читаемость'].append(warning)
        elif 'modernize-' in warning_text:
            categories['Модернизация'].append(warning)
        elif 'bugprone-' in warning_text:
            categories['Потенциальные баги'].append(warning)
        elif 'misc-' in warning_text:
            categories['Разное'].append(warning)
        elif 'cert-' in warning_text:
            categories['Безопасность'].append(warning)
        else:
            categories['Прочее'].append(warning)
    
    return categories

def generate_report(results):
    """Генерирует отчёт в формате Markdown"""
    report = []
    report.append("# CLANG-TIDY ПОЛНЫЙ ОТЧЁТ АНАЛИЗА")
    report.append(f"**Дата анализа:** {datetime.datetime.now().strftime('%d.%m.%Y %H:%M')}")
    report.append(f"**Версия clang-tidy:** 20.1.0")
    report.append("")
    
    # Статистика
    total_files = len(results)
    successful_files = sum(1 for r in results if r['returncode'] == 0 or r['returncode'] == 1)
    failed_files = total_files - successful_files
    
    total_warnings = 0
    all_warnings = []
    
    for result in results:
        if result['returncode'] in [0, 1]:  # 0 = нет проблем, 1 = есть предупреждения
            warnings = parse_warnings(result['stdout'])
            total_warnings += len(warnings)
            all_warnings.extend(warnings)
    
    report.append("## 📊 СТАТИСТИКА")
    report.append(f"- **Всего файлов проанализировано:** {total_files}")
    report.append(f"- **Успешно проанализировано:** {successful_files}")
    report.append(f"- **Ошибки анализа:** {failed_files}")
    report.append(f"- **Всего предупреждений:** {total_warnings}")
    report.append("")
    
    # Категоризация предупреждений
    categories = categorize_warnings(all_warnings)
    
    report.append("## 🔍 КАТЕГОРИИ ПРОБЛЕМ")
    for category, warnings in categories.items():
        report.append(f"### {category} ({len(warnings)} проблем)")
        
        # Топ-5 наиболее частых проблем в категории
        warning_counts = defaultdict(int)
        for warning in warnings:
            warning_type = warning['warning'].split('[')[1].split(']')[0] if '[' in warning['warning'] else 'unknown'
            warning_counts[warning_type] += 1
        
        top_warnings = sorted(warning_counts.items(), key=lambda x: x[1], reverse=True)[:5]
        
        for warning_type, count in top_warnings:
            report.append(f"- `{warning_type}`: {count} случаев")
        
        report.append("")
    
    # Детальный анализ по файлам
    report.append("## 📁 ДЕТАЛЬНЫЙ АНАЛИЗ ПО ФАЙЛАМ")
    
    for result in results:
        if result['returncode'] in [0, 1]:
            warnings = parse_warnings(result['stdout'])
            
            if warnings:
                report.append(f"### {result['file']} ({len(warnings)} предупреждений)")
                
                # Группируем по типам предупреждений
                file_categories = categorize_warnings(warnings)
                
                for category, category_warnings in file_categories.items():
                    if category_warnings:
                        report.append(f"**{category}:** {len(category_warnings)} проблем")
                        
                        # Показываем первые 3 предупреждения
                        for i, warning in enumerate(category_warnings[:3]):
                            report.append(f"- {warning['location']}: {warning['warning']}")
                        
                        if len(category_warnings) > 3:
                            report.append(f"- ... и ещё {len(category_warnings) - 3} проблем")
                        
                        report.append("")
            else:
                report.append(f"### {result['file']} ✅ Проблем не найдено")
                report.append("")
        else:
            report.append(f"### {result['file']} ❌ Ошибка анализа")
            report.append(f"```")
            report.append(result['stderr'])
            report.append(f"```")
            report.append("")
    
    # Рекомендации
    report.append("## 🎯 РЕКОМЕНДАЦИИ ПО ПРИОРИТЕТАМ")
    report.append("")
    report.append("### 🔴 КРИТИЧЕСКИЕ (исправить немедленно)")
    if 'Потенциальные баги' in categories:
        report.append("- Потенциальные баги - могут привести к неправильной работе")
    if 'Безопасность' in categories:
        report.append("- Проблемы безопасности - могут создать уязвимости")
    report.append("")
    
    report.append("### 🟡 СРЕДНИЕ (исправить в ближайшее время)")
    if 'Читаемость' in categories:
        report.append("- Проблемы читаемости - затрудняют сопровождение кода")
    if 'Модернизация' in categories:
        report.append("- Модернизация - улучшают качество кода")
    report.append("")
    
    report.append("### 🟢 НИЗКИЕ (можно исправить позже)")
    if 'Разное' in categories:
        report.append("- Разные мелкие проблемы")
    if 'Прочее' in categories:
        report.append("- Прочие стилистические замечания")
    report.append("")
    
    return '\n'.join(report)

def main():
    """Основная функция"""
    print("🔍 Запуск полного анализа clang-tidy...")
    
    # Находим все .cpp файлы
    cpp_files = []
    for root, dirs, files in os.walk('src'):
        for file in files:
            if file.endswith('.cpp'):
                cpp_files.append(os.path.join(root, file).replace('\\', '/'))
    
    print(f"📁 Найдено {len(cpp_files)} файлов для анализа")
    
    # Анализируем каждый файл
    results = []
    for i, file_path in enumerate(cpp_files, 1):
        print(f"⏳ Анализ {i}/{len(cpp_files)}: {file_path}")
        result = run_clang_tidy(file_path)
        results.append(result)
    
    # Генерируем отчёт
    print("📝 Генерация отчёта...")
    report = generate_report(results)
    
    # Сохраняем отчёт
    report_path = 'test_reports/clang_tidy_analysis_report.md'
    os.makedirs(os.path.dirname(report_path), exist_ok=True)
    
    with open(report_path, 'w', encoding='utf-8') as f:
        f.write(report)
    
    # Сохраняем JSON для дальнейшего использования
    json_path = 'test_reports/clang_tidy_analysis_data.json'
    with open(json_path, 'w', encoding='utf-8') as f:
        json.dump(results, f, indent=2, ensure_ascii=False)
    
    print(f"✅ Анализ завершён!")
    print(f"📄 Отчёт сохранён: {report_path}")
    print(f"🔧 Данные сохранены: {json_path}")
    
    # Краткая статистика
    total_warnings = 0
    for result in results:
        if result['returncode'] in [0, 1]:
            warnings = parse_warnings(result['stdout'])
            total_warnings += len(warnings)
    
    print(f"📊 Всего найдено предупреждений: {total_warnings}")

if __name__ == "__main__":
    main() 