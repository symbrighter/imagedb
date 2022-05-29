FROM ubuntu:focal

LABEL maintainer="Symbrighter <symbrighter@gmail.com>"

# avoid debconf from asking for input
ENV DEBIAN_FRONTEND noninteractive

# Update pkg caches, install latest pkg utils:
RUN apt-get update \
 && apt-get -qq install -y --no-install-recommends \
        apt-utils \
 && rm -rf /var/lib/apt/lists/*

# Utilities
RUN apt-get update \
 && apt-get -qq install -y --no-install-recommends \
        locales \
        ca-certificates \
        wget \
        man \
        git \
        sudo \
        vim \
        valgrind \
        jq \
 && rm -rf /var/lib/apt/lists/*

# Compilers, autotools
RUN apt-get update \
 && apt-get -qq install -y --no-install-recommends \
        build-essential \
        pkg-config \
        autotools-dev \
        libtool \
        autoconf \
        automake \
        make \
        cmake \
        clang \
        clang-tidy \
 && rm -rf /var/lib/apt/lists/*

RUN apt-get update \
 && apt-get -qq install -y --no-install-recommends \
	libsqlite3-dev \
	libssl-dev \
 && rm -rf /var/lib/apt/lists/*

RUN \
 mkdir GRPC \
 && cd GRPC \
 && git clone --recurse-submodules -b v1.46.3 --depth 1 --shallow-submodules https://github.com/grpc/grpc \
 && cd grpc \
 && mkdir -p cmake/build \
 && cd cmake/build \
 && cmake -DgRPC_INSTALL=ON -DgRPC_BUILD_TESTS=OFF ../.. \
 && make -j2 \
 && make install \
 && cd ../../

RUN \
 ln -s /usr/local/bin/grpc_cpp_plugin /usr/bin/protoc-gen-grpc

