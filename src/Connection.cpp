/*
 * This is an open source non-commercial project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
 */

#include <sitl/Connection.h>

#include <iostream>

#include <boost/asio/deadline_timer.hpp>
#include <boost/asio/read_until.hpp>
#include <boost/asio/write.hpp>

using namespace boost;

namespace sitl
{
Connection::Connection(const std::string &port, const unsigned int baudRate, bool loggingEnabled)
    : m_isLoggingEnabled{loggingEnabled}
{
    m_serialPort = std::make_unique<asio::serial_port>(m_service, port);

    m_serialPort->set_option(asio::serial_port::baud_rate(baudRate));
    m_serialPort->set_option(asio::serial_port::flow_control(asio::serial_port::flow_control::none));
    m_serialPort->set_option(asio::serial_port::parity(asio::serial_port::parity::none));
    m_serialPort->set_option(asio::serial_port::stop_bits(asio::serial_port::stop_bits::one));
    m_serialPort->set_option(asio::serial_port::character_size(8));
}


void Connection::setResponseTimeout(const size_t timeout_s)
{
    m_responseTimeout_s = timeout_s;
}


size_t Connection::getResponseTimeout() const
{
    return m_responseTimeout_s;
}


void Connection::serialPortRead(std::string &line, bool &hasRemaining, size_t timeout)
{
    // Настраиваем таймер

    asio::deadline_timer timer{m_service};
    std::optional<system::error_code> timerResult;

    const auto onTimeout = [this, &timerResult](const system::error_code &error) {
        if (error != asio::error::operation_aborted)
        {
            log("Reading timeout reached");
            timerResult = error;
            m_serialPort->cancel();
        }
    };

    timer.expires_from_now(posix_time::seconds(timeout));
    timer.async_wait(onTimeout);

    // Настраиваем запись
    std::optional<std::pair<system::error_code, size_t>> readResult;

    const auto onComplete = [this, &timer, &readResult](const system::error_code &error, size_t bytesTransferred) {
        readResult = std::make_pair(error, bytesTransferred);
        timer.cancel();
    };

    asio::async_read_until(*m_serialPort, m_responseBuffer, '\n', onComplete);

    // Запускаем цикл обработки пока не завершатся операции
    m_service.restart();
    m_service.run();

    // Обрабатываем результаты
    if (timerResult)
    {
        throw std::runtime_error{"Превышено время ожидания при записи"};
    }

    const auto &[error, bytesTransferred] = *readResult;

    if (error)
    {
        throw std::runtime_error(readResult->first.message());
    }

    // Записываем одну строку
    line = std::string{static_cast<const char *>(m_responseBuffer.data().data()), bytesTransferred};

    // Убираем из буффера считанную строку
    m_responseBuffer.consume(bytesTransferred);

    // true если требуется повторный вызов
    hasRemaining = m_responseBuffer.size() > 0;
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
