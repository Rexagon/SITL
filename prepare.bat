git submodule init
git submodule update

mkdir -p cmakefiles_gcc/3rdparty/Catch2
cd cmakefiles_gcc/3rdparty/Catch2

cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=../../../3rdparty/Catch2/dist ../../../3rdparty/Catch2/
cmake --install .
