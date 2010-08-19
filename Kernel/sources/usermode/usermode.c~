
//Function: switchToUserMode
//Purpose: To set the current code path to execute in user mode, stopping access to kernel data areas and specific instructions (Stop evil programmers/hackers from screwing up the system)
void switchToUserMode()
{
   asm volatile("  \
     cli; \
     mov $0x23, %ax; \
     mov %ax, %ds; \
     mov %ax, %es; \
     mov %ax, %fs; \
     mov %ax, %gs; \
                   \
     mov %esp, %eax; \
     pushl $0x23; \
     pushl %eax; \
     pushf; \
     pushl $0x1B; \
     push $1f; \
     iret; \
     1: \
     ");
}
