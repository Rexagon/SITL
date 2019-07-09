/*
* This is an open source non-commercial project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
*/

#include "commands/List.h"

#include <string_view>

namespace sitl::cmds
{

namespace
{
constexpr auto KEYWORD = "LIST";
}

List::List(List::Type type) :
    m_type(type)
{
}


void List::encode(std::string &buffer) const
{
    if (m_type == Type::FULL_INFO)
    {
        // У результатов этой команды нет точного конца, но есть точное начало.
        // Исполняем команду дважды, для того чтобы узнать конец первого сообщения.
        stuff::appendLine(buffer, KEYWORD, "\n", KEYWORD);
    }
    else
    {
        stuff::appendLine(buffer, KEYWORD);
    }
}


Status List::decodeLine(const std::string &line)
{
    if (line.find("LIST:") == 0 &&
        (m_type == SINGLE_PING || !m_availableCommands.empty()))
    {
        return Status::FINISHED_DONE;
    }

    const auto keyword = extractKeyword(line);

    if (keyword == "INTRQ")
    {
        //TODO: Обработать наличие и количество входов аппаратных прерываний
    }
    else
    {
        m_availableCommands.emplace(std::string{keyword}, CommandInfo{});
        //TODO: Обработать возможные параметры команд
    }

    return Status::IN_PROCESS;
}


List::ResultType List::getResult() const
{
    return m_availableCommands;
}

}
