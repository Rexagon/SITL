@echo off

call clean.bat

mkdir CMakeFiles
cd CMakeFiles

cmake .. -G "MinGW Makefiles" -DCMAKE_INSTALL_PREFIX=../dist -DCMAKE_BUILD_TYPE=Release
cmake --build . --target sitl
cmake -DCOMPONENT=sitl -P cmake_install.cmake
