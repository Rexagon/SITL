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
    /**
     * @brief           Создаёт команду записи данных по адресу.
     * @param address   Адрес записи
     * @param data      Слово данных
     */
    explicit Mwr(TA address, TD data) :
        m_address{address},
        m_dataWord{data}
    {
    }


    /**
     * @brief           Кодирует команду.
     * @return          Строка с командой SITL
     */
    std::string encode() const
    {
        const auto address = stuff::convertToHex(m_address);
        const auto data = stuff::convertToHex(m_dataWord);

        return "MWR " + address + " " + data + "\n";
    }


    /**
     * @brief           Обрабатывает строку результата.
     * @param line      Строка результата
     * @return          Статус обработки
     */
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


    /**
     * @brief           Команда не возвращает результата
     *
     * Этот метод должен быть определён.
     */
    void getResult() {}

private:
    TA m_address;
    TD m_dataWord;
};

}

#endif //LIBSITL_MWR_H
