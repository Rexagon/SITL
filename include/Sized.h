#ifndef LIBSITL_SIZED_H
#define LIBSITL_SIZED_H

#include "Config.h"
#include "Stuff.h"

namespace sitl
{

/**
 * @brief Обёртка для аргументов template конструктора
 */
template <typename T>
struct SITL_API Sized
{
    static_assert(stuff::is_any_of_v<T, uint8_t, uint16_t, uint32_t, uint64_t>);

    constexpr static size_t size = sizeof(T);
    constexpr static size_t hexLength = sizeof(T) * 2;
};


/**
 * @brief Обёртка для аргументов template конструктора
 */
template <typename T>
struct SITL_API SizedData : Sized<T>
{
    T data;
};


/**
 * @brief Обёртка для адреса
 */
template <typename T>
struct SITL_API Address : SizedData<T> {
    explicit Address(T data) : SizedData<T>{{}, data} {}
};


/**
 * @brief Обёртка для слова данных
 */
template <typename T>
struct SITL_API DataWord : SizedData<T> {
    explicit DataWord(T data) : SizedData<T>{{}, data} {}
};

}

#endif //LIBSITL_SIZED_H
