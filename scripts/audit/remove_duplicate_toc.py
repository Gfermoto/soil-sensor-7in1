#!/usr/bin/env python3
"""
Скрипт для удаления дублирующихся автоматически созданных оглавлений
"""

import os
import re
import glob

def remove_auto_toc(file_path):
    """Удаляет автоматически созданные оглавления с якорем {#Soderzhanie}"""
    try:
        with open(file_path, 'r', encoding='utf-8') as f:
            content = f.read()

        original_content = content

        # Удаляем автоматически созданные оглавления
        # Ищем блок от "## 📋 Содержание {#Soderzhanie}" до "---"
        pattern = r'## 📋 Содержание \{#Soderzhanie\}.*?^---\s*$'
        content = re.sub(pattern, '', content, flags=re.MULTILINE | re.DOTALL)

        # Убираем лишние пустые строки
        content = re.sub(r'\n{3,}', '\n\n', content)

        if content != original_content:
            with open(file_path, 'w', encoding='utf-8') as f:
                f.write(content)
            return True

        return False

    except Exception as e:
        print(f"Ошибка при обработке {file_path}: {e}")
        return False

def main():
    """Основная функция"""
    print("🗑️ Удаление дублирующихся оглавлений")
    print("=" * 40)

    # Найти все MD файлы
    md_files = []
    for pattern in ['docs/**/*.md', 'docs/*.md']:
        md_files.extend(glob.glob(pattern, recursive=True))

    print(f"Найдено {len(md_files)} MD файлов")

    cleaned_count = 0

    for file_path in md_files:
        rel_path = os.path.relpath(file_path)
        print(f"Обрабатываю: {rel_path}")

        if remove_auto_toc(file_path):
            print(f"  ✅ Очищено")
            cleaned_count += 1
        else:
            print(f"  ✅ Без изменений")

    print(f"\n🎉 Очистка завершена!")
    print(f"Очищено файлов: {cleaned_count}")

if __name__ == "__main__":
    main()
