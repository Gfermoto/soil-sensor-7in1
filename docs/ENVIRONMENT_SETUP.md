# Environment Setup Guide

## PowerShell Environment Setup

### Quick Setup

1. **Run the setup script** (one-time setup):
   ```powershell
   .\PowerShell_Profile_Setup.ps1
   ```

2. **Reload your profile**:
   ```powershell
   . $PROFILE
   ```

### Manual Setup

If you prefer manual setup, add these to your PowerShell profile:

```powershell
# JXCT Project Environment Settings
# Отключение пейджеров для автоматизации
$env:PAGER = "cat"
$env:GIT_PAGER = "cat"
$env:MANPAGER = "cat"

# Git настройки для отключения пейджера
git config --global core.pager cat
git config --global pager.log false
git config --global pager.diff false

# Алиасы для удобства
function git-log { git log --oneline -10 }
function git-status { git status --short }
function pio-build { pio run -e esp32dev }
function pio-test { pio test -e native -vv }

# Функция для быстрого просмотра версии
function Get-JXCTVersion {
    if (Test-Path "VERSION") {
        $version = Get-Content "VERSION" -Raw
        Write-Host "JXCT Version: $version" -ForegroundColor Cyan
    } else {
        Write-Host "VERSION file not found" -ForegroundColor Red
    }
}

# Функция для синхронизации версии
function Sync-JXCTVersion {
    Write-Host "Syncing JXCT version..." -ForegroundColor Yellow
    python scripts/auto_version.py
    if ($LASTEXITCODE -eq 0) {
        Write-Host "Version synchronized successfully" -ForegroundColor Green
    } else {
        Write-Host "Version sync failed" -ForegroundColor Red
    }
}
```

## Available Commands

After setup, you'll have these commands available:

- `Get-JXCTVersion` - Show current project version
- `Sync-JXCTVersion` - Sync version across all components
- `git-log` - Show last 10 commits
- `git-status` - Show git status in short format
- `pio-build` - Build firmware for development
- `pio-test` - Run native tests

## Version Management

### Automatic Version Sync

The project uses centralized versioning:

1. **Main version file**: `VERSION` (e.g., "3.6.8")
2. **Auto-generated files**:
   - `include/version.h` - C++ header with version macros
   - `platformio.ini` - PlatformIO configuration
   - `mkdocs.yml` - Documentation configuration
   - Documentation files

### Manual Version Sync

```powershell
# Sync version across all components
python scripts/sync_versions.py

# Or use the PowerShell function
Sync-JXCTVersion
```

### Version Format

The project uses semantic versioning: `MAJOR.MINOR.PATCH`

- **MAJOR**: Breaking changes
- **MINOR**: New features, backward compatible
- **PATCH**: Bug fixes, backward compatible

## CI/CD Integration

### GitHub Actions

The CI pipeline automatically:

1. **Reads version** from `VERSION` file
2. **Generates version.h** using `auto_version.py`
3. **Synchronizes versions** across all components
4. **Builds firmware** for multiple environments
5. **Runs tests** (unit, integration, routes)
6. **Performs analysis** (static analysis, technical debt)
7. **Generates reports** and uploads artifacts

### Workflow Files

- `.github/workflows/ci.yml` - Main CI pipeline
- `.github/workflows/comprehensive-testing.yml` - Extended testing
- `.github/workflows/ci-improved.yml` - Improved CI (backup)

## Troubleshooting

### Common Issues

1. **Commands not found**: Reload PowerShell profile with `. $PROFILE`
2. **Version sync fails**: Check Python dependencies in `requirements.txt`
3. **CI fails**: Check GitHub Actions logs for specific error messages

### Environment Variables

Ensure these are set for automation:

```powershell
$env:PAGER = "cat"
$env:GIT_PAGER = "cat"
$env:MANPAGER = "cat"
```

### Git Configuration

Global git settings to prevent pager issues:

```bash
git config --global core.pager cat
git config --global pager.log false
git config --global pager.diff false
```

## Development Workflow

### Typical Development Session

1. **Start PowerShell** (environment loads automatically)
2. **Check version**: `Get-JXCTVersion`
3. **Make changes** to code
4. **Test locally**: `pio-test`
5. **Build**: `pio-build`
6. **Commit changes**
7. **Push to trigger CI**

### Before Committing

```powershell
# Sync version if changed
Sync-JXCTVersion

# Run tests
pio-test

# Check status
git-status
```

## Additional Tools

### Code Formatting

```powershell
# Format all code
python scripts/auto_format.py

# Or use PowerShell script
.\scripts\format_all.ps1
```

### Documentation

```powershell
# Generate documentation
python scripts/gen_docs.ps1

# Or use mkdocs directly
mkdocs build
```

### Testing

```powershell
# Run all tests
python scripts/run_comprehensive_tests.py

# Run specific test types
python scripts/run_e2e_tests.py
python scripts/run_performance_tests.py
```

## Support

For issues with environment setup:

1. Check this documentation
2. Review PowerShell profile setup
3. Check GitHub Actions logs
4. Contact the development team 