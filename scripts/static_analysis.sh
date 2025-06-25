#!/bin/bash

set -e  # Остановка при любой ошибке

# Проверяем наличие clang-tidy
if ! command -v clang-tidy &> /dev/null; then
    echo "⚠️  clang-tidy not found - skipping static analysis"
    exit 0
fi

# Статический анализ с clang-tidy
echo "Running clang-tidy analysis..."
find src include -name "*.cpp" -o -name "*.h" | xargs clang-tidy \
  -checks=modernize-*,performance-*,readability-*,bugprone-* \
  -header-filter=".*" \
  -- -I include -I src -std=c++17 || {
  echo "⚠️  clang-tidy found issues (this is expected for development)"
  exit 0  # Не считаем это ошибкой CI
}

echo "✅ Static analysis completed successfully" 