#ifndef KERNEL_CPU_ASM_H_
#define KERNEL_CPU_ASM_H_

#include "kernel/lib/types.h"
#include <utility>

namespace cpu
{
    inline void idle()
    {
        __asm__ volatile ("hlt");
    }

    enum class IOPort : u16 {};

    inline constexpr IOPort operator +(IOPort port, u64 offset)
    {
        return static_cast<IOPort>(std::to_underlying(port) + offset);
    }

    inline void output_byte(IOPort port, u8 value)
    {
        __asm__ volatile (
            "outb %b0, %w1"
            :
            : "a"(value), "Nd"(std::to_underlying(port))
            : "memory"
        );
    }

    inline u8 input_byte(IOPort port)
    {
        u8 value;
        __asm__ volatile (
            "inb %w1, %b0"
            : "=a"(value)
            : "Nd"(std::to_underlying(port))
            : "memory"
        );
        return value;
    }
} // namespace cpu

#endif // KERNEL_CPU_ASM_H_