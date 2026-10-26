#include "kernel/cpu/gdt.h"

void Gdt::load() const
{
    Gdtr gdtr(sizeof(descriptors_) - 1, reinterpret_cast<u64>(descriptors_.data()));
    __asm__ volatile (
        "lgdt %[gdtr]\n\t"

        "pushq $0x8\n\t"
        "leaq 1f(%%rip), %%rax\n\t"
        "pushq %%rax\n\t"
        "lretq\n"

        "1:\n\t"
        "movw $0x10, %%ax\n\t"
        "movw %%ax, %%ds\n\t"
        "movw %%ax, %%es\n\t"
        "movw %%ax, %%fs\n\t"
        "movw %%ax, %%gs\n\t"
        "movw %%ax, %%ss\n\t"
        :
        : [gdtr] "m"(gdtr)
        : "rax"
    );
}