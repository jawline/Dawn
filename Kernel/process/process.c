#include <process/process.h>
#include <stdlib.h>

static process_t* kernel_proc = 0;
static uint32 next_pid = 1;

#define STACK_LOCATION 0x80000000

process_t* init_kproc()
{
	disable_interrupts();
	if (kernel_proc != 0) return kernel_proc;

	process_t* ret = (process_t*) malloc(sizeof(process_t));
	strcpy(ret->m_Name, "KernelProcess");
	ret->m_ID = 0;

	extern page_directory_t* kernel_pagedir;
	ret->m_pageDir = kernel_pagedir;

	kernel_proc = ret;
	return ret;
}

int fork()
{
}

void switch_process(process_t* from, process_t* to)
{
	if (!from || !to) return; //Invalid ptrs?

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

	asm volatile(
	"       	     \
	cli;                 \
	mov %0, %%ecx;       \
	mov %3, %%cr3;       \
	mov %2, %%ebp;       \
	mov %1, %%esp;       \
	mov $0x12345, %%eax; \
	sti;                 \
	jmp *%%ecx           "
	: : "r"(eip), "r"(esp), "r"(ebp), "r"(to->m_pageDir));

	return;
}
