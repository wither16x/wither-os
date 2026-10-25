#ifndef KERNEL_LIB_MEMORY_H_
#define KERNEL_LIB_MEMORY_H_

#include "kernel/lib/types.h"

extern "C" {

void *memcpy(void *__restrict dest, const void *__restrict src, usize n);
void *memset(void *s, int c, usize n);
void *memmove(void *dest, const void *src, usize n);
int memcmp(const void *s1, const void *s2, usize n);

} // extern "C"

/* For optimization purposes: the compiler can optimize builtins better than
* functions. */
#define memcpy __builtin_memcpy
#define memset __builtin_memset
#define memmove __builtin_memmove
#define memcmp __builtin_memcmp

#endif // KERNEL_LIB_MEMORY_H_