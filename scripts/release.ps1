# JXCT Release Manager - PowerShell wrapper
# Только обёртка для scripts/release_manager.py

param(
    [Parameter(Mandatory=$true)]
    [ValidateSet("version", "release", "bump", "sync")]
    [string]$Command,
    
    [Parameter(Mandatory=$false)]
    [string]$Version,
    
    [Parameter(Mandatory=$false)]
    [ValidateSet("major", "minor", "patch")]
    [string]$Type,
    
    [Parameter(Mandatory=$false)]
    [string]$Message,
    
    [Parameter(Mandatory=$false)]
    [switch]$NoPush,
    
    [Parameter(Mandatory=$false)]
    [switch]$NoCommit,
    
    [Parameter(Mandatory=$false)]
    [string]$ProjectDir
)

# Проверка Python и Git
function Test-Python {
    try { python --version | Out-Null; return $true } catch { return $false }
}
function Test-Git {
    try { git --version | Out-Null; return $true } catch { return $false }
}

if (-not (Test-Python)) { 
    Write-Host "❌ Python не найден. Установите Python и добавьте в PATH" -ForegroundColor Red
    exit 1 
}
if (-not (Test-Git)) { 
    Write-Host "❌ Git не найден. Установите Git и добавьте в PATH" -ForegroundColor Red
    exit 1 
}

# Формируем аргументы
$args = @($Command)
if ($Version) { $args += '--version'; $args += $Version }
if ($Type) { $args += '--type'; $args += $Type }
if ($Message) { $args += '--message'; $args += $Message }
if ($NoPush) { $args += '--no-push' }
if ($NoCommit) { $args += '--no-commit' }
if ($ProjectDir) { $args += '--project-dir'; $args += $ProjectDir }

# Выводим команду для отладки
Write-Host "🚀 Выполняю: python scripts/release_manager.py $($args -join ' ')" -ForegroundColor Cyan

# Запускаем скрипт
python scripts/release_manager.py @args 