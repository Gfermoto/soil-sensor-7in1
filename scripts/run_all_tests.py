#!/usr/bin/env python3
"""
Универсальный тестовый раннер для JXCT
Запускает ВСЕ тесты с подробным выводом и таймаутами
Версия: 1.0.0
"""

import os
import sys
import subprocess
import time
import signal
from pathlib import Path
from datetime import datetime
import threading

class TestRunner:
    def __init__(self):
        self.project_root = Path(__file__).parent.parent
        self.results = {
            "total": 0,
            "passed": 0,
            "failed": 0,
            "timeout": 0,
            "errors": 0
        }
        self.start_time = time.time()

    def print_header(self):
        print("=" * 80)
        print("🧪 УНИВЕРСАЛЬНЫЙ ТЕСТОВЫЙ РАННЕР JXCT")
        print("=" * 80)
        print(f"📅 Время запуска: {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}")
        print(f"📁 Директория: {self.project_root}")
        print("=" * 80)

    def run_test_with_timeout(self, test_name, command, timeout=30, description=""):
        """Запускает тест с таймаутом и подробным выводом"""
        print(f"\n🔍 [{test_name}] Запуск теста...")
        if description:
            print(f"   📝 {description}")
        
        try:
            # Запускаем процесс
            process = subprocess.Popen(
                command,
                stdout=subprocess.PIPE,
                stderr=subprocess.PIPE,
                text=True,
                encoding='utf-8',
                errors='ignore',
                cwd=self.project_root
            )
            
            # Ждем завершения с таймаутом
            try:
                stdout, stderr = process.communicate(timeout=timeout)
                return_code = process.returncode
                
                # Выводим результат
                if return_code == 0:
                    print(f"   ✅ [{test_name}] УСПЕШНО (код: {return_code})")
                    if stdout.strip():
                        print(f"   📤 Вывод:\n{stdout}")
                    self.results["passed"] += 1
                    return True
                else:
                    print(f"   ❌ [{test_name}] ПРОВАЛЕН (код: {return_code})")
                    if stdout.strip():
                        print(f"   📤 stdout:\n{stdout}")
                    if stderr.strip():
                        print(f"   📤 stderr:\n{stderr}")
                    self.results["failed"] += 1
                    return False
                    
            except subprocess.TimeoutExpired:
                print(f"   ⏰ [{test_name}] ТАЙМАУТ ({timeout}с)")
                process.kill()
                process.wait()
                self.results["timeout"] += 1
                return False
                
        except Exception as e:
            print(f"   💥 [{test_name}] ОШИБКА: {e}")
            self.results["errors"] += 1
            return False
        finally:
            self.results["total"] += 1

    def run_python_tests(self):
        """Запускает все Python тесты"""
        print("\n🐍 PYTHON ТЕСТЫ")
        print("-" * 40)
        
        python_tests = [
            ("test_format.py", [sys.executable, "test/test_format.py"], 30, "Тестирование форматирования"),
            ("test_validation.py", [sys.executable, "test/test_validation.py"], 30, "Тестирование валидации"),
            ("test_critical_functions.py", [sys.executable, "test/test_critical_functions.py"], 30, "Тестирование критических функций"),
            ("test_modbus_mqtt.py", [sys.executable, "test/test_modbus_mqtt.py"], 60, "Тестирование ModBus и MQTT"),
            ("test_system_functions.py", [sys.executable, "test/test_system_functions.py"], 30, "Тестирование системных функций"),
            ("test_routes.py", [sys.executable, "test/test_routes.py"], 30, "Тестирование маршрутов"),
            ("test_fake_sensor_values.py", [sys.executable, "test/test_fake_sensor_values.py"], 30, "Тестирование фейковых значений датчиков"),
            ("test_compensation_formulas.py", [sys.executable, "test/test_compensation_formulas.py"], 60, "Тестирование формул компенсации"),
            ("test_scientific_recommendations.py", [sys.executable, "test/test_scientific_recommendations.py"], 120, "Тестирование научных рекомендаций"),
        ]
        
        for test_name, command, timeout, description in python_tests:
            self.run_test_with_timeout(test_name, command, timeout, description)

    def run_e2e_tests(self):
        """Запускает E2E тесты"""
        print("\n🌐 E2E ТЕСТЫ")
        print("-" * 40)
        
        e2e_tests = [
            ("test_web_ui.py", [sys.executable, "test/e2e/test_web_ui.py"], 60, "E2E тесты веб-интерфейса"),
        ]
        
        for test_name, command, timeout, description in e2e_tests:
            self.run_test_with_timeout(test_name, command, timeout, description)

    def run_integration_tests(self):
        """Запускает интеграционные тесты"""
        print("\n🔗 ИНТЕГРАЦИОННЫЕ ТЕСТЫ")
        print("-" * 40)
        
        # Проверяем наличие интеграционных тестов
        integration_dir = self.project_root / "test" / "integration"
        if integration_dir.exists():
            for test_file in integration_dir.glob("*.cpp"):
                test_name = test_file.name
                print(f"   📋 [{test_name}] C++ интеграционный тест")
                
                # Компилируем и запускаем C++ тест
                try:
                    # Компилируем с помощью w64devkit
                    compile_cmd = [
                        "C:\\Program Files\\w64devkit\\bin\\g++.exe",
                        "-std=c++17",
                        "-I", str(self.project_root / "include"),
                        "-I", str(self.project_root / "src"),
                        "-I", str(self.project_root / "test" / "stubs"),
                        "-I", str(self.project_root / "test" / "stubs" / "Unity" / "src"),
                        str(test_file),
                        str(self.project_root / "test" / "stubs" / "Unity" / "src" / "unity.c"),
                        "-o", str(test_file.with_suffix(".exe"))
                    ]
                    
                    print(f"   🔨 Компиляция {test_name}...")
                    result = subprocess.run(
                        compile_cmd,
                        cwd=self.project_root,
                        capture_output=True,
                        text=True,
                        timeout=60
                    )
                    
                    if result.returncode == 0:
                        # Запускаем скомпилированный тест
                        exe_path = test_file.with_suffix(".exe")
                        print(f"   🚀 Запуск {test_name}...")
                        
                        test_result = subprocess.run(
                            [str(exe_path)],
                            cwd=self.project_root,
                            capture_output=True,
                            text=True,
                            timeout=30
                        )
                        
                        if test_result.returncode == 0:
                            print(f"   ✅ [{test_name}] УСПЕШНО")
                            if test_result.stdout.strip():
                                print(f"   📤 Вывод:\n{test_result.stdout}")
                            self.results["passed"] += 1
                        else:
                            print(f"   ❌ [{test_name}] ПРОВАЛЕН (код: {test_result.returncode})")
                            if test_result.stdout.strip():
                                print(f"   📤 stdout:\n{test_result.stdout}")
                            if test_result.stderr.strip():
                                print(f"   📤 stderr:\n{test_result.stderr}")
                            self.results["failed"] += 1
                    else:
                        print(f"   ❌ [{test_name}] ОШИБКА КОМПИЛЯЦИИ")
                        if result.stderr.strip():
                            print(f"   📤 stderr:\n{result.stderr}")
                        self.results["failed"] += 1
                        
                except subprocess.TimeoutExpired:
                    print(f"   ⏰ [{test_name}] ТАЙМАУТ")
                    self.results["timeout"] += 1
                except Exception as e:
                    print(f"   💥 [{test_name}] ОШИБКА: {e}")
                    self.results["errors"] += 1
                finally:
                    self.results["total"] += 1
                    
                    # Удаляем временный exe файл
                    try:
                        exe_path = test_file.with_suffix(".exe")
                        if exe_path.exists():
                            exe_path.unlink()
                    except:
                        pass

    def run_performance_tests(self):
        """Запускает тесты производительности"""
        print("\n⚡ ТЕСТЫ ПРОИЗВОДИТЕЛЬНОСТИ")
        print("-" * 40)
        
        perf_tests = [
            ("test_performance.py", [sys.executable, "test/performance/test_performance.py"], 120, "Тесты производительности"),
        ]
        
        for test_name, command, timeout, description in perf_tests:
            self.run_test_with_timeout(test_name, command, timeout, description)

    def run_esp32_tests(self):
        """Запускает тесты ESP32"""
        print("\n🔧 ESP32 ТЕСТЫ")
        print("-" * 40)
        
        try:
            # Проверяем сборку ESP32
            print("   🔨 Проверка сборки ESP32...")
            result = subprocess.run(
                ["pio", "run", "-e", "esp32dev"],
                cwd=self.project_root,
                capture_output=True,
                text=True,
                timeout=180
            )
            
            if result.returncode == 0:
                print("   ✅ Сборка ESP32 успешна")
                self.results["passed"] += 1
            else:
                print("   ❌ Сборка ESP32 провалена")
                print(f"   📤 stderr:\n{result.stderr}")
                self.results["failed"] += 1
                
        except subprocess.TimeoutExpired:
            print("   ⏰ Сборка ESP32 превысила таймаут")
            self.results["timeout"] += 1
        except Exception as e:
            print(f"   💥 Ошибка сборки ESP32: {e}")
            self.results["errors"] += 1
        finally:
            self.results["total"] += 1

    def print_summary(self):
        """Выводит итоговую сводку"""
        duration = time.time() - self.start_time
        
        print("\n" + "=" * 80)
        print("📊 ИТОГОВАЯ СВОДКА ТЕСТИРОВАНИЯ")
        print("=" * 80)
        print(f"⏱️  Время выполнения: {duration:.2f} секунд")
        print(f"📈 Всего тестов: {self.results['total']}")
        print(f"✅ Успешно: {self.results['passed']}")
        print(f"❌ Провалено: {self.results['failed']}")
        print(f"⏰ Таймаут: {self.results['timeout']}")
        print(f"💥 Ошибки: {self.results['errors']}")
        
        if self.results['total'] > 0:
            success_rate = (self.results['passed'] / self.results['total']) * 100
            print(f"📊 Успешность: {success_rate:.1f}%")
            
            if success_rate >= 90:
                print("🎉 Отличный результат!")
            elif success_rate >= 80:
                print("👍 Хороший результат")
            elif success_rate >= 70:
                print("⚠️  Требует внимания")
            else:
                print("🚨 Критический уровень")
        else:
            print("📊 Успешность: N/A (нет тестов)")
        
        print("=" * 80)

    def run_all(self):
        """Запускает все тесты"""
        self.print_header()
        
        # Запускаем все категории тестов
        self.run_python_tests()
        self.run_e2e_tests()
        self.run_integration_tests()
        self.run_performance_tests()
        self.run_esp32_tests()
        
        # Выводим итоговую сводку
        self.print_summary()
        
        # Возвращаем код выхода
        if self.results['failed'] > 0 or self.results['errors'] > 0:
            return 1
        return 0

def main():
    """Главная функция"""
    runner = TestRunner()
    return runner.run_all()

if __name__ == "__main__":
    sys.exit(main())
