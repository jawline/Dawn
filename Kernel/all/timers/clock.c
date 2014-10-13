#include <timers/clock.h>
#include <panic/panic.h>
#include <scheduler/process_scheduler.h>

//a unsigned long is used to store the number of ticks that have occured since boot
unsigned long systemClockTicks;

//Return the number of clock ticks that have occured since boot
unsigned long getClockTicks()
{
	return systemClockTicks;
}
