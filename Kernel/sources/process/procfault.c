#include <process/procfault.h>

extern process_t* get_current_process();

void handleFatalProcessFault(FAULT_ID id, FAULT_MSG msg)
{
	printf("Error, process %i faulted with id %i error message %s. Forced to exit from memory\n", get_current_process()->m_ID, id, msg);

	scheduler_kill_current_process();

	for (;;)
	{
		scheduler_on_tick();
	}

	return;
}
