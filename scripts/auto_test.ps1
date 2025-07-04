[Console]::OutputEncoding = [System.Text.Encoding]::UTF8
# Автоматический запуск всех тестов JXCT (без ожидания ввода)

param(
    [switch]$SkipE2E = $false,
    [switch]$SkipPerformance = $false,
    [switch]$GenerateReport = $true,
    [string]$OutputDir = "test_reports"
)

Write-Host "[auto-test] 🧪 Starting automatic testing..." -ForegroundColor Cyan

# Создаем папку для отчетов
if (-not (Test-Path $OutputDir)) {
    New-Item -ItemType Directory -Path $OutputDir | Out-Null
}

$testResults = @()
$overallSuccess = $true

# 1. Простые тесты
Write-Host "[auto-test] 📋 Running simple tests..." -ForegroundColor Yellow
try {
    & "$PSScriptRoot\run_simple_tests.py"
    if ($LASTEXITCODE -eq 0) {
        Write-Host "[auto-test] ✅ Simple tests passed" -ForegroundColor Green
        $testResults += "Simple tests: PASSED"
    } else {
        Write-Host "[auto-test] ❌ Simple tests failed" -ForegroundColor Red
        $testResults += "Simple tests: FAILED"
        $overallSuccess = $false
    }
} catch {
    Write-Host "[auto-test] ❌ Simple tests error: $_" -ForegroundColor Red
    $testResults += "Simple tests: ERROR"
    $overallSuccess = $false
}

# 2. E2E тесты (если не пропущены)
if (-not $SkipE2E) {
    Write-Host "[auto-test] 🔄 Running E2E tests..." -ForegroundColor Yellow
    try {
        & "$PSScriptRoot\run_e2e_tests.py"
        if ($LASTEXITCODE -eq 0) {
            Write-Host "[auto-test] ✅ E2E tests passed" -ForegroundColor Green
            $testResults += "E2E tests: PASSED"
        } else {
            Write-Host "[auto-test] ❌ E2E tests failed" -ForegroundColor Red
            $testResults += "E2E tests: FAILED"
            $overallSuccess = $false
        }
    } catch {
        Write-Host "[auto-test] ❌ E2E tests error: $_" -ForegroundColor Red
        $testResults += "E2E tests: ERROR"
        $overallSuccess = $false
    }
} else {
    Write-Host "[auto-test] ⏭️ Skipping E2E tests" -ForegroundColor Gray
    $testResults += "E2E tests: SKIPPED"
}

# 3. Performance тесты (если не пропущены)
if (-not $SkipPerformance) {
    Write-Host "[auto-test] ⚡ Running performance tests..." -ForegroundColor Yellow
    try {
        & "$PSScriptRoot\run_performance_tests.py"
        if ($LASTEXITCODE -eq 0) {
            Write-Host "[auto-test] ✅ Performance tests passed" -ForegroundColor Green
            $testResults += "Performance tests: PASSED"
        } else {
            Write-Host "[auto-test] ❌ Performance tests failed" -ForegroundColor Red
            $testResults += "Performance tests: FAILED"
            $overallSuccess = $false
        }
    } catch {
        Write-Host "[auto-test] ❌ Performance tests error: $_" -ForegroundColor Red
        $testResults += "Performance tests: ERROR"
        $overallSuccess = $false
    }
} else {
    Write-Host "[auto-test] ⏭️ Skipping performance tests" -ForegroundColor Gray
    $testResults += "Performance tests: SKIPPED"
}

# 4. Comprehensive тесты
Write-Host "[auto-test] 🔍 Running comprehensive tests..." -ForegroundColor Yellow
try {
    & "$PSScriptRoot\run_comprehensive_tests.py"
    if ($LASTEXITCODE -eq 0) {
        Write-Host "[auto-test] ✅ Comprehensive tests passed" -ForegroundColor Green
        $testResults += "Comprehensive tests: PASSED"
    } else {
        Write-Host "[auto-test] ❌ Comprehensive tests failed" -ForegroundColor Red
        $testResults += "Comprehensive tests: FAILED"
        $overallSuccess = $false
    }
} catch {
    Write-Host "[auto-test] ❌ Comprehensive tests error: $_" -ForegroundColor Red
    $testResults += "Comprehensive tests: ERROR"
    $overallSuccess = $false
}

# Генерируем отчет
if ($GenerateReport) {
    $reportFile = Join-Path $OutputDir "auto_test_report_$(Get-Date -Format 'yyyyMMdd_HHmmss').txt"
    $reportContent = @"
JXCT Auto Test Report
Generated: $(Get-Date -Format 'yyyy-MM-dd HH:mm:ss')
Overall Status: $(if ($overallSuccess) { "PASSED" } else { "FAILED" })

Test Results:
$($testResults -join "`n")

Summary:
- Total tests run: $($testResults.Count)
- Passed: $(($testResults | Where-Object { $_ -like "*PASSED*" }).Count)
- Failed: $(($testResults | Where-Object { $_ -like "*FAILED*" }).Count)
- Errors: $(($testResults | Where-Object { $_ -like "*ERROR*" }).Count)
- Skipped: $(($testResults | Where-Object { $_ -like "*SKIPPED*" }).Count)
"@
    
    $reportContent | Out-File -FilePath $reportFile -Encoding UTF8
    Write-Host "[auto-test] 📊 Report saved to: $reportFile" -ForegroundColor Cyan
}

# Финальный результат
if ($overallSuccess) {
    Write-Host "[auto-test] 🎉 All tests completed successfully!" -ForegroundColor Green
    exit 0
} else {
    Write-Host "[auto-test] ❌ Some tests failed!" -ForegroundColor Red
    exit 1
} 