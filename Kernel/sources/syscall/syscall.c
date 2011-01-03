#include <syscall/syscall.h>
#include <stdio.h>
#include <scheduler/process_scheduler.h>
#include <heap/heap.h>
#include <panic/panic.h>
#include <mm/virt_mm.h>
#include <mm/phys_mm.h>
#include <interrupts/interrupt_handler.h>
#include <input/keyboard.h>

extern heap_t kernel_heap;

extern process_t* get_current_process();
extern MEM_LOC calculate_free_frames();

extern unsigned char postboxHasNext();
extern void postboxReadTop(process_message* Message);
extern void postboxPopTop();
extern void postboxSetFlags(uint32 bit);
extern void syscallKillCurrentProcess();
extern void syscallRequestExit(int returnValue);

extern unsigned char syscallProcessValid(unsigned int iter);
extern unsigned int syscallGetPid(unsigned int iter);
extern unsigned long syscallGetProcessingTime(unsigned int iter);
extern void syscallGetName(char* StrLocation, unsigned int iter);
extern void syscallRequestRunNewProcess(const char* NewProcess);

char getKeyMapping(unsigned char scancode, unsigned long flags)
{
	return lookupAsciCharacterFromScancode(scancode, flags);
}

extern unsigned int syscallClocksPerSecond();
extern unsigned long syscallGetSystemUptime();

MEM_LOC get_page_size()
{
	return PAGE_SIZE;
}

extern void syscallRequestReboot();

extern void syscallPrint_t(const char* Line);
extern void syscallClearscreen();
extern void syscallSetFgc(unsigned char fgc);
extern void syscallSetBgc(unsigned char bgc);

extern void syscallSetDebugMode(uint8 Mode);

extern void scheduler_block_me();

static const unsigned int num_syscalls = 23;

static void* syscalls[23] = {
   &syscallPrint_t, //Syscall 0 - printf prints to active terminal
   &postboxReadTop, //Syscall 1 - Copys the top of the postbox to location given
   &postboxPopTop, //Syscall 2 -Pops the top of the postbox
   &postboxHasNext, //Syscall 3 - Does the postbox have anything for the process to read?
   &getKeyMapping, //Syscall 4 - Get the key mapping for a scancode
   &schedulerBlockMe, //Syscall 5 - Pause the current process forfitting any remaining processing time
   &postboxSetFlags, //Syscall 6 - Set the process message flags (What it wants to know about)
   &calculateFreeFrames, //Syscall 7 - Return the number of free frames available
   &get_page_size, //Syscall 8 - Return the size in bytes of each page
   &syscallRequestReboot, //Syscall 9 - Ask the kernel to reboot
   &syscallClocksPerSecond, //Syscall 10 - Get the number of clock cycles that occur every second
   &syscallGetSystemUptime, //Syscall 11 - Get the systems uptime in clocks
   &syscallClearscreen, //Syscall 12 - Clear the current terminal
   &syscallSetFgc, //Syscall 13 - Set the current terminals FGC
   &syscallSetBgc, //Syscall 14 - Set the current terminals BGC
   &syscallKillCurrentProcess, //Syscall 15 - Flag the current process to be destroyed
   &syscallProcessValid, //Syscall 16 - Does the iter given link to a valid process?
   &syscallGetPid, //Syscall 17 - Get the PID of the process linked to the iterator
   &syscallGetProcessingTime, //Syscall 18 - Get the processing time of the process
   &syscallGetName, //Syscall 19 - Get the name of the process linked to the iterator
   &syscallRequestExit, //Syscall 20 - Request exit of the current process (Supplied argument is used as the return value)
   &syscallRequestRunNewProcess, //Syscall 21 - Requests the execution of a new application (char* filename supplied)
   &syscallSetDebugMode //Syscall 22 - Requests the kernel change the debug mode to on or off
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
       return regs;



   // Get the required syscall location.
   void *location = syscalls[regs.eax];

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

void kernelInitializeSyscalls()
{
	registerInterruptHandler (127, &syscallHandler); //Syscall = Interrupt 127

	return;
}
