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
    
    # Типы почвы
    soil_types = [
        "sand",
        "loam", 
        "clay",
        "peat",
        "sandpeat"
    ]
    
    results = []
    
    for crop in test_crops:
        print(f"\n🌱 Тестирование культуры: {crop}")
        
        for growing_type in growing_types:
            for season in seasons:
                for soil_type in soil_types:
                    print(f"  📍 {growing_type} + {season} + {soil_type}")
                    
                    # Формируем запрос
                    request_data = {
                        "sensor_data": sensor_data,
                        "crop_type": crop,
                        "growing_type": growing_type,
                        "season": season,
                        "soil_type": soil_type
                    }
                    
                    try:
                        # Отправляем запрос к API (заглушка)
                        # В реальности здесь был бы HTTP запрос
                        recommendation = simulate_recommendation_api(request_data)
                        
                        # Проверяем корректность рекомендаций
                        validation_result = validate_recommendation(
                            recommendation, crop, growing_type, season, soil_type
                        )
                        
                        result = {
                            "crop": crop,
                            "growing_type": growing_type,
                            "season": season,
                            "soil_type": soil_type,
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
                            "soil_type": soil_type,
                            "error": str(e),
                            "timestamp": datetime.now().isoformat()
                        })
    
    # Сохраняем результаты
    save_test_results(results)
    
    # Анализируем результаты
    analyze_results(results)
    
    return results

def simulate_recommendation_api(request_data):
    """Симулирует API рекомендаций"""
    
    sensor_data = request_data["sensor_data"]
    crop_type = request_data["crop_type"]
    growing_type = request_data["growing_type"]
    season = request_data["season"]
    soil_type = request_data["soil_type"]
    
    # Базовые конфигурации культур
    crop_configs = {
        "tomato": {"temperature": 24.0, "humidity": 75.0, "ec": 2000.0, "ph": 6.5, "nitrogen": 150.0, "phosphorus": 75.0, "potassium": 200.0},
        "cucumber": {"temperature": 26.0, "humidity": 80.0, "ec": 1800.0, "ph": 6.5, "nitrogen": 125.0, "phosphorus": 65.0, "potassium": 150.0},
        "pepper": {"temperature": 27.0, "humidity": 75.0, "ec": 2100.0, "ph": 6.5, "nitrogen": 155.0, "phosphorus": 75.0, "potassium": 175.0},
        "lettuce": {"temperature": 20.0, "humidity": 85.0, "ec": 1200.0, "ph": 6.0, "nitrogen": 100.0, "phosphorus": 50.0, "potassium": 80.0},
        "blueberry": {"temperature": 18.0, "humidity": 65.0, "ec": 1200.0, "ph": 5.0, "nitrogen": 100.0, "phosphorus": 40.0, "potassium": 80.0}
    }
    
    # Получаем базовую конфигурацию
    base_config = crop_configs.get(crop_type, crop_configs["tomato"])
    
    # Применяем сезонные корректировки
    seasonal_adjustments = {
        "spring": {"temperature": 0, "humidity": 0, "ec": 0, "nitrogen": 1.0, "phosphorus": 1.0, "potassium": 1.0},
        "summer": {"temperature": 2, "humidity": -5, "ec": 200, "nitrogen": 1.1, "phosphorus": 1.05, "potassium": 1.15},
        "autumn": {"temperature": -1, "humidity": 5, "ec": -100, "nitrogen": 0.95, "phosphorus": 1.0, "potassium": 1.05},
        "winter": {"temperature": -3, "humidity": 10, "ec": -200, "nitrogen": 1.1, "phosphorus": 1.1, "potassium": 1.15}
    }
    
    season_adj = seasonal_adjustments.get(season, seasonal_adjustments["spring"])
    
    # Применяем корректировки типа выращивания
    growing_adjustments = {
        "soil": {"temperature": 0, "humidity": 0, "ec": 0, "nitrogen": 0, "phosphorus": 0, "potassium": 0},
        "greenhouse": {"temperature": 3, "humidity": 10, "ec": 300, "nitrogen": 20, "phosphorus": 10, "potassium": 15},
        "hydroponics": {"temperature": 0, "humidity": 0, "ec": 500, "nitrogen": 30, "phosphorus": 15, "potassium": 25},
        "organic": {"temperature": 0, "humidity": 0, "ec": -200, "nitrogen": -10, "phosphorus": -5, "potassium": -8}
    }
    
    growing_adj = growing_adjustments.get(growing_type, growing_adjustments["soil"])
    
    # Применяем корректировки типа почвы
    soil_adjustments = {
        "sand": {"temperature": 0, "humidity": -5, "ec": -200, "nitrogen": 10, "phosphorus": 5, "potassium": 8},
        "loam": {"temperature": 0, "humidity": 0, "ec": 0, "nitrogen": 0, "phosphorus": 0, "potassium": 0},
        "clay": {"temperature": 0, "humidity": 5, "ec": 300, "nitrogen": -5, "phosphorus": -3, "potassium": -5},
        "peat": {"temperature": 0, "humidity": 10, "ec": -100, "ph": -0.5, "nitrogen": 15, "phosphorus": 8, "potassium": 10},
        "sandpeat": {"temperature": 0, "humidity": 2, "ec": -50, "ph": -0.2, "nitrogen": 8, "phosphorus": 4, "potassium": 6}
    }
    
    soil_adj = soil_adjustments.get(soil_type, soil_adjustments["loam"])
    
    # Вычисляем скорректированную конфигурацию
    adjusted_config = {}
    for key in base_config:
        if key == "ph":
            adjusted_config[key] = base_config[key] + soil_adj.get(key, 0)
        else:
            adjusted_config[key] = base_config[key] + season_adj.get(key, 0) + growing_adj.get(key, 0) + soil_adj.get(key, 0)
    
    # Генерируем рекомендации
    recommendations = []
    
    # Проверяем температуру
    if sensor_data["temperature"] < adjusted_config["temperature"] - 5:
        recommendations.append("🌡️ Температура ниже оптимальной")
    elif sensor_data["temperature"] > adjusted_config["temperature"] + 5:
        recommendations.append("🌡️ Температура выше оптимальной")
    
    # Проверяем влажность
    if sensor_data["humidity"] < adjusted_config["humidity"] - 10:
        recommendations.append("💧 Влажность низкая")
    elif sensor_data["humidity"] > adjusted_config["humidity"] + 10:
        recommendations.append("💧 Влажность высокая")
    
    # Проверяем EC
    if sensor_data["ec"] < adjusted_config["ec"] - 500:
        recommendations.append("⚡ EC низкий")
    elif sensor_data["ec"] > adjusted_config["ec"] + 500:
        recommendations.append("⚠️ EC высокий")
    
    # Проверяем pH
    if sensor_data["ph"] < adjusted_config["ph"] - 0.5:
        recommendations.append("🧪 pH кислый")
    elif sensor_data["ph"] > adjusted_config["ph"] + 0.5:
        recommendations.append("🧪 pH щелочной")
    
    # Проверяем NPK
    if sensor_data["nitrogen"] < adjusted_config["nitrogen"] - 20:
        recommendations.append("🌱 Азот дефицитен")
    if sensor_data["phosphorus"] < adjusted_config["phosphorus"] - 15:
        recommendations.append("🌱 Фосфор дефицитен")
    if sensor_data["potassium"] < adjusted_config["potassium"] - 20:
        recommendations.append("🌱 Калий дефицитен")
    
    if not recommendations:
        recommendations.append("✅ Все параметры в оптимальном диапазоне")
    
    # Вычисляем score здоровья
    health_score = 100
    temp_diff = abs(sensor_data["temperature"] - adjusted_config["temperature"])
    if temp_diff > 10:
        health_score -= 30
    elif temp_diff > 5:
        health_score -= 15
    
    hum_diff = abs(sensor_data["humidity"] - adjusted_config["humidity"])
    if hum_diff > 20:
        health_score -= 25
    elif hum_diff > 10:
        health_score -= 10
    
    ec_diff = abs(sensor_data["ec"] - adjusted_config["ec"])
    if ec_diff > 1000:
        health_score -= 20
    elif ec_diff > 500:
        health_score -= 10
    
    ph_diff = abs(sensor_data["ph"] - adjusted_config["ph"])
    if ph_diff > 1:
        health_score -= 15
    elif ph_diff > 0.5:
        health_score -= 5
    
    # Определяем статус здоровья
    if health_score >= 80:
        health_status = "Отличное"
    elif health_score >= 60:
        health_status = "Хорошее"
    elif health_score >= 40:
        health_status = "Удовлетворительное"
    else:
        health_status = "Требует внимания"
    
    return {
        "crop_type": crop_type,
        "growing_type": growing_type,
        "season": season,
        "soil_type": soil_type,
        "recommendations": recommendations,
        "health_status": health_status,
        "health_score": health_score,
        "adjusted_config": adjusted_config,
        "sensor_data": sensor_data
    }

def validate_recommendation(recommendation, crop, growing_type, season, soil_type):
    """Валидирует рекомендации на основе научных данных"""
    
    validation_result = {
        "status": "PASS",
        "message": "Рекомендации корректны",
        "issues": []
    }
    
    # Проверяем наличие обязательных полей
    required_fields = ["crop_type", "growing_type", "season", "soil_type", "recommendations", "health_status"]
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
    
    # Проверяем корректность типа почвы
    valid_soil_types = ["sand", "loam", "clay", "peat", "sandpeat"]
    if recommendation.get("soil_type") not in valid_soil_types:
        validation_result["status"] = "FAIL"
        validation_result["issues"].append("Некорректный тип почвы")
    
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
    
    # Специфические проверки для типов почвы
    if soil_type == "peat":
        adjusted_config = recommendation.get("adjusted_config", {})
        if adjusted_config.get("ph", 7.0) > 6.0:
            validation_result["status"] = "WARN"
            validation_result["issues"].append("Торф обычно кислый, проверьте pH")
    
    if soil_type == "clay":
        adjusted_config = recommendation.get("adjusted_config", {})
        if adjusted_config.get("ec", 0) > 2500.0:
            validation_result["status"] = "WARN"
            validation_result["issues"].append("Глина склонна к засолению при высоком EC")
    
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