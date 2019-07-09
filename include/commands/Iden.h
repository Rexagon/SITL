#ifndef LIBSITL_IDEN_H
#define LIBSITL_IDEN_H

#include "Config.h"
#include "Command.h"

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
    using ResultType = const std::string&;


    void encode(std::string &buffer) const;


    Status decodeLine(const std::string& line);


    /**
     * @brief   Сведения о поддерживаемой версии языка SITL и конфигурации
     *          оборудования
     * @return  Текст из нескольких строк
     */
    ResultType getResult() const;

private:
    std::string m_info{};
};

}

#endif //LIBSITL_IDEN_H
