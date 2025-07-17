#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Cppcheck анализ для JXCT
Альтернатива clang-tidy, которая точно работает
"""

import sys
import subprocess
import json
import os
from pathlib import Path
from datetime import datetime

def run_cppcheck_analysis():
    """Запуск cppcheck анализа"""
    print("[INFO] 🔍 Cppcheck анализ JXCT...")
    
    # Находим все .cpp файлы
    cpp_files = []
    for root, dirs, files in os.walk('src'):
        for file in files:
            if file.endswith('.cpp'):
                cpp_files.append(os.path.join(root, file).replace('\\', '/'))
    
    print(f"📁 Найдено {len(cpp_files)} файлов для анализа")
    
    # Анализируем с помощью PlatformIO
    try:
        result = subprocess.run([
            "pio", "check", "-e", "static-analysis"
        ], capture_output=True, text=True, timeout=120)
        
        if result.returncode == 0:
            print("✅ Cppcheck анализ завершён успешно")
        else:
            print("⚠️ Cppcheck анализ завершён с предупреждениями")
        
        # Парсим результаты
        warnings = parse_cppcheck_output(result.stdout)
        
        # Генерируем отчёт
        report = generate_cppcheck_report(warnings, cpp_files)
        
        # Сохраняем отчёт
        report_path = 'test_reports/cppcheck_analysis_report.md'
        os.makedirs(os.path.dirname(report_path), exist_ok=True)
        
        with open(report_path, 'w', encoding='utf-8') as f:
            f.write(report)
        
        # Сохраняем JSON
        json_path = 'test_reports/cppcheck_analysis_data.json'
        with open(json_path, 'w', encoding='utf-8') as f:
            json.dump({
                'timestamp': datetime.now().isoformat(),
                'version': '3.10.1',
                'total_warnings': len(warnings),
                'warnings': warnings,
                'files_checked': len(cpp_files)
            }, f, indent=2, ensure_ascii=False)
        
        print(f"📄 Отчёт сохранён: {report_path}")
        print(f"🔧 Данные сохранены: {json_path}")
        print(f"📊 Всего найдено предупреждений: {len(warnings)}")
        
        return len(warnings)
        
    except subprocess.TimeoutExpired:
        print("❌ Таймаут анализа")
        return -1
    except Exception as e:
        print(f"❌ Ошибка анализа: {e}")
        return -1

def parse_cppcheck_output(output):
    """Парсинг вывода cppcheck"""
    warnings = []
    
    for line in output.split('\n'):
        if ':' in line and ('[low:' in line or '[medium:' in line or '[high:' in line):
            # Пример: src/advanced_filters.cpp:529: [low:style] The function 'resetAllFilters' is never used. [unusedFunction]
            parts = line.split(':')
            if len(parts) >= 4:
                file_path = parts[0]
                line_num = parts[1]
                severity = parts[2].split('[')[1].split(':')[0] if '[' in parts[2] else 'unknown'
                message = ':'.join(parts[3:]).strip()
                
                warnings.append({
                    'file': file_path,
                    'line': line_num,
                    'severity': severity,
                    'message': message
                })
    
    return warnings

def generate_cppcheck_report(warnings, files_checked):
    """Генерация отчёта по результатам анализа"""
    report = "# Cppcheck Анализ JXCT\n\n"
    report += f"Дата: {datetime.now().strftime('%d.%m.%Y %H:%M:%S')}\n\n"
    
    # Статистика
    total_warnings = len(warnings)
    high_warnings = len([w for w in warnings if w['severity'] == 'high'])
    medium_warnings = len([w for w in warnings if w['severity'] == 'medium'])
    low_warnings = len([w for w in warnings if w['severity'] == 'low'])
    
    report += f"## Статистика\n\n"
    report += f"- Всего файлов: {len(files_checked)}\n"
    report += f"- Всего предупреждений: {total_warnings}\n"
    report += f"- Высокий приоритет: {high_warnings}\n"
    report += f"- Средний приоритет: {medium_warnings}\n"
    report += f"- Низкий приоритет: {low_warnings}\n\n"
    
    # Группировка по файлам
    file_warnings = {}
    for warning in warnings:
        file = warning['file']
        if file not in file_warnings:
            file_warnings[file] = []
        file_warnings[file].append(warning)
    
    report += f"## Детали по файлам\n\n"
    for file, file_warns in sorted(file_warnings.items()):
        report += f"### {file}\n\n"
        for warning in file_warns:
            severity_emoji = {
                'high': '🔴',
                'medium': '🟡', 
                'low': '🟢'
            }.get(warning['severity'], '⚪')
            
            report += f"{severity_emoji} **{warning['severity'].upper()}** (строка {warning['line']}): {warning['message']}\n\n"
    
    return report

def main():
    """Главная функция"""
    print("🚀 Cppcheck анализ JXCT...")
    
    total_warnings = run_cppcheck_analysis()
    
    if total_warnings >= 0:
        print(f"\n📊 Результат: {total_warnings} предупреждений")
        if total_warnings == 0:
            print("🎉 Отлично! Нет предупреждений!")
        elif total_warnings < 50:
            print("✅ Хорошо! Немного предупреждений")
        else:
            print("⚠️ Много предупреждений, требуется внимание")
    else:
        print("❌ Анализ не удался")
        return 1
    
    return 0

if __name__ == "__main__":
    sys.exit(main()) 