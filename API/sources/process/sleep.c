#include <process/sleep.h>

DEFN_SYSCALL0(block_process, 4);

//Force the process to sleep
void sleep_proc()
{
	syscall_block_process();
}

//Unprecise sleep routine that just waits untill a certain amount of time has passed and gives up processing time untill then
void sleep_uprec(unsigned long ticks)
{
	unsigned long tell = clock() + ticks;

	for (;;)
	{
		if (clock() > tell) break;

		sleep_proc();
	}
}
