FROM ubuntu:noble

ARG QT_VERSION=6.10.0

RUN apt update && apt install -y ca-certificates gnupg wget

RUN wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | \
    gpg --dearmor - | \
    tee /usr/share/keyrings/kitware-archive-keyring.gpg >/dev/null

RUN echo 'deb [signed-by=/usr/share/keyrings/kitware-archive-keyring.gpg] https://apt.kitware.com/ubuntu/ noble main' | \
    tee /etc/apt/sources.list.d/kitware.list >/dev/null

RUN apt update && apt install -y libglib2.0-bin libdbus-1-3 libgtk-3-0 \
    libxcb* libxkbcommon* \
    freeglut3-dev \
    build-essential cmake ninja-build \
    fuse file

RUN apt install -y software-properties-common && \
    add-apt-repository -y ppa:ubuntu-toolchain-r/test && \
    apt update && apt install -y libstdc++-15-dev

RUN wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key | \
    gpg --dearmor - | \
    tee /usr/share/keyrings/llvm-archive-keyring.gpg >/dev/null

RUN echo 'deb [signed-by=/usr/share/keyrings/llvm-archive-keyring.gpg] https://apt.llvm.org/noble/ llvm-toolchain-noble-20 main' | \
    tee /etc/apt/sources.list.d/llvm.list >/dev/null

RUN apt update && apt install -y clang-20 llvm-20

RUN apt update && apt install -y wget unzip git patch python3-jinja2 python3-pip
RUN pip install aqtinstall gcovr --break-system-packages

RUN aqt install-qt -O /opt/qt linux desktop ${QT_VERSION} \
    --archives icu qtbase qtdeclarative qtsvg qttools \
    --modules qt3d qtshadertools

COPY patches.patch .
RUN patch -p0 -d "/opt/qt/${QT_VERSION}/gcc_64/include" < patches.patch

ENV QTDIR="/opt/qt/${QT_VERSION}/gcc_64"
ENV PATH="${QTDIR}/bin:$PATH"
ENV LD_LIBRARY_PATH="${QTDIR}/lib:$LD_LIBRARY_PATH"

RUN wget http://sonarcloud.io/static/cpp/build-wrapper-linux-x86.zip
RUN unzip build-wrapper-linux-x86.zip -d /opt

RUN wget https://binaries.sonarsource.com/Distribution/sonar-scanner-cli/sonar-scanner-cli-6.2.1.4610-linux-x64.zip
RUN unzip sonar-scanner-cli-6.2.1.4610-linux-x64.zip -d /opt

RUN wget https://github.com/google/or-tools/releases/download/v9.10/or-tools_amd64_ubuntu-24.04_cpp_v9.10.4067.tar.gz
RUN tar -C /opt --strip-components=1 -xvf or-tools_amd64_ubuntu-24.04_cpp_v9.10.4067.tar.gz

RUN git config --global --add safe.directory '*'
