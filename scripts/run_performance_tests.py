#!/usr/bin/env python3
"""
Скрипт для запуска тестов производительности JXCT
Использует PlatformIO native environment
"""

import os
import sys
import subprocess
import json
import time
import argparse
from pathlib import Path
from typing import Dict, List, Any

class PerformanceTestRunner:
    def __init__(self, verbose: bool = False):
        self.project_root = Path(__file__).parent.parent
        self.test_dir = self.project_root / "test" / "performance"
        self.reports_dir = self.project_root / "test_reports"
        self.verbose = verbose

        # Создаём директорию для отчётов
        self.reports_dir.mkdir(exist_ok=True)

    def log(self, message: str, level: str = "INFO"):
        """Логирование с временными метками"""
        timestamp = time.strftime("%H:%M:%S")
        print(f"[{timestamp}] {level}: {message}")

    def run_command(self, command: List[str], cwd: Path = None) -> tuple[int, str, str]:
        """Запуск команды и возврат результата"""
        try:
            if self.verbose:
                print(f"Running: {' '.join(command)}")

            result = subprocess.run(
                command,
                cwd=cwd or self.project_root,
                capture_output=not self.verbose,
                text=True,
                timeout=600  # 10 минут таймаут
            )
            return result.returncode, result.stdout, result.stderr
        except subprocess.TimeoutExpired:
            return -1, "", f"Command timed out: {' '.join(command)}"
        except Exception as e:
            return -1, "", str(e)

    def compile_performance_tests(self) -> bool:
        """Компиляция тестов производительности через PlatformIO"""
        self.log("Компиляция тестов производительности через PlatformIO...")

        # Проверяем наличие тестов
        test_file = self.test_dir / "test_performance.cpp"
        if not test_file.exists():
            self.log("Файл тестов производительности не найден", "WARNING")
            return False

        # Компилируем через PlatformIO native environment
        compile_command = [
            "pio", "test", "-e", "native", "-f", "performance"
        ]

        returncode, stdout, stderr = self.run_command(compile_command)

        if returncode != 0:
            self.log(f"Ошибка компиляции через PlatformIO: {stderr}", "ERROR")
            return False

        self.log("Тесты производительности скомпилированы успешно через PlatformIO")
        return True

    def run_performance_tests(self) -> Dict[str, Any]:
        """Запуск тестов производительности"""
        self.log("Запуск тестов производительности...")

        # Запускаем через PlatformIO
        run_command = [
            "pio", "test", "-e", "native", "-f", "performance"
        ]

        returncode, stdout, stderr = self.run_command(run_command)

        if returncode != 0:
            self.log(f"Тесты производительности провалились: {stderr}", "ERROR")
            return {}

        # Парсим результаты
        results = self.parse_performance_results(stdout)
        self.log("Тесты производительности завершены")

        return results

    def parse_performance_results(self, output: str) -> Dict[str, Any]:
        """Парсинг результатов тестов производительности"""
        results = {
            "timestamp": time.strftime("%Y-%m-%d %H:%M:%S"),
            "tests": {},
            "summary": {
                "total_tests": 0,
                "passed_tests": 0,
                "failed_tests": 0,
                "performance_metrics": {}
            }
        }

        lines = output.split('\n')
        current_test = None

        for line in lines:
            line = line.strip()

            # Ищем названия тестов
            if "Performance:" in line:
                current_test = line.split("Performance:")[0].strip()
                results["tests"][current_test] = {
                    "status": "passed",
                    "metrics": {},
                    "details": []
                }
                results["summary"]["total_tests"] += 1

            # Ищем метрики производительности
            elif current_test and ":" in line and ("ms" in line or "per second" in line):
                try:
                    key, value = line.split(":", 1)
                    key = key.strip()
                    value = value.strip()

                    # Извлекаем числовые значения
                    if "ms" in value:
                        numeric_value = float(value.split()[0])
                        results["tests"][current_test]["metrics"][key] = {
                            "value": numeric_value,
                            "unit": "ms"
                        }
                    elif "per second" in value:
                        numeric_value = float(value.split()[0])
                        results["tests"][current_test]["metrics"][key] = {
                            "value": numeric_value,
                            "unit": "ops/sec"
                        }

                except (ValueError, IndexError):
                    pass

            # Ищем детали тестов
            elif current_test and line:
                results["tests"][current_test]["details"].append(line)

        # Подсчитываем результаты
        for test_name, test_data in results["tests"].items():
            if test_data["status"] == "passed":
                results["summary"]["passed_tests"] += 1
            else:
                results["summary"]["failed_tests"] += 1

        # Вычисляем общие метрики производительности
        all_metrics = []
        for test_data in results["tests"].values():
            for metric_name, metric_data in test_data["metrics"].items():
                if metric_data["unit"] == "ms":
                    all_metrics.append(metric_data["value"])

        if all_metrics:
            results["summary"]["performance_metrics"] = {
                "average_response_time": sum(all_metrics) / len(all_metrics),
                "min_response_time": min(all_metrics),
                "max_response_time": max(all_metrics),
                "total_operations": len(all_metrics)
            }

        return results

    def run_system_benchmarks(self) -> Dict[str, Any]:
        """Запуск системных бенчмарков"""
        self.log("Запуск системных бенчмарков...")

        benchmarks = {
            "timestamp": time.strftime("%Y-%m-%d %H:%M:%S"),
            "system_info": self.get_system_info(),
            "benchmarks": {}
        }

        # Тест CPU
        benchmarks["benchmarks"]["cpu"] = self.benchmark_cpu()

        # Тест памяти
        benchmarks["benchmarks"]["memory"] = self.benchmark_memory()

        # Тест диска
        benchmarks["benchmarks"]["disk"] = self.benchmark_disk()

        self.log("Системные бенчмарки завершены")
        return benchmarks

    def get_system_info(self) -> Dict[str, str]:
        """Получение информации о системе"""
        import platform

        return {
            "platform": platform.system(),
            "version": platform.version(),
            "architecture": platform.machine(),
            "processor": platform.processor(),
            "python_version": platform.python_version()
        }

    def benchmark_cpu(self) -> Dict[str, float]:
        """Бенчмарк CPU"""
        import time

        start_time = time.time()

        # Простой CPU тест - вычисление факториала
        def factorial(n):
            if n <= 1:
                return 1
            return n * factorial(n - 1)

        # Вычисляем факториал 1000 раз
        for _ in range(1000):
            factorial(10)

        end_time = time.time()
        cpu_time = (end_time - start_time) * 1000  # в миллисекундах

        return {
            "cpu_test_time_ms": cpu_time,
            "operations_per_second": 1000.0 / (cpu_time / 1000.0)
        }

    def benchmark_memory(self) -> Dict[str, float]:
        """Бенчмарк памяти"""
        import time

        start_time = time.time()

        # Тест аллокации памяти
        test_data = []
        for i in range(10000):
            test_data.append([i] * 100)

        allocation_time = (time.time() - start_time) * 1000

        # Тест освобождения памяти
        start_time = time.time()
        test_data.clear()
        deallocation_time = (time.time() - start_time) * 1000

        return {
            "allocation_time_ms": allocation_time,
            "deallocation_time_ms": deallocation_time,
            "total_memory_operations": 10000
        }

    def benchmark_disk(self) -> Dict[str, float]:
        """Бенчмарк диска"""
        import time

        test_file = self.reports_dir / "disk_benchmark_test.tmp"

        # Тест записи
        start_time = time.time()
        with open(test_file, 'w') as f:
            for i in range(1000):
                f.write(f"Test line {i}: " + "x" * 100 + "\n")
        write_time = (time.time() - start_time) * 1000

        # Тест чтения
        start_time = time.time()
        with open(test_file, 'r') as f:
            content = f.read()
        read_time = (time.time() - start_time) * 1000

        # Очистка
        test_file.unlink(missing_ok=True)

        return {
            "write_time_ms": write_time,
            "read_time_ms": read_time,
            "file_size_bytes": len(content)
        }

    def generate_performance_report(self, test_results: Dict[str, Any],
                                  benchmark_results: Dict[str, Any]) -> str:
        """Генерация отчёта о производительности"""
        report = []
        report.append("# 📊 Отчёт о производительности JXCT")
        report.append("")
        report.append(f"**Дата**: {test_results.get('timestamp', 'N/A')}")
        report.append(f"**Платформа**: {benchmark_results.get('system_info', {}).get('platform', 'N/A')}")
        report.append("")

        # Результаты тестов
        report.append("## 🧪 Результаты тестов производительности")
        report.append("")

        if test_results.get('tests'):
            for test_name, test_data in test_results['tests'].items():
                status_emoji = "✅" if test_data['status'] == 'passed' else "❌"
                report.append(f"### {status_emoji} {test_name}")

                if test_data.get('metrics'):
                    for metric_name, metric_data in test_data['metrics'].items():
                        report.append(f"- **{metric_name}**: {metric_data['value']} {metric_data['unit']}")

                if test_data.get('details'):
                    for detail in test_data['details'][:3]:  # Показываем только первые 3 детали
                        report.append(f"  - {detail}")

                report.append("")
        else:
            report.append("❌ Нет данных о тестах")
            report.append("")

        # Системные бенчмарки
        report.append("## 💻 Системные бенчмарки")
        report.append("")

        if benchmark_results.get('benchmarks'):
            for benchmark_name, benchmark_data in benchmark_results['benchmarks'].items():
                report.append(f"### {benchmark_name.upper()}")
                for metric_name, metric_value in benchmark_data.items():
                    if isinstance(metric_value, float):
                        report.append(f"- **{metric_name}**: {metric_value:.2f}")
                    else:
                        report.append(f"- **{metric_name}**: {metric_value}")
                report.append("")

        # Сводка
        summary = test_results.get('summary', {})
        report.append("## 📈 Сводка")
        report.append("")
        report.append(f"- **Всего тестов**: {summary.get('total_tests', 0)}")
        report.append(f"- **Пройдено**: {summary.get('passed_tests', 0)}")
        report.append(f"- **Провалено**: {summary.get('failed_tests', 0)}")

        if summary.get('performance_metrics'):
            metrics = summary['performance_metrics']
            report.append(f"- **Среднее время отклика**: {metrics.get('average_response_time', 0):.2f} ms")
            report.append(f"- **Минимальное время**: {metrics.get('min_response_time', 0):.2f} ms")
            report.append(f"- **Максимальное время**: {metrics.get('max_response_time', 0):.2f} ms")

        return "\n".join(report)

    def save_json_report(self, test_results: Dict[str, Any],
                        benchmark_results: Dict[str, Any]) -> str:
        """Сохранение JSON отчёта"""
        combined_report = {
            "test_results": test_results,
            "benchmark_results": benchmark_results,
            "generated_at": time.strftime("%Y-%m-%d %H:%M:%S")
        }

        report_file = self.reports_dir / "performance-test-report.json"
        with open(report_file, 'w', encoding='utf-8') as f:
            json.dump(combined_report, f, indent=2, ensure_ascii=False)

        return str(report_file)

    def run_all_tests(self) -> bool:
        """Запуск всех тестов производительности"""
        self.log("🚀 Запуск полного тестирования производительности")

        # Компилируем тесты
        if not self.compile_performance_tests():
            self.log("❌ Тесты производительности провалились!", "ERROR")
            return False

        # Запускаем тесты
        test_results = self.run_performance_tests()
        if not test_results:
            self.log("❌ Не удалось получить результаты тестов", "ERROR")
            return False

        # Запускаем системные бенчмарки
        benchmark_results = self.run_system_benchmarks()

        # Генерируем отчёты
        report_content = self.generate_performance_report(test_results, benchmark_results)
        report_file = self.reports_dir / "performance-report.md"

        with open(report_file, 'w', encoding='utf-8') as f:
            f.write(report_content)

        json_file = self.save_json_report(test_results, benchmark_results)

        # Выводим результаты
        print("\n" + "="*60)
        print("📊 РЕЗУЛЬТАТЫ ТЕСТИРОВАНИЯ ПРОИЗВОДИТЕЛЬНОСТИ")
        print("="*60)

        summary = test_results.get('summary', {})
        total_tests = summary.get('total_tests', 0)
        passed_tests = summary.get('passed_tests', 0)

        if total_tests > 0:
            success_rate = (passed_tests / total_tests) * 100
            print(f"✅ Успешность: {success_rate:.1f}% ({passed_tests}/{total_tests})")
        else:
            print("❌ Нет данных о тестах")

        print(f"📄 Отчёты сохранены:")
        print(f"   - Markdown: {report_file}")
        print(f"   - JSON: {json_file}")

        return passed_tests == total_tests

def main():
    parser = argparse.ArgumentParser(description="Запуск тестов производительности JXCT")
    parser.add_argument("-v", "--verbose", action="store_true", help="Подробный вывод")
    args = parser.parse_args()

    runner = PerformanceTestRunner(verbose=args.verbose)
        success = runner.run_all_tests()

    if not success:
        sys.exit(1)

if __name__ == "__main__":
    main()
