FROM python:3.11-slim

# Установка системных зависимостей
RUN apt-get update && apt-get install -y \
    git \
    curl \
    build-essential \
    cmake \
    doxygen \
    graphviz \
    clang-tidy \
    clang-tools \
    && rm -rf /var/lib/apt/lists/*

# Установка include-what-you-use
RUN apt-get update && apt-get install -y \
    include-what-you-use \
    && rm -rf /var/lib/apt/lists/*

# Установка PlatformIO
RUN pip3 install platformio

# Установка Python-зависимостей для документации
RUN pip3 install mkdocs-material pymdown-extensions

# Рабочая директория
WORKDIR /workspace

# Копирование конфигурации PlatformIO
COPY platformio.ini ./

# Установка зависимостей проекта
RUN pio pkg install

# Команда по умолчанию
CMD ["pio", "run"] 