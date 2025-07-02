#!/usr/bin/env python3
"""
Универсальный скрипт форматирования кода для JXCT
Объединяет clang-format и auto_format.py
"""
import os
import sys
import subprocess
import platform
from pathlib import Path

def run_command(cmd, cwd=None):
    """Запуск команды с обработкой ошибок"""
    try:
        result = subprocess.run(cmd, capture_output=True, text=True, cwd=cwd)
        return result.returncode, result.stdout, result.stderr
    except Exception as e:
        return 1, "", str(e)

def run_clang_format(project_root):
    """Запуск clang-format для C++ файлов"""
    print("[CLANG-FORMAT] Formatting C++ code...")

    # Определяем команду в зависимости от ОС
    if platform.system() == "Windows":
        format_script = project_root / "scripts" / "format_all.ps1"
        if format_script.exists():
            cmd = ["powershell", "-ExecutionPolicy", "Bypass", "-File", str(format_script)]
        else:
            print("[CLANG-FORMAT] Script format_all.ps1 not found")
            return False
    else:
        # Для Linux/Mac можно использовать clang-format напрямую
        cmd = ["clang-format", "-i", "-r", str(project_root / "src"), str(project_root / "include")]

    returncode, stdout, stderr = run_command(cmd, cwd=project_root)

    if returncode == 0:
        print("[CLANG-FORMAT] Successfully completed")
        return True
    else:
        print(f"[CLANG-FORMAT] Error: {stderr}")
        return False

def run_auto_format(project_root):
    """Запуск auto_format.py для удаления trailing whitespace"""
    print("[AUTO-FORMAT] Removing trailing whitespace...")

    auto_format_script = project_root / "scripts" / "auto_format.py"
    if not auto_format_script.exists():
        print("[AUTO-FORMAT] Script auto_format.py not found")
        return False

    cmd = [sys.executable, str(auto_format_script)]
    returncode, stdout, stderr = run_command(cmd, cwd=project_root)

    if returncode == 0:
        print("[AUTO-FORMAT] Successfully completed")
        return True
    else:
        print(f"[AUTO-FORMAT] Error: {stderr}")
        return False

def main():
    """Главная функция"""
    project_root = Path(__file__).parent.parent

    print("=== JXCT Code Formatter ===")
    print(f"Project: {project_root}")
    print(f"OS: {platform.system()}")
    print()

    # 1. Форматирование C++ кода
    clang_success = run_clang_format(project_root)

    print()

    # 2. Удаление trailing whitespace
    auto_success = run_auto_format(project_root)

    print()
    print("=== RESULT ===")

    if clang_success and auto_success:
        print("[OK] All formatting operations completed successfully")
        return 0
    elif clang_success:
        print("[WARN] clang-format completed, auto_format.py skipped")
        return 0
    elif auto_success:
        print("[WARN] auto_format.py completed, clang-format skipped")
        return 0
    else:
        print("[ERROR] Formatting errors")
        return 1

if __name__ == "__main__":
    sys.exit(main())
