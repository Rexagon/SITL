/*
* This is an open source non-commercial project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
*/

#include "Command.h"

#include <unordered_map>

namespace sitl
{

Command::Command() :
    m_isCompleted{false}
{
}

bool Command::isCompleted() const
{
    return m_isCompleted;
}

void Command::markCompleted()
{
    m_isCompleted = true;
}

Command::Status Command::parseStatus(const std::string& statusString)
{
    static std::unordered_map<std::string, Status> statuses = {
        { "DONE",    Status::FINISHED_DONE  },
        { "ERDON",   Status::FINISHED_ERDON },
        { "ABORT",   Status::FINISHED_ABORT },
        { "ERABR",   Status::FINISHED_ERABR },
        { "RETRY",   Status::FINISHED_RETRY },
        { "ERRET",   Status::FINISHED_ERRET }
    };

    const auto it = statuses.find(statusString);
    if (it == statuses.end())
    {
        throw std::runtime_error{"Неизвестный статус"};
    }

    return it->second;
}

}
