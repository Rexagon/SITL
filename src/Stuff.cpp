/*
* This is an open source non-commercial project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
*/

#include "Stuff.h"

namespace sitl::stuff
{

std::vector<std::string> split(const std::string &string, const char delimiter)
{
    std::vector<std::string> result;

    size_t currentPos = 0;
    while ((currentPos = string.find_first_not_of(delimiter, currentPos)) != std::string::npos)
    {
        const auto it = string.find_first_of(delimiter, currentPos);
        result.emplace_back(string.substr(currentPos, it - currentPos));
        currentPos = it;
    }

    return result;
}

}
