#include <syscall/syscall.h>
#include <stdio.h>
#include <scheduler/scheduler.h>
#include <heap/heap.h>
#include <panic/panic.h>
#include <mm/virtual.h>
#include <mm/phys_mm.h>
#include <interrupts/interrupt_handler.h>
#include <input/keyboard.h>
#include <syscall/num.h>

extern unsigned char postboxHasNext();
extern void postboxReadTop(process_message* Message);
extern void postboxPopTop();
extern void postboxSetFlags(uint32_t bit);
extern void syscallKillCurrentProcess();
extern void syscallRequestExit(int returnValue);

extern unsigned char syscallProcessValid(unsigned int iter);
extern unsigned int syscallGetPid(unsigned int iter);
extern unsigned long syscallGetProcessingTime(unsigned int iter);
extern void syscallGetName(char* StrLocation, unsigned int iter);
extern void syscallRequestRunNewProcess(const char* NewProcess);

char getKeyMapping(unsigned char scancode, unsigned long flags) {
	return lookupAsciCharacterFromScancode(scancode, flags);
}

extern unsigned int syscallClocksPerSecond();
extern unsigned long syscallGetSystemUptime();

MEM_LOC get_page_size() {
	return PAGE_SIZE;
}

extern void syscallRequestReboot();

extern void syscallPrint_t(const char* Line);
extern void syscallClearscreen();
extern void syscallSetFgc(unsigned char fgc);
extern void syscallSetBgc(unsigned char bgc);

extern void syscallSetDebugMode(uint8_t Mode);

extern void scheduler_block_me();

void* syscall_callbacks[KERNEL_NUM_SYSCALLS];

//This replaces whatever the syscall at number is specified to call with a new call
void kernelRegisterSyscall(unsigned int number, void* newcall) {
	if (number < KERNEL_NUM_SYSCALLS) {
		syscall_callbacks[number] = newcall;
	}
}

void kernelInitializeSyscalls() {

	kernelRegisterSyscall(0, syscallPrint_t); //Syscall 0 - printf prints to active terminal
	kernelRegisterSyscall(1, postboxReadTop); //Syscall 1 - Copys the top of the postbox to location given
	kernelRegisterSyscall(2, postboxPopTop); //Syscall 2 -Pops the top of the postbox
	kernelRegisterSyscall(3, postboxHasNext); //Syscall 3 - Does the postbox have anything for the process to read?
	kernelRegisterSyscall(4, getKeyMapping); //Syscall 4 - Get the key mapping for a scancode
	kernelRegisterSyscall(5, schedulerYield); //Syscall 5 - Pause the current process forfitting any remaining processing time
	kernelRegisterSyscall(6, postboxSetFlags); //Syscall 6 - Set the process message flags (What it wants to know about)
	kernelRegisterSyscall(7, calculateFreeFrames); //Syscall 7 - Return the number of free frames available
	kernelRegisterSyscall(8, get_page_size); //Syscall 8 - Return the size in bytes of each page
	kernelRegisterSyscall(9, syscallRequestReboot); //Syscall 9 - Ask the kernel to reboot
	kernelRegisterSyscall(10, syscallClocksPerSecond); //Syscall 10 - Get the number of clock cycles that occur every second
	kernelRegisterSyscall(11, syscallGetSystemUptime); //Syscall 11 - Get the systems uptime in clocks
	kernelRegisterSyscall(12, syscallClearscreen); //Syscall 12 - Clear the current terminal
	kernelRegisterSyscall(13, syscallSetFgc); //Syscall 13 - Set the current terminals FGC
	kernelRegisterSyscall(14, syscallSetBgc); //Syscall 14 - Set the current terminals BGC
	kernelRegisterSyscall(15, syscallKillCurrentProcess); //Syscall 15 - Flag the current process to be destroyed
	kernelRegisterSyscall(16, syscallProcessValid); //Syscall 16 - Does the iter given link to a valid process?
	kernelRegisterSyscall(17, syscallGetPid); //Syscall 17 - Get the PID of the process linked to the iterator
	kernelRegisterSyscall(18, syscallGetProcessingTime); //Syscall 18 - Get the processing time of the process
	kernelRegisterSyscall(19, syscallGetName); //Syscall 19 - Get the name of the process linked to the iterator
	kernelRegisterSyscall(20, syscallRequestExit); //Syscall 20 - Request exit of the current process (Supplied argument is used as the return value)
	kernelRegisterSyscall(21, syscallRequestRunNewProcess); //Syscall 21 - Requests the execution of a new application (char* filename supplied)
	kernelRegisterSyscall(22, syscallSetDebugMode); //Syscall 22 - Requests the kernel change the debug mode to on or off
}
