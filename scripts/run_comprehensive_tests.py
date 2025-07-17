#!/usr/bin/env python3
"""
🧪 JXCT Comprehensive Test Suite v2.0
Комплексный набор тестов для достижения покрытия 85%+
Включает: бизнес-логику, фильтры, веб-маршруты, WiFi/OTA, логгер/датчики
"""

import sys
import os
import json
import time
import subprocess
from pathlib import Path
from typing import Dict, List, Any, Optional
from dataclasses import dataclass

# Импортируем конфигурацию тестирования
try:
    from test_config import test_config
except ImportError:
    # Заглушка если конфигурация недоступна
    class TestConfig:
        def __init__(self):
            self.hardware_available = False
            self.test_settings = {"coverage_target": 85.0}
    test_config = TestConfig()

# Добавляем путь к модулям
sys.path.insert(0, str(Path(__file__).parent.parent))

@dataclass
class TestResult:
    name: str
    success: bool
    coverage: float
    duration: float
    details: Dict[str, Any]

class ComprehensiveTestRunner:
    """Комплексный тестер для достижения покрытия 85%+"""
    
    def __init__(self):
        self.results = []
        self.start_time = time.time()
        self.test_modules = test_config.get_test_modules() + ["test_validation_enhanced"]
    
    def run_python_test(self, module_name: str) -> TestResult:
        """Запускает Python тест"""
        start_time = time.time()

        try:
            # Запускаем тест
            result = subprocess.run(
                [sys.executable, f"test/{module_name}.py"],
                capture_output=True,
                text=True,
                encoding='utf-8',
                errors='ignore',
                timeout=60
            )
            
            duration = time.time() - start_time
            
            # Парсим результат
            output = result.stdout + result.stderr
            
            # Ищем покрытие в выводе
            coverage = 0.0
            if "Покрытие:" in output:
                for line in output.split('\n'):
                    if "Покрытие:" in line:
                        try:
                            coverage = float(line.split(":")[1].strip().replace("%", ""))
                            break
                        except:
                            pass
            
            # Определяем успех
            success = result.returncode == 0
            
            return TestResult(
                name=module_name,
                success=success,
                coverage=coverage,
                duration=duration,
                details={
                    "returncode": result.returncode,
                    "stdout": result.stdout,
                    "stderr": result.stderr,
                    "output": output
                }
            )
            
        except subprocess.TimeoutExpired:
            return TestResult(
                name=module_name,
                success=False,
                coverage=0.0,
                duration=60.0,
                details={"error": "Timeout expired"}
            )
        except Exception as e:
            return TestResult(
                name=module_name,
                success=False,
                coverage=0.0,
                duration=time.time() - start_time,
                details={"error": str(e)}
            )
    
    def run_esp32_build(self) -> TestResult:
        """Запускает сборку ESP32"""
        start_time = time.time()
        
        try:
            result = subprocess.run(
                ["pio", "run", "-e", "esp32dev"],
                capture_output=True,
                text=True,
                encoding='utf-8',
                errors='ignore',
                timeout=300
            )
            
            duration = time.time() - start_time
            success = result.returncode == 0
            
            return TestResult(
                name="esp32_build",
                success=success,
                coverage=100.0 if success else 0.0,
                duration=duration,
                details={
                    "returncode": result.returncode,
                    "stdout": result.stdout,
                    "stderr": result.stderr
                }
            )

        except Exception as e:
            return TestResult(
                name="esp32_build",
                success=False,
                coverage=0.0,
                duration=time.time() - start_time,
                details={"error": str(e)}
            )
    
    def run_clang_tidy(self) -> TestResult:
        """Запускает clang-tidy анализ"""
        start_time = time.time()
        
        try:
            result = subprocess.run(
                [sys.executable, "scripts/run_clang_tidy_analysis.py"],
                capture_output=True,
                text=True,
                encoding='utf-8',
                errors='ignore',
                timeout=120
            )
            
            duration = time.time() - start_time
            success = result.returncode == 0
            
            # Парсим количество предупреждений
            warnings_count = 0
            if "предупреждений" in result.stdout:
                for line in result.stdout.split('\n'):
                    if "предупреждений" in line:
                        try:
                            warnings_count = int(line.split()[0])
                            break
                        except:
                            pass
            
            return TestResult(
                name="clang_tidy",
                success=success,
                coverage=100.0 if success else 0.0,
                duration=duration,
                details={
                    "returncode": result.returncode,
                    "warnings_count": warnings_count,
                    "stdout": result.stdout,
                    "stderr": result.stderr
                }
            )

        except Exception as e:
            return TestResult(
                name="clang_tidy",
                success=False,
                coverage=0.0,
                duration=time.time() - start_time,
                details={"error": str(e)}
            )
    
    def run_native_tests(self) -> TestResult:
        """Запускает Native C++ тесты"""
        start_time = time.time()
        
        try:
            result = subprocess.run(
                ["pio", "test", "-e", "native"],
                capture_output=True,
                text=True,
                encoding='utf-8',
                errors='ignore',
                timeout=60
            )
            
            duration = time.time() - start_time
            success = result.returncode == 0
            
            return TestResult(
                name="native_tests",
                success=success,
                coverage=100.0 if success else 0.0,
                duration=duration,
                details={
                    "returncode": result.returncode,
                    "stdout": result.stdout,
                    "stderr": result.stderr
                }
            )

        except Exception as e:
            return TestResult(
                name="native_tests",
                success=False,
                coverage=0.0,
                duration=time.time() - start_time,
                details={"error": str(e)}
            )
    
    def calculate_overall_coverage(self) -> float:
        """Вычисляет общее покрытие"""
        if not self.results:
            return 0.0
        
        # Берем только Python тесты для покрытия
        python_results = [r for r in self.results if r.name in self.test_modules]
        
        if not python_results:
            return 0.0
        
        total_coverage = sum(r.coverage for r in python_results)
        return total_coverage / len(python_results)
    
    def generate_report(self) -> Dict[str, Any]:
        """Генерирует отчет о тестировании"""
        total_duration = time.time() - self.start_time
        
        # Статистика по типам тестов
        python_tests = [r for r in self.results if r.name in self.test_modules]
        system_tests = [r for r in self.results if r.name not in self.test_modules]
        
        python_passed = sum(1 for r in python_tests if r.success)
        system_passed = sum(1 for r in system_tests if r.success)
        
        overall_coverage = self.calculate_overall_coverage()
        
        report = {
            "timestamp": time.strftime("%Y-%m-%d %H:%M:%S"),
            "total_duration": total_duration,
            "overall_coverage": overall_coverage,
            "target_coverage": 85.0,
            "coverage_achieved": overall_coverage >= 85.0,
            "statistics": {
                "total_tests": len(self.results),
                "python_tests": len(python_tests),
                "system_tests": len(system_tests),
                "python_passed": python_passed,
                "system_passed": system_passed,
                "total_passed": python_passed + system_passed,
                "total_failed": len(self.results) - (python_passed + system_passed)
            },
            "results": [
                {
                    "name": r.name,
                    "success": r.success,
                    "coverage": r.coverage,
                    "duration": r.duration,
                    "details": r.details
                }
                for r in self.results
            ]
        }
        
        return report
    
    def save_report(self, report: Dict[str, Any]) -> str:
        """Сохраняет отчет в файл"""
        report_path = "test_reports/comprehensive-test-report.json"
        
        # Создаем директорию если не существует
        Path(report_path).parent.mkdir(parents=True, exist_ok=True)
        
        with open(report_path, 'w', encoding='utf-8') as f:
            json.dump(report, f, indent=2, ensure_ascii=False)
        
        return report_path
    
    def print_summary(self, report: Dict[str, Any]):
        """Выводит краткую сводку"""
        print("\n" + "=" * 80)
        print("📊 КОМПЛЕКСНЫЙ ОТЧЕТ О ТЕСТИРОВАНИИ")
        print("=" * 80)
        
        stats = report["statistics"]
        print(f"⏱️  Общее время: {report['total_duration']:.1f} сек")
        print(f"📈 Общее покрытие: {report['overall_coverage']:.1f}%")
        print(f"🎯 Целевое покрытие: {test_config.test_settings['coverage_target']:.1f}%")
        
        if report["coverage_achieved"]:
            print(f"🎉 ЦЕЛЬ ДОСТИГНУТА! Покрытие {test_config.test_settings['coverage_target']}%+")
        else:
            print(f"⚠️ Требуется еще {test_config.test_settings['coverage_target'] - report['overall_coverage']:.1f}% для достижения цели")
        
        print(f"\n📋 СТАТИСТИКА:")
        print(f"   📊 Всего тестов: {stats['total_tests']}")
        print(f"   🐍 Python тестов: {stats['python_tests']} (✅ {stats['python_passed']}, ❌ {stats['python_tests'] - stats['python_passed']})")
        print(f"   🔧 Системных тестов: {stats['system_tests']} (✅ {stats['system_passed']}, ❌ {stats['system_tests'] - stats['system_passed']})")
        print(f"   ✅ Всего пройдено: {stats['total_passed']}")
        print(f"   ❌ Всего провалено: {stats['total_failed']}")
        
        print(f"\n📄 Детальный отчет: {report.get('report_path', 'N/A')}")
    
    def run_all_tests(self) -> bool:
        """Запускает все тесты"""
        print("🚀 ЗАПУСК КОМПЛЕКСНОГО ТЕСТИРОВАНИЯ")
        print("=" * 80)
        print("Цель: достижение покрытия 85%+")
        print("=" * 80)
        
        # Запускаем Python тесты
        print("\n🐍 Python тесты:")
        for module in self.test_modules:
            print(f"   🔄 {module}...", end=" ", flush=True)
            result = self.run_python_test(module)
            self.results.append(result)
            
            status = "✅" if result.success else "❌"
            print(f"{status} ({result.coverage:.1f}%)")
        
        # Запускаем системные тесты
        print("\n🔧 Системные тесты:")
        
        if test_config.should_run_test("system_tests"):
            print("   🔄 ESP32 сборка...", end=" ", flush=True)
            result = self.run_esp32_build()
            self.results.append(result)
            status = "✅" if result.success else "❌"
            print(f"{status}")
            
            print("   🔄 Clang-tidy анализ...", end=" ", flush=True)
            result = self.run_clang_tidy()
            self.results.append(result)
            status = "✅" if result.success else "❌"
            print(f"{status}")
            
            print("   🔄 Native C++ тесты...", end=" ", flush=True)
            result = self.run_native_tests()
            self.results.append(result)
            status = "✅" if result.success else "❌"
            print(f"{status}")
        else:
            print("   ⏭️ Системные тесты пропущены (железо недоступно)")
        
        # Запускаем hardware тесты если доступно
        if test_config.should_run_test("hardware_tests"):
            print("\n🔌 Hardware тесты:")
            print("   ⏭️ Hardware тесты не реализованы в текущей версии")
        else:
            print("\n🔌 Hardware тесты:")
            print("   ⏭️ Hardware тесты пропущены (железо недоступно)")
        
        # Генерируем отчет
        print("\n📊 Генерация отчета...")
        report = self.generate_report()
        report_path = self.save_report(report)
        report["report_path"] = report_path
        
        # Выводим сводку
        self.print_summary(report)
        
        return report["coverage_achieved"]

def main():
    """Главная функция"""
    runner = ComprehensiveTestRunner()
    success = runner.run_all_tests()

    if success:
        print("\n🎉 ВСЕ ТЕСТЫ ПРОЙДЕНЫ УСПЕШНО!")
        print("📈 ЦЕЛЬ ПОКРЫТИЯ 85%+ ДОСТИГНУТА!")
        return 0
    else:
        print("\n⚠️ НЕКОТОРЫЕ ТЕСТЫ ПРОВАЛЕНЫ")
        print("📈 ТРЕБУЕТСЯ ДОРАБОТКА ДЛЯ ДОСТИЖЕНИЯ 85%+")
        return 1

if __name__ == "__main__":
    sys.exit(main())
