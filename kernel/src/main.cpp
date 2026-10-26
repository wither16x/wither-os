#include "kernel/boot/requests.h"
#include "kernel/cpu/gdt.h"
#include "kernel/drivers/serial.h"
#include "kernel/log.h"
#include "kernel/panic.h"

extern void (*__init_array_start[])();
extern void (*__init_array_end[])();

extern void (*__ctors_start[])();
extern void (*__ctors_end[])();

extern "C" void kmain()
{
    for (void (**ctor)() = __init_array_start; ctor < __init_array_end;)
        (*ctor++)();
    for (void (**ctor)() = __ctors_end; ctor > __ctors_start;)
        (*--ctor)();

    if (serial::init().error() == serial::Error::InitFail)
        ; // TODO: add support for multiple output devices

    serial::write("\033[2J"); // clear the screen

    if (not requests::base_revision().supported())
        panic("unsupported limine base revision");

    g_gdt.load();
    ok("initialized GDT");

    panic("nothing to do");
}