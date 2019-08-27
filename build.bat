@echo off

PUSHD %CD%
call prepare.bat
POPD

set withoutBindings=0
for %%x in (%*) do (
    if "%%~x" EQU "--without-bindings" set withoutBindings=1
)

if %withoutBindings% EQU 1 set additionalParameters=%additionalParameters% -DBUILD_PYSITL=OFF

mkdir cmakefiles_gcc
cd cmakefiles_gcc

mkdir sitl
cd sitl

cmake -G "MinGW Makefiles" -DCMAKE_INSTALL_PREFIX=../../dist -DCMAKE_BUILD_TYPE=Release %additionalParameters% ../../
cmake --build . --target install
