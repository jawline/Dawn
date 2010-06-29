#include "cpu.h"

unsigned long get_cpuid_features()
{
	unsigned long input = 0x1;
	unsigned long cpuid_features_result = 0;

	asm volatile("movl %1, %%eax; \n \
		      cpuid; \n \
		      movl %%eax, %0;" : "=r" (cpuid_features_result) : "r" (input));

	return cpuid_features_result;
}

unsigned long cpuid_features()
{
	if (!cpuid_supported()) return 0;
	return get_cpuid_features();
}
