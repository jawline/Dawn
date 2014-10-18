#include <process/procfault.h>
#include <panic/panic.h>
#include <scheduler/scheduler.h>
#include <printf.h>

void handleFatalProcessFault(FAULT_ID id, FAULT_MSG msg) {
	printf("Error, process %i faulted with id %i error message %s. Forced to exit from memory\n",
			getCurrentProcess()->id, id, msg);
	ASSERT(getCurrentProcess()->id > 1, "System Process Error - Fatal kernel fault");
	schedulerKillCurrentProcess();
	PANIC("I should never execute");
}
