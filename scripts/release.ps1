# Автоматический цикл релиза JXCT
# Использование:
#   .\scripts\release.ps1           - patch версия (3.2.7 -> 3.2.8)
#   .\scripts\release.ps1 minor     - minor версия (3.2.7 -> 3.3.0)  
#   .\scripts\release.ps1 major     - major версия (3.2.7 -> 4.0.0)
#   .\scripts\release.ps1 patch --auto  - без подтверждения

param(
    [string]$IncrementType = "patch",
    [switch]$Auto
)

Write-Host "🚀 АВТОМАТИЧЕСКИЙ ЦИКЛ РЕЛИЗА JXCT" -ForegroundColor Green
Write-Host "=" * 50

# Проверяем, что мы в корневой директории проекта
if (-not (Test-Path "VERSION")) {
    Write-Host "❌ Запустите скрипт из корневой директории проекта!" -ForegroundColor Red
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
        Write-Host "🎉 РЕЛИЗ СОЗДАН УСПЕШНО!" -ForegroundColor Green
        Write-Host "💡 Теперь можете прошить устройство командой: pio run --target upload" -ForegroundColor Yellow
    } else {
        Write-Host "❌ Ошибка создания релиза" -ForegroundColor Red
        exit 1
    }
} catch {
    Write-Host "❌ Ошибка запуска скрипта: $_" -ForegroundColor Red
    exit 1
} 