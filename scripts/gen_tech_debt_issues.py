#!/usr/bin/env python3
"""Generate tech-debt Markdown & JSON from static-analysis artifacts.

Usage:
  python scripts/gen_tech_debt_issues.py --artifacts ./artifacts \
      --out-md docs_src/development/tech_debt_auto.md \
      --out-json artifacts/tech_debt_issues.json

The script is intentionally tolerant to absent or malformed inputs: if a file
is missing it is silently skipped.
"""
from __future__ import annotations

import argparse
import json
import re
import sys
from pathlib import Path
from datetime import datetime

try:
    import yaml  # type: ignore
except ImportError:
    yaml = None  # YAML parsing will be skipped if PyYAML not present


DIAG_SEVERITY = {
    "clang-tidy": "medium",
    "iwyu": "low",
    "lizard": "medium",
    "duplication": "low",
}


def parse_clang_tidy(path: Path):
    issues = []
    if not path.exists() or yaml is None:
        return issues
    try:
        data = yaml.safe_load(path.read_text(encoding="utf-8"))
        for diag in data.get("Diagnostics", []):
            issues.append(
                {
                    "tool": "clang-tidy",
                    "file": Path(diag.get("FilePath", "")).name,
                    "raw_path": diag.get("FilePath", ""),
                    "message": diag.get("DiagnosticName", diag.get("Message", "")),
                    "severity": DIAG_SEVERITY["clang-tidy"],
                }
            )
    except Exception as exc:  # noqa: broad-except
        print(f"[WARN] Failed to parse clang-tidy fixes: {exc}", file=sys.stderr)
    return issues


def parse_iwyu(path: Path):
    issues = []
    if not path.exists():
        return issues
    pattern = re.compile(r"^(?P<file>[^:]+): should add these lines")
    for line in path.read_text(encoding="utf-8", errors="ignore").splitlines():
        m = pattern.match(line)
        if m:
            f = m.group("file")
            issues.append(
                {
                    "tool": "iwyu",
                    "file": Path(f).name,
                    "raw_path": f,
                    "message": "Missing forward/includes",
                    "severity": DIAG_SEVERITY["iwyu"],
                }
            )
    return issues


def parse_lizard(path: Path):
    issues = []
    if not path.exists():
        return issues
    # Example line (wide): " 15    120  someFunc      src/file.cpp:123"
    for line in path.read_text(encoding="utf-8", errors="ignore").splitlines():
        tokens = line.split()
        if len(tokens) < 5:
            continue
        try:
            ccn = int(tokens[0])
        except ValueError:
            continue
        if ccn <= 15:
            continue
        func_name = tokens[2]
        file_loc = tokens[3]
        issues.append(
            {
                "tool": "lizard",
                "file": Path(file_loc.split(":")[0]).name,
                "raw_path": file_loc,
                "message": f"High complexity ({ccn}) in {func_name}",
                "severity": DIAG_SEVERITY["lizard"],
            }
        )
    return issues


def parse_cloc_dup(path: Path):
    issues = []
    if not path.exists():
        return issues
    for line in path.read_text(encoding="utf-8", errors="ignore").splitlines():
        if "DUPE" in line:
            parts = [p for p in line.split() if p]
            if len(parts) >= 2:
                f = parts[-1]
                issues.append(
                    {
                        "tool": "duplication",
                        "file": Path(f).name,
                        "raw_path": f,
                        "message": "Duplicated code block",
                        "severity": DIAG_SEVERITY["duplication"],
                    }
                )
    return issues


def build_markdown(issues):
    md = ["| Tool | File | Message | Severity |", "|------|------|---------|----------|"]
    for i in issues:
        md.append(f"| {i['tool']} | {i['file']} | {i['message']} | {i['severity']} |")
    if len(issues) == 0:
        md.append("| ✅ | No issues found | | |")
    return "\n".join(md)


def main():
    parser = argparse.ArgumentParser(description="Generate tech-debt issue report")
    parser.add_argument("--artifacts", type=Path, default=Path("."), help="Dir with analysis files")
    parser.add_argument("--out-md", type=Path, required=True, help="Path to output Markdown")
    parser.add_argument("--out-json", type=Path, required=False, help="Path to output JSON")
    args = parser.parse_args()

    art = args.artifacts
    issues = []
    issues += parse_clang_tidy(art / "clang_tidy_fixes.yaml")
    issues += parse_iwyu(art / "iwyu_report.txt")
    issues += parse_lizard(art / "lizard_report.txt")
    issues += parse_cloc_dup(art / "cloc_dup.txt")

    # Sort by tool then file for consistency
    issues.sort(key=lambda x: (x["tool"], x["file"]))

    header = f"# 🐞 Tech-Debt Auto-Report\nGenerated: {datetime.utcnow().isoformat(timespec='seconds')} UTC\n\n"
    md_body = build_markdown(issues)
    args.out_md.write_text(header + md_body, encoding="utf-8")

    if args.out_json:
        args.out_json.write_text(json.dumps(issues, indent=2), encoding="utf-8")

    print(f"Wrote {len(issues)} issues to {args.out_md}")


if __name__ == "__main__":
    main() 