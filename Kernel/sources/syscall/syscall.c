#include "syscall.h"
#include <stdio.h>
#include <process/process.h>
#include "../scheduler/default/process_scheduler.h"

process_t* get_current_process();

void proc_set_ibuffer(char* m_data, unsigned int len)
{
	set_process_input_buffer(get_current_process(), m_data, len);
}

MEM_LOC proc_get_cp()
{
	return get_current_process()->m_inputCurPosition;
}

void proc_set_cp(unsigned int loc)
{
	get_current_process()->m_inputCurPosition = loc;
}

unsigned int num_syscalls = 4;

static void *syscalls[4] = {
   &printf,
   &proc_set_ibuffer,
   &proc_get_cp,
   &proc_set_cp
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

   regs.eax = ret;

   return regs;
}

void kernel_initialise_syscalls()
{
	register_interrupt_handler (127, &syscall_handler);
	return;
}
