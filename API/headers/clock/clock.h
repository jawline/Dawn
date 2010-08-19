#ifndef _KERNEL_CLOCK_API_
#define _KERNEL_CLOCK_API_
#include <syscall/syscall.h>

extern unsigned long clock();
extern unsigned long getClocksPerSecond();

#endif //_KERNEL_CLOCK_API_
