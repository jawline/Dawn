#ifndef _CLOCK_DEF_H_
#define _CLOCK_DEF_H_

#define CLOCKS_PER_SECOND 1000
#define clock get_clock_ticks

//Export initialize_system_clock sets up the correct timer and callbacks
void initializeSystemClock();

unsigned long getClockTicks(); //Number of time the clock has "ticked"

#endif //_CLOCK_DEF_H_
