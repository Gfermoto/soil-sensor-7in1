#!/usr/bin/env python3
"""
🧪 JXCT Test Configuration v1.0
Централизованная конфигурация тестирования
Поддержка флага наличия железа, автоматическое определение окружения
"""

import os
import sys
import platform
import subprocess
from pathlib import Path
from typing import Dict, Any, Optional

class TestConfig:
    """Конфигурация тестирования"""
    
    def __init__(self):
        self.hardware_available = self._detect_hardware()
        self.platform = platform.system()
        self.python_version = sys.version_info
        self.project_root = Path(__file__).parent.parent
        
        # Настройки тестирования
        self.test_settings = {
            "hardware_available": self.hardware_available,
            "platform": self.platform,
            "python_version": f"{self.python_version.major}.{self.python_version.minor}",
            "project_root": str(self.project_root),
            "test_timeout": 300,  # секунды
            "coverage_target": 85.0,
            "max_retries": 3
        }
        
        # Настройки для разных типов тестов
        self.test_types = {
            "unit_tests": {
                "enabled": True,
                "requires_hardware": False,
                "timeout": 60,
                "modules": [
                    "test_validation",
                    "test_compensation_formulas",
                    "test_business_calibration_service",
                    "test_advanced_filters",
                    "test_web_routes",
                    "test_wifi_ota_manager",
                    "test_logger_fake_sensor"
                ]
            },
            "integration_tests": {
                "enabled": True,
                "requires_hardware": False,
                "timeout": 120,
                "modules": [
                    "test_api_integration",
                    "test_business_logic_integration"
                ]
            },
            "system_tests": {
                "enabled": True,
                "requires_hardware": False,
                "timeout": 300,
                "modules": [
                    "esp32_build",
                    "clang_tidy",
                    "native_tests"
                ]
            },
            "hardware_tests": {
                "enabled": self.hardware_available,
                "requires_hardware": True,
                "timeout": 600,
                "modules": [
                    "esp32_flash",
                    "esp32_serial_test",
                    "modbus_integration_test"
                ]
            }
        }
    
    def _detect_hardware(self) -> bool:
        """Автоматическое определение наличия железа"""
        # Проверяем переменную окружения
        env_hardware = os.getenv("HARDWARE_AVAILABLE")
        if env_hardware is not None:
            return env_hardware.lower() in ("1", "true", "yes", "on")
        
        # Автоматическое определение ESP32
        try:
            # Проверяем наличие COM-портов (Windows)
            if platform.system() == "Windows":
                result = subprocess.run(
                    ["mode"], 
                    capture_output=True, 
                    text=True, 
                    encoding='utf-8',
                    errors='ignore',
                    timeout=10
                )
                if "COM" in result.stdout:
                    return True
            
            # Проверяем наличие /dev/ttyUSB (Linux/Mac)
            elif platform.system() in ("Linux", "Darwin"):
                result = subprocess.run(
                    ["ls", "/dev/ttyUSB*"], 
                    capture_output=True, 
                    text=True,
                    timeout=10
                )
                if result.returncode == 0 and result.stdout.strip():
                    return True
            
            # Проверяем PlatformIO
            result = subprocess.run(
                ["pio", "device", "list"],
                capture_output=True,
                text=True,
                encoding='utf-8',
                errors='ignore',
                timeout=10
            )
            if "ESP32" in result.stdout or "esp32" in result.stdout.lower():
                return True
                
        except Exception:
            pass
        
        return False
    
    def get_test_modules(self, test_type: str = None) -> list:
        """Получает список модулей для тестирования"""
        if test_type:
            return self.test_types.get(test_type, {}).get("modules", [])
        
        # Возвращаем все модули
        all_modules = []
        for test_type, config in self.test_types.items():
            if config["enabled"] and (not config["requires_hardware"] or self.hardware_available):
                all_modules.extend(config["modules"])
        
        return all_modules
    
    def should_run_test(self, test_type: str) -> bool:
        """Определяет, нужно ли запускать тест"""
        config = self.test_types.get(test_type, {})
        return config.get("enabled", False) and (not config.get("requires_hardware", False) or self.hardware_available)
    
    def get_test_timeout(self, test_type: str) -> int:
        """Получает timeout для типа теста"""
        return self.test_types.get(test_type, {}).get("timeout", self.test_settings["test_timeout"])
    
    def print_config(self):
        """Выводит конфигурацию"""
        print("🧪 КОНФИГУРАЦИЯ ТЕСТИРОВАНИЯ")
        print("=" * 50)
        print(f"🔧 Платформа: {self.platform}")
        print(f"🐍 Python: {self.test_settings['python_version']}")
        print(f"💻 Железо доступно: {'✅' if self.hardware_available else '❌'}")
        print(f"🎯 Целевое покрытие: {self.test_settings['coverage_target']}%")
        print(f"⏱️ Timeout по умолчанию: {self.test_settings['test_timeout']} сек")
        
        print("\n📋 ТИПЫ ТЕСТОВ:")
        for test_type, config in self.test_types.items():
            status = "✅" if config["enabled"] else "❌"
            hardware_req = "🔌" if config["requires_hardware"] else "💻"
            will_run = "✅" if self.should_run_test(test_type) else "⏭️"
            print(f"   {status} {test_type}: {hardware_req} {will_run} ({len(config['modules'])} модулей)")
        
        print(f"\n📁 Корневая директория: {self.project_root}")

# Глобальный экземпляр конфигурации
test_config = TestConfig()

if __name__ == "__main__":
    test_config.print_config() 