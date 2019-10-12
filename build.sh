#!/bin/bash

cwd=$(dirname "$0")

bash "$cwd"/prepare.sh

withoutBindings=0
for var in "$@"
do
    [ "$var" = "--without-bindings" ] && withoutBindings=1
done

additionalParameters=""
[ $withoutBindings -eq 1 ] && additionalParameters="$additionalParameters -DBUILD_PYSITL=OFF"

echo "$additionalParameters"

mkdir -p "$cwd"/cmakefiles_gcc/sitl
cd "$cwd"/cmakefiles_gcc/sitl || exit

echo $pwd

cmake -DCMAKE_TOOLCHAIN_FILE=~/mingw-w64-i686.cmake \
      -DCMAKE_INSTALL_PREFIX=../../dist \
      -DCMAKE_BUILD_TYPE=Release \
      $additionalParameters \
      ../../

cmake --build . --target install
