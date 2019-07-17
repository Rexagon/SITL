/*
* This is an open source non-commercial project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
*/

#include "commands/List.h"

#include <string_view>

namespace sitl::cmds
{

List::List(const List::Type type)
    : m_type(type)
{
}


std::string List::encode() const
{
    std::string result = "LIST\n";

    if (m_type == Type::FULL_INFO)
    {
        // У результатов этой команды нет точного конца, но есть точное начало.
        // Исполняем команду дважды, для того чтобы узнать конец первого сообщения.
        return result + result;
    }

    return result;
}


Status List::decodeLine(const std::string &line)
{
    // Если строка начинается с 'LIST:' (такой строкой будет только первая в
    // результате выполнения команды)
    if (line.find("LIST:") == 0)
    {
        // Если команда была создана просто как пинг то первой строки достаточно.
        // Если мы уже заполнили какие-то данные, и получили строку 'LIST:', то
        // это означает начало результатов выполнения второй команды. Она нам не
        // нужна, но теперь точно известен конец первой команды
        if (m_type == SINGLE_PING || !m_availableCommands.empty())
        {
            return Status::FINISHED_DONE;
        }
        else
        {
            // Была только первая строка в результате, значит обрабатываем дальше
            return Status::IN_PROCESS;
        }
    }

    // Получаем информацию о команде из строки
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

    // Обработка ещё не закончена
    return Status::IN_PROCESS;
}


std::unordered_map<std::string, List::CommandInfo> List::getResult() const
{
    return m_availableCommands;
}

}
