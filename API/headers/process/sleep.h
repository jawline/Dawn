#ifndef _PROCESS_SLEEP_API_
#define _PROCESS_SLEEP_API_
#include <clock/clock.h>
#include <syscall/syscall.h>

void sleepProcess();
void sleepUnprecise(unsigned long ticks);

#endif //_PROCESS_SLEEP_API_
