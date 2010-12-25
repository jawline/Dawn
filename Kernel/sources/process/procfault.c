#include <process/procfault.h>
#include <panic/panic.h>
#include <scheduler/process_scheduler.h>
#include <printf.h>

void handleFatalProcessFault(FAULT_ID id, FAULT_MSG msg)
{
	printf("Error, process %i faulted with id %i error message %s. Forced to exit from memory\n", getCurrentProcess()->m_ID, id, msg);

	if (getCurrentProcess()->m_ID == 0 || getCurrentProcess()->m_ID == 1)
	{
		PANIC("FATAL KERNEL FAULT");
	}

	schedulerKillCurrentProcess();

	for (;;)
	{
		schedulerOnTick();
	}

	return;
}
