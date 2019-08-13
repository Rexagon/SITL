/*
 * This is an open source non-commercial project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
 */

#include <sitl/commands/List.h>

#include <string_view>

namespace sitl::cmds
{
std::string List::encode() const
{
    std::string result = "LIST\n";

    // У результатов этой команды нет точного конца, но есть точное начало.
    // Исполняем команду дважды, для того чтобы узнать длину первого сообщения.
    return result + result;
}


Status List::decodeLine(const std::string &line)
{
    // Если строка начинается с 'LIST:'
    if (line.find("LIST:") == 0)
    {
        incrementState();
        return Status::IN_PROCESS;
    }

    incrementLine();

    switch (m_state)
    {
        // Сюда мы не должны попадать при нормальных входных данных
        case State::None:
            throw std::runtime_error{"Невозможно декодировать результат"};

        // В первом состоянии происходит получение информации о командах
        case State::First:
        {
            // Получаем информацию о команде из строки
            const auto keyword = extractKeyword(line);

            if (keyword == "INTRQ")
            {
                // TODO: Обработать наличие и количество входов аппаратных прерываний
            }
            else if (!keyword.empty())
            {
                m_availableCommands.emplace(std::string{keyword}, CommandInfo{});
                // TODO: Обработать возможные параметры команд
            }

            break;
        }

        // Во втором состоянии ждём столько же строк, сколько и в первом
        case State::Second:
            if (m_lineRead.second == m_lineRead.first)
            {
                return Status::FINISHED_DONE;
            }

            break;
    }

    return Status::IN_PROCESS;
}


std::unordered_map<std::string, List::CommandInfo> List::getResult() const
{
    return m_availableCommands;
}


void List::incrementState()
{
    switch (m_state)
    {
        case State::None:
            m_state = State::First;
            break;

        case State::First:
            m_state = State::Second;
            break;

        default:
            break;
    }
}


void List::incrementLine()
{
    switch (m_state)
    {
        case State::First:
            m_lineRead.first++;
            break;

        case State::Second:
            m_lineRead.second++;
            break;

        default:
            break;
    }
}

} // namespace sitl::cmds
