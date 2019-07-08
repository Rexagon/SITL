/*
* This is an open source non-commercial project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
*/

#include "Connection.h"

#include <boost/asio/read.hpp>
#include <boost/asio/write.hpp>

using namespace boost;

namespace sitl
{

Connection::Connection(const std::string &port, unsigned int baudRate) :
    m_portMutex{},
    m_service{},
    m_serialPort{nullptr},
    m_commandsBuffer{},
    m_resultsBuffer{}
{
    m_serialPort = std::make_unique<asio::serial_port>(m_service, port);

    m_serialPort->set_option(asio::serial_port::baud_rate(baudRate));
    m_serialPort->set_option(asio::serial_port::flow_control(asio::serial_port::flow_control::none));
    m_serialPort->set_option(asio::serial_port::parity(asio::serial_port::parity::none));
    m_serialPort->set_option(asio::serial_port::stop_bits(asio::serial_port::stop_bits::one));
    m_serialPort->set_option(asio::serial_port::character_size(8));

    m_commandsBuffer.reserve(128);
    m_resultsBuffer.reserve(128);
}


void Connection::makeTransaction(sitl::Command &command)
{
    // write command
    m_commandsBuffer.clear();
    command.encodeCommand(m_commandsBuffer);
    serialPortWrite(m_commandsBuffer);

    // read command results
    bool isReading = true;
    while(isReading)
    {
        m_resultsBuffer.clear();
        serialPortRead(m_resultsBuffer);

        const auto status = command.handleResult(m_resultsBuffer);
        switch (status)
        {
            case Command::IN_PROCESS:
                break;

            case Command::FINISHED_DONE:
                command.markCompleted();
                m_resultsBuffer.clear();
                isReading = false;
                break;

            //TODO: проверить другие коды

            default:
                throw std::runtime_error{
                    "Операция завершена с ошибкой"
                };
        }
    }
}


void Connection::serialPortRead(std::string &line)
{
    while (true)
    {
        system::error_code error;

        char symbol = 0;
        asio::read(*m_serialPort, asio::buffer(&symbol, 1), error);

        if (error)
        {
            throw std::runtime_error(error.message());
        }

        switch (symbol)
        {
            case '\n':
                return;
            case '\r':
                break;
            default:
                line += symbol;
                break;
        }
    }
}


void Connection::serialPortWrite(const std::string &data)
{
    system::error_code error;
    asio::write(*m_serialPort, asio::buffer(data.c_str(), data.size()), error);

    if (error)
    {
        throw std::runtime_error(error.message());
    }
}

}
