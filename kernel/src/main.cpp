#include <cstdint>
#include <cstddef>
#include <limine.h>

namespace
{
    [[gnu::used, gnu::section(".limine_requests")]]
    volatile std::uint64_t limine_base_revision[] = LIMINE_BASE_REVISION(6);

    [[gnu::used, gnu::section(".limine_requests")]]
    volatile limine_framebuffer_request framebuffer_request = {
        .id = LIMINE_FRAMEBUFFER_REQUEST_ID,
        .revision = 0,
        .response = nullptr
    };

    [[gnu::used, gnu::section(".limine_requests_start")]]
    volatile std::uint64_t limine_requests_start_marker[] = LIMINE_REQUESTS_START_MARKER;

    [[gnu::used, gnu::section(".limine_requests_end")]]
    volatile std::uint64_t limine_requests_end_marker[] = LIMINE_REQUESTS_END_MARKER;

    void hcf()
    {
        for (;;)
            __asm__ ("hlt");
    }

    std::uint32_t fb_channel(std::uint8_t value, std::uint8_t mask_size,
        std::uint8_t mask_shift)
    {
        auto max = (static_cast<std::uint64_t>(1) << mask_size) - 1;
        return static_cast<std::uint32_t>((value * max / 255) << mask_shift);
    }

    std::uint32_t fb_pixel(limine_framebuffer *fb, std::uint8_t red, std::uint8_t green,
        std::uint8_t blue)
    {
        return fb_channel(red, fb->red_mask_size, fb->red_mask_shift)
            | fb_channel(green, fb->green_mask_size, fb->green_mask_shift)
            | fb_channel(blue, fb->blue_mask_size, fb->blue_mask_shift);
    }

    void fb_pattern(limine_framebuffer *fb)
    {
        auto fb_ptr = reinterpret_cast<volatile std::uint32_t *>(fb->address);
        for (std::size_t y = 0; y < fb->height; y++) {
            for (std::size_t x = 0; x < fb->width; x++) {
                auto nX = static_cast<std::uint8_t>(x * 255 / fb->width);
                auto nY = static_cast<std::uint8_t>(y * 255 / fb->height);
                fb_ptr[y * (fb->pitch / 4) + x] = fb_pixel(fb, 0, nY, nX);
            }
        }
    }
} // anonymous namespace

extern "C" {

int __cxa_atexit(void (*)(void *), void *, void *)
{
    return 0;
}

void __cxa_pure_virtual()
{
    hcf();
}

void __cxa_deleted_virtual()
{
    hcf();
}

void *__dso_handle;

int __cxa_guard_acquire(std::uint64_t *guard)
{
    return *reinterpret_cast<std::uint8_t *>(guard) == 0;
}

void __cxa_guard_release(std::uint64_t *guard)
{
    *reinterpret_cast<std::uint8_t *>(guard) = 1;
}

} // extern "C"

extern void (*__init_array_start[])();
extern void (*__init_array_end[])();

extern void (*__ctors_start[])();
extern void (*__ctors_end[])();

extern "C"
void kmain()
{
    if (LIMINE_BASE_REVISION_SUPPORTED(limine_base_revision) == false)
        hcf();

    for (void (**ctor)() = __init_array_start; ctor < __init_array_end;)
        (*ctor++)();
    for (void (**ctor)() = __ctors_end; ctor > __ctors_start;)
        (*--ctor)();

    if (framebuffer_request.response == nullptr
    or framebuffer_request.response->framebuffer_count < 1)
        hcf();

    for (std::uint64_t i = 0; i < framebuffer_request.response->framebuffer_count; i++) {
        limine_framebuffer *framebuffer = framebuffer_request.response->framebuffers[i];

        if (framebuffer->memory_model != LIMINE_FRAMEBUFFER_RGB or framebuffer->bpp != 32)
            hcf();

        fb_pattern(framebuffer);
    }

    hcf();
}