# PowerShell Profile Setup for JXCT Project
# Запустите этот скрипт один раз для настройки среды

Write-Host "Setting up PowerShell environment for JXCT project..." -ForegroundColor Green

# Создаем профиль PowerShell если его нет
$profilePath = $PROFILE.CurrentUserAllHosts
$profileDir = Split-Path $profilePath -Parent

if (!(Test-Path $profileDir)) {
    New-Item -ItemType Directory -Path $profileDir -Force
    Write-Host "Created profile directory: $profileDir" -ForegroundColor Yellow
}

# Добавляем настройки в профиль
$profileContent = @"

# JXCT Project Environment Settings
# Отключение пейджеров для автоматизации
`$env:PAGER = "cat"
`$env:GIT_PAGER = "cat"
`$env:MANPAGER = "cat"

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
        `$version = Get-Content "VERSION" -Raw
        Write-Host "JXCT Version: `$version" -ForegroundColor Cyan
    } else {
        Write-Host "VERSION file not found" -ForegroundColor Red
    }
}

# Функция для синхронизации версии
function Sync-JXCTVersion {
    Write-Host "Syncing JXCT version..." -ForegroundColor Yellow
    python scripts/auto_version.py
    if (`$LASTEXITCODE -eq 0) {
        Write-Host "Version synchronized successfully" -ForegroundColor Green
    } else {
        Write-Host "Version sync failed" -ForegroundColor Red
    }
}

Write-Host "JXCT environment loaded. Use Get-JXCTVersion and Sync-JXCTVersion commands." -ForegroundColor Green
"@

# Проверяем, есть ли уже настройки JXCT в профиле
if (Test-Path $profilePath) {
    $existingContent = Get-Content $profilePath -Raw
    if ($existingContent -notlike "*JXCT Project Environment Settings*") {
        Add-Content -Path $profilePath -Value $profileContent
        Write-Host "Added JXCT settings to existing profile" -ForegroundColor Yellow
    } else {
        Write-Host "JXCT settings already exist in profile" -ForegroundColor Yellow
    }
} else {
    Set-Content -Path $profilePath -Value $profileContent
    Write-Host "Created new profile with JXCT settings" -ForegroundColor Green
}

Write-Host "Profile setup complete! Restart PowerShell or run: . `$PROFILE" -ForegroundColor Green
Write-Host "Available commands: Get-JXCTVersion, Sync-JXCTVersion, git-log, git-status, pio-build, pio-test" -ForegroundColor Cyan 