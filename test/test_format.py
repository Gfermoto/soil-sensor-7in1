#!/usr/bin/env python3
"""
Тест форматирования кода для JXCT
Версия: 1.0.0
Автор: EYERA Development Team
Дата: 2025-07-04
"""

import sys
import os
import json
from pathlib import Path

# Добавляем путь к проекту
project_root = Path(__file__).parent.parent
sys.path.insert(0, str(project_root))

def check_file_encoding(file_path):
    """Проверка кодировки файла"""
    try:
        with open(file_path, 'r', encoding='utf-8') as f:
            f.read()
        return True
    except UnicodeDecodeError:
        return False

def check_line_length(file_path, max_length=120):
    """Проверка длины строк"""
    try:
        with open(file_path, 'r', encoding='utf-8') as f:
            lines = f.readlines()

        long_lines = []
        for i, line in enumerate(lines, 1):
            if len(line.rstrip('\n')) > max_length:
                long_lines.append(i)

        return len(long_lines) == 0, long_lines
    except Exception:
        return False, []

def check_trailing_whitespace(file_path):
    """Проверка trailing whitespace"""
    try:
        with open(file_path, 'r', encoding='utf-8') as f:
            lines = f.readlines()

        trailing_lines = []
        for i, line in enumerate(lines, 1):
            if line.rstrip('\n') != line.rstrip():
                trailing_lines.append(i)

        return len(trailing_lines) == 0, trailing_lines
    except Exception:
        return False, []

def check_file_ending(file_path):
    """Проверка окончания файла"""
    try:
        with open(file_path, 'r', encoding='utf-8') as f:
            content = f.read()

        return content.endswith('\n') or content == '', not content.endswith('\n')
    except Exception:
        return False, True

def should_skip_file(file_path):
    """Проверка, нужно ли пропустить файл"""
    # Пропускаем файлы в виртуальном окружении и зависимостях
    skip_patterns = [
        '.venv',
        '.pio',
        '__pycache__',
        'node_modules',
        'site-packages',
        '.git'
    ]

    file_str = str(file_path)
    for pattern in skip_patterns:
        if pattern in file_str:
            return True

    return False

def test_cpp_formatting():
    """Тест форматирования C++ файлов"""
    print("🧪 Тестирование форматирования C++ файлов...")

    cpp_files = list(project_root.rglob("*.cpp")) + list(project_root.rglob("*.h"))
    # Фильтруем только файлы проекта
    cpp_files = [f for f in cpp_files if not should_skip_file(f)]

    total_files = len(cpp_files)
    passed_files = 0

    for file_path in cpp_files:
        file_relative = file_path.relative_to(project_root)
        issues = []

        # Проверка кодировки
        if not check_file_encoding(file_path):
            issues.append("неверная кодировка")

        # Проверка длины строк
        length_ok, long_lines = check_line_length(file_path)
        if not length_ok:
            issues.append(f"длинные строки: {long_lines[:3]}{'...' if len(long_lines) > 3 else ''}")

        # Проверка trailing whitespace
        whitespace_ok, trailing_lines = check_trailing_whitespace(file_path)
        if not whitespace_ok:
            issues.append(f"trailing whitespace: {trailing_lines[:3]}{'...' if len(trailing_lines) > 3 else ''}")

        # Проверка окончания файла
        ending_ok, _ = check_file_ending(file_path)
        if not ending_ok:
            issues.append("нет новой строки в конце")

        if not issues:
            passed_files += 1
            print(f"  ✅ {file_relative}")
        else:
            print(f"  ⚠️ {file_relative}: {', '.join(issues)}")

    print(f"  📊 C++ файлы: {passed_files}/{total_files} прошли проверку")
    return passed_files == total_files

def test_python_formatting():
    """Тест форматирования Python файлов"""
    print("🧪 Тестирование форматирования Python файлов...")

    python_files = list(project_root.rglob("*.py"))
    # Фильтруем только файлы проекта
    python_files = [f for f in python_files if not should_skip_file(f)]

    total_files = len(python_files)
    passed_files = 0

    for file_path in python_files:
        file_relative = file_path.relative_to(project_root)
        issues = []

        # Проверка кодировки
        if not check_file_encoding(file_path):
            issues.append("неверная кодировка")

        # Проверка длины строк
        length_ok, long_lines = check_line_length(file_path, max_length=100)
        if not length_ok:
            issues.append(f"длинные строки: {long_lines[:3]}{'...' if len(long_lines) > 3 else ''}")

        # Проверка trailing whitespace
        whitespace_ok, trailing_lines = check_trailing_whitespace(file_path)
        if not whitespace_ok:
            issues.append(f"trailing whitespace: {trailing_lines[:3]}{'...' if len(trailing_lines) > 3 else ''}")

        # Проверка окончания файла
        ending_ok, _ = check_file_ending(file_path)
        if not ending_ok:
            issues.append("нет новой строки в конце")

        if not issues:
            passed_files += 1
            print(f"  ✅ {file_relative}")
        else:
            print(f"  ⚠️ {file_relative}: {', '.join(issues)}")

    print(f"  📊 Python файлы: {passed_files}/{total_files} прошли проверку")
    return passed_files == total_files

def test_markdown_formatting():
    """Тест форматирования Markdown файлов"""
    print("🧪 Тестирование форматирования Markdown файлов...")

    md_files = list(project_root.rglob("*.md"))
    # Фильтруем только файлы проекта
    md_files = [f for f in md_files if not should_skip_file(f)]

    total_files = len(md_files)
    passed_files = 0

    for file_path in md_files:
        file_relative = file_path.relative_to(project_root)
        issues = []

        # Проверка кодировки
        if not check_file_encoding(file_path):
            issues.append("неверная кодировка")

        # Проверка длины строк
        length_ok, long_lines = check_line_length(file_path, max_length=120)
        if not length_ok:
            issues.append(f"длинные строки: {long_lines[:3]}{'...' if len(long_lines) > 3 else ''}")

        # Проверка trailing whitespace
        whitespace_ok, trailing_lines = check_trailing_whitespace(file_path)
        if not whitespace_ok:
            issues.append(f"trailing whitespace: {trailing_lines[:3]}{'...' if len(trailing_lines) > 3 else ''}")

        # Проверка окончания файла
        ending_ok, _ = check_file_ending(file_path)
        if not ending_ok:
            issues.append("нет новой строки в конце")

        if not issues:
            passed_files += 1
            print(f"  ✅ {file_relative}")
        else:
            print(f"  ⚠️ {file_relative}: {', '.join(issues)}")

    print(f"  📊 Markdown файлы: {passed_files}/{total_files} прошли проверку")
    return passed_files == total_files

def test_json_formatting():
    """Тест форматирования JSON файлов"""
    print("🧪 Тестирование форматирования JSON файлов...")

    json_files = list(project_root.rglob("*.json"))
    # Фильтруем только файлы проекта
    json_files = [f for f in json_files if not should_skip_file(f)]

    total_files = len(json_files)
    passed_files = 0

    for file_path in json_files:
        file_relative = file_path.relative_to(project_root)
        issues = []

        try:
            with open(file_path, 'r', encoding='utf-8') as f:
                json.load(f)
            print(f"  ✅ {file_relative}")
            passed_files += 1
        except Exception as e:
            issues.append(f"неверный JSON: {str(e)[:50]}")
            print(f"  ❌ {file_relative}: {', '.join(issues)}")

    print(f"  📊 JSON файлы: {passed_files}/{total_files} прошли проверку")
    return passed_files == total_files

def main():
    """Главная функция тестирования"""
    print("🚀 Запуск тестов форматирования JXCT")
    print("=" * 50)

    tests = [
        ("C++ форматирование", test_cpp_formatting),
        ("Python форматирование", test_python_formatting),
        ("Markdown форматирование", test_markdown_formatting),
        ("JSON форматирование", test_json_formatting)
    ]

    total_tests = len(tests)
    passed_tests = 0

    for test_name, test_func in tests:
        print(f"\n📋 {test_name}:")
        try:
            if test_func():
                passed_tests += 1
                print(f"  ✅ {test_name} - ПРОЙДЕН")
            else:
                print(f"  ❌ {test_name} - ПРОВАЛЕН")
        except Exception as e:
            print(f"  ❌ {test_name} - ОШИБКА: {e}")

    print("\n" + "=" * 50)
    print(f"📊 РЕЗУЛЬТАТЫ ТЕСТИРОВАНИЯ ФОРМАТИРОВАНИЯ:")
    print(f"  Всего тестов: {total_tests}")
    print(f"  Пройдено: {passed_tests}")
    print(f"  Провалено: {total_tests - passed_tests}")
    print(f"  Успешность: {(passed_tests / total_tests * 100):.1f}%")

    if passed_tests == total_tests:
        print("  🎉 ВСЕ ТЕСТЫ ФОРМАТИРОВАНИЯ ПРОЙДЕНЫ!")
        return 0
    else:
        print("  ⚠️ НЕКОТОРЫЕ ТЕСТЫ ФОРМАТИРОВАНИЯ ПРОВАЛЕНЫ")
        return 1

if __name__ == "__main__":
    sys.exit(main())
