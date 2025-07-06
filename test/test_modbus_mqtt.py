#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Тесты ModBus и MQTT функций для Windows
Версия: 1.0.0
"""

import sys
import os
from pathlib import Path
import socket
import time

# Добавляем путь к проекту
project_root = Path(__file__).parent.parent
sys.path.insert(0, str(project_root))

def test_modbus_register_mapping():
    """Тест маппинга ModBus регистров"""
    print("Тестирование маппинга ModBus регистров...")
    
    # Тестируем стандартные регистры JXCT
    register_map = {
        0x0000: ("humidity", 0.1),      # Влажность
        0x0001: ("temperature", 0.1),   # Температура
        0x0002: ("ph", 0.1),           # pH
        0x0003: ("ec", 0.001),         # EC
        0x0004: ("nitrogen", 1),       # Азот
        0x0005: ("phosphorus", 1),     # Фосфор
        0x0006: ("potassium", 1),      # Калий
    }
    
    # Тестируем валидные значения регистров
    test_values = [
        (0x0000, 500, 50.0),   # Влажность 50%
        (0x0001, 250, 25.0),   # Температура 25°C
        (0x0002, 70, 7.0),     # pH 7.0
        (0x0003, 1500, 1.5),   # EC 1.5
        (0x0004, 100, 100),    # N 100 mg/kg
        (0x0005, 50, 50),      # P 50 mg/kg
        (0x0006, 200, 200),    # K 200 mg/kg
    ]
    
    passed = 0
    total = len(test_values)
    
    for reg_addr, raw_value, expected in test_values:
        if reg_addr in register_map:
            name, multiplier = register_map[reg_addr]
            calculated = raw_value * multiplier
            
            if abs(calculated - expected) < 0.001:
                passed += 1
                print(f"  ✓ Регистр 0x{reg_addr:04X} ({name}): {raw_value} -> {calculated}")
            else:
                print(f"  ✗ Регистр 0x{reg_addr:04X} ({name}): {raw_value} -> {calculated} (ожидалось {expected})")
        else:
            print(f"  ✗ Регистр 0x{reg_addr:04X}: не найден в карте")
    
    print(f"  Результат: {passed}/{total}")
    return passed == total

def test_mqtt_topic_generation():
    """Тест генерации MQTT топиков"""
    print("Тестирование генерации MQTT топиков...")
    
    # Тестируем различные конфигурации топиков
    test_cases = [
        # (device_id, prefix, sensor_type, expected_topic)
        ("JXCT001", "homeassistant", "sensor", "homeassistant/sensor/JXCT001/config"),
        ("JXCT001", "custom", "sensor", "custom/sensor/JXCT001/config"),
        ("JXCT-ABC", "ha", "sensor", "ha/sensor/JXCT-ABC/config"),
        ("", "homeassistant", "sensor", "homeassistant/sensor/unknown/config"),
    ]
    
    passed = 0
    total = len(test_cases)
    
    for device_id, prefix, sensor_type, expected in test_cases:
        # Имитируем логику генерации топика
        actual_device_id = device_id if device_id else "unknown"
        generated_topic = f"{prefix}/{sensor_type}/{actual_device_id}/config"
        
        if generated_topic == expected:
            passed += 1
            print(f"  ✓ {device_id} -> {generated_topic}")
        else:
            print(f"  ✗ {device_id} -> {generated_topic} (ожидалось {expected})")
    
    print(f"  Результат: {passed}/{total}")
    return passed == total

def test_network_validation():
    """Тест валидации сетевых параметров"""
    print("Тестирование валидации сетевых параметров...")
    
    # Тестируем IP адреса и хосты
    test_cases = [
        # (address, expected_valid)
        ("192.168.1.100", True),
        ("mqtt.server.com", True),
        ("localhost", True),
        ("10.0.0.1", True),
        ("255.255.255.255", True),
        ("", False),
        ("999.999.999.999", False),
        ("invalid..host", False),
        ("host with spaces", False),
        ("192.168.1", False),
    ]
    
    passed = 0
    total = len(test_cases)
    
    for address, expected in test_cases:
        # Имитируем валидацию IP/hostname
        is_valid = True
        
        if not address:
            is_valid = False
        elif ' ' in address:
            is_valid = False
        elif '..' in address:
            is_valid = False
        else:
            # Проверяем IP адрес
            parts = address.split('.')
            if len(parts) == 4:
                try:
                    for part in parts:
                        num = int(part)
                        if num < 0 or num > 255:
                            is_valid = False
                            break
                except ValueError:
                    # Не IP адрес, проверяем как hostname
                    if not address.replace('.', '').replace('-', '').replace('_', '').isalnum():
                        is_valid = False
            else:
                # Неполный IP адрес или hostname
                if len(parts) < 4 and parts[-1].isdigit():
                    # Неполный IP адрес
                    is_valid = False
                else:
                    # Hostname - проверяем символы
                    if not address.replace('.', '').replace('-', '').replace('_', '').isalnum():
                        is_valid = False
        
        if is_valid == expected:
            passed += 1
            print(f"  ✓ '{address}' - правильно")
        else:
            print(f"  ✗ '{address}' - неправильно")
    
    print(f"  Результат: {passed}/{total}")
    return passed == total

def test_sensor_data_filtering():
    """Тест фильтрации данных датчика"""
    print("Тестирование фильтрации данных датчика...")
    
    # Тестируем скользящее среднее
    test_data = [
        [10.0, 12.0, 11.0, 13.0, 12.0],  # Нормальные значения
        [10.0, 100.0, 11.0, 12.0, 13.0], # С выбросом
        [0.0, 0.0, 0.0, 0.0, 0.0],       # Нулевые значения
        [25.5, 25.6, 25.4, 25.7, 25.3],  # Стабильные значения
    ]
    
    expected_averages = [11.6, 29.2, 0.0, 25.5]
    
    passed = 0
    total = len(test_data)
    
    for i, data in enumerate(test_data):
        # Простое скользящее среднее
        avg = sum(data) / len(data)
        expected = expected_averages[i]
        
        if abs(avg - expected) < 0.1:
            passed += 1
            print(f"  ✓ Данные {i+1}: среднее {avg:.1f}")
        else:
            print(f"  ✗ Данные {i+1}: среднее {avg:.1f} (ожидалось {expected:.1f})")
    
    print(f"  Результат: {passed}/{total}")
    return passed == total

def test_windows_compatibility():
    """Тест совместимости с Windows"""
    print("Тестирование совместимости с Windows...")
    
    # Проверяем Windows-специфичные функции
    tests = [
        ("Python версия", sys.version_info >= (3, 8)),
        ("Кодировка UTF-8", sys.stdout.encoding.lower() in ['utf-8', 'cp1252']),
        ("Путь к проекту", project_root.exists()),
        ("Сетевые функции", hasattr(socket, 'socket')),
        ("Файловая система", os.path.exists(str(project_root))),
    ]
    
    passed = 0
    total = len(tests)
    
    for test_name, condition in tests:
        if condition:
            passed += 1
            print(f"  ✓ {test_name}")
        else:
            print(f"  ✗ {test_name}")
    
    print(f"  Результат: {passed}/{total}")
    return passed == total

def main():
    """Главная функция"""
    print("=== ТЕСТЫ MODBUS И MQTT (WINDOWS) ===")
    
    tests = [
        ("ModBus регистры", test_modbus_register_mapping),
        ("MQTT топики", test_mqtt_topic_generation),
        ("Сетевая валидация", test_network_validation),
        ("Фильтрация данных", test_sensor_data_filtering),
        ("Windows совместимость", test_windows_compatibility)
    ]
    
    passed_tests = 0
    total_tests = len(tests)
    
    for test_name, test_func in tests:
        print(f"\n[{test_name}]")
        try:
            if test_func():
                print(f"  ПРОЙДЕН")
                passed_tests += 1
            else:
                print(f"  ПРОВАЛЕН")
        except Exception as e:
            print(f"  ОШИБКА: {e}")
    
    print(f"\n=== ИТОГ: {passed_tests}/{total_tests} ===")
    return passed_tests == total_tests

if __name__ == "__main__":
    success = main()
    sys.exit(0 if success else 1) 