#include "syscall.h"
#include <stdio.h>
#include "../scheduler/default/process_scheduler.h"
#include <heap/heap.h>
#include <panic/panic.h>
#include <mm/virt_mm.h>

extern heap_t kernel_heap;

extern process_t* get_current_process();
extern MEM_LOC calculate_free_frames();

extern MEM_LOC postboxLocation();
extern void postboxPopTop();
extern void postboxSetBit(int bit);
extern MEM_LOC syscallReturnCurrentProcess();
extern void syscallKillCurrentProcess();

char get_key_mapping(unsigned char scancode, unsigned long flags)
{
	return keyboard_chlookup_asci(scancode, flags);
}

extern unsigned int syscallClocksPerSecond();
extern unsigned long syscallGetSystemUptime();

extern MEM_LOC syscallGetProcess(unsigned int iter);

MEM_LOC get_page_size()
{
	return PAGE_SIZE;
}

MEM_LOC syscall_get_kernel_heap()
{
	return &kernel_heap;
}

extern void syscallRequestReboot();

extern void syscallPrintf(const char* Line);
extern void syscallClearscreen();
extern void syscallSetFgc(unsigned char fgc);
extern void syscallSetBgc(unsigned char bgc);

extern void scheduler_block_me();

unsigned int num_syscalls = 18;

static void *syscalls[18] = {
   &syscallPrintf,
   &postboxLocation,
   &postboxPopTop,
   &get_key_mapping,
   &scheduler_block_me,
   &postboxSetBit,
   &calculate_free_frames,
   &get_page_size,
   &syscallRequestReboot,
   &syscallClocksPerSecond,
   &syscallGetSystemUptime,
   &syscallGetProcess,
   &syscallReturnCurrentProcess,
   &syscallKillCurrentProcess,
   &syscall_get_kernel_heap,
   &syscallClearscreen,
   &syscallSetFgc,
   &syscallSetBgc
};

//Function: syscallHandler
//Arguments: idt_call_registers_t regs
//Return: idt_call_registers_t with new values of the registers
//Description: Handles a syscall (Via interrupt 127) and returns a new set of registers
idt_call_registers_t syscallHandler(idt_call_registers_t regs)
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

void kernelInitializeSyscalls()
{
	register_interrupt_handler (127, &syscallHandler); //Syscall = Interrupt 127

	return;
}
