/*
* This is an open source non-commercial project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
*/

#include "commands/Mwr.h"

namespace sitl
{


void cmds::Mwr::encodeCommand(std::string &buffer) const
{
    buffer += KEYWORD;
    buffer += " ";
    buffer += stuff::convertToHex(m_address, m_addressLength);
    buffer += " ";
    buffer += stuff::convertToHex(m_dataWord, m_dataWordLength);
    buffer += "\n";
}


Command::Status cmds::Mwr::handleResult(const std::string &line)
{
    if (line.find(KEYWORD) != 0 || line.size() != 50 ||
        stuff::convertToHex(m_address, m_addressLength) != extractAddress(line) ||
        stuff::convertToHex(m_dataWord, m_dataWordLength) != extractDataWord(line))
    {
        return Status::IN_PROCESS;
    }

    const auto status = extractStatus(line);

    return statusFromString(status);
}

}