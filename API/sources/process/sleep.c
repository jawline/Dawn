#include <process/sleep.h>

DEFN_SYSCALL0(block_process, 5);

//Force the process to sleep
void sleepProcess() {
	syscall_block_process();
}

//Unprecise sleep routine that just waits untill a certain amount of time has passed and gives up processing time untill then
void sleepUnprecise(unsigned long ticks) {
	unsigned long tell = clock() + ticks;
	while (clock() < tell) {
		sleepProcess();
	}
}
