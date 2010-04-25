#include "cpu.h"
#include <printf.h>
#include "../timers/clock.h"
#define BIT_4 0x8
#define PERIOD CLOCKS_PER_SECOND / 10 /* 100 Ms */

#define rdtscll(val) \
     __asm__ __volatile__ ("rdtsc" : "=A" (val))

uint32 cpuid_getfrequency()
{
	enable_interrupts();
	uint32 lWord_p;
	uint32 hWord_p;

	if (!cpuid_supported()) return 0; //CPUID Not supported
	uint32 features = cpuid_features();
	if (!(features & BIT_4))
	{
		//TSC not supported, cannot calculate frequency
		return 0;
	}

	long long rd1 = 0;
	long long rd2 = 0;

	rdtscll(rd1);
	uint32 clockend = clock() + CLOCKS_PER_SECOND;
	while (clock() < clockend);
	rdtscll(rd2);

	uint32 delta = rd1 - rd2; //Assume the delta can fit in a 32bit unsigned int?

	return delta / 1000;
}
