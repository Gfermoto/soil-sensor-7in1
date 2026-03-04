#!/usr/bin/env python3
"""
🧪 Тест логики обработки данных JXCT
Проверяет правильность применения калибровки и научной компенсации
"""

import requests
import json
import time

def test_data_processing():
    """Тестирует логику обработки данных"""
    print("🧪 ТЕСТ ЛОГИКИ ОБРАБОТКИ ДАННЫХ JXCT")
    print("=" * 60)
    
    # IP адрес ESP32
    base_url = "http://192.168.2.65"
    
    print(f"🎯 Тестируем на: {base_url}")
    print()
    
    # Тест 1: Проверка статуса калибровки
    print("1️⃣ СТАТУС КАЛИБРОВКИ")
    try:
        response = requests.get(f"{base_url}/api/calibration/status", timeout=10)
        if response.status_code == 200:
            data = response.json()
            print(f"📊 Статус калибровки: {json.dumps(data, indent=2, ensure_ascii=False)}")
            
            # Анализируем статус
            calibration_enabled = data.get("calibration_enabled", False)
            ph_calibrated = data.get("ph_calibrated", False)
            ec_calibrated = data.get("ec_calibrated", False)
            npk_calibrated = data.get("npk_calibrated", False)
            
            print(f"🔧 Калибровка включена: {calibration_enabled}")
            print(f"🧪 pH откалиброван: {ph_calibrated}")
            print(f"⚡ EC откалиброван: {ec_calibrated}")
            print(f"🌿 NPK откалиброван: {npk_calibrated}")
            
        else:
            print(f"❌ Ошибка получения статуса: {response.status_code}")
            
    except Exception as e:
        print(f"❌ Ошибка запроса: {e}")
    
    # Тест 2: Проверка данных датчика
    print("\n2️⃣ ДАННЫЕ ДАТЧИКА")
    try:
        response = requests.get(f"{base_url}/api/sensor/data", timeout=10)
        if response.status_code == 200:
            data = response.json()
            print(f"📊 Данные датчика: {json.dumps(data, indent=2, ensure_ascii=False)}")
            
            # Анализируем данные
            if "temperature" in data:
                print(f"🌡️ Температура: {data['temperature']}°C")
            if "humidity" in data:
                print(f"💧 Влажность: {data['humidity']}%")
            if "ec" in data:
                print(f"⚡ EC: {data['ec']} μS/cm")
            if "ph" in data:
                print(f"🧪 pH: {data['ph']}")
            if "nitrogen" in data:
                print(f"🌿 Азот: {data['nitrogen']} mg/kg")
            if "phosphorus" in data:
                print(f"🌿 Фосфор: {data['phosphorus']} mg/kg")
            if "potassium" in data:
                print(f"🌿 Калий: {data['potassium']} mg/kg")
                
        else:
            print(f"❌ Ошибка получения данных: {response.status_code}")
            
    except Exception as e:
        print(f"❌ Ошибка запроса: {e}")
    
    # Тест 3: Проверка конфигурации
    print("\n3️⃣ КОНФИГУРАЦИЯ")
    try:
        response = requests.get(f"{base_url}/api/config", timeout=10)
        if response.status_code == 200:
            data = response.json()
            
            # Проверяем флаги обработки
            calibration_enabled = data.get("calibration_enabled", False)
            compensation_enabled = data.get("compensation_enabled", False)
            
            print(f"🔧 Калибровка включена: {calibration_enabled}")
            print(f"🔬 Компенсация включена: {compensation_enabled}")
            
            # Анализируем логику
            print("\n📋 АНАЛИЗ ЛОГИКИ ОБРАБОТКИ:")
            if calibration_enabled and compensation_enabled:
                print("✅ Калибровка + Компенсация: Используются калиброванные данные для научной компенсации")
            elif compensation_enabled:
                print("✅ Только компенсация: Используются сырые данные для научной компенсации")
            elif calibration_enabled:
                print("✅ Только калибровка: Применяется только калибровка")
            else:
                print("❌ Обработка отключена: Используются сырые данные")
                
        else:
            print(f"❌ Ошибка получения конфигурации: {response.status_code}")
            
    except Exception as e:
        print(f"❌ Ошибка запроса: {e}")
    
    print("\n📋 РЕКОМЕНДАЦИИ:")
    print("• Проверьте логи ESP32 для детальной информации")
    print("• Убедитесь, что калибровка и компенсация включены в настройках")
    print("• Проверьте статус калибровки каждого параметра")

if __name__ == "__main__":
    test_data_processing()
