#ifndef LIBSITL_IDEN_H
#define LIBSITL_IDEN_H

#include <sitl/Command.h>
#include <sitl/Config.h>

namespace sitl::cmds
{
/**
 * @brief   Команда для получения сведения о поддерживаемой версии языка SITL
 *          (версии спецификации SITL) и данные о текущей конфигурации
 *          оборудования (SITL-ассемблере, SITL-процессоре, SITL-дизассемблере).
 */
class SITL_API Iden
{
    enum State
    {
        None,
        First,
        Second
    };

public:
    /**
     * @brief       Кодирует команду.
     * @return      Строка с командой SITL
     */
    std::string encode() const;


    /**
     * @brief       Обрабатывает строку результата.
     * @param line  Строка результата
     * @return      Статус обработки
     */
    Status decodeLine(const std::string &line);


    /**
     * @brief   Сведения о поддерживаемой версии языка SITL и конфигурации
     *          оборудования
     * @return  Текст из нескольких строк
     */
    std::string getResult() const;

private:
    void incrementState();
    void incrementLine();

    std::string m_info{};

    State m_state = State::None;
    std::pair<size_t, size_t> m_lineRead = {0, 0};
};

} // namespace sitl::cmds

#endif // LIBSITL_IDEN_H
