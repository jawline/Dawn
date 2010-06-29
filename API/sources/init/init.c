#include <types/memory.h>

extern MEM_LOC initial_esp;
extern MEM_LOC initial_ebp;

int start(int argc, void* argv)
{
	asm volatile("mov %%esp, %0" : "=r"(initial_esp));
	asm volatile("mov %%ebp, %0" : "=r"(initial_ebp));

	main(argc, argv);
}
