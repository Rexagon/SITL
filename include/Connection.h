#ifndef LIBSITL_CONNECTION_H
#define LIBSITL_CONNECTION_H

#include <string>
#include <memory>

#include <boost/asio/serial_port.hpp>

#include "Config.h"

namespace sitl
{
class SITL_API Connection
{
public:
    explicit Connection(const std::string& port);
    ~Connection();

private:
    boost::asio::io_context m_context;
    std::unique_ptr<boost::asio::serial_port> m_serialPort;
};
}

#endif //LIBSITL_CONNECTION_H
