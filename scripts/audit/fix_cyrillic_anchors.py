#!/usr/bin/env python3
"""
Скрипт для исправления кириллических якорей в MD файлах
и создания рабочих оглавлений
"""

import os
import re
import glob
from pathlib import Path

def transliterate_anchor(text):
    """Транслитерация кириллицы в латиницу для якорей"""
    cyrillic_map = {
        'а': 'a', 'б': 'b', 'в': 'v', 'г': 'g', 'д': 'd', 'е': 'e', 'ё': 'yo',
        'ж': 'zh', 'з': 'z', 'и': 'i', 'й': 'y', 'к': 'k', 'л': 'l', 'м': 'm',
        'н': 'n', 'о': 'o', 'п': 'p', 'р': 'r', 'с': 's', 'т': 't', 'у': 'u',
        'ф': 'f', 'х': 'h', 'ц': 'ts', 'ч': 'ch', 'ш': 'sh', 'щ': 'shch',
        'ъ': '', 'ы': 'y', 'ь': '', 'э': 'e', 'ю': 'yu', 'я': 'ya',
        'А': 'A', 'Б': 'B', 'В': 'V', 'Г': 'G', 'Д': 'D', 'Е': 'E', 'Ё': 'Yo',
        'Ж': 'Zh', 'З': 'Z', 'И': 'I', 'Й': 'Y', 'К': 'K', 'Л': 'L', 'М': 'M',
        'Н': 'N', 'О': 'O', 'П': 'P', 'Р': 'R', 'С': 'S', 'Т': 'T', 'У': 'U',
        'Ф': 'F', 'Х': 'H', 'Ц': 'Ts', 'Ч': 'Ch', 'Ш': 'Sh', 'Щ': 'Shch',
        'Ъ': '', 'Ы': 'Y', 'Ь': '', 'Э': 'E', 'Ю': 'Yu', 'Я': 'Ya',
        ' ': '-', '(': '', ')': '', '[': '', ']': '', '{': '', '}': '',
        '!': '', '?': '', '.': '', ',': '', ':': '', ';': '', '"': '',
        "'": '', '`': '', '№': 'no', '«': '', '»': '', '—': '-', '–': '-'
    }

    result = ''
    for char in text:
        if char in cyrillic_map:
            result += cyrillic_map[char]
        elif char.isalnum() or char in '-_':
            result += char.lower()
        else:
            result += '-'

    # Убираем множественные дефисы и дефисы в начале/конце
    result = re.sub(r'-+', '-', result)
    result = result.strip('-')

    return result

def create_toc_from_headers(content):
    """Создание оглавления из заголовков"""
    headers = re.findall(r'^(#{2,6})\s+(.+?)(?:\s+\{#(.+?)\})?$', content, re.MULTILINE)

    if not headers:
        return None

    toc_lines = ["## 📋 Содержание\n"]

    for level_str, title, existing_anchor in headers:
        level = len(level_str)
        # Убираем эмодзи из заголовка для оглавления
        clean_title = re.sub(r'[^\w\s\-().,!?]', '', title).strip()

        # Создаем якорь
        if existing_anchor:
            anchor = existing_anchor
        else:
            anchor = transliterate_anchor(clean_title)

        # Создаем отступ
        indent = "  " * (level - 2)

        toc_lines.append(f"{indent}- [{clean_title}](#{anchor})")

    toc_lines.append("\n---\n")
    return "\n".join(toc_lines)

def fix_file_anchors(file_path):
    """Исправление якорей в одном файле"""
    try:
        with open(file_path, 'r', encoding='utf-8') as f:
            content = f.read()

        original_content = content

        # Исправляем кириллические якоря
        def fix_anchor(match):
            level = match.group(1)
            title = match.group(2)
            old_anchor = match.group(3)

            if old_anchor and re.search(r'[а-яё]', old_anchor, re.IGNORECASE):
                new_anchor = transliterate_anchor(old_anchor)
                return f"{level} {title} {{#{new_anchor}}}"
            elif not old_anchor and level in ['##', '###', '####']:
                # Создаем якорь только для важных заголовков
                clean_title = re.sub(r'[^\w\s\-().,!?]', '', title).strip()
                new_anchor = transliterate_anchor(clean_title)
                return f"{level} {title} {{#{new_anchor}}}"
            else:
                return match.group(0)

        content = re.sub(r'^(#{2,6})\s+(.+?)(?:\s+\{#(.+?)\})?$', fix_anchor, content, flags=re.MULTILINE)

        # НЕ добавляем автоматические оглавления - они создают дубли
        # Только исправляем существующие якоря

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
    print("🔧 Исправление якорей и создание оглавлений")
    print("=" * 50)

    # Найти все MD файлы
    md_files = []
    for pattern in ['docs/**/*.md', 'docs/*.md']:
        md_files.extend(glob.glob(pattern, recursive=True))

    print(f"Найдено {len(md_files)} MD файлов")

    fixed_count = 0

    for file_path in md_files:
        rel_path = os.path.relpath(file_path)
        print(f"Обрабатываю: {rel_path}")

        if fix_file_anchors(file_path):
            print(f"  ✅ Исправлено")
            fixed_count += 1
        else:
            print(f"  ✅ Без изменений")

    print(f"\n🎉 Обработка завершена!")
    print(f"Исправлено файлов: {fixed_count}")

if __name__ == "__main__":
    main()
