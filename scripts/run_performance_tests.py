#!/usr/bin/env python3
"""
Скрипт для запуска тестов производительности JXCT
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
        """Компиляция тестов производительности"""
        self.log("Компиляция тестов производительности...")

        # Проверяем наличие тестов
        test_file = self.test_dir / "test_performance.cpp"
        if not test_file.exists():
            self.log("Файл тестов производительности не найден", "WARNING")
            return False

        # Компилируем тесты
        compile_command = [
            "g++", "-std=c++17", "-O2", "-I../include",
            str(test_file),
            "-o", str(self.test_dir / "performance_test"),
            "-lunity"
        ]

        returncode, stdout, stderr = self.run_command(compile_command, self.test_dir)

        if returncode != 0:
            self.log(f"Ошибка компиляции: {stderr}", "ERROR")
            return False

        self.log("Тесты производительности скомпилированы успешно")
        return True

    def run_performance_tests(self) -> Dict[str, Any]:
        """Запуск тестов производительности"""
        self.log("Запуск тестов производительности...")

        test_executable = self.test_dir / "performance_test"
        if not test_executable.exists():
            self.log("Исполняемый файл тестов не найден", "ERROR")
            return {}

        # Запускаем тесты
        returncode, stdout, stderr = self.run_command([str(test_executable)])

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
            "platform_version": platform.version(),
            "architecture": platform.machine(),
            "processor": platform.processor(),
            "python_version": platform.python_version()
        }

    def benchmark_cpu(self) -> Dict[str, float]:
        """Бенчмарк CPU"""
        import time

        # Простой тест CPU
        start_time = time.time()

        # Выполняем вычисления
        result = 0
        for i in range(1000000):
            result += i * i

        end_time = time.time()
        cpu_time = end_time - start_time

        return {
            "computation_time": cpu_time,
            "operations_per_second": 1000000 / cpu_time
        }

    def benchmark_memory(self) -> Dict[str, float]:
        """Бенчмарк памяти"""
        import time

        # Тест аллокации памяти
        start_time = time.time()

        # Создаём большие структуры данных
        large_list = [i for i in range(100000)]
        large_dict = {str(i): i for i in range(100000)}

        end_time = time.time()
        memory_time = end_time - start_time

        return {
            "allocation_time": memory_time,
            "list_size": len(large_list),
            "dict_size": len(large_dict)
        }

    def benchmark_disk(self) -> Dict[str, float]:
        """Бенчмарк диска"""
        import time

        test_file = self.reports_dir / "disk_benchmark_test.tmp"

        # Тест записи
        start_time = time.time()
        with open(test_file, 'w') as f:
            for i in range(10000):
                f.write(f"Test line {i}\n")
        write_time = time.time() - start_time

        # Тест чтения
        start_time = time.time()
        with open(test_file, 'r') as f:
            content = f.read()
        read_time = time.time() - start_time

        # Очистка
        test_file.unlink(missing_ok=True)

        return {
            "write_time": write_time,
            "read_time": read_time,
            "write_speed": 10000 / write_time,  # строк в секунду
            "read_speed": 10000 / read_time     # строк в секунду
        }

    def generate_performance_report(self, test_results: Dict[str, Any],
                                  benchmark_results: Dict[str, Any]) -> str:
        """Генерация отчёта о производительности"""
        report_file = self.reports_dir / "performance-report.html"

        html_content = f"""
<!DOCTYPE html>
<html lang="ru">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Отчёт о производительности JXCT</title>
    <style>
        body {{ font-family: Arial, sans-serif; margin: 20px; }}
        .header {{ background: #f0f0f0; padding: 20px; border-radius: 5px; }}
        .section {{ margin: 20px 0; padding: 15px; border: 1px solid #ddd; border-radius: 5px; }}
        .test {{ margin: 10px 0; padding: 10px; background: #f9f9f9; border-radius: 3px; }}
        .metric {{ display: inline-block; margin: 5px 10px; padding: 5px; background: #e8f4f8; border-radius: 3px; }}
        .passed {{ color: green; }}
        .failed {{ color: red; }}
        .warning {{ color: orange; }}
        table {{ border-collapse: collapse; width: 100%; }}
        th, td {{ border: 1px solid #ddd; padding: 8px; text-align: left; }}
        th {{ background-color: #f2f2f2; }}
    </style>
</head>
<body>
    <div class="header">
        <h1>🚀 Отчёт о производительности JXCT</h1>
        <p><strong>Дата:</strong> {test_results.get('timestamp', 'N/A')}</p>
        <p><strong>Система:</strong> {benchmark_results.get('system_info', {}).get('platform', 'N/A')} {benchmark_results.get('system_info', {}).get('architecture', 'N/A')}</p>
    </div>

    <div class="section">
        <h2>📊 Результаты тестов производительности</h2>
        <p><strong>Всего тестов:</strong> {test_results.get('summary', {}).get('total_tests', 0)}</p>
        <p><strong>Пройдено:</strong> <span class="passed">{test_results.get('summary', {}).get('passed_tests', 0)}</span></p>
        <p><strong>Провалено:</strong> <span class="failed">{test_results.get('summary', {}).get('failed_tests', 0)}</span></p>

        <h3>Детали тестов:</h3>
"""

        for test_name, test_data in test_results.get('tests', {}).items():
            status_class = "passed" if test_data.get('status') == 'passed' else "failed"
            html_content += f"""
        <div class="test">
            <h4 class="{status_class}">✅ {test_name}</h4>
"""

            for metric_name, metric_data in test_data.get('metrics', {}).items():
                html_content += f"""
            <div class="metric">
                <strong>{metric_name}:</strong> {metric_data['value']:.2f} {metric_data['unit']}
            </div>
"""

            html_content += """
        </div>
"""

        # Добавляем системные бенчмарки
        html_content += """
    </div>

    <div class="section">
        <h2>💻 Системные бенчмарки</h2>
        <table>
            <tr>
                <th>Метрика</th>
                <th>Значение</th>
                <th>Единица</th>
            </tr>
"""

        benchmarks = benchmark_results.get('benchmarks', {})

        # CPU бенчмарк
        cpu_bench = benchmarks.get('cpu', {})
        html_content += f"""
            <tr>
                <td>CPU - Время вычислений</td>
                <td>{cpu_bench.get('computation_time', 0):.3f}</td>
                <td>секунды</td>
            </tr>
            <tr>
                <td>CPU - Операций в секунду</td>
                <td>{cpu_bench.get('operations_per_second', 0):.0f}</td>
                <td>ops/sec</td>
            </tr>
"""

        # Memory бенчмарк
        memory_bench = benchmarks.get('memory', {})
        html_content += f"""
            <tr>
                <td>Memory - Время аллокации</td>
                <td>{memory_bench.get('allocation_time', 0):.3f}</td>
                <td>секунды</td>
            </tr>
            <tr>
                <td>Memory - Размер списка</td>
                <td>{memory_bench.get('list_size', 0):,}</td>
                <td>элементов</td>
            </tr>
"""

        # Disk бенчмарк
        disk_bench = benchmarks.get('disk', {})
        html_content += f"""
            <tr>
                <td>Disk - Скорость записи</td>
                <td>{disk_bench.get('write_speed', 0):.0f}</td>
                <td>строк/сек</td>
            </tr>
            <tr>
                <td>Disk - Скорость чтения</td>
                <td>{disk_bench.get('read_speed', 0):.0f}</td>
                <td>строк/сек</td>
            </tr>
"""

        html_content += """
        </table>
    </div>

    <div class="section">
        <h2>📈 Рекомендации</h2>
        <ul>
            <li>Мониторьте производительность регулярно</li>
            <li>Оптимизируйте медленные операции</li>
            <li>Рассмотрите возможность кэширования</li>
            <li>Проверяйте использование памяти</li>
        </ul>
    </div>
</body>
</html>
"""

        with open(report_file, 'w', encoding='utf-8') as f:
            f.write(html_content)

        return str(report_file)

    def save_json_report(self, test_results: Dict[str, Any],
                        benchmark_results: Dict[str, Any]) -> str:
        """Сохранение JSON отчёта"""
        combined_report = {
            "test_results": test_results,
            "benchmark_results": benchmark_results,
            "generated_at": time.strftime("%Y-%m-%d %H:%M:%S")
        }

        report_file = self.reports_dir / "performance-report.json"
        with open(report_file, 'w', encoding='utf-8') as f:
            json.dump(combined_report, f, indent=2, ensure_ascii=False)

        return str(report_file)

    def run_all_tests(self) -> bool:
        """Запуск всех тестов производительности"""
        self.log("🚀 Запуск полного тестирования производительности")

        # Компилируем тесты
        if not self.compile_performance_tests():
            self.log("❌ Ошибка компиляции тестов", "ERROR")
            return False

        # Запускаем тесты производительности
        test_results = self.run_performance_tests()
        if not test_results:
            self.log("❌ Ошибка выполнения тестов производительности", "ERROR")
            return False

        # Запускаем системные бенчмарки
        benchmark_results = self.run_system_benchmarks()

        # Генерируем отчёты
        html_report = self.generate_performance_report(test_results, benchmark_results)
        json_report = self.save_json_report(test_results, benchmark_results)

        self.log(f"📊 HTML отчёт сохранён: {html_report}")
        self.log(f"📊 JSON отчёт сохранён: {json_report}")

        # Выводим краткую сводку
        summary = test_results.get('summary', {})
        self.log(f"📈 Результаты: {summary.get('passed_tests', 0)}/{summary.get('total_tests', 0)} тестов пройдено")

        if summary.get('performance_metrics'):
            metrics = summary['performance_metrics']
            self.log(f"⚡ Среднее время ответа: {metrics.get('average_response_time', 0):.2f}ms")

        return True

def main():
    """Основная функция"""
    parser = argparse.ArgumentParser(description="Запуск тестов производительности JXCT")
    parser.add_argument("--verbose", "-v", action="store_true", help="Подробный вывод")
    parser.add_argument("--quick", "-q", action="store_true", help="Быстрый тест (только основные метрики)")

    args = parser.parse_args()

    runner = PerformanceTestRunner(verbose=args.verbose)

    try:
        success = runner.run_all_tests()

        if success:
            print("\n✅ Тесты производительности завершены успешно!")
            sys.exit(0)
        else:
            print("\n❌ Тесты производительности провалились!")
            sys.exit(1)

    except KeyboardInterrupt:
        print("\n\n❌ Тестирование прервано пользователем")
        sys.exit(1)
    except Exception as e:
        print(f"\n❌ Неожиданная ошибка: {e}")
        sys.exit(1)

if __name__ == "__main__":
    main()
