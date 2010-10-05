#ifndef _KERNEL_CLOCK_API_
#define _KERNEL_CLOCK_API_
#include <syscall/syscall.h>

/**
 * @ingroup Timing
 * 
 * @brief This function returns the number of ticks taht have occured since the system booted
 * @param None
 * @return unsigned long with the value of the number of system ticks that have occured since the PIT was initialized
 */

extern unsigned long clock();

/**
 * @ingroup Timing
 * 
 * @brief This function returns the number of ticks that occur every second
 * @param None
 * @return unsigned long with the value of the number of ticks that occur per second.
 */

extern unsigned long getClocksPerSecond();

#endif //_KERNEL_CLOCK_API_
