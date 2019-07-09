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
class SITL_API Iden : public Command
{
public:
    /**
     * @brief   Сведения о поддерживаемой версии языка SITL и конфигурации
     *          оборудования
     * @return  Текст из нескольких строк
     */
    const std::string& getInfo() const;


    void encodeCommand(std::string &buffer) const override;
    Status handleResult(const std::string& line) override;

private:
    std::string m_info{};
};

}

#endif //LIBSITL_IDEN_H
