#include <panic/panic.h>

void text_mode_hardwrite(const char*);

//Function: panic
//Arguments, message (What error message to give), file (The filename of the file where the error occured), line (The line no. of where the error occured)
//Return: None
//Description: Panic the kernel, disabling interrupts and looping (calling asm("hlt")) should freeze the processor
void panic(const char *message, const char *file, uint32_t line)
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
    for(;;)
    {
	asm volatile("hlt");
    }
}

void panic_assert(const char *file, uint32_t line, const char *desc)
{
    // An assertion failed, and we have to panic.
    asm volatile("cli"); // Disable interrupts.
    char TBuffer[128];
    itoa(line, TBuffer, 10);

    text_mode_hardwrite("ASSERTION-FAILED(");
    text_mode_hardwrite(desc);
    text_mode_hardwrite(") at  h");
    text_mode_hardwrite(file);
    text_mode_hardwrite(":");
    text_mode_hardwrite(TBuffer);
    text_mode_hardwrite("\n");
    // Halt by going into an infinite loop.
    for(;;)
    {
	asm volatile("hlt");
    }
}