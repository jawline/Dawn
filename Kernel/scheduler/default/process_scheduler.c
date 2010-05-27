#include "process_scheduler.h"

process_t* kernel_proc = 0;
process_t* current_process = 0;

void scheduler_init(process_t* kp)
{
	kernel_proc = kp;
}

void scheduler_on_tick()
{
	if (kernel_proc != 0)
	{
		process_t* back = current_process;
		current_process = kernel_proc;
		switch_process(kernel_proc, kernel_proc);
	}
}
