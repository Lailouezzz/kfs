#! /bin/sh

export PREFIX="$HOME/opt/cross"
export TARGET=i386-elf
export PATH="$PREFIX/bin:$PATH"

wget https://ftp.gnu.org/gnu/gdb/gdb-16.2.tar.gz
wget https://ftp.gnu.org/gnu/gcc/gcc-14.2.0/gcc-14.2.0.tar.gz
wget https://ftp.gnu.org/gnu/binutils/binutils-2.44.tar.gz

tar xvf gdb-16.2.tar.gz
tar xvf gcc-14.2.0.tar.gz
tar xvf binutils-2.44.tar.gz

mkdir build-binutils
mkdir build-gcc
mkdir build-gdb

cd build-binutils
../binutils-2.44/configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror
make -j$(nproc)
make install

cd ../build-gdb
../gdb-16.2/configure --target=$TARGET --prefix="$PREFIX" --disable-werror
make all-gdb -j$(nproc)
make install-gdb

cd ../build-gcc
../gcc-14.2.0/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-languages=c,c++ --without-headers --disable-hosted-libstdcxx
make all-gcc -j$(nproc)
make all-target-libgcc -j$(nproc)
make all-target-libstdc++-v3 -j$(nproc)
make install-gcc
make install-target-libgcc
make install-target-libstdc++-v3
