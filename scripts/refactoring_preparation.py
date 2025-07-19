#!/usr/bin/env python3
"""
Refactoring Preparation Suite for JXCT Soil Sensor Project
Интегрированный набор инструментов для подготовки к рефакторингу

Author: JXCT Development Team
Version: 3.11.0
License: MIT
"""

import json
import subprocess
import sys
import os
from pathlib import Path
from typing import Dict, List, Any
from dataclasses import dataclass, asdict
from datetime import datetime
import time

@dataclass
class PreparationResult:
    """Результат подготовки к рефакторингу"""
    tool_name: str
    status: str  # PASS, FAIL, WARN
    execution_time: float
    details: Dict[str, Any]

@dataclass
class PreparationReport:
    """Отчёт о подготовке к рефакторингу"""
    version: str
    timestamp: str
    total_tools: int
    passed_tools: int
    failed_tools: int
    warning_tools: int
    results: List[PreparationResult]
    summary: Dict[str, Any]
    recommendations: List[str]

class JXCTRefactoringPreparation:
    """Интегрированный набор инструментов для подготовки к рефакторингу"""
    
    def __init__(self):
        self.project_root = Path(__file__).parent.parent
        self.results: List[PreparationResult] = []
        self.start_time = datetime.now()
        
    def log(self, message: str, level: str = "INFO"):
        """Логирование с временными метками"""
        timestamp = datetime.now().strftime("%H:%M:%S.%f")[:-3]
        print(f"[{timestamp}] {level}: {message}")
    
    def run_tool(self, tool_name: str, command: List[str], timeout: int = 300) -> PreparationResult:
        """Запуск инструмента с измерением времени"""
        self.log(f"🔧 Запуск {tool_name}...")
        
        start_time = time.time()
        
        try:
            # Исправление для Windows PowerShell
            if command[0] == "python":
                # Для Python скриптов используем полный путь
                python_exe = sys.executable
                command = [python_exe] + command[1:]
            
            result = subprocess.run(
                command,
                capture_output=False,  # Убираем capture_output для избежания зависаний
                text=True,
                timeout=timeout,
                cwd=self.project_root,
                shell=False,  # Не используем shell для безопасности
                env=os.environ.copy()  # Передаем текущее окружение
            )
            
            execution_time = time.time() - start_time
            
            # Простая логика на основе returncode
            if result.returncode == 0:
                status = "PASS"
                message = f"{tool_name} выполнен успешно"
            elif result.returncode == 2:
                status = "WARN"
                message = f"{tool_name} с предупреждениями"
            else:
                status = "FAIL"
                message = f"{tool_name} не выполнен (код: {result.returncode})"
            
            return PreparationResult(
                tool_name=tool_name,
                status=status,
                execution_time=execution_time,
                details={
                    "returncode": result.returncode,
                    "stdout": "Output captured separately",  # Вывод захватывается отдельно
                    "stderr": "Errors captured separately",
                    "message": message
                }
            )
            
        except subprocess.TimeoutExpired:
            execution_time = time.time() - start_time
            return PreparationResult(
                tool_name=tool_name,
                status="FAIL",
                execution_time=execution_time,
                details={
                    "error": "Timeout",
                    "timeout": timeout,
                    "message": f"{tool_name} превысил лимит времени"
                }
            )
        except Exception as e:
            execution_time = time.time() - start_time
            return PreparationResult(
                tool_name=tool_name,
                status="FAIL",
                execution_time=execution_time,
                details={
                    "error": str(e),
                    "message": f"Ошибка выполнения {tool_name}"
                }
            )
    
    def run_performance_benchmarks(self) -> PreparationResult:
        """Запуск бенчмарков производительности"""
        return self.run_tool(
            "Performance Benchmarks",
            ["python", "scripts/performance_benchmark.py"],
            timeout=300  # 5 минут
        )
    
    def run_safety_tests(self) -> PreparationResult:
        """Запуск тестов безопасности"""
        return self.run_tool(
            "Safety Tests",
            ["python", "test/test_refactoring_safety.py"],
            timeout=300  # 5 минут
        )
    
    def run_architecture_validation(self) -> PreparationResult:
        """Запуск валидации архитектуры"""
        return self.run_tool(
            "Architecture Validation",
            ["python", "scripts/architecture_validator.py"],
            timeout=180  # 3 минуты
        )
    
    def run_standard_tests(self) -> PreparationResult:
        """Запуск стандартных тестов"""
        return self.run_tool(
            "Standard Tests",
            ["python", "scripts/run_simple_tests.py"],
            timeout=300  # 5 минут
        )
    
    def run_clang_tidy(self) -> PreparationResult:
        """Запуск статического анализа"""
        return self.run_tool(
            "Clang-tidy Analysis",
            ["python", "scripts/run_clang_tidy_analysis.py"],
            timeout=180  # 3 минуты
        )
    
    def run_build_check(self) -> PreparationResult:
        """Проверка сборки"""
        return self.run_tool(
            "Build Check",
            ["pio", "run", "-e", "esp32dev"],
            timeout=600  # 10 минут
        )
    
    def generate_recommendations(self) -> List[str]:
        """Генерация рекомендаций на основе результатов"""
        recommendations = []
        
        failed_tools = [r for r in self.results if r.status == "FAIL"]
        warning_tools = [r for r in self.results if r.status == "WARN"]
        
        if failed_tools:
            recommendations.append("🚨 КРИТИЧЕСКИЕ ПРОБЛЕМЫ:")
            for tool in failed_tools:
                recommendations.append(f"   - Исправьте {tool.tool_name}: {tool.details.get('message', 'Ошибка')}")
            recommendations.append("   - НЕ НАЧИНАЙТЕ рефакторинг до исправления")
        
        if warning_tools:
            recommendations.append("⚠️ ПРЕДУПРЕЖДЕНИЯ:")
            for tool in warning_tools:
                recommendations.append(f"   - Рассмотрите {tool.tool_name}: {tool.details.get('message', 'Предупреждение')}")
            recommendations.append("   - Рекомендуется исправить перед рефакторингом")
        
        if not failed_tools and not warning_tools:
            recommendations.append("✅ ВСЕ ПРОВЕРКИ ПРОЙДЕНЫ:")
            recommendations.append("   - Рефакторинг безопасен для выполнения")
            recommendations.append("   - Можете начинать архитектурные изменения")
        
        # Общие рекомендации
        recommendations.append("")
        recommendations.append("📋 ОБЩИЕ РЕКОМЕНДАЦИИ:")
        recommendations.append("   1. Работайте только в ветке refactoring")
        recommendations.append("   2. Тестируйте после каждого значимого изменения")
        recommendations.append("   3. Документируйте все решения в REFACTORING_LOG.md")
        recommendations.append("   4. Соблюдайте принципы SOLID")
        recommendations.append("   5. Поддерживайте обратную совместимость API")
        
        return recommendations
    
    def generate_summary(self) -> Dict[str, Any]:
        """Генерация сводки результатов"""
        total_tools = len(self.results)
        passed_tools = sum(1 for r in self.results if r.status == "PASS")
        failed_tools = sum(1 for r in self.results if r.status == "FAIL")
        warning_tools = sum(1 for r in self.results if r.status == "WARN")
        
        total_time = (datetime.now() - self.start_time).total_seconds()
        
        return {
            "total_tools": total_tools,
            "passed_tools": passed_tools,
            "failed_tools": failed_tools,
            "warning_tools": warning_tools,
            "success_rate": (passed_tools / total_tools * 100) if total_tools > 0 else 0,
            "total_execution_time_seconds": total_time,
            "timestamp": datetime.now().isoformat()
        }
    
    def save_report(self, report: PreparationReport):
        """Сохранение отчёта в JSON"""
        report_path = self.project_root / "test_reports" / "refactoring_preparation.json"
        report_path.parent.mkdir(exist_ok=True)
        
        with open(report_path, 'w', encoding='utf-8') as f:
            json.dump(asdict(report), f, indent=2, ensure_ascii=False)
        
        self.log(f"📊 Отчёт подготовки сохранён: {report_path}")
    
    def print_report(self, report: PreparationReport):
        """Вывод отчёта в консоль"""
        print("\n" + "="*70)
        print("🚀 ОТЧЁТ ПОДГОТОВКИ К РЕФАКТОРИНГУ JXCT")
        print("="*70)
        print(f"📅 Дата: {report.timestamp}")
        print(f"🔢 Версия: {report.version}")
        
        print("\n📊 РЕЗУЛЬТАТЫ ИНСТРУМЕНТОВ:")
        print("-" * 70)
        
        for result in report.results:
            status_icon = {
                "PASS": "✅",
                "WARN": "⚠️",
                "FAIL": "❌"
            }.get(result.status, "❓")
            
            print(f"{status_icon} {result.tool_name}: {result.status}")
            print(f"   ⏱️ Время: {result.execution_time:.2f} сек")
            print(f"   💬 {result.details.get('message', 'Нет сообщения')}")
        
        print("\n📈 СВОДКА:")
        print("-" * 70)
        summary = report.summary
        print(f"🔧 Всего инструментов: {summary['total_tools']}")
        print(f"✅ Пройдено: {summary['passed_tools']}")
        print(f"⚠️ Предупреждения: {summary['warning_tools']}")
        print(f"❌ Провалено: {summary['failed_tools']}")
        print(f"📈 Успешность: {summary['success_rate']:.1f}%")
        print(f"⏱️ Общее время: {summary['total_execution_time_seconds']:.2f} сек")
        
        print("\n💡 РЕКОМЕНДАЦИИ:")
        print("-" * 70)
        for recommendation in report.recommendations:
            print(recommendation)
        
        print("\n" + "="*70)
    
    def run_all_preparations(self) -> PreparationReport:
        """Запуск всех инструментов подготовки"""
        self.log("🚀 Запуск подготовки к рефакторингу JXCT...")
        
        # Список инструментов подготовки
        preparation_tools = [
            self.run_performance_benchmarks,
            self.run_safety_tests,
            self.run_architecture_validation,
            self.run_standard_tests,
            self.run_clang_tidy,
            self.run_build_check
        ]
        
        # Выполнение инструментов
        for tool_func in preparation_tools:
            try:
                result = tool_func()
                self.results.append(result)
                self.log(f"{result.status} {result.tool_name}: {result.execution_time:.2f} сек")
            except Exception as e:
                self.log(f"❌ Ошибка в {tool_func.__name__}: {e}", "ERROR")
                error_result = PreparationResult(
                    tool_name=tool_func.__name__,
                    status="FAIL",
                    execution_time=0.0,
                    details={"error": str(e), "message": f"Ошибка выполнения {tool_func.__name__}"}
                )
                self.results.append(error_result)
        
        # Генерация отчёта
        summary = self.generate_summary()
        recommendations = self.generate_recommendations()
        
        report = PreparationReport(
            version="3.11.0",
            timestamp=datetime.now().isoformat(),
            total_tools=summary["total_tools"],
            passed_tools=summary["passed_tools"],
            failed_tools=summary["failed_tools"],
            warning_tools=summary["warning_tools"],
            results=self.results,
            summary=summary,
            recommendations=recommendations
        )
        
        # Сохранение и вывод отчёта
        self.save_report(report)
        self.print_report(report)
        
        return report

def main():
    """Главная функция"""
    preparation = JXCTRefactoringPreparation()
    
    try:
        report = preparation.run_all_preparations()
        
        # Возвращаем код выхода на основе результатов
        if report.failed_tools > 0:
            print("\n🚨 КРИТИЧЕСКИЕ ПРОБЛЕМЫ ОБНАРУЖЕНЫ!")
            print("   Рефакторинг НЕ РЕКОМЕНДУЕТСЯ до исправления проблем.")
            sys.exit(1)
        elif report.warning_tools > 0:
            print("\n⚠️ ПРЕДУПРЕЖДЕНИЯ ОБНАРУЖЕНЫ!")
            print("   Рекомендуется исправить предупреждения перед рефакторингом.")
            sys.exit(2)
        else:
            print("\n✅ ВСЕ ПРОВЕРКИ ПРОЙДЕНЫ!")
            print("   Рефакторинг безопасен для выполнения.")
            sys.exit(0)
            
    except KeyboardInterrupt:
        print("\n⚠️ Подготовка к рефакторингу прервана пользователем")
        sys.exit(1)
    except Exception as e:
        print(f"\n❌ Критическая ошибка: {e}")
        sys.exit(1)

if __name__ == "__main__":
    main() 