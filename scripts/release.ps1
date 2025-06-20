# Автоматический цикл релиза JXCT
# Использование:
#   .\scripts\release.ps1           - patch версия (3.2.7 -> 3.2.8)
#   .\scripts\release.ps1 minor     - minor версия (3.2.7 -> 3.3.0)  
#   .\scripts\release.ps1 major     - major версия (3.2.7 -> 4.0.0)
#   .\scripts\release.ps1 patch -Auto  - без подтверждения

param(
    [string]$IncrementType = "patch",
    [switch]$Auto
)

Write-Host "RELEASE CYCLE JXCT" -ForegroundColor Green
Write-Host "=" * 50

# Проверяем, что мы в корневой директории проекта
if (-not (Test-Path "VERSION")) {
    Write-Host "ERROR: Run script from project root directory!" -ForegroundColor Red
    exit 1
}

# Формируем аргументы для Python скрипта
$args = @($IncrementType)
if ($Auto) {
    $args += "--auto"
}

# Запускаем Python скрипт
try {
    python scripts/release_cycle.py @args
    if ($LASTEXITCODE -eq 0) {
        Write-Host ""
        Write-Host "RELEASE CREATED SUCCESSFULLY!" -ForegroundColor Green
        Write-Host "TIP: You can now flash device with: pio run --target upload" -ForegroundColor Yellow
    } else {
        Write-Host "ERROR: Release creation failed" -ForegroundColor Red
        exit 1
    }
} catch {
    Write-Host "ERROR: Script execution failed: $_" -ForegroundColor Red
    exit 1
} 