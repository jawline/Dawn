#include <timers/clock.h>

unsigned int syscallClocksPerSecond() {
	return CLOCKS_PER_SECOND;
}

unsigned long syscallGetSystemUptime() {
	return getClockTicks();
}

