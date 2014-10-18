#include <clock/clock.h>
#include <panic/panic.h>
#include <scheduler/scheduler.h>
#include <lists/linked.h>

//a unsigned long is used to store the number of ticks that have occured since boot
unsigned long systemClockTicks;

//Return the number of clock ticks that have occured since boot
unsigned long getClockTicks()
{
	return systemClockTicks;
}

linked_list_t* callbackList;

void clockHandleTick() {

	linked_list_t* iter = callbackList;

	while (iter) {
		((clock_callback)iter->payload)();
		iter = linkedListNext(iter);
	}

	systemClockTicks++;
}

void registerClockTickCallback(clock_callback cb) {
	callbackList = linkedListPrepend(callbackList, cb);
}
