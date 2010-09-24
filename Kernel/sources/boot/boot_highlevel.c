// SimpleOS Entry Point
// Main function is the entry point of the kernel.

#include <multiboot.h>
#include <version/kernel_version.h>
#include <common.h>
#include <stdlib.h>

//Main entry point of the Kernel. It is passed the multiboot header by GRUB when the bootloader begins the Kernel execution. (Multiboot header defined in multiboot.h)
int main(struct multiboot* mboot, uint32 stack_ptr /* Pointer to the stack pointer */)
{
    //The kernel SHOULLDD now init fine, and think its in the magical land of the higher half

    #if defined(DEBUG_MODE)
    	//Needs the multiboot location to function, 1 signals that the init routines should be noisy and cover the screen in stuff
    	initializeKernel(mboot, 1, stack_ptr);
    #else
    	initializeKernel(mboot, 0, stack_ptr);
    #endif

    printf("SimpleOS - Kernel initialized\n");

    printf("Kernel Codename: \"%s\" Version: %i.%i.%i\n", KVERSION_CODENAME, KVERSION_MAJOR, KVERSION_MINOR, KVERSION_REVISION);

    //We can has interrupts now...
    enable_interrupts();

    //Run whatever the kernel is designed to do after initialization
    postInitialization();

    for (;;) {  } 

    return 0xDEADBABA;
}
