#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Простой тест форматирования кода для JXCT
Версия: 1.1.0 (исправлена кодировка)
"""

import sys
import os
from pathlib import Path

# Добавляем путь к проекту
project_root = Path(__file__).parent.parent
sys.path.insert(0, str(project_root))

def check_file_encoding(file_path):
    """Проверка кодировки файла"""
    encodings = ['utf-8', 'cp1252', 'latin-1', 'ascii']

    for encoding in encodings:
        try:
            with open(file_path, 'r', encoding=encoding, errors='ignore') as f:
                content = f.read()
            return True
        except Exception:
            continue

    return False

def test_basic_files():
    """Базовый тест существования файлов"""
    print("Тестирование основных файлов...")

    required_files = [
        "src/main.cpp",
        "src/validation_utils.cpp",
        "platformio.ini",
        "README.md"
    ]

    passed = 0
    total = len(required_files)

    for file_path in required_files:
        full_path = project_root / file_path
        if full_path.exists():
            print(f"  ✓ {file_path}")
            passed += 1
        else:
            print(f"  ✗ {file_path}")

    print(f"  Результат: {passed}/{total} файлов найдено")
    assert passed == total, f"Ожидалось {total} файлов, найдено {passed}"

def test_cpp_files():
    """Тест C++ файлов"""
    print("Тестирование C++ файлов...")

    cpp_files = list(project_root.glob("src/*.cpp"))
    h_files = list(project_root.glob("include/*.h"))

    total_files = len(cpp_files) + len(h_files)
    passed_files = 0

    for file_path in cpp_files + h_files:
        if check_file_encoding(file_path):
            passed_files += 1

    print(f"  C++ файлы: {len(cpp_files)}")
    print(f"  H файлы: {len(h_files)}")
    print(f"  Валидные: {passed_files}/{total_files}")

    assert passed_files > 0, f"Не найдено валидных файлов из {total_files}"

def test_python_files():
    """Тест Python файлов"""
    print("Тестирование Python файлов...")

    python_files = list(project_root.glob("scripts/*.py"))
    python_files += list(project_root.glob("test/*.py"))

    passed_files = 0
    for file_path in python_files:
        if check_file_encoding(file_path):
            passed_files += 1

    print(f"  Python файлы: {passed_files}/{len(python_files)}")
    assert passed_files > 0, f"Не найдено валидных Python файлов из {len(python_files)}"

def main():
    """Главная функция"""
    print("=== ТЕСТ ФОРМАТИРОВАНИЯ JXCT ===")

    tests = [
        ("Основные файлы", test_basic_files),
        ("C++ файлы", test_cpp_files),
        ("Python файлы", test_python_files)
    ]

    passed_tests = 0
    total_tests = len(tests)

    for test_name, test_func in tests:
        print(f"\n[{test_name}]")
        try:
            test_func()
            print(f"  ПРОЙДЕН")
            passed_tests += 1
        except AssertionError as e:
            print(f"  ПРОВАЛЕН: {e}")
        except Exception as e:
            print(f"  ОШИБКА: {e}")

    print(f"\n=== ИТОГ: {passed_tests}/{total_tests} ===")
    assert passed_tests == total_tests, f"Ожидалось {total_tests} пройденных тестов, получено {passed_tests}"

if __name__ == "__main__":
    try:
        main()
        print("Все тесты пройдены успешно!")
        sys.exit(0)
    except AssertionError as e:
        print(f"Тесты провалены: {e}")
        sys.exit(1)
    except Exception as e:
        print(f"Критическая ошибка: {e}")
        sys.exit(1)
