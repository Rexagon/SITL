@echo off

call clean.bat

set withTests=0
for %%x in (%*) do (
    if "%%~x" EQU "--with-tests" set buildTests=1
)

if %withTests% EQU 1 (
    git submodule init
    git submodule update

    mkdir cmakefiles_gcc
    cd cmakefiles_gcc

    mkdir 3rdparty
    cd 3rdparty

    mkdir Catch2
    cd Catch2

    cmake -G "MinGW Makefiles" -DBUILD_TESTING=OFF -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=../../../3rdparty/Catch2/dist ../../../3rdparty/Catch2
    cmake --build . --target install
)
