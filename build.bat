@echo off

call clean.bat

mkdir temp
cd temp

cmake .. -G "MinGW Makefiles"
cmake --build .
