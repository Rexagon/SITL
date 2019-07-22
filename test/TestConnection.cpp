/*
* This is an open source non-commercial project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
*/

#include <iostream>

#include <catch.hpp>

#include <sitl/sitl.h>

constexpr static auto DEFAULT_COM = "COM4";
constexpr static auto DEFAULT_BAUD_RATE = 115200;


TEST_CASE("Successful construction")
{
    REQUIRE_NOTHROW([]() {
        sitl::Connection connection{DEFAULT_COM, DEFAULT_BAUD_RATE};
    }());
}

TEST_CASE("Failed construction")
{
    REQUIRE_THROWS([]() {
        sitl::Connection connection{ "some invalid COM", DEFAULT_BAUD_RATE };
    }());
}

TEST_CASE("Transaction test")
{
    REQUIRE_NOTHROW([]() {
        using namespace sitl;

        Connection connection{DEFAULT_COM, DEFAULT_BAUD_RATE};

        //const auto listResult = connection.execute<cmds::List>();

        //const auto info = connection.execute<cmds::Iden>();

        connection.execute<cmds::Mwr<uint16_t, uint8_t>>(0x0018, 0xFF);

        const auto data = connection.execute<cmds::Mrd<uint16_t, uint8_t>>(0x0018);

        REQUIRE(data == 0xFF);
    }());
}
