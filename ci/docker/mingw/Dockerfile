FROM ubuntu:noble

ARG QT_VERSION=6.10.0
# Pinned to the last release bundling LLVM 20, matching clang-20 used on Linux
ARG LLVM_MINGW_VERSION=20250709

RUN apt update && apt install -y ca-certificates gnupg wget

RUN wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | \
    gpg --dearmor - | \
    tee /usr/share/keyrings/kitware-archive-keyring.gpg >/dev/null

RUN echo 'deb [signed-by=/usr/share/keyrings/kitware-archive-keyring.gpg] https://apt.kitware.com/ubuntu/ noble main' | \
    tee /etc/apt/sources.list.d/kitware.list >/dev/null

# Host Qt tools (moc, rcc...) are linked against these
RUN apt update && apt install -y libglib2.0-0t64 libdbus-1-3 libxkbcommon0 libgl1 \
    cmake ninja-build xz-utils file

RUN wget https://github.com/mstorsjo/llvm-mingw/releases/download/${LLVM_MINGW_VERSION}/llvm-mingw-${LLVM_MINGW_VERSION}-ucrt-ubuntu-22.04-x86_64.tar.xz && \
    mkdir /opt/llvm-mingw && \
    tar -C /opt/llvm-mingw --strip-components=1 -xf llvm-mingw-${LLVM_MINGW_VERSION}-ucrt-ubuntu-22.04-x86_64.tar.xz && \
    rm llvm-mingw-${LLVM_MINGW_VERSION}-ucrt-ubuntu-22.04-x86_64.tar.xz

RUN apt update && apt install -y wget unzip git patch python3-jinja2 python3-pip
RUN pip install aqtinstall --break-system-packages

RUN aqt install-qt -O /opt/qt linux desktop ${QT_VERSION} linux_gcc_64 \
    --archives icu qtbase qtdeclarative qtsvg qttools \
    --modules qt3d qtshadertools

RUN aqt install-qt -O /opt/qt windows desktop ${QT_VERSION} win64_llvm_mingw \
    --archives qtbase qtdeclarative qtsvg qttools d3dcompiler_47 opengl32sw \
    --modules qt3d qtshadertools

COPY patches.patch .
# Windows headers are CRLF, which patch refuses to match against an LF patch
RUN sed -i 's/\r$//' "/opt/qt/${QT_VERSION}/llvm-mingw_64/include/QtCore/qtmetamacros.h" && \
    patch -p0 -d "/opt/qt/${QT_VERSION}/gcc_64/include" < patches.patch && \
    patch -p0 -d "/opt/qt/${QT_VERSION}/llvm-mingw_64/include" < patches.patch

ENV LLVM_MINGW="/opt/llvm-mingw"
ENV QT_HOST_PATH="/opt/qt/${QT_VERSION}/gcc_64"
ENV PATH="${LLVM_MINGW}/bin:${QT_HOST_PATH}/bin:$PATH"

RUN git config --global --add safe.directory '*'
