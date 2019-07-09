/*
* This is an open source non-commercial project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
*/

#include <catch.hpp>

#include "sitl.h"

void testManualEncoding(sitl::Command& command, const std::string& target)
{
    std::string buffer;
    command.encodeCommand(buffer);

    REQUIRE(buffer == target);
}


TEST_CASE("LIST command")
{
    sitl::cmds::List listCmd;

    SECTION("Encoding")
    {
        // Дважды, т.к. это единственный способ найти конец результата
        testManualEncoding(listCmd, "LIST\nLIST\n");
    }

    SECTION("Decoding")
    {
        const char* reply =
            "LIST:                                             \n"  // #0
            "IDEN                                              \n"
            "MWR   ADDRESS-24       DATA-WORD-08               \n"
            "MWR   ADDRESS-24       DATA-WORD-16               \n"
            "MWR   ADDRESS-24       DATA-WORD-32               \n"
            "MWR   ADDRESS-32       DATA-WORD-08               \n"
            "MWR   ADDRESS-32       DATA-WORD-16               \n"
            "MWR   ADDRESS-32       DATA-WORD-32               \n"
            "INTRQ IRQ[14:0]        -D08                       \n"  // #8
            "LIST:                                             \n"  // #0 <- x2;
            "IDEN                                              \n";
        //  ...

        sitl::Command::Status status{};

        const auto lines = sitl::stuff::split(reply, '\n');

        size_t currentLine = 0;
        for (const auto& line : lines)
        {
            status = listCmd.handleResult(line);
            if (status == sitl::Command::FINISHED_DONE)
            {
                break;
            }

            ++currentLine;
        }

        REQUIRE(status == sitl::Command::FINISHED_DONE);
        REQUIRE(currentLine == 9);
    }
}

TEST_CASE("IDEN command")
{
    sitl::cmds::Iden idenCmd;

    SECTION("Encoding")
    {
        std::string buffer;
        idenCmd.encodeCommand(buffer);

        // Дважды, т.к. это единственный способ найти конец результата
        REQUIRE(buffer == "IDEN\nIDEN\n");
    }

    SECTION("Decoding")
    {
        const char* reply =
            "IDEN: SITL-v.1.0                                  \n"  // #0
            "SITL-assembler:                                   \n"
            "UDP IO Client SW Ver.1.3                          \n"
            "For communication with 1986BE1T MCU               \n"
            "Via Ethernet IP network connection                \n"
            "SITL-processor:                                   \n"
            "UDP IO Client SW Ver.1.3                          \n"
            "For communication with 1986BE1T MCU               \n"
            "Via Ethernet IP network connection                \n"
            "SITL-disassembler:                                \n"
            "UDP IO Client SW Ver.1.3                          \n"
            "For communication with 1986BE1T MCU               \n"
            "Via Ethernet IP network connection                \n"
            "(C) Institute Argon. 2014.                        \n"
            "For OS Microsoft Windows version XP-7             \n"
            "Written by Ivan Perdunov                          \n"  // #15
            "IDEN: SITL-v.1.0                                  \n"  // #0 <- x2
            "SITL-assembler:                                   \n";
        //  ...

        sitl::Command::Status status{};

        const auto lines = sitl::stuff::split(reply, '\n');

        size_t currentLine = 0;
        for (const auto& line : lines)
        {
            status = idenCmd.handleResult(line);
            if (status == sitl::Command::FINISHED_DONE)
            {
                break;
            }

            ++currentLine;
        }

        REQUIRE(status == sitl::Command::FINISHED_DONE);
        REQUIRE(currentLine == 16);
    }
}

TEST_CASE("MWR command")
{
    sitl::cmds::Mwr mwrCmd{
        sitl::Address<uint16_t>{0xDEAD},
        sitl::DataWord<uint32_t>{0x0000BEAF}
    };

    SECTION("Encoding")
    {
        std::string buffer;
        mwrCmd.encodeCommand(buffer);

        REQUIRE(buffer == "MWR DEAD 0000BEAF\n");
    }

    SECTION("Decoding")
    {
        const char* reply =
            "MWR   DEAD             0000BEAF         LAST DONE ";

        const auto status = mwrCmd.handleResult(reply);

        REQUIRE(status == sitl::Command::FINISHED_DONE);
    }
}

TEST_CASE("MRD command")
{
    sitl::cmds::Mrd mrdCmd{
        sitl::Address<uint16_t>{0xDEAD},
        sitl::Sized<uint16_t>{}
    };

    SECTION("Encoding")
    {
        std::string buffer;
        mrdCmd.encodeCommand(buffer);

        REQUIRE(buffer == "MRD DEAD -D16\n");
    }

    SECTION("Decoding")
    {
        const char* reply =
            "MRD   DEAD             0000BEAF         LAST DONE ";

        const auto status = mrdCmd.handleResult(reply);

        REQUIRE(status == sitl::Command::FINISHED_DONE);
    }
}