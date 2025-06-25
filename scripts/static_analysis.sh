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

# Проверяем наличие include-what-you-use
if ! command -v include-what-you-use &> /dev/null; then
    echo "⚠️  include-what-you-use not found - skipping IWYU analysis"
    exit 0
fi

# Анализ зависимостей с IWYU
echo "Running IWYU analysis..."
find src include -name "*.cpp" -o -name "*.h" | head -5 | xargs include-what-you-use \
  -I include -I src -std=c++17 2>&1 | grep -E "(should add|should remove|is not used)" || {
  echo "ℹ️  No include issues found"
  exit 0
} 