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
    buffer += stuff::convert(m_address, m_addressLength);
    buffer += " ";
    buffer += stuff::convert(m_dataWord, m_dataWordLength);
    buffer += "\n";
}


Command::Status cmds::Mwr::handleResult(const std::string &line)
{
    if (line.find(KEYWORD) != 0 || line.size() != 50)
    {
        return Status::IN_PROCESS;
    }

    if (stuff::convert(m_address, m_addressLength)
        != std::string_view{&line[ADDRESS_BEGIN], m_addressLength})
    {
        throw std::runtime_error{"Неправильный адрес записи"};
    }

    if (stuff::convert(m_dataWord, m_dataWordLength)
        != std::string_view{&line[DATA_BEGIN], m_dataWordLength})
    {
        throw std::runtime_error{"Записаны неправильные данные"};
    }

    const auto status = line.substr(
        STATUS_BEGIN,
        line.find_last_not_of(' ', STATUS_BEGIN)
    );

    return parseStatus(status);
}


}
