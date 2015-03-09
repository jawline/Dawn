#include "cpu.h"

unsigned long queryCpuidFeatures() {
	unsigned long input = 0x1;
	unsigned long cpuid_features_result = 0;

	__asm__ volatile("movl %1, %%eax; \n \
		      cpuid; \n \
		      movl %%eax, %0;" : "=r" (cpuid_features_result) : "r" (input));

	return cpuid_features_result;
}

unsigned long cpuidFeatures() {

	if (!cpuidSupported()) { 
		return 0;
	}

	return queryCpuidFeatures();
}
