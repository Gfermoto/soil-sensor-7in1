# Code Formatter for JXCT v2.0 Project
# Automatically formats all C++/Arduino source files using clang-format

# Set UTF-8 encoding for proper display
[Console]::OutputEncoding = [System.Text.Encoding]::UTF8
$OutputEncoding = [System.Text.Encoding]::UTF8

# Color output functions
function Write-Success {
    param([string]$Message)
    Write-Host $Message -ForegroundColor Green
}

function Write-Info {
    param([string]$Message)
    Write-Host $Message -ForegroundColor Cyan
}

function Write-Warning {
    param([string]$Message)
    Write-Host $Message -ForegroundColor Yellow
}

function Write-Error-Custom {
    param([string]$Message)
    Write-Host $Message -ForegroundColor Red
}

# Header
Write-Host ""
Write-Info "=== JXCT Code Formatter v2.0 ==="
Write-Info "Formatting project source code..."
Write-Host ""

# Find clang-format
$clangFormatPaths = @(
    "C:\Program Files\LLVM\bin\clang-format.exe",
    "C:\Program Files (x86)\LLVM\bin\clang-format.exe",
    "clang-format.exe"
)

$clangFormatPath = $null
foreach ($path in $clangFormatPaths) {
    if ((Get-Command $path -ErrorAction SilentlyContinue) -or (Test-Path $path)) {
        $clangFormatPath = $path
        break
    }
}

if (-not $clangFormatPath) {
    Write-Error-Custom "ERROR: clang-format not found!"
    Write-Warning "Please install LLVM/Clang or add clang-format to PATH"
    Write-Host "Download: https://llvm.org/builds/"
    exit 1
}

Write-Success "Found clang-format: $clangFormatPath"

# Find source files
Write-Info "Searching for source files..."

$sourceExtensions = @("*.cpp", "*.h", "*.ino", "*.c", "*.hpp")
$excludePaths = @("*\.pio\*", "*\docs\*", "*\.git\*", "*\libdeps\*")

$allFiles = @()
foreach ($ext in $sourceExtensions) {
    $files = Get-ChildItem -Recurse -Include $ext -File
    foreach ($file in $files) {
        $shouldExclude = $false
        foreach ($excludePath in $excludePaths) {
            if ($file.FullName -like $excludePath) {
                $shouldExclude = $true
                break
            }
        }
        if (-not $shouldExclude) {
            $allFiles += $file
        }
    }
}

if ($allFiles.Count -eq 0) {
    Write-Warning "No files found for formatting"
    exit 0
}

Write-Success "Found files: $($allFiles.Count)"

# Show file list
Write-Info "`nFiles to format:"
foreach ($file in $allFiles) {
    $relativePath = $file.FullName.Replace((Get-Location).Path + "\", "")
    Write-Host "   • $relativePath" -ForegroundColor Gray
}

Write-Host ""

# Format files
Write-Info "Starting formatting..."
$processedCount = 0
$errorCount = 0

foreach ($file in $allFiles) {
    $relativePath = $file.FullName.Replace((Get-Location).Path + "\", "")
    
    try {
        Write-Host "   Formatting: $relativePath" -NoNewline
        
        # Execute formatting
        $result = & $clangFormatPath -i $file.FullName 2>&1
        
        if ($LASTEXITCODE -eq 0) {
            Write-Host " OK" -ForegroundColor Green
            $processedCount++
        } else {
            Write-Host " ERROR" -ForegroundColor Red
            Write-Warning "     Error: $result"
            $errorCount++
        }
    }
    catch {
        Write-Host " ERROR" -ForegroundColor Red
        Write-Warning "     Exception: $($_.Exception.Message)"
        $errorCount++
    }
}

# Final report
Write-Host ""
Write-Info "=== FORMATTING REPORT ==="
Write-Success "Successfully processed: $processedCount files"

if ($errorCount -gt 0) {
    Write-Warning "Errors: $errorCount files"
} else {
    Write-Success "All files formatted successfully!"
}

Write-Info "Using configuration: .clang-format"
Write-Host ""

# Commit suggestion
if ($processedCount -gt 0) {
    Write-Info "Recommendation: Check changes and commit:"
    Write-Host "   git add ." -ForegroundColor Gray
    Write-Host "   git commit -m `"Code formatting with clang-format`"" -ForegroundColor Gray
}

Write-Success "Formatting completed!" 