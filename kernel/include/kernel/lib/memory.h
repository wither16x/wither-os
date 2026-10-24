#ifndef KERNEL_LIB_MEMORY_H_
#define KERNEL_LIB_MEMORY_H_

#include <stddef.h>

void *memcpy(void *restrict dest, const void *restrict src, size_t n);
void *memset(void *s, int c, size_t n);
void *memmove(void *dest, const void *src, size_t n);
int memcmp(const void *s1, const void *s2, size_t n);

/* For optimization purposes: the compiler can optimize builtins better than
* functions. */
#define memcpy __builtin_memcpy
#define memset __builtin_memset
#define memmove __builtin_memmove
#define memcmp __builtin_memcmp

#endif /* KERNEL_LIB_MEMORY_H_ */
