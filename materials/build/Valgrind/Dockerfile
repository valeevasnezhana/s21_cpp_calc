FROM ubuntu:22.04

RUN apt-get -y update; apt-get -y install valgrind; apt-get -y install g++; apt-get -y install make; apt-get -y install libgtest-dev; apt-get -y install cmake; apt-get -y install qt6-base-dev; apt-get -y install build-essential libgl1-mesa-dev; mkdir -p /usr/src/

WORKDIR /usr/src/

COPY ./src /usr/src/
COPY ./materials/build/Valgrind/start.sh /usr/src

USER root

ENTRYPOINT ["/bin/bash", "start.sh"]

