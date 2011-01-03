#include <timers/clock.h>
#include <panic/panic.h>
#include <scheduler/process_scheduler.h>
#include "pit.h"

unsigned long clock_ticks;

//When the PIT is triggered call a scheduler tick and add one to the clock ticks (Setup so different timers can be used for the scheduler/clock Etcetera)
void pitCallback()
{
	clock_ticks++;

	schedulerOnTick();
}

void initializeSystemClock()
{
	clock_ticks = 0;
	setPitCallback(pitCallback);
	initializePit(1000);
}

unsigned long getClockTicks()
{
	return clock_ticks;
}
