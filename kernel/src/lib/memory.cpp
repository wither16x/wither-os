#include "kernel/lib/memory.h"
#include "kernel/lib/types.h"

extern "C" {

#ifdef memcpy
#undef memcpy
#endif
void *memcpy(void *__restrict dest, const void *__restrict src, usize n)
{
    auto pdest = reinterpret_cast<u8 *>(dest);
    auto psrc = reinterpret_cast<const u8 *>(src);

    for (usize i = 0; i < n; i++)
        pdest[i] = psrc[i];

    return dest;
}

#ifdef memset
#undef memset
#endif
void *memset(void *s, int c, usize n)
{
    auto p = reinterpret_cast<u8 *>(s);

    for (usize i = 0; i < n; i++)
        p[i] = static_cast<u8>(c);

    return s;
}

#ifdef memmove
#undef memmove
#endif
void *memmove(void *dest, const void *src, usize n)
{
    auto pdest = reinterpret_cast<u8 *>(dest);
    auto psrc = reinterpret_cast<const u8 *>(src);

    if (reinterpret_cast<uptr>(src) > reinterpret_cast<uptr>(dest)) {
        for (usize i = 0; i < n; i++)
            pdest[i] = psrc[i];
    } else if (reinterpret_cast<uptr>(src)
        < reinterpret_cast<uptr>(dest)) {
        for (usize i = n; i > 0; i--)
            pdest[i-1] = psrc[i-1];
    }

    return dest;
}

#ifdef memcmp
#undef memcmp
#endif
int memcmp(const void *s1, const void *s2, usize n)
{
    auto p1 = reinterpret_cast<const u8 *>(s1);
    auto p2 = reinterpret_cast<const u8 *>(s2);

    for (usize i = 0; i < n; i++) {
        if (p1[i] != p2[i])
            return p1[i] < p2[i] ? -1 : 1;
    }

    return 0;
}

} // extern "C"