#ifndef _CLOCK_DEF_H_
#define _CLOCK_DEF_H_
#include <clock/rate.h>

#define CLOCKS_PER_SECOND 1000
#define clock get_clock_ticks

unsigned long getClockTicks(); //Number of time the clock has "ticked" since boot

//Function defined in the architecture specific clock folder - so each architecture can handle its clock initialization differently
void initializeSystemClock();

#endif //_CLOCK_DEF_H_
