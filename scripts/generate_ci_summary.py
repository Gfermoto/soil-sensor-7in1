#!/usr/bin/env python3
"""
Скрипт для генерации сводного отчёта CI/CD
"""

import os
import sys
import json
import time
import argparse
from pathlib import Path
from typing import Dict, List, Any

class CISummaryGenerator:
    def __init__(self, artifacts_path: str, output_path: str):
        self.artifacts_path = Path(artifacts_path)
        self.output_path = Path(output_path)
        self.summary_data = {
            "generated_at": time.strftime("%Y-%m-%d %H:%M:%S"),
            "ci_run_id": os.environ.get("GITHUB_RUN_ID", "local"),
            "branch": os.environ.get("GITHUB_REF_NAME", "unknown"),
            "commit": os.environ.get("GITHUB_SHA", "unknown"),
            "jobs": {},
            "overall_status": "unknown",
            "performance_metrics": {},
            "recommendations": []
        }

    def load_artifact_data(self, artifact_name: str) -> Dict[str, Any]:
        """Загрузка данных из артефакта"""
        artifact_path = self.artifacts_path / artifact_name

        if not artifact_path.exists():
            return {}

        data = {}

        # Ищем JSON файлы
        for json_file in artifact_path.rglob("*.json"):
            try:
                with open(json_file, 'r', encoding='utf-8') as f:
                    file_data = json.load(f)
                    data[json_file.name] = file_data
            except Exception as e:
                print(f"Ошибка чтения {json_file}: {e}")

        # Ищем HTML файлы
        for html_file in artifact_path.rglob("*.html"):
            try:
                with open(html_file, 'r', encoding='utf-8') as f:
                    data[html_file.name] = f.read()
            except Exception as e:
                print(f"Ошибка чтения {html_file}: {e}")

        return data

    def analyze_unit_tests(self) -> Dict[str, Any]:
        """Анализ результатов unit тестов"""
        print("Анализ unit тестов...")

        unit_data = self.load_artifact_data("unit-test-results")

        analysis = {
            "status": "unknown",
            "total_tests": 0,
            "passed_tests": 0,
            "failed_tests": 0,
            "coverage": 0.0,
            "execution_time": 0.0,
            "details": {}
        }

        # Анализируем JSON отчёты
        for filename, data in unit_data.items():
            if filename.endswith('.json'):
                if isinstance(data, dict):
                    # Анализируем структуру отчёта
                    if 'summary' in data:
                        summary = data['summary']
                        analysis['total_tests'] += summary.get('total_tests', 0)
                        analysis['passed_tests'] += summary.get('passed_tests', 0)
                        analysis['failed_tests'] += summary.get('failed_tests', 0)

                    if 'coverage' in data:
                        analysis['coverage'] = max(analysis['coverage'], data['coverage'])

                    if 'execution_time' in data:
                        analysis['execution_time'] += data['execution_time']

                    analysis['details'][filename] = data

        # Определяем статус
        if analysis['total_tests'] > 0:
            if analysis['failed_tests'] == 0:
                analysis['status'] = "passed"
            elif analysis['failed_tests'] < analysis['total_tests']:
                analysis['status'] = "partial"
            else:
                analysis['status'] = "failed"
        else:
            analysis['status'] = "no_tests"

        return analysis

    def analyze_e2e_tests(self) -> Dict[str, Any]:
        """Анализ результатов E2E тестов"""
        print("Анализ E2E тестов...")

        e2e_data = self.load_artifact_data("e2e-test-results")

        analysis = {
            "status": "unknown",
            "total_tests": 0,
            "passed_tests": 0,
            "failed_tests": 0,
            "execution_time": 0.0,
            "browser_tests": 0,
            "api_tests": 0,
            "details": {}
        }

        # Анализируем JSON отчёты
        for filename, data in e2e_data.items():
            if filename.endswith('.json'):
                if isinstance(data, dict):
                    if 'summary' in data:
                        summary = data['summary']
                        analysis['total_tests'] += summary.get('total_tests', 0)
                        analysis['passed_tests'] += summary.get('passed_tests', 0)
                        analysis['failed_tests'] += summary.get('failed_tests', 0)

                    if 'test_types' in data:
                        test_types = data['test_types']
                        analysis['browser_tests'] += test_types.get('browser', 0)
                        analysis['api_tests'] += test_types.get('api', 0)

                    if 'execution_time' in data:
                        analysis['execution_time'] += data['execution_time']

                    analysis['details'][filename] = data

        # Определяем статус
        if analysis['total_tests'] > 0:
            if analysis['failed_tests'] == 0:
                analysis['status'] = "passed"
            elif analysis['failed_tests'] < analysis['total_tests']:
                analysis['status'] = "partial"
            else:
                analysis['status'] = "failed"
        else:
            analysis['status'] = "no_tests"

        return analysis

    def analyze_static_analysis(self) -> Dict[str, Any]:
        """Анализ результатов статического анализа"""
        print("Анализ статического анализа...")

        static_data = self.load_artifact_data("static-analysis-results")

        analysis = {
            "status": "unknown",
            "code_smells": 0,
            "duplicated_lines": 0,
            "complex_functions": 0,
            "security_vulnerabilities": 0,
            "maintainability_rating": "unknown",
            "technical_debt_ratio": 0.0,
            "details": {}
        }

        # Анализируем JSON отчёты
        for filename, data in static_data.items():
            if filename.endswith('.json'):
                if isinstance(data, dict):
                    if 'metrics' in data:
                        metrics = data['metrics']
                        analysis['code_smells'] += metrics.get('code_smells', 0)
                        analysis['duplicated_lines'] += metrics.get('duplicated_lines', 0)
                        analysis['complex_functions'] += metrics.get('complex_functions', 0)
                        analysis['security_vulnerabilities'] += metrics.get('security_vulnerabilities', 0)

                    if 'maintainability_rating' in data:
                        analysis['maintainability_rating'] = data['maintainability_rating']

                    if 'technical_debt_ratio' in data:
                        analysis['technical_debt_ratio'] = max(
                            analysis['technical_debt_ratio'],
                            data['technical_debt_ratio']
                        )

                    analysis['details'][filename] = data

        # Определяем статус
        if analysis['security_vulnerabilities'] > 0:
            analysis['status'] = "critical"
        elif analysis['code_smells'] > 50:
            analysis['status'] = "warning"
        elif analysis['code_smells'] > 0:
            analysis['status'] = "info"
        else:
            analysis['status'] = "clean"

        return analysis

    def analyze_performance_tests(self) -> Dict[str, Any]:
        """Анализ результатов тестов производительности"""
        print("Анализ тестов производительности...")

        perf_data = self.load_artifact_data("performance-test-results")

        analysis = {
            "status": "unknown",
            "average_response_time": 0.0,
            "max_response_time": 0.0,
            "operations_per_second": 0.0,
            "memory_usage": 0.0,
            "cpu_usage": 0.0,
            "details": {}
        }

        # Анализируем JSON отчёты
        for filename, data in perf_data.items():
            if filename.endswith('.json'):
                if isinstance(data, dict):
                    if 'summary' in data and 'performance_metrics' in data['summary']:
                        metrics = data['summary']['performance_metrics']
                        analysis['average_response_time'] = max(
                            analysis['average_response_time'],
                            metrics.get('average_response_time', 0.0)
                        )
                        analysis['max_response_time'] = max(
                            analysis['max_response_time'],
                            metrics.get('max_response_time', 0.0)
                        )

                    if 'benchmark_results' in data:
                        benchmarks = data['benchmark_results']
                        if 'benchmarks' in benchmarks:
                            bench_data = benchmarks['benchmarks']
                            if 'cpu' in bench_data:
                                analysis['cpu_usage'] = bench_data['cpu'].get('computation_time', 0.0)

                    analysis['details'][filename] = data

        # Определяем статус
        if analysis['average_response_time'] > 1000:  # > 1 секунды
            analysis['status'] = "slow"
        elif analysis['average_response_time'] > 100:  # > 100ms
            analysis['status'] = "warning"
        else:
            analysis['status'] = "good"

        return analysis

    def generate_recommendations(self) -> List[str]:
        """Генерация рекомендаций на основе результатов"""
        recommendations = []

        # Анализируем unit тесты
        unit_analysis = self.summary_data['jobs'].get('unit_tests', {})
        if unit_analysis.get('status') == 'failed':
            recommendations.append("🔴 Исправьте провалившиеся unit тесты")
        elif unit_analysis.get('coverage', 0) < 80:
            recommendations.append("🟡 Увеличьте покрытие unit тестов до 80%+")

        # Анализируем E2E тесты
        e2e_analysis = self.summary_data['jobs'].get('e2e_tests', {})
        if e2e_analysis.get('status') == 'failed':
            recommendations.append("🔴 Исправьте провалившиеся E2E тесты")

        # Анализируем статический анализ
        static_analysis = self.summary_data['jobs'].get('static_analysis', {})
        if static_analysis.get('security_vulnerabilities', 0) > 0:
            recommendations.append("🔴 Устраните уязвимости безопасности")
        if static_analysis.get('code_smells', 0) > 50:
            recommendations.append("🟡 Уменьшите количество code smells")
        if static_analysis.get('duplicated_lines', 0) > 1000:
            recommendations.append("🟡 Уменьшите дублирование кода")

        # Анализируем производительность
        perf_analysis = self.summary_data['jobs'].get('performance_tests', {})
        if perf_analysis.get('status') == 'slow':
            recommendations.append("🔴 Оптимизируйте производительность")
        elif perf_analysis.get('status') == 'warning':
            recommendations.append("🟡 Рассмотрите оптимизацию производительности")

        # Общие рекомендации
        if not recommendations:
            recommendations.append("✅ Все проверки пройдены успешно!")

        return recommendations

    def determine_overall_status(self) -> str:
        """Определение общего статуса CI/CD"""
        statuses = []

        for job_name, job_data in self.summary_data['jobs'].items():
            status = job_data.get('status', 'unknown')
            if status in ['failed', 'critical']:
                statuses.append('failed')
            elif status in ['partial', 'warning']:
                statuses.append('warning')
            elif status in ['passed', 'clean', 'good']:
                statuses.append('passed')
            else:
                statuses.append('unknown')

        if 'failed' in statuses:
            return 'failed'
        elif 'warning' in statuses:
            return 'warning'
        elif all(s == 'passed' for s in statuses):
            return 'passed'
        else:
            return 'unknown'

    def generate_html_report(self) -> str:
        """Генерация HTML отчёта"""
        html_content = f"""
<!DOCTYPE html>
<html lang="ru">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>CI/CD Summary - JXCT</title>
    <style>
        body {{ font-family: Arial, sans-serif; margin: 20px; background: #f5f5f5; }}
        .container {{ max-width: 1200px; margin: 0 auto; background: white; padding: 20px; border-radius: 10px; box-shadow: 0 2px 10px rgba(0,0,0,0.1); }}
        .header {{ text-align: center; margin-bottom: 30px; }}
        .status-badge {{ display: inline-block; padding: 5px 15px; border-radius: 20px; color: white; font-weight: bold; }}
        .status-passed {{ background: #28a745; }}
        .status-warning {{ background: #ffc107; color: #333; }}
        .status-failed {{ background: #dc3545; }}
        .status-unknown {{ background: #6c757d; }}
        .job-grid {{ display: grid; grid-template-columns: repeat(auto-fit, minmax(300px, 1fr)); gap: 20px; margin: 20px 0; }}
        .job-card {{ border: 1px solid #ddd; border-radius: 8px; padding: 15px; }}
        .job-header {{ display: flex; justify-content: space-between; align-items: center; margin-bottom: 10px; }}
        .job-title {{ font-weight: bold; font-size: 1.1em; }}
        .metrics {{ display: grid; grid-template-columns: 1fr 1fr; gap: 10px; margin: 10px 0; }}
        .metric {{ background: #f8f9fa; padding: 8px; border-radius: 5px; }}
        .metric-label {{ font-size: 0.9em; color: #666; }}
        .metric-value {{ font-weight: bold; }}
        .recommendations {{ background: #e8f4f8; padding: 15px; border-radius: 8px; margin: 20px 0; }}
        .recommendation {{ margin: 5px 0; }}
        .info-section {{ background: #f8f9fa; padding: 15px; border-radius: 8px; margin: 20px 0; }}
        .info-grid {{ display: grid; grid-template-columns: repeat(auto-fit, minmax(200px, 1fr)); gap: 10px; }}
    </style>
</head>
<body>
    <div class="container">
        <div class="header">
            <h1>🚀 CI/CD Summary Report</h1>
            <p><strong>JXCT Project</strong></p>
            <div class="status-badge status-{self.summary_data['overall_status']}">
                {self.summary_data['overall_status'].upper()}
            </div>
        </div>

        <div class="info-section">
            <h3>📋 Информация о запуске</h3>
            <div class="info-grid">
                <div class="metric">
                    <div class="metric-label">Run ID</div>
                    <div class="metric-value">{self.summary_data['ci_run_id']}</div>
                </div>
                <div class="metric">
                    <div class="metric-label">Branch</div>
                    <div class="metric-value">{self.summary_data['branch']}</div>
                </div>
                <div class="metric">
                    <div class="metric-label">Commit</div>
                    <div class="metric-value">{self.summary_data['commit'][:8]}</div>
                </div>
                <div class="metric">
                    <div class="metric-label">Generated</div>
                    <div class="metric-value">{self.summary_data['generated_at']}</div>
                </div>
            </div>
        </div>

        <div class="job-grid">
"""

        # Генерируем карточки для каждого job
        for job_name, job_data in self.summary_data['jobs'].items():
            status_class = f"status-{job_data.get('status', 'unknown')}"
            status_text = job_data.get('status', 'unknown').upper()

            html_content += f"""
            <div class="job-card">
                <div class="job-header">
                    <div class="job-title">{job_name.replace('_', ' ').title()}</div>
                    <div class="status-badge {status_class}">{status_text}</div>
                </div>
"""

            # Добавляем метрики
            if 'total_tests' in job_data:
                html_content += f"""
                <div class="metrics">
                    <div class="metric">
                        <div class="metric-label">Tests</div>
                        <div class="metric-value">{job_data.get('passed_tests', 0)}/{job_data.get('total_tests', 0)}</div>
                    </div>
                    <div class="metric">
                        <div class="metric-label">Coverage</div>
                        <div class="metric-value">{job_data.get('coverage', 0):.1f}%</div>
                    </div>
                </div>
"""

            if 'code_smells' in job_data:
                html_content += f"""
                <div class="metrics">
                    <div class="metric">
                        <div class="metric-label">Code Smells</div>
                        <div class="metric-value">{job_data.get('code_smells', 0)}</div>
                    </div>
                    <div class="metric">
                        <div class="metric-label">Security Issues</div>
                        <div class="metric-value">{job_data.get('security_vulnerabilities', 0)}</div>
                    </div>
                </div>
"""

            if 'average_response_time' in job_data:
                html_content += f"""
                <div class="metrics">
                    <div class="metric">
                        <div class="metric-label">Avg Response</div>
                        <div class="metric-value">{job_data.get('average_response_time', 0):.1f}ms</div>
                    </div>
                    <div class="metric">
                        <div class="metric-label">Max Response</div>
                        <div class="metric-value">{job_data.get('max_response_time', 0):.1f}ms</div>
                    </div>
                </div>
"""

            html_content += """
            </div>
"""

        # Добавляем рекомендации
        html_content += """
        </div>

        <div class="recommendations">
            <h3>💡 Рекомендации</h3>
"""

        for recommendation in self.summary_data['recommendations']:
            html_content += f"""
            <div class="recommendation">{recommendation}</div>
"""

        html_content += """
        </div>
    </div>
</body>
</html>
"""

        with open(self.output_path, 'w', encoding='utf-8') as f:
            f.write(html_content)

        return str(self.output_path)

    def generate_summary(self) -> bool:
        """Генерация сводного отчёта"""
        print("🔍 Анализ артефактов CI/CD...")

        # Проверяем существование директории артефактов
        if not self.artifacts_path.exists():
            print(f"⚠️ Директория артефактов не найдена: {self.artifacts_path}")
            print("Создаём минимальный отчёт...")

            # Создаём минимальные данные
            self.summary_data['jobs']['unit_tests'] = {"status": "no_artifacts", "total_tests": 0, "passed_tests": 0}
            self.summary_data['jobs']['e2e_tests'] = {"status": "no_artifacts", "total_tests": 0, "passed_tests": 0}
            self.summary_data['jobs']['static_analysis'] = {"status": "no_artifacts", "code_smells": 0}
            self.summary_data['jobs']['performance_tests'] = {"status": "no_artifacts", "average_response_time": 0}

            self.summary_data['overall_status'] = "warning"
            self.summary_data['recommendations'] = [
                "Артефакты CI/CD не найдены",
                "Проверьте настройки workflow",
                "Убедитесь, что все jobs завершились"
            ]
        else:
            # Анализируем результаты всех jobs
            self.summary_data['jobs']['unit_tests'] = self.analyze_unit_tests()
            self.summary_data['jobs']['e2e_tests'] = self.analyze_e2e_tests()
            self.summary_data['jobs']['static_analysis'] = self.analyze_static_analysis()
            self.summary_data['jobs']['performance_tests'] = self.analyze_performance_tests()

            # Определяем общий статус
            self.summary_data['overall_status'] = self.determine_overall_status()

            # Генерируем рекомендации
            self.summary_data['recommendations'] = self.generate_recommendations()

        # Генерируем HTML отчёт
        report_path = self.generate_html_report()

        print(f"📊 Сводный отчёт сохранён: {report_path}")
        print(f"📈 Общий статус: {self.summary_data['overall_status']}")
        print(f"💡 Рекомендаций: {len(self.summary_data['recommendations'])}")

        return True

def main():
    """Основная функция"""
    parser = argparse.ArgumentParser(description="Генерация сводного отчёта CI/CD")
    parser.add_argument("--artifacts", "-a", required=True, help="Путь к артефактам CI/CD")
    parser.add_argument("--output", "-o", required=True, help="Путь для сохранения отчёта")

    args = parser.parse_args()

    generator = CISummaryGenerator(args.artifacts, args.output)

    try:
        success = generator.generate_summary()

        if success:
            print("✅ Сводный отчёт сгенерирован успешно!")
            sys.exit(0)
        else:
            print("❌ Ошибка генерации отчёта")
            sys.exit(1)

    except Exception as e:
        print(f"❌ Неожиданная ошибка: {e}")
        sys.exit(1)

if __name__ == "__main__":
    main()
