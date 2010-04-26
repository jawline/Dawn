// SimpleOS Entry Point
// Main function is the entry point of the kernel.

#include <multiboot.h>
#include <version/version.h>
#include <common.h>

//Main entry point of the Kernel. It is passed the multiboot header by GRUB when the bootloader begins the Kernel execution. (Multiboot header defined in multiboot.h)
int main(uint32 initial_stackloc, struct multiboot *mboot_ptr)
{

    #if defined(DEBUG_MODE)
    //Needs the multiboot location to function, 1 signals that the init routines should be noisy and cover the screen in stuff
    init_kernel(mboot_ptr, 1, initial_stackloc);
    #else
    init_kernel(mboot_ptr, 0, initial_stackloc);
    #endif

    printf("Simple OS Version %i.%i.%i\n", VERSION_MAJOR, VERSION_MINOR, VERSION_REVISION);

    //We can has interrupts now...
    enable_interrupts();

    //Run whatever the kernel is designed to do after initialization
    post_init();

    for (;;) {  }

    return 0xDEADBABA;
}
