#ifndef LIBSITL_IDEN_H
#define LIBSITL_IDEN_H

#include "Config.h"
#include "Command.h"

namespace sitl::cmds
{

class SITL_API Iden : public Command
{
public:
    Iden();

    void encodeCommand(std::string &buffer) const override;

    Status handleResult(const std::string& line) override;

    const std::string& getInfo() const;

private:
    constexpr static auto KEYWORD = "IDEN";

    std::string m_info;
};

}

#endif //LIBSITL_IDEN_H
