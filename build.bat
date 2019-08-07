@echo off

PUSHD %CD%
call prepare.bat
POPD

mkdir cmakefiles_gcc
cd cmakefiles_gcc

mkdir sitl
cd sitl

cmake -G "MinGW Makefiles" -DCMAKE_INSTALL_PREFIX=../../dist -DCMAKE_BUILD_TYPE=Release ../../
cmake --build . --target install
