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
    // Исполняем команду дважды, для того чтобы узнать конец первого сообщения.
    return result + result;
}


Status Iden::decodeLine(const std::string &line)
{
    // Если информация уже чем-то заполнена и мы встретили строку, начинающуюся
    // с 'IDEN:', то значит уже обрабатывается результат выполнения второй
    // команды. Он нам не нужен, но зато теперь ясно что закончился результат
    // выполнения первой команды.
    if (!m_info.empty() && line.find("IDEN:") == 0)
    {
        // Результат выполнения команды обработан
        return Status::FINISHED_DONE;
    }

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

    // Не весь результат обработан
    return Status::IN_PROCESS;
}


std::string Iden::getResult() const
{
    return m_info;
}

} // namespace sitl::cmds
