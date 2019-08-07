@echo off

call clean.bat

PUSHD %CD%
call prepare.bat
POPD

mkdir cmakefiles_gcc
cd cmakefiles_gcc

mkdir sitl
cd sitl

cmake -G "MinGW Makefiles" -DCMAKE_INSTALL_PREFIX=../../dist -DCMAKE_BUILD_TYPE=Release ../../
cmake --build . --target sitl
cmake -DCOMPONENT=sitl -P cmake_install.cmake
