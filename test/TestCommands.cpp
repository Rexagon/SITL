/*
* This is an open source non-commercial project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
*/

#include <catch.hpp>

#include <sitl/sitl.h>

template <typename T>
void testManualEncoding(const T& command, const std::string& target)
{
    REQUIRE(command.encode() == target);
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

        sitl::cmds::Status status{};

        const auto lines = sitl::stuff::split(reply, '\n');

        size_t currentLine = 0;
        for (const auto& line : lines)
        {
            status = listCmd.decodeLine(line);
            if (status == sitl::cmds::FINISHED_DONE)
            {
                break;
            }

            ++currentLine;
        }

        REQUIRE(status == sitl::cmds::FINISHED_DONE);
        REQUIRE(currentLine == 9);
    }
}

TEST_CASE("IDEN command")
{
    sitl::cmds::Iden idenCmd;

    SECTION("Encoding")
    {
        // Дважды, т.к. это единственный способ найти конец результата
        testManualEncoding(idenCmd, "IDEN\nIDEN\n");
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

        sitl::cmds::Status status{};

        const auto lines = sitl::stuff::split(reply, '\n');

        size_t currentLine = 0;
        for (const auto& line : lines)
        {
            status = idenCmd.decodeLine(line);
            if (status == sitl::cmds::FINISHED_DONE)
            {
                break;
            }

            ++currentLine;
        }

        REQUIRE(status == sitl::cmds::FINISHED_DONE);
        REQUIRE(currentLine == 16);
    }
}

TEST_CASE("MWR command")
{
    sitl::cmds::Mwr<uint16_t, uint32_t> mwrCmd{
        0xDEAD,
        0x0000BEAF
    };

    SECTION("Encoding")
    {
        testManualEncoding(mwrCmd, "MWR DEAD 0000BEAF\n");
    }

    SECTION("Decoding")
    {
        const char* reply =
            "MWR   DEAD             0000BEAF         LAST DONE ";

        const auto status = mwrCmd.decodeLine(reply);

        REQUIRE(status == sitl::cmds::FINISHED_DONE);
    }
}

TEST_CASE("MRD command")
{
    sitl::cmds::Mrd<uint16_t, uint16_t> mrdCmd{0xDEAD};

    SECTION("Encoding")
    {
        testManualEncoding(mrdCmd, "MRD DEAD -D16\n");
    }

    SECTION("Decoding")
    {
        const char* reply =
            "MRD   DEAD             0000BEAF         LAST DONE ";

        const auto status = mrdCmd.decodeLine(reply);

        REQUIRE(status == sitl::cmds::FINISHED_DONE);
    }
}
