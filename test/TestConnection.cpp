/*
* This is an open source non-commercial project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
*/

#define CATCH_CONFIG_MAIN

#include <catch.hpp>

#include "sitl.h"

TEST_CASE("Successful construction")
{
    REQUIRE_NOTHROW([]() {
        sitl::Connection connection{"COM1"};
    }());
}

TEST_CASE("Failed construction")
{
    REQUIRE_THROWS([]() {
        sitl::Connection connection{ "COM1000" };
    }());
}

TEST_CASE("List command")
{
    sitl::Connection connection{ "COM1" };

    sitl::cmds::List listCmd;

    // Пока будет очень долго ждать, т.к. нет ещё устройства нужного
    connection.makeTransaction(listCmd);
}
