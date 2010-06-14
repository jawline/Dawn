#include <process/process.h>
#include <stdlib.h>
#include <panic/panic.h>

static process_t* kernel_proc = 0;
static unsigned int next_pid = 1;

#define STACK_LOCATION 0x80000000

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

	return ret;
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

	//Give it a page directory
	new_process->m_pageDir = newprocesspd;

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
		new_process->m_inputBuffer = 0;

		scheduler_add(new_process);	

		return 0; //Return 0 - Parent
	}
	else
	{
		return 1; //Return 1 - Child
	}
}

void set_process_input_buffer(process_t* process,char* m_Data, unsigned int len)
{
	process->m_inputBuffer = m_Data;
	process->m_inputBufferLength = len;
	process->m_inputCurPosition = 0;
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
		      jmp %%ecx;" :: "r" (eip), "r" (esp), "r" (ebp), "r" (pagedir));

	return;
}
