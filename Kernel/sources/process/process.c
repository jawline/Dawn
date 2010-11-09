#include <stdlib.h>
#include <panic/panic.h>
#include <mm/virt_mm.h>
#include <mm/pagedir.h>
#include <messages/messages.h>
#include <debug/debug.h>
#include "../stack/kstack.h"

static process_t* kernel_proc = 0;
static unsigned int next_pid = 0;
extern terminal_t* g_kernelTerminal;
extern process_t* get_current_process();

typedef struct {

	/**
	 * The name of the file to load
	 */

	char* Filename;

	/**
	 * From what directory should the search for this file begin
	 */
	fs_node_t* fromWhere;

	/**
	 * Should the application be run in systems software mode (needs access to privilidged instructions) or user mode (Limited access, better control by OS, Safer)
	 */
	int user_mode;

} new_process_orders_t;

new_process_orders_t* make_orders(const char* Where, fs_node_t* fromWhere)
{
	new_process_orders_t* nOrders = malloc(sizeof(new_process_orders_t));
	memset(nOrders, 0, sizeof(new_process_orders_t));
	nOrders->Filename = malloc(strlen(Where) + 1);
	strcpy(nOrders->Filename, Where);
	
	nOrders->fromWhere = fromWhere;
}

void free_orders(new_process_orders_t* orders)
{
	free(orders->Filename);
	free(orders);
}

/**
 * @brief Acts as a entry point for new applications
 */
void new_process_entry()
{
	process_message Msg = postbox_top(&get_current_process()->m_processPostbox);

	if (Msg.ID == LOAD_MESSAGE)
	{
		int usrMode = 0;

		DEBUG_PRINT("Valid message. continue to load\n");
		new_process_orders_t* Orders = (new_process_orders_t*) Msg.messageAdditionalData;

		DEBUG_PRINT("Telling system to attempt to run %s\n", Orders->Filename);

		fs_node_t* Node = evaluatePath(Orders->Filename, Orders->fromWhere);

		DEBUG_PRINT("Attempted to evaluate path\n");

		usrMode = Orders->user_mode;

		DEBUG_PRINT("Renaming process\n");
		rename_current_process(Orders->Filename);

		DEBUG_PRINT("User mode toggle checked\n");

		free_orders(Orders);

		DEBUG_PRINT("Orders freed\n");

		if (Node == 0)
		{
			DEBUG_PRINT("Unable to evaluate node. process could not load file specified\n");
			scheduler_kill_current_process();
		} else
		{
			DEBUG_PRINT("Passing to executable loader\n");
			loadAndExecuteElf(Node, usrMode);
		}
	}
	else
	{
		DEBUG_PRINT("Invalid message. Killing self\n");
	}

	for (;;) { }
}

process_t* initializeKernelProcess()
{
	disable_interrupts();
	if (kernel_proc != 0) return kernel_proc;

	process_t* ret = (process_t*) malloc(sizeof(process_t));
	memset(ret, 0, sizeof(process_t));
	strcpy(ret->m_Name, "KernelProcess");

	ret->m_ID = 0;

	extern page_directory_t* kernel_pagedir;
	ret->m_pageDir = kernel_pagedir;

	ret->m_executionDirectory = init_vfs();

	kernel_proc = ret;

	init_used_list(kernel_proc);

	ret->m_pTerminal = g_kernelTerminal;

	return ret;
}

void freeProcess(process_t* process)
{
	//This kills the used list and frees every used page
	while (process->m_usedListLocation != 0)
	{

		MEM_LOC top = used_list_top(process);
		freeFrame(top);
		used_list_remove(process, top);
	}

	if (process->m_usedListRoot != 0)
	{
		free(process->m_usedListRoot);
	}

	//Empty the post box
	while (process->m_processPostbox.first != 0)
	{
		
		postbox_top(&process->m_processPostbox);

	}

	free(process);

	DEBUG_PRINT("A process was freed\n");

}

extern process_t* get_current_process();

int kfork()
{
	uint32 esp, ebp;
	
	disable_interrupts();

	DEBUG_PRINT("Free frames at start %x\n", calculate_free_frames());

	//Store this for later use
	process_t* parent = get_current_process();

	//Create a process space for the new process and null iyt
	process_t* new_process = malloc(sizeof(process_t));
	memset(new_process, 0, sizeof(process_t));

	//Give it a generic name fo-now
	strcpy(new_process->m_Name, "ChildProcess");

	new_process->m_pTerminal = parent->m_pTerminal;
	init_used_list(new_process);

	//Set the processes unique ID
	next_pid++;
	new_process->m_ID = next_pid;

	//Located in virt_mm.c
	extern page_directory_t* current_pagedir;

	//Set the root execution directory
	new_process->m_executionDirectory = parent->m_executionDirectory;

	//Copy the page directory
	page_directory_t* newprocesspd = copyPageDir(current_pagedir, new_process);

	//Give it a page directory
	new_process->m_pageDir = newprocesspd;

	uint32 current_eip = read_eip();

	if (parent->m_ID == get_current_process()->m_ID)
	{
		asm volatile("mov %%esp, %0" : "=r"(esp));
		asm volatile("mov %%ebp, %0" : "=r"(ebp));

		new_process->esp = esp;
		new_process->ebp = ebp;
		new_process->eip = current_eip;

		scheduler_add(new_process);

		return 0; //Return 0 - Parent
	}
	else
	{
		return 1; //Return 1 - Child
	}
}

int createNewProcess(const char* Filename, fs_node_t* Where)
{
	uint32 esp, ebp;
	
	disable_interrupts();

	DEBUG_PRINT("PROCESS: FREE FRAMES AT START 0x%x\n", calculate_free_frames());

	//Store this for later use
	process_t* parent = schedulerGetProcessFromPid(0);

	//Create a process space for the new process and null it
	process_t* new_process = malloc(sizeof(process_t));
	memset(new_process, 0, sizeof(process_t));

	//Give it a generic name fo-now
	strcpy(new_process->m_Name, "New Process");

	//Setup terminal bindings
	new_process->m_pTerminal = parent->m_pTerminal;

	//Initialize the used frames list for the process
	init_used_list(new_process);

	//Set the processes unique ID
	next_pid++;
	new_process->m_ID = next_pid;

	//Located in virt_mm.c
	extern page_directory_t* current_pagedir;
	extern page_directory_t* kernel_pagedir;

	//Set the root execution directory
	new_process->m_executionDirectory = parent->m_executionDirectory;

	DEBUG_PRINT("PROCESS: CREATING PAGE DIR\n");

	//Copy the page directory
	page_directory_t* newprocesspd = copyPageDir(kernel_pagedir, new_process);

	DEBUG_PRINT("PROCESS: PAGE DIR CREATED\n");

	//Give it a page directory
	new_process->m_pageDir = newprocesspd;

	uint32 current_eip = new_process_entry;

	asm volatile("mov %%esp, %0" : "=r"(esp));
	asm volatile("mov %%ebp, %0" : "=r"(ebp));

	new_process->esp = USER_STACK_START;
	new_process->ebp = USER_STACK_START;
	new_process->eip = current_eip;

	process_message InfomaticMessage;
	InfomaticMessage.from_PID = 0;
	InfomaticMessage.ID = LOAD_MESSAGE;
	InfomaticMessage.messageAdditionalData = make_orders(Filename, Where);

	postbox_add(&new_process->m_processPostbox, InfomaticMessage);

	scheduler_add(new_process);

	DEBUG_PRINT("RETURN\n");

	return 0; //Return 0 - Parent
}

void rename_current_process(const char* Str)
{
	set_process_name(get_current_process(), Str);
}

inline void switch_process(process_t* from, process_t* to)
{
	if (!from || !to) return; //Invalid ptrs?
	disable_interrupts();

	uint32 esp, ebp, eip;

	asm volatile("mov %%esp, %0" : "=r"(esp));
	asm volatile("mov %%ebp, %0" : "=r"(ebp));

	eip = read_eip();

	if (eip == 0x12345) //Just switched tasks?
	{
		return; //Return to whence you came!
	}

	from->eip = eip;
	from->esp = esp;
	from->ebp = ebp;

	eip = to->eip;
	esp = to->esp;
	ebp = to->ebp;
	page_directory_t* pagedir = to->m_pageDir;

	extern page_directory_t* current_pagedir;
	current_pagedir = pagedir;

	asm volatile("cli; \
		      mov %1, %%esp; \
		      mov %2, %%ebp; \
		      mov %0, %%ecx; \
		      mov %3, %%cr3; \
		      mov $0x12345, %%eax; \
		      sti; \
		      jmp *%%ecx;" :: "r" (eip), "r" (esp), "r" (ebp), "r" (pagedir));

	return;
}

//Jump to the next process
inline void jump_process(process_t* to)
{
	if (!to) return; //Invalid ptrs?
	disable_interrupts();

	uint32 esp, ebp, eip;

	eip = to->eip;
	esp = to->esp;
	ebp = to->ebp;

	page_directory_t* pagedir = to->m_pageDir;

	extern page_directory_t* current_pagedir;
	current_pagedir = pagedir;

	asm volatile("cli; \
		      mov %1, %%esp; \
		      mov %2, %%ebp; \
		      mov %0, %%ecx; \
		      mov %3, %%cr3; \
		      mov $0x12345, %%eax; \
		      sti; \
		      jmp *%%ecx;" :: "r" (eip), "r" (esp), "r" (ebp), "r" (pagedir));

	return;
}

process_message postbox_top(process_postbox* pb)
{
	if (pb->first == 0)
	{
		process_message ret;
		ret.ID = -1; //-1 = No messages left to read
		return ret;
	}

	postbox_message_entry* message = pb->first;

	if (pb->first->next != 0)
	{
		pb->first = message->next;
	} else
	{
		pb->first = 0;
	}

	process_message ret = message->data;
	free(message);

	return ret;
}

//Get the top message without removing it
process_message postbox_peek(process_postbox* pb)
{
	if (pb->first == 0)
	{
		process_message ret;
		ret.ID = -1; //-1 = No messages left to read
		return ret;
	}

	return pb->first->data;
}

void set_process_name(process_t* proc, const char* Name)
{
	strcpy(proc->m_Name, Name);
}

void postbox_add(process_postbox* pb, process_message msg)
{
	if (pb->first == 0)
	{
		//Create pb->first
		postbox_message_entry* new_entry = malloc(sizeof(postbox_message_entry));
		new_entry->data = msg;
		new_entry->next = 0;

		pb->first = new_entry;
	}
	else
	{
		//Add to the end of the list
		postbox_message_entry* last = pb->first;

		//Find the last entry
		while (1)
		{
			if (last->next == 0)
			{
				break;
			} else
			{
				last = last->next;
			}
		}

		postbox_message_entry* new_entry = malloc(sizeof(postbox_message_entry));
		new_entry->data = msg;
		new_entry->next = 0;

		last->next = new_entry;		
	}
}
