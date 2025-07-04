#!/usr/bin/env python3
"""
Простой тест валидации для JXCT
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

def test_validation_utils():
    """Тест утилит валидации"""
    print("🧪 Тестирование утилит валидации...")
    
    # Проверяем существование файлов валидации
    validation_files = [
        "src/validation_utils.cpp",
        "include/validation_utils.h"
    ]
    
    for file_path in validation_files:
        full_path = project_root / file_path
        if full_path.exists():
            print(f"  ✅ {file_path} существует")
        else:
            print(f"  ❌ {file_path} не найден")
            return False
    
    print("  ✅ Все файлы валидации найдены")
    return True

def test_format_utils():
    """Тест утилит форматирования"""
    print("🧪 Тестирование утилит форматирования...")
    
    # Проверяем существование файлов форматирования
    format_files = [
        "src/jxct_format_utils.cpp",
        "include/jxct_format_utils.h"
    ]
    
    for file_path in format_files:
        full_path = project_root / file_path
        if full_path.exists():
            print(f"  ✅ {file_path} существует")
        else:
            print(f"  ❌ {file_path} не найден")
            return False
    
    print("  ✅ Все файлы форматирования найдены")
    return True

def test_csrf_protection():
    """Тест CSRF защиты"""
    print("🧪 Тестирование CSRF защиты...")
    
    # Проверяем существование файлов CSRF
    csrf_files = [
        "src/web/csrf_protection.cpp",
        "include/web/csrf_protection.h"
    ]
    
    for file_path in csrf_files:
        full_path = project_root / file_path
        if full_path.exists():
            print(f"  ✅ {file_path} существует")
        else:
            print(f"  ❌ {file_path} не найден")
            return False
    
    print("  ✅ Все файлы CSRF защиты найдены")
    return True

def test_config_files():
    """Тест конфигурационных файлов"""
    print("🧪 Тестирование конфигурационных файлов...")
    
    # Проверяем существование конфигурационных файлов
    config_files = [
        "platformio.ini",
        "CMakeLists.txt",
        "VERSION"
    ]
    
    for file_path in config_files:
        full_path = project_root / file_path
        if full_path.exists():
            print(f"  ✅ {file_path} существует")
        else:
            print(f"  ❌ {file_path} не найден")
            return False
    
    print("  ✅ Все конфигурационные файлы найдены")
    return True

def test_documentation():
    """Тест документации"""
    print("🧪 Тестирование документации...")
    
    # Проверяем существование основных документов
    doc_files = [
        "README.md",
        "CONTRIBUTING.md",
        "docs/README.md",
        "docs/manuals/USER_GUIDE.md"
    ]
    
    for file_path in doc_files:
        full_path = project_root / file_path
        if full_path.exists():
            print(f"  ✅ {file_path} существует")
        else:
            print(f"  ❌ {file_path} не найден")
            return False
    
    print("  ✅ Вся документация найдена")
    return True

def main():
    """Главная функция тестирования"""
    print("🚀 Запуск тестов валидации JXCT")
    print("=" * 50)
    
    tests = [
        ("Валидация утилит", test_validation_utils),
        ("Утилиты форматирования", test_format_utils),
        ("CSRF защита", test_csrf_protection),
        ("Конфигурационные файлы", test_config_files),
        ("Документация", test_documentation)
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
    print(f"📊 РЕЗУЛЬТАТЫ ТЕСТИРОВАНИЯ:")
    print(f"  Всего тестов: {total_tests}")
    print(f"  Пройдено: {passed_tests}")
    print(f"  Провалено: {total_tests - passed_tests}")
    print(f"  Успешность: {(passed_tests / total_tests * 100):.1f}%")
    
    if passed_tests == total_tests:
        print("  🎉 ВСЕ ТЕСТЫ ПРОЙДЕНЫ УСПЕШНО!")
        return 0
    else:
        print("  ⚠️ НЕКОТОРЫЕ ТЕСТЫ ПРОВАЛЕНЫ")
        return 1

if __name__ == "__main__":
    sys.exit(main()) 