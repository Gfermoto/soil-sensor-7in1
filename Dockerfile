FROM python:3.11-slim

# Установка системных зависимостей
RUN apt-get update && apt-get install -y \
    git \
    curl \
    build-essential \
    cmake \
    doxygen \
    graphviz \
    && rm -rf /var/lib/apt/lists/*

# Установка PlatformIO
RUN pip3 install platformio

# Установка Python-зависимостей для документации
RUN pip3 install mkdocs-material pymdown-extensions

# Добавление LLVM репозитория и установка clang-tidy
ARG DEBIAN_CODENAME=bookworm

RUN wget -qO - https://apt.llvm.org/llvm-snapshot.gpg.key | gpg --dearmor | tee /usr/share/keyrings/llvm.gpg > /dev/null && \
    echo "deb [signed-by=/usr/share/keyrings/llvm.gpg] http://apt.llvm.org/${DEBIAN_CODENAME}/ llvm-toolchain-${DEBIAN_CODENAME}-17 main" > /etc/apt/sources.list.d/llvm.list && \
    apt-get update && \
    apt-get install -y clang-17 clang-tidy-17 && \
    ln -s /usr/bin/clang-tidy-17 /usr/local/bin/clang-tidy && \
    rm -rf /var/lib/apt/lists/*

# Рабочая директория
WORKDIR /workspace

# Копирование конфигурации PlatformIO
COPY platformio.ini ./

# Установка зависимостей проекта
RUN pio pkg install

# Команда по умолчанию
CMD ["pio", "run"] 