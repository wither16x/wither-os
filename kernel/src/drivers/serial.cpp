#include "kernel/drivers/serial.h"
#include "kernel/cpu/asm.h"
#include <expected>

namespace serial
{
    namespace
    {
        constexpr cpu::IOPort COM1{0x3f8};
    } // anonymous namespace

    std::expected<void, Error> init()
    {
        cpu::output_byte(COM1 + 1, 0x00);
        cpu::output_byte(COM1 + 3, 0x80);
        cpu::output_byte(COM1, 0x03);
        cpu::output_byte(COM1 + 1, 0x00);
        cpu::output_byte(COM1 + 3, 0x03);
        cpu::output_byte(COM1 + 2, 0xc7);
        cpu::output_byte(COM1 + 4, 0x0b);
        cpu::output_byte(COM1 + 4, 0x1e);

        cpu::output_byte(COM1, 0xae);
        if (cpu::input_byte(COM1) != 0xae)
            return std::unexpected(Error::InitFail);

        cpu::output_byte(COM1 + 4, 0x0f);
        return {};
    }

    void write(u8 value)
    {
        while ((cpu::input_byte(COM1 + 5) & 0x20) == 0);
        cpu::output_byte(COM1, value);
    }

    void write(char value)
    {
        write(static_cast<u8>(value));
    }

    void write(std::string_view str)
    {
        for (auto ch : str)
            write(ch);
    }
} // namespace serial