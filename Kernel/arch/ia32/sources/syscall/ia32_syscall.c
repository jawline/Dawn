/* This file defines the functions that may be architecture specific related to syscalls - these functions include interrupt callbacks */

#include <syscall/syscall.h>
#include <syscall/num.h>
#include <types/memory.h>
#include <printf.h>

extern void* syscall_callbacks[];

//Function: syscallHandler
//Arguments: idt_call_registers_t regs
//Return: idt_call_registers_t with new values of the registers
//Description: Handles a syscall (Via interrupt 127) and returns a new set of registers
idt_call_registers_t syscallHandler(idt_call_registers_t regs)
{
   // Firstly, check if the requested syscall number is valid.
   // The syscall number is found in EAX.
   if (regs.eax >= KERNEL_NUM_SYSCALLS)
       return regs;

   // Get the required syscall location.
   void *location = syscall_callbacks[regs.eax];

   // Don't know how many parameters the function wants, so just
   // push them all onto the stack in the correct order. The function will
   // use all the parameters it wants, pop them all back off afterwards.
   MEM_LOC ret;

   //TODO: I'm not quite sure, the * might break it. testt

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

void kernelInitializeSyscallSystem()
{
	registerInterruptHandler (127, &syscallHandler); //Syscall = Interrupt 127

	return;
}
