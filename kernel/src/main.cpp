#include "kernel/boot/requests.h"
#include "kernel/drivers/serial.h"
#include "kernel/lib/numbers.h"
#include "kernel/lib/print.h"
#include "kernel/panic.h"
#include <string_view>

extern void (*__init_array_start[])();
extern void (*__init_array_end[])();

extern void (*__ctors_start[])();
extern void (*__ctors_end[])();

extern "C" void kmain()
{
    if (not requests::base_revision().supported())
        panic();

    for (void (**ctor)() = __init_array_start; ctor < __init_array_end;)
        (*ctor++)();
    for (void (**ctor)() = __ctors_end; ctor > __ctors_start;)
        (*--ctor)();

    if (serial::init().error() == serial::Error::InitFail)
        ; // TODO: add support for multiple output devices

    std::string_view name = "Wither__";
    char first_letter = name[0];
    unsigned int age = 16;
    print("Hello! My name is {}!\r\n", name);
    print("The first letter of my name is {}.\r\n", first_letter);
    print("I am {} years old, which is 0b{} in binary, 0o{} in octal and 0x{} in hexa.\r\n",
        age, bin(age), oct(age), hex(age));

    panic();
}