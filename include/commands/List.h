#ifndef LIBSITL_LIST_H
#define LIBSITL_LIST_H

#include "Config.h"
#include "Command.h"

namespace sitl::cmds
{

class SITL_API List : public Command
{
public:
    void encode(std::string &buffer) const override;

    void decodeResult(const std::string& buffer) const override;

private:
    constexpr static auto KEYWORD = "LIST";
};

}

#endif //LIBSITL_LIST_H
