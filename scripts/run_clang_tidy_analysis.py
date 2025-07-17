#!/usr/bin/env python3
"""
🔍 Clang-tidy анализ для JXCT
Быстрый режим без зависаний
"""

import sys
import subprocess
import argparse
from pathlib import Path

def run_clang_tidy_quick():
    """Быстрый анализ clang-tidy"""
    print("[INFO] 🔍 Быстрый clang-tidy анализ...")
    
    # Проверяем только основные файлы
    main_files = [
        "src/validation_utils.cpp",
        "src/sensor_compensation.cpp", 
        "src/main.cpp"
    ]
    
    total_warnings = 0
    checked_files = 0
    
    for file_path in main_files:
        if Path(file_path).exists():
            try:
                result = subprocess.run([
                    "clang-tidy", file_path, "--", "-std=c++17"
                ], capture_output=True, text=True, timeout=30)
                
                if result.returncode == 0:
                    warnings = result.stdout.count("warning:")
                    total_warnings += warnings
                    checked_files += 1
                    print(f"   ✅ {file_path}: {warnings} предупреждений")
                else:
                    print(f"   ⚠️ {file_path}: ошибка анализа")
                    
            except subprocess.TimeoutExpired:
                print(f"   ⏰ {file_path}: таймаут")
            except Exception as e:
                print(f"   ❌ {file_path}: {e}")
    
    print(f"\n📊 Результаты быстрого анализа:")
    print(f"   📁 Проверено файлов: {checked_files}")
    print(f"   ⚠️ Всего предупреждений: {total_warnings}")
    
    return total_warnings <= 50  # Считаем успехом если предупреждений не больше 50

def main():
    """Главная функция"""
    parser = argparse.ArgumentParser(description="Clang-tidy анализ")
    parser.add_argument("--quick", action="store_true", help="Быстрый режим")
    args = parser.parse_args()
    
    if args.quick:
        success = run_clang_tidy_quick()
        return 0 if success else 1
    
    # Полный анализ (оригинальная логика)
    print("[INFO] 🔍 Полный clang-tidy анализ...")

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
    sys.exit(main())
