#ifndef KERNEL_DRIVERS_SERIAL_H_
#define KERNEL_DRIVERS_SERIAL_H_

#include "kernel/lib/types.h"
#include <expected>
#include <string_view>

namespace serial
{
    enum class Error {
        InitFail
    };

    std::expected<void, Error> init();
    void write(u8 value);
    void write(char value);
    void write(std::string_view str);
    void write(int) = delete;
} // namespace serial

#endif // KERNEL_DRIVERS_SERIAL_H_