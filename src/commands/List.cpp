/*
* This is an open source non-commercial project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
*/

#include "commands/List.h"

namespace sitl::cmds
{

void List::encode(std::string &buffer) const
{
    buffer.append(KEYWORD);
    buffer.append("\n");
}

void List::decodeResult(const std::string &/*buffer*/) const
{
    //TODO: parse results
}

}
