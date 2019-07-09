#ifndef LIBSITL_MRD_H
#define LIBSITL_MRD_H

#include <Sized.h>
#include "Config.h"
#include "Command.h"
#include "Sized.h"

namespace sitl::cmds
{

/**
 * @brief   Команда, выполняющая чтение одиночного слова данных в адресном
 *          пространстве памяти.
 */
class SITL_API Mrd : public Command
{
public:
    /**
     * @brief           Создаёт команду с определённым адресом и данными для
     *                  чтения.
     * @tparam TA       Тип данных адреса
     * @tparam TD       Тип данных слова данных
     * @param address   Адрес
     * @param dataWord  Словао данных
     */
    template <typename TA, typename TD>
    Mrd(const Address<TA> &address, const Sized<TD> &dataWord) :
        m_address{address.data},
        m_addressLength{address.hexLength},
        m_dataWord{0},
        m_dataWordSize{dataWord.size}
    {
    }


    /**
     * @brief   Возвращает прочитанное слово данных.
     *
     * Оно гарантированно будет не больше, чем максимально возможное значение
     * указанного в конструкторе типа данных слова данных.
     *
     * @return  Слово данных
     */
    uint64_t getDataWord() const;


    void encodeCommand(std::string &buffer) const override;
    Status handleResult(const std::string &line) override;

private:
    uint64_t m_address;
    size_t m_addressLength;

    uint64_t m_dataWord;
    size_t m_dataWordSize;
};

}

#endif //LIBSITL_MRD_H
