sh clean.sh

sh prepare.sh

mkdir -p cmakefiles_gcc/sitl
cd cmakefiles_gcc/sitl

cmake -DCMAKE_TOOLCHAIN_FILE=~/mingw-w64-i686.cmake -DCMAKE_INSTALL_PREFIX=../../dist -DCMAKE_BUILD_TYPE=Release ../../
cmake --build . --target install
