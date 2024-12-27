FROM ubuntu:noble

RUN apt update && apt install -y qtbase5-dev qt3d5-dev libqt5svg5-dev \
    freeglut3-dev lcov \
    build-essential cmake

RUN apt update && apt install -y wget unzip git python3-jinja2

RUN wget http://sonarcloud.io/static/cpp/build-wrapper-linux-x86.zip
RUN unzip build-wrapper-linux-x86.zip -d /opt

RUN wget https://binaries.sonarsource.com/Distribution/sonar-scanner-cli/sonar-scanner-cli-6.2.1.4610-linux-x64.zip
RUN unzip sonar-scanner-cli-6.2.1.4610-linux-x64.zip -d /opt

RUN wget https://github.com/google/or-tools/releases/download/v9.10/or-tools_amd64_ubuntu-24.04_cpp_v9.10.4067.tar.gz
RUN tar -C /opt --strip-components=1 -xvf or-tools_amd64_ubuntu-24.04_cpp_v9.10.4067.tar.gz

RUN git config --global --add safe.directory '*'