FROM ubuntu:22.04

ENV HOME /root

WORKDIR $HOME/projects/diprog/

RUN apt-get update && apt-get install -y \
	git \
	vim \
	curl \
	wget \
	tar \
	g++ \
	unzip\
	cmake
	
RUN wget -O opencv.zip https://github.com/opencv/opencv/archive/4.x.zip
RUN unzip opencv.zip
RUN mkdir -p build && cd build
RUN cmake  ../diprog/opencv-4.x
RUN cmake --build .

