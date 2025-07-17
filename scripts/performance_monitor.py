#!/usr/bin/env python3
"""
📊 JXCT Performance Monitor v1.0
Мониторинг производительности и размера прошивки
"""

import os
import json
import subprocess
import time
from pathlib import Path
from typing import Dict, List, Optional
from dataclasses import dataclass
from datetime import datetime

@dataclass
class BuildMetrics:
    timestamp: str
    firmware_size: int
    flash_usage: int
    ram_usage: int
    build_time: float
    warnings_count: int
    errors_count: int

@dataclass
class PerformanceMetrics:
    timestamp: str
    memory_usage: Dict[str, int]
    cpu_usage: float
    free_heap: int
    min_free_heap: int
    max_alloc_heap: int

class PerformanceMonitor:
    def __init__(self, project_root: str = "."):
        self.project_root = Path(project_root)
        self.history_file = "test_reports/performance_history.json"
        self.thresholds = {
            "firmware_size_mb": 1.5,
            "ram_usage_percent": 80,
            "build_time_seconds": 60,
            "warnings_threshold": 50
        }
    
    def collect_build_metrics(self) -> BuildMetrics:
        """Собирает метрики сборки"""
        print("🔨 Сбор метрик сборки...")
        
        start_time = time.time()
        
        try:
            # Запускаем сборку
            result = subprocess.run(
                ["pio", "run", "-e", "esp32dev"],
                capture_output=True,
                text=True,
                cwd=self.project_root
            )
            
            build_time = time.time() - start_time
            
            # Анализируем вывод
            output = result.stdout + result.stderr
            firmware_size = self._extract_firmware_size(output)
            flash_usage = self._extract_flash_usage(output)
            ram_usage = self._extract_ram_usage(output)
            warnings_count = output.count("warning:")
            errors_count = output.count("error:")
            
            return BuildMetrics(
                timestamp=datetime.now().isoformat(),
                firmware_size=firmware_size,
                flash_usage=flash_usage,
                ram_usage=ram_usage,
                build_time=build_time,
                warnings_count=warnings_count,
                errors_count=errors_count
            )
            
        except Exception as e:
            print(f"❌ Ошибка сбора метрик сборки: {e}")
            return BuildMetrics(
                timestamp=datetime.now().isoformat(),
                firmware_size=0,
                flash_usage=0,
                ram_usage=0,
                build_time=0,
                warnings_count=0,
                errors_count=1
            )
    
    def _extract_firmware_size(self, output: str) -> int:
        """Извлекает размер прошивки из вывода сборки"""
        # Ищем строки типа "Memory Summary:"
        lines = output.split('\n')
        for line in lines:
            if "firmware.bin" in line and "bytes" in line:
                # Извлекаем размер в байтах
                import re
                match = re.search(r'(\d+)\s*bytes', line)
                if match:
                    return int(match.group(1))
        return 0
    
    def _extract_flash_usage(self, output: str) -> int:
        """Извлекает использование Flash"""
        lines = output.split('\n')
        for line in lines:
            if "Flash" in line and "%" in line:
                import re
                match = re.search(r'(\d+)%', line)
                if match:
                    return int(match.group(1))
        return 0
    
    def _extract_ram_usage(self, output: str) -> int:
        """Извлекает использование RAM"""
        lines = output.split('\n')
        for line in lines:
            if "RAM" in line and "%" in line:
                import re
                match = re.search(r'(\d+)%', line)
                if match:
                    return int(match.group(1))
        return 0
    
    def analyze_performance_trends(self) -> Dict:
        """Анализирует тренды производительности"""
        if not os.path.exists(self.history_file):
            return {"status": "no_history"}
        
        with open(self.history_file, 'r') as f:
            history = json.load(f)
        
        if not history.get("builds"):
            return {"status": "no_builds"}
        
        builds = history["builds"]
        
        # Анализируем тренды
        firmware_sizes = [b["firmware_size"] for b in builds[-10:]]  # Последние 10 сборок
        build_times = [b["build_time"] for b in builds[-10:]]
        
        trends = {
            "firmware_size_trend": "stable",
            "build_time_trend": "stable",
            "size_growth_rate": 0,
            "time_growth_rate": 0
        }
        
        if len(firmware_sizes) >= 2:
            size_growth = (firmware_sizes[-1] - firmware_sizes[0]) / len(firmware_sizes)
            trends["size_growth_rate"] = size_growth
            trends["firmware_size_trend"] = "increasing" if size_growth > 1000 else "stable"
        
        if len(build_times) >= 2:
            time_growth = (build_times[-1] - build_times[0]) / len(build_times)
            trends["time_growth_rate"] = time_growth
            trends["build_time_trend"] = "increasing" if time_growth > 5 else "stable"
        
        return trends
    
    def check_thresholds(self, metrics: BuildMetrics) -> List[str]:
        """Проверяет превышение пороговых значений"""
        warnings = []
        
        if metrics.firmware_size > self.thresholds["firmware_size_mb"] * 1024 * 1024:
            warnings.append(f"⚠️ Размер прошивки превышает порог: {metrics.firmware_size / 1024 / 1024:.2f} MB")
        
        if metrics.ram_usage > self.thresholds["ram_usage_percent"]:
            warnings.append(f"⚠️ Использование RAM превышает порог: {metrics.ram_usage}%")
        
        if metrics.build_time > self.thresholds["build_time_seconds"]:
            warnings.append(f"⚠️ Время сборки превышает порог: {metrics.build_time:.1f} сек")
        
        if metrics.warnings_count > self.thresholds["warnings_threshold"]:
            warnings.append(f"⚠️ Много предупреждений: {metrics.warnings_count}")
        
        return warnings
    
    def save_metrics(self, metrics: BuildMetrics):
        """Сохраняет метрики в историю"""
        os.makedirs(os.path.dirname(self.history_file), exist_ok=True)
        
        if os.path.exists(self.history_file):
            with open(self.history_file, 'r') as f:
                history = json.load(f)
        else:
            history = {"builds": [], "performance": []}
        
        # Добавляем новые метрики
        history["builds"].append({
            "timestamp": metrics.timestamp,
            "firmware_size": metrics.firmware_size,
            "flash_usage": metrics.flash_usage,
            "ram_usage": metrics.ram_usage,
            "build_time": metrics.build_time,
            "warnings_count": metrics.warnings_count,
            "errors_count": metrics.errors_count
        })
        
        # Ограничиваем историю последними 50 записями
        if len(history["builds"]) > 50:
            history["builds"] = history["builds"][-50:]
        
        with open(self.history_file, 'w') as f:
            json.dump(history, f, indent=2)
    
    def generate_report(self) -> str:
        """Генерирует отчет о производительности"""
        metrics = self.collect_build_metrics()
        trends = self.analyze_performance_trends()
        warnings = self.check_thresholds(metrics)
        
        self.save_metrics(metrics)
        
        report = f"""
📊 ОТЧЕТ О ПРОИЗВОДИТЕЛЬНОСТИ JXCT
=====================================
Время: {metrics.timestamp}
Размер прошивки: {metrics.firmware_size / 1024 / 1024:.2f} MB
Использование Flash: {metrics.flash_usage}%
Использование RAM: {metrics.ram_usage}%
Время сборки: {metrics.build_time:.1f} сек
Предупреждения: {metrics.warnings_count}
Ошибки: {metrics.errors_count}

📈 ТРЕНДЫ:
Размер прошивки: {trends.get('firmware_size_trend', 'unknown')}
Время сборки: {trends.get('build_time_trend', 'unknown')}
Рост размера: {trends.get('size_growth_rate', 0):.0f} байт/сборка
Рост времени: {trends.get('time_growth_rate', 0):.1f} сек/сборка

"""
        
        if warnings:
            report += "⚠️ ПРЕДУПРЕЖДЕНИЯ:\n"
            for warning in warnings:
                report += f"  {warning}\n"
        
        return report

def main():
    monitor = PerformanceMonitor()
    report = monitor.generate_report()
    
    print(report)
    
    # Сохраняем отчет
    report_file = "test_reports/performance_report.txt"
    os.makedirs(os.path.dirname(report_file), exist_ok=True)
    
    with open(report_file, 'w', encoding='utf-8') as f:
        f.write(report)
    
    print(f"✅ Отчет сохранен: {report_file}")

if __name__ == "__main__":
    main() 