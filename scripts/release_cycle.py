#!/usr/bin/env python3
"""
Автоматический скрипт цикла релиза JXCT
Выполняет все необходимые шаги для создания нового релиза:
1. Повышает версию
2. Собирает проект  
3. Создает коммит, тег и отправляет на GitHub
"""

import os
import sys
import subprocess
import json
import re
from datetime import datetime

def run_command(cmd, check=True):
    """Выполняет команду и возвращает результат"""
    print(f"🔧 Выполняю: {cmd}")
    try:
        result = subprocess.run(cmd, shell=True, capture_output=True, text=True, check=check, encoding='utf-8', errors='ignore')
        if result and result.stdout and result.stdout.strip():
            print(f"✅ {result.stdout.strip()}")
        return result
    except subprocess.CalledProcessError as e:
        print(f"❌ Ошибка: {e}")
        if e.stderr:
            print(f"❌ {e.stderr}")
        return None

def get_current_version():
    """Читает текущую версию из файла VERSION"""
    try:
        with open('VERSION', 'r') as f:
            return f.read().strip()
    except FileNotFoundError:
        print("❌ Файл VERSION не найден!")
        return None

def increment_version(version, increment_type='patch'):
    """Повышает версию согласно семантическому версионированию"""
    parts = version.split('.')
    if len(parts) != 3:
        print(f"❌ Неверный формат версии: {version}")
        return None
    
    major, minor, patch = map(int, parts)
    
    if increment_type == 'major':
        major += 1
        minor = 0
        patch = 0
    elif increment_type == 'minor':
        minor += 1
        patch = 0
    else:  # patch
        patch += 1
    
    return f"{major}.{minor}.{patch}"

def update_version_files(new_version):
    """Обновляет версию во всех необходимых файлах"""
    print(f"📝 Обновляю версию до {new_version}")
    
    # Обновляем VERSION
    with open('VERSION', 'w') as f:
        f.write(new_version)
    print("✅ VERSION обновлен")
    
    # Обновляем manifest.json
    try:
        with open('manifest.json', 'r') as f:
            manifest = json.load(f)
        
        manifest['version'] = new_version
        manifest['url'] = f"https://github.com/Gfermoto/soil-sensor-7in1/releases/download/v{new_version}/firmware.bin"
        
        with open('manifest.json', 'w') as f:
            json.dump(manifest, f, indent=2)
        print("✅ manifest.json обновлен")
        
    except Exception as e:
        print(f"❌ Ошибка обновления manifest.json: {e}")
        return False
    
    return True

def build_project():
    """Собирает проект с PlatformIO"""
    print("🔨 Собираю проект...")
    result = run_command("pio run")
    if result and result.returncode == 0:
        print("✅ Проект собран успешно")
        return True
    else:
        print("❌ Ошибка сборки проекта")
        return False

def create_git_release(version):
    """Создает Git коммит, тег и отправляет на GitHub"""
    print(f"📦 Создаю Git релиз v{version}")
    
    # Добавляем все изменения
    result = run_command("git add .")
    if not result:
        return False
    
    # Создаем коммит
    commit_msg = f"""RELEASE v{version}: Automatic version bump

- VERSION: Updated to {version}
- BUILD: Project built successfully  
- FILES: VERSION and manifest.json updated
- AUTO: Created by release_cycle.py script

Date: {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}
Version: {version}"""
    
    result = run_command(f'git commit -m "{commit_msg}"')
    if not result:
        print("❌ Ошибка создания коммита")
        return False
    
    # Создаем тег
    result = run_command(f"git tag v{version}")
    if not result:
        print("❌ Ошибка создания тега")
        return False
    
    # Отправляем на GitHub
    result = run_command("git push origin main --tags")
    if not result:
        print("❌ Ошибка отправки на GitHub")
        return False
    
    print(f"✅ Релиз v{version} создан и отправлен на GitHub")
    return True

def main():
    """Главная функция скрипта"""
    print("🚀 АВТОМАТИЧЕСКИЙ ЦИКЛ РЕЛИЗА JXCT")
    print("=" * 50)
    
    # Проверяем аргументы
    increment_type = 'patch'
    if len(sys.argv) > 1:
        if sys.argv[1] in ['--help', '-h']:
            print("Использование: python release_cycle.py [major|minor|patch] [--auto]")
            print("")
            print("Аргументы:")
            print("  major    - повышение major версии (3.2.7 -> 4.0.0)")
            print("  minor    - повышение minor версии (3.2.7 -> 3.3.0)")
            print("  patch    - повышение patch версии (3.2.7 -> 3.2.8) [по умолчанию]")
            print("  --auto   - без интерактивного подтверждения")
            sys.exit(0)
        
        increment_type = sys.argv[1].lower()
        if increment_type not in ['major', 'minor', 'patch']:
            print("❌ Неверный тип инкремента. Используйте: major, minor, patch")
            print("💡 Для справки: python release_cycle.py --help")
            sys.exit(1)
    
    # Получаем текущую версию
    current_version = get_current_version()
    if not current_version:
        sys.exit(1)
    
    print(f"📋 Текущая версия: {current_version}")
    
    # Повышаем версию
    new_version = increment_version(current_version, increment_type)
    if not new_version:
        sys.exit(1)
    
    print(f"📈 Новая версия: {new_version}")
    
    # Подтверждение
    if len(sys.argv) <= 2 or sys.argv[-1] != '--auto':
        response = input(f"🤔 Продолжить создание релиза v{new_version}? (y/N): ")
        if response.lower() != 'y':
            print("❌ Отменено пользователем")
            sys.exit(0)
    
    print("🎯 НАЧИНАЮ АВТОМАТИЧЕСКИЙ ЦИКЛ РЕЛИЗА")
    print("-" * 40)
    
    # Шаг 1: Обновляем версии в файлах
    if not update_version_files(new_version):
        sys.exit(1)
    
    # Шаг 2: Собираем проект
    if not build_project():
        sys.exit(1)
    
    # Шаг 3: Создаем Git релиз
    if not create_git_release(new_version):
        sys.exit(1)
    
    print("=" * 50)
    print(f"🎉 РЕЛИЗ v{new_version} СОЗДАН УСПЕШНО!")
    print(f"🔗 GitHub: https://github.com/Gfermoto/soil-sensor-7in1/releases/tag/v{new_version}")
    print("=" * 50)

if __name__ == "__main__":
    main() 