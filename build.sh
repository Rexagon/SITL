#!/bin/sh

cwd=`dirname "$0"`

sh $cwd/clean.sh

sh $cwd/prepare.sh

mkdir -p $cwd/cmakefiles_gcc/sitl
cd $cwd/cmakefiles_gcc/sitl

cmake -DCMAKE_TOOLCHAIN_FILE=~/mingw-w64-i686.cmake -DCMAKE_INSTALL_PREFIX=../../dist -DCMAKE_BUILD_TYPE=Release ../../
cmake --build . --target install
