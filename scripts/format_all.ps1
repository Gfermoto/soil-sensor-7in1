# Скрипт для форматирования всех исходных файлов C++/Arduino в проекте
$clangFormatPath = "C:\Program Files\LLVM\bin\clang-format.exe"
$files = Get-ChildItem -Recurse -Include *.cpp,*.h,*.ino
foreach ($file in $files) {
    & "$clangFormatPath" -i $file.FullName
}
Write-Host "Форматирование завершено." 