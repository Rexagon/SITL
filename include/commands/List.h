#ifndef LIBSITL_LIST_H
#define LIBSITL_LIST_H

#include <vector>
#include <unordered_map>

#include "Config.h"
#include "Command.h"

namespace sitl::cmds
{

/**
 * @brief   Команда для полечения списка команд, поддерживаемых текущей
 *          конфигурацией оборудования.
 */
class SITL_API List
{
public:
    /**
     * @brief Режим исполения команды.
     */
    enum Type
    {
        FULL_INFO,  ///< Команда выполняется 2 раза, получая всю необходимую информацию.
        SINGLE_PING ///< Команда выполняется 1 раз, не обрабатывая полученный результат.
                    ///< В таком режиме можно с некоторой периодичностью проверять
                    ///< работоспособность устройства.
    };


    /**
     * @brief Информация о команде
     */
    struct CommandInfo
    {
        //TODO: указать здесь возможные разрядности и т.д.
    };


    using ResultType = const std::unordered_map<std::string, CommandInfo>&;


    /**
     * @brief   Команда LIST в одном из 2х режимов.
     * @param type  Режим исполнения команды.
     */
    explicit List(Type type = Type::FULL_INFO);


    void encode(std::string &buffer) const;


    Status decodeLine(const std::string &line);


    /**
     * @brief   Возвращает таблицу с возможными конфигурациями команд.
     *          Ключом является название команды.
     *
     * Если команда была создана в режиме SINGLE_PING, то таблица всё время
     * будет пустой.
     *
     * @return  Таблица с возможными конфигурациями команд.
     */
    ResultType getResult() const;

private:
    Type m_type;
    std::unordered_map<std::string, CommandInfo> m_availableCommands{};
};

}

#endif //LIBSITL_LIST_H
