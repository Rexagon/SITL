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


void Connection::serialPortRead(std::string &line, bool &hasRemaining)
{
    // После выполнения этой функции в response будет содержаться как минимум одна строка
    const auto bytesTransferred = asio::read_until(*m_serialPort, m_responseBuffer, '\n');

    // Записываем одну строку
    line = std::string{static_cast<const char *>(m_responseBuffer.data().data()), bytesTransferred - 1};

    // Убираем из буффера считанную строку
    m_responseBuffer.consume(bytesTransferred);

    // true если требуется повторный вызов
    hasRemaining = m_responseBuffer.size() > 0;
}


void Connection::serialPortRead(std::string &line, bool &hasRemaining, size_t timeout)
{
    // Настраиваем таймер для таймаута
    std::optional<system::error_code> timerResult;
    asio::deadline_timer timer{m_service};
    timer.expires_from_now(posix_time::seconds(timeout));
    timer.async_wait([&timerResult](const system::error_code &error) { timerResult = error; });

    // Настраиваем запись
    std::optional<std::pair<system::error_code, size_t>> readResult;
    asio::async_read_until(*m_serialPort, m_responseBuffer, '\n',
                           [&readResult](const system::error_code &error, size_t bytesTransferred) {
                               readResult = std::make_pair(error, bytesTransferred);
                           });

    // Запускаем операцию
    m_service.restart();
    while (m_service.run_one())
    {
        if (readResult)
        {
            const auto &[error, bytesTransferred] = *readResult;

            if (error)
            {
                throw std::runtime_error(readResult->first.message());
            }

            // Записываем одну строку
            line = std::string{static_cast<const char *>(m_responseBuffer.data().data()), bytesTransferred - 1};

            // Убираем из буффера считанную строку
            m_responseBuffer.consume(bytesTransferred);

            // true если требуется повторный вызов
            hasRemaining = m_responseBuffer.size() > 0;
        }
        else if (timerResult)
        {
            throw std::runtime_error{"Превышено время ожидания при записи"};
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


void Connection::serialPortWrite(const std::string &data, const size_t timeout)
{
    // Настраиваем таймер для таймаута
    std::optional<system::error_code> timerResult;
    asio::deadline_timer timer{m_service};
    timer.expires_from_now(posix_time::seconds(timeout));
    timer.async_wait([&timerResult](const system::error_code &error) { timerResult = error; });

    // Настраиваем запись
    std::optional<system::error_code> writeResult;
    asio::async_write(*m_serialPort, asio::buffer(data.c_str(), data.size()),
                      [&writeResult](const system::error_code &error, size_t) { writeResult = error; });

    // Запускаем операцию
    m_service.restart();
    while (m_service.run_one())
    {
        if (writeResult && *writeResult)
        {
            throw std::runtime_error(writeResult->message());
        }
        else if (timerResult)
        {
            throw std::runtime_error{"Превышено время ожидания при записи"};
        }
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
