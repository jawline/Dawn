#ifndef _PROCESS_SLEEP_API_
#define _PROCESS_SLEEP_API_
#include <clock/clock.h>
#include <syscall/syscall.h>

/**
 * @ingroup Process Managment
 * 
 * @brief This function allows a program to pass up any remaining processing time it may have been allocated by the scheduler and sleeps it untill the scheduler assigns it more processing time
 * @param None
 * @return None
 */

void sleepProcess();

/**
 * @ingroup Process Managment
 * 
 * @brief This function sleeps untill a number have ticks have passed (Note: Not precise due to the fact that this sleep function may leave the process idled tell past the time given in ticks)
 * @param unsigned long ticks for the amount of time the process wants to sleep
 * @return None
 */

void sleepUnprecise(unsigned long ticks);

#endif //_PROCESS_SLEEP_API_
