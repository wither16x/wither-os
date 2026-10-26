#ifndef KERNEL_PANIC_H_
#define KERNEL_PANIC_H_

#include "kernel/lib/print.h"
#include "kernel/cpu/asm.h"
#include <string_view>

template<typename... Args>
void panic(std::string_view str, Args &&...args)
{
    print("!!! KERNEL PANIC !!!\r\n");
    print("The kernel ran into an unrecoverable error.\r\n");
    print("Because: ");
    print(str, args...);
    print("\r\n");
    print("Please reboot your device.\r\n");

    while (true)
        cpu::idle();
}

#endif // KERNEL_PANIC_H_