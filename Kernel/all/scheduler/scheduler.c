#include <scheduler/scheduler.h>
#include <panic/panic.h>
#include <stdlib.h>
#include <common.h>
#include <stack/kstack.h>
#include <debug/debug.h>

struct process_entry_t {
	process_t* process_pointer;
	int ticks_tell_die;
	struct process_entry_t* next;
};

typedef struct process_entry_t scheduler_proc;

scheduler_proc* list_root = 0;
scheduler_proc* list_current = 0;

void swapToProcess(scheduler_proc* scheduler_entry) {

	process_t* old_proc = list_current->process_pointer;
	setKernelStack(KERNEL_STACK_START);

	//Swap to the next process
	list_current = scheduler_entry;
	process_t* new_proc = list_current->process_pointer;
	list_current->ticks_tell_die = _STD_NANO_;
	switchProcess(old_proc, new_proc);
}

void schedulerYield() {
	ASSERT(list_current && list_root,
			"Cannot yield if scheduler has not been initialized");
	if (list_current->next->process_pointer->shouldDestroy == 1) {
		swapToProcess(list_root);
	} else {
		swapToProcess(list_current->next);
	}
}

void schedulerOnTick() {

	if (list_root == 0) {
		return;
	}

	if (!list_current->ticks_tell_die) {
		schedulerYield();
	} else {
		list_current->process_pointer->processingTime++;
		list_current->ticks_tell_die--;
	}
}

//To anybody calling this function, remember to re-enable interrupts where applicable
void schedulerAdd(process_t* op) {

	//Create and set new_process to all 0's
	scheduler_proc* new_process = malloc(sizeof(scheduler_proc));
	memset(new_process, 0, sizeof(scheduler_proc));
	new_process->process_pointer = op;
	unsigned int iterator = 0;
	scheduler_proc* iterator_process = list_root;

	//Loop to find the last entry in the list
	for (;;) {
		if (iterator_process->next == list_root) {
			break;
		} else {
			iterator_process = iterator_process->next;
		}
	}
	iterator_process->next = new_process;
	new_process->next = list_root;
}

//To anybody calling this, remember to re-enable interrupts
void schedulerRemove(process_t* op) {

	//Find the scheduler entry
	scheduler_proc* iterator_process = list_root;

	for (;;) {
		if (iterator_process->next == list_root) {
			return; //Cannot find the right proc
		} else {
			//Is the next process the one that needs to be removed
			if (iterator_process->next->process_pointer == op) {
				//Process
				break;
			} else {
				iterator_process = iterator_process->next;
			}
		}
	}

	//Store the procecess
	scheduler_proc* next = iterator_process->next;
	if (list_current == next) {
		PANIC(
				"Scheduler trying to remove currently accessed process, this shouldn't happen... DEBUG!!\n");
	}

	//Remove it from the list
	iterator_process->next = iterator_process->next->next;
	free(next);
}

process_t* getCurrentProcess() {

	if (!list_current) {
		return 0;
	}

	return list_current->process_pointer;
}

int schedulerNumProcesses() {

	ASSERT(list_root,
			"schedulerNumProcess cannot be run before the scheduler is initialized");

	scheduler_proc* iter = list_root;
	int numTotal = 0;
	while (1) {
		numTotal++;

		if (iter->next == list_root) {
			break;
		}

		iter = iter->next;
	}
	return numTotal;
}

void schedulerKillCurrentProcess() {

	ASSERT(list_current, "Cannot kill current - no executing process");
	process_t* process = getCurrentProcess();
	process->shouldDestroy = 1;
	schedulerYield();
	for (;;) {
	}
}

process_t* schedulerReturnProcess(unsigned int iter) {
	scheduler_proc* iterator = list_root;
	unsigned int i = 0;

	for (i = 0; i < iter; i++) {
		if (iterator->next == list_root)
			return 0;
		iterator = iterator->next;
	}

	return iterator->process_pointer;
}

process_t* schedulerGetProcessFromPid(unsigned int pid) {

	scheduler_proc* iterator = list_root;
	unsigned int i = 0;

	for (;;) {
		//If it is the process I'm loooking for break
		if (iterator->process_pointer->id == pid)
			break;

		//If its the last entry and not correct then return null
		if (iterator->next == list_root)
			return 0;

		//Set the iterator to the next entry
		iterator = iterator->next;
	}

	//Return what I got
	return iterator->process_pointer;
}

void schedulerGlobalMessage(process_message msg, unsigned int bit) {
	scheduler_proc* iter = list_root;

	while (1) {

		//Test if this process wants to hear about this event
		if (iter->process_pointer->postboxFlags & bit == bit) {
			postboxPush(&iter->process_pointer->processPostbox, msg);
		}

		if (iter->next == list_root)
			break;

		iter = iter->next;
	}

}

void schedulerInitialize(process_t* kp) {

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

	registerClockTickCallback(schedulerOnTick);
}
