#ifndef LIBSITL_LIST_H
#define LIBSITL_LIST_H

#include <vector>
#include <unordered_map>

#include "Config.h"
#include "Command.h"

namespace sitl::cmds
{

class SITL_API List : public Command
{
public:
    struct CommandInfo
    {
        //TODO: указать здесь возможные разрядности и т.д.
    };

    void encodeCommand(std::string &buffer) const override;

    Status handleResult(const std::string &line) override;

    const std::unordered_map<std::string, CommandInfo>& getAvailableCommands() const;

private:
    constexpr static auto KEYWORD = "LIST";

    std::unordered_map<std::string, CommandInfo> m_availableCommands;
};

}

#endif //LIBSITL_LIST_H
