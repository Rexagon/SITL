#ifndef LIBSITL_IDEN_H
#define LIBSITL_IDEN_H

#include "Command.h"
#include "Config.h"

namespace sitl::cmds
{
/**
 * @brief   Команда для получения сведения о поддерживаемой версии языка SITL
 *          (версии спецификации SITL) и данные о текущей конфигурации
 *          оборудования (SITL-ассемблере, SITL-процессоре, SITL-дизассемблере).
 */
class SITL_API Iden
{
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
    std::string m_info{};
};

} // namespace sitl::cmds

#endif // LIBSITL_IDEN_H
