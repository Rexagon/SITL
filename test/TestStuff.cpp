/*
* This is an open source non-commercial project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
*/

#include <catch.hpp>

#include "sitl.h"

TEST_CASE("Test converter")
{
    REQUIRE(sitl::stuff::convert(static_cast<uint8_t>(0x3F)) == "3F");
    REQUIRE(sitl::stuff::convert(static_cast<uint16_t>(0x3F)) == "003F");
}

TEST_CASE("Test string split")
{
    const char delimiter = '\n';

    std::string testString = "some\nlong\nstring\nwith\n\ndelimiters\n";
    const auto lines = sitl::stuff::split(testString, delimiter);

    REQUIRE(lines.size() == 5);

    bool containsDelimiter = false;
    for (const auto& line : lines)
    {
        if (line.find_first_of(delimiter) != std::string::npos)
        {
            containsDelimiter = true;
            break;
        }
    }

    CAPTURE(lines);

    REQUIRE(!containsDelimiter);
}
