#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <limine.h>

__attribute__((used, section(".limine_requests")))
static volatile uint64_t limine_base_revision[] = LIMINE_BASE_REVISION(6);

__attribute__((used, section(".limine_requests")))
static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST_ID,
    .revision = 0
};

__attribute__((used, section(".limine_requests_start")))
static volatile uint64_t limine_requests_start_marker[] = LIMINE_REQUESTS_START_MARKER;

__attribute__((used, section(".limine_requests_end")))
static volatile uint64_t limine_requests_end_marker[] = LIMINE_REQUESTS_END_MARKER;

static void hcf(void) {
    for (;;)
        __asm__ ("hlt");
}

static uint32_t fb_channel(uint8_t value, uint8_t mask_size, uint8_t mask_shift) {
    uint64_t max = ((uint64_t)1 << mask_size) - 1;
    return (uint32_t)((value * max / 255) << mask_shift);
}

static uint32_t fb_pixel(struct limine_framebuffer *fb, uint8_t red, uint8_t green, uint8_t blue) {
    return fb_channel(red, fb->red_mask_size, fb->red_mask_shift)
         | fb_channel(green, fb->green_mask_size, fb->green_mask_shift)
         | fb_channel(blue, fb->blue_mask_size, fb->blue_mask_shift);
}

static void fb_pattern(struct limine_framebuffer *fb) {
    volatile uint32_t *fb_ptr = fb->address;
    for (size_t y = 0; y < fb->height; y++) {
        for (size_t x = 0; x < fb->width; x++) {
            uint8_t nX = x * 255 / fb->width;
            uint8_t nY = y * 255 / fb->height;
            fb_ptr[y * (fb->pitch / 4) + x] = fb_pixel(fb, 0, nY, nX);
        }
    }
}

void kmain(void) {
    if (LIMINE_BASE_REVISION_SUPPORTED(limine_base_revision) == false)
        hcf();

    if (framebuffer_request.response == NULL
    || framebuffer_request.response->framebuffer_count < 1)
        hcf();

    for (uint64_t i = 0; i < framebuffer_request.response->framebuffer_count; i++) {
        struct limine_framebuffer *framebuffer =
            framebuffer_request.response->framebuffers[i];

        if (framebuffer->memory_model != LIMINE_FRAMEBUFFER_RGB || framebuffer->bpp != 32)
            hcf();

        fb_pattern(framebuffer);
    }

    hcf();
}
