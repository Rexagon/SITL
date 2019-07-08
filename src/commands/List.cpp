/*
* This is an open source non-commercial project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
*/

#include "commands/List.h"

#include <string_view>

namespace sitl::cmds
{

void List::encodeCommand(std::string &buffer) const
{
    buffer.append(KEYWORD);
    buffer.append("\n");

    // У результатов этой команды нет точного конца, но есть точное начало.
    // Исполняем команду дважды, для того чтобы узнать конец первого сообщения.
    buffer.append(KEYWORD);
    buffer.append("\n");
}

Command::Status List::handleResult(const std::string &line)
{
    if (!m_availableCommands.empty() && line.find("LIST:") == 0) {
        return Status::FINISHED_DONE;
    }

    auto pos = line.find_first_of(' ');
    const auto keyword = line.substr(0, pos);

    if (keyword == "INTRQ")
    {
        //TODO: Обработать наличие и количество входов аппаратных прерываний
    }
    else
    {
        m_availableCommands.emplace(keyword, CommandInfo{});
        //TODO: Обработать возможные параметры команд
    }

    return Status::IN_PROCESS;
}

const std::unordered_map<std::string, List::CommandInfo> & List::getAvailableCommands() const
{
    return m_availableCommands;
}

}
