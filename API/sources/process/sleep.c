#include <process/sleep.h>

DEFN_SYSCALL0(block_process, 5);

void sleepProcess() {
	syscall_block_process();
}

void sleepUnprecise(unsigned long ticks) {
	unsigned long tell = clock() + ticks;
	while (clock() < tell) {
		sleepProcess();
	}
}
