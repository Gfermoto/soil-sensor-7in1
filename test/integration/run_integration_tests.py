#!/usr/bin/env python3
"""
Скрипт для запуска интеграционных тестов C++ в Windows
Автоматически настраивает PATH для компилятора w64devkit
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

def compile_test(test_file, output_name):
    """Компилирует тестовый файл"""
    project_root = Path(__file__).parent.parent.parent
    include_path = project_root / "include"
    src_path = project_root / "src"
    test_stubs_path = project_root / "test" / "stubs"
    unity_path = project_root / "test" / "stubs" / "Unity"
    unity_src_path = project_root / "test" / "stubs" / "Unity" / "src"
    unity_c = unity_src_path / "unity.c"
    cmd = [
        "g++", "-std=c++17",
        f"-I{include_path}",
        f"-I{src_path}",
        f"-I{test_stubs_path}",
        f"-I{unity_path}",
        f"-I{unity_src_path}",
        str(test_file),
        str(unity_c),
        "-o", output_name
    ]
    print(f"🔨 Компиляция: {' '.join(cmd)}")
    try:
        result = subprocess.run(cmd, capture_output=True, text=True, encoding="utf-8", timeout=60)
        if result.returncode == 0:
            print(f"✅ Компиляция успешна: {output_name}")
            return True
        else:
            print(f"❌ Ошибка компиляции:")
            print(f"   stdout: {result.stdout}")
            print(f"   stderr: {result.stderr}")
            return False
    except subprocess.TimeoutExpired:
        print("❌ Таймаут при компиляции")
        return False

def run_test(executable):
    """Запускает скомпилированный тест"""
    print(f"🚀 Запуск теста: {executable}")

    try:
        result = subprocess.run([f"./{executable}"],
                              capture_output=True, text=True, encoding="utf-8", timeout=30)
        print(f"📊 Результат выполнения:")
        print(f"   Код возврата: {result.returncode}")
        print(f"   stdout:\n{result.stdout}")
        if result.stderr:
            print(f"   stderr:\n{result.stderr}")

        return result.returncode == 0
    except subprocess.TimeoutExpired:
        print("❌ Таймаут при выполнении теста")
        return False

def main():
    """Главная функция"""
    print("🧪 ЗАПУСК ИНТЕГРАЦИОННЫХ ТЕСТОВ JXCT")
    print("=" * 50)

    # Настраиваем PATH для компилятора
    setup_compiler_path()

    # Проверяем доступность компилятора
    if not check_compiler():
        print("\n❌ Компилятор недоступен. Убедитесь, что w64devkit установлен.")
        print("   Путь: C:\\Program Files\\w64devkit\\bin")
        return 1

    # Список тестов для компиляции
    tests = [
        ("test_component_interaction.cpp", "test_component_interaction"),
        ("test_web_api_integration.cpp", "test_web_api_integration")
    ]

    success_count = 0
    total_count = len(tests)

    for test_file, output_name in tests:
        test_path = Path(__file__).parent / test_file

        if not test_path.exists():
            print(f"⚠️ Файл теста не найден: {test_file}")
            continue

        print(f"\n📋 Тест: {test_file}")
        print("-" * 30)

        # Компилируем тест
        if compile_test(test_path, output_name):
            # Запускаем тест
            if run_test(output_name):
                success_count += 1
                print(f"✅ Тест {test_file} ПРОЙДЕН")
            else:
                print(f"❌ Тест {test_file} ПРОВАЛЕН")
        else:
            print(f"❌ Ошибка компиляции {test_file}")

    # Итоговый результат
    print(f"\n📊 ИТОГОВЫЙ РЕЗУЛЬТАТ:")
    print(f"   Пройдено: {success_count}/{total_count}")
    print(f"   Успешность: {(success_count/total_count)*100:.1f}%")

    if success_count == total_count:
        print("🎉 ВСЕ ИНТЕГРАЦИОННЫЕ ТЕСТЫ ПРОЙДЕНЫ!")
        return 0
    else:
        print("⚠️ НЕКОТОРЫЕ ТЕСТЫ ПРОВАЛЕНЫ")
        return 1

if __name__ == "__main__":
    sys.exit(main())
