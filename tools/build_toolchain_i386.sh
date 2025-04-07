#!/bin/sh

# This script *MUST* be run it's directory and *NOT*
# from the root of the project !

BINUTILS_URL=https://ftp.gnu.org/gnu/binutils/binutils-2.44.tar.gz
GCC_URL=https://ftp.gnu.org/gnu/gcc/gcc-14.2.0/gcc-14.2.0.tar.gz
GDB_URL=https://ftp.gnu.org/gnu/gdb/gdb-16.2.tar.gz

BINUTILS_ARCHIVE_NAME=$(basename $BINUTILS_URL .tar.gz)
GCC_ARCHIVE_NAME=$(basename $GCC_URL .tar.gz)
GDB_ARCHIVE_NAME=$(basename $GDB_URL .tar.gz)

PWD=$(pwd)
export PREFIX="$PWD/cross"
export TARGET=i386-elf
export PATH="$PREFIX/bin:$PATH"

NPROC=$(nproc)
ROOT="$PWD"/build-toolchain

mkdir -p "$PREFIX" "$ROOT"
cd "$ROOT"

if ! (wget --continue "$BINUTILS_URL" && tar xf "$BINUTILS_ARCHIVE_NAME".tar.gz)
then
	echo failed to download or extract BINUTILS >&2
	exit 1
fi

if ! (wget --continue "$GCC_URL" && tar xf "$GCC_ARCHIVE_NAME".tar.gz)
then
	echo failed to download or extract GCC >&2
	exit 1
fi

if ! (wget --continue "$GDB_URL" && tar xf "$GDB_ARCHIVE_NAME".tar.gz)
then
	echo failed to download or extract GDB >&2
	exit 1
fi

cd "$ROOT/$BINUTILS_ARCHIVE_NAME"
mkdir build
cd build
../configure				\
	--target=$TARGET		\
	--prefix="$PREFIX"		\
	--with-sysroot			\
	--disable-nls			\
	--disable-werror
make -j$NPROC
make install

cd "$ROOT"
rm -rf "$BINUTILS_ARCHIVE_NAME"

cd "$ROOT/$GCC_ARCHIVE_NAME"
mkdir build
cd build
../configure					\
	--target=$TARGET			\
	--prefix="$PREFIX"			\
	--disable-nls				\
	--enable-languages=c,c++	\
	--without-headers			\
	--disable-hosted-libstdcxx
make all-gcc -j$NPROC
make all-target-libgcc -j$NPROC
make all-target-libstdc++-v3 -j$NPROC
make install-gcc
make install-target-libgcc
make install-target-libstdc++-v3

cd "$ROOT"
rm -rf "$GCC_ARCHIVE_NAME"

cd "$ROOT/$GDB_ARCHIVE_NAME"
mkdir build
cd build
../configure			\
	--target=$TARGET	\
	--prefix="$PREFIX"	\
	--disable-werror
make all-gdb -j$NPROC
make install-gdb

cd "$ROOT"
rm -rf "$GDB_ARCHIVE_NAME"
