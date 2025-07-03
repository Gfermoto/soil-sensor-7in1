#!/usr/bin/env python3
"""
Version synchronization script for JXCT project.
Synchronizes version across all project components.
"""

import os
import re
import sys
import json
from datetime import datetime, timezone
from pathlib import Path

def read_version_file(version_file):
    """Read version from VERSION file."""
    try:
        with open(version_file, 'r', encoding='utf-8') as f:
            version = f.read().strip()
        return version
    except FileNotFoundError:
        print(f"ERROR: VERSION file not found at {version_file}")
        return None
    except Exception as e:
        print(f"ERROR: Failed to read VERSION file: {e}")
        return None

def validate_version(version):
    """Validate version format (semantic versioning)."""
    pattern = r'^(\d+)\.(\d+)\.(\d+)(?:-([0-9A-Za-z-]+(?:\.[0-9A-Za-z-]+)*))?(?:\+([0-9A-Za-z-]+(?:\.[0-9A-Za-z-]+)*))?$'
    match = re.match(pattern, version)
    if not match:
        return False, None
    return True, match.groups()

def update_platformio_version(platformio_file, version):
    """Update version in platformio.ini."""
    if not os.path.exists(platformio_file):
        print(f"WARNING: platformio.ini not found at {platformio_file}")
        return False

    try:
        with open(platformio_file, 'r', encoding='utf-8') as f:
            content = f.read()

        # Update version in comments
        content = re.sub(
            r'JXCT Soil Sensor v[\d.]+',
            f'JXCT Soil Sensor v{version}',
            content
        )

        with open(platformio_file, 'w', encoding='utf-8') as f:
            f.write(content)

        print(f"✓ Updated platformio.ini to version {version}")
        return True
    except Exception as e:
        print(f"ERROR: Failed to update platformio.ini: {e}")
        return False

def update_mkdocs_version(mkdocs_file, version):
    """Update version in mkdocs.yml."""
    if not os.path.exists(mkdocs_file):
        print(f"WARNING: mkdocs.yml not found at {mkdocs_file}")
        return False

    try:
        with open(mkdocs_file, 'r', encoding='utf-8') as f:
            content = f.read()

        # Update version in site_name or other relevant fields
        content = re.sub(
            r'site_name:\s*.*',
            f'site_name: JXCT Soil Sensor v{version}',
            content
        )

        with open(mkdocs_file, 'w', encoding='utf-8') as f:
            f.write(content)

        print(f"✓ Updated mkdocs.yml to version {version}")
        return True
    except Exception as e:
        print(f"ERROR: Failed to update mkdocs.yml: {e}")
        return False

def update_package_json(package_file, version):
    """Update version in package.json if it exists."""
    if not os.path.exists(package_file):
        return True  # Not required

    try:
        with open(package_file, 'r', encoding='utf-8') as f:
            data = json.load(f)

        data['version'] = version

        with open(package_file, 'w', encoding='utf-8') as f:
            json.dump(data, f, indent=2)

        print(f"✓ Updated package.json to version {version}")
        return True
    except Exception as e:
        print(f"ERROR: Failed to update package.json: {e}")
        return False

def update_documentation_files(docs_dir, version):
    """Update version in documentation files."""
    if not os.path.exists(docs_dir):
        print(f"WARNING: docs directory not found at {docs_dir}")
        return True

    # Files that might contain version information
    version_files = [
        'docs/index.md',
        'docs/manuals/API.md',
        'docs/manuals/TECHNICAL_DOCS.md',
        'README.md'
    ]

    updated_count = 0
    for file_path in version_files:
        if not os.path.exists(file_path):
            continue

        try:
            with open(file_path, 'r', encoding='utf-8') as f:
                content = f.read()

            # Update version patterns
            patterns = [
                (r'JXCT Soil Sensor v[\d.]+', f'JXCT Soil Sensor v{version}'),
                (r'Version:[\s]*[\d.]+', f'Version: {version}'),
                (r'v[\d.]+', f'v{version}')  # Be careful with this one
            ]

            original_content = content
            for pattern, replacement in patterns:
                content = re.sub(pattern, replacement, content)

            if content != original_content:
                with open(file_path, 'w', encoding='utf-8') as f:
                    f.write(content)
                updated_count += 1
                print(f"✓ Updated {file_path}")

        except Exception as e:
            print(f"WARNING: Failed to update {file_path}: {e}")

    if updated_count > 0:
        print(f"✓ Updated {updated_count} documentation files")
    return True

def create_version_report(version, project_root):
    """Create a version synchronization report."""
    report = {
        "timestamp": datetime.now(timezone.utc).isoformat(),
        "version": version,
        "project_root": str(project_root),
        "components_updated": [],
        "status": "success"
    }

    # Check which components were updated
    components = [
        ("VERSION", os.path.join(project_root, "VERSION")),
        ("include/version.h", os.path.join(project_root, "include", "version.h")),
        ("platformio.ini", os.path.join(project_root, "platformio.ini")),
        ("mkdocs.yml", os.path.join(project_root, "mkdocs.yml")),
        ("package.json", os.path.join(project_root, "package.json"))
    ]

    for name, path in components:
        if os.path.exists(path):
            report["components_updated"].append(name)

    # Save report
    report_file = os.path.join(project_root, "test_reports", "version-sync.json")
    os.makedirs(os.path.dirname(report_file), exist_ok=True)

    with open(report_file, 'w', encoding='utf-8') as f:
        json.dump(report, f, indent=2)

    print(f"✓ Version sync report saved to {report_file}")
    return report

def main():
    """Main function."""
    print("JXCT Version Synchronization Tool")
    print("=" * 40)

    # Find project root
    script_dir = Path(__file__).parent
    project_root = script_dir.parent

    version_file = project_root / "VERSION"

    # Read version
    version = read_version_file(version_file)
    if not version:
        sys.exit(1)

    print(f"Current version: {version}")

    # Validate version
    is_valid, parts = validate_version(version)
    if not is_valid:
        print(f"ERROR: Invalid version format: {version}")
        print("Expected format: MAJOR.MINOR.PATCH[-PRERELEASE][+BUILD]")
        sys.exit(1)

    print(f"Version format: ✓ Valid")

    # Update components
    success = True

    # 1. Generate version.h (using existing script)
    print("\n1. Generating version.h...")
    try:
        import subprocess
        result = subprocess.run([sys.executable, "scripts/auto_version.py"],
                              cwd=project_root, capture_output=True, text=True)
        if result.returncode == 0:
            print("✓ version.h generated successfully")
        else:
            print(f"ERROR: Failed to generate version.h: {result.stderr}")
            success = False
    except Exception as e:
        print(f"ERROR: Failed to run auto_version.py: {e}")
        success = False

    # 2. Update platformio.ini
    print("\n2. Updating platformio.ini...")
    if not update_platformio_version(project_root / "platformio.ini", version):
        success = False

    # 3. Update mkdocs.yml
    print("\n3. Updating mkdocs.yml...")
    if not update_mkdocs_version(project_root / "mkdocs.yml", version):
        success = False

    # 4. Update package.json (if exists)
    print("\n4. Updating package.json...")
    if not update_package_json(project_root / "package.json", version):
        success = False

    # 5. Update documentation
    print("\n5. Updating documentation...")
    if not update_documentation_files(project_root / "docs", version):
        success = False

    # 6. Create report
    print("\n6. Creating version sync report...")
    report = create_version_report(version, project_root)

    # Summary
    print("\n" + "=" * 40)
    print("VERSION SYNCHRONIZATION SUMMARY")
    print("=" * 40)
    print(f"Version: {version}")
    print(f"Status: {'✓ SUCCESS' if success else '✗ FAILED'}")
    print(f"Components updated: {len(report['components_updated'])}")
    print(f"Report: test_reports/version-sync.json")

    if not success:
        sys.exit(1)

    print("\n✓ Version synchronization completed successfully!")

if __name__ == "__main__":
    main()
