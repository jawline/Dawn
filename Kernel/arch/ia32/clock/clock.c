#include <clock/clock.h>
#include <clock/rate.h>
#include <clock/pit.h>
#include <lists/linked.h>

extern linked_list_t* callbackList;
extern unsigned long systemClockTicks;

void initializeSystemClock() {
	callbackList = 0;
	systemClockTicks = 0;
	initializePit(CLOCKS_PER_SECOND);
}
