#!/usr/bin/env python3
"""
Pre-commit hook для JXCT проекта
Автоматически проверяет код перед коммитом
"""

import os
import sys
import subprocess
import json
import time
from pathlib import Path
from typing import List, Dict, Tuple

class PreCommitHook:
    def __init__(self):
        self.project_root = Path(__file__).parent.parent
        self.test_reports_dir = self.project_root / "test_reports"
        self.errors = []
        self.warnings = []

    def log(self, message: str, level: str = "INFO"):
        """Логирование с временными метками"""
        timestamp = time.strftime("%H:%M:%S")
        print(f"[{timestamp}] {level}: {message}")

    def run_command(self, command: List[str], cwd: Path = None, env: dict = None) -> Tuple[int, str, str]:
        """Запуск команды и возврат результата"""
        try:
            result = subprocess.run(
                command,
                cwd=cwd or self.project_root,
                capture_output=True,
                text=True,
                timeout=300,  # 5 минут таймаут
                env=env
            )
            return result.returncode, result.stdout, result.stderr
        except subprocess.TimeoutExpired:
            return -1, "", f"Command timed out: {' '.join(command)}"
        except Exception as e:
            return -1, "", str(e)

    def check_python_syntax(self) -> bool:
        """Проверка синтаксиса Python файлов"""
        self.log("Проверка синтаксиса Python файлов...")

        python_files = list(self.project_root.rglob("*.py"))
        python_files = [f for f in python_files if "venv" not in str(f) and "site-packages" not in str(f)]

        if not python_files:
            self.log("Python файлы не найдены")
            return True

        success = True
        for py_file in python_files:
            returncode, stdout, stderr = self.run_command([sys.executable, "-m", "py_compile", str(py_file)])
            if returncode != 0:
                self.log(f"Ошибка синтаксиса в {py_file}: {stderr}", "ERROR")
                self.errors.append(f"Python syntax error in {py_file}: {stderr}")
                success = False

        if success:
            self.log(f"Синтаксис Python файлов корректен ({len(python_files)} файлов)")
        return success

    def check_cpp_syntax(self) -> bool:
        """C++ синтакс-проверка отключена для ESP32/Arduino проекта (невозможно проверить без toolchain)."""
        self.log("C++ синтакс-проверка пропущена для ESP32/Arduino файлов.")
        return True

    def run_unit_tests(self) -> bool:
        """Unit тесты отключены для ESP32/Arduino проекта (требуют PlatformIO toolchain)"""
        self.log("Unit тесты пропущены для ESP32/Arduino проекта (требуют PlatformIO toolchain)")
        self.warnings.append("Unit tests skipped - require PlatformIO toolchain")
        return True

    def run_static_analysis(self) -> bool:
        """Запуск статического анализа (без emoji, совместимо с Windows)"""
        self.log("Запуск статического анализа...")
        analysis_script = self.project_root / "scripts" / "analyze_technical_debt.py"
        if not analysis_script.exists():
            self.log("Скрипт анализа не найден", "WARNING")
            self.warnings.append("Analysis script not found")
            return True

        # Настраиваем окружение для корректной работы с Unicode в Windows
        env = os.environ.copy()
        env["PYTHONIOENCODING"] = "utf-8"

        returncode, stdout, stderr = self.run_command([
            sys.executable, str(analysis_script), "--quick"
        ], env=env)

        if returncode != 0:
            self.log(f"Статический анализ провалился: {stderr}", "ERROR")
            self.errors.append(f"Static analysis failed: {stderr}")
            return False

        # Проверяем результаты анализа (оставляем как есть)
        try:
            report_file = self.test_reports_dir / "technical-debt.json"
            if report_file.exists():
                with open(report_file, 'r', encoding='utf-8') as f:
                    report = json.load(f)
                if report.get('critical_issues', 0) > 0:
                    self.log(f"Найдены критические проблемы: {report['critical_issues']}", "ERROR")
                    self.errors.append(f"Critical issues found: {report['critical_issues']}")
                    return False
                if report.get('security_vulnerabilities', 0) > 0:
                    self.log(f"Найдены уязвимости безопасности: {report['security_vulnerabilities']}", "ERROR")
                    self.errors.append(f"Security vulnerabilities found: {report['security_vulnerabilities']}")
                    return False
        except Exception as e:
            self.log(f"Ошибка чтения отчёта анализа: {e}", "WARNING")
            self.warnings.append(f"Failed to read analysis report: {e}")
        self.log("Статический анализ завершён успешно")
        return True

    def check_file_sizes(self) -> bool:
        """Проверка размеров файлов"""
        self.log("Проверка размеров файлов...")

        max_file_size = 1024 * 1024  # 1MB
        large_files = []

        for file_path in self.project_root.rglob("*"):
            if file_path.is_file() and file_path.stat().st_size > max_file_size:
                # Исключаем некоторые типы файлов
                if not any(exclude in str(file_path) for exclude in [
                    '.git', 'node_modules', 'venv', 'site-packages',
                    '.pytest_cache', '__pycache__', 'build', 'dist'
                ]):
                    large_files.append(file_path)

        if large_files:
            self.log(f"Найдены большие файлы (>1MB):", "WARNING")
            for file_path in large_files:
                size_mb = file_path.stat().st_size / (1024 * 1024)
                self.log(f"  {file_path} ({size_mb:.1f}MB)", "WARNING")
                self.warnings.append(f"Large file: {file_path} ({size_mb:.1f}MB)")

        return True

    def check_trailing_whitespace(self) -> bool:
        """Проверка на trailing whitespace"""
        self.log("Проверка на trailing whitespace...")

        text_extensions = {'.py', '.cpp', '.h', '.md', '.txt', '.json', '.yml', '.yaml'}
        files_with_whitespace = []

        # Исключаем директории зависимостей
        exclude_dirs = {'.venv', '.pio', 'node_modules', '__pycache__', '.git', 'site-packages'}

        for file_path in self.project_root.rglob("*"):
            if file_path.is_file() and file_path.suffix in text_extensions:
                # Пропускаем файлы в исключённых директориях
                if any(exclude_dir in str(file_path) for exclude_dir in exclude_dirs):
                    continue

                try:
                    with open(file_path, 'r', encoding='utf-8') as f:
                        for line_num, line in enumerate(f, 1):
                            if line.rstrip() != line:
                                files_with_whitespace.append((file_path, line_num))
                                break
                except Exception:
                    continue

        if files_with_whitespace:
            self.log("Найдены файлы с trailing whitespace:", "WARNING")
            for file_path, line_num in files_with_whitespace:
                self.log(f"  {file_path}:{line_num}", "WARNING")
                self.warnings.append(f"Trailing whitespace in {file_path}:{line_num}")

        return True

    def check_line_lengths(self) -> bool:
        """Проверка длины строк"""
        self.log("Проверка длины строк...")

        max_line_length = 120
        text_extensions = {'.py', '.cpp', '.h', '.md', '.txt', '.json', '.yml', '.yaml'}
        files_with_long_lines = []

        # Исключаем директории зависимостей
        exclude_dirs = {'.venv', '.pio', 'node_modules', '__pycache__', '.git', 'site-packages'}

        for file_path in self.project_root.rglob("*"):
            if file_path.is_file() and file_path.suffix in text_extensions:
                # Пропускаем файлы в исключённых директориях
                if any(exclude_dir in str(file_path) for exclude_dir in exclude_dirs):
                    continue

                try:
                    with open(file_path, 'r', encoding='utf-8') as f:
                        for line_num, line in enumerate(f, 1):
                            if len(line.rstrip()) > max_line_length:
                                files_with_long_lines.append((file_path, line_num, len(line.rstrip())))
                                break
                except Exception:
                    continue

        if files_with_long_lines:
            self.log("Найдены файлы с длинными строками (>120 символов):", "WARNING")
            for file_path, line_num, length in files_with_long_lines:
                self.log(f"  {file_path}:{line_num} ({length} chars)", "WARNING")
                self.warnings.append(f"Long line in {file_path}:{line_num} ({length} chars)")

        return True

    def check_commit_message(self) -> bool:
        """Проверка сообщения коммита"""
        self.log("Проверка сообщения коммита...")

        commit_msg_file = os.environ.get('COMMIT_EDITMSG')
        if not commit_msg_file or not os.path.exists(commit_msg_file):
            self.log("Файл сообщения коммита не найден", "WARNING")
            return True

        try:
            with open(commit_msg_file, 'r', encoding='utf-8') as f:
                commit_message = f.read()

            lines = commit_message.strip().split('\n')
            if not lines:
                self.log("Пустое сообщение коммита", "ERROR")
                self.errors.append("Empty commit message")
                return False

            first_line = lines[0].strip()

            # Проверяем длину первой строки
            if len(first_line) > 72:
                self.log("Первая строка сообщения коммита слишком длинная (>72 символов)", "WARNING")
                self.warnings.append("Commit message first line too long")

            # Проверяем наличие описания
            if len(lines) < 2 or not lines[1].strip():
                self.log("Отсутствует описание коммита", "WARNING")
                self.warnings.append("No commit description")

            # Проверяем формат
            if not first_line:
                self.log("Пустая первая строка сообщения коммита", "ERROR")
                self.errors.append("Empty first line in commit message")
                return False

        except Exception as e:
            self.log(f"Ошибка чтения сообщения коммита: {e}", "WARNING")
            self.warnings.append(f"Failed to read commit message: {e}")

        self.log("Сообщение коммита проверено")
        return True

    def generate_report(self) -> str:
        """Генерация отчёта о проверках"""
        report = []
        report.append("=" * 60)
        report.append("ОТЧЁТ PRE-COMMIT HOOK")
        report.append("=" * 60)

        if self.errors:
            report.append("\n❌ ОШИБКИ:")
            for error in self.errors:
                report.append(f"  • {error}")

        if self.warnings:
            report.append("\n⚠️  ПРЕДУПРЕЖДЕНИЯ:")
            for warning in self.warnings:
                report.append(f"  • {warning}")

        if not self.errors and not self.warnings:
            report.append("\n✅ Все проверки пройдены успешно!")

        report.append("\n" + "=" * 60)
        return "\n".join(report)

    def run_all_checks(self) -> bool:
        """Запуск всех проверок"""
        self.log("Запуск pre-commit проверок...")

        # Универсальное форматирование кода (clang-format + auto_format)
        self.log("Универсальное форматирование кода...")
        format_script = self.project_root / "scripts" / "format_code.py"
        if format_script.exists():
            try:
                returncode, stdout, stderr = self.run_command([
                    sys.executable, str(format_script)
                ])
                if returncode != 0:
                    self.log(f"format_code.py завершился с ошибкой: {stderr}", "WARNING")
                    self.warnings.append("format_code.py failed")
                else:
                    self.log("format_code.py успешно выполнен")
            except Exception as e:
                self.log(f"Ошибка запуска format_code.py: {e}", "WARNING")
                self.warnings.append("format_code.py error")
        else:
            self.log("Скрипт format_code.py не найден", "WARNING")
            self.warnings.append("format_code.py not found")

        checks = [
            ("Проверка синтаксиса Python", self.check_python_syntax),
            ("Проверка синтаксиса C++", self.check_cpp_syntax),
            ("Unit тесты", self.run_unit_tests),
            ("Статический анализ", self.run_static_analysis),
            ("Проверка размеров файлов", self.check_file_sizes),
            ("Проверка trailing whitespace", self.check_trailing_whitespace),
            ("Проверка длины строк", self.check_line_lengths),
            ("Проверка сообщения коммита", self.check_commit_message),
        ]

        all_passed = True

        for check_name, check_func in checks:
            try:
                self.log(f"Выполняется: {check_name}")
                if not check_func():
                    all_passed = False
                    self.log(f"❌ {check_name} провалился", "ERROR")
                else:
                    self.log(f"✅ {check_name} пройден", "INFO")
            except Exception as e:
                self.log(f"❌ {check_name} завершился с ошибкой: {e}", "ERROR")
                self.errors.append(f"{check_name} failed: {e}")
                all_passed = False

        # Выводим финальный отчёт
        print("\n" + self.generate_report())

        return all_passed

def main():
    """Основная функция"""
    hook = PreCommitHook()

    try:
        success = hook.run_all_checks()

        if not success:
            print("\n❌ Pre-commit проверки провалились!")
            print("Исправьте ошибки и попробуйте снова.")
            sys.exit(1)
        else:
            print("\n✅ Pre-commit проверки пройдены успешно!")
            sys.exit(0)

    except KeyboardInterrupt:
        print("\n\n❌ Pre-commit проверки прерваны пользователем")
        sys.exit(1)
    except Exception as e:
        print(f"\n❌ Неожиданная ошибка в pre-commit hook: {e}")
        sys.exit(1)

if __name__ == "__main__":
    main()
