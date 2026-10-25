#include "kernel/cpu/asm.h"
#include "kernel/lib/types.h"
#include <limine.h>

namespace
{
    [[gnu::used, gnu::section(".limine_requests")]]
    volatile u64 limine_base_revision[] = LIMINE_BASE_REVISION(6);

    [[gnu::used, gnu::section(".limine_requests")]]
    volatile limine_framebuffer_request framebuffer_request = {
        .id = LIMINE_FRAMEBUFFER_REQUEST_ID,
        .revision = 0,
        .response = nullptr
    };

    [[gnu::used, gnu::section(".limine_requests_start")]]
    volatile u64 limine_requests_start_marker[] = LIMINE_REQUESTS_START_MARKER;

    [[gnu::used, gnu::section(".limine_requests_end")]]
    volatile u64 limine_requests_end_marker[] = LIMINE_REQUESTS_END_MARKER;

    u32 fb_channel(u8 value, u8 mask_size, u8 mask_shift)
    {
        auto max = (static_cast<u64>(1) << mask_size) - 1;
        return static_cast<u32>((value * max / 255) << mask_shift);
    }

    u32 fb_pixel(limine_framebuffer *fb, u8 red, u8 green, u8 blue)
    {
        return fb_channel(red, fb->red_mask_size, fb->red_mask_shift)
            | fb_channel(green, fb->green_mask_size, fb->green_mask_shift)
            | fb_channel(blue, fb->blue_mask_size, fb->blue_mask_shift);
    }

    void fb_pattern(limine_framebuffer *fb)
    {
        auto fb_ptr = reinterpret_cast<volatile u32 *>(fb->address);
        for (usize y = 0; y < fb->height; y++) {
            for (usize x = 0; x < fb->width; x++) {
                auto n_x = static_cast<u8>(x * 255 / fb->width);
                auto n_y = static_cast<u8>(y * 255 / fb->height);
                fb_ptr[y * (fb->pitch / 4) + x] = fb_pixel(fb, 0, n_y, n_x);
            }
        }
    }
} // anonymous namespace

extern void (*__init_array_start[])();
extern void (*__init_array_end[])();

extern void (*__ctors_start[])();
extern void (*__ctors_end[])();

extern "C"
void kmain()
{
    if (LIMINE_BASE_REVISION_SUPPORTED(limine_base_revision) == false) {
        while (true)
            __asm__ volatile ("hlt");
    }

    for (void (**ctor)() = __init_array_start; ctor < __init_array_end;)
        (*ctor++)();
    for (void (**ctor)() = __ctors_end; ctor > __ctors_start;)
        (*--ctor)();

    if (framebuffer_request.response == nullptr
    or framebuffer_request.response->framebuffer_count < 1) {
        while (true)
            cpu::idle();
    }

    for (u64 i = 0; i < framebuffer_request.response->framebuffer_count; i++) {
        limine_framebuffer *framebuffer = framebuffer_request.response->framebuffers[i];

        if (framebuffer->memory_model != LIMINE_FRAMEBUFFER_RGB or framebuffer->bpp != 32) {
            while (true)
                cpu::idle();
        }

        fb_pattern(framebuffer);
    }

    while (true)
        cpu::idle();
}