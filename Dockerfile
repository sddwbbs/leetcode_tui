FROM fedora:40

RUN dnf update -y && \
    dnf install -y \
    git \
    gcc \
    g++ \
    cmake \
    clang \
    ninja-build \
    make  \
    curl \
    elinks \
    neovim \
    libcurl-devel \
    ncurses-devel \
    nlohmann-json-devel \
    postgresql \
    libpq \
    libpq-devel \
    libpqxx-devel \
    openssl-devel \
    libasan \
    libubsan \
    && ldconfig

ENV LANG=en_US.UTF-8
ENV TERM=xterm-256color

COPY third_party/cpr /usr/src/cpr

RUN cd /usr/src/cpr && mkdir build && cd build && \
    cmake .. -DCPR_USE_SYSTEM_CURL=ON && \
    cmake --build . --parallel && \
    cmake --install .

WORKDIR /usr/src/app

COPY . .

RUN cd scripts && ./build_app