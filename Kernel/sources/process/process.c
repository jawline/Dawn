#include <process/process.h>
#include <stdlib.h>
#include <panic/panic.h>

static process_t* kernel_proc = 0;
static unsigned int next_pid = 0;
extern terminal_t* g_kernelTerminal;

process_t* init_kproc()
{
	disable_interrupts();
	if (kernel_proc != 0) return kernel_proc;

	process_t* ret = (process_t*) malloc(sizeof(process_t));
	memset(ret, 0, sizeof(process_t));
	strcpy(ret->m_Name, "KernelProcess");

	ret->m_ID = 0;

	extern page_directory_t* kernel_pagedir;
	ret->m_pageDir = kernel_pagedir;

	kernel_proc = ret;

	init_used_list(kernel_proc);

	ret->m_pTerminal = g_kernelTerminal;

	return ret;
}

void free_process(process_t* process)
{
	//No interrupts ploz
	disable_interrupts();

	if (process == kernel_proc) {
		return; //Don't want to get rid of PID 0
	}
	else
	{
		//This kills the used list and frees every used page
		while (process->m_usedListLocation != 0)
		{
			MEM_LOC top = used_list_top(process);
			free_frame(top);
			used_list_remove(process, top);
		}

		//Empty the post box
		while (process->m_processPostbox.first != 0)
		{
			postbox_top(&process->m_processPostbox);
		}

		free(process->m_usedListRoot);

		//TODO: Free page directory

		free(process);
	}

}

extern process_t* get_current_process();

int kfork()
{
	uint32 esp, ebp;
	
	disable_interrupts();

	//Store this for later use
	process_t* parent = get_current_process();

	//Located in virt_mm.c
	extern page_directory_t* current_pagedir;

	//Copy the page directory
	page_directory_t* newprocesspd = copy_page_dir(current_pagedir);

	//Create a process space for the new process and null iyt
	process_t* new_process = malloc(sizeof(process_t));
	memset(new_process, 0, sizeof(process_t));

	//Give it a generic name fo-now
	strcpy(new_process->m_Name, "ChildProcess");

	new_process->m_pTerminal = parent->m_pTerminal;

	//Give it a page directory
	new_process->m_pageDir = newprocesspd;

	init_used_list(new_process);

	//Set the processes unique ID
	next_pid++;
	new_process->m_ID = next_pid;

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

//Kill the process
void kill_process(process_t* p)
{
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
