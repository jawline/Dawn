#include <process/process.h>
#include <stdlib.h>
#include <panic/panic.h>
#include <mm/virt_mm.h>
#include <debug/debug.h>
#include <stack/kstack.h>
#include <scheduler/scheduler.h>
#include <process/used_list.h>
#include <mm/phys_mm.h>
#include <loaders/executable_loader.h>
#include <interrupts/interrupts.h>
#include <fs/vfs.h>
#include <messages/messages.h>

extern MEM_LOC read_eip();
static process_t* kernel_proc = 0;
static unsigned int next_pid = 0;
extern terminal_t* g_kernelTerminal;

static const int cProcessSwapMagic = 0x12345;

#define PROCESS_HEAP_START 0xA0000000

typedef struct {

	/**
	 * The name of the file to load
	 */
	char* filename;

	/**
	 * From what directory should the search for this file begin
	 */
	fs_node_t* fromWhere;

	/**
	 * Should the application be run in systems software mode (needs access
	 * to privileged instructions) or user mode (Limited access, better control
	 * by OS, safer)
	 */
	int userMode;

} new_process_orders_t;

new_process_orders_t* makeOrders(const char* Where, fs_node_t* fromWhere) {
	new_process_orders_t* createdOrder = malloc(sizeof(new_process_orders_t));
	memset(createdOrder, 0, sizeof(new_process_orders_t));
	createdOrder->filename = malloc(strlen(Where) + 1);
	strcpy(createdOrder->filename, Where);
	createdOrder->fromWhere = fromWhere;
	return createdOrder;
}

void freeOrders(new_process_orders_t* orders) {
	free(orders->filename);
	free(orders);
}

/**
 * Acts as a entry point for new OS processes
 */
void newProcessEntryPoint() {

	process_message msg = postboxTop(&getCurrentProcess()->processPostbox);

	if (msg.ID == LOAD_MESSAGE) {
		int userMode = 0;
		new_process_orders_t* orders =
				(new_process_orders_t*) msg.messageAdditionalData;

		fs_node_t* node = evaluatePath(orders->filename, orders->fromWhere);
		userMode = orders->userMode;
		renameCurrentProcess(orders->filename);
		freeOrders(orders);

		if (!node || !node->parent) {
			DEBUG_PRINT(
					"Unable to evaluate node. process could not load file specified\n");
			schedulerKillCurrentProcess();
		} else {
			setProcessExecutionDirectory(getCurrentProcess(), node->parent);
			loadAndExecuteProgram(node, userMode);
		}
		schedulerKillCurrentProcess();
	} else {
		DEBUG_PRINT("Invalid message. Killing self\n");
		schedulerKillCurrentProcess();
	}

	PANIC("Should never get here");
}

process_t* initializeKernelProcess() {
	disableInterrupts();

	if (kernel_proc != 0) {
		return kernel_proc;
	}

	process_t* ret = (process_t*) malloc(sizeof(process_t));
	memset(ret, 0, sizeof(process_t));
	strcpy(ret->name, "KernelProcess");

	ret->id = 0;

	extern page_directory_t* kernel_pagedir;
	ret->pageDir = kernel_pagedir;
	ret->executionDirectory = init_vfs();
	kernel_proc = ret;
	initializeUsedList(kernel_proc);
	ret->processTerminal = g_kernelTerminal;

	return ret;
}

void freeProcess(process_t* process) {

	//This kills the used list and frees every used page
	while (process->usedListLocation != 0) {
		MEM_LOC top = usedListTop(process);
		freeFrame(top);
		usedListRemove(process, top);
	}

	if (process->usedListRoot != 0) {
		free(process->usedListRoot);
	}

	while (process->processPostbox.first != 0) {
		postboxTop(&process->processPostbox);
	}

	free(process);
}

int kfork() {
	uint32_t esp, ebp;

	disableInterrupts();

	DEBUG_PRINT("Free frames at start %x\n", calculateFreeFrames());

	//Store this for later use
	process_t* parent = getCurrentProcess();

	//Create a process space for the new process and null iyt
	process_t* new_process = malloc(sizeof(process_t));
	memset(new_process, 0, sizeof(process_t));

	//Give it a generic name fo-now
	strcpy(new_process->name, "Forklet");

	new_process->processTerminal = parent->processTerminal;
	initializeUsedList(new_process);

	//Set the processes unique ID
	next_pid++;
	new_process->id = next_pid;

	//Located in virt_mm.c
	extern page_directory_t* current_pagedir;

	//Set the root execution directory
	new_process->executionDirectory = parent->executionDirectory;

	//Copy the page directory
	page_directory_t* newprocesspd = copyPageDir(current_pagedir, new_process);

	//Give it a page directory
	new_process->pageDir = newprocesspd;

	MEM_LOC current_eip = (MEM_LOC) read_eip();

	if (parent->id == getCurrentProcess()->id) {
		__asm__ volatile("mov %%esp, %0" : "=r"(esp));
		__asm__ volatile("mov %%ebp, %0" : "=r"(ebp));
		new_process->esp = esp;
		new_process->ebp = ebp;
		new_process->eip = current_eip;
		schedulerAdd(new_process);
		return 0; //Return 0 - Parent
	} else {
		return 1; //Return 1 - Child
	}
}

int createNewProcess(const char* filename, fs_node_t* where) {
	uint32_t esp, ebp;

	disableInterrupts();

	//Store this for later use
	process_t* parent = schedulerGetProcessFromPid(0);

	//Create a process space for the new process and null it
	process_t* new_process = malloc(sizeof(process_t));
	memset(new_process, 0, sizeof(process_t));

	//Give it a generic name fo-now
	strcpy(new_process->name, "New Process");

	//Setup terminal bindings
	new_process->processTerminal = parent->processTerminal;

	//Initialize the used frames list for the process
	initializeUsedList(new_process);

	//Set the processes unique ID
	next_pid++;
	new_process->id = next_pid;

	//Located in virt_mm.c
	extern page_directory_t* current_pagedir;
	extern page_directory_t* kernel_pagedir;

	//Set the root execution directory
	new_process->executionDirectory = parent->executionDirectory;

	//Copy the page directory
	page_directory_t* newprocesspd = copyPageDir(kernel_pagedir, new_process);

	//Give it a page directory
	new_process->pageDir = newprocesspd;

	MEM_LOC current_eip = (MEM_LOC) newProcessEntryPoint;

	__asm__ volatile("mov %%esp, %0" : "=r"(esp));
	__asm__ volatile("mov %%ebp, %0" : "=r"(ebp));

	new_process->esp = USER_STACK_START;
	new_process->ebp = USER_STACK_START;
	new_process->eip = current_eip;

	process_message InfomaticMessage;
	InfomaticMessage.from_PID = getCurrentProcess()->id;
	InfomaticMessage.ID = LOAD_MESSAGE;
	InfomaticMessage.messageAdditionalData = (MEM_LOC) makeOrders(filename,
			where);

	postboxPush(&new_process->processPostbox, InfomaticMessage);

	schedulerAdd(new_process);

	return 0; //Return 0 - Parent
}

void switchProcess(process_t* from, process_t* to) {

	ASSERT(from && to, "from & to process have to be valid for switchProcess");
	disableInterrupts();

	uint32_t esp, ebp, eip;

	__asm__ volatile("mov %%esp, %0" : "=r"(esp));
	__asm__ volatile("mov %%ebp, %0" : "=r"(ebp));

	eip = read_eip();

	if (eip == cProcessSwapMagic) {
		//If this is triggered then we have just switched tasks so return
		return;
	}

	from->eip = eip;
	from->esp = esp;
	from->ebp = ebp;

	eip = to->eip;
	esp = to->esp;
	ebp = to->ebp;
	page_directory_t* pagedir = to->pageDir;

	extern page_directory_t* current_pagedir;
	current_pagedir = pagedir;

	__asm__ volatile("cli; \
		      mov %1, %%esp; \
		      mov %2, %%ebp; \
		      mov %0, %%ecx; \
		      mov %3, %%cr3; \
		      mov %4, %%eax; \
		      sti; \
		      jmp *%%ecx;" :: "r" (eip), "r" (esp), "r" (ebp), "r" (pagedir), "r" (cProcessSwapMagic));

	return;
}
