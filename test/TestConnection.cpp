/*
* This is an open source non-commercial project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
*/
#define CATCH_CONFIG_MAIN

#include <catch.hpp>

#include "sitl.h"

TEST_CASE("Test Serial Port connection")
{
    SECTION("Simple constructor")
    {
        REQUIRE_NOTHROW([&]() {
            sitl::Connection connection{ "COM1" };
        }());
    }

    SECTION("Simple constructor throws")
    {
        REQUIRE_THROWS([&]() {
            sitl::Connection connection{ "COM1000" };
        }());
    }
}
