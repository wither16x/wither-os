#ifndef KERNEL_LOG_H_
#define KERNEL_LOG_H_

#include "kernel/lib/print.h"
#include <string_view>

enum class LogLevel {
    Debug,
    Note,
    Ok,
    Warn,
    Err,
    Fatal
};

template<typename... Args>
void log(LogLevel level, std::string_view str, Args &&...args)
{
    switch (level) {
    case LogLevel::Debug:
        print("\033[37mDEBUG\033[0m: ");
        break;
    case LogLevel::Note:
        print("\033[36mNOTE\033[0m: ");
        break;
    case LogLevel::Ok:
        print("\033[32mOK\033[0m: ");
        break;
    case LogLevel::Warn:
        print("\033[33mWARN\033[0m: ");
        break;
    case LogLevel::Err:
        print("\033[31mERR\033[0m: ");
        break;
    case LogLevel::Fatal:
        print("\033[35mFATAL\033[0m: ");
        break;
    }

    print(str, args...);
    print("\r\n");
}

template<typename... Args>
void debug(std::string_view str, Args &&...args)
{
    log(LogLevel::Debug, str, args...);
}

template<typename... Args>
void note(std::string_view str, Args &&...args)
{
    log(LogLevel::Note, str, args...);
}

template<typename... Args>
void ok(std::string_view str, Args &&...args)
{
    log(LogLevel::Ok, str, args...);
}

template<typename... Args>
void warn(std::string_view str, Args &&...args)
{
    log(LogLevel::Warn, str, args...);
}

template<typename... Args>
void err(std::string_view str, Args &&...args)
{
    log(LogLevel::Err, str, args...);
}

template<typename... Args>
void fatal(std::string_view str, Args &&...args)
{
    log(LogLevel::Fatal, str, args...);
}

#endif // KERNEL_LOG_H_