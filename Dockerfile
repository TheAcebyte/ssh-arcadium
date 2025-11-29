FROM ubuntu:22.04 AS builder

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    build-essential \
    libboost-system-dev \
    libboost-thread-dev 

WORKDIR /build

COPY src/ ./src/
COPY Makefile .

RUN make all

FROM ubuntu:22.04

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

COPY --from=builder /build/bin/* /opt/arcadium/bin/

RUN chmod 755 /opt/arcadium/bin/*

RUN passwd -d root

COPY sshd_config /etc/ssh/sshd_config

RUN mkdir -p /run/sshd

COPY scripts/entrypoint.sh /entrypoint.sh
RUN chmod +x /entrypoint.sh

EXPOSE 22

ENTRYPOINT ["/entrypoint.sh"]
