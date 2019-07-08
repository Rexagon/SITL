#ifndef LIBSITL_COMMAND_H
#define LIBSITL_COMMAND_H

#include <string>
#include <utility>
#include <functional>

#include "Config.h"
#include "Stuff.h"

namespace sitl
{

template <typename T>
struct Address : stuff::SizedData<T> {};

template <typename T>
struct DataWord : stuff::SizedData<T> {};

class SITL_API Command
{
public:
    enum Status
    {
        IN_PROCESS,
        FINISHED_DONE,
        FINISHED_ERDON,
        FINISHED_ABORT,
        FINISHED_ERABR,
        FINISHED_RETRY,
        FINISHED_ERRET
    };

    Command();

    virtual ~Command() = default;

    virtual void encodeCommand(std::string &buffer) const = 0;

    virtual Status handleResult(const std::string& line) = 0;

    bool isCompleted() const;

protected:
    friend class Connection;

    constexpr static size_t KEYWORD_BEGIN = 0;
    constexpr static size_t KEYWORD_END = 4;
    constexpr static size_t ADDRESS_BEGIN = 6;
    constexpr static size_t ADDRESS_END = 21;
    constexpr static size_t DATA_BEGIN = 23;
    constexpr static size_t DATA_END = 38;
    constexpr static size_t ORDER_BEGIN = 40;
    constexpr static size_t ORDER_END = 43;
    constexpr static size_t STATUS_BEGIN = 45;
    constexpr static size_t STATUS_END = 49;

    void markCompleted();

    static Status parseStatus(const std::string& statusString);

private:
    bool m_isCompleted;
};

}

#endif //LIBSITL_COMMAND_H
