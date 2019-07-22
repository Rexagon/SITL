#ifndef LIBSITL_COMMAND_H
#define LIBSITL_COMMAND_H

#include <string>
#include <utility>
#include <functional>

#include "Config.h"
#include "Stuff.h"

namespace sitl::cmds
{
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


template<typename, typename = void>
struct IsCommand : std::false_type
{
};

template<typename T>
struct IsCommand<T, std::void_t<
    decltype(static_cast<std::string(T::*)() const>(&T::encode)),
    decltype(static_cast<Status(T::*)(const std::string &)>(&T::decodeLine)),
    decltype(std::declval<T>().getResult())
>>
    : std::true_type
{
};

template<typename T>
constexpr static bool is_command_v = IsCommand<T>::value;


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


/**
 * @brief       Выделяет ключевое слово из строки результата.
 * @param line  Строка результата
 * @return      Ключевое слово
 */
SITL_API std::string_view extractKeyword(const std::string &line);


/**
 * @brief       Выделяет адрес из строки результата.
 * @param line  Строка результата
 * @return      Адрес
 */
SITL_API std::string_view extractAddress(const std::string &line);


/**
 * @brief       Выделяет слово данных из строки результата.
 * @param line  Строка результата
 * @return      Слово данных
 */
SITL_API std::string_view extractDataWord(const std::string &line);


/**
 * @brief       Выделяет слово порядка из строки результата.
 * @param line  Строка результата
 * @return      Слово порядка
 */
SITL_API std::string_view extractOrder(const std::string &line);


/**
 * @brief       Выделяет статус из строки результата.
 * @param line  Строка результата
 * @return      Статус
 */
SITL_API std::string_view extractStatus(const std::string &line);


/**
 * @brief               Преобразует строку со статусом в enum
 * @param statusString  Строка со статусом
 * @return              enum Status
 */
SITL_API Status statusFromString(std::string_view statusString);

}

#endif //LIBSITL_COMMAND_H
