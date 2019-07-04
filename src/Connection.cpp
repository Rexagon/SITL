/*
* This is an open source non-commercial project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
*/

#include "Connection.h"

using namespace boost;

namespace sitl
{

Connection::Connection(const std::string& port) :
    m_context{},
    m_serialPort{nullptr}
{
    m_serialPort = std::make_unique<asio::serial_port>(m_context, port);

    m_context.run();
}

Connection::~Connection()
{
}


}