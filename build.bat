mkdir -p cmakefiles_gcc/sitl
cd cmakefiles_gcc/sitl

cmake -G "MinGW Makefiles" -DCMAKE_INSTALL_PREFIX=../../dist -DCMAKE_BUILD_TYPE=Release ../../
cmake --build . --target install
