FROM ubuntu:jammy

ARG DEBIAN_FRONTEND=noninteractive

RUN apt-get update && \
	apt-get -y install lcov git wget cmake build-essential python3-pip && \
	apt-get -y install qtbase5-dev qt3d5-dev libqt5svg5-dev freeglut3-dev

RUN pip install jinja2

RUN wget https://github.com/google/or-tools/releases/download/v9.7/or-tools_amd64_ubuntu-22.04_cpp_v9.7.2996.tar.gz
RUN tar -xvf or-tools_amd64_ubuntu-22.04_cpp_v9.7.2996.tar.gz --strip-components=1 -C /usr/local/
