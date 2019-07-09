#ifndef LIBSITL_MRD_H
#define LIBSITL_MRD_H

#include <Sized.h>
#include "Config.h"
#include "Command.h"
#include "Sized.h"

namespace sitl::cmds
{

class SITL_API Mrd : public Command
{
public:
    template <typename TA, typename TD>
    Mrd(const Address<TA> &address, const Sized<TD> &dataWord) :
        m_address{address.data},
        m_addressLength{address.hexLength},
        m_dataWord{0},
        m_dataWordSize{dataWord.size}
    {
    }

    void encodeCommand(std::string &buffer) const override;

    Status handleResult(const std::string &line) override;

    uint64_t getDataWord() const;

private:
    constexpr static auto KEYWORD = "MRD";

    uint64_t m_address;
    size_t m_addressLength;

    uint64_t m_dataWord;
    size_t m_dataWordSize;
};

}

#endif //LIBSITL_MRD_H
