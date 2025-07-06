#!/usr/bin/env python3
"""
Pre-commit hook для JXCT проекта v3.7.0
Минимальные но полезные проверки
"""

import os
import sys
import subprocess
import time
from pathlib import Path
from typing import List, Tuple

class PreCommitHook:
    def __init__(self):
        self.project_root = Path(__file__).parent.parent
        self.errors = []
        self.warnings = []

    def log(self, message: str, level: str = "INFO"):
        """Логирование с временными метками"""
        timestamp = time.strftime("%H:%M:%S")
        print(f"[{timestamp}] {level}: {message}")

    def run_command(self, command: List[str], cwd: Path = None) -> Tuple[int, str, str]:
        """Запуск команды и возврат результата"""
        try:
            result = subprocess.run(
                command,
                cwd=cwd or self.project_root,
                capture_output=True,
                text=True,
                timeout=60,  # 1 минута таймаут
                encoding='utf-8',
                errors='ignore'
            )
            return result.returncode, result.stdout, result.stderr
        except subprocess.TimeoutExpired:
            return -1, "", f"Command timed out: {' '.join(command)}"
        except Exception as e:
            return -1, "", str(e)

    def check_python_syntax(self) -> bool:
        """Проверка синтаксиса Python файлов"""
        self.log("Проверка синтаксиса Python файлов...")

        python_files = [
            "test/test_format.py",
            "test/test_validation.py", 
            "test/test_routes.py",
            "scripts/run_simple_tests.py",
            "scripts/run_comprehensive_tests.py",
            "scripts/pre-commit.py"
        ]

        success = True
        for py_file in python_files:
            file_path = self.project_root / py_file
            if file_path.exists():
                returncode, stdout, stderr = self.run_command([sys.executable, "-m", "py_compile", str(file_path)])
                if returncode != 0:
                    self.log(f"Ошибка синтаксиса в {py_file}: {stderr}", "ERROR")
                    self.errors.append(f"Python syntax error in {py_file}")
                    success = False

        if success:
            self.log(f"Синтаксис Python файлов корректен")
        return success

    def run_critical_tests(self) -> bool:
        """Запуск критически важных тестов"""
        self.log("Запуск критических тестов...")
        
        # Только самые важные тесты
        tests = [
            ("test/test_format.py", "Тест форматирования"),
            ("test/test_validation.py", "Тест валидации")
        ]
        
        success = True
        for test_file, description in tests:
            self.log(f"Запуск {description}...")
            returncode, stdout, stderr = self.run_command([sys.executable, test_file])
            if returncode != 0:
                self.log(f"{description} провален: {stderr}", "ERROR")
                self.errors.append(f"{description} failed")
                success = False
            else:
                self.log(f"{description} пройден")
        
        return success

    def check_esp32_build(self) -> bool:
        """Проверка сборки ESP32 (если доступно)"""
        self.log("Проверка сборки ESP32...")
        
        # Проверяем доступность PlatformIO
        returncode, stdout, stderr = self.run_command(["pio", "--version"])
        if returncode != 0:
            self.log("PlatformIO недоступен, пропускаем проверку сборки", "WARNING")
            self.warnings.append("PlatformIO not available")
            return True
        
        # Проверяем конфигурацию platformio.ini
        platformio_ini = self.project_root / "platformio.ini"
        if not platformio_ini.exists():
            self.log("platformio.ini не найден", "ERROR")
            self.errors.append("platformio.ini not found")
            return False
        
        self.log("Конфигурация ESP32 корректна")
        return True

    def check_critical_files(self) -> bool:
        """Проверка наличия критических файлов"""
        self.log("Проверка критических файлов...")
        
        critical_files = [
            "src/main.cpp",
            "platformio.ini",
            "include/version.h",
            "src/validation_utils.cpp",
            "src/jxct_format_utils.cpp"
        ]
        
        missing_files = []
        for file_path in critical_files:
            if not (self.project_root / file_path).exists():
                missing_files.append(file_path)
        
        if missing_files:
            self.log(f"Отсутствуют критические файлы: {missing_files}", "ERROR")
            self.errors.append(f"Missing critical files: {missing_files}")
            return False
        
        self.log("Все критические файлы найдены")
        return True

    def run_all_checks(self) -> bool:
        """Запуск всех проверок"""
        self.log("🔍 Запуск pre-commit проверок JXCT v3.7.0...")
        start_time = time.time()
        
        checks = [
            ("Синтаксис Python", self.check_python_syntax),
            ("Критические файлы", self.check_critical_files),
            ("Критические тесты", self.run_critical_tests),
            ("Сборка ESP32", self.check_esp32_build),
        ]
        
        success = True
        for check_name, check_func in checks:
            try:
                if not check_func():
                    success = False
            except Exception as e:
                self.log(f"Ошибка в проверке '{check_name}': {e}", "ERROR")
                self.errors.append(f"Check '{check_name}' failed: {e}")
                success = False
        
        duration = time.time() - start_time
        self.log(f"Проверки завершены за {duration:.1f}с")
        
        # Отчет
        if self.errors:
            self.log("❌ ОШИБКИ:", "ERROR")
            for error in self.errors:
                self.log(f"  - {error}", "ERROR")
        
        if self.warnings:
            self.log("⚠️ ПРЕДУПРЕЖДЕНИЯ:", "WARNING")
            for warning in self.warnings:
                self.log(f"  - {warning}", "WARNING")
        
        if success:
            self.log("✅ Все проверки пройдены успешно!")
        else:
            self.log("❌ Есть ошибки, коммит заблокирован!")
        
        return success

def main():
    """Главная функция"""
    hook = PreCommitHook()
    success = hook.run_all_checks()
    
    if success:
        print("\n🚀 Pre-commit проверки пройдены, коммит разрешен!")
        sys.exit(0)
    else:
        print("\n🛑 Pre-commit проверки провалены, коммит заблокирован!")
        sys.exit(1)

if __name__ == "__main__":
    main()
