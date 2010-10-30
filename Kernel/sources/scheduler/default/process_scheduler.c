#include "process_scheduler.h"
#include <panic/panic.h>
#include <stdlib.h>
#include <common.h>
#include "../../stack/kstack.h"

struct process_entry_t
{
	process_t* process_pointer;
	int ticks_tell_die;

	struct process_entry_t* next;
};

typedef struct process_entry_t scheduler_proc;

scheduler_proc* list_root = 0;
scheduler_proc* list_current = 0;

process_t* get_current_process();

void initializeScheduler(process_t* kp)
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

void swapToProcess(scheduler_proc* scheduler_entry)
{
	process_t* old_proc = list_current->process_pointer;
	
	setKernelStack(KERNEL_STACK_START);

	//Swap to the next process
	list_current = scheduler_entry;

	process_t* new_proc = list_current->process_pointer;

	//Give me a nano!
	list_current->ticks_tell_die = _STD_NANO_;

	//Swap to the new process
	switch_process(old_proc, new_proc);
}

void scheduler_on_tick()
{
	if (list_root == 0) return;

	if (list_current->ticks_tell_die < 1)
	{
		if (list_current->next->process_pointer->m_shouldDestroy == 1)
		{
			swapToProcess(list_root);
		}
		else
		{
			swapToProcess(list_current->next);
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
				printf("Found process\n");
				//Process
				break;
			}
			else
			{
				iterator_process = iterator_process->next;
			}
		}
	}


	printf("Next set\n");
	//Store the proc
	scheduler_proc* next = iterator_process->next;

	if (list_current == next)
	{
		PANIC("List current = Next. Scheduler cock up\n");
	}

	printf("Removing it from the list of iterators\n");

	//Remove it from the list
	iterator_process->next = iterator_process->next->next;

	printf("Freeing it\n");

	//Free it
	free(next);

	printf("Done\n");

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

int scheduler_num_processes()
{
	if (list_root == 0) return 0;
	
	scheduler_proc* iter = list_root;

	int numTotal = 0;

	while (1)
	{
		numTotal++;

		if (iter->next == list_root)
		{
			break;
		}
		
		iter = iter->next;
	}

	return numTotal;
}

void scheduler_kill_current_process()
{
	if (list_current == 0) return 0;

	process_t* process = get_current_process();

	if (process->m_ID == 0)
	{
		printf("Error will not close system idle\n");
		return;
	}

	printf("Process %i (%s) terminated with return value %i\n", process->m_ID, process->m_Name, process->m_returnValue);

	process->m_shouldDestroy = 1;

	for (;;)
	{
		scheduler_on_tick(); //Fake ticks untill the next process is brought into context. Next time this process is handled by the scheduler it will be sent to /dev/null
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

process_t* schedulerGetProcessFromPid(unsigned int pid)
{

	scheduler_proc* iterator = list_root;
	unsigned int i = 0;

	for (;;)
	{
		//If it is the process I'm loooking for break
		if (iterator->process_pointer->m_ID == pid) break;

		//If its the last entry and not correct then return null
		if (iterator->next == list_root) return 0;

		//Set the iterator to the next entry
		iterator = iterator->next;
	}

	//Return what I got
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
