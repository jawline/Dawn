#include "cpu.h"

extern uint32 get_cpuid_features();

uint32 cpuid_features()
{
	if (!cpuid_supported()) return 0;
	return get_cpuid_features();
}
