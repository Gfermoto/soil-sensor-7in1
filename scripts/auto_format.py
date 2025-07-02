#!/usr/bin/env python3
"""
Автоматическое исправление trailing whitespace и поиск длинных строк
"""
import os
from pathlib import Path

# Расширения текстовых файлов
TEXT_EXTENSIONS = {'.py', '.cpp', '.h', '.md', '.txt', '.json', '.yml', '.yaml'}
# Исключаемые директории
EXCLUDE_DIRS = {'.venv', '.pio', 'node_modules', '__pycache__', '.git', 'site-packages', 'build', 'dist'}
# Максимальная длина строки
MAX_LINE_LENGTH = 120

def is_excluded(path: Path) -> bool:
    return any(exclude in str(path) for exclude in EXCLUDE_DIRS)

def fix_trailing_whitespace(root: Path):
    fixed_files = []
    for file_path in root.rglob('*'):
        if file_path.is_file() and file_path.suffix in TEXT_EXTENSIONS and not is_excluded(file_path):
            try:
                with open(file_path, 'r', encoding='utf-8') as f:
                    lines = f.readlines()
                new_lines = [line.rstrip() + '\n' for line in lines]
                if lines != new_lines:
                    with open(file_path, 'w', encoding='utf-8') as f:
                        f.writelines(new_lines)
                    fixed_files.append(str(file_path))
            except Exception:
                continue
    return fixed_files

def find_long_lines(root: Path):
    long_lines = []
    for file_path in root.rglob('*'):
        if file_path.is_file() and file_path.suffix in TEXT_EXTENSIONS and not is_excluded(file_path):
            try:
                with open(file_path, 'r', encoding='utf-8') as f:
                    for i, line in enumerate(f, 1):
                        if len(line.rstrip()) > MAX_LINE_LENGTH:
                            long_lines.append((str(file_path), i, len(line.rstrip())))
            except Exception:
                continue
    return long_lines

def main():
    root = Path(__file__).parent.parent
    print('[AUTOFORMAT] Removing trailing whitespace...')
    fixed = fix_trailing_whitespace(root)
    print(f'[AUTOFORMAT] Fixed files: {len(fixed)}')
    if fixed:
        for f in fixed:
            print(f'  [FIXED] {f}')
    print('[AUTOFORMAT] Searching for long lines...')
    long_lines = find_long_lines(root)
    if long_lines:
        print(f'[AUTOFORMAT] Found long lines (> {MAX_LINE_LENGTH}):')
        for file_path, line_num, length in long_lines:
            print(f'  [LONG] {file_path}:{line_num} ({length} chars)')
    else:
        print('[AUTOFORMAT] No long lines found.')

if __name__ == '__main__':
    main()
