#include <clock/clock.h>

DEFN_SYSCALL0(clocks_per_second, 9);
DEFN_SYSCALL0(get_clock, 10);

unsigned long get_clocks_per_second()
{
	return syscall_clocks_per_second();
}

unsigned long clock()
{
	return syscall_get_clock();
}
