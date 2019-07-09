/*
* This is an open source non-commercial project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
*/

#include <commands/Mrd.h>

#include "commands/Mwr.h"

namespace sitl::cmds
{

void Mrd::encodeCommand(std::string &buffer) const
{
    buffer += KEYWORD;
    buffer += " ";
    buffer += stuff::convertToHex(m_address, m_addressLength);
    buffer += " -D";
    buffer += std::to_string(m_dataWordSize * 8);
    buffer += '\n';
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
