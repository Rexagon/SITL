cmake_minimum_required(VERSION 3.14)
project(libsitl)

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(SYSTEM_BIT_POSTFIX x64)
elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
    set(SYSTEM_BIT_POSTFIX x86)
endif()

set(EXTLIBS_DIR ${CMAKE_SOURCE_DIR}/extlibs)
set(BUILD_OUTPUT_DIR ${CMAKE_SOURCE_DIR}/build)

set(Boost_USE_DEBUG_LIBS    OFF)    # не используем дебажные версии
set(Boost_USE_RELEASE_LIBS  ON)     # используем только релизные

set(Boost_ARCHITECTURE      "-x32")

find_package(Boost REQUIRED COMPONENTS python27 system)

# sitl library
add_subdirectory(src)

# tests
add_subdirectory(test)
