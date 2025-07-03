#!/usr/bin/env python3
"""
Скрипт синхронизации версий JXCT
Версия: 1.0.0
Автор: EYERA Development Team
Дата: 2025-07-03

Этот скрипт синхронизирует версии во всех файлах проекта
"""

import os
import re
import sys
from pathlib import Path
from datetime import datetime, UTC


class VersionSynchronizer:
    """Синхронизирует версии во всех файлах проекта"""

    def __init__(self, project_root: Path):
        self.project_root = project_root
        self.version_file = project_root / "VERSION"
        self.current_version = self._read_version()

        # Файлы для обновления версии
        self.version_files = [
            "VERSION",
            "include/version.h",
            "platformio.ini",
            "mkdocs.yml",
            "README.md",
            "docs/index.md",
            "docs/CURRENT_TEST_RESULTS.md",
            "docs/TESTING_GUIDE.md",
            "docs/STAGE_2_COMPLETION_REPORT.md",
            "docs/NAVIGATION.md",
            "docs/dev/TECH_DEBT_REPORT_2025-06.md",
            "docs/dev/ARCH_OVERALL.md",
            "docs/manuals/API.md",
            "docs/manuals/USER_GUIDE.md",
            "docs/manuals/TECHNICAL_DOCS.md",
            "docs/manuals/CONFIG_MANAGEMENT.md",
            "docs/manuals/COMPENSATION_GUIDE.md",
            "docs/manuals/AGRO_RECOMMENDATIONS.md",
            "docs/manuals/MODBUS_PROTOCOL.md",
            "docs/manuals/VERSION_MANAGEMENT.md",
            "docs/manuals/WIRING_DIAGRAM.md",
            "src/web/routes_reports.cpp",
            "test/esp/test_simple.cpp",
            "test/esp/test_all_combined.cpp",
            ".github/workflows/comprehensive-testing.yml",
            "scripts/run_comprehensive_tests.py"
        ]

        # JSON файлы для обновления
        self.json_files = [
            "test_reports/simple-test-report.json",
            "test_reports/comprehensive-report.json",
            "test_reports/combined-test-report.json",
            "test_reports/technical-debt.json",
            "docs/test_reports/simple-test-report.json",
            "docs/test_reports/comprehensive-report.json",
            "docs/test_reports/combined-test-report.json",
            "docs/test_reports/technical-debt.json",
            "site/test_reports/simple-test-report.json",
            "site/test_reports/comprehensive-report.json",
            "site/test_reports/combined-test-report.json",
            "site/test_reports/technical-debt.json"
        ]

    def _read_version(self) -> str:
        """Читает текущую версию из файла VERSION"""
        try:
            # Пробуем разные кодировки
            for encoding in ['utf-8', 'cp1251', 'latin-1', 'iso-8859-1']:
                try:
                    with open(self.version_file, 'r', encoding=encoding) as f:
                        version = f.read().strip()
                        print(f"📋 Текущая версия: {version}")
                        return version
                except UnicodeDecodeError:
                    continue
            # Если ничего не получилось, пробуем бинарное чтение
            with open(self.version_file, 'rb') as f:
                content = f.read()
                # Убираем BOM и декодируем
                if content.startswith(b'\xef\xbb\xbf'):
                    content = content[3:]
                version = content.decode('utf-8', errors='ignore').strip()
                print(f"📋 Текущая версия (бинарное чтение): {version}")
                return version
        except FileNotFoundError:
            print("❌ Файл VERSION не найден!")
            sys.exit(1)
        except Exception as e:
            print(f"❌ Ошибка чтения VERSION: {e}")
            sys.exit(1)

    def sync_all_versions(self):
        """Синхронизирует версии во всех файлах"""
        print(f"🔄 Синхронизация версии {self.current_version}...")

        # Обновляем основные файлы
        for file_path in self.version_files:
            self._update_file_version(file_path)

        # Обновляем JSON файлы
        for file_path in self.json_files:
            self._update_json_version(file_path)

        print("✅ Синхронизация версий завершена!")

    def _update_file_version(self, file_path: str):
        """Обновляет версию в текстовом файле"""
        full_path = self.project_root / file_path
        if not full_path.exists():
            print(f"⚠️ Файл не найден: {file_path}")
            return

        try:
            with open(full_path, 'r', encoding='utf-8') as f:
                content = f.read()

            # Паттерны для поиска версий
            patterns = [
                r'version["\s]*[:=]["\s]*3\.6\.\d+["\s]*',
                r'Version["\s]*[:=]["\s]*3\.6\.\d+["\s]*',
                r'Версия["\s]*[:=]["\s]*3\.6\.\d+["\s]*',
                r'v3\.6\.\d+',
                r'3\.6\.\d+',
                r'PROJECT_VERSION["\s]*[:=]["\s]*3\.6\.\d+["\s]*'
            ]

            updated = False
            for pattern in patterns:
                if re.search(pattern, content):
                    # Заменяем старую версию на новую
                    new_content = re.sub(pattern, lambda m: self._replace_version(m.group(), self.current_version), content)
                    if new_content != content:
                        content = new_content
                        updated = True

            if updated:
                with open(full_path, 'w', encoding='utf-8') as f:
                    f.write(content)
                print(f"✅ Обновлен: {file_path}")
            else:
                print(f"ℹ️ Без изменений: {file_path}")

        except Exception as e:
            print(f"❌ Ошибка обновления {file_path}: {e}")

    def _update_json_version(self, file_path: str):
        """Обновляет версию в JSON файле"""
        full_path = self.project_root / file_path
        if not full_path.exists():
            print(f"⚠️ Файл не найден: {file_path}")
            return

        try:
            with open(full_path, 'r', encoding='utf-8') as f:
                content = f.read()

            # Заменяем версию в JSON
            pattern = r'"version"\s*:\s*"3\.6\.\d+"'
            if re.search(pattern, content):
                new_content = re.sub(pattern, f'"version": "{self.current_version}"', content)
                if new_content != content:
                    with open(full_path, 'w', encoding='utf-8') as f:
                        f.write(new_content)
                    print(f"✅ Обновлен JSON: {file_path}")
                else:
                    print(f"ℹ️ JSON без изменений: {file_path}")
            else:
                print(f"ℹ️ Версия не найдена в JSON: {file_path}")

        except Exception as e:
            print(f"❌ Ошибка обновления JSON {file_path}: {e}")

    def _replace_version(self, match: str, new_version: str) -> str:
        """Заменяет версию в найденном совпадении"""
        # Заменяем любую версию 3.6.x на новую
        return re.sub(r'3\.6\.\d+', new_version, match)

    def verify_sync(self):
        """Проверяет синхронизацию версий"""
        print("\n🔍 Проверка синхронизации версий...")

        mismatches = []

        for file_path in self.version_files + self.json_files:
            full_path = self.project_root / file_path
            if not full_path.exists():
                continue

            try:
                with open(full_path, 'r', encoding='utf-8') as f:
                    content = f.read()

                # Ищем версии 3.6.x
                versions = re.findall(r'3\.6\.\d+', content)
                for version in versions:
                    if version != self.current_version:
                        mismatches.append(f"{file_path}: {version} != {self.current_version}")

            except Exception as e:
                print(f"❌ Ошибка проверки {file_path}: {e}")

        if mismatches:
            print("❌ Найдены несоответствия версий:")
            for mismatch in mismatches:
                print(f"  - {mismatch}")
        else:
            print("✅ Все версии синхронизированы!")


def main():
    """Главная функция"""
    project_root = Path.cwd()

    print("🚀 Синхронизация версий JXCT")
    print(f"📁 Рабочая директория: {project_root}")

    synchronizer = VersionSynchronizer(project_root)

    # Синхронизируем версии
    synchronizer.sync_all_versions()

    # Проверяем результат
    synchronizer.verify_sync()

    print(f"\n🎉 Синхронизация завершена: {datetime.now(UTC).strftime('%Y-%m-%d %H:%M:%S UTC')}")


if __name__ == "__main__":
    main()
