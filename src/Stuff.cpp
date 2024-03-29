/*
 * This is an open source non-commercial project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
 */

#include <sitl/Stuff.h>

#include <stdexcept>
#include <unordered_map>

namespace sitl::stuff
{
uint64_t convertFromHex(const std::string_view hex)
{
    uint64_t result = 0;

    size_t size = hex.length();

    // Проходимся по шестнадцитиричным разрядам строки, одновременно ведя
    // счётчик для сдвига результата
    for (size_t i = 0, n = (size - 1) * 4; i < hex.length(); ++i, n -= 4)
    {
        // Приводим разряд к одному формату (uppercase)
        const auto &symbol = toupper(hex[i]);

        // Бросам исключение если символ не шестнадцатиричный
        if (!isdigit(symbol) && (symbol < 'A' || symbol > 'F'))
        {
            throw std::runtime_error{"Невозможно конвертировать hex строку в число"};
        }

        // Переводим шестнадцатиричный символ в число
        const auto number = static_cast<uint64_t>(isdigit(symbol) ? (symbol - '0') : (10 + (symbol - 'A')));

        // Добавляем это число на соответствующее место результата
        result |= (number & 0xfu) << n;
    }

    return result;
}


std::vector<std::string> split(const std::string &string, const std::string &delimiter)
{
    std::vector<std::string> result;

    size_t currentPos = 0;

    // Находим первый символ отличный от делителя
    while ((currentPos = string.find_first_not_of(delimiter, currentPos)) != std::string::npos)
    {
        // Если получилось найти

        // Находим делитель, начиная с позиции найденного ранее символа
        const auto it = string.find_first_of(delimiter, currentPos);

        // Добавляем в массив результатов слово между первым символом и делителем
        result.emplace_back(string.substr(currentPos, it - currentPos));

        // Сдвигаем позицию
        currentPos = it;
    }

    return result;
}


SITL_API std::string escaped(const std::string &string)
{
    static std::unordered_map<char, std::string> const escapes = {
        {'\a', "\\a"}, //  7, ^G, alert (bell)
        {'\b', "\\b"}, //  8, ^H, backspace
        {'\t', "\\t"}, //  9, ^I, tab
        {'\n', "\\n"}, // 10, ^J, newline / linefeed
        {'\v', "\\v"}, // 11, ^K, vertical tab
        {'\f', "\\f"}, // 12, ^L, formfeed
        {'\r', "\\r"}, // 13, ^M, carriage return
        {'\\', "\\\\"} // backslash
    };

    std::string result;
    for (const auto &symbol : string)
    {
        auto const it = escapes.find(symbol);
        if (it == escapes.end())
        {
            result += symbol;
        }
        else
        {
            result += it->second;
        }
    }

    return result;
}

} // namespace sitl::stuff
