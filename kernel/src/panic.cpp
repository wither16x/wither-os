#include "kernel/panic.h"
#include "kernel/cpu/asm.h"

void panic()
{
    while (true)
        cpu::idle();
}