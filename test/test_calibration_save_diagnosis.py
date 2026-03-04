#!/usr/bin/env python3
"""
Диагностика проблемы с сохранением калибровки
"""

import requests
import json
import time

def test_calibration_save():
    base_url = "http://192.168.2.65"
    
    print("🔍 ДИАГНОСТИКА СОХРАНЕНИЯ КАЛИБРОВКИ")
    print("=" * 50)
    
    # 1. Проверяем текущий статус
    print("\n1️⃣ Проверяем текущий статус калибровки...")
    try:
        response = requests.get(f"{base_url}/api/calibration/status", timeout=5)
        if response.status_code == 200:
            status = response.json()
            print(f"✅ Статус получен: {json.dumps(status, indent=2, ensure_ascii=False)}")
        else:
            print(f"❌ Ошибка получения статуса: {response.status_code}")
            return
    except Exception as e:
        print(f"❌ Ошибка подключения: {e}")
        return
    
    # 2. Пробуем выполнить EC калибровку
    print("\n2️⃣ Пробуем выполнить EC калибровку...")
    ec_data = {
        "expected_1": 1000.0,
        "measured_1": 950.0,
        "expected_2": 2000.0,
        "measured_2": 1900.0
    }
    
    try:
        response = requests.post(
            f"{base_url}/api/calibration/ec/calculate",
            json=ec_data,
            timeout=10
        )
        print(f"📊 Ответ EC калибровки: {response.status_code}")
        if response.status_code == 200:
            result = response.json()
            print(f"✅ Результат: {json.dumps(result, indent=2, ensure_ascii=False)}")
        else:
            print(f"❌ Ошибка: {response.text}")
    except Exception as e:
        print(f"❌ Ошибка EC калибровки: {e}")
    
    # 3. Проверяем статус после калибровки
    print("\n3️⃣ Проверяем статус после калибровки...")
    time.sleep(2)
    try:
        response = requests.get(f"{base_url}/api/calibration/status", timeout=5)
        if response.status_code == 200:
            status = response.json()
            print(f"✅ Статус после калибровки: {json.dumps(status, indent=2, ensure_ascii=False)}")
            
            # Проверяем, сохранилась ли калибровка
            if status.get('ec_calibrated'):
                print("✅ EC калибровка сохранилась!")
            else:
                print("❌ EC калибровка НЕ сохранилась!")
        else:
            print(f"❌ Ошибка получения статуса: {response.status_code}")
    except Exception as e:
        print(f"❌ Ошибка проверки статуса: {e}")
    
    # 4. Проверяем API коррекции
    print("\n4️⃣ Проверяем API коррекции...")
    try:
        response = requests.get(f"{base_url}/api/correction/settings", timeout=5)
        if response.status_code == 200:
            correction = response.json()
            print(f"✅ Настройки коррекции: {json.dumps(correction, indent=2, ensure_ascii=False)}")
        else:
            print(f"❌ Ошибка получения настроек коррекции: {response.status_code}")
    except Exception as e:
        print(f"❌ Ошибка API коррекции: {e}")

if __name__ == "__main__":
    test_calibration_save()
