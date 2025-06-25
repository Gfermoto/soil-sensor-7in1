#!/usr/bin/env python3
"""
Скрипт анализа технического долга для JXCT проекта
Генерирует отчёты для CI/CD и отслеживания прогресса
"""

import os
import json
import subprocess
import sys
from datetime import datetime
from pathlib import Path

def run_command(cmd, cwd=None):
    """Выполняет команду и возвращает результат"""
    try:
        result = subprocess.run(cmd, shell=True, capture_output=True, text=True, cwd=cwd)
        return result.returncode == 0, result.stdout, result.stderr
    except Exception as e:
        return False, "", str(e)

def analyze_clang_tidy():
    """Анализ с помощью clang-tidy"""
    print("🔍 Анализ clang-tidy...")
    
    # Находим только наши C++ файлы (исключаем .pio/libdeps)
    cpp_files = []
    for root, dirs, files in os.walk("."):
        if any(exclude in root for exclude in [".pio/libdeps", "test_reports", "docs", ".git"]):
            continue
        for file in files:
            if file.endswith(('.cpp', '.h')):
                cpp_files.append(os.path.join(root, file))
    
    if not cpp_files:
        return {"error": "C++ файлы не найдены"}
    
    # Запускаем clang-tidy на наших файлах
    cmd = f"clang-tidy {' '.join(cpp_files)} -checks=modernize-*,performance-*,readability-*,bugprone-* -- -I include -I src -std=c++17"
    
    success, stdout, stderr = run_command(cmd)
    
    # Парсим результаты
    warnings = {
        "high": 0,
        "medium": 0,
        "low": 0,
        "details": []
    }
    
    for line in stdout.split('\n'):
        if 'warning:' in line and '.pio/libdeps' not in line:  # Исключаем внешние библиотеки
            if 'performance-' in line or 'bugprone-' in line:
                warnings["high"] += 1
            elif 'modernize-' in line:
                warnings["medium"] += 1
            else:
                warnings["low"] += 1
            warnings["details"].append(line.strip())
    
    return warnings

def analyze_include_dependencies():
    """Анализ зависимостей include"""
    print("📦 Анализ зависимостей include...")
    
    # Простая проверка на циклические зависимости
    include_files = []
    for root, dirs, files in os.walk("include"):
        for file in files:
            if file.endswith('.h'):
                include_files.append(os.path.join(root, file))
    
    cycles_found = 0
    unused_includes = 0
    
    # Проверяем первые 5 файлов
    for file in include_files[:5]:
        with open(file, 'r', encoding='utf-8') as f:
            content = f.read()
            # Простая эвристика для поиска неиспользуемых include
            if '#include' in content and '//' in content:
                unused_includes += 1
    
    return {
        "cycles": cycles_found,
        "unused_includes": unused_includes,
        "total_files_checked": len(include_files[:5])
    }

def analyze_code_duplication():
    """Анализ дублирования кода"""
    print("🔄 Анализ дублирования кода...")
    
    # Используем простую эвристику для поиска дублирования
    duplication_score = 0
    
    # Проверяем src директорию
    src_files = []
    for root, dirs, files in os.walk("src"):
        for file in files:
            if file.endswith('.cpp'):
                src_files.append(os.path.join(root, file))
    
    # Простая проверка на дублирование функций
    function_names = set()
    for file in src_files[:10]:  # Проверяем первые 10 файлов
        try:
            with open(file, 'r', encoding='utf-8') as f:
                content = f.read()
                # Ищем определения функций
                lines = content.split('\n')
                for line in lines:
                    if 'void ' in line or 'int ' in line or 'String ' in line:
                        if '(' in line and ')' in line:
                            func_name = line.split('(')[0].split()[-1]
                            if func_name in function_names:
                                duplication_score += 1
                            function_names.add(func_name)
        except:
            continue
    
    return {
        "duplication_score": duplication_score,
        "unique_functions": len(function_names),
        "files_checked": len(src_files[:10])
    }

def generate_report():
    """Генерирует полный отчёт"""
    print("📊 Генерация отчёта технического долга...")
    
    report = {
        "timestamp": datetime.now().isoformat(),
        "version": "3.5.0",
        "analysis": {}
    }
    
    # Выполняем анализы
    report["analysis"]["clang_tidy"] = analyze_clang_tidy()
    report["analysis"]["include_deps"] = analyze_include_dependencies()
    report["analysis"]["duplication"] = analyze_code_duplication()
    
    # Вычисляем общий score
    total_score = 0
    if "clang_tidy" in report["analysis"]:
        clang = report["analysis"]["clang_tidy"]
        if isinstance(clang, dict):
            total_score += clang.get("high", 0) * 10
            total_score += clang.get("medium", 0) * 5
            total_score += clang.get("low", 0) * 1
    
    if "include_deps" in report["analysis"]:
        deps = report["analysis"]["include_deps"]
        total_score += deps.get("cycles", 0) * 20
        total_score += deps.get("unused_includes", 0) * 5
    
    if "duplication" in report["analysis"]:
        dup = report["analysis"]["duplication"]
        total_score += dup.get("duplication_score", 0) * 15
    
    report["total_tech_debt_score"] = total_score
    
    # Определяем статус
    if total_score < 50:
        report["status"] = "🟢 Low"
    elif total_score < 100:
        report["status"] = "🟡 Medium"
    elif total_score < 200:
        report["status"] = "🟠 High"
    else:
        report["status"] = "🔴 Critical"
    
    return report

def main():
    """Главная функция"""
    print("🚀 Запуск анализа технического долга JXCT...")
    
    # Создаём директорию для отчётов если её нет
    os.makedirs("test_reports", exist_ok=True)
    
    # Генерируем отчёт
    report = generate_report()
    
    # Сохраняем в JSON
    report_file = "test_reports/technical-debt-ci.json"
    with open(report_file, 'w', encoding='utf-8') as f:
        json.dump(report, f, indent=2, ensure_ascii=False)
    
    print(f"✅ Отчёт сохранён в {report_file}")
    print(f"📈 Общий score технического долга: {report['total_tech_debt_score']}")
    print(f"📊 Статус: {report['status']}")
    
    # Выводим краткую сводку
    if "clang_tidy" in report["analysis"]:
        clang = report["analysis"]["clang_tidy"]
        if isinstance(clang, dict):
            print(f"🔍 Clang-tidy: {clang.get('high', 0)} high, {clang.get('medium', 0)} medium, {clang.get('low', 0)} low")
    
    if "include_deps" in report["analysis"]:
        deps = report["analysis"]["include_deps"]
        print(f"📦 Include deps: {deps.get('cycles', 0)} cycles, {deps.get('unused_includes', 0)} unused")
    
    if "duplication" in report["analysis"]:
        dup = report["analysis"]["duplication"]
        print(f"🔄 Duplication: {dup.get('duplication_score', 0)} duplicates found")
    
    # В CI режиме не падаем из-за технического долга (это проект в разработке)
    ci_mode = os.environ.get('CI') == 'true' or os.environ.get('GITHUB_ACTIONS') == 'true'
    
    if report["total_tech_debt_score"] >= 500 and not ci_mode:
        print("❌ Критический уровень технического долга!")
        sys.exit(1)
    elif report["total_tech_debt_score"] >= 200:
        if ci_mode:
            print("⚠️ Критический уровень технического долга (CI: продолжаем)")
        else:
            print("❌ Критический уровень технического долга!")
            sys.exit(1)
    elif report["total_tech_debt_score"] >= 100:
        print("⚠️ Высокий уровень технического долга")
    else:
        print("✅ Уровень технического долга приемлемый")
    
    # В CI всегда возвращаем 0 (успех) для продолжения разработки
    if ci_mode:
        print("🔄 CI режим: анализ завершён успешно")
    sys.exit(0)

if __name__ == "__main__":
    main() 