[Console]::OutputEncoding = [System.Text.Encoding]::UTF8
# Doxygen documentation generation for JXCT project

# Check for Doxyfile
if (!(Test-Path "Doxyfile")) {
    Write-Host "[gen_docs] Doxyfile not found!" -ForegroundColor Red
    exit 1
}

# Check for doxygen in PATH
$doxygen = Get-Command doxygen -ErrorAction SilentlyContinue
if (-not $doxygen) {
    Write-Host "[gen_docs] Doxygen not found in PATH. Please install Doxygen and add it to PATH." -ForegroundColor Red
    exit 1
}

# Сначала генерируем Doxygen
Write-Host "[gen_docs] Generating Doxygen documentation..." -ForegroundColor Cyan
$doxyResult = doxygen Doxyfile
if ($LASTEXITCODE -ne 0) {
    Write-Host "[gen_docs] Doxygen generation failed!" -ForegroundColor Red
    exit 1
}
Write-Host "[gen_docs] Doxygen documentation successfully created in site/api/html" -ForegroundColor Green

# Теперь собираем MkDocs (Doxygen уже есть)
Write-Host "[gen_docs] Building MkDocs site..." -ForegroundColor Cyan
mkdocs build --clean
if ($LASTEXITCODE -ne 0) {
    Write-Host "[gen_docs] MkDocs build failed!" -ForegroundColor Red
    exit 1
}
Write-Host "[gen_docs] MkDocs site built successfully" -ForegroundColor Green

# Проверяем, что Doxygen остался на месте
if (Test-Path "site/api/html/index.html") {
    Write-Host "[gen_docs] Doxygen documentation verified in site/api/html" -ForegroundColor Green
} else {
    Write-Host "[gen_docs] WARNING: Doxygen documentation not found after MkDocs build!" -ForegroundColor Yellow
    # Копируем обратно, если MkDocs удалил
    if (Test-Path "site/api/html") {
        Remove-Item -Recurse -Force "site/api/html"
    }
    New-Item -ItemType Directory -Force -Path "site/api/html" | Out-Null
    Copy-Item -Recurse -Force "site/api/html/*" "site/api/html/" | Out-Null
    Write-Host "[gen_docs] Doxygen documentation restored" -ForegroundColor Green
} 