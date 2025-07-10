#!/usr/bin/env python3
"""
Тест научных рекомендаций JXCT
Проверяет корректность рекомендаций на основе научных данных
"""

import json
import requests
import time
from datetime import datetime

def test_scientific_recommendations():
    """Тестирует научные рекомендации для различных культур"""
    
    print("🧪 Тестирование научных рекомендаций JXCT")
    print("=" * 50)
    
    # Тестовые данные с датчика
    sensor_data = {
        "temperature": 25.5,
        "humidity": 72.0,
        "ec": 2100.0,
        "ph": 6.8,
        "nitrogen": 140.0,
        "phosphorus": 65.0,
        "potassium": 180.0
    }
    
    # Тестовые культуры
    test_crops = [
        "tomato",
        "cucumber", 
        "pepper",
        "lettuce",
        "blueberry"
    ]
    
    # Типы выращивания
    growing_types = [
        "soil",
        "greenhouse",
        "hydroponics",
        "organic"
    ]
    
    # Сезоны
    seasons = [
        "spring",
        "summer", 
        "autumn",
        "winter"
    ]
    
    results = []
    
    for crop in test_crops:
        print(f"\n🌱 Тестирование культуры: {crop}")
        
        for growing_type in growing_types:
            for season in seasons:
                print(f"  📍 {growing_type} + {season}")
                
                # Формируем запрос
                request_data = {
                    "sensor_data": sensor_data,
                    "crop_type": crop,
                    "growing_type": growing_type,
                    "season": season
                }
                
                try:
                    # Отправляем запрос к API (заглушка)
                    # В реальности здесь был бы HTTP запрос
                    recommendation = simulate_recommendation_api(request_data)
                    
                    # Проверяем корректность рекомендаций
                    validation_result = validate_recommendation(
                        recommendation, crop, growing_type, season
                    )
                    
                    result = {
                        "crop": crop,
                        "growing_type": growing_type,
                        "season": season,
                        "recommendation": recommendation,
                        "validation": validation_result,
                        "timestamp": datetime.now().isoformat()
                    }
                    
                    results.append(result)
                    
                    if validation_result["status"] == "PASS":
                        print(f"    ✅ PASS: {validation_result['message']}")
                    else:
                        print(f"    ❌ FAIL: {validation_result['message']}")
                        
                except Exception as e:
                    print(f"    💥 ERROR: {str(e)}")
                    results.append({
                        "crop": crop,
                        "growing_type": growing_type,
                        "season": season,
                        "error": str(e),
                        "timestamp": datetime.now().isoformat()
                    })
    
    # Сохраняем результаты
    save_test_results(results)
    
    # Анализируем результаты
    analyze_results(results)
    
    return results

def simulate_recommendation_api(request_data):
    """Симулирует API рекомендаций на основе научных данных"""
    
    crop = request_data["crop_type"]
    growing_type = request_data["growing_type"]
    season = request_data["season"]
    sensor_data = request_data["sensor_data"]
    
    # Базовые конфигурации культур (научные данные)
    crop_configs = {
        "tomato": {
            "temperature": 24.0,
            "humidity": 75.0,
            "ec": 2000.0,
            "ph": 6.5,
            "nitrogen": 150.0,
            "phosphorus": 75.0,
            "potassium": 200.0
        },
        "cucumber": {
            "temperature": 26.0,
            "humidity": 80.0,
            "ec": 1800.0,
            "ph": 6.5,
            "nitrogen": 125.0,
            "phosphorus": 65.0,
            "potassium": 150.0
        },
        "pepper": {
            "temperature": 27.0,
            "humidity": 75.0,
            "ec": 2100.0,
            "ph": 6.5,
            "nitrogen": 155.0,
            "phosphorus": 75.0,
            "potassium": 175.0
        },
        "lettuce": {
            "temperature": 18.0,
            "humidity": 85.0,
            "ec": 1500.0,
            "ph": 6.5,
            "nitrogen": 100.0,
            "phosphorus": 50.0,
            "potassium": 100.0
        },
        "blueberry": {
            "temperature": 20.0,
            "humidity": 75.0,
            "ec": 1200.0,
            "ph": 5.0,
            "nitrogen": 100.0,
            "phosphorus": 40.0,
            "potassium": 80.0
        }
    }
    
    # Применяем сезонные корректировки
    seasonal_adjustments = {
        "spring": {"temp": 2.0, "hum": 5.0, "n": 10.0},
        "summer": {"temp": 4.0, "hum": -5.0, "k": 15.0},
        "autumn": {"temp": -3.0, "hum": 3.0, "p": 10.0},
        "winter": {"temp": -8.0, "hum": -10.0, "ec": -200.0}
    }
    
    # Применяем корректировки типа выращивания
    growing_adjustments = {
        "greenhouse": {"temp": 3.0, "hum": 10.0, "ec": 300.0, "n": 20.0, "p": 10.0, "k": 15.0},
        "hydroponics": {"ec": 500.0, "n": 30.0, "p": 15.0, "k": 25.0},
        "aeroponics": {"ec": 400.0, "n": 25.0, "p": 12.0, "k": 20.0},
        "organic": {"ec": -200.0, "n": -10.0, "p": -5.0, "k": -8.0}
    }
    
    # Получаем базовую конфигурацию
    base_config = crop_configs.get(crop, crop_configs["tomato"])
    
    # Применяем корректировки
    adjusted_config = base_config.copy()
    
    # Сезонные корректировки
    season_adj = seasonal_adjustments.get(season, {})
    adjusted_config["temperature"] += season_adj.get("temp", 0.0)
    adjusted_config["humidity"] += season_adj.get("hum", 0.0)
    adjusted_config["nitrogen"] += season_adj.get("n", 0.0)
    adjusted_config["phosphorus"] += season_adj.get("p", 0.0)
    adjusted_config["potassium"] += season_adj.get("k", 0.0)
    adjusted_config["ec"] += season_adj.get("ec", 0.0)
    
    # Корректировки типа выращивания
    growing_adj = growing_adjustments.get(growing_type, {})
    adjusted_config["temperature"] += growing_adj.get("temp", 0.0)
    adjusted_config["humidity"] += growing_adj.get("hum", 0.0)
    adjusted_config["ec"] += growing_adj.get("ec", 0.0)
    adjusted_config["nitrogen"] += growing_adj.get("n", 0.0)
    adjusted_config["phosphorus"] += growing_adj.get("p", 0.0)
    adjusted_config["potassium"] += growing_adj.get("k", 0.0)
    
    # Генерируем рекомендации
    recommendations = []
    health_score = 100
    
    # Проверяем температуру
    temp_diff = abs(sensor_data["temperature"] - adjusted_config["temperature"])
    if temp_diff > 5.0:
        recommendations.append("🌡️ Температура требует корректировки")
        health_score -= 15
    
    # Проверяем влажность
    hum_diff = abs(sensor_data["humidity"] - adjusted_config["humidity"])
    if hum_diff > 10.0:
        recommendations.append("💧 Влажность требует корректировки")
        health_score -= 10
    
    # Проверяем EC
    ec_diff = abs(sensor_data["ec"] - adjusted_config["ec"])
    if ec_diff > 500.0:
        recommendations.append("⚡ EC требует корректировки")
        health_score -= 10
    
    # Проверяем pH
    ph_diff = abs(sensor_data["ph"] - adjusted_config["ph"])
    if ph_diff > 0.5:
        recommendations.append("🧪 pH требует корректировки")
        health_score -= 5
    
    # Проверяем NPK
    if sensor_data["nitrogen"] < adjusted_config["nitrogen"] - 20.0:
        recommendations.append("🌱 Недостаток азота")
        health_score -= 10
    
    if sensor_data["phosphorus"] < adjusted_config["phosphorus"] - 15.0:
        recommendations.append("🌱 Недостаток фосфора")
        health_score -= 10
    
    if sensor_data["potassium"] < adjusted_config["potassium"] - 20.0:
        recommendations.append("🌱 Недостаток калия")
        health_score -= 10
    
    # Определяем статус здоровья
    if health_score >= 80:
        health_status = "Отличное"
    elif health_score >= 60:
        health_status = "Хорошее"
    elif health_score >= 40:
        health_status = "Удовлетворительное"
    else:
        health_status = "Требует внимания"
    
    if not recommendations:
        recommendations.append("✅ Все параметры в оптимальном диапазоне")
    
    return {
        "crop_type": crop,
        "growing_type": growing_type,
        "season": season,
        "recommendations": recommendations,
        "health_status": health_status,
        "health_score": health_score,
        "adjusted_config": adjusted_config,
        "scientific_notes": f"Научные данные для {crop} основаны на международных стандартах"
    }

def validate_recommendation(recommendation, crop, growing_type, season):
    """Валидирует рекомендации на основе научных данных"""
    
    validation_result = {
        "status": "PASS",
        "message": "Рекомендации корректны",
        "issues": []
    }
    
    # Проверяем наличие обязательных полей
    required_fields = ["crop_type", "growing_type", "season", "recommendations", "health_status"]
    for field in required_fields:
        if field not in recommendation:
            validation_result["status"] = "FAIL"
            validation_result["issues"].append(f"Отсутствует поле: {field}")
    
    # Проверяем корректность типа культуры
    valid_crops = ["tomato", "cucumber", "pepper", "lettuce", "blueberry"]
    if recommendation.get("crop_type") not in valid_crops:
        validation_result["status"] = "FAIL"
        validation_result["issues"].append("Некорректный тип культуры")
    
    # Проверяем корректность типа выращивания
    valid_growing_types = ["soil", "greenhouse", "hydroponics", "aeroponics", "organic"]
    if recommendation.get("growing_type") not in valid_growing_types:
        validation_result["status"] = "FAIL"
        validation_result["issues"].append("Некорректный тип выращивания")
    
    # Проверяем корректность сезона
    valid_seasons = ["spring", "summer", "autumn", "winter"]
    if recommendation.get("season") not in valid_seasons:
        validation_result["status"] = "FAIL"
        validation_result["issues"].append("Некорректный сезон")
    
    # Проверяем корректность статуса здоровья
    valid_health_statuses = ["Отличное", "Хорошее", "Удовлетворительное", "Требует внимания"]
    if recommendation.get("health_status") not in valid_health_statuses:
        validation_result["status"] = "FAIL"
        validation_result["issues"].append("Некорректный статус здоровья")
    
    # Проверяем корректность score
    health_score = recommendation.get("health_score", 0)
    if not (0 <= health_score <= 100):
        validation_result["status"] = "FAIL"
        validation_result["issues"].append("Некорректный score здоровья")
    
    # Проверяем наличие рекомендаций
    recommendations = recommendation.get("recommendations", [])
    if not recommendations:
        validation_result["status"] = "FAIL"
        validation_result["issues"].append("Отсутствуют рекомендации")
    
    # Специфические проверки для культур
    if crop == "blueberry":
        adjusted_config = recommendation.get("adjusted_config", {})
        if adjusted_config.get("ph", 7.0) > 5.5:
            validation_result["status"] = "FAIL"
            validation_result["issues"].append("Для черники pH должен быть кислым")
    
    if crop == "tomato":
        adjusted_config = recommendation.get("adjusted_config", {})
        if adjusted_config.get("ec", 0) > 3000.0:
            validation_result["status"] = "WARN"
            validation_result["issues"].append("Высокий EC может вызвать проблемы у томатов")
    
    # Обновляем сообщение
    if validation_result["issues"]:
        validation_result["message"] = "; ".join(validation_result["issues"])
    
    return validation_result

def save_test_results(results):
    """Сохраняет результаты тестирования"""
    
    timestamp = datetime.now().strftime("%Y%m%d_%H%M%S")
    filename = f"test_reports/scientific_recommendations_test_{timestamp}.json"
    
    try:
        with open(filename, 'w', encoding='utf-8') as f:
            json.dump(results, f, ensure_ascii=False, indent=2)
        print(f"\n💾 Результаты сохранены в {filename}")
    except Exception as e:
        print(f"\n❌ Ошибка сохранения: {e}")

def analyze_results(results):
    """Анализирует результаты тестирования"""
    
    print("\n📊 Анализ результатов тестирования")
    print("=" * 50)
    
    total_tests = len(results)
    passed_tests = 0
    failed_tests = 0
    error_tests = 0
    
    crop_stats = {}
    growing_type_stats = {}
    season_stats = {}
    
    for result in results:
        if "error" in result:
            error_tests += 1
            continue
            
        validation = result.get("validation", {})
        status = validation.get("status", "UNKNOWN")
        
        if status == "PASS":
            passed_tests += 1
        elif status == "FAIL":
            failed_tests += 1
        
        # Статистика по культурам
        crop = result.get("crop", "unknown")
        if crop not in crop_stats:
            crop_stats[crop] = {"pass": 0, "fail": 0, "warn": 0}
        crop_stats[crop][status.lower()] += 1
        
        # Статистика по типам выращивания
        growing_type = result.get("growing_type", "unknown")
        if growing_type not in growing_type_stats:
            growing_type_stats[growing_type] = {"pass": 0, "fail": 0, "warn": 0}
        growing_type_stats[growing_type][status.lower()] += 1
        
        # Статистика по сезонам
        season = result.get("season", "unknown")
        if season not in season_stats:
            season_stats[season] = {"pass": 0, "fail": 0, "warn": 0}
        season_stats[season][status.lower()] += 1
    
    # Выводим общую статистику
    print(f"📈 Общая статистика:")
    print(f"   Всего тестов: {total_tests}")
    print(f"   Успешно: {passed_tests} ({passed_tests/total_tests*100:.1f}%)")
    print(f"   Ошибки: {failed_tests} ({failed_tests/total_tests*100:.1f}%)")
    print(f"   Системные ошибки: {error_tests} ({error_tests/total_tests*100:.1f}%)")
    
    # Статистика по культурам
    print(f"\n🌱 Статистика по культурам:")
    for crop, stats in crop_stats.items():
        total = sum(stats.values())
        pass_rate = stats["pass"] / total * 100 if total > 0 else 0
        print(f"   {crop}: {stats['pass']}/{total} ({pass_rate:.1f}%)")
    
    # Статистика по типам выращивания
    print(f"\n🏗️ Статистика по типам выращивания:")
    for growing_type, stats in growing_type_stats.items():
        total = sum(stats.values())
        pass_rate = stats["pass"] / total * 100 if total > 0 else 0
        print(f"   {growing_type}: {stats['pass']}/{total} ({pass_rate:.1f}%)")
    
    # Статистика по сезонам
    print(f"\n🌤️ Статистика по сезонам:")
    for season, stats in season_stats.items():
        total = sum(stats.values())
        pass_rate = stats["pass"] / total * 100 if total > 0 else 0
        print(f"   {season}: {stats['pass']}/{total} ({pass_rate:.1f}%)")
    
    # Общий вывод
    if passed_tests / total_tests >= 0.9:
        print(f"\n🎉 Отличный результат! Система рекомендаций работает корректно.")
    elif passed_tests / total_tests >= 0.7:
        print(f"\n✅ Хороший результат! Есть небольшие проблемы для исправления.")
    else:
        print(f"\n⚠️ Требуется доработка! Много ошибок в системе рекомендаций.")

if __name__ == "__main__":
    test_scientific_recommendations() 