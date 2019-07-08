#ifndef LIBSITL_MWR_H
#define LIBSITL_MWR_H

#include "Config.h"
#include "Command.h"

namespace sitl::cmds
{

class SITL_API Mwr : public Command
{
public:
    template <typename TA, typename TD>
    Mwr(const Address<TA> &address, const DataWord<TD> &dataWord) :
        m_address{address.data},
        m_addressLength{address.hexLength},
        m_dataWord{dataWord.data},
        m_dataWordLength{dataWord.hexLength}
    {
    }

    void encodeCommand(std::string &buffer) const override;

    Status handleResult(const std::string &line) override;

private:
    constexpr static auto KEYWORD = "MWR";

    uint64_t m_address;
    size_t m_addressLength;

    uint64_t m_dataWord;
    size_t m_dataWordLength;
};

}

#endif //LIBSITL_MWR_H
