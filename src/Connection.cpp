/*
* This is an open source non-commercial project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
*/

#include "Connection.h"

#include <condition_variable>

using namespace boost;

namespace sitl
{

Connection::Connection(const std::string &port) :
    m_portMutex{},
    m_context{},
    m_serialPort{nullptr},
    m_buffer{64}
{
    m_serialPort = std::make_unique<asio::serial_port>(m_context);

    // TODO: назначить параметры подключения
    // m_serialPort->set_option(asio::serial_port::baud_rate,       );
    // m_serialPort->set_option(asio::serial_port::flow_control,    );
    // m_serialPort->set_option(asio::serial_port::parity,          );
    // m_serialPort->set_option(asio::serial_port::stop_bits,       );
    // m_serialPort->set_option(asio::serial_port::character_size,  );

    m_serialPort->open(port);
}


void Connection::makeTransaction(sitl::Command &command)
{
    // write command
    m_buffer.clear();
    command.encode(m_buffer);
    serialPortWrite(m_buffer);

    // read command
    serialPortRead(m_buffer);
    command.decodeResult(m_buffer);
}


void Connection::serialPortRead(std::string &data)
{
    std::condition_variable readSignal;

    bool finished = false;

    struct {
        system::error_code error{};
        size_t bytesRead{0};
    } result;

    m_serialPort->async_read_some(asio::buffer(data), [&](const system::error_code& error, size_t bytesRead) {
        result = { error, bytesRead };
        finished = true;
        readSignal.notify_one();
    });

    {
        std::unique_lock lock(m_portMutex);
        readSignal.wait(lock, [&]{ return finished; });
    }

    if (result.error)
    {
        throw std::runtime_error(result.error.message());
    }
}


void Connection::serialPortWrite(const std::string &data)
{
    system::error_code error;
    m_serialPort->write_some(asio::buffer(data), error);

    if (error)
    {
        throw std::runtime_error(error.message());
    }
}

}
