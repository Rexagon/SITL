#ifndef LIBSITL_COMMAND_H
#define LIBSITL_COMMAND_H

#include <string>
#include <utility>
#include <functional>

#include "Config.h"
#include "Stuff.h"

namespace sitl
{

/**
 * @brief   Базовый класс для всех команд языка SITL.
 */
class SITL_API Command
{
public:
    /**
     * @brief Статус обработки команды.
     */
    enum Status
    {
        IN_PROCESS,     ///< Результат выполнения команды ещё обрабатывается
        FINISHED_DONE,  ///< Успешное завершение команды
        FINISHED_ERDON, ///< Завершение с ошибкой чётности
        FINISHED_ABORT, ///< Остановка цикла выполнения команды
        FINISHED_ERABR, ///< Остановка цикла выполнения команды с ошибкой чётности
        FINISHED_RETRY, ///< Запрос повтора транзакции
        FINISHED_ERRET  ///< Запрос повтора с ошибкой чётнсти
    };


    /**
     * @brief   Деструктор по умолчанию.
     */
    virtual ~Command() = default;


    /**
     * @brief           Записывает в буффер сообщений закодированную команду.
     * @param buffer    Буффер сообщений
     */
    virtual void encodeCommand(std::string &buffer) const = 0;


    /**
     * @brief           Обрабатывает одну строку результата.
     * @param line      Строка результата. Должна быть не короче 50 символов.
     * @return          Статус обраотки команды
     */
    virtual Status handleResult(const std::string& line) = 0;


    /**
     * @brief           Готовность данных команды.
     * @return          true, если команда обработана
     */
    bool isCompleted() const;

protected:
    friend class Connection;

    constexpr static size_t RESULT_LINE_LENGTH = 50;

    constexpr static size_t KEYWORD_BEGIN = 0;
    constexpr static size_t KEYWORD_LENGTH_MAX = 5;

    constexpr static size_t ADDRESS_BEGIN = 6;
    constexpr static size_t ADDRESS_LENGTH_MAX = 16;

    constexpr static size_t DATA_WORD_BEGIN = 23;
    constexpr static size_t DATA_WORD_LENGTH_MAX = 16;

    constexpr static size_t ORDER_BEGIN = 40;
    constexpr static size_t ORDER_LENGTH_MAX = 4;

    constexpr static size_t STATUS_BEGIN = 45;
    constexpr static size_t STATUS_LENGTH_MAX = 5;

    void markCompleted();

    static std::string_view extractKeyword(const std::string &line);
    static std::string_view extractAddress(const std::string &line);
    static std::string_view extractDataWord(const std::string &line);
    static std::string_view extractOrder(const std::string &line);
    static std::string_view extractStatus(const std::string &line);

    static Status statusFromString(std::string_view statusString);

private:
    bool m_isCompleted = false;
};

}

#endif //LIBSITL_COMMAND_H
