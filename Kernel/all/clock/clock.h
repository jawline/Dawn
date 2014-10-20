#ifndef _CLOCK_DEF_H_
#define _CLOCK_DEF_H_
#include <clock/rate.h>

#define TICKS_PER_SECOND 1000

/**
 * Clock callback is a function called on every clock tick
 */

typedef void (*clock_callback)();

/**
 * Number of time the clock has ticked since boot
 */

unsigned long getClockTicks();

/**
 * This function is called whenever the process clock
 * ticks, increments the internal clock
 */

void clockHandleTick();

/**
 * Initialise the clock (architecture dependant) and setup callbacks
 */

void initializeSystemClock();

/**
 * Register a callback to be called on tick
 */

void registerClockTickCallback(clock_callback cb);

#endif //_CLOCK_DEF_H_
