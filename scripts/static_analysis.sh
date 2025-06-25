#!/bin/bash

# Статический анализ с clang-tidy
echo "Running clang-tidy analysis..."
find src include -name "*.cpp" -o -name "*.h" | xargs clang-tidy \
  -checks=modernize-*,performance-*,readability-*,bugprone-* \
  -header-filter=".*" \
  -- -I include -I src -std=c++17

# Анализ зависимостей с IWYU
echo "Running IWYU analysis..."
find src include -name "*.cpp" -o -name "*.h" | head -5 | xargs include-what-you-use \
  -I include -I src -std=c++17 2>&1 | grep -E "(should add|should remove|is not used)" || true 