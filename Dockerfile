FROM debian:bookworm-slim

RUN apt-get update \
 && DEBIAN_FRONTEND=noninteractive \
    apt-get install -y --no-install-recommends \
        cc65 \
        make \
 && apt-get clean \
 && rm -rf /var/lib/apt/lists/*
