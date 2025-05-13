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

Write-Host "[gen_docs] Generating documentation..." -ForegroundColor Cyan
$doxyResult = doxygen Doxyfile
if ($LASTEXITCODE -ne 0) {
    Write-Host "[gen_docs] Documentation generation failed!" -ForegroundColor Red
    exit 1
}
Write-Host "[gen_docs] Documentation successfully created in docs\html" -ForegroundColor Green 