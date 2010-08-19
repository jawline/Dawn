#include <clock/clock.h>

DEFN_SYSCALL0(clocks_per_second, 10);
DEFN_SYSCALL0(get_clock, 11);

unsigned long getClocksPerSecond()
{
	return syscall_clocks_per_second();
}

unsigned long clock()
{
	return syscall_get_clock();
}
