#include "kernel/lib/memory.h"
#include <cstdint>
#include <cstddef>

extern "C" {

#ifdef memcpy
#undef memcpy
#endif
void *memcpy(void *__restrict dest, const void *__restrict src, std::size_t n)
{
    auto pdest = reinterpret_cast<std::uint8_t *>(dest);
    auto psrc = reinterpret_cast<const std::uint8_t *>(src);

    for (std::size_t i = 0; i < n; i++)
        pdest[i] = psrc[i];

    return dest;
}

#ifdef memset
#undef memset
#endif
void *memset(void *s, int c, std::size_t n)
{
    auto p = reinterpret_cast<std::uint8_t *>(s);

    for (std::size_t i = 0; i < n; i++)
        p[i] = static_cast<std::uint8_t>(c);

    return s;
}

#ifdef memmove
#undef memmove
#endif
void *memmove(void *dest, const void *src, std::size_t n) {
    auto pdest = reinterpret_cast<std::uint8_t *>(dest);
    auto psrc = reinterpret_cast<const std::uint8_t *>(src);

    if (reinterpret_cast<std::uintptr_t>(src) > reinterpret_cast<std::uintptr_t>(dest)) {
        for (std::size_t i = 0; i < n; i++)
            pdest[i] = psrc[i];
    } else if (reinterpret_cast<std::uintptr_t>(src)
        < reinterpret_cast<std::uintptr_t>(dest)) {
        for (std::size_t i = n; i > 0; i--)
            pdest[i-1] = psrc[i-1];
    }

    return dest;
}

#ifdef memcmp
#undef memcmp
#endif
int memcmp(const void *s1, const void *s2, size_t n)
{
    auto p1 = reinterpret_cast<const std::uint8_t *>(s1);
    auto p2 = reinterpret_cast<const std::uint8_t *>(s2);

    for (std::size_t i = 0; i < n; i++) {
        if (p1[i] != p2[i])
            return p1[i] < p2[i] ? -1 : 1;
    }

    return 0;
}

} // extern "C"