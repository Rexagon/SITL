/*
* This is an open source non-commercial project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
*/

#include "Stuff.h"

#include <stdexcept>

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

uint64_t convertFromHex(std::string_view hex)
{
    uint64_t result = 0;

    size_t size = hex.length();

    for (size_t i = 0, n = (size - 1) * 4; i < hex.length(); ++i, n -= 4)
    {
        const auto &symbol = toupper(hex[i]);

        if (!isdigit(symbol) && (symbol < 'A' || symbol > 'F'))
        {
            throw std::runtime_error{
                "Невозможно конвертировать hex строку в число"
            };
        }

        const auto number = static_cast<uint64_t>(
            isdigit(symbol) ? (symbol - '0') : (10 + (symbol - 'A'))
        );

        result |= (number & 0xfu ) << n;
    }

    return result;
}

}
