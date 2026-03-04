#!/usr/bin/env python3
"""
🔬 Тест API калибровки JXCT
Проверяет доступность и работоспособность API калибровки
"""

import requests
import json
import time

def test_calibration_api():
    """Тестирует API калибровки"""
    print("🔬 ТЕСТ API КАЛИБРОВКИ JXCT")
    print("=" * 50)
    
    # IP адрес ESP32 (замените на ваш)
    base_url = "http://192.168.2.65"  # STA режим - подключен к eyera
    # base_url = "http://192.168.4.1"  # AP режим
    
    endpoints = [
        "/api/calibration/status",
        "/api/calibration/ph",
        "/api/calibration/ec",
        "/api/calibration/temperature",
        "/api/calibration/humidity",
        "/api/calibration/npk"
    ]
    
    print(f"🎯 Тестируем API на: {base_url}")
    print()
    
    # Тест 1: Проверка доступности сервера
    print("1️⃣ ПРОВЕРКА ДОСТУПНОСТИ СЕРВЕРА")
    try:
        response = requests.get(f"{base_url}/", timeout=5)
        print(f"✅ Сервер отвечает: {response.status_code}")
    except requests.exceptions.RequestException as e:
        print(f"❌ Сервер недоступен: {e}")
        return False
    
    # Тест 2: Проверка статуса калибровки
    print("\n2️⃣ ТЕСТ СТАТУСА КАЛИБРОВКИ")
    try:
        response = requests.get(f"{base_url}/api/calibration/status", timeout=10)
        print(f"📡 Статус ответа: {response.status_code}")
        print(f"📄 Заголовки: {dict(response.headers)}")
        
        if response.status_code == 200:
            try:
                data = response.json()
                print(f"📊 Данные: {json.dumps(data, indent=2, ensure_ascii=False)}")
                
                if data.get("success"):
                    print("✅ API статуса калибровки работает корректно")
                else:
                    print("⚠️ API вернул success=false")
                    
            except json.JSONDecodeError as e:
                print(f"❌ Ошибка парсинга JSON: {e}")
                print(f"📄 Сырой ответ: {response.text}")
        else:
            print(f"❌ Неожиданный статус: {response.status_code}")
            print(f"📄 Ответ: {response.text}")
            
    except requests.exceptions.RequestException as e:
        print(f"❌ Ошибка запроса: {e}")
    
    # Тест 3: Проверка других endpoints
    print("\n3️⃣ ПРОВЕРКА ДРУГИХ ENDPOINTS")
    for endpoint in endpoints[1:]:  # Пропускаем status
        try:
            response = requests.get(f"{base_url}{endpoint}", timeout=5)
            print(f"✅ {endpoint}: {response.status_code}")
        except requests.exceptions.RequestException as e:
            print(f"❌ {endpoint}: {e}")
    
    print("\n📋 РЕКОМЕНДАЦИИ:")
    print("• Если сервер недоступен - проверьте IP адрес")
    print("• Если статус 404 - проверьте регистрацию маршрутов")
    print("• Если JSON не парсится - проверьте формат ответа")
    print("• Если success=false - проверьте инициализацию gSensorCorrection")

if __name__ == "__main__":
    test_calibration_api()
