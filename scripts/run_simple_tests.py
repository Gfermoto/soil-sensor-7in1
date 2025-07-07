#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
JXCT Simple Test Runner v1.4.1 - Исправленная версия
Быстрое тестирование основных компонентов
"""

import json
import sys
import os
import subprocess
import platform
from pathlib import Path
from datetime import datetime

def log_info(message):
    """Логирование информации"""
    print(f"[INFO] {message}")

def log_success(message):
    """Логирование успеха"""
    print(f"[SUCCESS] ✅ {message}")

def log_error(message):
    """Логирование ошибки"""
    print(f"[ERROR] ❌ {message}")

def log_warning(message):
    """Логирование предупреждения"""
    print(f"[WARNING] ⚠️ {message}")

def run_python_tests():
    """Запуск Python тестов"""
    log_info("Запуск Python тестов...")

    test_files = [
        "test/test_format.py",
        "test/test_validation.py"
    ]

    results = {}
    total_tests = 0
    passed_tests = 0

    for test_file in test_files:
        if Path(test_file).exists():
            try:
                log_info(f"Выполнение {test_file}...")
                result = subprocess.run([
                    sys.executable, str(test_file)
                ], capture_output=True, text=True, timeout=30, encoding='utf-8', errors='ignore')

                success = result.returncode == 0

                if success:
                    log_success(f"{test_file}: PASS")
                    # Подсчитываем количество тестов из вывода
                    output = result.stdout or ""
                    test_count = output.count("PASS") + output.count("OK")
                    if test_count == 0:
                        test_count = 1  # Минимум один тест
                    passed_tests += test_count
                    total_tests += test_count
                else:
                    log_error(f"{test_file}: FAIL")
                    if result.stderr:
                        log_error(f"Ошибка: {result.stderr}")
                    total_tests += 1

                results[test_file] = {
                    "status": "PASS" if success else "FAIL",
                    "output": result.stdout or "",
                    "error": result.stderr if not success else None,
                    "test_count": test_count if success else 1
                }

            except subprocess.TimeoutExpired:
                log_error(f"{test_file}: TIMEOUT")
                results[test_file] = {"status": "TIMEOUT", "error": "Превышено время ожидания"}
                total_tests += 1
            except Exception as e:
                log_error(f"{test_file}: ERROR - {e}")
                results[test_file] = {"status": "ERROR", "error": str(e)}
                total_tests += 1
        else:
            log_warning(f"{test_file}: Файл не найден")

    return results, total_tests, passed_tests

def check_esp32_connection():
    """Проверка подключения ESP32"""
    log_info("Проверка подключения ESP32...")

    try:
        result = subprocess.run([
            "pio", "device", "list"
        ], capture_output=True, text=True, timeout=10, encoding='utf-8', errors='ignore')

        if result.returncode == 0:
            # Ищем ESP32 устройства
            output = result.stdout or ""
            if "Silicon Labs CP210x" in output or "USB" in output:
                for line in output.split('\n'):
                    if "COM" in line and ("Silicon Labs" in line or "USB" in line):
                        # Извлекаем порт более надежно
                        parts = line.split()
                        for part in parts:
                            if part.startswith("COM"):
                                port = part
                                log_success(f"ESP32 найден на порту {port}")
                                return port

        log_warning("ESP32 не найден или не подключен")
        return None

    except Exception as e:
        log_error(f"Ошибка проверки ESP32: {e}")
        return None

def run_esp32_build():
    """Проверка сборки ESP32"""
    log_info("Проверка сборки ESP32...")

    try:
        result = subprocess.run([
            "pio", "run", "-e", "esp32dev"
        ], capture_output=True, text=True, timeout=120, encoding='utf-8', errors='ignore')

        if result.returncode == 0:
            # Проверяем размер прошивки
            firmware_path = Path(".pio/build/esp32dev/firmware.bin")
            if firmware_path.exists():
                size = firmware_path.stat().st_size
                log_success(f"Сборка ESP32 успешна (размер: {size:,} байт)")
                return {"status": "PASS", "firmware_size": size}
            else:
                log_error("Сборка ESP32: firmware.bin не найден")
                return {"status": "FAIL", "error": "firmware.bin не найден"}
        else:
            log_error("Сборка ESP32 не удалась")
            if result.stderr:
                log_error(f"Ошибка: {result.stderr}")
            return {"status": "FAIL", "error": result.stderr or "Неизвестная ошибка"}

    except Exception as e:
        log_error(f"Ошибка сборки ESP32: {e}")
        return {"status": "ERROR", "error": str(e)}

def run_esp32_tests(port=None):
    """Запуск тестов ESP32"""
    if not port:
        port = check_esp32_connection()
        if not port:
            return {"status": "SKIPPED", "reason": "ESP32 не подключен"}

    # Проверяем, что порт корректный
    if not port or not port.startswith("COM"):
        log_warning("Некорректный порт ESP32, пропускаем тесты")
        return {"status": "SKIPPED", "reason": "Некорректный порт ESP32"}

    log_info(f"Запуск тестов ESP32 на порту {port}...")

    try:
        result = subprocess.run([
            "pio", "test", "-e", "esp32dev", "--upload-port", port, "--test-port", port
        ], capture_output=True, text=True, timeout=120, encoding='utf-8', errors='ignore')

        if result.returncode == 0:
            log_success("Тесты ESP32 прошли успешно")
            return {"status": "PASS", "output": result.stdout or ""}
        else:
            log_error("Тесты ESP32 не прошли")
            if result.stderr:
                log_error(f"Ошибка: {result.stderr}")
            return {"status": "FAIL", "error": result.stderr or "Неизвестная ошибка"}

    except Exception as e:
        log_error(f"Ошибка тестов ESP32: {e}")
        return {"status": "ERROR", "error": str(e)}

def generate_report(results):
    """Генерация отчета"""
    log_info("Генерация отчета...")

    # Создаем директорию для отчетов
    reports_dir = Path("test_reports")
    reports_dir.mkdir(exist_ok=True)

    # Подготавливаем отчет
    report = {
        "timestamp": datetime.now().isoformat(),
        "version": "3.7.0",
        "platform": platform.system(),
        "test_results": results,
        "summary": {
            "total_categories": len(results),
            "passed_categories": sum(1 for r in results.values() if r.get("status") == "PASS"),
            "overall_status": "PASS" if all(r.get("status") == "PASS" for r in results.values() if r.get("status") != "SKIPPED") else "FAIL"
        }
    }

    # Сохраняем JSON отчет
    json_path = reports_dir / "simple-test-report.json"
    with open(json_path, 'w', encoding='utf-8') as f:
        json.dump(report, f, indent=2, ensure_ascii=False)

    log_success(f"Отчет сохранен: {json_path}")
    return report

def main():
    """Основная функция"""
    log_info("🧪 JXCT Simple Test Runner v1.4.1 (Исправленная версия)")
    log_info("=" * 60)

    # Переходим в корневую директорию проекта
    script_dir = Path(__file__).parent
    project_root = script_dir.parent
    os.chdir(project_root)

    results = {}

    # 1. Python тесты (основные)
    python_results, total_tests, passed_tests = run_python_tests()
    results["python_tests"] = {
        "status": "PASS" if passed_tests == total_tests else "FAIL",
        "total_tests": total_tests,
        "passed_tests": passed_tests,
        "details": python_results
    }

    # 2. Сборка ESP32
    build_result = run_esp32_build()
    results["esp32_build"] = build_result

    # 3. ESP32 тесты (если устройство подключено)
    esp32_result = run_esp32_tests()
    results["esp32_tests"] = esp32_result

    # Генерируем отчет
    report = generate_report(results)

    # Выводим итоги
    log_info("=" * 60)
    log_info("📊 ИТОГИ ТЕСТИРОВАНИЯ:")

    for category, result in results.items():
        status = result.get("status", "UNKNOWN")
        if status == "PASS":
            log_success(f"{category}: {status}")
        elif status == "SKIPPED":
            log_warning(f"{category}: {status} - {result.get('reason', 'Неизвестная причина')}")
        else:
            log_error(f"{category}: {status}")

    # Общий результат
    overall_status = report["summary"]["overall_status"]
    if overall_status == "PASS":
        log_success("🎉 ВСЕ ОСНОВНЫЕ ТЕСТЫ ПРОЙДЕНЫ УСПЕШНО!")
        return 0
    else:
        log_error("❌ НЕКОТОРЫЕ ТЕСТЫ НЕ ПРОШЛИ")
        return 1

if __name__ == "__main__":
    sys.exit(main())
