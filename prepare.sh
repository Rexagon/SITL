#!/bin/bash

cwd=$(dirname "$0")

bash "$cwd"/clean.sh

withTests=0
for var in "$@"
do
    [ "$var" = "--with-tests" ] && withTests=1
done

if [ $withTests -eq 1 ]; then
    mkdir -p "$cwd"/cmakefiles_gcc/3rdparty/Catch2
    cd "$cwd"/cmakefiles_gcc/3rdparty/Catch2 || exit

    cmake -DCMAKE_TOOLCHAIN_FILE=~/mingw-w64-i686.cmake \
          -DBUILD_TESTING=OFF \
          -DCMAKE_BUILD_TYPE=Release \
          -DCMAKE_INSTALL_PREFIX=../../../3rdparty/Catch2/dist \
          ../../../3rdparty/Catch2

    cmake --build . --target install
fi
