#include "process_scheduler.h"
#include <panic/panic.h>

static process_t* kernel_proc = 0;
static process_t* other_proc = 0;

static process_t* current_proc = 0;
unsigned int tsls = 0;

void scheduler_init(process_t* kp)
{
	kernel_proc = kp;
	current_proc = kp;
}

void scheduler_on_tick()
{
	tsls++;

	if (tsls > 5) {

		if (kernel_proc != 0)
		{
			if (other_proc != 0)
			{
				if (current_proc == other_proc)
				{
					current_proc = kernel_proc;
					switch_process(other_proc, kernel_proc);
				}
				else
				{
					current_proc = other_proc;
					switch_process(kernel_proc, other_proc);
				}
			}
			else
			{
				//OP Not set
				current_proc = kernel_proc;
				switch_process(kernel_proc, kernel_proc);
			}
		}

	}

}

void scheduler_add(process_t* op)
{
	other_proc = op;
}

process_t* get_current_process()
{
	return current_proc;
}
