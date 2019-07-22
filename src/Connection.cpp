/*
 * This is an open source non-commercial project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
 */

#include <sitl/Connection.h>

#include <boost/asio/read_until.hpp>
#include <boost/asio/streambuf.hpp>
#include <boost/asio/write.hpp>

using namespace boost;

namespace sitl
{
Connection::Connection(const std::string &port, const unsigned int baudRate)
    : m_service{}
    , m_serialPort{nullptr}
{
    m_serialPort = std::make_unique<asio::serial_port>(m_service, port);

    m_serialPort->set_option(asio::serial_port::baud_rate(baudRate));
    m_serialPort->set_option(asio::serial_port::flow_control(asio::serial_port::flow_control::none));
    m_serialPort->set_option(asio::serial_port::parity(asio::serial_port::parity::none));
    m_serialPort->set_option(asio::serial_port::stop_bits(asio::serial_port::stop_bits::one));
    m_serialPort->set_option(asio::serial_port::character_size(8));
}


size_t Connection::serialPortRead(std::string &line)
{
    asio::streambuf response;

    asio::read_until(*m_serialPort, response, '\n');

    const std::string resp{(std::istreambuf_iterator<char>(&response)), std::istreambuf_iterator<char>()};
    line = resp;

    return resp.length();
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

} // namespace sitl
