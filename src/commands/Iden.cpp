/*
 * This is an open source non-commercial project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
 */

#include <sitl/commands/Iden.h>

#include <functional>

namespace sitl::cmds
{
std::string Iden::encode() const
{
    std::string result = "IDEN\n";

    // У результатов этой команды нет точного конца, но есть точное начало.
    // Исполняем команду дважды, для того чтобы узнать длину первого сообщения.
    return result + result;
}


Status Iden::decodeLine(const std::string &line)
{
    // Если строка начинается с 'IDEN:'
    if (line.find("IDEN:") == 0)
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

        // В первом состоянии происходит получение информации
        case State::First:
        {
            // Находим позицию последнего пробела
            auto pos = line.find_last_not_of(' ');
            if (pos == std::string::npos)
            {
                pos = line.length();
            }
            else
            {
                ++pos;
            }

            // Убираем пробелы с конца строки и добавляем к результату
            m_info += line.substr(0, pos) + "\n";

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


std::string Iden::getResult() const
{
    return m_info;
}


void Iden::incrementState()
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


void Iden::incrementLine()
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
