#ifndef _PROCESS_SLEEP_API_
#define _PROCESS_SLEEP_API_
#include <clock/clock.h>
#include <syscall/syscall.h>

/**
 * This function allows a program to pass up any remaining processing time 
 * it has been allocated by the scheduler and sleeps it until the scheduler
 * allocates it more time.
 */
void sleepProcess();

/**
 * This function sleeps for at least the number of ticks specified.
 * (Note: It may sleep for longer)
 */
void sleepUnprecise(unsigned long ticks);

#endif //_PROCESS_SLEEP_API_
