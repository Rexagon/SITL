#ifndef LIBSITL_MRD_H
#define LIBSITL_MRD_H

#include <sitl/Command.h>
#include <sitl/Config.h>

namespace sitl::cmds
{
/**
 * @brief   Команда, выполняющая чтение одиночного слова данных в адресном
 *          пространстве памяти.
 *
 * connection.execute<Mrd<TA, TD>>(TA address) -> TD
 */
template <typename TA, typename TD>
class SITL_API Mrd
{
public:
    static_assert(stuff::is_any_of_v<TA, uint8_t, uint16_t, uint32_t, uint64_t>);
    static_assert(stuff::is_any_of_v<TD, uint8_t, uint16_t, uint32_t, uint64_t>);

    /**
     * @brief           Создаёт команду чтения.
     * @param address   Адрес чтения
     */
    explicit Mrd(const TA address)
        : m_address{address}
        , m_data{}
    {
    }


    /**
     * @brief           Кодирует команду.
     * @return          Строка с командой SITL
     */
    std::string encode() const
    {
        const auto address = stuff::convertToHex(m_address);
        auto dataSize = std::to_string(sizeof(TD) * 8);

        // Отдельный случай для -D08
        if (dataSize.length() < 2)
        {
            dataSize = "0" + dataSize;
        }

        return "MRD " + address + " -D" + dataSize + "\n";
    }


    /**
     * @brief           Обрабатывает строку результата.
     * @param line      Строка результата
     * @return          Статус обработки
     */
    Status decodeLine(const std::string &line)
    {
        if (line.find("MRD") != 0)
        {
            return Status::IN_PROCESS;
        }

        if (stuff::convertToHex(m_address) != extractAddress(line))
        {
            throw std::runtime_error{"Несовпадение адреса в ответе"};
        }

        m_data = static_cast<TD>(stuff::convertFromHex(extractDataWord(line)));

        const auto status = extractStatus(line);

        return statusFromString(status);
    }


    /**
     * @brief   Возвращает прочитанное слово данных.
     *
     * Оно гарантированно будет не больше, чем максимально возможное значение
     * указанного в конструкторе типа данных слова данных.
     *
     * @return  Слово данных
     */
    TD getResult() const
    {
        return m_data;
    }

private:
    TA m_address;
    TD m_data;
};

} // namespace sitl::cmds

#endif // LIBSITL_MRD_H
