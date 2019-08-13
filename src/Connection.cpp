/*
 * This is an open source non-commercial project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
 */

#include <sitl/Connection.h>

#include <iostream>

#include <boost/asio/read_until.hpp>
#include <boost/asio/write.hpp>

using namespace boost;

namespace sitl
{
Connection::Connection(const std::string &port, const unsigned int baudRate, bool loggingEnabled)
    : m_service{}
    , m_serialPort{nullptr}
    , m_responseBuffer{}
    , m_isLoggingEnabled{loggingEnabled}
{
    m_serialPort = std::make_unique<asio::serial_port>(m_service, port);

    m_serialPort->set_option(asio::serial_port::baud_rate(baudRate));
    m_serialPort->set_option(asio::serial_port::flow_control(asio::serial_port::flow_control::none));
    m_serialPort->set_option(asio::serial_port::parity(asio::serial_port::parity::none));
    m_serialPort->set_option(asio::serial_port::stop_bits(asio::serial_port::stop_bits::one));
    m_serialPort->set_option(asio::serial_port::character_size(8));
}


bool Connection::serialPortRead(std::string &line)
{
    // После выполнения этой функции в response будет содержаться как минимум одна строка
    const auto bytesTransferred = asio::read_until(*m_serialPort, m_responseBuffer, '\n');

    // Записываем одну строку
    line = std::string{static_cast<const char *>(m_responseBuffer.data().data()), bytesTransferred - 1};

    // Убираем из буффера считанную строку
    m_responseBuffer.consume(bytesTransferred);

    // true если требуется повторный вызов
    return m_responseBuffer.size() > 0;
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


void Connection::log(const std::string &message)
{
    if (!m_isLoggingEnabled)
    {
        return;
    }

    std::cout << message << std::endl;
}

} // namespace sitl
