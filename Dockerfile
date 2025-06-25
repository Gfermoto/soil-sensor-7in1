FROM python:3.11-slim

# ---------------- БАЗОВЫЕ УТИЛИТЫ + Ninja ------------------
RUN apt-get update && apt-get install -y \
    git curl wget gnupg lsb-release \
    build-essential cmake ninja-build \
    doxygen graphviz \
    && rm -rf /var/lib/apt/lists/*

# ---------------- LLVM 17 + IWYU ---------------------------
# 1. Подключаем официальный репозиторий LLVM для Debian bookworm
# 2. Ставим clang-17, clang-tidy-17 и include-what-you-use (зависит от LLVM)
#    IWYU всегда компилируется под последнюю версию LLVM в репозитории.
RUN echo "deb http://apt.llvm.org/$(lsb_release -cs)/ llvm-toolchain-$(lsb_release -cs)-17 main" > /etc/apt/sources.list.d/llvm.list && \
    wget -qO - https://apt.llvm.org/llvm-snapshot.gpg.key | tee /usr/share/keyrings/llvm.asc && \
    apt-get update && \
    apt-get install -y clang-17 clang-tidy-17 include-what-you-use-17 && \
    ln -s /usr/bin/clang-tidy-17 /usr/local/bin/clang-tidy && \
    ln -s /usr/bin/include-what-you-use-17 /usr/local/bin/include-what-you-use && \
    rm -rf /var/lib/apt/lists/*

# ---------------- Python зависимости -----------------------
RUN pip3 install --no-cache-dir platformio mkdocs-material pymdown-extensions

# ---------------- Прочее -----------------------------------
WORKDIR /workspace

COPY platformio.ini ./

RUN pio pkg install

CMD ["pio", "run"] 