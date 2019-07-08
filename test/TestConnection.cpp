/*
* This is an open source non-commercial project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
*/

#include <catch.hpp>

#include "sitl.h"

constexpr static auto DEFAULT_COM = "COM3";
constexpr static auto DEFAULT_BAUD_RATE = 9600;

namespace
{
void testManualEncoding(sitl::Command &command, const std::string &target)
{
    std::string buffer;
    command.encodeCommand(buffer);

    REQUIRE(buffer == target);
}
}

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
        sitl::Connection connection{DEFAULT_COM, DEFAULT_BAUD_RATE};

        sitl::cmds::List listCmd;
        connection.makeTransaction(listCmd);

        sitl::cmds::Iden idenCmd;
        connection.makeTransaction(idenCmd);

        sitl::cmds::Mwr mwrCmd{
            sitl::Address<uint16_t>{0xF74A},
            sitl::DataWord<uint8_t>{0xFF}
        };
        connection.makeTransaction(mwrCmd);
    }());
}
