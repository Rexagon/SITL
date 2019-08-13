#ifndef LIBSITL_CONNECTION_H
#define LIBSITL_CONNECTION_H

#include <algorithm>
#include <chrono>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

#include <boost/asio/io_service.hpp>
#include <boost/asio/serial_port.hpp>
#include <boost/asio/streambuf.hpp>

#include <sitl/Command.h>
#include <sitl/Config.h>

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
     * @param loggingEnabled Выводить ли все передаваемые и получаемые команды
     */
    explicit Connection(const std::string &port, unsigned int baudRate, bool logginEnabled = false);


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
    auto execute(Ts &&... args) -> auto;

private:
    /**
     * @brief       Отправляет на устройство полностью всю строку.
     * @param data  Строка данных
     */
    void serialPortWrite(const std::string &data);


    /**
     * @brief       Считывает одну строку (читает все данные из буффера, пока
     *              не встретит \n).
     * @param line  Буффер, в который допишется считанная строка.
     * @return      true, если требуется повторный вызов
     */
    bool serialPortRead(std::string &line);

    void log(const std::string &message);


    boost::asio::io_service m_service;
    std::unique_ptr<boost::asio::serial_port> m_serialPort;

    boost::asio::streambuf m_responseBuffer;

    bool m_isLoggingEnabled;
};


template <typename T, typename... Ts>
auto Connection::execute(Ts &&... args) -> auto
{
    static_assert(cmds::is_command_v<T>, "Class T doesn't not implement command trait.\n"
                                         "See Connection::execute comments for detailed explanation");

    T command(std::forward<Ts>(args)...);

    // Конвертируем и отправляем всю команду
    auto buffer = command.encode();
    serialPortWrite(buffer);
    log("Sent:\t\t" + buffer);

    // Начинаем считывание
    bool isReading = true;
    while (isReading)
    {
        // Считываем результат
        while (true)
        {
            bool hasRemaining = false;
            serialPortRead(buffer, hasRemaining);

            // Очищаем от лишних символов
            buffer.erase(std::remove(buffer.begin(), buffer.end(), '\r'), buffer.end());

            log("Received:\t" + buffer);

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

            if (!hasRemaining)
            {
                break;
            }
        }
    }

    return command.getResult();
}

} // namespace sitl

#endif // LIBSITL_CONNECTION_H
