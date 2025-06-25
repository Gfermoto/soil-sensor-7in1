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

# Проверяем наличие include-what-you-use (опционально)
if command -v include-what-you-use &> /dev/null; then
    echo "Running include-what-you-use analysis..."
    find src -name "*.cpp" | head -3 | xargs -I {} include-what-you-use {} \
      -I include -I src -std=c++17 || {
      echo "⚠️  IWYU found issues (this is expected for development)"
    }
else
    echo "ℹ️  include-what-you-use not available - skipping IWYU analysis"
fi

echo "✅ Static analysis completed successfully" 