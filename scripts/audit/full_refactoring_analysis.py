#!/usr/bin/env python3
"""
🔍 JXCT Full Refactoring Analysis v1.0
Полный анализ проекта перед рефакторингом
"""

import os
import sys
import subprocess
import time
from pathlib import Path
from typing import Dict, List, Any
from datetime import datetime

class FullRefactoringAnalyzer:
    def __init__(self, project_root: str = "."):
        self.project_root = Path(project_root)
        self.results = {}
        self.start_time = time.time()

    def run_all_analyses(self) -> Dict[str, Any]:
        """Запускает все анализы"""
        print("🔍 ПОЛНЫЙ АНАЛИЗ ПРОЕКТА JXCT ДЛЯ РЕФАКТОРИНГА")
        print("=" * 60)

        # 1. Анализ зависимостей
        print("\n1️⃣ АНАЛИЗ ЗАВИСИМОСТЕЙ...")
        self.results["dependencies"] = self._run_dependency_analysis()

        # 2. Статический анализ
        print("\n2️⃣ СТАТИЧЕСКИЙ АНАЛИЗ...")
        self.results["static_analysis"] = self._run_static_analysis()

        # 3. Тестирование
        print("\n3️⃣ ТЕСТИРОВАНИЕ...")
        self.results["testing"] = self._run_testing()

        # 4. Мониторинг производительности
        print("\n4️⃣ МОНИТОРИНГ ПРОИЗВОДИТЕЛЬНОСТИ...")
        self.results["performance"] = self._run_performance_monitoring()

        # 5. Мониторинг регрессий
        print("\n5️⃣ МОНИТОРИНГ РЕГРЕССИЙ...")
        self.results["regression"] = self._run_regression_monitoring()

        # 6. Сборка
        print("\n6️⃣ ПРОВЕРКА СБОРКИ...")
        self.results["build"] = self._run_build_check()

        return self.results

    def _run_dependency_analysis(self) -> Dict[str, Any]:
        """Запускает анализ зависимостей"""
        try:
            result = subprocess.run(
                [sys.executable, "scripts/dependency_analyzer.py"],
                capture_output=True,
                text=True,
                cwd=self.project_root
            )

            return {
                "success": result.returncode == 0,
                "output": result.stdout,
                "error": result.stderr if result.returncode != 0 else None
            }
        except Exception as e:
            return {"success": False, "error": str(e)}

    def _run_static_analysis(self) -> Dict[str, Any]:
        """Запускает статический анализ"""
        try:
            # Clang-tidy
            clang_result = subprocess.run(
                [sys.executable, "scripts/run_clang_tidy_analysis.py"],
                capture_output=True,
                text=True,
                cwd=self.project_root
            )

            # Cppcheck
            cppcheck_result = subprocess.run(
                ["pio", "check", "-e", "static-analysis"],
                capture_output=True,
                text=True,
                cwd=self.project_root
            )

            return {
                "clang_tidy": {
                    "success": clang_result.returncode == 0,
                    "output": clang_result.stdout,
                    "error": clang_result.stderr if clang_result.returncode != 0 else None
                },
                "cppcheck": {
                    "success": cppcheck_result.returncode == 0,
                    "output": cppcheck_result.stdout,
                    "error": cppcheck_result.stderr if cppcheck_result.returncode != 0 else None
                }
            }
        except Exception as e:
            return {"error": str(e)}

    def _run_testing(self) -> Dict[str, Any]:
        """Запускает тестирование"""
        try:
            # Python тесты
            pytest_result = subprocess.run(
                [sys.executable, "-m", "pytest", "test/test_validation.py", "test/test_compensation_formulas.py"],
                capture_output=True,
                text=True,
                cwd=self.project_root
            )

            # Интеграционные тесты
            integration_result = subprocess.run(
                [sys.executable, "test/test_business_logic_integration.py"],
                capture_output=True,
                text=True,
                cwd=self.project_root
            )

            # Native тесты
            native_result = subprocess.run(
                ["pio", "test", "-e", "native"],
                capture_output=True,
                text=True,
                cwd=self.project_root
            )

            return {
                "pytest": {
                    "success": pytest_result.returncode == 0,
                    "output": pytest_result.stdout,
                    "error": pytest_result.stderr if pytest_result.returncode != 0 else None
                },
                "integration": {
                    "success": integration_result.returncode == 0,
                    "output": integration_result.stdout,
                    "error": integration_result.stderr if integration_result.returncode != 0 else None
                },
                "native": {
                    "success": native_result.returncode == 0,
                    "output": native_result.stdout,
                    "error": native_result.stderr if native_result.returncode != 0 else None
                }
            }
        except Exception as e:
            return {"error": str(e)}

    def _run_performance_monitoring(self) -> Dict[str, Any]:
        """Запускает мониторинг производительности"""
        try:
            result = subprocess.run(
                [sys.executable, "scripts/performance_monitor.py"],
                capture_output=True,
                text=True,
                cwd=self.project_root
            )

            return {
                "success": result.returncode == 0,
                "output": result.stdout,
                "error": result.stderr if result.returncode != 0 else None
            }
        except Exception as e:
            return {"success": False, "error": str(e)}

    def _run_regression_monitoring(self) -> Dict[str, Any]:
        """Запускает мониторинг регрессий"""
        try:
            result = subprocess.run(
                [sys.executable, "scripts/regression_monitor.py"],
                capture_output=True,
                text=True,
                cwd=self.project_root
            )

            return {
                "success": result.returncode == 0,
                "output": result.stdout,
                "error": result.stderr if result.returncode != 0 else None
            }
        except Exception as e:
            return {"success": False, "error": str(e)}

    def _run_build_check(self) -> Dict[str, Any]:
        """Проверяет сборку"""
        try:
            result = subprocess.run(
                ["pio", "run", "-e", "esp32dev"],
                capture_output=True,
                text=True,
                cwd=self.project_root
            )

            return {
                "success": result.returncode == 0,
                "output": result.stdout,
                "error": result.stderr if result.returncode != 0 else None
            }
        except Exception as e:
            return {"success": False, "error": str(e)}

    def generate_summary(self) -> str:
        """Генерирует сводный отчет"""
        total_time = time.time() - self.start_time

        # Подсчитываем результаты
        total_checks = 0
        passed_checks = 0
        failed_checks = 0
        warnings = []

        for category, result in self.results.items():
            if isinstance(result, dict):
                if "success" in result:
                    total_checks += 1
                    if result["success"]:
                        passed_checks += 1
                    else:
                        failed_checks += 1
                        if "error" in result:
                            warnings.append(f"{category}: {result['error']}")
                elif "error" in result:
                    warnings.append(f"{category}: {result['error']}")

        success_rate = (passed_checks/total_checks*100) if total_checks > 0 else 0
        summary = f"""
📊 СВОДНЫЙ ОТЧЕТ АНАЛИЗА РЕФАКТОРИНГА
=====================================
Время выполнения: {total_time:.1f} сек
Всего проверок: {total_checks}
Пройдено: {passed_checks}
Провалено: {failed_checks}
Успешность: {success_rate:.1f}%

"""

        if warnings:
            summary += "⚠️ ПРЕДУПРЕЖДЕНИЯ:\n"
            for warning in warnings:
                summary += f"  {warning}\n"

        # Рекомендации
        summary += "\n💡 РЕКОМЕНДАЦИИ ДЛЯ РЕФАКТОРИНГА:\n"

        if failed_checks > 0:
            summary += "  ❌ НЕ РЕКОМЕНДУЕТСЯ рефакторинг - есть проблемы\n"
        elif passed_checks == total_checks:
            summary += "  ✅ МОЖНО выполнять рефакторинг - все проверки пройдены\n"
        else:
            summary += "  ⚠️ РЕФАКТОРИНГ С ОСТОРОЖНОСТЬЮ - есть предупреждения\n"

        summary += """
🔧 СЛЕДУЮЩИЕ ШАГИ:
1. Исправить все ошибки и предупреждения
2. Создать точку отката: git tag before-refactoring
3. Выполнять изменения небольшими порциями
4. После каждого изменения запускать тесты
5. Мониторить производительность и размер прошивки
"""

        return summary

    def save_results(self):
        """Сохраняет результаты анализа"""
        import json

        results_file = "test_reports/full_refactoring_analysis.json"
        os.makedirs(os.path.dirname(results_file), exist_ok=True)

        # Подготавливаем данные для сохранения
        save_data = {
            "timestamp": datetime.now().isoformat(),
            "execution_time": time.time() - self.start_time,
            "results": self.results
        }

        with open(results_file, 'w', encoding='utf-8') as f:
            json.dump(save_data, f, indent=2, ensure_ascii=False)

        print(f"✅ Результаты сохранены: {results_file}")

def main():
    analyzer = FullRefactoringAnalyzer()
    results = analyzer.run_all_analyses()

    summary = analyzer.generate_summary()
    print(summary)

    analyzer.save_results()

    # Сохраняем сводный отчет
    summary_file = "test_reports/refactoring_analysis_summary.txt"
    os.makedirs(os.path.dirname(summary_file), exist_ok=True)

    with open(summary_file, 'w', encoding='utf-8') as f:
        f.write(summary)

    print(f"✅ Сводный отчет сохранен: {summary_file}")

    # Возвращаем код выхода
    total_checks = sum(1 for r in results.values() if isinstance(r, dict) and "success" in r)
    failed_checks = sum(1 for r in results.values() if isinstance(r, dict) and "success" in r and not r["success"])

    return 1 if failed_checks > 0 else 0

if __name__ == "__main__":
    exit(main())
