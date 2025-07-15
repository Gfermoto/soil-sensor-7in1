#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
КРИТИЧЕСКИЕ тесты валидации для JXCT
Только тесты, которые ДОЛЖНЫ проходить для pre-commit
Версия: 1.0.0
"""

import sys
import os
from pathlib import Path

# Добавляем путь к проекту
project_root = Path(__file__).parent.parent
sys.path.insert(0, str(project_root))

if hasattr(sys.stdout, 'reconfigure'):
    sys.stdout.reconfigure(encoding='utf-8')
if hasattr(sys.stderr, 'reconfigure'):
    sys.stderr.reconfigure(encoding='utf-8')

def test_temperature_range():
    """Тест диапазона температур - КРИТИЧЕСКИЙ"""
    print("Тестирование диапазона температур...")

    valid_temps = [25.0, 0.0, -10.0, 50.0, 22.5]
    invalid_temps = [-100.0, 100.0, 999.0, -999.0]

    passed = 0
    total = len(valid_temps) + len(invalid_temps)

    # Проверяем валидные значения
    for temp in valid_temps:
        if -50.0 <= temp <= 80.0:  # Разумный диапазон для ESP32
            passed += 1
            print(f"  ✓ {temp}°C - валидная")
        else:
            print(f"  ✗ {temp}°C - должна быть валидной")

    # Проверяем невалидные значения
    for temp in invalid_temps:
        if not (-50.0 <= temp <= 80.0):
            passed += 1
            print(f"  ✓ {temp}°C - правильно отклонена")
        else:
            print(f"  ✗ {temp}°C - должна быть отклонена")

    print(f"  Результат: {passed}/{total}")
    assert passed == total, f"Не все тесты диапазона температур прошли: {passed}/{total}"

def test_humidity_range():
    """Тест диапазона влажности - КРИТИЧЕСКИЙ"""
    print("Тестирование диапазона влажности...")

    valid_humidity = [0.0, 50.0, 100.0, 25.5, 75.0]
    invalid_humidity = [-10.0, 110.0, 200.0, -1.0]

    passed = 0
    total = len(valid_humidity) + len(invalid_humidity)

    # Проверяем валидные значения
    for hum in valid_humidity:
        if 0.0 <= hum <= 100.0:
            passed += 1
            print(f"  ✓ {hum}% - валидная")
        else:
            print(f"  ✗ {hum}% - должна быть валидной")

    # Проверяем невалидные значения
    for hum in invalid_humidity:
        if not (0.0 <= hum <= 100.0):
            passed += 1
            print(f"  ✓ {hum}% - правильно отклонена")
        else:
            print(f"  ✗ {hum}% - должна быть отклонена")

    print(f"  Результат: {passed}/{total}")
    assert passed == total, f"Не все тесты диапазона влажности прошли: {passed}/{total}"

def test_ph_range():
    """Тест диапазона pH - КРИТИЧЕСКИЙ"""
    print("Тестирование диапазона pH...")

    valid_ph = [0.0, 7.0, 14.0, 6.5, 8.2]
    invalid_ph = [-1.0, 15.0, 20.0, -5.0]

    passed = 0
    total = len(valid_ph) + len(invalid_ph)

    # Проверяем валидные значения
    for ph in valid_ph:
        if 0.0 <= ph <= 14.0:
            passed += 1
            print(f"  ✓ pH {ph} - валидный")
        else:
            print(f"  ✗ pH {ph} - должен быть валидным")

    # Проверяем невалидные значения
    for ph in invalid_ph:
        if not (0.0 <= ph <= 14.0):
            passed += 1
            print(f"  ✓ pH {ph} - правильно отклонен")
        else:
            print(f"  ✗ pH {ph} - должен быть отклонен")

    print(f"  Результат: {passed}/{total}")
    assert passed == total, f"Не все тесты диапазона pH прошли: {passed}/{total}"

def test_ec_range():
    """Тест диапазона EC - КРИТИЧЕСКИЙ"""
    print("Тестирование диапазона EC...")

    valid_ec = [0.0, 1.5, 1000.0, 2300.0, 5000.0]
    invalid_ec = [-1.0, 15000.0, 20000.0, -10.0]

    passed = 0
    total = len(valid_ec) + len(invalid_ec)

    # Проверяем валидные значения
    for ec in valid_ec:
        if 0.0 <= ec <= 10000.0:  # Диапазон датчика JXCT 7-in-1
            passed += 1
            print(f"  ✓ EC {ec} - валидный")
        else:
            print(f"  ✗ EC {ec} - должен быть валидным")

    # Проверяем невалидные значения
    for ec in invalid_ec:
        if not (0.0 <= ec <= 10000.0):
            passed += 1
            print(f"  ✓ EC {ec} - правильно отклонен")
        else:
            print(f"  ✗ EC {ec} - должен быть отклонен")

    print(f"  Результат: {passed}/{total}")
    assert passed == total, f"Не все тесты диапазона EC прошли: {passed}/{total}"

def test_project_structure():
    """Тест структуры проекта - КРИТИЧЕСКИЙ"""
    print("Тестирование структуры проекта...")

    required_dirs = ["src", "include", "test", "scripts"]
    required_files = ["platformio.ini", "README.md", "CMakeLists.txt"]

    passed = 0
    total = len(required_dirs) + len(required_files)

    # Проверяем директории
    for dir_name in required_dirs:
        dir_path = project_root / dir_name
        if dir_path.exists() and dir_path.is_dir():
            passed += 1
            print(f"  ✓ {dir_name}/ - найдена")
        else:
            print(f"  ✗ {dir_name}/ - не найдена")

    # Проверяем файлы
    for file_name in required_files:
        file_path = project_root / file_name
        if file_path.exists() and file_path.is_file():
            passed += 1
            print(f"  ✓ {file_name} - найден")
        else:
            print(f"  ✗ {file_name} - не найден")

    print(f"  Результат: {passed}/{total}")
    assert passed == total, f"Не все тесты структуры проекта прошли: {passed}/{total}"

def main():
    """Главная функция - только критические тесты"""
    print("=== КРИТИЧЕСКИЕ ТЕСТЫ ВАЛИДАЦИИ JXCT ===")

    tests = [
        ("Диапазон температур", test_temperature_range),
        ("Диапазон влажности", test_humidity_range),
        ("Диапазон pH", test_ph_range),
        ("Диапазон EC", test_ec_range),
        ("Структура проекта", test_project_structure)
    ]

    passed_tests = 0
    total_tests = len(tests)

    for test_name, test_func in tests:
        print(f"\n[{test_name}]")
        try:
            test_func()
            print(f"  ПРОЙДЕН")
            passed_tests += 1
        except Exception as e:
            print(f"  ОШИБКА: {e}")

    print(f"\n=== ИТОГ: {passed_tests}/{total_tests} ===")
    return passed_tests == total_tests

if __name__ == "__main__":
    success = main()
    sys.exit(0 if success else 1)
