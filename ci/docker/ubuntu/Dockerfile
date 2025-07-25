FROM ubuntu:noble

RUN apt update && apt install -y libglib2.0-bin libxkbcommon0 libdbus-1-3 \
    freeglut3-dev lcov \
    build-essential cmake \
    fuse

RUN apt update && apt install -y wget unzip git python3-jinja2 python3-pip
RUN pip install aqtinstall --break-system-packages

RUN aqt install-qt -O /opt/qt linux desktop 6.8.2 -m qt3d

RUN wget http://sonarcloud.io/static/cpp/build-wrapper-linux-x86.zip
RUN unzip build-wrapper-linux-x86.zip -d /opt

RUN wget https://binaries.sonarsource.com/Distribution/sonar-scanner-cli/sonar-scanner-cli-6.2.1.4610-linux-x64.zip
RUN unzip sonar-scanner-cli-6.2.1.4610-linux-x64.zip -d /opt

RUN wget https://github.com/google/or-tools/releases/download/v9.10/or-tools_amd64_ubuntu-24.04_cpp_v9.10.4067.tar.gz
RUN tar -C /opt --strip-components=1 -xvf or-tools_amd64_ubuntu-24.04_cpp_v9.10.4067.tar.gz

RUN git config --global --add safe.directory '*'
