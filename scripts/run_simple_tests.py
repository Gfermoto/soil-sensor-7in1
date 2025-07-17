#!/usr/bin/env python3
"""
🚀 Улучшенный скрипт запуска тестов JXCT
Без зависаний и с автоматическим завершением
"""

import sys
import subprocess
import time
from pathlib import Path

def run_command_safe(command, timeout=60, description=""):
    """Безопасный запуск команды с таймаутом"""
    print(f"[INFO] {description}...")
    
    try:
        # Запускаем команду с таймаутом и без интерактивности
        result = subprocess.run(
            command,
            shell=True,
            capture_output=True,
            text=True,
            timeout=timeout,
            cwd=Path.cwd()
        )
        
        if result.returncode == 0:
            print(f"[SUCCESS] ✅ {description}: УСПЕШНО")
            return True, result.stdout
        else:
            print(f"[ERROR] ❌ {description}: ОШИБКА")
            print(f"   Ошибка: {result.stderr}")
            return False, result.stderr
            
    except subprocess.TimeoutExpired:
        print(f"[ERROR] ❌ {description}: ТАЙМАУТ ({timeout}с)")
        return False, "Timeout"
    except Exception as e:
        print(f"[ERROR] ❌ {description}: ИСКЛЮЧЕНИЕ - {e}")
        return False, str(e)

def main():
    """Главная функция"""
    print("🚀 JXCT УЛУЧШЕННОЕ ТЕСТИРОВАНИЕ")
    print("=" * 60)
    
    results = {}
    
    # 1. Python тесты (быстро)
    print("\n📋 ЭТАП 1: Python тесты")
    success, output = run_command_safe(
        "python -m pytest test/test_validation.py test/test_compensation_formulas.py -q",
        timeout=30,
        description="Python тесты валидации и компенсации"
    )
    results["python_tests"] = success
    
    # 2. Тесты с моками (быстро)
    print("\n📋 ЭТАП 2: Тесты с моками")
    success, output = run_command_safe(
        "python test/test_mock_coverage.py",
        timeout=30,
        description="Тесты покрытия с моками"
    )
    results["mock_tests"] = success
    
    # 3. REST API тесты (быстро)
    print("\n📋 ЭТАП 3: REST API тесты")
    success, output = run_command_safe(
        "python test/test_rest_api_integration.py",
        timeout=30,
        description="REST API интеграционные тесты"
    )
    results["rest_api_tests"] = success
    
    # 4. Сборка ESP32 (может занять время)
    print("\n📋 ЭТАП 4: Сборка ESP32")
    success, output = run_command_safe(
        "pio run -e esp32dev --silent",
        timeout=120,  # 2 минуты на сборку
        description="Сборка ESP32"
    )
    results["esp32_build"] = success
    
    if success:
        # Показываем размер прошивки
        firmware_path = Path(".pio/build/esp32dev/firmware.bin")
        if firmware_path.exists():
            size = firmware_path.stat().st_size
            print(f"   📦 Размер прошивки: {size:,} байт")
    
    # 5. Статический анализ (быстро)
    print("\n📋 ЭТАП 5: Статический анализ")
    success, output = run_command_safe(
        "python scripts/run_clang_tidy_analysis.py --quick",
        timeout=60,
        description="Clang-tidy анализ (быстрый режим)"
    )
    results["static_analysis"] = success
    
    # Итоги
    print("\n" + "=" * 60)
    print("📊 ИТОГИ ТЕСТИРОВАНИЯ:")
    
    total_tests = len(results)
    passed_tests = sum(results.values())
    failed_tests = total_tests - passed_tests
    
    for test_name, success in results.items():
        status = "✅ PASS" if success else "❌ FAIL"
        print(f"[{status}] {test_name}")
    
    print(f"\n📈 СТАТИСТИКА:")
    print(f"   ✅ Пройдено: {passed_tests}")
    print(f"   ❌ Провалено: {failed_tests}")
    print(f"   📊 Всего: {total_tests}")
    print(f"   📈 Успешность: {(passed_tests/total_tests*100):.1f}%")
    
    if failed_tests == 0:
        print(f"\n🎉 ВСЕ ТЕСТЫ ПРОЙДЕНЫ УСПЕШНО!")
        return 0
    else:
        print(f"\n⚠️ {failed_tests} ТЕСТОВ ПРОВАЛЕНО")
        return 1

if __name__ == "__main__":
    exit_code = main()
    sys.exit(exit_code)
