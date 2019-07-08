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

class SITL_API Connection final
{
public:
    explicit Connection(const std::string &port, unsigned int baudRate);
    virtual ~Connection() = default;

    void makeTransaction(Command& command);

private:
    void serialPortWrite(const std::string &data);
    void serialPortRead(std::string &line);

    std::mutex m_portMutex;

    boost::asio::io_service m_service;
    std::unique_ptr<boost::asio::serial_port> m_serialPort;

    std::string m_commandsBuffer;
    std::string m_resultsBuffer;
};

}

#endif //LIBSITL_CONNECTION_H
