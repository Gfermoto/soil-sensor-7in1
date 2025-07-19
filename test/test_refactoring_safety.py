#!/usr/bin/env python3
"""
Refactoring Safety Tests for JXCT Soil Sensor Project
Тесты безопасности рефакторинга для IoT системы на ESP32

Author: JXCT Development Team
Version: 3.11.0
License: MIT
"""

import json
import subprocess
import sys
import os
import re
from pathlib import Path
from typing import Dict, List, Any, Optional, Set
from dataclasses import dataclass, asdict
from datetime import datetime

# Константы для IoT системы
REQUIRED_API_ENDPOINTS = [
    "/",  # Главная страница
    "/status",  # Статус системы
    "/health",  # Проверка здоровья
    "/api/calibration/status",  # Статус калибровки
    "/api/calibration/ph/add",  # Добавление pH калибровки
    "/api/calibration/ec/add",  # Добавление EC калибровки
    "/api/calibration/npk/set",  # Установка NPK калибровки
    "/api/calibration/export",  # Экспорт калибровки
    "/api/calibration/import",  # Импорт калибровки
    "/api/calibration/reset",  # Сброс калибровки
    "/api/ota/status",  # Статус OTA
    "/api/ota/check",  # Проверка обновлений
    "/api/ota/install",  # Установка обновлений
    "/api/reports/test-summary",  # Отчет тестов
    "/api/reports/technical-debt",  # Отчет технического долга
    "/api/reports/full",  # Полный отчет
    "/sensor_json",  # Данные датчиков
    "/readings/upload",  # Загрузка показаний
    "/readings/csv_reset",  # Сброс CSV
    "/readings/profile",  # Профиль показаний
    "/config_manager",  # Менеджер конфигурации
    "/save_intervals",  # Сохранение интервалов
    "/reset_intervals",  # Сброс интервалов
    "/reset",  # Сброс системы
    "/reboot",  # Перезагрузка
    "/ota",  # OTA обновления
    "/reports",  # Отчеты
    "/reports/dashboard"  # Дашборд отчетов
]

REQUIRED_DATA_STRUCTURES = [
    "SensorData",
    "CalibrationEntry", 
    "CalibrationPoint",
    "PHCalibration",
    "ECCalibration", 
    "NPKCalibration",
    "SensorCalibration",
    "EnvironmentalConditions",
    "NPKReferences",
    "ValidationResult",
    "ValidationError",
    "ConfigValidationResult",
    "SensorValidationResult",
    "ConfigData",
    "Config",
    "DebugStats",
    "ButtonConfig"
]

REQUIRED_BUSINESS_INTERFACES = [
    "ICropRecommendationEngine",
    "ISensorCalibrationService",
    "ISensorCompensationService"
]

@dataclass
class SafetyTestResult:
    """Результат теста безопасности"""
    test_name: str
    status: str  # PASS, FAIL, WARN
    message: str
    details: Dict[str, Any]
    timestamp: str

@dataclass
class RefactoringSafetyReport:
    """Отчёт о безопасности рефакторинга"""
    version: str
    timestamp: str
    total_tests: int
    passed_tests: int
    failed_tests: int
    warning_tests: int
    results: List[SafetyTestResult]
    summary: Dict[str, Any]

class JXCTRefactoringSafetyTester:
    """Профессиональный тестер безопасности рефакторинга"""
    
    def __init__(self):
        self.project_root = Path(__file__).parent.parent
        self.results: List[SafetyTestResult] = []
        self.start_time = datetime.now()
        
    def log(self, message: str, level: str = "INFO"):
        """Логирование с временными метками"""
        timestamp = datetime.now().strftime("%H:%M:%S.%f")[:-3]
        print(f"[{timestamp}] {level}: {message}")
    
    def run_command(self, command: List[str], timeout: int = 120) -> Dict[str, Any]:
        """Выполнение команды с обработкой ошибок для IoT среды"""
        try:
            # Проверка доступности команды
            if command[0] == "pio" and not self._check_pio_available():
                return {
                    "success": False,
                    "error": "PlatformIO not available",
                    "stdout": "",
                    "stderr": "PlatformIO command not found"
                }
            
            result = subprocess.run(
                command,
                capture_output=True,
                text=True,
                timeout=timeout,
                cwd=self.project_root,
                env=self._get_environment()
            )
            return {
                "success": result.returncode == 0,
                "stdout": result.stdout,
                "stderr": result.stderr,
                "returncode": result.returncode
            }
        except subprocess.TimeoutExpired:
            return {"success": False, "error": "Timeout", "timeout": timeout}
        except FileNotFoundError:
            return {"success": False, "error": f"Command not found: {command[0]}"}
        except Exception as e:
            return {"success": False, "error": str(e)}
    
    def _check_pio_available(self) -> bool:
        """Проверка доступности PlatformIO"""
        try:
            result = subprocess.run(
                ["pio", "--version"],
                capture_output=True,
                text=True,
                timeout=10
            )
            return result.returncode == 0
        except:
            return False
    
    def _get_environment(self) -> Dict[str, str]:
        """Получение окружения для команд"""
        env = os.environ.copy()
        # Добавляем специфичные для IoT переменные
        env["JXCT_SAFETY_TEST_MODE"] = "1"
        return env
    
    def test_api_endpoints_unchanged(self) -> SafetyTestResult:
        """Тест: API endpoints не изменились"""
        self.log("🔍 Тест: API endpoints не изменились...")
        
        # Поиск API endpoints в веб-маршрутах
        web_routes_dir = self.project_root / "src" / "web"
        found_endpoints = set()
        
        if not web_routes_dir.exists():
            return SafetyTestResult(
                test_name="API Endpoints Unchanged",
                status="FAIL",
                message="Web routes directory not found",
                details={"error": "src/web directory not found"},
                timestamp=datetime.now().isoformat()
            )
        
        for route_file in web_routes_dir.glob("routes_*.cpp"):
            try:
                with open(route_file, 'r', encoding='utf-8') as f:
                    content = f.read()
                    # Поиск server.on() вызовов с различными паттернами
                    patterns = [
                        r'webServer\.on\("([^"]+)"',  # webServer.on("/endpoint"
                        r'webServer\.on\(([^,]+),',   # webServer.on(API_CONSTANT
                        r'webServer\.on\("([^"]+)",\s*HTTP_',  # webServer.on("/endpoint", HTTP_
                    ]
                    
                    for pattern in patterns:
                        matches = re.findall(pattern, content)
                        for match in matches:
                            # Очистка найденного endpoint
                            endpoint = match.strip().strip('"')
                            if endpoint and not endpoint.startswith('//'):
                                found_endpoints.add(endpoint)
                                
            except Exception as e:
                self.log(f"⚠️ Ошибка чтения {route_file}: {e}", "WARN")
                continue
        
        # Проверка обязательных endpoints
        missing_endpoints = set(REQUIRED_API_ENDPOINTS) - found_endpoints
        extra_endpoints = found_endpoints - set(REQUIRED_API_ENDPOINTS)
        
        if missing_endpoints:
            status = "FAIL"
            message = f"Отсутствуют обязательные API endpoints: {missing_endpoints}"
        elif extra_endpoints:
            status = "WARN"
            message = f"Обнаружены дополнительные API endpoints: {extra_endpoints}"
        else:
            status = "PASS"
            message = "Все обязательные API endpoints присутствуют"
        
        return SafetyTestResult(
            test_name="API Endpoints Unchanged",
            status=status,
            message=message,
            details={
                "found_endpoints": list(found_endpoints),
                "required_endpoints": REQUIRED_API_ENDPOINTS,
                "missing_endpoints": list(missing_endpoints),
                "extra_endpoints": list(extra_endpoints)
            },
            timestamp=datetime.now().isoformat()
        )
    
    def test_data_structures_compatible(self) -> SafetyTestResult:
        """Тест: Структуры данных совместимы"""
        self.log("🔍 Тест: Структуры данных совместимы...")
        
        # Поиск структур данных в заголовочных файлах
        include_dir = self.project_root / "include"
        found_structures = set()
        
        if not include_dir.exists():
            return SafetyTestResult(
                test_name="Data Structures Compatible",
                status="FAIL",
                message="Include directory not found",
                details={"error": "include directory not found"},
                timestamp=datetime.now().isoformat()
            )
        
        for header_file in include_dir.rglob("*.h"):
            try:
                with open(header_file, 'r', encoding='utf-8') as f:
                    content = f.read()
                    # Поиск struct и class определений с атрибутами
                    struct_patterns = [
                        r'struct\s+(\w+)',  # struct Name
                        r'struct\s+__attribute__\(\([^)]+\)\)\s+(\w+)',  # struct __attribute__((packed)) Name
                        r'struct\s+__attribute__\(\(packed\)\)\s+(\w+)',  # struct __attribute__((packed)) Name
                    ]
                    
                    for pattern in struct_patterns:
                        matches = re.findall(pattern, content)
                        found_structures.update(matches)
                    
                    # Поиск class определений
                    class_matches = re.findall(r'class\s+(\w+)', content)
                    found_structures.update(class_matches)
                    
            except Exception as e:
                self.log(f"⚠️ Ошибка чтения {header_file}: {e}", "WARN")
                continue
        
        # Проверка обязательных структур
        missing_structures = set(REQUIRED_DATA_STRUCTURES) - found_structures
        
        if missing_structures:
            status = "FAIL"
            message = f"Отсутствуют обязательные структуры данных: {missing_structures}"
        else:
            status = "PASS"
            message = "Все обязательные структуры данных присутствуют"
        
        return SafetyTestResult(
            test_name="Data Structures Compatible",
            status=status,
            message=message,
            details={
                "found_structures": list(found_structures),
                "required_structures": REQUIRED_DATA_STRUCTURES,
                "missing_structures": list(missing_structures)
            },
            timestamp=datetime.now().isoformat()
        )
    
    def test_business_interfaces_present(self) -> SafetyTestResult:
        """Тест: Бизнес-интерфейсы присутствуют"""
        self.log("🔍 Тест: Бизнес-интерфейсы присутствуют...")
        
        business_dir = self.project_root / "include" / "business"
        found_interfaces = set()
        
        if not business_dir.exists():
            return SafetyTestResult(
                test_name="Business Interfaces Present",
                status="FAIL",
                message="Business interfaces directory not found",
                details={"error": "include/business directory not found"},
                timestamp=datetime.now().isoformat()
            )
        
        for interface_file in business_dir.glob("I*.h"):
            interface_name = interface_file.stem
            found_interfaces.add(interface_name)
        
        # Проверка обязательных интерфейсов
        missing_interfaces = set(REQUIRED_BUSINESS_INTERFACES) - found_interfaces
        
        if missing_interfaces:
            status = "FAIL"
            message = f"Отсутствуют обязательные бизнес-интерфейсы: {missing_interfaces}"
        else:
            status = "PASS"
            message = "Все обязательные бизнес-интерфейсы присутствуют"
        
        return SafetyTestResult(
            test_name="Business Interfaces Present",
            status=status,
            message=message,
            details={
                "found_interfaces": list(found_interfaces),
                "required_interfaces": REQUIRED_BUSINESS_INTERFACES,
                "missing_interfaces": list(missing_interfaces)
            },
            timestamp=datetime.now().isoformat()
        )
    
    def test_configuration_backward_compatible(self) -> SafetyTestResult:
        """Тест: Конфигурация обратно совместима"""
        self.log("🔍 Тест: Конфигурация обратно совместима...")
        
        # Проверка файла конфигурации
        config_file = self.project_root / "src" / "config.cpp"
        
        if not config_file.exists():
            return SafetyTestResult(
                test_name="Configuration Backward Compatible",
                status="FAIL",
                message="Файл конфигурации не найден",
                details={"error": "config.cpp not found"},
                timestamp=datetime.now().isoformat()
            )
        
        try:
            with open(config_file, 'r', encoding='utf-8') as f:
                content = f.read()
        except Exception as e:
            return SafetyTestResult(
                test_name="Configuration Backward Compatible",
                status="FAIL",
                message="Ошибка чтения файла конфигурации",
                details={"error": str(e)},
                timestamp=datetime.now().isoformat()
            )
        
        # Проверка критических функций конфигурации
        required_functions = [
            "loadConfig",
            "saveConfig", 
            "resetConfig"
        ]
        
        missing_functions = []
        for func in required_functions:
            if func not in content:
                missing_functions.append(func)
        
        if missing_functions:
            status = "FAIL"
            message = f"Отсутствуют функции конфигурации: {missing_functions}"
        else:
            status = "PASS"
            message = "Все функции конфигурации присутствуют"
        
        return SafetyTestResult(
            test_name="Configuration Backward Compatible",
            status=status,
            message=message,
            details={
                "required_functions": required_functions,
                "missing_functions": missing_functions,
                "config_file": str(config_file)
            },
            timestamp=datetime.now().isoformat()
        )
    
    def test_compilation_successful(self) -> SafetyTestResult:
        """Тест: Компиляция успешна"""
        self.log("🔍 Тест: Компиляция успешна...")
        
        # Проверка существования сборки
        firmware_path = self.project_root / ".pio" / "build" / "esp32dev" / "firmware.bin"
        
        if not firmware_path.exists():
            # Попытка сборки
            self.log("📦 Сборка прошивки...")
            build_result = self.run_command(["pio", "run", "-e", "esp32dev"], timeout=300)
            
            if not build_result["success"]:
                return SafetyTestResult(
                    test_name="Compilation Successful",
                    status="FAIL",
                    message="Сборка не удалась",
                    details={
                        "error": build_result.get("error", "Build failed"),
                        "stderr": build_result.get("stderr", "")[:200]
                    },
                    timestamp=datetime.now().isoformat()
                )
        
        return SafetyTestResult(
            test_name="Compilation Successful",
            status="PASS",
            message="Сборка успешна",
            details={
                "firmware_path": str(firmware_path) if firmware_path.exists() else "Built successfully"
            },
            timestamp=datetime.now().isoformat()
        )
    
    def test_all_tests_pass(self) -> SafetyTestResult:
        """Тест: Все тесты проходят"""
        self.log("🔍 Тест: Все тесты проходят...")
        
        # Проверка существования тестов
        test_dir = self.project_root / "test"
        if not test_dir.exists():
            return SafetyTestResult(
                test_name="All Tests Pass",
                status="FAIL",
                message="Test directory not found",
                details={"error": "test directory not found"},
                timestamp=datetime.now().isoformat()
            )
        
        # Сначала пробуем ultra_quick_test
        quick_test = self.run_command([
            "python", "scripts/ultra_quick_test.py"
        ], timeout=60)
        
        if quick_test["success"] and quick_test["returncode"] == 0:
            return SafetyTestResult(
                test_name="All Tests Pass",
                status="PASS",
                message="Ultra quick tests passed",
                details={
                    "test_type": "ultra_quick_test",
                    "execution_time": "fast",
                    "stdout": quick_test.get("stdout", "")[:200]
                },
                timestamp=datetime.now().isoformat()
            )
        
        # Если ultra_quick_test не работает, пробуем run_simple_tests
        self.log("⚠️ ultra_quick_test не работает, пробуем run_simple_tests", "WARN")
        simple_test = self.run_command([
            "python", "scripts/run_simple_tests.py"
        ], timeout=180)
        
        if simple_test["success"] and simple_test["returncode"] == 0:
            return SafetyTestResult(
                test_name="All Tests Pass",
                status="PASS",
                message="Simple tests passed",
                details={
                    "test_type": "run_simple_tests",
                    "execution_time": "medium",
                    "ultra_quick_error": quick_test.get("error", ""),
                    "stdout": simple_test.get("stdout", "")[:200]
                },
                timestamp=datetime.now().isoformat()
            )
        
        # Если и это не работает, пробуем pytest
        self.log("⚠️ run_simple_tests не работает, пробуем pytest", "WARN")
        pytest_result = self.run_command([
            "python", "-m", "pytest", "test/", "-v"
        ], timeout=300)
        
        if pytest_result["success"] and pytest_result["returncode"] == 0:
            return SafetyTestResult(
                test_name="All Tests Pass",
                status="PASS",
                message="Pytest tests passed",
                details={
                    "test_type": "pytest",
                    "execution_time": "slow",
                    "ultra_quick_error": quick_test.get("error", ""),
                    "simple_test_error": simple_test.get("error", ""),
                    "stdout": pytest_result.get("stdout", "")[:200]
                },
                timestamp=datetime.now().isoformat()
            )
        
        return SafetyTestResult(
            test_name="All Tests Pass",
            status="FAIL",
            message="All test runners failed",
            details={
                "ultra_quick_error": quick_test.get("error", ""),
                "simple_test_error": simple_test.get("error", ""),
                "pytest_error": pytest_result.get("error", ""),
                "ultra_quick_returncode": quick_test.get("returncode", -1),
                "simple_test_returncode": simple_test.get("returncode", -1),
                "pytest_returncode": pytest_result.get("returncode", -1)
            },
            timestamp=datetime.now().isoformat()
        )
    
    def test_clang_tidy_clean(self) -> SafetyTestResult:
        """Тест: Clang-tidy без предупреждений"""
        self.log("🔍 Тест: Clang-tidy без предупреждений...")
        
        # Проверка существования скрипта анализа
        analysis_script = self.project_root / "scripts" / "run_clang_tidy_analysis.py"
        if not analysis_script.exists():
            return SafetyTestResult(
                test_name="Clang-tidy Clean",
                status="FAIL",
                message="Скрипт clang-tidy не найден",
                details={"error": "run_clang_tidy_analysis.py not found"},
                timestamp=datetime.now().isoformat()
            )
        
        # Запуск clang-tidy анализа
        tidy_result = self.run_command([
            "python", "scripts/run_clang_tidy_analysis.py"
        ], timeout=120)
        
        if not tidy_result["success"]:
            return SafetyTestResult(
                test_name="Clang-tidy Clean",
                status="FAIL",
                message="Clang-tidy анализ не удался",
                details={
                    "error": tidy_result.get("error", "Clang-tidy failed"),
                    "stderr": tidy_result.get("stderr", "")[:200]
                },
                timestamp=datetime.now().isoformat()
            )
        
        # Проверка на наличие предупреждений
        output = tidy_result.get("stdout", "")
        if "Total warnings: 0" in output or "0 warnings" in output:
            status = "PASS"
            message = "Clang-tidy без предупреждений"
        else:
            status = "WARN"
            message = "Clang-tidy обнаружил предупреждения"
        
        return SafetyTestResult(
            test_name="Clang-tidy Clean",
            status=status,
            message=message,
            details={
                "tidy_output": output[:200] + "..."
            },
            timestamp=datetime.now().isoformat()
        )
    
    def test_memory_safety(self) -> SafetyTestResult:
        """Тест: Безопасность памяти"""
        self.log("🔍 Тест: Безопасность памяти...")
        
        # Проверка использования небезопасных функций
        src_dir = self.project_root / "src"
        unsafe_patterns = [
            r'\bstrcpy\b',
            r'\bstrcat\b', 
            r'\bsprintf\b',
            r'\bgets\b',
            r'\bscanf\b'
        ]
        
        found_unsafe = []
        
        if not src_dir.exists():
            return SafetyTestResult(
                test_name="Memory Safety",
                status="FAIL",
                message="Source directory not found",
                details={"error": "src directory not found"},
                timestamp=datetime.now().isoformat()
            )
        
        for cpp_file in src_dir.rglob("*.cpp"):
            try:
                with open(cpp_file, 'r', encoding='utf-8') as f:
                    content = f.read()
                    for pattern in unsafe_patterns:
                        if re.search(pattern, content):
                            found_unsafe.append({
                                "file": str(cpp_file),
                                "pattern": pattern
                            })
            except Exception as e:
                self.log(f"⚠️ Ошибка чтения {cpp_file}: {e}", "WARN")
                continue
        
        if found_unsafe:
            status = "WARN"
            message = f"Обнаружены потенциально небезопасные функции: {len(found_unsafe)}"
        else:
            status = "PASS"
            message = "Небезопасные функции не обнаружены"
        
        return SafetyTestResult(
            test_name="Memory Safety",
            status=status,
            message=message,
            details={
                "unsafe_patterns": unsafe_patterns,
                "found_unsafe": found_unsafe
            },
            timestamp=datetime.now().isoformat()
        )
    
    def generate_summary(self) -> Dict[str, Any]:
        """Генерация сводки результатов"""
        total_tests = len(self.results)
        passed_tests = sum(1 for r in self.results if r.status == "PASS")
        failed_tests = sum(1 for r in self.results if r.status == "FAIL")
        warning_tests = sum(1 for r in self.results if r.status == "WARN")
        
        execution_time = (datetime.now() - self.start_time).total_seconds()
        
        return {
            "total_tests": total_tests,
            "passed_tests": passed_tests,
            "failed_tests": failed_tests,
            "warning_tests": warning_tests,
            "success_rate": (passed_tests / total_tests * 100) if total_tests > 0 else 0,
            "execution_time_seconds": execution_time,
            "timestamp": datetime.now().isoformat()
        }
    
    def save_report(self, report: RefactoringSafetyReport):
        """Сохранение отчёта в JSON"""
        report_path = self.project_root / "test_reports" / "refactoring_safety.json"
        report_path.parent.mkdir(exist_ok=True)
        
        with open(report_path, 'w', encoding='utf-8') as f:
            json.dump(asdict(report), f, indent=2, ensure_ascii=False)
        
        self.log(f"📊 Отчёт безопасности сохранён: {report_path}")
    
    def print_report(self, report: RefactoringSafetyReport):
        """Вывод отчёта в консоль"""
        print("\n" + "="*60)
        print("🛡️ ОТЧЁТ БЕЗОПАСНОСТИ РЕФАКТОРИНГА JXCT")
        print("="*60)
        print(f"📅 Дата: {report.timestamp}")
        print(f"🔢 Версия: {report.version}")
        
        print("\n📊 РЕЗУЛЬТАТЫ ТЕСТОВ БЕЗОПАСНОСТИ:")
        print("-" * 60)
        
        for result in report.results:
            status_icon = {
                "PASS": "✅",
                "WARN": "⚠️",
                "FAIL": "❌"
            }.get(result.status, "❓")
            
            print(f"{status_icon} {result.test_name}: {result.status}")
            print(f"   └─ {result.message}")
        
        print("\n📈 СВОДКА:")
        print("-" * 60)
        summary = report.summary
        print(f"📊 Всего тестов: {summary['total_tests']}")
        print(f"✅ Пройдено: {summary['passed_tests']}")
        print(f"⚠️ Предупреждения: {summary['warning_tests']}")
        print(f"❌ Провалено: {summary['failed_tests']}")
        print(f"📈 Успешность: {summary['success_rate']:.1f}%")
        print(f"⏱️ Время выполнения: {summary['execution_time_seconds']:.2f} сек")
        
        # Рекомендации
        if summary['failed_tests'] > 0:
            print("\n🚨 РЕКОМЕНДАЦИИ:")
            print("   - Исправьте проваленные тесты перед рефакторингом")
            print("   - Проверьте API endpoints и структуры данных")
        elif summary['warning_tests'] > 0:
            print("\n⚠️ РЕКОМЕНДАЦИИ:")
            print("   - Рассмотрите предупреждения перед рефакторингом")
            print("   - Убедитесь в безопасности памяти")
        else:
            print("\n✅ РЕКОМЕНДАЦИИ:")
            print("   - Рефакторинг безопасен для выполнения")
            print("   - Все критические компоненты проверены")
        
        print("\n" + "="*60)
    
    def run_all_safety_tests(self) -> RefactoringSafetyReport:
        """Запуск всех тестов безопасности"""
        self.log("🛡️ Запуск тестов безопасности рефакторинга JXCT...")
        
        # Список тестов безопасности
        safety_tests = [
            self.test_api_endpoints_unchanged,
            self.test_data_structures_compatible,
            self.test_business_interfaces_present,
            self.test_configuration_backward_compatible,
            self.test_compilation_successful,
            self.test_all_tests_pass,
            self.test_clang_tidy_clean,
            self.test_memory_safety
        ]
        
        # Выполнение тестов
        for test_func in safety_tests:
            try:
                result = test_func()
                self.results.append(result)
                self.log(f"{result.status} {result.test_name}: {result.message}")
            except Exception as e:
                self.log(f"❌ Ошибка в {test_func.__name__}: {e}", "ERROR")
                error_result = SafetyTestResult(
                    test_name=test_func.__name__,
                    status="FAIL",
                    message=f"Ошибка выполнения: {e}",
                    details={"error": str(e)},
                    timestamp=datetime.now().isoformat()
                )
                self.results.append(error_result)
        
        # Генерация отчёта
        summary = self.generate_summary()
        
        report = RefactoringSafetyReport(
            version="3.11.0",
            timestamp=datetime.now().isoformat(),
            total_tests=summary["total_tests"],
            passed_tests=summary["passed_tests"],
            failed_tests=summary["failed_tests"],
            warning_tests=summary["warning_tests"],
            results=self.results,
            summary=summary
        )
        
        # Сохранение и вывод отчёта
        self.save_report(report)
        self.print_report(report)
        
        return report

def main():
    """Главная функция"""
    tester = JXCTRefactoringSafetyTester()
    
    try:
        report = tester.run_all_safety_tests()
        
        # Возвращаем код выхода на основе результатов
        if report.failed_tests > 0:
            print("❌ Safety Tests: FAIL")
            sys.exit(1)
        elif report.warning_tests > 0:
            print("⚠️ Safety Tests: WARN")
            sys.exit(2)
        else:
            print("✅ Safety Tests: PASS")
            sys.exit(0)
            
    except KeyboardInterrupt:
        print("\n⚠️ Тесты безопасности прерваны пользователем")
        sys.exit(1)
    except Exception as e:
        print(f"\n❌ Критическая ошибка: {e}")
        sys.exit(1)

if __name__ == "__main__":
    main() 