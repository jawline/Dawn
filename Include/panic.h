#ifndef _KERNEL_PANIC_DEF_H_
#define _KERNEL_PANIC_DEF_H_
#include <common.h>

#define PANIC(msg) panic(msg, __FILE__, __LINE__);
#define ASSERT(b) ((b) ? (void)0 : panic_assert(__FILE__, __LINE__, #b))

void panic(const char *message, const char *file, uint32 line);
void panic_assert(const char *file, uint32 line, const char *desc);

#endif
