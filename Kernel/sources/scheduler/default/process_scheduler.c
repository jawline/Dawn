#include "process_scheduler.h"
#include <panic/panic.h>
#include <stdlib.h>
#include <common.h>

struct process_entry_t
{
	process_t* process_pointer;
	int ticks_tell_die;

	struct process_entry_t* next;
};

typedef struct process_entry_t scheduler_proc;

scheduler_proc* list_root = 0;
scheduler_proc* list_current = 0;

void scheduler_init(process_t* kp)
{
	//Create and set new_process to all 0's
	scheduler_proc* new_process = malloc(sizeof(scheduler_proc));
	memset(new_process, 0, sizeof(scheduler_proc));

	//Set its process pointer to the kernels processing path
	new_process->process_pointer = kp;
	
	//Loop backs
	new_process->next = new_process;

	//Set the root and current set of the list to new_process
	list_root = new_process;
	list_current = new_process;
}

void scheduler_on_tick()
{
	if (list_current->ticks_tell_die < 1)
	{
		if (list_current->next->process_pointer->m_shouldDestroy == 1)
		{
			process_t* current = list_current->next->process_pointer;

			scheduler_remove(current);
			free_process(current);
		}
		else
		{
			process_t* proc = list_current->process_pointer;

			//Swap to the next process
			list_current = list_current->next;

			process_t* oproc = list_current->process_pointer;

			//Give me a nano!
			list_current->ticks_tell_die = _STD_NANO_;

			//Swap to the new process
			switch_process(proc, oproc);
		}

	} else
	{

		//increment the counter of the time spent on this process
		list_current->process_pointer->m_processingTime++;

		//One tick closer to being context switched
		list_current->ticks_tell_die--;
	}
}


//To anybody calling this function, remember to re-enable interrupts where applicable
void scheduler_add(process_t* op)
{
	disable_interrupts();

	//Create and set new_process to all 0's
	scheduler_proc* new_process = malloc(sizeof(scheduler_proc));
	memset(new_process, 0, sizeof(scheduler_proc));

	new_process->process_pointer = op;

	unsigned int iterator = 0;

	scheduler_proc* iterator_process = list_root;

	//Loop to find the last entry in the list
	for(;;)
	{
		if (iterator_process->next == list_root)
		{
			break;
		}
		else
		{
			iterator_process = iterator_process->next;
		}
	}

	iterator_process->next = new_process;

	new_process->next = list_root;

}

//To anybody calling this, remember to re-enable interrupts
void scheduler_remove(process_t* op)
{
	disable_interrupts();

	//Find the scheduler entry
	scheduler_proc* iterator_process = list_root;

	for (;;)
	{
		if (iterator_process->next == list_root)
		{
			return; //Cannot find the right proc
		}
		else
		{
			//Is the next process the one that needs to be killed
			if (iterator_process->next->process_pointer == op)
			{
				//Process
				break;
			}
			else
			{
				iterator_process = iterator_process->next;
			}
		}
	}

	//Store the proc
	scheduler_proc* next = iterator_process->next;

	if (list_current == next)
	{
		//If its currently being executed then go ahhhhh
		list_current = next->next;
	}

	//Remove it from the list
	iterator_process->next = iterator_process->next->next;

	//Free it
	free(next);

	return;
}

//Sleeps the current process on the next tick
void scheduler_block_me()
{
	if (list_current == 0) return;

	list_current->ticks_tell_die = 0;
}

process_t* get_current_process()
{
	if (list_current == 0) return 0;

	return list_current->process_pointer;
}

void scheduler_kill_current_process()
{
	if (list_current == 0) return 0;

	get_current_process()->m_shouldDestroy = 1;

	for (;;)
	{
		scheduler_block_me();
	}
}

process_t* scheduler_return_process(unsigned int iter)
{
	scheduler_proc* iterator = list_root;
	unsigned int i = 0;

	for (i = 0; i < iter; i++)
	{
		if (iterator->next == list_root) return 0;
		iterator = iterator->next;
	}

	return iterator->process_pointer;
}

void scheduler_global_message(process_message msg, unsigned int bit)
{
	scheduler_proc* iter = list_root;

	while (1)
	{
		//Test if this process wants to hear about this event
		if (iter->process_pointer->m_postboxFlags & bit == bit)
		{

			postbox_add(&iter->process_pointer->m_processPostbox, msg);

		}

		if (iter->next == list_root) break;

		iter = iter->next;

	}

}
