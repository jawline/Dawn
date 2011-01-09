#include <scheduler/process_scheduler.h>
#include <panic/panic.h>
#include <stdlib.h>
#include <common.h>
#include <stack/kstack.h>
#include <debug/debug.h>

struct process_entry_t
{
	process_t* process_pointer;
	int ticks_tell_die;

	struct process_entry_t* next;
};

typedef struct process_entry_t scheduler_proc;

scheduler_proc* list_root = 0;
scheduler_proc* list_current = 0;

void schedulerInitialize(process_t* kp)
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

void schedulerOnTick()
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
void schedulerAdd(process_t* op)
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
void schedulerRemove(process_t* op)
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
			//Is the next process the one that needs to be removed
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
		PANIC("Scheduler trying to remove currently accessed process, this shouldn't happen... DEBUG!!\n");
	}

	//Remove it from the list
	iterator_process->next = iterator_process->next->next;

	//Free it
	free(next);

	DEBUG_PRINT("Removed scheduler entry\n");

	return;
}

//Sleeps the current process on the next tick
void schedulerBlockMe()
{
	if (list_current == 0) return;

	list_current->ticks_tell_die = 0;
}

process_t* getCurrentProcess()
{
	if (list_current == 0) return 0;

	return list_current->process_pointer;
}

int schedulerNumProcesses()
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

void schedulerKillCurrentProcess()
{
	if (list_current == 0) return 0;

	process_t* process = getCurrentProcess();
	process->m_shouldDestroy = 1;

	for (;;)
	{
		schedulerOnTick(); //Fake ticks untill the next process is brought into context. Next time this process is handled by the scheduler it will be destroyed
	}
}

process_t* schedulerReturnProcess(unsigned int iter)
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

void schedulerGlobalMessage(process_message msg, unsigned int bit)
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
