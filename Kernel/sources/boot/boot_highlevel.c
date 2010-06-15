// SimpleOS Entry Point
// Main function is the entry point of the kernel.

#include <multiboot.h>
#include <version/kernel_version.h>
#include <common.h>
#include <stdlib.h>

// Declare the page directory and a page table, both 4kb-aligned
uint32 kernelpagedir[1024] __attribute__ ((aligned (4096)));
uint32 lowpagetable[1024] __attribute__ ((aligned (4096)));
 
// This function fills the page directory and the page table,
// then enables paging by putting the address of the page directory
// into the CR3 register and setting the 31st bit into the CR0 one
void init_paging()
{
	// Pointers to the page directory and the page table
	void *kernelpagedirPtr = 0;
	void *lowpagetablePtr = 0;
	int k = 0;
 
	kernelpagedirPtr = (char *)kernelpagedir + 0x40000000;	// Translate the page directory from
								// virtual address to physical address
	lowpagetablePtr = (char *)lowpagetable + 0x40000000;	// Same for the page table
 
	memset(kernelpagedir, 0, sizeof(uint32) * 1024);

	// Counts from 0 to 1023 to...
	for (k = 0; k < 1024; k++)
	{
		lowpagetable[k] = (k * 4096) | 0x3;	// ...map the first 4MB of memory into the page table...
		kernelpagedir[k] = 0;			// ...and clear the page directory entries
	}
 
	// Fills the addresses 0...4MB and 3072MB...3076MB of the page directory
	// with the same page table
 
	kernelpagedir[0] = (unsigned long)lowpagetablePtr | 0x3;
	kernelpagedir[768] = (unsigned long)lowpagetablePtr | 0x3;
 
	// Copies the address of the page directory into the CR3 register and, finally, enables paging!
 
	asm volatile (	"mov %0, %%eax\n"
			"mov %%eax, %%cr3\n"
			"mov %%cr0, %%eax\n"
			"orl $0x80000000, %%eax\n"
			"mov %%eax, %%cr0\n" :: "m" (kernelpagedirPtr));
}

//Main entry point of the Kernel. It is passed the multiboot header by GRUB when the bootloader begins the Kernel execution. (Multiboot header defined in multiboot.h)
int main(struct multiboot* mboot, uint32 stack_ptr /* Pointer to the stack pointer */)
{
    //Install a temporery GDT that maps the first 4MB of code so tha shizzle can be accsizzled
    init_paging();

    //The kernel SHOULLDD now init fine, and think its in the magical land of the higher half

    #if defined(DEBUG_MODE)
    	//Needs the multiboot location to function, 1 signals that the init routines should be noisy and cover the screen in stuff
    	init_kernel(mboot, 1, stack_ptr);
    #else
    	init_kernel(mboot, 0, stack_ptr);
    #endif

    printf("Kernel Name: %s Version: %i.%i.%i\n", KVERSION_CODENAME, KVERSION_MAJOR, KVERSION_MINOR, KVERSION_REVISION);

    //We can has interrupts now...
    enable_interrupts();

    //Run whatever the kernel is designed to do after initialization
    post_init();

    for (;;) {  } 

    return 0xDEADBABA;
}
