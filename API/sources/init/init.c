#include <types/memory.h>

extern MEM_LOC initial_esp;
extern MEM_LOC initial_ebp;

extern int main(int argc, void* argv);

int _start(int argc, void* argv)
{
	MEM_LOC esp;
	MEM_LOC ebp;

	asm volatile("mov %%esp, %0" : "=r"(esp));
	asm volatile("mov %%ebp, %0" : "=r"(ebp));

	initial_esp = esp;
	initial_ebp = ebp;

	int result = main(argc, argv);

	return result;
}
