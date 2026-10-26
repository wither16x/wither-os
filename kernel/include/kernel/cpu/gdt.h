#ifndef KERNEL_CPU_GDT_H_
#define KERNEL_CPU_GDT_H_

#include "kernel/lib/types.h"
#include <array>

struct [[gnu::packed]] Gdtr {
    u16 size;
    u64 offset;

    constexpr explicit Gdtr(u16 sz, u64 off)
        : size(sz), offset(off)
    {}
};

class GdtDescriptor {
public:
    constexpr explicit GdtDescriptor(u32 base, u32 limit, u16 flags)
    {
        descriptor_ = limit & 0xf0000;
        descriptor_ |= (flags << 8) & 0xf0ff00;
        descriptor_ |= (base >> 16) & 0xff;
        descriptor_ |= base & 0xff000000;
        descriptor_ <<= 32;
        descriptor_ |= base << 16;
        descriptor_ |= limit & 0xffff;
    }

    constexpr u64 get() const
    {
        return descriptor_;
    }

private:
    u64 descriptor_ = 0;
};

class Gdt {
public:
    static constexpr usize kDescriptorCount = 3;

    using DescriptorTable = std::array<GdtDescriptor, kDescriptorCount>;

    constexpr explicit Gdt(const DescriptorTable &descriptors)
        : descriptors_(descriptors)
    {}

    constexpr auto &descriptors() const
    {
        return descriptors_;
    }

    void load() const;

private:
    DescriptorTable descriptors_;
};

inline constinit Gdt g_gdt(Gdt::DescriptorTable{
    GdtDescriptor(0, 0, 0),
    GdtDescriptor(0, 0xfffff, 0xa09a),
    GdtDescriptor(0, 0xfffff, 0x92)
});

#endif // KERNEL_CPU_GDT_H_