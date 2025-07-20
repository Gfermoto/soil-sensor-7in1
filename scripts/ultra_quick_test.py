#!/usr/bin/env python3
"""
⚡ УЛЬТРА-БЫСТРОЕ ТЕСТИРОВАНИЕ JXCT
Без зависаний, только основные тесты
"""

import sys
import subprocess
from pathlib import Path

def test_validation():
    """Тест валидации"""
    try:
        result = subprocess.run(
            [sys.executable, "-c", "import sys; sys.path.append('test'); import test_validation; print('OK')"],
            capture_output=False, text=True, timeout=10, cwd=Path(__file__).parent.parent
        )
        return result.returncode == 0
    except Exception:
        return False

def test_compensation():
    """Тест компенсации"""
    try:
        result = subprocess.run(
            [sys.executable, "-c", "import sys; sys.path.append('test'); import test_compensation_formulas; print('OK')"],
            capture_output=False, text=True, timeout=10, cwd=Path(__file__).parent.parent
        )
        return result.returncode == 0
    except Exception:
        return False

def test_build():
    """Проверка сборки"""
    try:
        # Проверяем наличие прошивки
        firmware = Path(".pio/build/esp32dev/firmware.bin")
        if firmware.exists():
            size = firmware.stat().st_size
            print(f"   📦 Размер: {size:,} байт")
            return size > 500000  # Минимум 500KB
        else:
            print("   ⚠️ Прошивка не найдена")
            return False
    except Exception as e:
        print(f"   ❌ Ошибка: {e}")
        return False

def test_adaptive_filters():
    """Тест адаптивных фильтров"""
    try:
        result = subprocess.run(
            [sys.executable, "test/test_adaptive_filters.py"],
            capture_output=False, text=True, timeout=10, cwd=Path(__file__).parent.parent
        )
        return result.returncode == 0
    except Exception:
        return False



def main():
    print("⚡ УЛЬТРА-БЫСТРОЕ ТЕСТИРОВАНИЕ")
    print("=" * 40)
    
    tests = [
        ("Валидация", test_validation),
        ("Компенсация", test_compensation),
        ("Адаптивные фильтры", test_adaptive_filters),
        ("Сборка", test_build)
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