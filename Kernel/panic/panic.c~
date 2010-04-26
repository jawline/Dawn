#include "panic.h"

void panic(const char *message, const char *file, uint32 line)
{
    // We encountered a massive problem and have to stop.
    asm volatile("cli"); // Disable interrupts.
    char TBuffer[128];
    itoa(line, TBuffer, 10);
    text_mode_write("PANIC(");
    text_mode_write(message);
    text_mode_write(") at ");
    text_mode_write(file);
    text_mode_write(":");
    text_mode_write(TBuffer);
    text_mode_write("\n");
    // Halt by going into an infinite loop.
    for(;;);
}

void panic_assert(const char *file, uint32 line, const char *desc)
{
    // An assertion failed, and we have to panic.
    asm volatile("cli"); // Disable interrupts.
    char TBuffer[128];
    itoa(line, TBuffer, 10);

    text_mode_write("ASSERTION-FAILED(");
    text_mode_write(desc);
    text_mode_write(") at ");
    text_mode_write(file);
    text_mode_write(":");
    text_mode_write(TBuffer);
    text_mode_write("\n");
    // Halt by going into an infinite loop.
    for(;;);
}
