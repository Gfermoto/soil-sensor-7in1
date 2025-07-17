#!/usr/bin/env python3
"""
Скрипт сборки Python-обёрток с w64devkit
Использует компилятор из настроек проекта
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

def check_compiler():
    """Проверяет доступность компилятора"""
    try:
        result = subprocess.run(["g++", "--version"],
                              capture_output=True, text=True, encoding="utf-8", timeout=10)
        if result.returncode == 0:
            print("✅ Компилятор g++ доступен")
            print(f"   Версия: {result.stdout.splitlines()[0]}")
            return True
        else:
            print("❌ Компилятор g++ недоступен")
            return False
    except FileNotFoundError:
        print("❌ Компилятор g++ не найден в PATH")
        return False
    except subprocess.TimeoutExpired:
        print("❌ Таймаут при проверке компилятора")
        return False

def build_bindings():
    """Собирает Python-обёртки с w64devkit"""
    project_root = Path(__file__).parent.parent
    include_path = project_root / "include"
    src_path = project_root / "src"
    
    # Команда сборки
    cmd = [
        "g++", "-std=c++17", "-shared", "-fPIC",
        f"-I{include_path}",
        f"-I{src_path}",
        "-I/usr/include/python3.13",  # Python headers
        "-lpython3.13",  # Python library
        "jxct_core.cpp",
        "-o", "jxct_core.pyd"  # Windows shared library
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

def main():
    """Главная функция"""
    print("🔧 СБОРКА PYTHON-ОБЁРТОК JXCT")
    print("=" * 50)
    
    # Настраиваем PATH для компилятора
    setup_compiler_path()
    
    # Проверяем доступность компилятора
    if not check_compiler():
        print("\n❌ Компилятор недоступен. Убедитесь, что w64devkit установлен.")
        return 1
    
    # Собираем обёртки
    if build_bindings():
        print("\n🎉 PYTHON-ОБЁРТКИ УСПЕШНО СОБРАНЫ!")
        return 0
    else:
        print("\n❌ ОШИБКА СБОРКИ PYTHON-ОБЁРТОК")
        return 1

if __name__ == "__main__":
    sys.exit(main()) 