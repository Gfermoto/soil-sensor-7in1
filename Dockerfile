# JXCT Soil Sensor Development Environment
# Docker контейнер для инструментов разработки ESP32 проекта

FROM ubuntu:22.04

# Установка базовых пакетов
RUN apt-get update && apt-get install -y \
    git \
    curl \
    wget \
    python3 \
    python3-pip \
    python3-venv \
    build-essential \
    cmake \
    ninja-build \
    clang \
    clang-tidy \
    clang-format \
    cppcheck \
    valgrind \
    doxygen \
    graphviz \
    nodejs \
    npm \
    && rm -rf /var/lib/apt/lists/*

# Установка PlatformIO
RUN pip3 install platformio

# Установка Python зависимостей для тестирования
COPY requirements.txt /tmp/
RUN pip3 install -r /tmp/requirements.txt

# Установка MkDocs и темы
RUN pip3 install mkdocs mkdocs-material

# Создание рабочей директории
WORKDIR /workspace

# Копирование проекта
COPY . .

# Установка pre-commit hooks
RUN pip3 install pre-commit && pre-commit install

# Настройка переменных окружения
ENV PLATFORMIO_HOME_DIR=/workspace/.pio
ENV PATH="/workspace/.pio/penv/bin:$PATH"

# Проверка установки инструментов
RUN echo "=== Проверка инструментов ===" && \
    clang-tidy --version && \
    clang-format --version && \
    cppcheck --version && \
    doxygen --version && \
    pio --version && \
    python3 --version && \
    echo "✅ Все инструменты установлены!"

# Создание точки входа
ENTRYPOINT ["/bin/bash"]
CMD ["-c", "echo 'JXCT Soil Sensor Development Environment' && echo 'Доступные команды:' && echo '- pio run -e esp32dev' && echo '- python scripts/ultra_quick_test.py' && echo '- python scripts/run_simple_tests.py' && echo '- python scripts/run_clang_tidy_analysis.py' && bash"] 