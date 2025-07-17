#!/usr/bin/env python3
"""
Скрипт сборки упрощённых Python-обёрток с Python C API
Использует w64devkit из настроек проекта
"""

import os
import sys
import subprocess
import platform
from pathlib import Path

def setup_compiler_path():
    """Настраивает PATH для компилятора w64devkit"""
    compiler_path = r"C:\Program Files\w64devkit\bin"
    
    if platform.system() == "Windows":
        current_path = os.environ.get("PATH", "")
        if compiler_path not in current_path:
            os.environ["PATH"] = current_path + ";" + compiler_path
            print(f"✅ Добавлен путь к компилятору: {compiler_path}")
        else:
            print(f"✅ Путь к компилятору уже настроен: {compiler_path}")
    else:
        print("⚠️ Автоматическая настройка PATH доступна только в Windows")

def get_python_include():
    """Получает путь к Python include директории"""
    import sysconfig
    return sysconfig.get_path('include')

def get_python_lib():
    """Получает путь к Python library"""
    import sysconfig
    return sysconfig.get_config_var('LIBDIR')

def build_simple_bindings():
    """Собирает упрощённые Python-обёртки"""
    project_root = Path(__file__).parent.parent
    include_path = project_root / "include"
    src_path = project_root / "src"
    python_include = get_python_include()
    bindings_path = Path(__file__).parent
    
    # Команда сборки для Windows
    cmd = [
        "g++", "-std=c++17", "-shared",
        f"-I{include_path}",
        f"-I{src_path}",
        f"-I{python_include}",
        f"-I{bindings_path}",
        "-DWIN32",
        "-D_WINDOWS",
        "-D_USRDLL",
        "-DDESKTOP_BUILD",
        "jxct_core_simple.cpp",
        "-o", "jxct_core.pyd"
    ]
    
    print(f"🔨 Сборка: {' '.join(cmd)}")
    
    try:
        result = subprocess.run(cmd, capture_output=True, text=True, encoding="utf-8", timeout=120)
        if result.returncode == 0:
            print("✅ Сборка успешна: jxct_core.pyd")
            return True
        else:
            print(f"❌ Ошибка сборки:")
            print(f"   stdout: {result.stdout}")
            print(f"   stderr: {result.stderr}")
            return False
    except subprocess.TimeoutExpired:
        print("❌ Таймаут при сборке")
        return False

def test_bindings():
    """Тестирует собранные обёртки"""
    try:
        import jxct_core
        
        # Тест валидации
        test_data = {
            "temperature": 25.0,
            "humidity": 60.0,
            "ph": 6.5,
            "ec": 1500.0
        }
        
        result = jxct_core.validate_sensor_data(test_data)
        print(f"✅ Тест валидации: {result['isValid']}")
        
        # Тест компенсации
        compensated = jxct_core.apply_compensation(test_data, 1)
        print(f"✅ Тест компенсации: pH={compensated['ph']:.2f}, EC={compensated['ec']:.1f}")
        
        # Тест рекомендаций
        recommendations = jxct_core.get_recommendations(test_data, "tomato", 1, 0)
        print(f"✅ Тест рекомендаций: {recommendations['ph_action']}")
        
        return True
        
    except ImportError as e:
        print(f"❌ Ошибка импорта: {e}")
        return False
    except Exception as e:
        print(f"❌ Ошибка тестирования: {e}")
        return False

def main():
    """Главная функция"""
    print("🔧 СБОРКА УПРОЩЁННЫХ PYTHON-ОБЁРТОК JXCT")
    print("=" * 50)
    
    # Настраиваем PATH для компилятора
    setup_compiler_path()
    
    # Собираем обёртки
    if build_simple_bindings():
        print("\n🧪 Тестирование обёрток...")
        if test_bindings():
            print("\n🎉 PYTHON-ОБЁРТКИ УСПЕШНО СОБРАНЫ И ПРОТЕСТИРОВАНЫ!")
            return 0
        else:
            print("\n⚠️ Обёртки собраны, но тестирование не прошло")
            return 1
    else:
        print("\n❌ ОШИБКА СБОРКИ PYTHON-ОБЁРТОК")
        return 1

if __name__ == "__main__":
    sys.exit(main()) 