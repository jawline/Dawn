#include <stack/kstack.h>

void switchToUserMode() {

	setKernelStack(KERNEL_STACK_START);

	__asm__ volatile("  \
      cli; \
      mov $0x23, %ax; \
      mov %ax, %ds; \
      mov %ax, %es; \
      mov %ax, %fs; \
      mov %ax, %gs; \
                    \
       \
      mov %esp, %eax; \
      pushl $0x23; \
      pushl %eax; \
      pushf; \
      popl %eax; \
      orl $0x200, %eax; \
      pushl %eax; \
      pushl $0x1B; \
      push $1f; \
      iret; \
    1: \
      ");
}
