/*
* This is an open source non-commercial project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
*/

#include "commands/Mrd.h"

namespace sitl::cmds
{

namespace
{
constexpr auto KEYWORD = "MRD";
}

void Mrd::encodeCommand(std::string &buffer) const
{
    stuff::appendLine(buffer,
        KEYWORD,
        " ",
        stuff::convertToHex(m_address, m_addressLength),
        " -D",
        std::to_string(m_dataWordSize * 8)
    );
}

Command::Status Mrd::handleResult(const std::string &line)
{
    if (line.find(KEYWORD) != 0 || line.size() != 50 ||
        stuff::convertToHex(m_address, m_addressLength) != extractAddress(line))
    {
        return Status::IN_PROCESS;
    }

    m_dataWord = stuff::convertFromHex(extractDataWord(line));

    const auto status = extractStatus(line);

    return statusFromString(status);
}

uint64_t Mrd::getDataWord() const
{
    return m_dataWord;
}

}
