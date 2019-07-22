#ifndef LIBSITL_LIST_H
#define LIBSITL_LIST_H

#include <unordered_map>
#include <vector>

#include "Command.h"
#include "Config.h"

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
     * @brief   Режим исполения команды.
     */
    enum Type
    {
        FULL_INFO, ///< Команда выполняется 2 раза, получая всю необходимую информацию.
        SINGLE_PING ///< Команда выполняется 1 раз, не обрабатывая полученный результат.
                    ///< В таком режиме можно с некоторой периодичностью проверять
                    ///< работоспособность устройства.
    };


    /**
     * @brief   Информация о команде
     */
    struct CommandInfo
    {
        // TODO: указать здесь возможные разрядности и т.д.
    };


    /**
     * @brief           Команда LIST в одном из 2х режимов.
     * @param type      Режим исполнения команды.
     */
    explicit List(Type type = Type::FULL_INFO);


    /**
     * @brief           Кодирует команду.
     * @return          Строка с командой SITL
     */
    std::string encode() const;


    /**
     * @brief           Обрабатывает строку результата.
     * @param line      Строка результата
     * @return          Статус обработки
     */
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
    std::unordered_map<std::string, CommandInfo> getResult() const;

private:
    Type m_type;
    std::unordered_map<std::string, CommandInfo> m_availableCommands{};
};

} // namespace sitl::cmds

#endif // LIBSITL_LIST_H
