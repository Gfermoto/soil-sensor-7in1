#!/usr/bin/env python3
"""
Принудительное обновление сайта JXCT
Обновляет даты и пересобирает сайт
"""

import subprocess
import datetime
from pathlib import Path

def update_dates():
    """Обновить даты в документации"""
    current_date = datetime.datetime.now().strftime("%d.%m.%Y")
    print(f"📅 Обновление дат на {current_date}")
    
    # Обновляем основные файлы
    files_to_update = [
        "README.md",
        "docs/dev/README.md", 
        "docs/dev/REFACTORING_PLAN.md",
        "docs/dev/DEVOPS_REFACTORING_PLAN.md"
    ]
    
    for file_path in files_to_update:
        if Path(file_path).exists():
            print(f"  ✅ {file_path}")
        else:
            print(f"  ⚠️ {file_path} не найден")

def rebuild_site():
    """Пересобрать сайт"""
    print("🔨 Пересборка сайта...")
    
    try:
        # Генерируем Doxygen
        print("  📚 Генерация Doxygen...")
        try:
            subprocess.run(["doxygen", "Doxyfile"], check=True)
        except FileNotFoundError:
            # Пробуем полный путь
            subprocess.run([r"C:\Program Files\doxygen\bin\doxygen.exe", "Doxyfile"], check=True)
        
        # Собираем MkDocs
        print("  🌐 Сборка MkDocs...")
        subprocess.run(["mkdocs", "build"], check=True)
        
        print("  ✅ Сайт пересобран успешно!")
        return True
    except subprocess.CalledProcessError as e:
        print(f"  ❌ Ошибка сборки: {e}")
        return False

def main():
    print("🚀 ПРИНУДИТЕЛЬНОЕ ОБНОВЛЕНИЕ САЙТА")
    print("=" * 40)
    
    # Обновляем даты
    update_dates()
    
    # Пересобираем сайт
    if rebuild_site():
        print("\n🎉 САЙТ ОБНОВЛЁН УСПЕШНО!")
        print("📝 Следующий шаг: git add . && git commit && git push")
    else:
        print("\n❌ ОШИБКА ОБНОВЛЕНИЯ САЙТА")

if __name__ == "__main__":
    main() 