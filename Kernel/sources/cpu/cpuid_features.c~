#include "cpu.h"

//Function: getCpuidFeatures
//Arguments: None
//Returns: eax register of the CPUID call (Features)
//Description: Calls cpuid and returns the features register
unsigned long getCpuidFeatures()
{
	unsigned long input = 0x1;
	unsigned long cpuid_features_result = 0;

	asm volatile("movl %1, %%eax; \n \
		      cpuid; \n \
		      movl %%eax, %0;" : "=r" (cpuid_features_result) : "r" (input));

	return cpuid_features_result;
}

//Function cpuidFeatures()
//Arguments: None
//Returns: features integer returned from CPUID instruction
//Description: Checks if cpuid is supported, if not return null if true return getCpuidFeatures()
unsigned long cpuidFeatures()
{
	if (!cpuidSupported()) return 0;
	return getCpuidFeatures();
}
