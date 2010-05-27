#include <timers/clock.h>
#include "pit.h"

unsigned long long clock_ticks;

void pit_callback()
{
	clock_ticks++;
	scheduler_on_tick();
}

void initialize_system_clock()
{
	clock_ticks = 0;
	set_pit_callback(pit_callback);
	init_pit(1000);
}

unsigned long long get_clock_ticks()
{
	return clock_ticks;
}
