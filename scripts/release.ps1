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
    Write-Host "💡 Перейдите в корневую папку проекта JXCT" -ForegroundColor Yellow
    exit 1
}

# Проверяем наличие Python
try {
    $pythonVersion = python --version 2>&1
    if ($LASTEXITCODE -ne 0) {
        Write-Host "ERROR: Python не найден!" -ForegroundColor Red
        Write-Host "💡 Установите Python и добавьте в PATH" -ForegroundColor Yellow
        exit 1
    }
    Write-Host "✅ Python найден: $pythonVersion" -ForegroundColor Green
} catch {
    Write-Host "ERROR: Не удалось проверить Python: $_" -ForegroundColor Red
    exit 1
}

# Проверяем наличие Python скрипта
if (-not (Test-Path "scripts/release_cycle.py")) {
    Write-Host "ERROR: scripts/release_cycle.py не найден!" -ForegroundColor Red
    exit 1
}

# Проверяем git статус
Write-Host "🔍 Проверяю git статус..." -ForegroundColor Cyan
try {
    $gitStatus = git status --porcelain 2>&1
    if ($LASTEXITCODE -ne 0) {
        Write-Host "ERROR: Git не инициализирован или ошибка git!" -ForegroundColor Red
        exit 1
    }
    
    if ($gitStatus) {
        Write-Host "⚠️  Есть несохраненные изменения:" -ForegroundColor Yellow
        Write-Host $gitStatus -ForegroundColor Gray
        if (-not $Auto) {
            $response = Read-Host "🤔 Продолжить? (y/N)"
            if ($response -ne "y" -and $response -ne "Y") {
                Write-Host "❌ Отменено пользователем" -ForegroundColor Red
                exit 0
            }
        }
    } else {
        Write-Host "✅ Рабочая директория чистая" -ForegroundColor Green
    }
} catch {
    Write-Host "ERROR: Ошибка проверки git: $_" -ForegroundColor Red
    exit 1
}

# Формируем аргументы для Python скрипта
$args = @($IncrementType)
if ($Auto) {
    $args += "--auto"
}

Write-Host "🚀 Запускаю Python скрипт с аргументами: $($args -join ' ')" -ForegroundColor Cyan

# Запускаем Python скрипт
try {
    $startTime = Get-Date
    python scripts/release_cycle.py @args
    $endTime = Get-Date
    $duration = $endTime - $startTime
    
    if ($LASTEXITCODE -eq 0) {
        Write-Host ""
        Write-Host "🎉 RELEASE CREATED SUCCESSFULLY!" -ForegroundColor Green
        Write-Host "⏱️  Время выполнения: $($duration.TotalSeconds.ToString('F1')) секунд" -ForegroundColor Gray
        Write-Host ""
        Write-Host "💡 Следующие шаги:" -ForegroundColor Yellow
        Write-Host "   1. Проверьте релиз на GitHub" -ForegroundColor White
        Write-Host "   2. Прошивка готова для загрузки: pio run --target upload" -ForegroundColor White
        Write-Host "   3. Или используйте OTA обновление через веб-интерфейс" -ForegroundColor White
    } else {
        Write-Host ""
        Write-Host "❌ ERROR: Release creation failed" -ForegroundColor Red
        Write-Host "💡 Проверьте логи выше для деталей" -ForegroundColor Yellow
        exit 1
    }
} catch {
    Write-Host ""
    Write-Host "❌ ERROR: Script execution failed: $_" -ForegroundColor Red
    Write-Host "💡 Проверьте:" -ForegroundColor Yellow
    Write-Host "   - Python установлен и доступен" -ForegroundColor White
    Write-Host "   - Все зависимости установлены" -ForegroundColor White
    Write-Host "   - Git настроен правильно" -ForegroundColor White
    exit 1
} 