/*
* This is an open source non-commercial project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
*/

#include "Connection.h"

#include <boost/asio/read.hpp>
#include <boost/asio/write.hpp>

#include "commands/Mwr.h"
#include "commands/Mrd.h"

using namespace boost;

namespace sitl
{

Connection::Connection(const std::string &port, unsigned int baudRate) :
    m_service{},
    m_serialPort{nullptr}
{
    m_serialPort = std::make_unique<asio::serial_port>(m_service, port);

    m_serialPort->set_option(asio::serial_port::baud_rate(baudRate));
    m_serialPort->set_option(asio::serial_port::flow_control(asio::serial_port::flow_control::none));
    m_serialPort->set_option(asio::serial_port::parity(asio::serial_port::parity::none));
    m_serialPort->set_option(asio::serial_port::stop_bits(asio::serial_port::stop_bits::one));
    m_serialPort->set_option(asio::serial_port::character_size(8));;
}


void Connection::writeMemory(uint32_t address, uint32_t data)
{
    execute<cmds::Mwr<uint32_t, uint32_t>>(address, data);
}


uint32_t Connection::readMemory(uint32_t address)
{
    return execute<cmds::Mrd<uint32_t, uint32_t>>(address);
}


size_t Connection::serialPortRead(std::string &line)
{
    size_t symbolCount = 0;

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
                return symbolCount;
            case '\r':
                break;
            default:
                ++symbolCount;
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
