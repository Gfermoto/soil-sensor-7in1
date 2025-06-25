FROM python:3.11-slim

# Установка системных зависимостей и clang-tidy
RUN apt-get update && apt-get install -y \
    git \
    curl \
    build-essential \
    cmake \
    doxygen \
    graphviz \
    clang-tidy \
    clang-tools \
    wget \
    gnupg \
    lsb-release \
    && rm -rf /var/lib/apt/lists/*

# ---- include-what-you-use ----------------------------------------------------
# IWYU нет в репозиториях Debian bookworm. Сборка из исходников (~3-4 мин).
RUN apt-get update && apt-get install -y ninja-build libclang-15-dev git && \
    git clone --depth 1 https://github.com/include-what-you-use/include-what-you-use /tmp/iwyu && \
    cd /tmp/iwyu && cmake -GNinja -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_C_COMPILER=clang . && \
    ninja -j$(nproc) && ninja install && \
    cd / && rm -rf /tmp/iwyu /var/lib/apt/lists/*

# ----------------------------------------------------------------------------

# Установка PlatformIO
RUN pip3 install --no-cache-dir platformio

# Установка Python-зависимостей для документации
RUN pip3 install --no-cache-dir mkdocs-material pymdown-extensions

# Рабочая директория
WORKDIR /workspace

# Копирование конфигурации PlatformIO
COPY platformio.ini ./

# Установка зависимостей проекта
RUN pio pkg install

# Команда по умолчанию
CMD ["pio", "run"] 