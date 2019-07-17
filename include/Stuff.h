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
 * Можно использовать вместе со static_assert для ограничения передаваемых
 * типов
 *
 * @param T     Проверяемый тип
 * @param Ts    Множество типов
 */
template<typename T, typename... Ts>
struct is_any_of : std::bool_constant<(std::is_same_v<T, Ts> || ...)> {};


/**
 * @brief Вспомогательная метафункция для проверки вхождения типа в множество
 *
 * Можно использовать вместе со static_assert для ограничения передаваемых
 * типов
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
std::string convertToHex(const T number, const size_t length = sizeof(T) * 2)
{
    static auto digits = "0123456789ABCDEF";
    std::string result(length, '0');

    for (size_t i = 0, n = (length - 1) * 4; i < length; ++i, n -= 4)
    {
        result[i] = digits[(static_cast<T>(number) >> n) & 0xfu];
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

}

#endif //LIBSITL_STUFF_H
