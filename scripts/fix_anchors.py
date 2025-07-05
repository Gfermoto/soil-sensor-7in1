#!/usr/bin/env python3
"""
Скрипт для исправления неправильных якорей в документации MkDocs.
Автоматически находит все заголовки и исправляет ссылки на них.
"""

import os
import re
import glob
from pathlib import Path

def extract_anchors_from_file(file_path):
    """Извлекает все заголовки и их якоря из файла."""
    anchors = {}
    
    with open(file_path, 'r', encoding='utf-8') as f:
        content = f.read()
    
    # Находим все заголовки (## и ###)
    header_pattern = r'^(#{2,3})\s+(.+)$'
    matches = re.findall(header_pattern, content, re.MULTILINE)
    
    for level, title in matches:
        # Создаем якорь по правилам MkDocs
        anchor = re.sub(r'[^\w\s-]', '', title.lower())
        anchor = re.sub(r'[-\s]+', '-', anchor)
        anchor = anchor.strip('-')
        
        anchors[title] = anchor
    
    return anchors

def fix_anchors_in_file(file_path):
    """Исправляет якоря в файле."""
    print(f"Обрабатываю: {file_path}")
    
    with open(file_path, 'r', encoding='utf-8') as f:
        content = f.read()
    
    # Извлекаем реальные якоря
    anchors = extract_anchors_from_file(file_path)
    
    # Находим все ссылки на якоря
    link_pattern = r'\[([^\]]+)\]\(#([^)]+)\)'
    
    def replace_link(match):
        link_text = match.group(1)
        old_anchor = match.group(2)
        
        # Ищем соответствующий заголовок
        for title, anchor in anchors.items():
            if anchor == old_anchor or title.lower().replace(' ', '-').replace('️', '') == old_anchor:
                return f'[{link_text}](#{anchor})'
        
        # Если не нашли, оставляем как есть
        return match.group(0)
    
    # Заменяем ссылки
    new_content = re.sub(link_pattern, replace_link, content)
    
    # Записываем обратно
    if new_content != content:
        with open(file_path, 'w', encoding='utf-8') as f:
            f.write(new_content)
        print(f"  ✅ Исправлено")
    else:
        print(f"  ✅ Без изменений")

def main():
    """Основная функция."""
    docs_dir = Path("docs")
    
    # Находим все MD файлы
    md_files = []
    md_files.extend(docs_dir.glob("*.md"))
    md_files.extend(docs_dir.glob("manuals/*.md"))
    md_files.extend(docs_dir.glob("dev/*.md"))
    
    print(f"Найдено {len(md_files)} MD файлов")
    
    # Обрабатываем каждый файл
    for file_path in md_files:
        fix_anchors_in_file(file_path)
    
    print("\n🎉 Исправление якорей завершено!")

if __name__ == "__main__":
    main() 