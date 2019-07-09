#ifndef LIBSITL_STUFF_H
#define LIBSITL_STUFF_H

#include <vector>
#include <string>

#include "Config.h"

namespace sitl::stuff
{

/**
 * @brief Вспомогательная метафункция для проверки вхождения типа в множество
 *
 * @param T     Проверяемый тип
 * @param Ts    Множество типов
 */
template<typename T, typename... Ts>
struct is_any_of : std::bool_constant<(std::is_same_v<T, Ts> || ...)> {};


/**
 * @brief Вспомогательная метафункция для проверки вхождения типа в множество
 *
 * @param T     Проверяемый тип
 * @param Ts    Множество типов
 */
template<typename T, typename T1, typename... Ts>
inline constexpr bool is_any_of_v = is_any_of<T, T1, Ts...>::value;


/**
 * @brief Конвертирует число в строку в шестнадцатиричном коде с ведущими нулями
 * @tparam T        Тип числа
 * @param number    Число
 * @param length    Длина итоговой строки. По умолчанию - удвоенный размер типа
 *                  числа
 * @return          Строка в шестнадцатиричном коде с ведущими нулями
 *                  представляющая переданное число
 */
template <typename T>
std::string convertToHex(T number, size_t length = sizeof(T) * 2)
{
    static auto digits = "0123456789ABCDEF";
    std::string result(length, '0');

    for (size_t i = 0, n = (length - 1) * 4; i < length; ++i, n -= 4)
    {
        result[i] = digits[(number >> n) & 0xfu];
    }

    return result;
}


/***
 * @brief Конвертирует строку в шестнадцатиричном коде в число
 * @param hex       Строка в шестнадцатиричном коде
 * @return          Число из строки
 * @throws          std::runtime_error если в строке были символы не
 *                  шестнадцатиричные символы
 */
SITL_API uint64_t convertFromHex(std::string_view hex);


/**
 * @brief Разбивает одну строку на массив из нескольких с помощью делителя
 *
 * @param string    Исходная строка, которую надо разбить
 * @param delimiter Делитель
 * @return          Массив из элементов исходной строки. Ни один элемент не
 *                  содержит делитель
 */
SITL_API std::vector<std::string> split(const std::string &string, char delimiter);


/**
 * @brief Добавление к строке нескольких кусков строк
 *
 * Это работает без создания временных объектов, как например здесь:
 * @code buffer += "temp" + std::string{"asd"} + "anther";
 *
 * @tparam T        Тип первого аргемента
 * @tparam Ts       Типы остальных аргументов
 * @param buffer    Буффер, в который будут добавлятся куски строк
 * @param arg       Первый аргумент. Всегда должен быть
 * @param args      Остальные аргументы. Их может и не быть
 */
template <typename T, typename... Ts>
void append(std::string &buffer, T&& arg, Ts&&... args)
{
    using dummy = int[];
    static_cast<void>(dummy {
        (static_cast<void>(buffer.append(std::forward<T>(arg))), 0),
        (static_cast<void>(buffer.append(std::forward<Ts>(args))), 0)...
    });
}


/**
 * @brief Добавление к строке нескольких кусков строк и одного символа переноса
 *
 * Это работает без создания временных объектов, как например здесь:
 * @code buffer += "temp" + std::string{"asd"} + "anther\n";
 *
 * @tparam T        Тип первого аргемента
 * @tparam Ts       Типы остальных аргументов
 * @param buffer    Буффер, в который будут добавлятся куски строк
 * @param arg       Первый аргумент. Всегда должен быть
 * @param args      Остальные аргументы. Их может и не быть
 */
template <typename T, typename... Ts>
void appendLine(std::string &buffer, T&& arg, Ts&&... args)
{
    append(buffer, std::forward<T>(arg), std::forward<Ts>(args)...);
    buffer.append(1, '\n');
}

}

#endif //LIBSITL_STUFF_H
