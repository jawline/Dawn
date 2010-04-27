#include <panic/panic.h>

void panic(const char *message, const char *file, uint32 line)
{
    // We encountered a massive problem and have to stop.
    asm volatile("cli"); // Disable interrupts.
    char TBuffer[128];
    itoa(line, TBuffer, 10);
    text_mode_hardwrite("PANIC(");
    text_mode_hardwrite(message);
    text_mode_hardwrite(") at ");
    text_mode_hardwrite(file);
    text_mode_hardwrite(":");
    text_mode_hardwrite(TBuffer);
    text_mode_hardwrite("\n");
    // Halt by going into an infinite loop.
    for(;;);
}

void panic_assert(const char *file, uint32 line, const char *desc)
{
    // An assertion failed, and we have to panic.
    asm volatile("cli"); // Disable interrupts.
    char TBuffer[128];
    itoa(line, TBuffer, 10);

    text_mode_hardwrite("ASSERTION-FAILED(");
    text_mode_hardwrite(desc);
    text_mode_hardwrite(") at ");
    text_mode_hardwrite(file);
    text_mode_hardwrite(":");
    text_mode_hardwrite(TBuffer);
    text_mode_hardwrite("\n");
    // Halt by going into an infinite loop.
    for(;;);
}
