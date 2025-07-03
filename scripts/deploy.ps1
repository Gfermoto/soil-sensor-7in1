[Console]::OutputEncoding = [System.Text.Encoding]::UTF8
# Full deployment script for JXCT documentation

Write-Host "[deploy] Starting full deployment..." -ForegroundColor Cyan

# Проверяем статус git
$gitStatus = git status --porcelain
if ($gitStatus) {
    Write-Host "[deploy] Uncommitted changes detected:" -ForegroundColor Yellow
    Write-Host $gitStatus -ForegroundColor Gray
    $response = Read-Host "Do you want to commit these changes? (y/n)"
    if ($response -eq "y" -or $response -eq "Y") {
        $commitMessage = Read-Host "Enter commit message (or press Enter for auto-message)"
        if (-not $commitMessage) {
            $commitMessage = "Auto-update documentation $(Get-Date -Format 'yyyy-MM-dd HH:mm')"
        }
        git add .
        git commit -m $commitMessage
        Write-Host "[deploy] Changes committed" -ForegroundColor Green
    } else {
        Write-Host "[deploy] Deployment cancelled" -ForegroundColor Red
        exit 1
    }
}

# Генерируем документацию
Write-Host "[deploy] Generating documentation..." -ForegroundColor Cyan
& "$PSScriptRoot\gen_docs.ps1"
if ($LASTEXITCODE -ne 0) {
    Write-Host "[deploy] Documentation generation failed!" -ForegroundColor Red
    exit 1
}

# Проверяем, что сайт собрался
if (-not (Test-Path "site/index.html")) {
    Write-Host "[deploy] Site build failed - index.html not found!" -ForegroundColor Red
    exit 1
}

# Проверяем, что Doxygen доступен
if (-not (Test-Path "site/api/html/index.html")) {
    Write-Host "[deploy] Doxygen not found after build!" -ForegroundColor Red
    exit 1
}

Write-Host "[deploy] Documentation generated successfully" -ForegroundColor Green

# Пуш в git
Write-Host "[deploy] Pushing to git..." -ForegroundColor Cyan
git add site/
$deployMessage = "Deploy documentation $(Get-Date -Format 'yyyy-MM-dd HH:mm')"
git commit -m $deployMessage
git push

if ($LASTEXITCODE -eq 0) {
    Write-Host "[deploy] Successfully deployed to git!" -ForegroundColor Green
    Write-Host "[deploy] Site should be available at: https://gfermoto.github.io/soil-sensor-7in1/" -ForegroundColor Cyan
} else {
    Write-Host "[deploy] Git push failed!" -ForegroundColor Red
    exit 1
}

Write-Host "[deploy] Full deployment completed!" -ForegroundColor Green 