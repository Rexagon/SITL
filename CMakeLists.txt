cmake_minimum_required(VERSION 3.14)
project(sitl)

option(BUILD_TESTS "Build tests" OFF)
option(BUILD_PYSITL "Build python bindings" ON)

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(SYSTEM_BIT_POSTFIX x64)
elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
    set(SYSTEM_BIT_POSTFIX x86)
endif()

set(EXTLIBS_DIR ${CMAKE_SOURCE_DIR}/3rdparty)
set(BUILD_OUTPUT_DIR ${CMAKE_SOURCE_DIR}/build)

# boost library
if (NOT DEFINED ENV{BOOST_ROOT})
    set(ENV{BOOST_ROOT} C:/boost_1_70_0)
endif()

set(Boost_USE_DEBUG_LIBS    OFF)    # не используем дебажные версии
set(Boost_USE_RELEASE_LIBS  ON)     # используем только релизные

set(Boost_ARCHITECTURE      "-x32")

find_package(Boost REQUIRED COMPONENTS system)

# components and libraries needed for python bindings
if (BUILD_PYSITL)
    find_package(Boost REQUIRED COMPONENTS python27)

    # python library
    find_package(Python2 COMPONENTS Development)
endif()

# sitl library
add_subdirectory(src)

# tests
if (BUILD_TESTS)
    # catch2 library
    if (NOT DEFINED ENV{Catch2_DIR})
        set(ENV{Catch2_DIR} ${EXTLIBS_DIR}/Catch2/dist/lib/cmake/Catch2)
    endif()

    find_package(Catch2 REQUIRED)

    add_subdirectory(test)
endif()
