/*
* This is an open source non-commercial project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
*/

#include "Command.h"

#include <unordered_map>

namespace sitl::cmds
{

std::string_view extractKeyword(const std::string &line)
{
    return std::string_view{
        &line[KEYWORD_BEGIN],
        std::min(line.find_first_of(' '), KEYWORD_LENGTH_MAX)
    };
}


std::string_view extractAddress(const std::string &line)
{
    return std::string_view{
        &line[ADDRESS_BEGIN],
        std::min(line.find_first_of(' ', ADDRESS_BEGIN) - ADDRESS_BEGIN, ADDRESS_LENGTH_MAX)
    };
}


std::string_view extractDataWord(const std::string &line)
{
    return std::string_view{
        &line[DATA_WORD_BEGIN],
        std::min(line.find_first_of(' ', DATA_WORD_BEGIN) - DATA_WORD_BEGIN, DATA_WORD_LENGTH_MAX)
    };
}


std::string_view extractOrder(const std::string &line)
{
    return std::string_view{
        &line[ORDER_BEGIN],
        ORDER_LENGTH_MAX
    };
}


std::string_view extractStatus(const std::string &line)
{
    return std::string_view{
        &line[STATUS_BEGIN],
        std::min(line.find_first_of(' ', STATUS_BEGIN) - STATUS_BEGIN, STATUS_LENGTH_MAX)
    };
}


Status statusFromString(const std::string_view statusString)
{
    static std::unordered_map<std::string_view, Status> statuses = {
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
