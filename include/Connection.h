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
     * @brief           Выполняет команду.
     *
     * При успешном выполнении команда будет помечена как completed и
     * все данные в ней должны быть установлены из результата.
     *
     * @param command   Комманда для выполнения
     */
    void makeTransaction(Command& command);

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
