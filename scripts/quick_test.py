#!/usr/bin/env python3
"""
⚡ Быстрое тестирование JXCT
Без зависаний, с таймаутами
"""

import sys
import subprocess
import time
from pathlib import Path

def run_quick_test():
    """Быстрое тестирование основных компонентов"""
    print("⚡ БЫСТРОЕ ТЕСТИРОВАНИЕ JXCT")
    print("=" * 50)
    
    tests = [
        {
            "name": "Python тесты",
            "command": "python -m pytest test/test_validation.py test/test_compensation_formulas.py -q",
            "timeout": 20
        },
        {
            "name": "Моки тесты", 
            "command": "python test/test_mock_coverage.py",
            "timeout": 15
        },
        {
            "name": "REST API тесты",
            "command": "python test/test_rest_api_integration.py", 
            "timeout": 10
        }
    ]
    
    results = []
    
    for test in tests:
        print(f"\n🔍 {test['name']}...")
        
        try:
            result = subprocess.run(
                test["command"],
                shell=True,
                capture_output=True,
                text=True,
                timeout=test["timeout"]
            )
            
            success = result.returncode == 0
            results.append(success)
            
            if success:
                print(f"   ✅ {test['name']}: УСПЕШНО")
            else:
                print(f"   ❌ {test['name']}: ОШИБКА")
                if result.stderr:
                    print(f"      {result.stderr[:100]}...")
                    
        except subprocess.TimeoutExpired:
            print(f"   ⏰ {test['name']}: ТАЙМАУТ")
            results.append(False)
        except Exception as e:
            print(f"   ❌ {test['name']}: {e}")
            results.append(False)
    
    # Итоги
    passed = sum(results)
    total = len(results)
    
    print(f"\n📊 ИТОГИ:")
    print(f"   ✅ Пройдено: {passed}/{total}")
    print(f"   📈 Успешность: {(passed/total*100):.1f}%")
    
    if passed == total:
        print(f"\n🎉 ВСЕ ТЕСТЫ ПРОЙДЕНЫ!")
        return 0
    else:
        print(f"\n⚠️ {total-passed} ТЕСТОВ ПРОВАЛЕНО")
        return 1

if __name__ == "__main__":
    sys.exit(run_quick_test()) 