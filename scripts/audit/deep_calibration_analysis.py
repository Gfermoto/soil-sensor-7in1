#!/usr/bin/env python3
"""
🔬 Глубокий анализ системы калибровок JXCT
Анализирует все компоненты системы калибровок и выявляет проблемы
"""

import json
import os
import re
import sys
from pathlib import Path
from typing import Dict, List, Any, Optional

class CalibrationAnalyzer:
    def __init__(self):
        self.project_root = Path(__file__).parent.parent
        self.analysis_results = {
            "system_status": "unknown",
            "issues": [],
            "warnings": [],
            "recommendations": [],
            "components": {}
        }
    
    def analyze_calibration_system(self) -> Dict[str, Any]:
        """Основной анализ системы калибровок"""
        print("🔬 ГЛУБОКИЙ АНАЛИЗ СИСТЕМЫ КАЛИБРОВОК JXCT")
        print("=" * 60)
        
        # 1. Анализ структуры файлов
        self._analyze_file_structure()
        
        # 2. Анализ инициализации
        self._analyze_initialization()
        
        # 3. Анализ API endpoints
        self._analyze_api_endpoints()
        
        # 4. Анализ JavaScript кода
        self._analyze_javascript_code()
        
        # 5. Анализ бизнес-логики
        self._analyze_business_logic()
        
        # 6. Анализ хранения данных
        self._analyze_data_storage()
        
        # 7. Генерация отчета
        self._generate_report()
        
        return self.analysis_results
    
    def _analyze_file_structure(self):
        """Анализ структуры файлов калибровки"""
        print("\n📁 АНАЛИЗ СТРУКТУРЫ ФАЙЛОВ")
        
        calibration_files = [
            "src/sensor_correction.cpp",
            "src/sensor_correction.h", 
            "src/web/routes_calibration.cpp",
            "src/web/routes_calibration.h",
            "src/business/sensor_calibration_service.cpp",
            "src/business/sensor_calibration_service.h"
        ]
        
        missing_files = []
        existing_files = []
        
        for file_path in calibration_files:
            full_path = self.project_root / file_path
            if full_path.exists():
                existing_files.append(file_path)
                print(f"✅ {file_path}")
            else:
                missing_files.append(file_path)
                print(f"❌ {file_path} - ОТСУТСТВУЕТ")
                self.analysis_results["issues"].append(f"Отсутствует файл: {file_path}")
        
        self.analysis_results["components"]["file_structure"] = {
            "existing_files": existing_files,
            "missing_files": missing_files,
            "status": "ok" if not missing_files else "error"
        }
    
    def _analyze_initialization(self):
        """Анализ инициализации системы калибровок"""
        print("\n🔧 АНАЛИЗ ИНИЦИАЛИЗАЦИИ")
        
        init_files = [
            "src/modbus_sensor.cpp",
            "src/main.cpp"
        ]
        
        init_patterns = [
            r"SensorCorrection::init\(\)",
            r"gSensorCorrection\.init\(\)",
            r"gCalibrationService",
            r"gCompensationService"
        ]
        
        for file_path in init_files:
            full_path = self.project_root / file_path
            if not full_path.exists():
                continue
                
            content = full_path.read_text(encoding='utf-8', errors='ignore')
            print(f"\n📄 {file_path}:")
            
            for pattern in init_patterns:
                matches = re.findall(pattern, content)
                if matches:
                    print(f"  ✅ {pattern}: найдено {len(matches)} раз")
                else:
                    print(f"  ❌ {pattern}: НЕ НАЙДЕНО")
                    self.analysis_results["warnings"].append(f"Отсутствует инициализация {pattern} в {file_path}")
    
    def _analyze_api_endpoints(self):
        """Анализ API endpoints калибровки"""
        print("\n🔌 АНАЛИЗ API ENDPOINTS")
        
        api_file = self.project_root / "src/web/routes_calibration.cpp"
        if not api_file.exists():
            self.analysis_results["issues"].append("Файл routes_calibration.cpp не найден")
            return
        
        content = api_file.read_text(encoding='utf-8', errors='ignore')
        
        # Ищем регистрацию endpoints
        endpoints = [
            "/api/calibration/status",
            "/api/calibration/ph", 
            "/api/calibration/ec",
            "/api/calibration/temperature",
            "/api/calibration/humidity",
            "/api/calibration/npk"
        ]
        
        for endpoint in endpoints:
            if endpoint in content:
                print(f"✅ {endpoint}")
            else:
                print(f"❌ {endpoint} - НЕ ЗАРЕГИСТРИРОВАН")
                self.analysis_results["issues"].append(f"API endpoint не зарегистрирован: {endpoint}")
    
    def _analyze_javascript_code(self):
        """Анализ JavaScript кода калибровки"""
        print("\n📜 АНАЛИЗ JAVASCRIPT КОДА")
        
        js_file = self.project_root / "src/web/routes_calibration.cpp"
        if not js_file.exists():
            self.analysis_results["issues"].append("Файл routes_calibration.cpp не найден")
            return
        
        content = js_file.read_text(encoding='utf-8', errors='ignore')
        
        # Ищем JavaScript функции
        js_functions = [
            "loadCalibrationStatus",
            "calibratePH",
            "calibrateEC", 
            "calibrateTemperature",
            "calibrateHumidity",
            "calibrateNPK"
        ]
        
        for func in js_functions:
            if func in content:
                print(f"✅ {func}()")
            else:
                print(f"❌ {func}() - НЕ НАЙДЕНА")
                self.analysis_results["issues"].append(f"JavaScript функция не найдена: {func}")
        
        # Проверяем обработку ошибок
        if "console.error" in content:
            print("✅ Обработка ошибок в консоли")
        else:
            print("⚠️ Отсутствует обработка ошибок в консоли")
            self.analysis_results["warnings"].append("Отсутствует обработка ошибок в JavaScript")
    
    def _analyze_business_logic(self):
        """Анализ бизнес-логики калибровки"""
        print("\n🧠 АНАЛИЗ БИЗНЕС-ЛОГИКИ")
        
        business_files = [
            "src/business/sensor_calibration_service.cpp",
            "src/sensor_correction.cpp"
        ]
        
        for file_path in business_files:
            full_path = self.project_root / file_path
            if not full_path.exists():
                continue
                
            content = full_path.read_text(encoding='utf-8', errors='ignore')
            print(f"\n📄 {file_path}:")
            
            # Проверяем функции калибровки
            calibration_functions = [
                "calculatePHCalibration",
                "calculateECCalibration", 
                "calculateTemperatureCalibration",
                "calculateHumidityCalibration"
            ]
            
            for func in calibration_functions:
                if func in content:
                    print(f"  ✅ {func}")
                else:
                    print(f"  ❌ {func} - НЕ НАЙДЕНА")
                    self.analysis_results["issues"].append(f"Функция калибровки не найдена: {func}")
            
            # Проверяем обработку ошибок
            if "try" in content and "catch" in content:
                print("  ✅ Обработка исключений")
            else:
                print("  ⚠️ Отсутствует обработка исключений")
                self.analysis_results["warnings"].append(f"Отсутствует обработка исключений в {file_path}")
    
    def _analyze_data_storage(self):
        """Анализ хранения данных калибровки"""
        print("\n💾 АНАЛИЗ ХРАНЕНИЯ ДАННЫХ")
        
        storage_file = self.project_root / "src/sensor_correction.cpp"
        if not storage_file.exists():
            self.analysis_results["issues"].append("Файл sensor_correction.cpp не найден")
            return
        
        content = storage_file.read_text(encoding='utf-8', errors='ignore')
        
        # Проверяем функции сохранения/загрузки
        storage_functions = [
            "saveFactors",
            "loadFactors",
            "Preferences"
        ]
        
        for func in storage_functions:
            if func in content:
                print(f"✅ {func}")
            else:
                print(f"❌ {func} - НЕ НАЙДЕН")
                self.analysis_results["issues"].append(f"Функция хранения не найдена: {func}")
        
        # Проверяем структуру данных
        if "CorrectionFactors" in content:
            print("✅ Структура CorrectionFactors")
        else:
            print("❌ Структура CorrectionFactors - НЕ НАЙДЕНА")
            self.analysis_results["issues"].append("Структура CorrectionFactors не найдена")
    
    def _generate_report(self):
        """Генерация итогового отчета"""
        print("\n📊 ИТОГОВЫЙ ОТЧЕТ")
        print("=" * 60)
        
        # Определяем общий статус
        if self.analysis_results["issues"]:
            self.analysis_results["system_status"] = "error"
            print("❌ СИСТЕМА КАЛИБРОВОК: КРИТИЧЕСКИЕ ОШИБКИ")
        elif self.analysis_results["warnings"]:
            self.analysis_results["system_status"] = "warning"
            print("⚠️ СИСТЕМА КАЛИБРОВОК: ПРЕДУПРЕЖДЕНИЯ")
        else:
            self.analysis_results["system_status"] = "ok"
            print("✅ СИСТЕМА КАЛИБРОВОК: РАБОТАЕТ КОРРЕКТНО")
        
        # Выводим проблемы
        if self.analysis_results["issues"]:
            print(f"\n🚨 КРИТИЧЕСКИЕ ПРОБЛЕМЫ ({len(self.analysis_results['issues'])}):")
            for issue in self.analysis_results["issues"]:
                print(f"  • {issue}")
        
        if self.analysis_results["warnings"]:
            print(f"\n⚠️ ПРЕДУПРЕЖДЕНИЯ ({len(self.analysis_results['warnings'])}):")
            for warning in self.analysis_results["warnings"]:
                print(f"  • {warning}")
        
        # Рекомендации
        self._generate_recommendations()
        
        if self.analysis_results["recommendations"]:
            print(f"\n💡 РЕКОМЕНДАЦИИ ({len(self.analysis_results['recommendations'])}):")
            for rec in self.analysis_results["recommendations"]:
                print(f"  • {rec}")
        
        # Сохраняем отчет
        report_file = self.project_root / "test_reports" / "deep_calibration_analysis.json"
        report_file.parent.mkdir(exist_ok=True)
        
        with open(report_file, 'w', encoding='utf-8') as f:
            json.dump(self.analysis_results, f, indent=2, ensure_ascii=False)
        
        print(f"\n📄 Отчет сохранен: {report_file}")
    
    def _generate_recommendations(self):
        """Генерация рекомендаций на основе анализа"""
        recommendations = []
        
        # Проверяем наличие проблем
        if any("отсутствует" in issue.lower() for issue in self.analysis_results["issues"]):
            recommendations.append("Проверить все отсутствующие файлы и функции")
        
        if any("инициализация" in issue.lower() for issue in self.analysis_results["issues"]):
            recommendations.append("Добавить правильную инициализацию в main.cpp")
        
        if any("api endpoint" in issue.lower() for issue in self.analysis_results["issues"]):
            recommendations.append("Зарегистрировать все необходимые API endpoints")
        
        if any("javascript" in issue.lower() for issue in self.analysis_results["issues"]):
            recommendations.append("Проверить JavaScript код на странице калибровки")
        
        if any("исключений" in warning.lower() for warning in self.analysis_results["warnings"]):
            recommendations.append("Добавить обработку исключений в критических местах")
        
        # Общие рекомендации
        recommendations.extend([
            "Провести тестирование всех функций калибровки",
            "Проверить сохранение/загрузку калибровочных данных",
            "Добавить логирование для отладки проблем",
            "Создать unit-тесты для функций калибровки"
        ])
        
        self.analysis_results["recommendations"] = recommendations

def main():
    """Главная функция"""
    analyzer = CalibrationAnalyzer()
    results = analyzer.analyze_calibration_system()
    
    # Возвращаем код выхода
    if results["system_status"] == "error":
        sys.exit(1)
    elif results["system_status"] == "warning":
        sys.exit(2)
    else:
        sys.exit(0)

if __name__ == "__main__":
    main()
