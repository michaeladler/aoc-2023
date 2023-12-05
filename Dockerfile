FROM archlinux:latest

RUN pacman -Syu --noconfirm clang meson hyperfine curl \
        && curl -L -o /usr/local/bin/poop https://github.com/andrewrk/poop/releases/download/0.4.0/x86_64-linux-poop \
        && chmod 0755 /usr/local/bin/poop \
        && mkdir -p /src \
        && pacman -Scc

WORKDIR /src
