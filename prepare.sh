sh clean.sh

git submodule init
git submodule update

mkdir -p cmakefiles_gcc/3rdparty/Catch2
cd cmakefiles_gcc/3rdparty/Catch2

cmake -DCMAKE_TOOLCHAIN_FILE=~/mingw-w64-i686.cmake -DBUILD_TESTING=OFF -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=../../../3rdparty/Catch2/dist ../../../3rdparty/Catch2
cmake --build . --target install
