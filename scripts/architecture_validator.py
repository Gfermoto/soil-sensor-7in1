#!/usr/bin/env python3
"""
Architecture Validator for JXCT Soil Sensor Project
Валидатор архитектуры для IoT системы на ESP32

Author: JXCT Development Team
Version: 3.11.0
License: MIT
"""

import json
import re
import sys
from pathlib import Path
from typing import Dict, List, Any, Optional, Set, Tuple
from dataclasses import dataclass, asdict
from datetime import datetime

# Константы архитектурных принципов
MAX_FILE_SIZE_LINES = 500
MAX_MAIN_SIZE_LINES = 200
MIN_INTERFACE_COUNT = 3
MAX_CYCLIC_DEPENDENCIES = 0
MAX_DUPLICATE_CODE_RATIO = 0.1  # 10%

@dataclass
class ArchitectureViolation:
    """Нарушение архитектурного принципа"""
    principle: str
    severity: str  # HIGH, MEDIUM, LOW
    file: str
    line: Optional[int]
    message: str
    details: Dict[str, Any]
    
    def __post_init__(self):
        """Проверка обязательных полей"""
        if not hasattr(self, 'principle'):
            self.principle = "Unknown"
        if not hasattr(self, 'details'):
            self.details = {}

@dataclass
class ArchitectureReport:
    """Отчёт о архитектуре"""
    version: str
    timestamp: str
    total_violations: int
    high_violations: int
    medium_violations: int
    low_violations: int
    violations: List[ArchitectureViolation]
    metrics: Dict[str, Any]
    summary: Dict[str, Any]

class JXCTArchitectureValidator:
    """Профессиональный валидатор архитектуры для JXCT"""
    
    def __init__(self):
        self.project_root = Path(__file__).parent.parent
        self.violations: List[ArchitectureViolation] = []
        self.start_time = datetime.now()
        
    def log(self, message: str, level: str = "INFO"):
        """Логирование с временными метками"""
        timestamp = datetime.now().strftime("%H:%M:%S.%f")[:-3]
        print(f"[{timestamp}] {level}: {message}")
    
    def validate_single_responsibility(self) -> List[ArchitectureViolation]:
        """Валидация принципа единственной ответственности"""
        self.log("🔍 Валидация: Принцип единственной ответственности...")
        violations = []
        
        src_dir = self.project_root / "src"
        if not src_dir.exists():
            return [ArchitectureViolation(
                principle="Single Responsibility",
                severity="HIGH",
                file="src/",
                line=0,
                message="Source directory not found",
                details={"error": "src directory not found"}
            )]
        
        try:
            for cpp_file in src_dir.rglob("*.cpp"):
                try:
                    with open(cpp_file, 'r', encoding='utf-8') as f:
                        content = f.read()
                        
                    # Подсчет функций в файле
                    function_count = len(re.findall(r'\w+\s+\w+\s*\([^)]*\)\s*\{', content))
                    
                    if function_count > 20:  # Порог для единственной ответственности
                        violations.append(ArchitectureViolation(
                            principle="Single Responsibility",
                            severity="MEDIUM",
                            file=str(cpp_file.relative_to(self.project_root)),
                            line=0,
                            message=f"Файл содержит {function_count} функций (порог: 20)",
                            details={
                                "function_count": function_count,
                                "threshold": 20,
                                "recommendation": "Разделить файл на модули"
                            }
                        ))
                        
                except Exception as e:
                    self.log(f"⚠️ Ошибка чтения {cpp_file}: {e}", "WARN")
                    continue
                    
        except Exception as e:
            self.log(f"❌ Ошибка валидации SRP: {e}", "ERROR")
            
        self.log(f"✅ validate_single_responsibility: {len(violations)} нарушений")
        return violations
    
    def validate_dependency_inversion(self) -> List[ArchitectureViolation]:
        """Проверка инверсии зависимостей"""
        self.log("🔍 Валидация: Инверсия зависимостей...")
        
        violations = []
        include_dir = self.project_root / "include"
        
        if not include_dir.exists():
            violations.append(ArchitectureViolation(
                principle="Dependency Inversion",
                severity="HIGH",
                file="include/",
                line=None,
                message="Include directory not found",
                details={"error": "include directory not found"}
            ))
            return violations
        
        # Проверка наличия интерфейсов
        interface_files = list(include_dir.rglob("I*.h"))
        if len(interface_files) < MIN_INTERFACE_COUNT:
            violations.append(ArchitectureViolation(
                principle="Dependency Inversion",
                severity="HIGH",
                file="include/",
                line=None,
                message=f"Недостаточно интерфейсов: {len(interface_files)} (минимум {MIN_INTERFACE_COUNT})",
                details={
                    "interface_count": len(interface_files),
                    "min_count": MIN_INTERFACE_COUNT,
                    "found_interfaces": [f.name for f in interface_files]
                }
            ))
        
        # Проверка использования интерфейсов
        src_dir = self.project_root / "src"
        interface_usage = {}
        
        if src_dir.exists():
            for interface_file in interface_files:
                interface_name = interface_file.stem
                usage_count = 0
                
                for cpp_file in src_dir.rglob("*.cpp"):
                    try:
                        with open(cpp_file, 'r', encoding='utf-8') as f:
                            content = f.read()
                            if interface_name in content:
                                usage_count += 1
                    except Exception as e:
                        self.log(f"⚠️ Ошибка чтения {cpp_file}: {e}", "WARN")
                        continue
                
                interface_usage[interface_name] = usage_count
            
            # Проверка неиспользуемых интерфейсов
            for interface_name, usage_count in interface_usage.items():
                if usage_count == 0:
                    violations.append(ArchitectureViolation(
                        principle="Dependency Inversion",
                        severity="MEDIUM",
                        file=f"include/{interface_name}.h",
                        line=None,
                        message=f"Интерфейс {interface_name} не используется",
                        details={
                            "interface_name": interface_name,
                            "usage_count": usage_count,
                            "recommendation": "Удалить или использовать интерфейс"
                        }
                    ))
        
        return violations
    
    def validate_interface_segregation(self) -> List[ArchitectureViolation]:
        """Проверка разделения интерфейсов"""
        self.log("🔍 Валидация: Разделение интерфейсов...")
        
        violations = []
        include_dir = self.project_root / "include"
        
        if not include_dir.exists():
            violations.append(ArchitectureViolation(
                principle="Interface Segregation",
                severity="HIGH",
                file="include/",
                line=None,
                message="Include directory not found",
                details={"error": "include directory not found"}
            ))
            return violations
        
        for interface_file in include_dir.rglob("I*.h"):
            try:
                with open(interface_file, 'r', encoding='utf-8') as f:
                    content = f.read()
                    
                    # Подсчёт методов в интерфейсе
                    method_count = len(re.findall(r'virtual\s+\w+\s+\w+\s*\([^)]*\)\s*=\s*0', content))
                    
                    if method_count > 10:
                        violations.append(ArchitectureViolation(
                            principle="Interface Segregation",
                            severity="MEDIUM",
                            file=str(interface_file),
                            line=None,
                            message=f"Интерфейс слишком большой: {method_count} методов",
                            details={
                                "interface_name": interface_file.stem,
                                "method_count": method_count,
                                "recommendation": "Разделить интерфейс на более мелкие"
                            }
                        ))
            except Exception as e:
                self.log(f"⚠️ Ошибка чтения {interface_file}: {e}", "WARN")
                continue
        
        return violations
    
    def validate_open_closed_principle(self) -> List[ArchitectureViolation]:
        """Проверка принципа открытости/закрытости"""
        self.log("🔍 Валидация: Принцип открытости/закрытости...")
        
        violations = []
        src_dir = self.project_root / "src"
        
        if not src_dir.exists():
            violations.append(ArchitectureViolation(
                principle="Open/Closed Principle",
                severity="HIGH",
                file="src/",
                line=None,
                message="Source directory not found",
                details={"error": "src directory not found"}
            ))
            return violations
        
        for cpp_file in src_dir.rglob("*.cpp"):
            try:
                with open(cpp_file, 'r', encoding='utf-8') as f:
                    content = f.read()
                    
                    # Поиск switch/case конструкций (потенциальные нарушения OCP)
                    switch_matches = re.finditer(r'switch\s*\([^)]+\)\s*\{', content)
                    for match in switch_matches:
                        line_number = content[:match.start()].count('\n') + 1
                        violations.append(ArchitectureViolation(
                            principle="Open/Closed Principle",
                            severity="LOW",
                            file=str(cpp_file),
                            line=line_number,
                            message="Switch/case конструкция может нарушать OCP",
                            details={
                                "switch_statement": match.group(),
                                "recommendation": "Рассмотреть использование полиморфизма"
                            }
                        ))
            except Exception as e:
                self.log(f"⚠️ Ошибка чтения {cpp_file}: {e}", "WARN")
                continue
        
        return violations
    
    def validate_liskov_substitution(self) -> List[ArchitectureViolation]:
        """Проверка принципа подстановки Лисков"""
        self.log("🔍 Валидация: Принцип подстановки Лисков...")
        
        violations = []
        include_dir = self.project_root / "include"
        
        if not include_dir.exists():
            violations.append(ArchitectureViolation(
                principle="Liskov Substitution",
                severity="HIGH",
                file="include/",
                line=None,
                message="Include directory not found",
                details={"error": "include directory not found"}
            ))
            return violations
        
        # Поиск наследования
        for header_file in include_dir.rglob("*.h"):
            try:
                with open(header_file, 'r', encoding='utf-8') as f:
                    content = f.read()
                    
                    # Поиск public наследования
                    inheritance_matches = re.findall(r'class\s+(\w+)\s*:\s*public\s+(\w+)', content)
                    
                    for derived, base in inheritance_matches:
                        # Проверка виртуального деструктора в базовом классе
                        if f"class {base}" in content:
                            if f"virtual ~{base}" not in content:
                                violations.append(ArchitectureViolation(
                                    principle="Liskov Substitution",
                                    severity="MEDIUM",
                                    file=str(header_file),
                                    line=None,
                                    message=f"Базовый класс {base} должен иметь виртуальный деструктор",
                                    details={
                                        "base_class": base,
                                        "derived_class": derived,
                                        "recommendation": "Добавить virtual ~BaseClass() = default;"
                                    }
                                ))
            except Exception as e:
                self.log(f"⚠️ Ошибка чтения {header_file}: {e}", "WARN")
                continue
        
        return violations
    
    def validate_cyclic_dependencies(self) -> List[ArchitectureViolation]:
        """Проверка циклических зависимостей"""
        self.log("🔍 Валидация: Циклические зависимости...")
        
        violations = []
        include_dir = self.project_root / "include"
        
        if not include_dir.exists():
            violations.append(ArchitectureViolation(
                principle="Cyclic Dependencies",
                severity="HIGH",
                file="include/",
                line=None,
                message="Include directory not found",
                details={"error": "include directory not found"}
            ))
            return violations
        
        # Простая проверка на циклические include
        dependencies = {}
        
        for header_file in include_dir.rglob("*.h"):
            try:
                with open(header_file, 'r', encoding='utf-8') as f:
                    content = f.read()
                    
                    # Поиск #include директив
                    includes = re.findall(r'#include\s*["<]([^">]+)[">]', content)
                    dependencies[header_file.name] = [inc for inc in includes if inc.endswith('.h')]
            except Exception as e:
                self.log(f"⚠️ Ошибка чтения {header_file}: {e}", "WARN")
                continue
        
        # Простая проверка на циклические зависимости
        for file_name, deps in dependencies.items():
            for dep in deps:
                if dep in dependencies and file_name in dependencies.get(dep, []):
                    violations.append(ArchitectureViolation(
                        principle="Cyclic Dependencies",
                        severity="HIGH",
                        file=f"include/{file_name}",
                        line=None,
                        message=f"Циклическая зависимость между {file_name} и {dep}",
                        details={
                            "file1": file_name,
                            "file2": dep,
                            "recommendation": "Разорвать циклическую зависимость"
                        }
                    ))
        
        return violations
    
    def validate_main_file_size(self) -> List[ArchitectureViolation]:
        """Проверка размера main.cpp"""
        self.log("🔍 Валидация: Размер main.cpp...")
        
        violations = []
        main_file = self.project_root / "src" / "main.cpp"
        
        if not main_file.exists():
            violations.append(ArchitectureViolation(
                principle="Main File Size",
                severity="HIGH",
                file=str(main_file),
                line=None,
                message="Main file not found",
                details={"error": "main.cpp not found"}
            ))
            return violations
        
        try:
            with open(main_file, 'r', encoding='utf-8') as f:
                content = f.read()
                lines = content.split('\n')
                
                if len(lines) > MAX_MAIN_SIZE_LINES:
                    violations.append(ArchitectureViolation(
                        principle="Main File Size",
                        severity="HIGH",
                        file=str(main_file),
                        line=None,
                        message=f"main.cpp слишком большой: {len(lines)} строк (максимум {MAX_MAIN_SIZE_LINES})",
                        details={
                            "file_size": len(lines),
                            "max_size": MAX_MAIN_SIZE_LINES,
                            "recommendation": "Вынести функциональность в отдельные модули"
                        }
                    ))
        except Exception as e:
            violations.append(ArchitectureViolation(
                principle="Main File Size",
                severity="HIGH",
                file=str(main_file),
                line=None,
                message="Ошибка чтения main.cpp",
                details={"error": str(e)}
            ))
        
        return violations
    
    def validate_duplicate_code(self) -> List[ArchitectureViolation]:
        """Проверка дублирования кода"""
        self.log("🔍 Валидация: Дублирование кода...")
        
        violations = []
        src_dir = self.project_root / "src"
        
        if not src_dir.exists():
            violations.append(ArchitectureViolation(
                principle="Duplicate Code",
                severity="HIGH",
                file="src/",
                line=None,
                message="Source directory not found",
                details={"error": "src directory not found"}
            ))
            return violations
        
        # Простая проверка на дублирование функций
        function_signatures = {}
        
        for cpp_file in src_dir.rglob("*.cpp"):
            try:
                with open(cpp_file, 'r', encoding='utf-8') as f:
                    content = f.read()
                    
                    # Поиск определений функций
                    function_matches = re.findall(r'(\w+\s+\w+\s*\([^)]*\))\s*\{', content)
                    
                    for signature in function_matches:
                        if signature in function_signatures:
                            function_signatures[signature].append(str(cpp_file))
                        else:
                            function_signatures[signature] = [str(cpp_file)]
            except Exception as e:
                self.log(f"⚠️ Ошибка чтения {cpp_file}: {e}", "WARN")
                continue
        
        # Проверка дублирования
        duplicate_functions = {sig: files for sig, files in function_signatures.items() if len(files) > 1}
        
        if duplicate_functions:
            violations.append(ArchitectureViolation(
                principle="Duplicate Code",
                severity="MEDIUM",
                file="src/",
                line=None,
                message=f"Обнаружено {len(duplicate_functions)} дублированных функций",
                details={
                    "duplicate_count": len(duplicate_functions),
                    "duplicates": list(duplicate_functions.keys())[:5],  # Первые 5
                    "recommendation": "Вынести дублированный код в общие функции"
                }
            ))
        
        return violations
    
    def calculate_metrics(self) -> Dict[str, Any]:
        """Расчёт архитектурных метрик"""
        self.log("📊 Расчёт архитектурных метрик...")
        
        src_dir = self.project_root / "src"
        include_dir = self.project_root / "include"
        
        # Подсчёт файлов
        cpp_files = list(src_dir.rglob("*.cpp")) if src_dir.exists() else []
        header_files = list(include_dir.rglob("*.h")) if include_dir.exists() else []
        
        # Подсчёт строк кода
        total_lines = 0
        file_sizes = {}
        
        for cpp_file in cpp_files:
            try:
                with open(cpp_file, 'r', encoding='utf-8') as f:
                    lines = len(f.readlines())
                    total_lines += lines
                    file_sizes[cpp_file.name] = lines
            except Exception as e:
                self.log(f"⚠️ Ошибка чтения {cpp_file}: {e}", "WARN")
                continue
        
        # Подсчёт интерфейсов
        interface_files = list(include_dir.rglob("I*.h")) if include_dir.exists() else []
        
        # Подсчёт классов
        total_classes = 0
        for header_file in header_files:
            try:
                with open(header_file, 'r', encoding='utf-8') as f:
                    content = f.read()
                    total_classes += len(re.findall(r'class\s+\w+', content))
            except Exception as e:
                self.log(f"⚠️ Ошибка чтения {header_file}: {e}", "WARN")
                continue
        
        return {
            "total_files": len(cpp_files) + len(header_files),
            "cpp_files": len(cpp_files),
            "header_files": len(header_files),
            "total_lines": total_lines,
            "interface_count": len(interface_files),
            "class_count": total_classes,
            "average_file_size": total_lines / len(cpp_files) if cpp_files else 0,
            "largest_file": max(file_sizes.values()) if file_sizes else 0,
            "largest_file_name": max(file_sizes, key=file_sizes.get) if file_sizes else "N/A"
        }
    
    def generate_summary(self) -> Dict[str, Any]:
        """Генерация сводки результатов"""
        total_violations = len(self.violations)
        high_violations = sum(1 for v in self.violations if v.severity == "HIGH")
        medium_violations = sum(1 for v in self.violations if v.severity == "MEDIUM")
        low_violations = sum(1 for v in self.violations if v.severity == "LOW")
        
        execution_time = (datetime.now() - self.start_time).total_seconds()
        
        return {
            "total_violations": total_violations,
            "high_violations": high_violations,
            "medium_violations": medium_violations,
            "low_violations": low_violations,
            "execution_time_seconds": execution_time,
            "timestamp": datetime.now().isoformat()
        }
    
    def save_report(self, report: ArchitectureReport):
        """Сохранение отчёта в JSON"""
        report_path = self.project_root / "test_reports" / "architecture_validation.json"
        report_path.parent.mkdir(exist_ok=True)
        
        with open(report_path, 'w', encoding='utf-8') as f:
            json.dump(asdict(report), f, indent=2, ensure_ascii=False)
        
        self.log(f"📊 Отчёт архитектуры сохранён: {report_path}")
    
    def print_report(self, report: ArchitectureReport):
        """Вывод отчёта в консоль"""
        print("\n" + "="*60)
        print("🏗️ ОТЧЁТ ВАЛИДАЦИИ АРХИТЕКТУРЫ JXCT")
        print("="*60)
        print(f"📅 Дата: {report.timestamp}")
        print(f"🔢 Версия: {report.version}")
        
        print("\n📊 АРХИТЕКТУРНЫЕ МЕТРИКИ:")
        print("-" * 60)
        metrics = report.metrics
        print(f"📁 Всего файлов: {metrics['total_files']}")
        print(f"📄 C++ файлов: {metrics['cpp_files']}")
        print(f"📋 Заголовочных файлов: {metrics['header_files']}")
        print(f"📝 Всего строк: {metrics['total_lines']:,}")
        print(f"🔗 Интерфейсов: {metrics['interface_count']}")
        print(f"🏗️ Классов: {metrics['class_count']}")
        print(f"📏 Средний размер файла: {metrics['average_file_size']:.1f} строк")
        print(f"📏 Самый большой файл: {metrics['largest_file_name']} ({metrics['largest_file']} строк)")
        
        print("\n🚨 НАРУШЕНИЯ АРХИТЕКТУРНЫХ ПРИНЦИПОВ:")
        print("-" * 60)
        
        for violation in report.violations:
            severity_icon = {
                "HIGH": "🔴",
                "MEDIUM": "🟡", 
                "LOW": "🟢"
            }.get(violation.severity, "❓")
            
            print(f"{severity_icon} {violation.principle}: {violation.severity}")
            print(f"   📁 {violation.file}")
            if violation.line:
                print(f"   📍 Строка: {violation.line}")
            print(f"   💬 {violation.message}")
        
        print("\n📈 СВОДКА НАРУШЕНИЙ:")
        print("-" * 60)
        summary = report.summary
        print(f"🚨 Всего нарушений: {summary['total_violations']}")
        print(f"🔴 Высокий приоритет: {summary['high_violations']}")
        print(f"🟡 Средний приоритет: {summary['medium_violations']}")
        print(f"🟢 Низкий приоритет: {summary['low_violations']}")
        print(f"⏱️ Время выполнения: {summary['execution_time_seconds']:.2f} сек")
        
        # Рекомендации
        if summary['high_violations'] > 0:
            print("\n🚨 КРИТИЧЕСКИЕ РЕКОМЕНДАЦИИ:")
            print("   - Исправьте нарушения высокого приоритета")
            print("   - Проверьте циклические зависимости")
            print("   - Уменьшите размер main.cpp")
        elif summary['medium_violations'] > 0:
            print("\n⚠️ РЕКОМЕНДАЦИИ:")
            print("   - Рассмотрите нарушения среднего приоритета")
            print("   - Улучшите разделение интерфейсов")
            print("   - Устраните дублирование кода")
        else:
            print("\n✅ АРХИТЕКТУРА В ПОРЯДКЕ:")
            print("   - Все принципы соблюдены")
            print("   - Можно продолжать рефакторинг")
        
        print("\n" + "="*60)
    
    def run_all_validations(self) -> ArchitectureReport:
        """Запуск всех валидаций архитектуры"""
        self.log("🏗️ Запуск валидации архитектуры JXCT...")
        
        # Список валидаций
        validations = [
            self.validate_single_responsibility,
            self.validate_dependency_inversion,
            self.validate_interface_segregation,
            self.validate_open_closed_principle,
            self.validate_liskov_substitution,
            self.validate_cyclic_dependencies,
            self.validate_main_file_size,
            self.validate_duplicate_code
        ]
        
        # Выполнение валидаций
        for validation_func in validations:
            try:
                violations = validation_func()
                self.violations.extend(violations)
                self.log(f"✅ {validation_func.__name__}: {len(violations)} нарушений")
            except Exception as e:
                self.log(f"❌ Ошибка в {validation_func.__name__}: {e}", "ERROR")
        
        # Расчёт метрик и сводки
        metrics = self.calculate_metrics()
        summary = self.generate_summary()
        
        report = ArchitectureReport(
            version="3.11.0",
            timestamp=datetime.now().isoformat(),
            total_violations=summary["total_violations"],
            high_violations=summary["high_violations"],
            medium_violations=summary["medium_violations"],
            low_violations=summary["low_violations"],
            violations=self.violations,
            metrics=metrics,
            summary=summary
        )
        
        # Сохранение и вывод отчёта
        self.save_report(report)
        self.print_report(report)
        
        return report

def main():
    """Главная функция"""
    validator = JXCTArchitectureValidator()
    
    try:
        report = validator.run_all_validations()
        
        # Возвращаем код выхода на основе результатов
        if report.summary["total_violations"] > 10:  # Много нарушений
            print("❌ Architecture Validation: FAIL")
            sys.exit(1)
        elif report.summary["total_violations"] > 0:  # Есть нарушения
            print("⚠️ Architecture Validation: WARN")
            sys.exit(2)
        else:
            print("✅ Architecture Validation: PASS")
            sys.exit(0)
            
    except KeyboardInterrupt:
        print("\n⚠️ Валидация прервана пользователем")
        sys.exit(1)
    except Exception as e:
        print(f"\n❌ Критическая ошибка: {e}")
        sys.exit(1)

if __name__ == "__main__":
    main() 