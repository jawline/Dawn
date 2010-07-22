#include <types/memory.h>

MEM_LOC initial_esp = 0;
MEM_LOC initial_ebp = 0;

int exit(int val)
{
	MEM_LOC mebp = initial_ebp;
	MEM_LOC mesp = initial_esp;

	asm volatile("mov %0, %%ebp; \
		      mov %1, %%esp;" :: "r" (mebp), "r" (mesp));

	return val;
}
