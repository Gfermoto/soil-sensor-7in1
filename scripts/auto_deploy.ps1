[Console]::OutputEncoding = [System.Text.Encoding]::UTF8
# Автоматический деплой документации JXCT (без ожидания ввода)

param(
    [string]$CommitMessage,
    [switch]$SkipTests,
    [switch]$Force
)

# Устанавливаем значения по умолчанию
if (-not $CommitMessage) {
    $CommitMessage = "Auto-deploy documentation $(Get-Date -Format 'yyyy-MM-dd HH:mm')"
}

Write-Host "[auto-deploy] 🚀 Starting automatic deployment..." -ForegroundColor Cyan

# Проверяем статус git
$gitStatus = git status --porcelain
if ($gitStatus) {
    Write-Host "[auto-deploy] 📝 Uncommitted changes detected, auto-committing..." -ForegroundColor Yellow
    Write-Host $gitStatus -ForegroundColor Gray
    
    git add .
    git commit -m $CommitMessage
    if ($LASTEXITCODE -eq 0) {
        Write-Host "[auto-deploy] ✅ Changes committed automatically" -ForegroundColor Green
    } else {
        Write-Host "[auto-deploy] ❌ Failed to commit changes" -ForegroundColor Red
        exit 1
    }
}

# Генерируем документацию
Write-Host "[auto-deploy] 📚 Generating documentation..." -ForegroundColor Cyan
& "$PSScriptRoot\gen_docs.ps1"
if ($LASTEXITCODE -ne 0) {
    Write-Host "[auto-deploy] ❌ Documentation generation failed!" -ForegroundColor Red
    exit 1
}

# Проверяем, что сайт собрался
if (-not (Test-Path "site/index.html")) {
    Write-Host "[auto-deploy] ❌ Site build failed - index.html not found!" -ForegroundColor Red
    exit 1
}

Write-Host "[auto-deploy] ✅ Documentation generated successfully" -ForegroundColor Green

# Пуш в git
Write-Host "[auto-deploy] 📤 Pushing to git..." -ForegroundColor Cyan
git add site/
$deployMessage = "Auto-deploy documentation $(Get-Date -Format 'yyyy-MM-dd HH:mm')"
git commit -m $deployMessage
git push

if ($LASTEXITCODE -eq 0) {
    Write-Host "[auto-deploy] ✅ Successfully deployed to git!" -ForegroundColor Green
    Write-Host "[auto-deploy] 🌐 Site available at: https://gfermoto.github.io/soil-sensor-7in1/" -ForegroundColor Cyan
} else {
    Write-Host "[auto-deploy] ❌ Git push failed!" -ForegroundColor Red
    exit 1
}

Write-Host "[auto-deploy] 🎉 Automatic deployment completed!" -ForegroundColor Green 