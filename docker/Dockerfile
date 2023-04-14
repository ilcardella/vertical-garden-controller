FROM ubuntu:latest AS builder

ARG DEBIAN_FRONTEND=noninteractive
RUN apt-get update \
    && apt-get install --no-install-recommends -y cmake \
    build-essential \
    git \
    apt-transport-https \
    ca-certificates \
    python3 \
    gcc-arm-none-eabi \
    libnewlib-arm-none-eabi \
    libstdc++-arm-none-eabi-newlib \
    && rm -rf /var/lib/apt/lists/*

# Keep PICO_SDK_PATH persistent in the image
ENV PICO_SDK_PATH=/opt/pico-sdk
ARG PICO_SDK_VERSION=1.5.0
RUN git clone --depth 1 --branch ${PICO_SDK_VERSION} \
    https://github.com/raspberrypi/pico-sdk \
    ${PICO_SDK_PATH} \
    && git -C ${PICO_SDK_PATH} submodule update --init
# Clone also the PICO_EXTRAS library
ENV PICO_EXTRAS_PATH=/opt/pico-extras
ARG PICO_EXTRAS_VERSION=sdk-${PICO_SDK_VERSION}
RUN git clone --depth 1 --branch ${PICO_EXTRAS_VERSION} \
    https://github.com/raspberrypi/pico-extras \
    ${PICO_EXTRAS_PATH} \
    && git -C ${PICO_EXTRAS_PATH} submodule update --init