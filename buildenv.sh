#!/bin/bash

export PREFIX="/usr/local/cross"
export TARGET="i686-elf"
export PATH="$PREFIX/bin:$PATH"

export BINUTILS_VERSION=2.37
export GCC_VERSION=11.2.0

download_binutils() {
    if [ ! -e "binutils-${BINUTILS_VERSION}.tar.gz" ]; then
        wget "https://ftp.gnu.org/gnu/binutils/binutils-${BINUTILS_VERSION}.tar.gz"
        tar -xzvf "binutils-${BINUTILS_VERSION}.tar.gz"
    fi
}

download_gcc() {
    if [ ! -e "gcc-${GCC_VERSION}.tar.gz" ]; then
        wget "https://ftp.gnu.org/gnu/gcc/gcc-${GCC_VERSION}/gcc-${GCC_VERSION}.tar.gz"
        tar -xzvf "gcc-${GCC_VERSION}.tar.gz"
    fi
}

build_binutils() {
    mkdir -p build-binutils-$BINUTILS_VERSION
    cd build-binutils-$BINUTILS_VERSION
    ../binutils-$BINUTILS_VERSION/configure --target=$TARGET --prefix=$PREFIX --with-sysroot --disable-nls --disable-werror
    make -j9
    sudo make install
}

build_gcc() {
    mkdir -p build-gcc-$GCC_VERSION
    cd build-gcc-$GCC_VERSION
    ../gcc-$GCC_VERSION/configure --target=$TARGET --prefix=$PREFIX --disable-nls --enable-languages=c --without-headers
    make all-gcc -j9
    make all-target-libgcc -j9
    sudo make install-gcc
    sudo make install-target-libgcc
}

build_all() {
    d="$(pwd)"
    download_binutils
    build_binutils
    cd "$d"
    download_gcc
    build_gcc
}
