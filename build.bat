@echo off

call clean.bat

mkdir CMakeFiles
cd CMakeFiles

cmake .. -G "MinGW Makefiles"
cmake --build .
