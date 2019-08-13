/*
* This is an open source non-commercial project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
*/

#include <catch2/catch.hpp>

#include <sitl/sitl.h>

TEST_CASE("Test converter")
{
    REQUIRE(sitl::stuff::convertToHex(static_cast<uint8_t>(0x3F)) == "3F");
    REQUIRE(sitl::stuff::convertToHex(static_cast<uint16_t>(0x103F)) == "103F");

    REQUIRE(sitl::stuff::convertFromHex("3F") == 0x3Fu);
    REQUIRE(sitl::stuff::convertFromHex("103F") == 0x103Fu);
}

TEST_CASE("Test string split")
{
    const auto delimiter = "\n";

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
