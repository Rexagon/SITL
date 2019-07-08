/*
* This is an open source non-commercial project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
*/

#include "commands/Iden.h"

#include <functional>

namespace sitl::cmds
{

Iden::Iden() :
    m_info{}
{
}


void Iden::encodeCommand(std::string &buffer) const
{
    buffer.append(KEYWORD);
    buffer.append("\n");

    // У результатов этой команды нет точного конца, но есть точное начало.
    // Исполняем команду дважды, для того чтобы узнать конец первого сообщения.
    buffer.append(KEYWORD);
    buffer.append("\n");
}


Command::Status Iden::handleResult(const std::string &line)
{
    if (!m_info.empty() && line.find("IDEN:") == 0) {
        return Status::FINISHED_DONE;
    }
    else {
        m_info.append(1, '\n');
    }

    auto pos = line.find_last_not_of(' ');
    if (pos == std::string::npos) {
        pos = line.length();
    }

    m_info.append(line.substr(0, pos));

    return Status::IN_PROCESS;
}


const std::string &Iden::getInfo() const
{
    return m_info;
}

}
