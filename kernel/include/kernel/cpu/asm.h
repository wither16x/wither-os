#ifndef KERNEL_CPU_ASM_H_
#define KERNEL_CPU_ASM_H_

namespace cpu
{
    inline void idle()
    {
        __asm__ volatile ("hlt");
    }
} // namespace cpu

#endif // KERNEL_CPU_ASM_H_