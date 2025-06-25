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
# IWYU нет в стандартных репозиториях Debian. Подключаем apt.llvm.org и
# ставим пакет для LLVM 15.
RUN echo "deb http://apt.llvm.org/$(lsb_release -cs)/ llvm-toolchain-$(lsb_release -cs)-15 main" \
      > /etc/apt/sources.list.d/llvm.list && \
    wget -qO - https://apt.llvm.org/llvm-snapshot.gpg.key | apt-key add - && \
    apt-get update && apt-get install -y include-what-you-use-15 && \
    ln -s /usr/bin/include-what-you-use-15 /usr/local/bin/include-what-you-use && \
    rm -rf /var/lib/apt/lists/*

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