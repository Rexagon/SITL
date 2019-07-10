#ifndef LIBSITL_CONNECTION_H
#define LIBSITL_CONNECTION_H

#include <chrono>
#include <string>
#include <memory>
#include <mutex>
#include <vector>

#include <boost/asio/io_service.hpp>
#include <boost/asio/serial_port.hpp>

#include "Config.h"
#include "Command.h"

namespace sitl
{

/**
 * @brief   Соединение с устройством по протоколу SITL через serial port.
 */
class SITL_API Connection final
{
public:
    /**
     * @brief           Открывает соединение на указанном порту.
     * @param port      Название порта для установки соединения
     * @param baudRate  Скорость передачи
     */
    explicit Connection(const std::string &port, unsigned int baudRate);


    /**
     * @brief           Записывает данные по указанному адреса
     * @param address   Адрес (32 разряда)
     * @param data      Слово данных (32 разряда)
     */
    void writeMemory(uint32_t address, uint32_t data);


    /**
     * @brief           Считывает данные по указанному адресу
     * @param address   Адрес (32 разряда)
     * @return          Слово данных (32 разряда)
     */
    uint32_t readMemory(uint32_t address);


    /**
     * @brief           Выполняет указанную команду.
     *
     * Класс команды должен выглядеть примерно так (то есть содержать указанные
     * методы):
     *
     * @code{.cpp}
     *
     * class CMD {
     * public:
     *     std::string encode();
     *
     *     Status decodeLine(const std::string& line);
     *
     *     ANY_NEEDED_TYPE getResult();
     * };
     *
     * @endcode
     *
     * Функция encode() должна возвращать строку, с закодированной командой.
     * Например для MRD с адресом 0xFFAA и размером данных 1 байт она должна
     * возвращать 'MRW FFAA -D8'.
     *
     * Функция decodeLine() нужна для построчной обработки приходящего
     * результата. Команда должна выделять из строки необходимые данные и
     * обрабатывать их. Функция будет вызываеться для вновь пришедших от
     * устройства данных до тех пор, пока она не вернёт статус отличный от
     * IN_PROCESS.
     *
     * Тип данных, возвращаемый методом getResult() будет определять что вернёт
     * функция Connection::execute() при успешной обработки результата. Метод
     * getResult() не должен возвращать ссылки или указатели, время жизни
     * которых не больше времени жизни самой команды. Т.к. команда создаётся
     * только на время выполнения метода Connection::execute().
     *
     * @tparam T        Класс команды
     * @param args      Аргументы конструктора указанной команды
     * @return          При успешной обработке верёт результат вызова метода
     *                  getResult() у команды
     */
    template <typename T, typename... Ts>
    auto execute(Ts&&... args) -> auto;

private:
    /**
     * @brief       Отправляет на устройство полностью всю строку.
     * @param data  Строка данных
     */
    void serialPortWrite(const std::string &data);


    /**
     * @brief       Считывает одну строку (читает все данные из буффера, пока
     *              не встретит \n).
     * @param line  Буффер, куда допишется считанная строка. Причём строка точно
     *              не будет содержать символы \n и \r
     * @return      Длина считанной строки
     */
    size_t serialPortRead(std::string &line);


    boost::asio::io_service m_service;
    std::unique_ptr<boost::asio::serial_port> m_serialPort;
};


template<typename T, typename... Ts>
auto Connection::execute(Ts &&... args) -> auto
{
    T command(std::forward<Ts>(args)...);

    // Конвертируем и отправляем всю команду
    auto buffer = command.encode();
    serialPortWrite(buffer);

    // Начинаем считывание
    bool isReading = true;
    while (isReading)
    {
        // Считываем одну строку результата
        buffer.clear();
        serialPortRead(buffer);

        // Обрабатываем строку
        const auto status = command.decodeLine(buffer);
        switch (status)
        {
            case cmds::Status::IN_PROCESS:
                break;

            case cmds::Status::FINISHED_DONE:
                isReading = false;
                break;

                // TODO: проверить другие коды

            default:
                throw std::runtime_error{"Операция завершена с ошибкой"};
        }
    }

    return command.getResult();
}

}

#endif //LIBSITL_CONNECTION_H
