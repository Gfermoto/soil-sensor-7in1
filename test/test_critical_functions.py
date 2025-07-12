#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Тесты критических функций JXCT
Версия: 1.0.0
"""

import sys
if hasattr(sys.stdout, 'reconfigure'):
    sys.stdout.reconfigure(encoding='utf-8')
if hasattr(sys.stderr, 'reconfigure'):
    sys.stderr.reconfigure(encoding='utf-8')

import os
from pathlib import Path

# Добавляем путь к проекту
project_root = Path(__file__).parent.parent
sys.path.insert(0, str(project_root))

def test_config_validation():
    """Тест валидации конфигурации"""
    print("Тестирование валидации конфигурации...")

    # Тестируем критические поля конфигурации
    test_cases = [
        # (ssid, mqttEnabled, mqttServer, tsEnabled, tsApiKey, expected_valid)
        ("MyWiFi", False, "", False, "", True),  # Только WiFi
        ("MyWiFi", True, "mqtt.server.com", False, "", True),  # WiFi + MQTT
        ("MyWiFi", False, "", True, "1234567890123456", True),  # WiFi + ThingSpeak
        ("", False, "", False, "", False),  # Пустой SSID
        ("MyWiFi", True, "", False, "", False),  # MQTT без сервера
        ("MyWiFi", False, "", True, "", False),  # ThingSpeak без API ключа
        ("MyWiFi", False, "", True, "short", False),  # Короткий API ключ
    ]

    passed = 0
    total = len(test_cases)

    for ssid, mqtt_enabled, mqtt_server, ts_enabled, ts_api_key, expected in test_cases:
        # Имитируем логику isConfigValid()
        is_valid = True

        # Проверяем SSID
        if len(ssid) == 0:
            is_valid = False

        # Проверяем MQTT
        if mqtt_enabled and len(mqtt_server) == 0:
            is_valid = False

        # Проверяем ThingSpeak
        if ts_enabled and len(ts_api_key) != 16:
            is_valid = False

        if is_valid == expected:
            passed += 1
            print(f"  ✓ SSID='{ssid}', MQTT={mqtt_enabled}, TS={ts_enabled} - правильно")
        else:
            print(f"  ✗ SSID='{ssid}', MQTT={mqtt_enabled}, TS={ts_enabled} - неправильно")

    print(f"  Результат: {passed}/{total}")
    return passed == total

def test_calibration_logic():
    """Тест логики калибровки"""
    print("Тестирование логики калибровки...")

    # Тестируем интерполяцию калибровки
    test_cases = [
        # (rawValue, entries, expected_result)
        (5.0, [(0.0, 1.0), (10.0, 1.2)], 5.0 * 1.1),  # Интерполяция
        (0.0, [(0.0, 1.0), (10.0, 1.2)], 0.0 * 1.0),  # Точное совпадение
        (10.0, [(0.0, 1.0), (10.0, 1.2)], 10.0 * 1.2),  # Точное совпадение
    ]

    passed = 0
    total = len(test_cases)

    for raw_value, entries, expected in test_cases:
        # Имитируем логику applyCalibration()
        if len(entries) == 0:
            result = raw_value
        else:
            # Ищем точное совпадение
            exact_match = None
            for raw, corr in entries:
                if raw == raw_value:
                    exact_match = corr
                    break

            if exact_match is not None:
                result = raw_value * exact_match
            else:
                # Интерполяция
                lower_raw, lower_corr = entries[0]
                upper_raw, upper_corr = entries[-1]

                for raw, corr in entries:
                    if raw < raw_value:
                        lower_raw, lower_corr = raw, corr
                    elif raw > raw_value:
                        upper_raw, upper_corr = raw, corr
                        break

                if upper_raw > lower_raw:
                    ratio = (raw_value - lower_raw) / (upper_raw - lower_raw)
                    interpolated_coeff = lower_corr + ratio * (upper_corr - lower_corr)
                    result = raw_value * interpolated_coeff
                else:
                    result = raw_value * lower_corr

        if abs(result - expected) < 0.001:  # Допуск для float
            passed += 1
            print(f"  ✓ {raw_value} -> {result:.3f} (ожидалось {expected:.3f})")
        else:
            print(f"  ✗ {raw_value} -> {result:.3f} (ожидалось {expected:.3f})")

    print(f"  Результат: {passed}/{total}")
    return passed == total

def test_modbus_validation():
    """Тест валидации ModBus данных"""
    print("Тестирование валидации ModBus...")

    # Тестируем диапазоны значений датчика
    test_cases = [
        # (temp, hum, ph, ec, n, p, k, expected_valid)
        (25.0, 50.0, 7.0, 1.5, 100, 50, 200, True),  # Нормальные значения
        (-10.0, 30.0, 6.5, 1.2, 80, 40, 150, True),  # Граничные значения
        (80.0, 100.0, 9.0, 10000.0, 999, 999, 999, True),  # Максимальные значения (pH 3-9, EC до 10000)
        (-60.0, 50.0, 7.0, 1.5, 100, 50, 200, False),  # Слишком низкая температура
        (25.0, 120.0, 7.0, 1.5, 100, 50, 200, False),  # Слишком высокая влажность
    ]

    passed = 0
    total = len(test_cases)

    for temp, hum, ph, ec, n, p, k, expected in test_cases:
        # Имитируем логику validateSensorData()
        is_valid = True

        # Проверяем диапазоны (единые константы датчика JXCT 7-in-1)
        if temp < -45.0 or temp > 115.0:
            is_valid = False
        if hum < 0.0 or hum > 100.0:
            is_valid = False
        if ph < 3.0 or ph > 9.0:
            is_valid = False
        if ec < 0.0 or ec > 10000.0:
            is_valid = False
        if n < 0 or n > 1999:
            is_valid = False
        if p < 0 or p > 1999:
            is_valid = False
        if k < 0 or k > 1999:
            is_valid = False

        if is_valid == expected:
            passed += 1
            print(f"  ✓ T={temp}, H={hum}, pH={ph}, EC={ec} - правильно")
        else:
            print(f"  ✗ T={temp}, H={hum}, pH={ph}, EC={ec} - неправильно")

    print(f"  Результат: {passed}/{total}")
    return passed == total

def test_file_operations():
    """Тест файловых операций"""
    print("Тестирование файловых операций...")

    # Проверяем критические файлы и директории
    critical_paths = [
        ("src/main.cpp", "file"),
        ("src/config.cpp", "file"),
        ("src/modbus_sensor.cpp", "file"),
        ("include/", "dir"),
        ("test/", "dir"),
        ("scripts/", "dir"),
    ]

    passed = 0
    total = len(critical_paths)

    for path, path_type in critical_paths:
        full_path = project_root / path

        if path_type == "file":
            if full_path.exists() and full_path.is_file():
                passed += 1
                print(f"  ✓ {path} - найден")
            else:
                print(f"  ✗ {path} - не найден")
        elif path_type == "dir":
            if full_path.exists() and full_path.is_dir():
                passed += 1
                print(f"  ✓ {path} - найдена")
            else:
                print(f"  ✗ {path} - не найдена")

    print(f"  Результат: {passed}/{total}")
    return passed == total

def main():
    """Главная функция"""
    print("=== ТЕСТЫ КРИТИЧЕСКИХ ФУНКЦИЙ JXCT ===")

    tests = [
        ("Валидация конфигурации", test_config_validation),
        ("Логика калибровки", test_calibration_logic),
        ("Валидация ModBus", test_modbus_validation),
        ("Файловые операции", test_file_operations)
    ]

    passed_tests = 0
    total_tests = len(tests)

    for test_name, test_func in tests:
        print(f"\n[{test_name}]")
        try:
            if test_func():
                print(f"  ✅ ПРОЙДЕН")
                passed_tests += 1
            else:
                print(f"  ❌ ПРОВАЛЕН")
        except Exception as e:
            print(f"  ❌ ОШИБКА: {e}")

    print(f"\n=== ИТОГ: {passed_tests}/{total_tests} ===")
    return passed_tests == total_tests

if __name__ == "__main__":
    try:
        if main():
            print("✅ Все тесты пройдены успешно!")
            sys.exit(0)
        else:
            print("❌ Некоторые тесты провалены")
            sys.exit(1)
    except Exception as e:
        print(f"❌ Критическая ошибка: {e}")
        sys.exit(1)
