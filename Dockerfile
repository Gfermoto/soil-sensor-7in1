FROM platformio/platformio:latest

# Установка дополнительных зависимостей
RUN apt-get update && apt-get install -y \
    doxygen \
    graphviz \
    python3-pip \
    && rm -rf /var/lib/apt/lists/*

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