FROM ubuntu:24.04 AS builder

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    build-essential \
    zip \
    unzip \
    tar \
    curl \
    pkg-config \
    git

WORKDIR /build

COPY src/ ./src/
COPY vcpkg.json .
COPY Makefile .

RUN make -s all

FROM ubuntu:24.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    openssh-server \
    wget \
    curl \
    && rm -rf /var/lib/apt/lists/*

RUN wget https://bin.equinox.io/c/bNyj1mQVY4c/ngrok-v3-stable-linux-amd64.tgz && \
    tar xvzf ngrok-v3-stable-linux-amd64.tgz -C /usr/local/bin/ && \
    rm ngrok-v3-stable-linux-amd64.tgz

RUN useradd -m -s /bin/bash player && \
    passwd -d player

COPY --from=builder /build/bin /opt/ssh-arcadium/bin

RUN chmod -R 755 /opt/ssh-arcadium/bin

RUN passwd -d root

COPY sshd_config /etc/ssh/sshd_config

RUN mkdir -p /run/sshd

COPY scripts/entrypoint.sh /entrypoint.sh
RUN chmod +x /entrypoint.sh

ENTRYPOINT ["/entrypoint.sh"]
