// SimpleOS Entry Point
// Main function is the entry point of the kernel.

#include "multiboot.h"
#include "screen.h"

int main(struct multiboot *mboot_ptr)
{
    text_mode_clearscreen();
    text_mode_write("Hello World");    
    return 0xDEADBABA;
}
