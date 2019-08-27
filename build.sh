#!/bin/bash

cwd=$(dirname "$0")

bash "$cwd"/clean.sh

bash "$cwd"/prepare.sh

mkdir -p "$cwd"/cmakefiles_gcc/sitl
cd "$cwd"/cmakefiles_gcc/sitl || exit

cmake -DCMAKE_TOOLCHAIN_FILE=~/mingw-w64-i686.cmake -DCMAKE_INSTALL_PREFIX=../../dist -DCMAKE_BUILD_TYPE=Release ../../
cmake --build . --target install
