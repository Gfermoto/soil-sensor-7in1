#!/bin/bash

echo "🔨 Пересборка Docker образа с инструментами статического анализа..."

# Сборка нового образа
docker build -t ghcr.io/gfermoto/soil-sensor-7in1:main .

# Тестирование образа
echo "🧪 Тестирование образа..."
docker run --rm ghcr.io/gfermoto/soil-sensor-7in1:main which clang-tidy
docker run --rm ghcr.io/gfermoto/soil-sensor-7in1:main which include-what-you-use

echo "✅ Docker образ пересобран успешно!"
echo "📤 Для публикации выполните: docker push ghcr.io/gfermoto/soil-sensor-7in1:main" # ci: trigger rebuild workflow
