# -*- mode: dockerfile-mode -*-
FROM ubuntu:18.04
ENV DEBIAN_FRONTEND noninteractive
MAINTAINER Henrik Austad <henrik.austad@sintef.no>

# Install core dependencies
RUN apt-get update
RUN apt-get install -y gcc g++ cmake antlr3 libzmq3-dev \
    libboost-dev libboost-test-dev libboost-log-dev libboost-program-options-dev \
    swig python3-dev python3-pip libopencv-dev
RUN pip3 install --upgrade pip
RUN pip3 install numpy

ENV LC_NAME=nb_NO.UTF-8
ENV TERM=xterm
ENV HOME /

RUN mkdir -p /app
WORKDIR /app
COPY scripts/hostmake.sh /

# If we run as non-root, make sure anyone can exec
RUN chmod a+x /hostmake.sh
ENTRYPOINT ["/hostmake.sh"]
