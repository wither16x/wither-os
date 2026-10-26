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
    if (not requests::base_revision().supported())
        panic();

    for (void (**ctor)() = __init_array_start; ctor < __init_array_end;)
        (*ctor++)();
    for (void (**ctor)() = __ctors_end; ctor > __ctors_start;)
        (*--ctor)();

    if (serial::init().error() == serial::Error::InitFail)
        ; // TODO: add support for multiple output devices

    serial::write("\033[2J"); // clear the screen

    g_gdt.load();
    ok("initialized GDT");

    debug("this is a debug message");
    note("just in case you need to know that");
    ok("operation successfull");
    warn("i suggest you do something else");
    err("we can't continue sorry");
    fatal("you fucked up the whole kernel, GGs");

    panic();
}