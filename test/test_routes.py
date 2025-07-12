#!/usr/bin/env python3
"""
Тест маршрутов JXCT
Проверяет наличие обязательных API маршрутов
"""

import os
import re
import sys
import unittest

# Принудительно устанавливаем stdout в utf-8 для Windows
if hasattr(sys.stdout, 'encoding') and sys.stdout.encoding and sys.stdout.encoding.lower() not in ['utf-8', 'utf8']:
    try:
        sys.stdout = open(sys.stdout.fileno(), mode='w', encoding='utf-8', buffering=1)
    except Exception:
        pass

# Каталоги с исходниками
SRC_DIRS = [
    os.path.join(os.path.dirname(__file__), os.pardir, 'src', 'web'),
]

# Список обязательных маршрутов и файлов, в которых они должны встречаться
REQUIRED_ROUTES = {
    '/api/config/import': 'routes_config.cpp',
    '/api/v1/config/export': 'routes_config.cpp',
}

def test_required_routes_exist():
    """Тест наличия обязательных маршрутов"""
    print("🔗 ТЕСТ МАРШРУТОВ JXCT")
    print("=" * 40)
    
    passed = 0
    total = len(REQUIRED_ROUTES)
    
    for route, expected_file in REQUIRED_ROUTES.items():
        found = False
        for src_dir in SRC_DIRS:
            if os.path.exists(src_dir):
                for root, _dirs, files in os.walk(src_dir):
                    for fname in files:
                        if fname.endswith('.cpp') and expected_file in fname:
                            path = os.path.join(root, fname)
                            try:
                                with open(path, 'r', encoding='utf-8') as f:
                                    content = f.read()
                                    if route in content:
                                        found = True
                                        print(f"  ✅ Маршрут '{route}' найден в {fname}")
                                        passed += 1
                                        break
                            except Exception as e:
                                print(f"  ❌ Ошибка чтения {fname}: {e}")
                    if found:
                        break
                if found:
                    break
        
        if not found:
            print(f"  ❌ Маршрут '{route}' НЕ найден в {expected_file}")
    
    print(f"\n📊 Результат: {passed}/{total}")
    print(f"🎯 {'✅ ТЕСТ ПРОЙДЕН' if passed == total else '❌ ТЕСТ ПРОВАЛЕН'}")
    
    assert passed == total, f"Не все обязательные маршруты найдены: {passed}/{total}"

if __name__ == '__main__':
    success = test_required_routes_exist()
    sys.exit(0 if success else 1)
