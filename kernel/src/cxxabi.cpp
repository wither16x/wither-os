#include "kernel/lib/types.h"
#include "kernel/panic.h"

extern "C" {

int __cxa_atexit(void (*)(void *), void *, void *)
{
    return 0;
}

void __cxa_pure_virtual()
{
    panic();
}

void __cxa_deleted_virtual()
{
    panic();
}

void *__dso_handle;

int __cxa_guard_acquire(u64 *guard)
{
    return *reinterpret_cast<u8 *>(guard) == 0;
}

void __cxa_guard_release(u64 *guard)
{
    *reinterpret_cast<u8 *>(guard) = 1;
}

} // extern "C"