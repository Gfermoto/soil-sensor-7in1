#!/usr/bin/env python3
"""
Автоматический скрипт цикла релиза JXCT
Выполняет все необходимые шаги для создания нового релиза:
1. Проверяет статус git
2. Повышает версию
3. Собирает проект  
4. Создает коммит, тег и отправляет на GitHub
5. Создает GitHub Release с описанием
"""

import os
import sys
import subprocess
import json
import re
from datetime import datetime

def run_command(cmd, check=True, capture_output=True):
    """Выполняет команду и возвращает результат"""
    print(f"🔧 Выполняю: {cmd}")
    try:
        result = subprocess.run(cmd, shell=True, capture_output=capture_output, text=True, check=check, encoding='utf-8', errors='ignore')
        if result and result.stdout and result.stdout.strip():
            print(f"✅ {result.stdout.strip()}")
        return result
    except subprocess.CalledProcessError as e:
        print(f"❌ Ошибка: {e}")
        if e.stderr:
            print(f"❌ {e.stderr}")
        return None

def check_git_status():
    """Проверяет статус git репозитория"""
    print("🔍 Проверяю статус git...")
    
    # Проверяем, что мы в git репозитории
    result = run_command("git status", check=False)
    if not result or result.returncode != 0:
        print("❌ Не git репозиторий или ошибка git")
        return False
    
    # Проверяем, что нет несохраненных изменений
    result = run_command("git diff --quiet HEAD", check=False)
    if result and result.returncode != 0:
        print("⚠️  Есть несохраненные изменения в рабочей директории")
        response = input("🤔 Продолжить? (y/N): ")
        if response.lower() != 'y':
            return False
    
    # Проверяем, что мы на ветке main
    result = run_command("git branch --show-current", capture_output=True)
    if result and result.stdout.strip() != "main":
        print(f"⚠️  Текущая ветка: {result.stdout.strip()}")
        response = input("🤔 Продолжить на этой ветке? (y/N): ")
        if response.lower() != 'y':
            return False
    
    print("✅ Git статус проверен")
    return True

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
    # Сборка production-окружения, чтобы раннее обнаружить ошибки, которые проявляются
    # только при строгих флагах оптимизации/NO_ANSI_COLORS и т.п.
    prod_env = "esp32dev-production"
    result = run_command(f"pio run -e {prod_env}")
    if result and result.returncode == 0:
        print("✅ Проект собран успешно")
        return True
    else:
        print("❌ Ошибка сборки проекта")
        return False

def get_changelog_entries(version):
    """Получает записи CHANGELOG для версии"""
    try:
        with open('CHANGELOG.md', 'r', encoding='utf-8') as f:
            content = f.read()
        
        # Ищем секцию для данной версии
        pattern = rf"## \[{re.escape(version)}\]"
        match = re.search(pattern, content)
        if match:
            # Извлекаем содержимое до следующей версии
            start = match.end()
            next_version_match = re.search(r"## \[[\d.]+]", content[start:])
            if next_version_match:
                end = start + next_version_match.start()
            else:
                end = len(content)
            
            changelog = content[start:end].strip()
            return changelog
        else:
            return f"Release v{version}\n\n- Automatic version bump\n- Project built successfully"
    except FileNotFoundError:
        return f"Release v{version}\n\n- Automatic version bump\n- Project built successfully"

def create_github_release(version):
    """Создает GitHub Release с описанием"""
    print(f"📦 Создаю GitHub Release v{version}")
    
    changelog = get_changelog_entries(version)
    
    # Создаем временный файл с описанием релиза
    release_body = f"""# JXCT Soil Sensor 7-in-1 v{version}

## 📋 Описание
Автоматический релиз прошивки для датчика почвы JXCT 7-в-1.

## 🔧 Изменения
{changelog}

## 📦 Файлы
- `firmware.bin` - Прошивка для ESP32
- `manifest.json` - Манифест для OTA обновлений

## 🚀 Установка
1. Скачайте `firmware.bin`
2. Загрузите через PlatformIO или esptool
3. Или используйте OTA обновление через веб-интерфейс

---
*Создано автоматически: {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}*
"""
    
    with open('release_body.md', 'w', encoding='utf-8') as f:
        f.write(release_body)
    
    # Создаем GitHub Release
    cmd = f'gh release create v{version} .pio/build/esp32dev-production/firmware.bin --title "JXCT v{version}" --notes-file release_body.md'
    result = run_command(cmd, check=False)
    
    # Удаляем временный файл
    os.remove('release_body.md')
    
    if result and result.returncode == 0:
        print(f"✅ GitHub Release v{version} создан")
        return True
    else:
        print("⚠️  GitHub Release не создан (возможно, gh CLI не установлен)")
        return True  # Не критично

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
    auto_mode = False
    
    if len(sys.argv) > 1:
        if sys.argv[1] in ['--help', '-h']:
            print("Использование: python release_cycle.py [major|minor|patch] [--auto]")
            print("")
            print("Аргументы:")
            print("  major    - повышение major версии (3.2.7 -> 4.0.0)")
            print("  minor    - повышение minor версии (3.2.7 -> 3.3.0)")
            print("  patch    - повышение patch версии (3.2.7 -> 3.2.8) [по умолчанию]")
            print("  --auto   - без интерактивного подтверждения")
            print("")
            print("Примеры:")
            print("  python release_cycle.py patch --auto")
            print("  python release_cycle.py minor")
            print("  python release_cycle.py major")
            sys.exit(0)
        
        for arg in sys.argv[1:]:
            if arg == '--auto':
                auto_mode = True
            elif arg in ['major', 'minor', 'patch']:
                increment_type = arg
            else:
                print(f"❌ Неизвестный аргумент: {arg}")
                print("💡 Для справки: python release_cycle.py --help")
                sys.exit(1)
    
    # Шаг 0: Проверяем git статус
    if not check_git_status():
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
    if not auto_mode:
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
    
    # Шаг 4: Создаем GitHub Release
    create_github_release(new_version)
    
    print("=" * 50)
    print(f"🎉 РЕЛИЗ v{new_version} СОЗДАН УСПЕШНО!")
    print(f"🔗 GitHub: https://github.com/Gfermoto/soil-sensor-7in1/releases/tag/v{new_version}")
    print("=" * 50)
    print("💡 Следующие шаги:")
    print("   1. Проверьте релиз на GitHub")
    print("   2. Обновите документацию при необходимости")
    print("   3. Уведомите пользователей о новом релизе")

if __name__ == "__main__":
    main() 