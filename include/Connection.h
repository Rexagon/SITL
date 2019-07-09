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


    template <typename T, typename... Ts>
    typename T::ResultType execute(Ts&&... args)
    {
        T command(std::forward<Ts>(args)...);

        // Конвертируем и отправляем всю команду
        m_commandsBuffer.clear();
        command.encode(m_commandsBuffer);
        serialPortWrite(m_commandsBuffer);

        // Начинаем считывание
        bool isReading = true;
        while (isReading)
        {
            // Считываем одну строку результата
            m_resultsBuffer.clear();
            serialPortRead(m_resultsBuffer);

            // Обрабатываем строку
            const auto status = command.decodeLine(m_resultsBuffer);
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

private:
    void serialPortWrite(const std::string &data);
    size_t serialPortRead(std::string &line);

    std::mutex m_portMutex;

    boost::asio::io_service m_service;
    std::unique_ptr<boost::asio::serial_port> m_serialPort;

    std::string m_commandsBuffer;
    std::string m_resultsBuffer;
};

}

#endif //LIBSITL_CONNECTION_H
