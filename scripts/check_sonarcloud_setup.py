#!/usr/bin/env python3
"""
Скрипт для проверки настроек SonarCloud в проекте JXCT
Проверяет наличие всех необходимых файлов и настроек
"""

import os
import sys
import subprocess
import yaml
from pathlib import Path

def check_file_exists(file_path, description):
    """Проверяет существование файла"""
    if os.path.exists(file_path):
        print(f"✅ {description}: {file_path}")
        return True
    else:
        print(f"❌ {description}: {file_path} - НЕ НАЙДЕН")
        return False

def check_yaml_syntax(file_path):
    """Проверяет синтаксис YAML файла"""
    try:
        with open(file_path, 'r', encoding='utf-8') as f:
            yaml.safe_load(f)
        print(f"✅ Синтаксис YAML корректен: {file_path}")
        return True
    except yaml.YAMLError as e:
        print(f"❌ Ошибка YAML в {file_path}: {e}")
        return False
    except Exception as e:
        print(f"❌ Ошибка чтения {file_path}: {e}")
        return False

def check_sonar_project_properties():
    """Проверяет конфигурацию sonar-project.properties"""
    file_path = "sonar-project.properties"
    if not check_file_exists(file_path, "Конфигурация SonarCloud"):
        return False
    
    try:
        with open(file_path, 'r', encoding='utf-8') as f:
            content = f.read()
        
        required_keys = [
            "sonar.projectKey",
            "sonar.organization", 
            "sonar.projectName",
            "sonar.sources",
            "sonar.tests"
        ]
        
        missing_keys = []
        for key in required_keys:
            if key not in content:
                missing_keys.append(key)
        
        if missing_keys:
            print(f"❌ Отсутствуют обязательные ключи: {', '.join(missing_keys)}")
            return False
        else:
            print("✅ Все обязательные ключи присутствуют")
            return True
            
    except Exception as e:
        print(f"❌ Ошибка проверки {file_path}: {e}")
        return False

def check_github_secrets():
    """Проверяет наличие GitHub Secrets"""
    print("\n🔐 ПРОВЕРКА GITHUB SECRETS:")
    print("Убедитесь, что в настройках репозитория добавлены:")
    print("✅ SONAR_TOKEN - токен SonarCloud")
    print("✅ GITHUB_TOKEN - автоматически доступен")
    
    # Проверяем, есть ли workflow файл
    workflow_file = ".github/workflows/sonarcloud.yml"
    if os.path.exists(workflow_file):
        print(f"✅ Workflow файл найден: {workflow_file}")
        
        with open(workflow_file, 'r', encoding='utf-8') as f:
            content = f.read()
        
        if "SONAR_TOKEN" in content:
            print("✅ SONAR_TOKEN используется в workflow")
        else:
            print("❌ SONAR_TOKEN не найден в workflow")
            
        if "GITHUB_TOKEN" in content:
            print("✅ GITHUB_TOKEN используется в workflow")
        else:
            print("❌ GITHUB_TOKEN не найден в workflow")
    else:
        print(f"❌ Workflow файл не найден: {workflow_file}")

def check_project_structure():
    """Проверяет структуру проекта для SonarCloud"""
    print("\n📁 ПРОВЕРКА СТРУКТУРЫ ПРОЕКТА:")
    
    required_dirs = ["src", "include", "test"]
    for dir_name in required_dirs:
        if os.path.exists(dir_name):
            print(f"✅ Директория найдена: {dir_name}")
        else:
            print(f"❌ Директория не найдена: {dir_name}")
    
    # Проверяем наличие исходных файлов
    cpp_files = list(Path("src").rglob("*.cpp")) + list(Path("include").rglob("*.h"))
    if cpp_files:
        print(f"✅ Найдено {len(cpp_files)} C++ файлов")
    else:
        print("❌ C++ файлы не найдены")
    
    # Проверяем наличие тестов
    test_files = list(Path("test").rglob("*.py")) + list(Path("test").rglob("*.cpp"))
    if test_files:
        print(f"✅ Найдено {len(test_files)} тестовых файлов")
    else:
        print("❌ Тестовые файлы не найдены")

def check_coverage_setup():
    """Проверяет настройки покрытия кода"""
    print("\n📊 ПРОВЕРКА НАСТРОЕК ПОКРЫТИЯ:")
    
    # Проверяем requirements.txt
    if os.path.exists("requirements.txt"):
        with open("requirements.txt", 'r', encoding='utf-8') as f:
            content = f.read()
        
        coverage_tools = ["pytest", "pytest-cov", "coverage"]
        for tool in coverage_tools:
            if tool in content:
                print(f"✅ {tool} найден в requirements.txt")
            else:
                print(f"⚠️ {tool} не найден в requirements.txt")
    else:
        print("❌ requirements.txt не найден")

def run_test_analysis():
    """Запускает тестовый анализ"""
    print("\n🧪 ТЕСТОВЫЙ АНАЛИЗ:")
    
    try:
        # Проверяем Python тесты
        result = subprocess.run([
            "python", "-m", "pytest", "test/", "--collect-only", "-q"
        ], capture_output=True, text=True, timeout=30)
        
        if result.returncode == 0:
            print("✅ Python тесты готовы к запуску")
        else:
            print(f"❌ Ошибка Python тестов: {result.stderr}")
            
    except subprocess.TimeoutExpired:
        print("⚠️ Тест Python тестов превысил таймаут")
    except Exception as e:
        print(f"❌ Ошибка запуска Python тестов: {e}")
    
    try:
        # Проверяем PlatformIO
        result = subprocess.run([
            "pio", "--version"
        ], capture_output=True, text=True, timeout=10)
        
        if result.returncode == 0:
            print("✅ PlatformIO доступен")
        else:
            print(f"❌ PlatformIO недоступен: {result.stderr}")
            
    except subprocess.TimeoutExpired:
        print("⚠️ Проверка PlatformIO превысила таймаут")
    except Exception as e:
        print(f"❌ Ошибка проверки PlatformIO: {e}")

def main():
    """Основная функция проверки"""
    print("🔍 ПРОВЕРКА НАСТРОЕК SONARCLOUD ДЛЯ JXCT")
    print("=" * 50)
    
    checks_passed = 0
    total_checks = 0
    
    # Проверка файлов конфигурации
    print("\n📋 ПРОВЕРКА ФАЙЛОВ КОНФИГУРАЦИИ:")
    
    total_checks += 1
    if check_file_exists("sonar-project.properties", "Конфигурация SonarCloud"):
        checks_passed += 1
    
    total_checks += 1
    if check_sonar_project_properties():
        checks_passed += 1
    
    total_checks += 1
    if check_yaml_syntax(".github/workflows/sonarcloud.yml"):
        checks_passed += 1
    
    # Проверка структуры проекта
    check_project_structure()
    
    # Проверка настроек покрытия
    check_coverage_setup()
    
    # Проверка GitHub Secrets
    check_github_secrets()
    
    # Тестовый анализ
    run_test_analysis()
    
    # Итоги
    print("\n" + "=" * 50)
    print(f"📊 ИТОГИ ПРОВЕРКИ: {checks_passed}/{total_checks} пройдено")
    
    if checks_passed == total_checks:
        print("🎉 ВСЕ ПРОВЕРКИ ПРОЙДЕНЫ! SonarCloud готов к работе!")
        print("\n🚀 СЛЕДУЮЩИЕ ШАГИ:")
        print("1. Добавьте SONAR_TOKEN в GitHub Secrets")
        print("2. Сделайте push в main/refactoring/develop")
        print("3. Проверьте результаты на sonarcloud.io")
        return 0
    else:
        print("⚠️ НЕКОТОРЫЕ ПРОВЕРКИ НЕ ПРОЙДЕНЫ!")
        print("\n🔧 ТРЕБУЕТСЯ ДОРАБОТКА:")
        print("1. Исправьте найденные проблемы")
        print("2. Запустите проверку снова")
        return 1

if __name__ == "__main__":
    sys.exit(main()) 