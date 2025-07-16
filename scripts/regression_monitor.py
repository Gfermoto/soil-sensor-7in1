#!/usr/bin/env python3
"""
📈 JXCT Regression Monitor v1.0
Мониторинг регрессий и изменений в коде
"""

import os
import json
import subprocess
import hashlib
from pathlib import Path
from typing import Dict, List, Set, Optional
from dataclasses import dataclass
from datetime import datetime
import difflib

@dataclass
class CodeMetrics:
    timestamp: str
    total_lines: int
    code_lines: int
    comment_lines: int
    blank_lines: int
    functions_count: int
    classes_count: int
    complexity_score: float
    file_hashes: Dict[str, str]

@dataclass
class RegressionAlert:
    severity: str  # "low", "medium", "high", "critical"
    category: str  # "size", "complexity", "performance", "api"
    message: str
    details: Dict[str, any]

class RegressionMonitor:
    def __init__(self, project_root: str = "."):
        self.project_root = Path(project_root)
        self.history_file = "test_reports/regression_history.json"
        self.baseline_file = "test_reports/baseline_metrics.json"
        self.thresholds = {
            "size_increase_percent": 10,
            "complexity_increase": 0.5,
            "function_count_change": 5,
            "api_changes_count": 3
        }
    
    def collect_code_metrics(self) -> CodeMetrics:
        """Собирает метрики кода"""
        print("📊 Сбор метрик кода...")
        
        total_lines = 0
        code_lines = 0
        comment_lines = 0
        blank_lines = 0
        functions_count = 0
        classes_count = 0
        complexity_score = 0.0
        file_hashes = {}
        
        # Сканируем все C++ файлы
        cpp_files = list(self.project_root.rglob("*.cpp"))
        h_files = list(self.project_root.rglob("*.h"))
        
        for file_path in cpp_files + h_files:
            if "test/" in str(file_path) or "docs/" in str(file_path):
                continue
            
            try:
                content = file_path.read_text(encoding='utf-8', errors='ignore')
                lines = content.split('\n')
                
                # Подсчитываем строки
                file_total = len(lines)
                file_code = 0
                file_comments = 0
                file_blank = 0
                
                for line in lines:
                    stripped = line.strip()
                    if not stripped:
                        file_blank += 1
                    elif stripped.startswith('//') or stripped.startswith('/*'):
                        file_comments += 1
                    else:
                        file_code += 1
                
                total_lines += file_total
                code_lines += file_code
                comment_lines += file_comments
                blank_lines += file_blank
                
                # Подсчитываем функции и классы
                functions_count += content.count('void ') + content.count('int ') + content.count('float ')
                classes_count += content.count('class ') + content.count('struct ')
                
                # Простая оценка сложности
                complexity_score += content.count('if ') + content.count('for ') + content.count('while ')
                
                # Хеш файла для отслеживания изменений
                file_hash = hashlib.md5(content.encode()).hexdigest()
                file_hashes[str(file_path)] = file_hash
                
            except Exception as e:
                print(f"⚠️ Ошибка анализа {file_path}: {e}")
        
        return CodeMetrics(
            timestamp=datetime.now().isoformat(),
            total_lines=total_lines,
            code_lines=code_lines,
            comment_lines=comment_lines,
            blank_lines=blank_lines,
            functions_count=functions_count,
            classes_count=classes_count,
            complexity_score=complexity_score,
            file_hashes=file_hashes
        )
    
    def detect_api_changes(self) -> List[Dict[str, any]]:
        """Обнаруживает изменения в API"""
        changes = []
        
        # Анализируем заголовочные файлы
        h_files = list(self.project_root.rglob("*.h"))
        
        for h_file in h_files:
            if "test/" in str(h_file) or "docs/" in str(h_file):
                continue
            
            try:
                content = h_file.read_text(encoding='utf-8', errors='ignore')
                
                # Ищем объявления функций
                import re
                function_pattern = r'(\w+)\s+(\w+)\s*\([^)]*\)\s*;'
                functions = re.findall(function_pattern, content)
                
                for return_type, func_name in functions:
                    changes.append({
                        "file": str(h_file),
                        "type": "function_declaration",
                        "name": func_name,
                        "return_type": return_type
                    })
                
                # Ищем объявления классов
                class_pattern = r'class\s+(\w+)'
                classes = re.findall(class_pattern, content)
                
                for class_name in classes:
                    changes.append({
                        "file": str(h_file),
                        "type": "class_declaration",
                        "name": class_name
                    })
                    
            except Exception as e:
                print(f"⚠️ Ошибка анализа API {h_file}: {e}")
        
        return changes
    
    def compare_with_baseline(self, current_metrics: CodeMetrics) -> List[RegressionAlert]:
        """Сравнивает текущие метрики с базовой линией"""
        alerts = []
        
        if not os.path.exists(self.baseline_file):
            print("⚠️ Базовая линия не найдена. Создаем новую...")
            self._create_baseline(current_metrics)
            return alerts
        
        with open(self.baseline_file, 'r') as f:
            baseline = json.load(f)
        
        # Сравниваем размер кода
        size_increase = ((current_metrics.code_lines - baseline["code_lines"]) / baseline["code_lines"]) * 100
        if size_increase > self.thresholds["size_increase_percent"]:
            alerts.append(RegressionAlert(
                severity="medium" if size_increase < 20 else "high",
                category="size",
                message=f"Увеличение размера кода на {size_increase:.1f}%",
                details={"increase_percent": size_increase, "threshold": self.thresholds["size_increase_percent"]}
            ))
        
        # Сравниваем сложность
        complexity_change = current_metrics.complexity_score - baseline["complexity_score"]
        if complexity_change > self.thresholds["complexity_increase"]:
            alerts.append(RegressionAlert(
                severity="medium",
                category="complexity",
                message=f"Увеличение сложности на {complexity_change:.1f}",
                details={"complexity_change": complexity_change, "threshold": self.thresholds["complexity_increase"]}
            ))
        
        # Сравниваем количество функций
        function_change = abs(current_metrics.functions_count - baseline["functions_count"])
        if function_change > self.thresholds["function_count_change"]:
            alerts.append(RegressionAlert(
                severity="low",
                category="structure",
                message=f"Изменение количества функций на {function_change}",
                details={"function_change": function_change, "threshold": self.thresholds["function_count_change"]}
            ))
        
        # Проверяем изменения в файлах
        changed_files = self._detect_file_changes(baseline["file_hashes"], current_metrics.file_hashes)
        if changed_files:
            alerts.append(RegressionAlert(
                severity="low",
                category="files",
                message=f"Изменено файлов: {len(changed_files)}",
                details={"changed_files": changed_files}
            ))
        
        return alerts
    
    def _detect_file_changes(self, baseline_hashes: Dict[str, str], current_hashes: Dict[str, str]) -> List[str]:
        """Обнаруживает изменения в файлах"""
        changed_files = []
        
        for file_path, current_hash in current_hashes.items():
            baseline_hash = baseline_hashes.get(file_path)
            if baseline_hash and baseline_hash != current_hash:
                changed_files.append(file_path)
        
        return changed_files
    
    def _create_baseline(self, metrics: CodeMetrics):
        """Создает базовую линию"""
        baseline = {
            "timestamp": metrics.timestamp,
            "total_lines": metrics.total_lines,
            "code_lines": metrics.code_lines,
            "comment_lines": metrics.comment_lines,
            "blank_lines": metrics.blank_lines,
            "functions_count": metrics.functions_count,
            "classes_count": metrics.classes_count,
            "complexity_score": metrics.complexity_score,
            "file_hashes": metrics.file_hashes
        }
        
        os.makedirs(os.path.dirname(self.baseline_file), exist_ok=True)
        with open(self.baseline_file, 'w') as f:
            json.dump(baseline, f, indent=2)
        
        print(f"✅ Базовая линия создана: {self.baseline_file}")
    
    def save_metrics(self, metrics: CodeMetrics):
        """Сохраняет метрики в историю"""
        os.makedirs(os.path.dirname(self.history_file), exist_ok=True)
        
        if os.path.exists(self.history_file):
            with open(self.history_file, 'r') as f:
                history = json.load(f)
        else:
            history = {"metrics": []}
        
        # Добавляем новые метрики
        history["metrics"].append({
            "timestamp": metrics.timestamp,
            "total_lines": metrics.total_lines,
            "code_lines": metrics.code_lines,
            "comment_lines": metrics.comment_lines,
            "blank_lines": metrics.blank_lines,
            "functions_count": metrics.functions_count,
            "classes_count": metrics.classes_count,
            "complexity_score": metrics.complexity_score
        })
        
        # Ограничиваем историю последними 30 записями
        if len(history["metrics"]) > 30:
            history["metrics"] = history["metrics"][-30:]
        
        with open(self.history_file, 'w') as f:
            json.dump(history, f, indent=2)
    
    def generate_report(self) -> str:
        """Генерирует отчет о регрессиях"""
        metrics = self.collect_code_metrics()
        alerts = self.compare_with_baseline(metrics)
        api_changes = self.detect_api_changes()
        
        self.save_metrics(metrics)
        
        report = f"""
📈 ОТЧЕТ О МОНИТОРИНГЕ РЕГРЕССИЙ JXCT
========================================
Время: {metrics.timestamp}

📊 МЕТРИКИ КОДА:
Всего строк: {metrics.total_lines:,}
Строк кода: {metrics.code_lines:,}
Комментарии: {metrics.comment_lines:,}
Пустые строки: {metrics.blank_lines:,}
Функций: {metrics.functions_count}
Классов: {metrics.classes_count}
Сложность: {metrics.complexity_score:.1f}

🔍 ИЗМЕНЕНИЯ API:
Обнаружено изменений: {len(api_changes)}

"""
        
        if alerts:
            report += "⚠️ ПРЕДУПРЕЖДЕНИЯ О РЕГРЕССИЯХ:\n"
            for alert in alerts:
                severity_icon = {"low": "🔵", "medium": "🟡", "high": "🟠", "critical": "🔴"}[alert.severity]
                report += f"{severity_icon} {alert.message}\n"
                if alert.details:
                    for key, value in alert.details.items():
                        report += f"    {key}: {value}\n"
        else:
            report += "✅ Регрессий не обнаружено\n"
        
        return report

def main():
    monitor = RegressionMonitor()
    report = monitor.generate_report()
    
    print(report)
    
    # Сохраняем отчет
    report_file = "test_reports/regression_report.txt"
    os.makedirs(os.path.dirname(report_file), exist_ok=True)
    
    with open(report_file, 'w', encoding='utf-8') as f:
        f.write(report)
    
    print(f"✅ Отчет сохранен: {report_file}")

if __name__ == "__main__":
    main() 