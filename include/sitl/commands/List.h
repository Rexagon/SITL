#ifndef LIBSITL_LIST_H
#define LIBSITL_LIST_H

#include <unordered_map>
#include <vector>

#include <sitl/Command.h>
#include <sitl/Config.h>

namespace sitl::cmds
{
/**
 * @brief   Команда для полечения списка команд, поддерживаемых текущей
 *          конфигурацией оборудования.
 */
class SITL_API List
{
    enum State
    {
        None,
        First,
        Second
    };

public:
    /**
     * @brief   Информация о команде
     */
    struct CommandInfo
    {
        // TODO: указать здесь возможные разрядности и т.д.
    };


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
    void incrementState();
    void incrementLine();

    std::unordered_map<std::string, CommandInfo> m_availableCommands{};

    State m_state = State::None;
    std::pair<size_t, size_t> m_lineRead = {0, 0};
};

} // namespace sitl::cmds

#endif // LIBSITL_LIST_H
