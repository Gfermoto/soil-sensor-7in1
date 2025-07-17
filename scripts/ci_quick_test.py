#!/usr/bin/env python3
"""
⚡ CI-БЕЗОПАСНОЕ ТЕСТИРОВАНИЕ JXCT
Версия для GitHub Actions без проверки сборки
"""

import sys
import subprocess
from pathlib import Path

def test_validation():
    """Тест валидации"""
    try:
        result = subprocess.run(
            "python -c \"import sys; sys.path.append('test'); import test_validation; print('OK')\"",
            shell=True, capture_output=True, text=True, timeout=10
        )
        return result.returncode == 0
    except:
        return False

def test_compensation():
    """Тест компенсации"""
    try:
        result = subprocess.run(
            "python -c \"import sys; sys.path.append('test'); import test_compensation_formulas; print('OK')\"",
            shell=True, capture_output=True, text=True, timeout=10
        )
        return result.returncode == 0
    except:
        return False

def test_critical():
    """Критические тесты"""
    try:
        result = subprocess.run(
            "python -c \"import sys; sys.path.append('test'); import test_critical_validation; print('OK')\"",
            shell=True, capture_output=True, text=True, timeout=10
        )
        return result.returncode == 0
    except:
        return False

def test_format():
    """Тест форматирования"""
    try:
        result = subprocess.run(
            "python -c \"import sys; sys.path.append('test'); import test_format; print('OK')\"",
            shell=True, capture_output=True, text=True, timeout=10
        )
        return result.returncode == 0
    except:
        return False

def main():
    print("⚡ CI-БЕЗОПАСНОЕ ТЕСТИРОВАНИЕ")
    print("=" * 40)
    
    tests = [
        ("Валидация", test_validation),
        ("Компенсация", test_compensation),
        ("Критические", test_critical),
        ("Форматирование", test_format)
    ]
    
    results = []
    
    for name, test_func in tests:
        print(f"🔍 {name}...", end=" ")
        try:
            success = test_func()
            results.append(success)
            print("✅" if success else "❌")
        except Exception as e:
            results.append(False)
            print("❌")
    
    passed = sum(results)
    total = len(results)
    
    print(f"\n📊 ИТОГИ: {passed}/{total} ✅")
    
    if passed == total:
        print("🎉 ВСЕ ТЕСТЫ ПРОЙДЕНЫ!")
        return 0
    else:
        print(f"⚠️ {total-passed} ТЕСТОВ ПРОВАЛЕНО")
        return 1

if __name__ == "__main__":
    sys.exit(main()) 