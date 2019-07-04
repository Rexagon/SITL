#ifndef LIBSITL_COMMAND_H
#define LIBSITL_COMMAND_H

#include <string>
#include <utility>

#include "Config.h"

namespace sitl
{

class SITL_API Command
{
public:
    virtual ~Command() = default;

    virtual void encode(std::string &buffer) const = 0;

    virtual void decodeResult(const std::string &buffer) const;

protected:
    template <typename T>
    std::string convert(T number, size_t length = sizeof(T) * 2)
    {
        static auto digits = "0123456789ABCDEF";
        std::string result(length, '0');

        for (size_t i = 0, n = (length - 1) * 4; i < length; ++i, n -= 4)
        {
            result[i] = digits[(number >> n) & 0xfu];
        }

        return result;
    }
};

}

#endif //LIBSITL_COMMAND_H
