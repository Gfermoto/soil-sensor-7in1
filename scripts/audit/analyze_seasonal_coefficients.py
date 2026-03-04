#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
🔬 Детальный анализ сезонных коэффициентов
Поиск конкретных числовых значений сезонных корректировок в научных источниках
"""

import json
import requests
import time
from datetime import datetime
from typing import Dict, List

class SeasonalCoefficientsAnalyzer:
    """Анализатор сезонных коэффициентов"""
    
    def __init__(self):
        self.analysis_results = {
            "timestamp": datetime.now().isoformat(),
            "specific_coefficients_found": [],
            "validation_status": "NEEDS_VERIFICATION",
            "recommendations": []
        }
        
        # Загружаем найденные источники
        try:
            with open("test_reports/seasonal_sources_validation.json", "r", encoding="utf-8") as f:
                self.sources_data = json.load(f)
        except FileNotFoundError:
            print("❌ Файл с источниками не найден. Запустите сначала validate_seasonal_sources.py")
            self.sources_data = {"sources_found": []}

    def search_specific_coefficients(self, doi: str) -> Dict:
        """Поиск конкретных коэффициентов в источнике"""
        try:
            # Получаем метаданные статьи
            url = f"https://api.crossref.org/works/{doi}"
            response = requests.get(url, timeout=10)
            response.raise_for_status()
            
            data = response.json()
            work = data.get("message", {})
            
            # Извлекаем информацию
            title = work.get("title", [""])[0]
            abstract = work.get("abstract", "")
            published = work.get("published-print", {}).get("date-parts", [[0]])[0][0]
            
            # Анализируем на наличие коэффициентов
            coefficients = self.extract_coefficients_from_text(title + " " + abstract)
            
            return {
                "doi": doi,
                "title": title,
                "year": published,
                "coefficients_found": len(coefficients) > 0,
                "coefficients": coefficients,
                "relevance_score": self.calculate_relevance_score(title, abstract)
            }
            
        except Exception as e:
            return {
                "doi": doi,
                "error": str(e),
                "coefficients_found": False
            }

    def extract_coefficients_from_text(self, text: str) -> List[Dict]:
        """Извлечение коэффициентов из текста"""
        coefficients = []
        text_lower = text.lower()
        
        # Ищем сезонные паттерны
        seasonal_patterns = [
            r"spring.*(\d+\.?\d*)%?",
            r"summer.*(\d+\.?\d*)%?",
            r"autumn.*(\d+\.?\d*)%?",
            r"winter.*(\d+\.?\d*)%?",
            r"seasonal.*(\d+\.?\d*)%?",
            r"nitrogen.*(\d+\.?\d*)%?",
            r"phosphorus.*(\d+\.?\d*)%?",
            r"potassium.*(\d+\.?\d*)%?"
        ]
        
        import re
        for pattern in seasonal_patterns:
            matches = re.findall(pattern, text_lower)
            for match in matches:
                try:
                    value = float(match)
                    if 0.5 <= value <= 2.0:  # Разумный диапазон для множителей
                        coefficients.append({
                            "type": "seasonal_factor",
                            "value": value,
                            "pattern": pattern
                        })
                except ValueError:
                    continue
                    
        return coefficients

    def calculate_relevance_score(self, title: str, abstract: str) -> float:
        """Расчет релевантности источника"""
        score = 0.0
        text = (title + " " + abstract).lower()
        
        # Ключевые слова для сезонных корректировок
        keywords = {
            "seasonal": 2.0,
            "season": 1.5,
            "spring": 1.0,
            "summer": 1.0,
            "autumn": 1.0,
            "winter": 1.0,
            "nitrogen": 1.5,
            "phosphorus": 1.5,
            "potassium": 1.5,
            "fertilizer": 1.0,
            "nutrient": 1.0,
            "agricultural": 1.0,
            "crop": 1.0
        }
        
        for keyword, weight in keywords.items():
            if keyword in text:
                score += weight
                
        return min(score, 10.0)  # Максимум 10 баллов

    def analyze_high_relevance_sources(self) -> List[Dict]:
        """Анализ высокорелевантных источников"""
        print("🔍 Анализ высокорелевантных источников...")
        
        high_relevance_sources = [
            s for s in self.sources_data["sources_found"] 
            if s["relevance"] == "high" and s["doi"] != "N/A"
        ]
        
        analyzed_sources = []
        for i, source in enumerate(high_relevance_sources[:5]):  # Анализируем первые 5
            print(f"   Анализ источника {i+1}/5: {source['source'][:50]}...")
            
            analysis = self.search_specific_coefficients(source["doi"])
            analyzed_sources.append(analysis)
            
            if analysis["coefficients_found"]:
                print("   ✅ Найдены коэффициенты!")
            else:
                print("   ⚠️ Коэффициенты не найдены")
                
            time.sleep(2)  # Уважаем API
            
        return analyzed_sources

    def validate_current_coefficients(self) -> Dict:
        """Валидация текущих коэффициентов"""
        current_coefficients = {
            "spring": {"nitrogen": 1.15, "phosphorus": 1.10, "potassium": 1.12},
            "summer": {"nitrogen": 1.08, "phosphorus": 1.05, "potassium": 1.20},
            "autumn": {"nitrogen": 1.06, "phosphorus": 1.12, "potassium": 1.15},
            "winter": {"nitrogen": 0.95, "phosphorus": 1.08, "potassium": 1.10}  # Исправлено с 0.85
        }
        
        validation = {
            "coefficients": current_coefficients,
            "scientific_basis": "PARTIALLY_VERIFIED",
            "issues": [],
            "strengths": []
        }
        
        # Проверяем разумность значений
        for season, coeffs in current_coefficients.items():
            for nutrient, value in coeffs.items():
                if value < 0.5 or value > 2.0:
                    validation["issues"].append(f"Подозрительное значение {season}.{nutrient}: {value}")
                elif 0.8 <= value <= 1.3:
                    validation["strengths"].append(f"Разумное значение {season}.{nutrient}: {value}")
                    
        # Проверяем логику сезонов
        if current_coefficients["winter"]["nitrogen"] < 1.0:
            validation["strengths"].append("Логичное снижение азота зимой")
        else:
            validation["issues"].append("Неожиданно высокий азот зимой")
            
        if current_coefficients["summer"]["potassium"] > 1.15:
            validation["strengths"].append("Логичное повышение калия летом для жаростойкости")
            
        return validation

    def run_comprehensive_analysis(self) -> Dict:
        """Комплексный анализ"""
        print("🔬 ДЕТАЛЬНЫЙ АНАЛИЗ СЕЗОННЫХ КОЭФФИЦИЕНТОВ")
        print("=" * 60)
        print(f"📅 Дата: {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}")
        print()
        
        # Анализируем источники
        analyzed_sources = self.analyze_high_relevance_sources()
        
        # Валидируем текущие коэффициенты
        validation = self.validate_current_coefficients()
        
        # Формируем рекомендации
        recommendations = []
        
        sources_with_coefficients = [s for s in analyzed_sources if s["coefficients_found"]]
        if sources_with_coefficients:
            recommendations.append("✅ Найдены источники с конкретными коэффициентами")
            self.analysis_results["validation_status"] = "PARTIALLY_VERIFIED"
        else:
            recommendations.append("⚠️ Не найдены источники с конкретными коэффициентами")
            
        if validation["strengths"]:
            recommendations.append("✅ Текущие коэффициенты имеют логичное обоснование")
            
        if validation["issues"]:
            recommendations.append("⚠️ Обнаружены потенциальные проблемы в коэффициентах")
            
        # Проверяем соответствие научным принципам
        if self.check_scientific_principles():
            recommendations.append("✅ Коэффициенты соответствуют агрономическим принципам")
        else:
            recommendations.append("❌ Коэффициенты не соответствуют агрономическим принципам")
            
        # Сохраняем результаты
        self.analysis_results["specific_coefficients_found"] = analyzed_sources
        self.analysis_results["current_coefficients_validation"] = validation
        self.analysis_results["recommendations"] = recommendations
        
        return self.analysis_results

    def check_scientific_principles(self) -> bool:
        """Проверка соответствия агрономическим принципам"""
        # Основные принципы:
        # 1. Зимой растения в покое - меньше азота
        # 2. Летом больше калия для жаростойкости
        # 3. Весной больше азота для роста
        # 4. Осенью больше фосфора для подготовки к зиме
        
        current = {
            "spring": {"nitrogen": 1.15, "phosphorus": 1.10, "potassium": 1.12},
            "summer": {"nitrogen": 1.08, "phosphorus": 1.05, "potassium": 1.20},
            "autumn": {"nitrogen": 1.06, "phosphorus": 1.12, "potassium": 1.15},
            "winter": {"nitrogen": 0.95, "phosphorus": 1.08, "potassium": 1.10}  # Исправлено с 0.85
        }
        
        principles_met = 0
        
        # Принцип 1: Зимой меньше азота
        if current["winter"]["nitrogen"] < current["spring"]["nitrogen"]:
            principles_met += 1
            
        # Принцип 2: Летом больше калия
        if current["summer"]["potassium"] > current["winter"]["potassium"]:
            principles_met += 1
            
        # Принцип 3: Весной больше азота
        if current["spring"]["nitrogen"] > current["autumn"]["nitrogen"]:
            principles_met += 1
            
        # Принцип 4: Осенью больше фосфора
        if current["autumn"]["phosphorus"] > current["summer"]["phosphorus"]:
            principles_met += 1
            
        return principles_met >= 3  # Минимум 3 из 4 принципов

    def generate_detailed_report(self) -> str:
        """Генерация детального отчета"""
        report = []
        report.append("🔬 ДЕТАЛЬНЫЙ ОТЧЕТ ПО СЕЗОННЫМ КОЭФФИЦИЕНТАМ")
        report.append("=" * 60)
        report.append(f"📅 Дата: {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}")
        report.append(f"📊 Статус: {self.analysis_results['validation_status']}")
        report.append("")
        
        # Анализ источников
        report.append("📚 АНАЛИЗ ИСТОЧНИКОВ:")
        sources_with_coeffs = [s for s in self.analysis_results["specific_coefficients_found"] if s["coefficients_found"]]
        if sources_with_coeffs:
            for i, source in enumerate(sources_with_coeffs[:3], 1):
                report.append(f"   {i}. {source['title'][:60]}...")
                report.append(f"      DOI: {source['doi']}")
                report.append(f"      Релевантность: {source['relevance_score']:.1f}/10")
                report.append(f"      Коэффициенты: {len(source['coefficients'])}")
                report.append("")
        else:
            report.append("   ⚠️ Не найдены источники с конкретными коэффициентами")
            report.append("")
            
        # Валидация текущих коэффициентов
        validation = self.analysis_results["current_coefficients_validation"]
        report.append("🔍 ВАЛИДАЦИЯ ТЕКУЩИХ КОЭФФИЦИЕНТОВ:")
        
        if validation["strengths"]:
            report.append("   ✅ Сильные стороны:")
            for strength in validation["strengths"]:
                report.append(f"      • {strength}")
                
        if validation["issues"]:
            report.append("   ⚠️ Проблемы:")
            for issue in validation["issues"]:
                report.append(f"      • {issue}")
                
        report.append("")
        
        # Рекомендации
        report.append("💡 РЕКОМЕНДАЦИИ:")
        for rec in self.analysis_results["recommendations"]:
            report.append(f"   {rec}")
            
        return "\n".join(report)

def main():
    """Главная функция"""
    analyzer = SeasonalCoefficientsAnalyzer()
    
    try:
        # Запускаем анализ
        results = analyzer.run_comprehensive_analysis()
        
        # Генерируем отчет
        report = analyzer.generate_detailed_report()
        print(report)
        
        # Сохраняем результаты
        with open("test_reports/seasonal_coefficients_analysis.json", "w", encoding="utf-8") as f:
            json.dump(results, f, indent=2, ensure_ascii=False)
            
        print("\n📄 Отчет сохранен: test_reports/seasonal_coefficients_analysis.json")
        
        return results["validation_status"] != "NEEDS_VERIFICATION"
        
    except Exception as e:
        print(f"💥 КРИТИЧЕСКАЯ ОШИБКА: {e}")
        return False

if __name__ == "__main__":
    success = main()
    exit(0 if success else 1) 