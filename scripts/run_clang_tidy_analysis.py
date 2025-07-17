#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Minimalistic clang-tidy analyzer for JXCT
"""

import sys
import os
import subprocess
from pathlib import Path
from datetime import datetime

SRC_DIR = 'src'
REPORT_PATH = 'test_reports/clang_tidy_analysis_report.md'
CLANG_TIDY = r'C:/Program Files/LLVM/bin/clang-tidy.exe'


def find_cpp_files():
    files = []
    for root, dirs, filelist in os.walk(SRC_DIR):
        for file in filelist:
            if file.endswith('.cpp'):
                files.append(os.path.join(root, file).replace('\\', '/'))
    return files


def run_clang_tidy(file_path):
    try:
        result = subprocess.run([
            CLANG_TIDY, file_path, '--', '-std=c++17'
        ], capture_output=True, text=True, timeout=60)
        return result.stdout, result.stderr
    except Exception as e:
        return '', str(e)


def main():
    cpp_files = find_cpp_files()
    total_warnings = 0
    report_lines = [
        f'# Clang-tidy analysis JXCT',
        f'Date: {datetime.now().strftime("%d.%m.%Y %H:%M:%S")}',
        f'\nFiles checked: {len(cpp_files)}\n'
    ]

    print(f'Checking files: {len(cpp_files)}')
    for file_path in cpp_files:
        print(f'\n=== ANALYSIS {file_path} ===')
        stdout, stderr = run_clang_tidy(file_path)
        warnings = [line for line in stdout.split('\n') if 'warning:' in line]
        total_warnings += len(warnings)
        report_lines.append(f'\n## {file_path}')
        if warnings:
            for w in warnings:
                print(f'[WARNING] {w}')
                report_lines.append(f'- {w}')
        elif stderr:
            print(f'[ERROR] {stderr}')
            report_lines.append(f'[ERROR] {stderr}')
        else:
            print('[OK] No warnings')
            report_lines.append('[OK] No warnings')

    report_lines.append(f'\nTotal warnings: {total_warnings}')

    # Save report
    os.makedirs(os.path.dirname(REPORT_PATH), exist_ok=True)
    with open(REPORT_PATH, 'w', encoding='utf-8') as f:
        f.write('\n'.join(report_lines))

    print(f'\nAnalysis completed. Total warnings: {total_warnings}')
    print(f'Report: {REPORT_PATH}')

if __name__ == '__main__':
    main() 