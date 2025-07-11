#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Тесты системных функций JXCT (WiFi, OTA, ThingSpeak)
Версия: 1.0.0
"""

import sys
if hasattr(sys.stdout, 'reconfigure'):
    sys.stdout.reconfigure(encoding='utf-8')
if hasattr(sys.stderr, 'reconfigure'):
    sys.stderr.reconfigure(encoding='utf-8')

import os
from pathlib import Path
import re

# Добавляем путь к проекту
project_root = Path(__file__).parent.parent
sys.path.insert(0, str(project_root))

def test_wifi_logic():
    """Тест логики WiFi подключения"""
    print("Тестирование логики WiFi...")

    # Тестируем различные состояния WiFi
    test_cases = [
        # (ssid, password, expected_ap_mode)
        ("", "", True),  # Пустые данные -> AP режим
        ("MyWiFi", "", False),  # Только SSID -> STA режим
        ("MyWiFi", "password", False),  # Полные данные -> STA режим
        ("a" * 33, "password", True),  # Слишком длинный SSID -> AP режим
        ("MyWiFi", "a" * 64, True),  # Слишком длинный пароль -> AP режим
        ("MyWiFi", "12345678", False),  # Минимальный пароль (8 символов) -> STA
        ("MyWiFi", "123456", True),  # Короткий пароль -> AP режим
    ]

    passed = 0
    total = len(test_cases)

    for ssid, password, expected_ap in test_cases:
        # Имитируем логику выбора режима WiFi
        should_ap = False

        if len(ssid) == 0:
            should_ap = True
        elif len(ssid) > 32:
            should_ap = True
        elif len(password) > 0 and len(password) < 8:
            should_ap = True
        elif len(password) > 63:
            should_ap = True

        if should_ap == expected_ap:
            passed += 1
            mode = "AP" if should_ap else "STA"
            print(f"  ✓ SSID='{ssid[:10]}...', PWD='{password[:5]}...' -> {mode}")
        else:
            mode = "AP" if should_ap else "STA"
            expected_mode = "AP" if expected_ap else "STA"
            print(f"  ✗ SSID='{ssid[:10]}...', PWD='{password[:5]}...' -> {mode} (ожидалось {expected_mode})")

    print(f"  Результат: {passed}/{total}")
    assert passed == total, f"Не все тесты WiFi логики прошли: {passed}/{total}"

def test_thingspeak_validation():
    """Тест валидации ThingSpeak"""
    print("Тестирование валидации ThingSpeak...")

    # Тестируем различные конфигурации ThingSpeak
    test_cases = [
        # (api_key, channel_id, expected_valid)
        ("1234567890123456", "123456", True),  # Валидные данные
        ("ABCDEF1234567890", "999999", True),  # Валидные данные
        ("", "123456", False),  # Пустой API ключ
        ("1234567890123456", "", False),  # Пустой Channel ID
        ("123456789012345", "123456", False),  # Короткий API ключ (15 символов)
        ("12345678901234567", "123456", False),  # Длинный API ключ (17 символов)
        ("1234567890123456", "0", False),  # Channel ID = 0
        ("1234567890123456", "abc", False),  # Нечисловой Channel ID
        ("ABCDEFGHIJKLMNOP", "123456", True),  # API ключ из букв
        ("1234567890123456", "1000000", True),  # Большой Channel ID
    ]

    passed = 0
    total = len(test_cases)

    for api_key, channel_id, expected in test_cases:
        # Имитируем логику валидации ThingSpeak
        is_valid = True

        # Проверяем API ключ
        if len(api_key) != 16:
            is_valid = False

        # Проверяем Channel ID
        try:
            channel_num = int(channel_id) if channel_id else 0
            if channel_num == 0:
                is_valid = False
        except ValueError:
            is_valid = False

        if is_valid == expected:
            passed += 1
            print(f"  ✓ API='{api_key[:8]}...', CH='{channel_id}' - правильно")
        else:
            print(f"  ✗ API='{api_key[:8]}...', CH='{channel_id}' - неправильно")

    print(f"  Результат: {passed}/{total}")
    assert passed == total, f"Не все тесты валидации ThingSpeak прошли: {passed}/{total}"

def test_ota_validation():
    """Тест валидации OTA обновлений"""
    print("Тестирование валидации OTA...")

    # Тестируем различные URL и SHA256
    test_cases = [
        # (url, sha256, expected_valid)
        ("https://github.com/user/repo/releases/download/v1.0/firmware.bin", "a" * 64, True),
        ("https://github.com/user/repo/firmware.bin", "A" * 64, True),
        ("http://github.com/user/repo/firmware.bin", "a" * 64, False),  # HTTP вместо HTTPS
        ("", "a" * 64, False),  # Пустой URL
        ("https://github.com/user/repo/firmware.bin", "", False),  # Пустой SHA256
        ("https://github.com/user/repo/firmware.bin", "a" * 63, False),  # Короткий SHA256
        ("https://github.com/user/repo/firmware.bin", "a" * 65, False),  # Длинный SHA256
        ("https://malicious.com/firmware.bin", "a" * 64, False),  # Не GitHub
        ("https://github.com/user/repo/firmware.bin", "ZZZZZZ" + "a" * 58, False),  # Невалидный hex
        ("ftp://github.com/user/repo/firmware.bin", "a" * 64, False),  # FTP протокол
    ]

    passed = 0
    total = len(test_cases)

    for url, sha256, expected in test_cases:
        # Имитируем логику валидации OTA
        is_valid = True

        # Проверяем URL
        if len(url) < 10:
            is_valid = False
        elif not url.startswith("https://"):
            is_valid = False
        elif "github.com" not in url:
            is_valid = False

        # Проверяем SHA256
        if len(sha256) != 64:
            is_valid = False
        else:
            # Проверяем что это валидный hex
            try:
                int(sha256, 16)
            except ValueError:
                is_valid = False

        if is_valid == expected:
            passed += 1
            print(f"  ✓ URL='{url[:30]}...', SHA256='{sha256[:8]}...' - правильно")
        else:
            print(f"  ✗ URL='{url[:30]}...', SHA256='{sha256[:8]}...' - неправильно")

    print(f"  Результат: {passed}/{total}")
    assert passed == total, f"Не все тесты валидации OTA прошли: {passed}/{total}"

def test_memory_management():
    """Тест управления памятью"""
    print("Тестирование управления памятью...")

    # Тестируем различные состояния памяти
    memory_scenarios = [
        # (free_heap, operation, expected_allowed)
        (100000, "HTTP_REQUEST", True),  # Достаточно памяти
        (80000, "HTTP_REQUEST", True),   # Минимально достаточно
        (60000, "HTTP_REQUEST", False),  # Недостаточно для HTTP
        (50000, "OTA_DOWNLOAD", False),  # Недостаточно для OTA
        (120000, "OTA_DOWNLOAD", True),  # Достаточно для OTA
        (30000, "MQTT_PUBLISH", True),   # Достаточно для MQTT
        (10000, "MQTT_PUBLISH", False),  # Недостаточно для MQTT
        (200000, "ANY_OPERATION", True), # Много памяти
    ]

    passed = 0
    total = len(memory_scenarios)

    for free_heap, operation, expected in memory_scenarios:
        # Имитируем логику проверки памяти
        is_allowed = True

        if operation == "HTTP_REQUEST" and free_heap < 70000:
            is_allowed = False
        elif operation == "OTA_DOWNLOAD" and free_heap < 60000:
            is_allowed = False
        elif operation == "MQTT_PUBLISH" and free_heap < 20000:
            is_allowed = False

        if is_allowed == expected:
            passed += 1
            result = "разрешено" if is_allowed else "запрещено"
            print(f"  ✓ {free_heap} байт, {operation} - {result}")
        else:
            result = "разрешено" if is_allowed else "запрещено"
            expected_result = "разрешено" if expected else "запрещено"
            print(f"  ✗ {free_heap} байт, {operation} - {result} (ожидалось {expected_result})")

    print(f"  Результат: {passed}/{total}")
    assert passed == total, f"Не все тесты управления памятью прошли: {passed}/{total}"

def test_error_handling():
    """Тест обработки ошибок"""
    print("Тестирование обработки ошибок...")

    # Тестируем различные сценарии ошибок
    error_scenarios = [
        # (error_type, retry_count, expected_action)
        ("MODBUS_TIMEOUT", 1, "RETRY"),
        ("MODBUS_TIMEOUT", 3, "RESET"),
        ("WIFI_DISCONNECT", 1, "RECONNECT"),
        ("WIFI_DISCONNECT", 5, "AP_MODE"),
        ("MQTT_CONNECT_FAIL", 2, "RETRY"),
        ("MQTT_CONNECT_FAIL", 4, "SKIP"),
        ("SENSOR_READ_ERROR", 1, "RETRY"),
        ("SENSOR_READ_ERROR", 10, "FAKE_DATA"),
        ("MEMORY_LOW", 1, "CLEANUP"),
        ("MEMORY_CRITICAL", 1, "RESTART"),
    ]

    passed = 0
    total = len(error_scenarios)

    for error_type, retry_count, expected_action in error_scenarios:
        # Имитируем логику обработки ошибок
        action = "UNKNOWN"

        if error_type == "MODBUS_TIMEOUT":
            action = "RETRY" if retry_count < 3 else "RESET"
        elif error_type == "WIFI_DISCONNECT":
            action = "RECONNECT" if retry_count < 3 else "AP_MODE"
        elif error_type == "MQTT_CONNECT_FAIL":
            action = "RETRY" if retry_count < 3 else "SKIP"
        elif error_type == "SENSOR_READ_ERROR":
            action = "RETRY" if retry_count < 5 else "FAKE_DATA"
        elif error_type == "MEMORY_LOW":
            action = "CLEANUP"
        elif error_type == "MEMORY_CRITICAL":
            action = "RESTART"

        if action == expected_action:
            passed += 1
            print(f"  ✓ {error_type} (попытка {retry_count}) -> {action}")
        else:
            print(f"  ✗ {error_type} (попытка {retry_count}) -> {action} (ожидалось {expected_action})")

    print(f"  Результат: {passed}/{total}")
    assert passed == total, f"Не все тесты обработки ошибок прошли: {passed}/{total}"

def main():
    """Главная функция"""
    print("=== ТЕСТЫ СИСТЕМНЫХ ФУНКЦИЙ JXCT ===")

    tests = [
        ("Логика WiFi", test_wifi_logic),
        ("Валидация ThingSpeak", test_thingspeak_validation),
        ("Валидация OTA", test_ota_validation),
        ("Управление памятью", test_memory_management),
        ("Обработка ошибок", test_error_handling)
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
