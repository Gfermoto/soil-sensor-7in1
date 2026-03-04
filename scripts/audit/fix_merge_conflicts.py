#!/usr/bin/env python3
"""
Скрипт для автоматического исправления конфликтов мержа в проекте JXCT
"""

import os
import re
import sys
from pathlib import Path

def fix_conflicts_in_file(file_path):
    """Исправляет конфликты мержа в одном файле"""
    print(f"Исправляю конфликты в: {file_path}")
    
    try:
        with open(file_path, 'r', encoding='utf-8') as f:
            content = f.read()
    except Exception as e:
        print(f"Ошибка чтения файла {file_path}: {e}")
        return False
    
    # Паттерны для поиска конфликтов
    conflict_patterns = [
        (r'<<<<<<< HEAD\s*\n(.*?)\n=======\s*\n(.*?)\n>>>>>>> develop\s*\n', r'\2'),
        (r'<<<<<<< HEAD\s*\n(.*?)\n=======\s*\n(.*?)\n>>>>>>> develop', r'\2'),
        (r'<<<<<<< HEAD\s*\n(.*?)\n=======\s*\n(.*?)\n>>>>>>> develop\s*', r'\2'),
    ]
    
    original_content = content
    for pattern, replacement in conflict_patterns:
        content = re.sub(pattern, replacement, content, flags=re.DOTALL)
    
    if content != original_content:
        try:
            with open(file_path, 'w', encoding='utf-8') as f:
                f.write(content)
            print(f"✅ Исправлены конфликты в: {file_path}")
            return True
        except Exception as e:
            print(f"Ошибка записи файла {file_path}: {e}")
            return False
    else:
        print(f"ℹ️  Конфликтов не найдено в: {file_path}")
        return True

def find_files_with_conflicts():
    """Находит все файлы с конфликтами мержа"""
    project_root = Path.cwd()
    files_with_conflicts = []
    
    # Расширения файлов для проверки
    extensions = ['.cpp', '.h', '.hpp', '.c', '.md', '.json', '.yml', '.yaml']
    
    for ext in extensions:
        for file_path in project_root.rglob(f'*{ext}'):
            if file_path.is_file():
                try:
                    with open(file_path, 'r', encoding='utf-8') as f:
                        content = f.read()
                        if '<<<<<<< HEAD' in content:
                            files_with_conflicts.append(file_path)
                except Exception:
                    continue
    
    return files_with_conflicts

def main():
    """Основная функция"""
    print("🔧 Исправление конфликтов мержа в проекте JXCT")
    print("=" * 50)
    
    # Находим файлы с конфликтами
    files_with_conflicts = find_files_with_conflicts()
    
    if not files_with_conflicts:
        print("✅ Конфликтов мержа не найдено!")
        return 0
    
    print(f"Найдено файлов с конфликтами: {len(files_with_conflicts)}")
    
    # Исправляем конфликты
    fixed_count = 0
    for file_path in files_with_conflicts:
        if fix_conflicts_in_file(file_path):
            fixed_count += 1
    
    print("=" * 50)
    print(f"✅ Исправлено файлов: {fixed_count}/{len(files_with_conflicts)}")
    
    if fixed_count == len(files_with_conflicts):
        print("🎉 Все конфликты успешно исправлены!")
        return 0
    else:
        print("⚠️  Некоторые конфликты не удалось исправить автоматически")
        return 1

if __name__ == "__main__":
    sys.exit(main()) 