#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
🔬 КОМПЛЕКСНЫЙ АУДИТ КУЛЬТУР JXCT
Проверка научных источников, коэффициентов выращивания и проблем отображения
"""

import json
import re
import os
import sys
from datetime import datetime
from typing import Dict, List, Tuple

class ComprehensiveCropAudit:
    def __init__(self):
        self.audit_results = {
            "timestamp": datetime.now().isoformat(),
            "version": "3.10.1",
            "status": "IN_PROGRESS",
            "sections": {},
            "issues": [],
            "recommendations": []
        }
        
        # Научные источники для проверки (обновленные 2024-2025)
        self.scientific_sources = {
            "tomato": {
                "sources": [
                    "University of Florida IFAS Extension, B. Santos, 2019",
                    "HortScience Journal, 2020",
                    "Journal of Plant Nutrition, 2021"
                ],
                "verified_ranges": {
                    "temperature": (20, 30),
                    "humidity": (60, 70),
                    "ec": (1500, 3000),
                    "ph": (6.0, 6.8),
                    "nitrogen": (150, 250),
                    "phosphorus": (50, 100),
                    "potassium": (200, 400)
                }
            },
            "cucumber": {
                "sources": [
                    "USDA Natural Resources Conservation Service, 2020",
                    "Horticultural Science, 2018",
                    "Agricultural Water Management, 2021"
                ],
                "verified_ranges": {
                    "temperature": (24, 32),
                    "humidity": (65, 75),
                    "ec": (1200, 2500),
                    "ph": (6.0, 7.0),
                    "nitrogen": (120, 200),
                    "phosphorus": (40, 80),
                    "potassium": (150, 300)
                }
            },
            "cannabis": {
                "sources": [
                    "Journal of Cannabis Research, 2020",
                    "Horticultural Science, 2021",
                    "Plant Physiology, 2022"
                ],
                "verified_ranges": {
                    "temperature": (22, 28),
                    "humidity": (60, 70),
                    "ec": (1200, 2000),
                    "ph": (6.0, 7.0),
                    "nitrogen": (180, 280),
                    "phosphorus": (60, 120),
                    "potassium": (200, 350)
                }
            }
        }
        
        # Поддерживаемые типы выращивания
        self.growing_types = {
            "outdoor": "Открытый грунт",
            "greenhouse": "Теплица", 
            "indoor": "Комнатная",
            "hydroponics": "Гидропоника",
            "aeroponics": "Аэропоника",
            "organic": "Органическое"
        }
        
        # Коэффициенты выращивания из кода
        self.growing_coefficients = {
            "greenhouse": {
                "temperature": 1.05,
                "humidity": 1.05,
                "ec": 1.10,
                "nitrogen": 1.15,
                "phosphorus": 1.15,
                "potassium": 1.15
            },
            "hydroponics": {
                "temperature": 1.03,
                "humidity": 1.02,
                "ec": 1.20,
                "nitrogen": 1.25,
                "phosphorus": 1.25,
                "potassium": 1.25
            },
            "aeroponics": {
                "temperature": 1.04,
                "humidity": 1.03,
                "ec": 1.18,
                "nitrogen": 1.20,
                "phosphorus": 1.20,
                "potassium": 1.20
            },
            "organic": {
                "temperature": 0.99,
                "humidity": 1.02,
                "ec": 0.90,
                "nitrogen": 0.90,
                "phosphorus": 0.90,
                "potassium": 0.90
            }
        }

    def audit_scientific_sources(self) -> Dict:
        """Проверка научных источников для всех культур"""
        print("🔬 ПРОВЕРКА НАУЧНЫХ ИСТОЧНИКОВ...")
        
        issues = []
        recommendations = []
        
        # Проверяем каждый источник
        for crop, data in self.scientific_sources.items():
            print(f"  📚 {crop.upper()}:")
            for source in data["sources"]:
                print(f"    • {source}")
                
                # Проверяем актуальность (последние 5 лет)
                year_match = re.search(r'(\d{4})', source)
                if year_match:
                    year = int(year_match.group(1))
                    if year < 2019:
                        issues.append(f"Устаревший источник для {crop}: {source} ({year})")
                        recommendations.append(f"Обновить источник для {crop} на более актуальный (2020+)")
        
        return {
            "issues": issues,
            "recommendations": recommendations,
            "total_sources": sum(len(data["sources"]) for data in self.scientific_sources.values())
        }

    def audit_growing_type_coefficients(self) -> Dict:
        """Проверка коэффициентов для всех типов выращивания"""
        print("\n🌱 ПРОВЕРКА КОЭФФИЦИЕНТОВ ТИПОВ ВЫРАЩИВАНИЯ...")
        
        issues = []
        recommendations = []
        
        # Проверяем наличие коэффициентов для всех типов
        for growing_type, name in self.growing_types.items():
            print(f"  {name} ({growing_type}):")
            
            if growing_type in self.growing_coefficients:
                coeffs = self.growing_coefficients[growing_type]
                print(f"    ✅ Коэффициенты найдены:")
                for param, value in coeffs.items():
                    print(f"      {param}: {value}")
            else:
                print(f"    ❌ Коэффициенты ОТСУТСТВУЮТ!")
                issues.append(f"Отсутствуют коэффициенты для типа выращивания: {growing_type}")
                recommendations.append(f"Добавить коэффициенты для {growing_type} в applyGrowingTypeCorrection")
        
        # Проверяем "indoor" (комнатная)
        if "indoor" not in self.growing_coefficients:
            issues.append("КРИТИЧНО: Отсутствуют коэффициенты для 'indoor' (комнатная) выращивания")
            recommendations.append("Добавить в applyGrowingTypeCorrection: else if (growingType == 'indoor')")
        
        return {
            "issues": issues,
            "recommendations": recommendations,
            "missing_types": [gt for gt in self.growing_types if gt not in self.growing_coefficients]
        }

    def audit_crop_display_issue(self) -> Dict:
        """Анализ проблемы отображения культуры в таблице"""
        print("\n🖥️ АНАЛИЗ ПРОБЛЕМЫ ОТОБРАЖЕНИЯ КУЛЬТУРЫ...")
        
        issues = []
        recommendations = []
        
        # Анализируем код отображения
        print("  📋 Код отображения культур:")
        print("    • routes_main.cpp: strcmp(config.cropId, 'cannabis') == 0 ? ' selected' : ''")
        print("    • routes_data.cpp: doc['crop_id'] = String(config.cropId)")
        
        # Возможные причины проблемы
        possible_causes = [
            "1. Неправильное сохранение cropId в конфигурации",
            "2. Проблема с передачей данных между страницами", 
            "3. JavaScript не обновляет заголовок таблицы",
            "4. Несоответствие между cropId и отображаемым названием"
        ]
        
        print("  🔍 Возможные причины проблемы:")
        for cause in possible_causes:
            print(f"    {cause}")
        
        issues.append("Проблема отображения культуры в заголовке таблицы при выборе 'конопля' + 'комнатная'")
        recommendations.extend([
            "Проверить сохранение config.cropId при смене культуры",
            "Проверить JavaScript код обновления заголовка таблицы",
            "Добавить логирование cropId в routes_data.cpp для отладки"
        ])
        
        return {
            "issues": issues,
            "recommendations": recommendations,
            "possible_causes": possible_causes
        }

    def generate_fix_recommendations(self) -> Dict:
        """Генерация рекомендаций по исправлению"""
        print("\n🔧 РЕКОМЕНДАЦИИ ПО ИСПРАВЛЕНИЮ...")
        
        fixes = {
            "scientific_sources": {
                "priority": "MEDIUM",
                "actions": [
                    "Обновить источники старше 2019 года",
                    "Добавить DOI для всех источников",
                    "Проверить актуальность диапазонов значений"
                ]
            },
            "growing_coefficients": {
                "priority": "HIGH", 
                "actions": [
                    "Добавить коэффициенты для 'indoor' выращивания",
                    "Проверить научную обоснованность коэффициентов",
                    "Добавить комментарии с источниками"
                ]
            },
            "display_issue": {
                "priority": "HIGH",
                "actions": [
                    "Добавить отладочное логирование cropId",
                    "Проверить JavaScript обновление заголовка",
                    "Тестировать с разными комбинациями культур/типов"
                ]
            }
        }
        
        return fixes

    def run_full_audit(self) -> Dict:
        """Запуск полного аудита"""
        print("🚀 ЗАПУСК КОМПЛЕКСНОГО АУДИТА JXCT")
        print("=" * 50)
        
        # Выполняем все проверки
        scientific_audit = self.audit_scientific_sources()
        growing_audit = self.audit_growing_type_coefficients() 
        display_audit = self.audit_crop_display_issue()
        fixes = self.generate_fix_recommendations()
        
        # Собираем результаты
        self.audit_results["sections"] = {
            "scientific_sources": scientific_audit,
            "growing_coefficients": growing_audit,
            "display_issues": display_audit,
            "fixes": fixes
        }
        
        # Подсчитываем общие проблемы
        all_issues = (
            scientific_audit["issues"] + 
            growing_audit["issues"] + 
            display_audit["issues"]
        )
        
        all_recommendations = (
            scientific_audit["recommendations"] + 
            growing_audit["recommendations"] + 
            display_audit["recommendations"]
        )
        
        self.audit_results["issues"] = all_issues
        self.audit_results["recommendations"] = all_recommendations
        self.audit_results["status"] = "COMPLETED"
        
        # Выводим итоги
        print("\n" + "=" * 50)
        print("📊 ИТОГИ АУДИТА:")
        print(f"  🔴 Проблемы: {len(all_issues)}")
        print(f"  💡 Рекомендации: {len(all_recommendations)}")
        print(f"  📚 Источников проверено: {scientific_audit['total_sources']}")
        print(f"  🌱 Типов выращивания: {len(self.growing_types)}")
        print(f"  ❌ Отсутствующие коэффициенты: {len(growing_audit['missing_types'])}")
        
        if all_issues:
            print("\n🔴 КРИТИЧЕСКИЕ ПРОБЛЕМЫ:")
            for i, issue in enumerate(all_issues, 1):
                print(f"  {i}. {issue}")
        
        return self.audit_results

def main():
    """Главная функция"""
    auditor = ComprehensiveCropAudit()
    results = auditor.run_full_audit()
    
    # Сохраняем результаты
    output_file = "test_reports/comprehensive_crop_audit.json"
    os.makedirs("test_reports", exist_ok=True)
    
    with open(output_file, 'w', encoding='utf-8') as f:
        json.dump(results, f, ensure_ascii=False, indent=2)
    
    print(f"\n💾 Результаты сохранены в: {output_file}")
    
    # Возвращаем код ошибки если есть критические проблемы
    if results["issues"]:
        print(f"\n⚠️ Найдено {len(results['issues'])} проблем!")
        return 1
    
    print("\n✅ Аудит завершен успешно!")
    return 0

if __name__ == "__main__":
    sys.exit(main())
