@echo off

call clean.bat

mkdir cmakefiles_gcc
cd cmakefiles_gcc

cmake .. -G "MinGW Makefiles" -DCMAKE_INSTALL_PREFIX=../dist -DCMAKE_BUILD_TYPE=Release
cmake --build . --target sitl
cmake -DCOMPONENT=sitl -P cmake_install.cmake
