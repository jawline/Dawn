#include <process/procfault.h>
#include <panic/panic.h>

extern process_t* get_current_process();

void handleFatalProcessFault(FAULT_ID id, FAULT_MSG msg)
{
	printf("Error, process %i faulted with id %i error message %s. Forced to exit from memory\n", get_current_process()->m_ID, id, msg);

	if (get_current_process()->m_ID == 0 || get_current_process()->m_ID == 1)
	{
		PANIC("FATAL KERNEL FAULT");
	}

	scheduler_kill_current_process();

	for (;;)
	{
		scheduler_on_tick();
	}

	return;
}
