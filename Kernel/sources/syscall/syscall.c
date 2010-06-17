#include "syscall.h"
#include <stdio.h>
#include <process/process.h>
#include "../scheduler/default/process_scheduler.h"

process_t* get_current_process();

MEM_LOC postbox_location()
{
	return &get_current_process()->m_processPostbox;
}

void postbox_pop_top()
{
	postbox_top(&get_current_process()->m_processPostbox);
}

char get_key_mapping(unsigned char scancode, unsigned long flags)
{
	return keyboard_chlookup_asci(scancode, flags);
}

extern void scheduler_block_me();

unsigned int num_syscalls = 5;

static void *syscalls[5] = {
   &printf,
   &postbox_location,
   &postbox_pop_top,
   &get_key_mapping,
   &scheduler_block_me,
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
		   call *%6; \
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
