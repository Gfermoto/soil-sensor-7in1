#!/usr/bin/env python3
"""
Анализатор технического долга JXCT
Версия: 1.0.0
Автор: EYERA Development Team
Дата: 2025-01-22

Этот скрипт анализирует технический долг проекта и генерирует метрики
для интеграции с системой тестирования.
"""

import os
import sys
import json
import subprocess
import argparse
from pathlib import Path
from typing import Dict, List, Tuple, Optional
from dataclasses import dataclass, asdict
import tempfile
import re


@dataclass
class TechnicalDebtMetrics:
    """Метрики технического долга"""
    code_smells: int = 0
    duplicated_lines: int = 0
    complexity_issues: int = 0
    security_hotspots: int = 0
    maintainability_rating: str = "A"
    reliability_rating: str = "A"
    security_rating: str = "A"
    coverage: float = 0.0
    debt_ratio: float = 0.0
    total_lines: int = 0
    
    # Детальная информация
    high_complexity_functions: List[str] = None
    duplicate_blocks: List[Dict] = None
    security_issues: List[Dict] = None
    
    def __post_init__(self):
        if self.high_complexity_functions is None:
            self.high_complexity_functions = []
        if self.duplicate_blocks is None:
            self.duplicate_blocks = []
        if self.security_issues is None:
            self.security_issues = []


class TechnicalDebtAnalyzer:
    """Анализатор технического долга"""
    
    def __init__(self, project_root: Path):
        self.project_root = project_root
        self.src_dirs = [
            project_root / "src",
            project_root / "include",
            project_root / "test"
        ]
        self.metrics = TechnicalDebtMetrics()
        
    def analyze(self) -> TechnicalDebtMetrics:
        """Выполнить полный анализ технического долга"""
        print("🔍 Анализ технического долга JXCT...")
        
        # Основные анализы
        self._analyze_code_complexity()
        self._analyze_duplicates()
        self._analyze_security()
        self._analyze_code_smells()
        self._calculate_ratings()
        
        print(f"✅ Анализ завершён. Найдено проблем: {self._total_issues()}")
        return self.metrics
    
    def _analyze_code_complexity(self):
        """Анализ сложности кода"""
        print("  📊 Анализ сложности кода...")
        
        complexity_issues = 0
        high_complexity_functions = []
        
        for src_dir in self.src_dirs:
            if not src_dir.exists():
                continue
                
            for file_path in src_dir.rglob("*.cpp"):
                issues, functions = self._analyze_file_complexity(file_path)
                complexity_issues += issues
                high_complexity_functions.extend(functions)
        
        self.metrics.complexity_issues = complexity_issues
        self.metrics.high_complexity_functions = high_complexity_functions
        
    def _analyze_file_complexity(self, file_path: Path) -> Tuple[int, List[str]]:
        """Анализ сложности конкретного файла"""
        try:
            with open(file_path, 'r', encoding='utf-8') as f:
                content = f.read()
        except:
            return 0, []
        
        issues = 0
        high_complexity_functions = []
        
        # Простой анализ цикломатической сложности
        # Ищем функции с высокой сложностью
        function_pattern = r'(\w+\s+\w+\s*\([^)]*\)\s*\{[^}]*\})'
        functions = re.findall(function_pattern, content, re.DOTALL)
        
        for func in functions:
            complexity = self._calculate_cyclomatic_complexity(func)
            if complexity > 10:  # Порог сложности
                issues += 1
                func_name = self._extract_function_name(func)
                high_complexity_functions.append(f"{file_path.name}::{func_name} (CC={complexity})")
        
        return issues, high_complexity_functions
    
    def _calculate_cyclomatic_complexity(self, function_code: str) -> int:
        """Вычисление цикломатической сложности"""
        # Упрощённый подсчёт: базовая сложность + количество решений
        complexity = 1
        
        # Ключевые слова, увеличивающие сложность
        decision_keywords = ['if', 'else', 'while', 'for', 'switch', 'case', 'catch', '&&', '||', '?']
        
        for keyword in decision_keywords:
            if keyword in ['&&', '||', '?']:
                complexity += function_code.count(keyword)
            else:
                complexity += len(re.findall(r'\b' + keyword + r'\b', function_code))
        
        return complexity
    
    def _extract_function_name(self, function_code: str) -> str:
        """Извлечение имени функции"""
        match = re.search(r'(\w+)\s*\(', function_code)
        return match.group(1) if match else "unknown"
    
    def _analyze_duplicates(self):
        """Анализ дублированного кода"""
        print("  📋 Анализ дублированного кода...")
        
        # Используем простой анализ дубликатов
        duplicate_lines = 0
        duplicate_blocks = []
        
        # Собираем все строки кода
        all_lines = {}
        
        for src_dir in self.src_dirs:
            if not src_dir.exists():
                continue
                
            for file_path in src_dir.rglob("*.cpp"):
                try:
                    with open(file_path, 'r', encoding='utf-8') as f:
                        lines = f.readlines()
                    
                    for i, line in enumerate(lines):
                        clean_line = line.strip()
                        if len(clean_line) > 10 and not clean_line.startswith('//'):
                            if clean_line in all_lines:
                                all_lines[clean_line].append((file_path, i + 1))
                            else:
                                all_lines[clean_line] = [(file_path, i + 1)]
                except:
                    continue
        
        # Находим дубликаты
        for line, occurrences in all_lines.items():
            if len(occurrences) > 1:
                duplicate_lines += len(occurrences) - 1
                duplicate_blocks.append({
                    "code": line[:100] + "..." if len(line) > 100 else line,
                    "occurrences": len(occurrences),
                    "files": [str(f[0].name) for f in occurrences]
                })
        
        self.metrics.duplicated_lines = duplicate_lines
        self.metrics.duplicate_blocks = duplicate_blocks[:10]  # Топ 10
        
    def _analyze_security(self):
        """Анализ безопасности"""
        print("  🔒 Анализ безопасности...")
        
        security_hotspots = 0
        security_issues = []
        
        # Паттерны потенциальных уязвимостей
        security_patterns = {
            r'strcpy\s*\(': "Использование небезопасной функции strcpy",
            r'strcat\s*\(': "Использование небезопасной функции strcat",
            r'sprintf\s*\(': "Использование небезопасной функции sprintf",
            r'gets\s*\(': "Использование небезопасной функции gets",
            r'system\s*\(': "Вызов системных команд",
            r'eval\s*\(': "Использование eval",
            r'exec\s*\(': "Выполнение внешних команд",
            r'password\s*=\s*["\'][^"\']+["\']': "Хардкод пароля",
            r'key\s*=\s*["\'][^"\']+["\']': "Хардкод ключа",
        }
        
        for src_dir in self.src_dirs:
            if not src_dir.exists():
                continue
                
            for file_path in src_dir.rglob("*.cpp"):
                try:
                    with open(file_path, 'r', encoding='utf-8') as f:
                        content = f.read()
                    
                    for pattern, description in security_patterns.items():
                        matches = re.findall(pattern, content, re.IGNORECASE)
                        if matches:
                            security_hotspots += len(matches)
                            security_issues.append({
                                "file": str(file_path.name),
                                "issue": description,
                                "count": len(matches)
                            })
                except:
                    continue
        
        self.metrics.security_hotspots = security_hotspots
        self.metrics.security_issues = security_issues
        
    def _analyze_code_smells(self):
        """Анализ code smells"""
        print("  🔍 Анализ code smells...")
        
        code_smells = 0
        
        # Анализируем различные code smells
        for src_dir in self.src_dirs:
            if not src_dir.exists():
                continue
                
            for file_path in src_dir.rglob("*.cpp"):
                code_smells += self._analyze_file_smells(file_path)
        
        self.metrics.code_smells = code_smells
        
    def _analyze_file_smells(self, file_path: Path) -> int:
        """Анализ code smells в файле"""
        try:
            with open(file_path, 'r', encoding='utf-8') as f:
                content = f.read()
                lines = content.splitlines()
        except:
            return 0
        
        smells = 0
        
        # Длинные функции (>50 строк)
        function_starts = []
        brace_count = 0
        in_function = False
        
        for i, line in enumerate(lines):
            if '{' in line and not line.strip().startswith('//'):
                if not in_function:
                    function_starts.append(i)
                    in_function = True
                brace_count += line.count('{')
            
            if '}' in line and not line.strip().startswith('//'):
                brace_count -= line.count('}')
                if brace_count == 0 and in_function:
                    function_length = i - function_starts[-1]
                    if function_length > 50:
                        smells += 1
                    in_function = False
        
        # Длинные строки (>120 символов)
        for line in lines:
            if len(line) > 120:
                smells += 1
        
        # Слишком много параметров в функции
        function_pattern = r'\w+\s+\w+\s*\(([^)]+)\)'
        functions = re.findall(function_pattern, content)
        for params in functions:
            param_count = len([p.strip() for p in params.split(',') if p.strip()])
            if param_count > 5:
                smells += 1
        
        # TODO комментарии
        todo_count = len(re.findall(r'//\s*TODO', content, re.IGNORECASE))
        smells += todo_count
        
        return smells
    
    def _calculate_ratings(self):
        """Вычисление рейтингов качества"""
        total_issues = self._total_issues()
        total_lines = self._count_total_lines()
        
        self.metrics.total_lines = total_lines
        
        if total_lines > 0:
            self.metrics.debt_ratio = (total_issues / total_lines) * 100
            
            # Рейтинги на основе плотности проблем
            if self.metrics.debt_ratio <= 0.1:
                self.metrics.maintainability_rating = "A"
            elif self.metrics.debt_ratio <= 0.5:
                self.metrics.maintainability_rating = "B"
            elif self.metrics.debt_ratio <= 1.0:
                self.metrics.maintainability_rating = "C"
            elif self.metrics.debt_ratio <= 2.0:
                self.metrics.maintainability_rating = "D"
            else:
                self.metrics.maintainability_rating = "E"
        
        # Рейтинг надёжности
        if self.metrics.complexity_issues == 0:
            self.metrics.reliability_rating = "A"
        elif self.metrics.complexity_issues <= 3:
            self.metrics.reliability_rating = "B"
        elif self.metrics.complexity_issues <= 10:
            self.metrics.reliability_rating = "C"
        else:
            self.metrics.reliability_rating = "D"
        
        # Рейтинг безопасности
        if self.metrics.security_hotspots == 0:
            self.metrics.security_rating = "A"
        elif self.metrics.security_hotspots <= 2:
            self.metrics.security_rating = "B"
        elif self.metrics.security_hotspots <= 5:
            self.metrics.security_rating = "C"
        else:
            self.metrics.security_rating = "D"
    
    def _total_issues(self) -> int:
        """Общее количество проблем"""
        return (self.metrics.code_smells + 
                self.metrics.complexity_issues + 
                self.metrics.security_hotspots)
    
    def _count_total_lines(self) -> int:
        """Подсчёт общего количества строк кода"""
        total_lines = 0
        
        for src_dir in self.src_dirs:
            if not src_dir.exists():
                continue
                
            for file_path in src_dir.rglob("*.cpp"):
                try:
                    with open(file_path, 'r', encoding='utf-8') as f:
                        total_lines += len(f.readlines())
                except:
                    continue
                    
            for file_path in src_dir.rglob("*.h"):
                try:
                    with open(file_path, 'r', encoding='utf-8') as f:
                        total_lines += len(f.readlines())
                except:
                    continue
        
        return total_lines
    
    def generate_report(self, output_file: Path):
        """Генерация отчёта о техническом долге"""
        report = {
            "timestamp": "2025-01-22T12:00:00Z",
            "project": "JXCT Soil Sensor",
            "version": "3.6.0",
            "metrics": asdict(self.metrics),
            "summary": {
                "total_issues": self._total_issues(),
                "debt_ratio": f"{self.metrics.debt_ratio:.2f}%",
                "maintainability": self.metrics.maintainability_rating,
                "reliability": self.metrics.reliability_rating,
                "security": self.metrics.security_rating
            },
            "recommendations": self._generate_recommendations()
        }
        
        with open(output_file, 'w', encoding='utf-8') as f:
            json.dump(report, f, indent=2, ensure_ascii=False)
        
        print(f"📊 Отчёт сохранён: {output_file}")
    
    def _generate_recommendations(self) -> List[str]:
        """Генерация рекомендаций по улучшению"""
        recommendations = []
        
        if self.metrics.code_smells > 20:
            recommendations.append("Высокое количество code smells - рекомендуется рефакторинг")
        
        if self.metrics.duplicated_lines > 100:
            recommendations.append("Много дублированного кода - выделить общие функции")
        
        if self.metrics.complexity_issues > 5:
            recommendations.append("Есть сложные функции - упростить логику и разбить на части")
        
        if self.metrics.security_hotspots > 0:
            recommendations.append("Обнаружены потенциальные уязвимости - провести аудит безопасности")
        
        if self.metrics.debt_ratio > 2.0:
            recommendations.append("Высокий технический долг - запланировать масштабный рефакторинг")
        
        if not recommendations:
            recommendations.append("Качество кода находится на хорошем уровне")
        
        return recommendations


def main():
    """Главная функция"""
    parser = argparse.ArgumentParser(description="Анализатор технического долга JXCT")
    parser.add_argument("--project-root", type=Path, default=Path.cwd(),
                       help="Корневая директория проекта")
    parser.add_argument("--output", type=Path, default=Path("test_reports/technical-debt.json"),
                       help="Файл для сохранения отчёта")
    parser.add_argument("--verbose", "-v", action="store_true",
                       help="Подробный вывод")
    
    args = parser.parse_args()
    
    # Создаём директорию для отчётов
    args.output.parent.mkdir(parents=True, exist_ok=True)
    
    # Анализируем проект
    analyzer = TechnicalDebtAnalyzer(args.project_root)
    metrics = analyzer.analyze()
    
    # Генерируем отчёт
    analyzer.generate_report(args.output)
    
    # Выводим краткую сводку
    print("\n📈 Краткая сводка:")
    print(f"  Всего строк кода: {metrics.total_lines}")
    print(f"  Code smells: {metrics.code_smells}")
    print(f"  Дублированные строки: {metrics.duplicated_lines}")
    print(f"  Проблемы сложности: {metrics.complexity_issues}")
    print(f"  Уязвимости: {metrics.security_hotspots}")
    print(f"  Технический долг: {metrics.debt_ratio:.2f}%")
    print(f"  Рейтинг поддерживаемости: {metrics.maintainability_rating}")
    
    # Возвращаем код завершения на основе качества
    if metrics.debt_ratio > 5.0 or metrics.security_hotspots > 5:
        return 1  # Критичные проблемы
    elif metrics.debt_ratio > 2.0:
        return 2  # Предупреждения
    else:
        return 0  # Всё хорошо


if __name__ == "__main__":
    sys.exit(main()) 