#include "syscall.h"
#include <stdio.h>
#include <process/process.h>
#include "../scheduler/default/process_scheduler.h"
#include <timers/clock.h>
#include <heap/heap.h>
#include <panic/panic.h>

extern heap_t kernel_heap;

process_t* get_current_process();
extern MEM_LOC calculate_free_frames();

MEM_LOC postbox_location()
{
	return &get_current_process()->m_processPostbox;
}

void postbox_pop_top()
{
	postbox_top(&get_current_process()->m_processPostbox);
}

void postbox_set_bit(unsigned int bit)
{
	get_current_process()->m_postboxFlags = get_current_process()->m_postboxFlags | bit;
}

MEM_LOC syscall_return_current_process()
{
	return get_current_process();
}

char get_key_mapping(unsigned char scancode, unsigned long flags)
{
	return keyboard_chlookup_asci(scancode, flags);
}

MEM_LOC clocks_per_second()
{
	return CLOCKS_PER_SECOND;
}

MEM_LOC system_uptime()
{
	return get_clock_ticks();
}

extern unsigned int PAGE_SIZE;

MEM_LOC get_page_size()
{
	return PAGE_SIZE;
}

MEM_LOC syscall_get_process(unsigned int iter)
{
	return scheduler_return_process(iter);
}

MEM_LOC syscall_get_kernel_heap()
{
	return &kernel_heap;
}

void request_reboot()
{
	kernel_reboot(); //For now, just reboot the system. Its prolly the right thing to do
}

extern page_directory_t* kernel_pagedir;
void kill_current_process()
{
	get_current_process()->m_shouldDestroy = 1;
}

void syscall_printf(const char* Line)
{
	if (get_current_process()->m_pTerminal != 0)
	{
		while (*Line)
		{
			get_current_process()->m_pTerminal->f_putchar(get_current_process()->m_pTerminal, *Line);
			Line++;
		}
	}
}

void syscall_clearscreen()
{
	if (get_current_process()->m_pTerminal != 0)
	{
		get_current_process()->m_pTerminal->f_clear(get_current_process()->m_pTerminal);
	}
}

extern void scheduler_block_me();

unsigned int num_syscalls = 16;

static void *syscalls[16] = {
   &syscall_printf,
   &postbox_location,
   &postbox_pop_top,
   &get_key_mapping,
   &scheduler_block_me,
   &postbox_set_bit,
   &calculate_free_frames,
   &get_page_size,
   &request_reboot,
   &clocks_per_second,
   &system_uptime,
   &syscall_get_process,
   &syscall_return_current_process,
   &kill_current_process,
   &syscall_get_kernel_heap,
   &syscall_clearscreen
};

idt_call_registers_t syscall_handler(idt_call_registers_t regs)
{
   // Firstly, check if the requested syscall number is valid.
   // The syscall number is found in EAX.
   if (regs.eax >= num_syscalls)
       return;

   // Get the required syscall location.
   void *location = syscalls[regs.eax];

   // We don't know how many parameters the function wants, so we just
   // push them all onto the stack in the correct order. The function will
   // use all the parameters it wants, and we can pop them all back off afterwards.
   MEM_LOC ret;

   asm volatile (" \
		   push %1; \
		   push %2; \
		   push %3; \
		   push %4; \
		   push %5; \
		   call %6; \
		   pop %%ebx; \
		   pop %%ebx; \
		   pop %%ebx; \
		   pop %%ebx; \
		   pop %%ebx; \
		" : "=a" (ret) : "r" (regs.edi), "r" (regs.esi), "r" (regs.edx), "r" (regs.ecx), "r" (regs.ebx), "r" (location));

   //Ret the return value of registers_t to ret. Its pushed to eax before the interrupt returns (iret instruction) allowing the process that was executing before the syscall to handle it
   regs.eax = ret;

   return regs;
}

void kernel_initialise_syscalls()
{
	register_interrupt_handler (127, &syscall_handler); //Syscall = Interrupt 127

	return;
}
