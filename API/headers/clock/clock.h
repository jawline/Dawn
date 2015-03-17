#ifndef _KERNEL_CLOCK_API_
#define _KERNEL_CLOCK_API_
#include <syscall/syscall.h>

/**
 * This function returns the number of ticks that have occured since the system booted
 */
extern unsigned long clock();

/**
 * This function returns the number of ticks that occur every second
 */
extern unsigned long getClocksPerSecond();

#endif //_KERNEL_CLOCK_API_
