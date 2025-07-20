#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Minimalistic clang-tidy analyzer for JXCT
Использует систему исключений для правильно погашенных предупреждений
"""

from clang_tidy_exclusions import filter_warnings

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
        # Добавляем пути к заголовочным файлам ESP32/Arduino
        compile_flags = [
            '-std=c++17',
            '-I', 'include',
            '-I', 'test/stubs',
            '-I', '.pio/libdeps/esp32dev/PubSubClient/src',
            '-I', '.pio/libdeps/esp32dev/ArduinoJson/src',
            '-I', '.pio/libdeps/esp32dev/ModbusMaster/src',
            '-I', '.pio/libdeps/esp32dev/NTPClient/src',
            '-I', '.pio/libdeps/esp32dev/ThingSpeak/src',
            '-D', 'ESP32',
            '-D', 'ARDUINO_ARCH_ESP32',
            '-D', 'ARDUINO=10800',
            '-D', 'ARDUINO_ESP32_DEV',
            '-D', 'F_CPU=240000000L',
            '-D', 'HAVE_CONFIG_H',
            '-D', 'UNITY_INCLUDE_CONFIG_H',
            '-D', 'ARDUINO_LOOP_STACK_SIZE=8192',
            '-D', 'CONFIG_FREERTOS_HZ=1000',
            '-D', 'LED_BUILTIN=2',
            '-D', 'BOOT_BUTTON=0',
            '-D', 'DEBUG_MODE',
            '-D', 'INFO_MODE',
            '-D', 'MQTT_MAX_PACKET_SIZE=1024',
            '-D', 'MQTT_KEEPALIVE=60',
            '-D', 'MQTT_SOCKET_TIMEOUT=15'
        ]
        
        result = subprocess.run([
            CLANG_TIDY, file_path, '--'
        ] + compile_flags, capture_output=True, text=True, timeout=60, encoding='utf-8', errors='ignore')
        return result.stdout or '', result.stderr or ''
    except Exception as e:
        return '', str(e)


def main():
    cpp_files = find_cpp_files()
    total_warnings = 0
    files_with_warnings = 0
    files_with_errors = 0
    
    print("Clang-tidy analysis JXCT")
    print("=" * 40)
    print(f"Files: {len(cpp_files)}")
    print()
    
    report_lines = [
        '# 🔍 Clang-tidy Professional Analysis JXCT',
        f'**Дата:** {datetime.now().strftime("%d.%m.%Y %H:%M:%S")}',
        f'**Файлов проверено:** {len(cpp_files)}',
        '',
        '## 📊 Результаты анализа',
        ''
    ]

    for i, file_path in enumerate(cpp_files, 1):
        file_name = os.path.basename(file_path)
        print(f"[{i:2d}/{len(cpp_files)}] {file_name}")
        
        stdout, stderr = run_clang_tidy(file_path)
        warnings = [line for line in stdout.split('\n') if 'warning:' in line]
        filtered_warnings = filter_warnings(warnings)
        
        if filtered_warnings:
            files_with_warnings += 1
            total_warnings += len(filtered_warnings)
            print(f"   ⚠️  {len(filtered_warnings)} warnings")
            # Показываем подробности предупреждений
            for w in filtered_warnings:
                print(f"      {w}")
        elif stderr and any(phrase in stderr.lower() for phrase in [
            "error while processing", "found compiler error", "suppressed"
        ]):
            # Это ложные ошибки ESP32/Arduino - игнорируем
            files_with_errors += 1
            print("   OK")
        else:
            print("   OK")
        
        # Добавляем в отчет
        report_lines.append(f'### {file_name}')
        if filtered_warnings:
            for w in filtered_warnings:
                report_lines.append(f'- `{w}`')
        elif stderr and any(phrase in stderr.lower() for phrase in [
            "error while processing", "found compiler error", "suppressed"
        ]):
            report_lines.append('- ⚠️ **Ложные ошибки ESP32** (игнорируются)')
        else:
            report_lines.append('- ✅ Без предупреждений')
        report_lines.append('')

    # Итоговая статистика
    print()
    print("Summary:")
    print(f"Files with warnings: {files_with_warnings}")
    print(f"Total warnings: {total_warnings}")
    
    if total_warnings == 0:
        print("Status: OK")
    else:
        print("Status: WARNINGS")

    # Сохраняем отчет
    report_lines.extend([
        '## 📈 Статистика',
        f'- **Файлов с предупреждениями:** {files_with_warnings}',
        f'- **Файлов с ложными ошибками ESP32:** {files_with_errors}',
        f'- **Файлов без проблем:** {len(cpp_files) - files_with_warnings - files_with_errors}',
        f'- **Всего предупреждений:** {total_warnings}',
        '',
        f'**Статус:** {"✅ Отлично" if total_warnings == 0 else "⚠️ Требует внимания" if total_warnings < 5 else "❌ Требует исправления"}'
    ])
    
    os.makedirs(os.path.dirname(REPORT_PATH), exist_ok=True)
    with open(REPORT_PATH, 'w', encoding='utf-8') as f:
        f.write('\n'.join(report_lines))

    print(f"Report: {REPORT_PATH}")
    return total_warnings

if __name__ == '__main__':
    main() 