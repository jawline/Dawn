#include <types/memory.h>

extern MEM_LOC initial_esp;
extern MEM_LOC initial_ebp;

extern int main(int argc, void* argv);

int _start(int argc, void* argv)
{
	asm volatile("mov %%esp, %0" : "=r"(initial_esp));
	asm volatile("mov %%ebp, %0" : "=r"(initial_ebp));

	int result = main(argc, argv);

	return result;
}
