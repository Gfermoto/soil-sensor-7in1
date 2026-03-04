#!/usr/bin/env python3
"""
Док-аудит JXCT: проверка согласованности версий и состояния чек-листов.

Запускается в CI:
1. Сверяет содержимое файлов VERSION / include/version.h / platformio.ini.
2. Собирает все Markdown-файлы, ищет чек-боксы «- [ ]» / «- [x]» и формирует отчёт.
3. Генерирует JSON-файл test_reports/docs-consistency.json.
4. Завершает процесс кодом 1, если найдены несоответствия версий.

Использование (локально):
    python scripts/doc_consistency_check.py --verbose
"""
import argparse
import json
import re
import sys
from pathlib import Path
from typing import List, Dict, Any

ROOT = Path(__file__).resolve().parent.parent
REPORTS_DIR = ROOT / "test_reports"
REPORTS_DIR.mkdir(exist_ok=True)

VERSION_FILE = ROOT / "VERSION"
VERSION_H_FILE = ROOT / "include" / "version.h"
PLATFORMIO_FILE = ROOT / "platformio.ini"

VERSION_RE = re.compile(r"(\d+)\.(\d+)\.(\d+)")
CHECKBOX_RE = re.compile(r"^- \[( |x|X)\] (.*)$")


class ConsistencyError(Exception):
    """Базовое исключение несоответствия."""


def read_version_from_version_file() -> str:
    text = VERSION_FILE.read_text(encoding="utf-8").strip()
    match = VERSION_RE.search(text)
    if not match:
        raise ConsistencyError(f"Не могу извлечь версию из {VERSION_FILE}")
    return match.group(0)


def read_version_from_header() -> str:
    text = VERSION_H_FILE.read_text(encoding="utf-8")
    # Ищем макроопределения
    major = minor = patch = None
    for line in text.splitlines():
        if line.startswith("#define JXCT_VERSION_MAJOR"):
            major = line.split()[-1]
        elif line.startswith("#define JXCT_VERSION_MINOR"):
            minor = line.split()[-1]
        elif line.startswith("#define JXCT_VERSION_PATCH"):
            patch = line.split()[-1]
    if None in (major, minor, patch):
        raise ConsistencyError("Не найден один из JXCT_VERSION_* в version.h")
    return f"{major}.{minor}.{patch}"


def read_version_from_platformio() -> str:
    text = PLATFORMIO_FILE.read_text(encoding="utf-8")
    # Ищем строку "; Version: X.Y.Z"
    for line in text.splitlines():
        if line.lower().startswith("; version"):  # комментарий
            match = VERSION_RE.search(line)
            if match:
                return match.group(0)
    raise ConsistencyError("Не найден коментарий Version: в platformio.ini")


def collect_checkboxes() -> List[Dict[str, Any]]:
    """Сканирует все .md в docs/ и корне, возвращает список чек-боксов."""
    checkboxes = []
    for path in ROOT.rglob("*.md"):
        # Игнорируем большой автогенерированный сайт в docs/html и site/
        if any(segment in {"site", "html"} for segment in path.parts):
            continue
        try:
            for idx, line in enumerate(path.read_text(encoding="utf-8", errors="ignore").splitlines(), start=1):
                m = CHECKBOX_RE.match(line)
                if m:
                    status_char, text = m.groups()
                    checkboxes.append({
                        "file": str(path.relative_to(ROOT)),
                        "line": idx,
                        "checked": status_char.lower() == "x",
                        "task": text.strip(),
                    })
        except Exception:
            # Пропускаем файлы, которые не читаются
            continue
    return checkboxes


def main(argv: List[str] = None) -> None:
    parser = argparse.ArgumentParser(description="Doc consistency check for JXCT project")
    parser.add_argument("--verbose", action="store_true", help="Подробный вывод")
    args = parser.parse_args(argv)

    errors: List[str] = []

    # 1. Проверка версий
    try:
        ver_version = read_version_from_version_file()
        ver_header = read_version_from_header()
        ver_pio = read_version_from_platformio()
        if len({ver_version, ver_header, ver_pio}) != 1:
            errors.append(
                f"Несогласованные версии: VERSION={ver_version}, version.h={ver_header}, platformio.ini={ver_pio}")
    except ConsistencyError as e:
        errors.append(str(e))

    # 2. Сбор TODO-чек-листов
    checkboxes = collect_checkboxes()

    # 3. Формируем отчёт
    report = {
        "version_consistency": len(errors) == 0,
        "errors": errors,
        "version": {
            "VERSION": locals().get("ver_version", "n/a"),
            "version.h": locals().get("ver_header", "n/a"),
            "platformio.ini": locals().get("ver_pio", "n/a"),
        },
        "checkboxes": checkboxes,
        "checkbox_stats": {
            "total": len(checkboxes),
            "done": sum(1 for cb in checkboxes if cb["checked"]),
            "pending": sum(1 for cb in checkboxes if not cb["checked"]),
        },
    }

    # Записываем JSON
    out_file = REPORTS_DIR / "docs-consistency.json"
    out_file.write_text(json.dumps(report, ensure_ascii=False, indent=2), encoding="utf-8")

    if args.verbose:
        print(json.dumps(report, ensure_ascii=False, indent=2))

    # 4. Завершаем с ошибкой, если версии расходятся
    if errors:
        print("❌ Doc consistency check FAILED:\n" + "\n".join(errors), file=sys.stderr)
        sys.exit(1)
    else:
        print("✅ Doc consistency check passed: версии согласованы.")


if __name__ == "__main__":
    main()
