FROM python:3.11-slim

# ---------------- БАЗОВЫЕ УТИЛИТЫ + Ninja ------------------
RUN apt-get update && apt-get install -y \
    git curl wget gnupg lsb-release \
    build-essential cmake ninja-build \
    doxygen graphviz \
    && rm -rf /var/lib/apt/lists/*

# ---------------- LLVM 17 + IWYU ---------------------------
ARG DEBIAN_CODENAME=bookworm

RUN wget -qO - https://apt.llvm.org/llvm-snapshot.gpg.key | gpg --dearmor | tee /usr/share/keyrings/llvm.gpg > /dev/null && \
    echo "deb [signed-by=/usr/share/keyrings/llvm.gpg] http://apt.llvm.org/${DEBIAN_CODENAME}/ llvm-toolchain-${DEBIAN_CODENAME}-17 main" > /etc/apt/sources.list.d/llvm.list && \
    apt-get update && \
    apt-get install -y clang-17 clang-tidy-17 && \
    ln -s /usr/bin/clang-tidy-17 /usr/local/bin/clang-tidy && \
    rm -rf /var/lib/apt/lists/*

# ---------------- Python зависимости -----------------------
RUN pip3 install --no-cache-dir platformio mkdocs-material pymdown-extensions

# ---------------- Прочее -----------------------------------
WORKDIR /workspace

COPY platformio.ini ./

RUN pio pkg install

CMD ["pio", "run"] 