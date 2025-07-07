#!/usr/bin/env python3
"""
Скрипт для замены вызовов C-style variadic functions на безопасные template версии
"""

import os
import re
import glob

def fix_logger_calls():
    """Заменяет все вызовы C-style variadic functions на безопасные template версии"""
    
    # Паттерны для замены
    replacements = [
        (r'logError\s*\(\s*"([^"]*)"\s*,', r'logErrorSafe("\\1",'),
        (r'logWarn\s*\(\s*"([^"]*)"\s*,', r'logWarnSafe("\\1",'),
        (r'logInfo\s*\(\s*"([^"]*)"\s*,', r'logInfoSafe("\\1",'),
        (r'logDebug\s*\(\s*"([^"]*)"\s*,', r'logDebugSafe("\\1",'),
        (r'logSuccess\s*\(\s*"([^"]*)"\s*,', r'logSuccessSafe("\\1",'),
        (r'logSensor\s*\(\s*"([^"]*)"\s*,', r'logSensorSafe("\\1",'),
        (r'logWiFi\s*\(\s*"([^"]*)"\s*,', r'logWiFiSafe("\\1",'),
        (r'logMQTT\s*\(\s*"([^"]*)"\s*,', r'logMQTTSafe("\\1",'),
        (r'logHTTP\s*\(\s*"([^"]*)"\s*,', r'logHTTPSafe("\\1",'),
        (r'logSystem\s*\(\s*"([^"]*)"\s*,', r'logSystemSafe("\\1",'),
        (r'logData\s*\(\s*"([^"]*)"\s*,', r'logDataSafe("\\1",'),
    ]
    
    # Файлы для обработки
    cpp_files = glob.glob('src/**/*.cpp', recursive=True)
    
    total_fixes = 0
    
    for file_path in cpp_files:
        if 'logger.cpp' in file_path:  # Пропускаем сам logger.cpp
            continue
            
        try:
            with open(file_path, 'r', encoding='utf-8') as f:
                content = f.read()
            
            original_content = content
            
            # Применяем все замены
            for pattern, replacement in replacements:
                content = re.sub(pattern, replacement, content)
            
            # Если что-то изменилось, записываем файл
            if content != original_content:
                with open(file_path, 'w', encoding='utf-8') as f:
                    f.write(content)
                
                # Подсчитываем изменения
                changes = 0
                for pattern, replacement in replacements:
                    matches = len(re.findall(pattern, original_content))
                    changes += matches
                
                total_fixes += changes
                print(f"✅ {file_path}: {changes} замен")
                
        except Exception as e:
            print(f"❌ Ошибка при обработке {file_path}: {e}")
    
    print(f"\n🎯 Всего заменено: {total_fixes} вызовов")
    return total_fixes

if __name__ == "__main__":
    print("🔧 Замена вызовов C-style variadic functions на безопасные template версии...")
    fix_logger_calls()
    print("✅ Завершено!") 