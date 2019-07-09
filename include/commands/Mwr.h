#ifndef LIBSITL_MWR_H
#define LIBSITL_MWR_H

#include "Config.h"
#include "Command.h"

namespace sitl::cmds
{

/**
 * @brief   Команда, выполняющая запись одиночного слова данных в адресном
 *          пространстве памяти.
 */
template <typename TA, typename TD>
class SITL_API Mwr
{
public:
    using ResultType = void;

    explicit Mwr(TA address, TD data) :
        m_address{address},
        m_dataWord{data}
    {
    }

    void encode(std::string &buffer) const
    {
        stuff::appendLine(buffer,
            "MWR ",
            stuff::convertToHex(m_address),
            " ",
            stuff::convertToHex(m_dataWord)
        );
    }


    Status decodeLine(const std::string &line)
    {
        if (line.find("MWR") != 0 ||
            stuff::convertToHex(m_address) != extractAddress(line) ||
            stuff::convertToHex(m_dataWord) != extractDataWord(line))
        {
            return Status::IN_PROCESS;
        }

        const auto status = extractStatus(line);

        return statusFromString(status);
    }


    void getResult() {}

private:
    TA m_address;
    TD m_dataWord;
};

}

#endif //LIBSITL_MWR_H
