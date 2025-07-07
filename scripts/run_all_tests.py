#!/usr/bin/env python3
"""
Скрипт для запуска всех тестов JXCT
Автоматически запускает Python, native и ESP32 тесты в правильной последовательности
"""

import subprocess
import sys
import os
import json
from datetime import datetime

class TestRunner:
    def __init__(self):
        self.results = {
            'python': {'status': 'not_run', 'output': '', 'error': ''},
            'native': {'status': 'not_run', 'output': '', 'error': ''},
            'esp32': {'status': 'not_run', 'output': '', 'error': ''},
            'e2e': {'status': 'not_run', 'output': '', 'error': ''},
            'timestamp': datetime.now().isoformat()
        }

    def run_command(self, command, test_type):
        """Запуск команды и сохранение результата"""
        print(f"\n🔍 Запуск {test_type} тестов...")
        print(f"Команда: {' '.join(command)}")

        try:
            result = subprocess.run(
                command,
                capture_output=True,
                text=True,
                timeout=300  # 5 минут таймаут
            )

            if result.returncode == 0:
                self.results[test_type]['status'] = 'passed'
                self.results[test_type]['output'] = result.stdout
                print(f"✅ {test_type} тесты прошли успешно")
            else:
                self.results[test_type]['status'] = 'failed'
                self.results[test_type]['output'] = result.stdout
                self.results[test_type]['error'] = result.stderr
                print(f"❌ {test_type} тесты провалились")
                print(f"Ошибка: {result.stderr}")

        except subprocess.TimeoutExpired:
            self.results[test_type]['status'] = 'timeout'
            self.results[test_type]['error'] = 'Тест превысил лимит времени (5 минут)'
            print(f"⏰ {test_type} тесты превысили лимит времени")
        except Exception as e:
            self.results[test_type]['status'] = 'error'
            self.results[test_type]['error'] = str(e)
            print(f"💥 Ошибка запуска {test_type} тестов: {e}")

    def run_python_tests(self):
        """Запуск Python тестов"""
        command = [sys.executable, '-m', 'pytest', 'test/', '-v']
        self.run_command(command, 'python')

    def run_native_tests(self):
        """Запуск native C++ тестов"""
        # Определяем платформу для native тестов
        if os.name == 'nt':  # Windows
            command = ['pio', 'test', '-e', 'native-windows']
        else:  # Linux/Mac
            command = ['pio', 'test', '-e', 'native']
        self.run_command(command, 'native')

    def run_esp32_tests(self):
        """Запуск ESP32 тестов"""
        command = ['pio', 'test', '-e', 'esp32dev-test', '--without-uploading']
        self.run_command(command, 'esp32')

    def run_e2e_tests(self):
        """Запуск end-to-end тестов"""
        command = [sys.executable, '-m', 'pytest', 'test/e2e/', '-v']
        self.run_command(command, 'e2e')

    def generate_report(self):
        """Генерация отчета о тестировании"""
        print("\n" + "="*60)
        print("📊 ОТЧЕТ О ТЕСТИРОВАНИИ JXCT")
        print("="*60)

        total_tests = len(self.results) - 2  # Исключаем timestamp и e2e
        passed_tests = sum(1 for k, v in self.results.items()
                          if k not in ['timestamp', 'e2e'] and v['status'] == 'passed')

        for test_type, result in self.results.items():
            if test_type == 'timestamp':
                continue

            status_emoji = {
                'passed': '✅',
                'failed': '❌',
                'timeout': '⏰',
                'error': '💥',
                'not_run': '⏸️'
            }.get(result['status'], '❓')

            print(f"{status_emoji} {test_type.upper()}: {result['status']}")

            if result['error']:
                print(f"   Ошибка: {result['error']}")

        print(f"\n📈 ИТОГО: {passed_tests}/{total_tests} тестов прошли успешно")

        # Сохраняем отчет в JSON
        report_file = f"test_reports/comprehensive-test-report-{datetime.now().strftime('%Y%m%d-%H%M%S')}.json"
        os.makedirs('test_reports', exist_ok=True)

        with open(report_file, 'w', encoding='utf-8') as f:
            json.dump(self.results, f, indent=2, ensure_ascii=False)

        print(f"📄 Подробный отчет сохранен: {report_file}")

        return passed_tests == total_tests

    def run_all_tests(self):
        """Запуск всех тестов в правильной последовательности"""
        print("🚀 ЗАПУСК ПОЛНОГО ТЕСТИРОВАНИЯ JXCT")
        print("="*60)

        # 1. Python тесты (быстрые, проверяют логику)
        self.run_python_tests()

        # 2. Native тесты (проверяют C++ код на хосте)
        self.run_native_tests()

        # 3. ESP32 тесты (проверяют на реальной платформе)
        self.run_esp32_tests()

        # 4. E2E тесты (проверяют интеграцию)
        self.run_e2e_tests()

        # Генерируем отчет
        success = self.generate_report()

        if success:
            print("\n🎉 ВСЕ ТЕСТЫ ПРОШЛИ УСПЕШНО!")
            return 0
        else:
            print("\n⚠️ НЕКОТОРЫЕ ТЕСТЫ ПРОВАЛИЛИСЬ!")
            return 1

def main():
    """Главная функция"""
    runner = TestRunner()
    exit_code = runner.run_all_tests()
    sys.exit(exit_code)

if __name__ == "__main__":
    main()
